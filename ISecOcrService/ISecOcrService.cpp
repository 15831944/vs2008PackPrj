// ISecOcrService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "OcrRegco.h"
#include "ISecOcrService.h"
#include "common.h"
#include <fstream>
#include "OcrDBSource.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace dll = boost::dll;
using namespace std;

// 唯一的应用程序对象
CWinApp theApp;
COcrDBSource g_ocrDB;
void onTimer(const boost::system::error_code& e,  boost::asio::deadline_timer* t, boost::shared_ptr<COcrRegco> pOcr);
void checkAndHandle(const RegcoResult& rd, const ImgRecord& ird);
std::string saveResult2File(const RegcoResult& rd, const std::string& sOutFileName);
EN_OcrSourceType getImgType(const std::string& imgURI);
void OnFinished(const RegcoResult& ocrResult, void* pUserData);
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    // 日志初始化
    ILog4zManager& lm = ILog4zManager::getRef();
    lm.start();


	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		LOGE(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
        return nRetCode;
	}

    boost::property_tree::ptree pt;
    try
    {
        boost::property_tree::ini_parser::read_ini(".\\config\\config.ini", pt); 
    }
    catch (const std::exception& e)
    {

        LOGE("配置文件打开失败: " << e.what());

        return false;
    }
    int nDbType = pt.get<int>("OCR.DbType", 0);
    std::string szDBIP =  pt.get<std::string>("OCR.DbIP", "192.168.4.161");
    std::string szDBPort = pt.get<std::string>("OCR.DbPort", "1433");
    std::string szDbName = pt.get<std::string>("OCR.DbName", "iSec_Print");
    std::string szDbUserName = pt.get<std::string>("OCR.DbUser", "sa");
    std::string szdbUserPwd = pt.get<std::string>("OCR.DbPwd", "123456");
    g_ocrDB.m_pAdo->SetConnStr(nDbType,szDBIP.c_str(),szDbName.c_str(),szDbUserName.c_str(),szdbUserPwd.c_str(),szDBPort.c_str());

    const int nOCREngineType = pt.get<int>("OCR.OCREngine", 0);
    int nOCRPrivilLevel = pt.get<int>("OCR.OCRPrivil", 0);
    if(nOCRPrivilLevel != en_OcrPrivilgeAdvanced)
        nOCRPrivilLevel = en_OcrPrivilgeNormal;
    const int nDllLogStart = pt.get<int>("OCR.DllLog", 0);

    boost::shared_ptr<COcrRegco> pOcr;
    string dllName = "";
    try
    {
        if (0 == nOCREngineType)
            dllName = "ISecOcrForTess.dll";
        else
        {
            LOGE("无效的 OCR 引擎类型!");
            return -1;
        }


        boost::filesystem::path lib_path(".");          // argv[1] contains path to directory with our plugin library
        std::cout << "Loading the plugin: "<< dllName << std::endl;

        // 导入插件中的变量
        pOcr = dll::import<COcrRegco>(lib_path / dllName, "ocr_engine", dll::load_mode::append_decorations);
        
        pOcr->EnableLog(nDllLogStart == 1); // 开启ocr dll 内部日志
        if (!pOcr->Init(1000))
        {
            LOGE("OCR 初始化失败!");
            return 1;
        }
        // 该设置应该从配置文件读取
        pOcr->SetPrivilLevel((EN_OcrPrivilgeLevel)nOCRPrivilLevel);
    }
    catch (std::exception& e)
    {
        LOGE("OCR 插件加载失败, 请确保 " << dllName << "在当前目录: " << e.what());
        return -1;
    }

    boost::asio::io_service io;  
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(XAB::OcrPeriod));
    t.async_wait(boost::bind(onTimer, boost::asio::placeholders::error, &t, pOcr));
    io.run();

    // 释放资源
    pOcr->Release();

	return nRetCode;
}

void onTimer(const boost::system::error_code& e,  boost::asio::deadline_timer* t, boost::shared_ptr<COcrRegco> pOcr)
{
    static boost::filesystem::path tempPath = boost::filesystem::temp_directory_path();// (XAB::OutRootDir);


    if (!g_ocrDB.checkAndConnect())
    {
        t->expires_from_now(boost::posix_time::seconds(5)); // 5 秒后重连
        t->async_wait(boost::bind(onTimer,boost::asio::placeholders::error,t, pOcr));
        return;
    }
    
    
    std::vector<std::string> keyWords;
    g_ocrDB.getKeyWords(keyWords);
    char** ppKWD = new char*[keyWords.size()];
    for (int i = 0; i < keyWords.size(); ++i)
        ppKWD[i] = (char*)keyWords[i].data();
    pOcr->SetKeywords(ppKWD, keyWords.size());


    /*
        检索所有已打印文档
    */
    std::queue<ImgRecord> quImgRecord;
    g_ocrDB.getImagesURI(quImgRecord);

    
    std::string imgPath;
    RegcoResult res;
    while (!quImgRecord.empty())
    {
        const ImgRecord imgRd = quImgRecord.front();
        quImgRecord.pop();
        imgPath = imgRd.fileURI;
        const int tmpPos = imgPath.find_last_of('\\');
        const std::string fileName = imgPath.substr(tmpPos + 1, -1);
        const std::string tempFileURI = tempPath.string() + "\\" + fileName;
        LOGD("待识别文件: " << fileName);

        const int dotPos = imgPath.find_last_of(".");
        if (imgPath.substr(dotPos + 1, -1) == "gz")
        {

            // copy *.gz to temp dir
            try
            {
                boost::filesystem::path tempFilePath(tempFileURI);
                boost::filesystem::path fileURIPath(imgPath);
                if(!boost::filesystem::exists(fileURIPath))  //判断文件存在性     
                {
                    LOGE("文件不存在: " << imgPath);
                    continue;
                }
                if (boost::filesystem::exists(tempFilePath))
                {
                    // 如果目标文件存在, 则删除
                    boost::filesystem::remove(tempFilePath);
                }

                // 如果目标文件已经存在会导致异常
                boost::filesystem::copy_file(fileURIPath, tempFilePath);
            }
            catch (const std::exception& e)
            {
                LOGE("copy " << imgPath << " 文件时候错误: " << e.what());
                continue;
            }



            if (file_uncompress(const_cast<char*>(tempFileURI.c_str())) < 0)
            {
                LOGE("文件解压失败, 文件: " << imgPath);
                continue;
            }

            const int dotPosOfTmpFile = tempFileURI.find_last_of(".");
            imgPath = tempFileURI.substr(0, dotPosOfTmpFile);
        }

        // 开始识别
        if (!pOcr->RegcoFile(imgPath.data(), OnFinished, (void*)&imgRd, false, getImgType(imgPath)))
        {
            LOGE("识别失败, 开始识别下一个图片");
            continue;
        }
    }


    LOGD("定时器继续...");
    // 继续定时器
    t->expires_from_now(boost::posix_time::seconds(XAB::OcrPeriod));
    //t->expires_at(t->expires_at() + boost::posix_time::seconds(XAB::OcrPeriod));  
    t->async_wait(boost::bind(onTimer,boost::asio::placeholders::error,t, pOcr));
}

void OnFinished(const RegcoResult& ocrResult, void* pUserData)
{
    ImgRecord* pRd = static_cast<ImgRecord*>(pUserData);
    if (NULL == pRd)
    {
        LOGE("USER DATA error");
        return ;
    }

    // 保存到文本文件
    // 文件名根据唯一标示QueueNo来
    std::string txtFileURI = saveResult2File(ocrResult, pRd->oriId + ".txt");

    g_ocrDB.insert2LogOcr(*pRd, txtFileURI, ocrResult.isHit, ocrResult.pageNo, ocrResult.keyword);
}

std::string saveResult2File(const RegcoResult& rd, const std::string& sOutFileName)
{
    /*
        将转换后的文档内容, 存储到指定目录下txt文档中
    */

    std::string fileURI;
    try
    {
        boost::filesystem::path outPath = boost::filesystem::current_path();// (XAB::OutRootDir);
        outPath.append(XAB::OutRootDir);

        if(!boost::filesystem::exists(outPath))  //判断文件存在性     
        {
            //目录不存在;     
            boost::filesystem::create_directory(outPath);  //目录不存在，创建   
        }

        // 注意: path.string()  返还的是 XAB::OutRootDir 字符串
        fileURI = outPath.string() + sOutFileName;
    }
    catch (const std::exception& e)
    {
        LOGE("文件夹创建失败: " << e.what());
        return "";
    }
    

    std::ofstream ofs(fileURI.c_str(), std::ios::trunc);
    if (!ofs)
    {
        LOGE("文件创建失败: " << fileURI);
        return "";
    }

    for (int i = 0; i < rd.pageCount; ++i)
    {
        ofs << "第 " << i + 1 << " 页" << endl;
        ofs << rd.pages[i];
        ofs << endl << endl;
    }
    
    return fileURI;
}


EN_OcrSourceType getImgType(const std::string& imgURI)
{
    int dotPos =  imgURI.find_last_of(".");
    if (dotPos < 0)
        return EN_OcrSourceType::en_OcrSourceUnknown;

    std::string strSuffix = imgURI.substr(dotPos + 1, -1);
    // 转小写
    std::transform(strSuffix.begin(), strSuffix.end(), strSuffix.begin(), std::tolower);

    if ("pdf" == strSuffix)
        return EN_OcrSourceType::en_OcrSourcePdf;
    else if ("tiff" == strSuffix)
        return EN_OcrSourceType::en_OcrSourceTif;
    else if ("jpg" == strSuffix)
        return EN_OcrSourceType::en_OcrSourceJpg;
    else if ("png" == strSuffix)
        return EN_OcrSourceType::en_OcrSourcePng;
    else
        return EN_OcrSourceType::en_OcrSourceUnknown;
    
}