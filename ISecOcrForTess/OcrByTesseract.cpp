#include "TextAndImgExtractor.h"
#include "OcrByTesseract.h"
#include <codecvt>
#include <cstdlib>
#include "util/log4z.h"
#include "util/TextHelper.h"
#include "TiffImage.h"
#include "GsHelper.h"
#include <regex>
#include <stdlib.h>     /* at_quick_exit, quick_exit, EXIT_SUCCESS, EXIT_FAILURE */

using namespace std;
using namespace cv;
using namespace zsummer::log4z;
using namespace PoDoFo;
// #define MY_DEBUG


COcrByTesseract ocr_engine;

COcrByTesseract::COcrByTesseract() noexcept
    : COcrRegco(), m_thresh(-1), m_lookUpTable(1, 256, CV_8U)
    , m_strTmpPath(std::getenv("temp"))
    , m_privilgeLevel(en_OcrPrivilgeNormal)
{
    PdfError::EnableDebug(false);

    const double gamma_ = 3;
    uchar* p = m_lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
}


COcrByTesseract::~COcrByTesseract() noexcept
{
}

void COcrByTesseract::SetPrivilLevel(EN_OcrPrivilgeLevel privilgeLevel) noexcept
{
    m_privilgeLevel = privilgeLevel;
}
void COcrByTesseract::EnableLog(bool val) noexcept
{
    ILog4zManager& lm = ILog4zManager::getRef();
    lm.enableLogger(LOG4Z_MAIN_LOGGER_ID, val); 
    lm.start();
}

bool COcrByTesseract::Init(unsigned dwTimeOutMS)  noexcept
{
    CGsHelper& helper = CGsHelper::getInstance();
    if (!helper.isLoadOk())
    {
        LOGD("加载gsdll.dll");
        // 加载当前目录的 dll
        if (!helper.loadDll("gsdll32.dll"))
        {
            LOGE("加载 gsdll32.dll 失败, 该dll 缺失会导致pdf转图片失败!");
            //std::exit(EXIT_FAILURE); // 退出并清理 static 对象, 终止前先调用经由 atexit() 注册的函数
            //std::quick_exit(EXIT_FAILURE); // 退出不清理任何对象, 退出前调用 at_quick_exit 注册的函数
        }
    }
    if (m_bInit)
        return true;

    CMyClock clk;
    clk.clockBeg();

    m_bInit = false;
    std::string strlang = "eng";
    const std::string langPath(".\\tessdata"); // 加载当前的语言路径
    LOGI("初始化引擎语言: " << strlang);
    if (m_apiEng.Init(langPath.c_str(), strlang.c_str(), tesseract::OEM_DEFAULT))//OEM_TESSERACT_ONLY OEM_DEFAULT OEM_CUBE_ONLY
    {
        LOGE("初始化" << strlang << " OCR 引擎失败!");
        return false;
    }
    float timeLen = clk.clockEnd();
    LOGD("初始化" << strlang << " OCR 引擎耗时: " << std::move(timeLen));

    clk.clockBeg();
    strlang = "chi_sim";
    LOGI("初始化引擎语言: " << strlang);
    if (m_apiChi.Init(langPath.c_str(), strlang.c_str(), tesseract::OEM_DEFAULT))//OEM_TESSERACT_ONLY OEM_DEFAULT OEM_CUBE_ONLY
    {
        LOGE("初始化" << strlang << " OCR 引擎失败! ");
        return false;
    }
    timeLen = clk.clockEnd();
    LOGD("初始化" << strlang << " OCR 引擎耗时: " << std::move(timeLen));

    m_bInit = true;
    return m_bInit;
}

void COcrByTesseract::SetKeywords(char* keyWords[], int keyWordCount) noexcept
{
    m_keywords.clear();
    string sss = "keywords: ";
    for (int i = 0; i < keyWordCount; ++i)
    {
        sss += string(keyWords[i]) + " ";
        m_keywords.push_back(keyWords[i]);
    }
    LOGD(sss.data());
}

void COcrByTesseract::Release() noexcept
{
    if (!m_bInit)
        return;
    m_apiEng.End();
    m_apiChi.End();

    m_bInit = false;
}

bool COcrByTesseract::RegcoFile(const char* strSrcFilePath, FNC_OnFinished callback, void* pUserData, bool bQuik
    , EN_OcrSourceType enOcrSourceType) noexcept
{
    LOGI("识别开始...");
    if (nullptr == callback)
    {
        LOGD("请设置回调函数!");
        return false;
    }

    cv::Mat resultImg;
    RegcoResult res;
    std::string hitKeyword;

    vector<string> pageList;
    bool bOk = false;
    if (tag_EN_OcrSourceType::en_OcrSourcePdf == enOcrSourceType)
    {
        // 取得 页码--文本map
        CTextAndImgExtractor extractor;
        try
        {
            extractor.Init(strSrcFilePath);

            std::string strTmp;
            int pgCnt = extractor.getPagesCount();
            for (int pgIndex = 0; pgIndex < pgCnt; ++pgIndex)
            {
                LOGI("正在识 pdf 别第 " << pgIndex + 1 << " 页");
                std::string strCurPageContent;

                auto page = extractor.getPage(pgIndex + 1);
                strCurPageContent = page.strText;

                if (!res.isHit)
                {// 只进行一次中标检索操作
                    for (const auto& keyword : m_keywords)
                    {
                        const int keywordLen = keyword.length();
                        const size_t pos = strCurPageContent.find(keyword);
                        if (string::npos == pos)
                            continue;
                        else
                        {
                            LOGI("pdf 文本中标keyword: " << keyword << ", 位置: " << pos);
                            res.pageNo = pgIndex + 1;
                            hitKeyword = keyword;
                            res.isHit = true;
                            bOk = true;
                            break;
                        }
                    }
                    if (res.isHit)
                    {
                        if (m_privilgeLevel == en_OcrPrivilgeAdvanced)
                        {
                            // 把当前页转成图片
                            CGsHelper& helper = CGsHelper::getInstance();
                            const string outImgPath = m_strTmpPath + "/out.jpg";
                            if (helper.isLoadOk() && !helper.Convert2Img(strSrcFilePath, outImgPath, pgIndex + 1, pgIndex + 1))
                            {
                                LOGE("pdf 转图片失败");
                                break;
                            }
                            resultImg = imread(outImgPath);
                            bOk = regcoMat(resultImg, strTmp, false);
                            if (!bOk)
                                break;
                            // 再次进行orc
                            checkAndHandle(resultImg, hitKeyword, pgIndex + 1);
                            strCurPageContent = strTmp;

                            pageList.push_back(strCurPageContent);
                            // 是否继续后续页的识别
                            if (bQuik)
                                break;
                            else
                                continue;
                        }
                        else
                        {
                            // 是否继续后续页的识别
                            if (bQuik)
                            {
                                pageList.push_back(strCurPageContent);
                                break;
                            }
                        }
                    }
                }            

                // 图片
                for (int imgIndex = 0; imgIndex < page.imgURIs.size(); ++imgIndex)
                {
                    if (page.imgURIs[imgIndex].empty())
                        continue;
                    LOGI("正在识 pdf 别第 " << pgIndex + 1 << " 页, 第 " << imgIndex + 1 << " 张图片...");

                    Mat srcImage = imread(page.imgURIs[imgIndex]);
                    bOk = regcoMat(srcImage, strTmp);
                    if (!bOk)
                        break;
                    if (!res.isHit)
                    {
                        res.isHit = checkAndHandle(srcImage, hitKeyword, pgIndex + 1);
                        if (res.isHit)
                        {
                            LOGI("pdf 图片中标keyword: " << hitKeyword);
                            srcImage.copyTo(resultImg);
                            res.pageNo = pgIndex + 1;
                        }
                    }
                    strCurPageContent += " " + strTmp;
                    // 是否继续当前页的下一张图片识别
                    if (res.isHit && bQuik)
                        break;
                }
                pageList.push_back(strCurPageContent);
                
                // 是否继续后续页的识别
                if (res.isHit && bQuik)
                    break;
            }
        }
        catch (PdfError & e)
        {
            LOGE("Error: An error " << e.GetError() << " ocurred during processing the pdf file. msg: " << e.what());
            fprintf(stderr, "Error: An error %i ocurred during processing the pdf file.\n", e.GetError());
            return false;
        }
    }
    else if (EN_OcrSourceType::en_OcrSourceTif == enOcrSourceType)
    {
        std::string outStr;
        CMultiPageImageBase& imagePage = CTiffImage();
        if (!imagePage.loadFromFile(strSrcFilePath))
        {
            LOGE("图片加载失败!");
            return false;
        }
        const int pageCount = imagePage.getPageCount();
        for (int i = 0; i < pageCount; ++i)
        {
            LOGD("正在识别 tif 第 " << i + 1 << " 张图片...");

            Mat srcImage = imagePage.getMat(i);
            bOk = regcoMat(srcImage, outStr);
            if (!bOk)
                break;
            pageList.push_back(outStr);

            if (!res.isHit)
            {
                res.isHit = checkAndHandle(srcImage, hitKeyword, i + 1);
                if (res.isHit)
                {
                    LOGI("图片中标keyword: " << hitKeyword);
                    srcImage.copyTo(resultImg);
                    res.pageNo = i + 1;
                }
            }
            // 是否继续后续页的识别
            if (res.isHit && bQuik)
                break;
        }
    }
    else
    {
        Mat srcImage = imread(strSrcFilePath);

        std::string outStr;
        bOk = regcoMat(srcImage, outStr);
        if (bOk)
        {
            pageList.push_back(outStr);

            res.isHit = checkAndHandle(srcImage, hitKeyword, 1);
            if (res.isHit)
            {
                LOGI("图片中标keyword: " << hitKeyword);
                srcImage.copyTo(resultImg);
                res.pageNo = 1;
            }
        }
    }

    
    if (bOk)
    {
        makePages(pageList, res);

        vector<uchar> imgBuff;//buffer for coding 
        if (res.isHit)
        {
            res.keyword = hitKeyword.data();

            if (m_privilgeLevel == en_OcrPrivilgeAdvanced)
            {// 有权限, 才会填充图片数据, 传回高亮显示的图片
                vector<int> param = vector<int>(2);
                param[0] = CV_IMWRITE_JPEG_QUALITY;
                param[1] = 95;//default(95) 0-100 
                cv::imencode(".jpg", resultImg, imgBuff, param);

                res.pImgData = imgBuff.data();
                res.imgLen = imgBuff.size();
            }
        }
        callback(res, pUserData);
        releaseResult(res);
    }

    LOGI("识别结束");
    return bOk;
}

void COcrByTesseract::makePages(std::vector<std::string>& pageList, RegcoResult& result) noexcept
{
    result.pageCount = pageList.size();
    result.pages = new char*[result.pageCount];
    std::string strTmp;
    for (int i = 0; i < result.pageCount; ++i)
    {
        strTmp = pageList[i];
        result.pages[i] = new char[strTmp.size() + 1];
        std::strcpy(result.pages[i], strTmp.data());
    }
}

void COcrByTesseract::releaseResult(RegcoResult& result) noexcept
{
    for (int i = 0; i < result.pageCount; ++i)
        delete[] result.pages[i];
    delete[] result.pages;
}

bool COcrByTesseract::RegcoMem(const unsigned char *pSourceFileMem, unsigned dwSourceFileSize, FNC_OnFinished callback, void* pUserData, bool bQuik
    , EN_OcrSourceType enOcrSourceType) noexcept
{
    LOGI("识别开始...");
    if (nullptr == callback)
    {
        LOGD("请设置回调函数!");
        return false;
    }

    std::vector<uchar> data(pSourceFileMem, pSourceFileMem + dwSourceFileSize);
    Mat srcImage = imdecode(data, CV_LOAD_IMAGE_COLOR);

    std::vector<std::string> pageList;
    std::string outStr;
    bool bOk = regcoMat(srcImage, outStr);
    pageList.push_back(outStr);

    if (bOk)
    {
        RegcoResult res;
        makePages(pageList, res);

        vector<uchar> imgBuff;//buffer for coding 
        std::string hitKeyword;
        res.isHit = checkAndHandle(srcImage, hitKeyword, 1);
        res.pageNo = 1;
        res.pageCount = 1;
        if (res.isHit)
        {
            res.keyword = hitKeyword.data();

            //(1) jpeg compression 
            vector<int> param = vector<int>(2);
            param[0] = CV_IMWRITE_JPEG_QUALITY;
            param[1] = 95;//default(95) 0-100 
            cv::imencode(".jpg", srcImage, imgBuff, param);

            res.pImgData = imgBuff.data();
            res.imgLen = imgBuff.size();
        }
        callback(res, pUserData);
        releaseResult(res);
    }

    LOGI("识别结束");
    return bOk;
}

bool COcrByTesseract::regcoMat(const cv::Mat& srcImage, std::string& outStr, bool canRotate) noexcept
{
    if (!m_bInit)
        return false;

    LOGI("bengin...");
    CMyClock myClock;

    
    if (srcImage.empty())
    {
        LOGE("打开图片失败!");
        return false;
    }
#ifdef MY_DEBUG
    namedWindow("原图", WINDOW_AUTOSIZE);
    imshow("原图", srcImage);
#endif

    // 1> 灰度化
    Mat outImage;
    cvtColor(srcImage, outImage, CV_BGR2GRAY);
#ifdef MY_DEBUG
    namedWindow("灰度化", WINDOW_AUTOSIZE);
    imshow("灰度化", outImage);
#endif

    float timeLen;
    if (canRotate && m_privilgeLevel == en_OcrPrivilgeAdvanced)
    {
        myClock.clockBeg();
        adjustImageOrient(outImage);
        timeLen = myClock.clockEnd();
        LOGD("检测文本方向 耗时: " << timeLen << " 秒");
#ifdef MY_DEBUG
        namedWindow("旋转后", WINDOW_AUTOSIZE);
        imshow("旋转后", outImage);
#endif
    }

    improveImg(outImage);


#ifdef MY_DEBUG
    waitKey();
#endif
    // 识别
    std::string strResult = "";
    //char* pBuffer;

    LOGI("开始识别...");
    myClock.clockBeg();
    m_apiChi.SetImage(outImage.data, outImage.cols, outImage.rows, outImage.channels(), outImage.step);
    m_apiChi.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO_ONLY);
    
    
    cv::Rect rtTmp;
    MyOCRWord wdTmp;
    // 开始识别
    m_apiChi.Recognize(0);

    // 传回带格式的文本
    char* pBuffer = m_apiChi.GetUTF8Text();
    strResult = pBuffer;
    delete[] pBuffer;
    strResult = XAB::CTextHelper::UTF2GBK(strResult);
    outStr = strResult;

    LOGD("识别的文本: \n" << outStr);
    timeLen = myClock.clockEnd();
    LOGD("识别耗时: " << timeLen << " 秒");

    // 记录每个字符的位置信息, 用来在图片上进行高亮显示
    tesseract::ResultIterator* ri = m_apiChi.GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
    string sss;
    if (ri != 0) 
    {
        MyOCRChar chTmp;
        m_charList.clear();
        do 
        {
            const char* chr = ri->GetUTF8Text(level);
            if (chr)
            {// 如果图片是倾斜的, 该指针为空
                const float conf = ri->Confidence(level);
                int x1, y1, x2, y2;
                ri->BoundingBox(level, &x1, &y1, &x2, &y2);
                //wdTmp.word = word;
                // 注意: 英文单词之间应该以空格区分开, 而中文的word不需要空格
                if (std::isalpha(chr[0], m_loc))
                    chTmp.chr = string(chr);
                else
                    chTmp.chr = XAB::CTextHelper::UTF2GBK(chr);
                chTmp.rect.x = x1;
                chTmp.rect.y = y1;
                chTmp.rect.width = x2 - x1;
                chTmp.rect.height = y2 - y1;
                m_charList.push_back(chTmp);

                delete[] chr;
            }
        } while (ri->Next(level));
    }

    LOGI("end...");

    return true;
}

bool COcrByTesseract::checkAndHandle(cv::Mat& srcImage, std::string& hitKeyword, int pageNo)
{
    if (m_charList.empty())
    {
        LOGD("字符列表为空!");
        return false;
    }

    LOGD("中标检测begin");
    string symbContent = ""; // 没有空白字符的字符串
    for (const auto& chrStr : m_charList)
        symbContent += chrStr.chr;
    LOGD("symbContent = " << symbContent);


    bool bFind = false;
    size_t pos;
    size_t totalPos;
    int index;
    string strKeywordNoBlank;
    vector<int> indexList;
    for (const auto& keyword : m_keywords)
    {
        // 去掉keyword 中的所有空格
        static std::regex pattern("[[:blank:]]*");
        // 首先去掉 keyword 中的所有空白字符
        strKeywordNoBlank = std::regex_replace(keyword, pattern, "");
        LOGD("keyword no blank: " << strKeywordNoBlank);

        // 先查找到位置
        pos = symbContent.find(keyword);
        if (string::npos == pos)
        {
            continue;
        }
        else
        {
            bFind = true;
            hitKeyword = keyword;
            LOGD("中标keyword: " << keyword << ", 位置: " << pos);
            break;
        }        
    }
    if (bFind && m_privilgeLevel == en_OcrPrivilgeAdvanced)
    {
        const int keywordLen = strKeywordNoBlank.length();
        // 然后再定位到具体字符元素
        totalPos = 0;
        index = 0;
        for (const auto& e : m_charList)
        {
            totalPos += e.chr.length();
            if (totalPos > pos)
            {// 找到位置
                break;
            }
            ++index;
        }
        do
        {
            indexList.push_back(index);
                
            if (totalPos < pos + keywordLen)
                totalPos += m_charList[++index].chr.length();
            else
                break;

        } while (true);

        LOGD("rect count: " << indexList.size());
        vector<cv::Rect> rtList;
        // 合并同行矩形
        index = indexList[0];
        cv::Rect rtTmp = m_charList[index].rect;
        if (indexList.size() > 1)
        {
            for (int i = 1; i < indexList.size(); ++i)
            {
                index = indexList[i];
                if (isSameLine(rtTmp, m_charList[index].rect))
                    rtTmp = mergeRect(rtTmp, m_charList[index].rect);
                else
                {
                    rtList.push_back(rtTmp);
                    rtTmp = m_charList[index].rect;
                }
            }
        }            
        rtList.push_back(rtTmp);
            

        // 绘制矩形
        for (const auto& rt : rtList)
            cv::rectangle(srcImage, rt, Scalar(0, 0, 255));
                
        const string ss = "page number: " + std::to_string(pageNo);
        cv::putText(srcImage, ss, Point(0, srcImage.rows - 10), HersheyFonts::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255)); // 绘制页码
    }
    LOGD("中标检测end");

    return bFind;
}

bool COcrByTesseract::isSameLine(const cv::Rect& box1, const cv::Rect& box2)
{
    if (box1.y + box1.height < box2.y || box1.y > box2.y + box2.height)
        return false;
    else
        return true;
}
cv::Rect COcrByTesseract::mergeRect(const cv::Rect& box1, const cv::Rect& box2)
{
    Rect rtTmp;
    if (box1.x < box2.x)
        rtTmp.x = box1.x;
    else
        rtTmp.x = box2.x;

    if (box1.y < box2.y)
        rtTmp.y = box1.y;
    else
        rtTmp.y = box2.y;

    if (box1.x + box1.width > box2.x + box2.width)
        rtTmp.width = box1.x + box1.width - rtTmp.x;
    else
        rtTmp.width = box2.x + box2.width - rtTmp.x;

    if (box1.y + box1.height > box2.y + box2.height)
        rtTmp.height = box1.y + box1.height - rtTmp.y;
    else
        rtTmp.height = box2.y + box2.height - rtTmp.y;

    return rtTmp;
}

void COcrByTesseract::removeRt(std::string& srcStr)
{
    static std::regex pattern("[\n|\r]");
    // 首先去掉所有回车
    srcStr = std::regex_replace(srcStr, pattern, "");
}

void COcrByTesseract::adjustImageOrient(cv::Mat& srcImage)
{
    if (!m_bInit)
        return;

// eng, chi_sim
    //对于 620 * 1119 分辨率的图片, 白底黑字纯中文文本:
    //如果检查的语言选择 eng, 则耗时2秒检查完毕, 可信度 : 1.41327;
    //如果选 chi_sim, 则耗时28 秒左右秒, 可信度: 46.2038
    //    如果选 eng+chi_sim, 则耗时5 秒左右, 可信度 : 1.41327
    //    如果选 chi_sim+eng, 则耗时28 秒左右, 可信度: 46.2038
    //    运行环境 : win7 虚拟机, 双核, 2G内存
    // 1> 先修正图片, 旋转角度小于90度
    double degree = CalcDegree(srcImage);
    LOGD("degree:" << degree);
    if (std::abs(degree) - 0.1 > 0)
    {
        myRotate(srcImage, -degree);
#ifdef MY_DEBUG
        imshow("矫正后图片", srcImage);
#endif
    }


    // 2> 再获得页面整体的旋转角, 该角度是90度倍数
    m_apiEng.InitForAnalysePage();
    m_apiEng.SetPageSegMode(tesseract::PageSegMode::PSM_OSD_ONLY);
    m_apiEng.SetImage(srcImage.data, srcImage.cols, srcImage.rows, srcImage.channels(), srcImage.step);
    int orient_deg;
    float orient_conf;
    const char* script_name;
    float script_conf;
    if (m_apiEng.DetectOrientationScript(&orient_deg, &orient_conf, &script_name, &script_conf))
    {
        /*
        只支持旋转角为90度的倍数
        confidence (15.0 is reasonably confident)
        如果图片是正的, 则输出: deg: 0, confidence: 3.76825, script_name: Latin, script_conf: 5.33333
        如果顺时针旋转90度, 输出: deg: 90, confidence: 2.96749, script_name: Latin, script_conf: 5.33333
        180度: deg: 180, confidence: 3.46679, script_name: Latin, script_conf: 5.33333
        270度: deg: 270, confidence: 3.01704, script_name: Latin, script_conf: 6.2963
        */
        ;
    }
    else
    {
        orient_deg = 0;
    }
    LOGD("degree: " << orient_deg << ", confidence: " << orient_conf << ", script_name: " << script_name << ", script_conf: " << script_conf);
    if (orient_deg > 0.1)
        myRotate(srcImage, -orient_deg);
}

// 通过霍夫变换计算角度, 返回顺时针角度
// 1> 只取所有找出的直线中, 角度相似(相差小与1度)且最多的线段,
// 2> 计算的角度不是90 度倍数, 因为如果是90 度倍数, 说明图片是不需要旋转
double COcrByTesseract::CalcDegree(const cv::Mat &srcImage)
{
    Mat dstImage;
    Canny(srcImage, dstImage, 50, 200, 3);

    //通过霍夫变换检测直线
    vector<Vec2f> lines;
    HoughLines(dstImage, lines, 1, CV_PI / 180, 300, 0, 0);//第5个参数就是阈值，阈值越大，检测精度越高

    //由于图像不同，阈值不好设定，因为阈值设定过高导致无法检测直线，阈值过低直线太多，速度很慢
    //所以根据阈值由大到小设置了三个阈值，如果经过大量试验后，可以固定一个适合的阈值。
    if (!lines.size())
    {
        HoughLines(dstImage, lines, 1, CV_PI / 180, 200, 0, 0);
    }

    if (!lines.size())
    {
        HoughLines(dstImage, lines, 1, CV_PI / 180, 150, 0, 0);
    }
    if (!lines.size())
    {
        LOGE("没有检测到直线！");
        return 0;
    }

    LOGD("line size = " << lines.size());
    //Vec4f  0: 线段长, 1: 弧度, 2: 角度, 3: 与其他线段角度相似的个数
    list<Vec4f> lineList(lines.size());
    auto pLine1 = lineList.begin();
    auto pLine2 = lines.cbegin();
    for (; pLine1 != lineList.end(); ++pLine1, ++pLine2)
    {
        (*pLine1)[0] = (*pLine2)[0];
        (*pLine1)[1] = (*pLine2)[1];
        (*pLine1)[2] = DegreeTrans((*pLine2)[1]);;
        (*pLine1)[3] = 0;
    }

    // 1> 找出角度相差较小的, 并且数量最多的线段
    int maxCount = 0;
    for (auto& line : lineList)
    {
        for (const auto& lineTmp : lineList)
        {
            if (std::abs(line[2] - lineTmp[2]) - 1 < 0)
                ++line[3];
        }
        if (line[3] > maxCount)
            maxCount = line[3];
    }
    // 2> 过滤出相似个数为 maxCount 的线段
    for (auto it = lineList.begin(); it != lineList.end();)
    {
        if ((*it)[3] == maxCount)
            ++it;
        else
            it = lineList.erase(it);
    }
    LOGD("最多相似角的线段: line size = " << lineList.size() << ", maxCount = " << maxCount);

    // 3> 转换弧度为角度, 并且去掉所有 90度 倍数的line
    for (auto it = lineList.begin(); it != lineList.end();)
    {
        const float mod = std::fmod((*it)[2], 90);
        if (std::abs(mod) - 0.1 < 0)
            it = lineList.erase(it);
        else
            ++it;
    }
    LOGD("去掉90度倍数的线段: line size = " << lineList.size());


    float sum = 0;
    //依次画出每条线段, 并计算平均角度
    Point pt1, pt2;
    for (const auto item : lineList)
    {
        const float theta = item[1];
        sum += item[2];
#ifdef MY_DEBUG
        const float rho = item[0];
        const double a = cos(theta), b = sin(theta);
        const double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dstImage, pt1, pt2, Scalar(255), 1, LINE_AA); //Scalar函数用于调节线段颜色
        imshow("直线探测效果图", dstImage);
#endif
    }
    float average = 0;
    if (lineList.size() > 0)
        average = sum / lineList.size(); //对所有角度求平均，这样做旋转效果会更好 lines.size()

    LOGD("line size: " << lineList.size());
    if (std::abs(average) - 0.1 > 0)
        return std::round(average * 10) / 10 - 90;
    else
        return 0;
}

// 度数转换
double COcrByTesseract::DegreeTrans(double theta)
{
    const double res = theta / CV_PI * 180;
    return res;
}

// 顺时针旋转
void COcrByTesseract::myRotate(cv::Mat& srcImage, double degree) noexcept
{
    degree = -degree;
    const double angle = degree  * CV_PI / 180.; // 弧度
    const double a = sin(angle), b = cos(angle);
    const int width = srcImage.cols;
    const int height = srcImage.rows;
    const int width_rotate = int(height * fabs(a) + width * fabs(b));
    const int height_rotate = int(width * fabs(a) + height * fabs(b));
    //旋转数组map
    // [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
    // [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

    cv::Mat rotMat(2, 3, CV_32F);
    cv::Point center = cv::Point(srcImage.cols / 2, srcImage.rows / 2);
    double scale = 1;
    // 通过上面的旋转细节信息求得旋转矩阵
    rotMat = getRotationMatrix2D(center, degree, scale);

    // 平移到矩形区中心中心
    rotMat.ptr<double>(0)[2] += (width_rotate - width) / 2;
    rotMat.ptr<double>(1)[2] += (height_rotate - height) / 2;

    cv::bitwise_not(srcImage, srcImage);
    cv::Mat img_rotate;

    //对图像做仿射变换
    const double angle_less90 = std::fmod(degree, 90);
    const double angle_large = degree - angle_less90;
    if (std::abs(angle_large) - 0.1 > 0)
        warpAffine(srcImage, img_rotate, rotMat, cv::Size(width_rotate, height_rotate));
    else if (std::abs(angle_less90) - 0.1 > 0)
        warpAffine(srcImage, img_rotate, rotMat, cv::Size(width_rotate, height_rotate), INTER_CUBIC); // 
    else
    {
        cv::bitwise_not(srcImage, srcImage);
        return;
    }

    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = img_rotate.begin<uchar>();
    cv::Mat_<uchar>::iterator end = img_rotate.end<uchar>();
    for (; it != end; ++it)
        if (*it)
            points.push_back(it.pos());
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
    cv::Rect rt = box.boundingRect();
    cv::getRectSubPix(img_rotate, rt.size(), box.center, srcImage);

    //srcImage = img_rotate;
    cv::bitwise_not(srcImage, srcImage);
}

void COcrByTesseract::improveImg(cv::Mat& srcImg) noexcept
{
    gammaCorrection(srcImg, srcImg);

    // 2> 二值化
    //const int srcChannel = srcImg.channels();
    //int iTmp;
    //float thresh = 0;
    //unsigned int nCount = 0;
    //for (int row = 0; row < srcImg.rows; ++row)
    //{
    //    const uchar* srcPtr = srcImg.ptr<uchar>(row);
    //    for (int col = 0; col < srcImg.cols; ++col)
    //    {
    //        if (srcChannel == 3)
    //        {
    //            const int b = srcPtr[col * srcChannel];
    //            const int g = srcPtr[col * srcChannel + 1];
    //            const int r = srcPtr[col * srcChannel + 2];

    //            iTmp = (b + g + r) / 3.0;
    //        }
    //        else if (srcChannel == 1)
    //        {
    //            iTmp = srcPtr[col * srcChannel];
    //        }

    //        // 计算阈值
    //        if (iTmp > 0 && iTmp < 255)
    //        {
    //            // method 1:
    //            // thresh = 0.1 * thresh + 0.9 * iTmp; // 求加权

    //            // method 2:
    //            thresh += iTmp;
    //            ++nCount;
    //        }
    //    }
    //}
    //thresh = thresh / nCount;
    //if (m_thresh != -1)
    //    thresh = m_thresh;
    //LOGI("二值化阈值: " << thresh);
    //threshold(srcImg, srcImg, thresh, 255, THRESH_BINARY);

    // method 2: 
    threshold(srcImg, srcImg, 0, 255, THRESH_OTSU | THRESH_BINARY);
#ifdef MY_DEBUG
    namedWindow("二值化", WINDOW_AUTOSIZE);
    imshow("二值化", srcImg);
#endif
}

void COcrByTesseract::gammaCorrection(const cv::Mat &img, cv::Mat& dst)
{
    //![changing-contrast-brightness-gamma-correction]
    dst = img.clone();
    LUT(img, m_lookUpTable, dst); // http://docs.opencv.org/master/d2/de8/group__core__array.html#gab55b8d062b7f5587720ede032d34156f
}

void COcrByTesseract::setThresh(float thresh)  noexcept
{
    m_thresh = thresh;
}

void CMyClock::clockBeg() noexcept
{
    m_tmpTime  = chrono::system_clock::now();
}

float CMyClock::clockEnd() noexcept
{
    const auto timePoint2 = chrono::system_clock::now();
    const auto tickets = timePoint2 - m_tmpTime;
    const auto timeLen = std::chrono::duration_cast<std::chrono::milliseconds>(tickets);

    return timeLen.count() / 1000.0;
}
