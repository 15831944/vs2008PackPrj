#include "StdAfx.h"
#include "OcrDBSource.h"
#include <cstdlib>
//#include <map>

COcrDBSource::COcrDBSource() : m_pAdo(new CAdo)
{
}

COcrDBSource::~COcrDBSource(void)
{
}

bool COcrDBSource::checkAndConnect()
{
    if (!m_pAdo->IsConnected())
    {
        if (m_pAdo->Connect())
        {
            LOGI("连接sqlsever成功");
            return true;
        }
        else
        {
            LOGE("连接sqlsever失败");
            return false;
        }
    }

    return true;
}

void COcrDBSource::getImagesURI(std::queue<ImgRecord>& quImgRecord)
{
    /*
        【select DocName,DirPath,JobPath
        from t_log_SubmitJob as A
        inner join t_log_PrintedJob as B on A.ID=B.JobId】
        【远程 192.168.2.110 数据库本地账户登录,iSec_Print】
    */
    ImgRecord ir;
    CStringArray sArr;
    long nRowCount, nColCount, lIdx; 

    CString sql;
    CString cstrTmp;
    std::vector<OcrType> types = getSrcTypes();
    for (int i = 0; i < types.size(); ++i)
    {
        sql = "";
        switch (types[i])
        {
        case OcrType::Copy:
            LOGE("复印留底暂不支持!");
            break;
        case OcrType::Fax:
            LOGE("传真暂不支持!");
            break;
        case OcrType::Print:
            sql = "select OriId = m.QueueNO \n"             // 0
                  "     ,  OnDataTime = m.PrintedTime \n"   // 1
                  "     , UserName = m.AccountName \n"      // 2
                  "     , b.DocName \n"                     // 3
                  "     , b.DirPath \n"                     // 4
                  "     , b.JobPath \n"                     // 5
                  "     , SubmitAddr = b.IP \n"             // 6
                  "     , PrintAddr = c.ip \n"              // 7
                  "      from t_log_printedjob as m \n"
                  "      inner join t_log_SubmitJob as b on m.JobId = b.ID \n"
                  "      inner join t_c_Equipment as c on m.DepId = c.ID \n"
                  "  where not exists(select 0 from t_log_ocr as a where a.ori_type = 1 and m.QueueNO = a.ori_id and a.IsValid = 1)";

            break;
        case OcrType::Scan:
            LOGE("扫描暂不支持!");
            break;
        default:
            LOGE("位置打印源类型!");
            break;
        }

        // 查询数据库
        sArr.RemoveAll();
        if (FALSE == m_pAdo->GetRecordSet(sql, sArr, nRowCount, nColCount))
        {
            LOGE("sql执行失败: " << sql.GetBuffer(0));
            continue;
        }
        for (int j = 0; j < nRowCount; ++j)
        {
            lIdx = nColCount * j;
            sArr.GetAt(lIdx + 0);

            ir.ocrType = types[i];

            cstrTmp = sArr.GetAt(lIdx + 0);
            ir.oriId = cstrTmp.GetBuffer(0);

            cstrTmp = sArr.GetAt(lIdx + 1);
            ir.onDataTime = cstrTmp.GetBuffer(0);

            cstrTmp = sArr.GetAt(lIdx + 2);
            ir.user = cstrTmp.GetBuffer(0);

            cstrTmp = sArr.GetAt(lIdx + 3);
            ir.title = cstrTmp.GetBuffer(0);

            cstrTmp = sArr.GetAt(lIdx + 4) + "\\" + sArr.GetAt(lIdx + 5);
            ir.fileURI = cstrTmp.GetBuffer(0);

            cstrTmp = sArr.GetAt(lIdx + 6);
            ir.submitAddr = cstrTmp.GetBuffer(0);

            cstrTmp = sArr.GetAt(lIdx + 7);
            ir.printAddr = cstrTmp.GetBuffer(0);
            
            
            quImgRecord.push(ir);
        }
    }
}

void COcrDBSource::getKeyWords(std::vector<std::string>& keyWords)
{
    CString sql = "select KeyWords from t_c_OcrKeyWords";
    CStringArray sArr;
    long nRowCount, nColCount, lIdx;
    if (FALSE == m_pAdo->GetRecordSet(sql, sArr, nRowCount, nColCount))
    {
        LOGE("sql 执行失败: " << sql.GetBuffer(0));
        return;
    }

    CString cstrTmp;
    for (int i = 0; i < nRowCount; ++i)
    {
        lIdx = nColCount * i;
        cstrTmp = sArr.GetAt(lIdx + 0);

        keyWords.push_back(cstrTmp.GetBuffer(0));
    }
}

void COcrDBSource::insert2LogOcr(const ImgRecord& ird, const std::string& txtPath, bool bFind, int pageNo, const std::string& keyWord)
{
    std::string checkWords(boost::lexical_cast<std::string>(pageNo) + "," + keyWord);
    // 插入 t_log_ocr
    LOGI("t_log_ocr:  txt 路径: " << txtPath << "; "<< (bFind ? "中标" : "没中标") << "; 中标的页数及具体字符串: " << checkWords);
    CString sql;
    sql.Format("insert into t_log_ocr(ori_type, ori_id, ocr_txt_path, IsCheck, CheckWords, IsValid) \n"
               "     select ori_type = %d, ori_id = '%s', ocr_txt_path = '%s', IsCheck = %d, CheckWords = '%s', IsValid = %d"
               , ird.ocrType, ird.oriId.c_str(), txtPath.c_str(), bFind ? 1 : 0, checkWords.c_str(), 1);
    if (FALSE == m_pAdo->ExecuteSQL(sql))
    {
        LOGE("sql 执行失败: " << sql.GetBuffer(0));
        return;
    }

    if (!bFind)
        return;


    // 取得上面插入的id
    sql = "select MAX(id) from t_log_ocr";
    CStringArray sArr;
    long nRowCount, nColCount;   
    if (FALSE == m_pAdo->GetRecordSet(sql, sArr, nRowCount, nColCount))
    {
        LOGE("sql 执行失败: " << sql.GetBuffer(0));
        return;
    }
    CString cstrId = sArr.GetAt(0);

    // 插入中标追溯表: t_log_OcrChecked
    // 何人在何时何地打印了标题为什么的文档, 在哪里提交的, 内容涉及什么非法
    LOGI("t_log_OcrChecked: 谁: " << ird.user << ", 在哪里提交: " << ird.submitAddr << ", 在哪里打印: " << ird.printAddr << ", 何时: " << ird.onDataTime << ", 标题: " << ird.title << ", 内容涉及: " << checkWords);
    sql.Format("insert into t_log_OcrChecked(OcrID, UserName, OnDateTime, PrintAddr, SubmitAddr, DocTitle, KeyWord) \n"
               "    select OcrID = %s, UserName = '%s', OnDateTime = '%s', PrintAddr = '%s', SubmitAddr = '%s', DocTitle = '%s', KeyWord = '%s'"
               , cstrId.GetBuffer(0), ird.user.c_str(), ird.onDataTime.c_str(), ird.printAddr.c_str(), ird.submitAddr.c_str(), ird.title.c_str(), keyWord.c_str());
    if (FALSE == m_pAdo->ExecuteSQL(sql))
    {
        LOGE("sql 执行失败: " << sql.GetBuffer(0));
        return;
    }
}

std::vector<OcrType> COcrDBSource::getSrcTypes()
{
    std::vector<OcrType> types;

    CString sql = "select SetName, SetValue1 from t_s_SystemSet where SetName in('EnablePrintOcr', 'EnableCopyOcr', 'EnableScanOcr', 'EnableFaxOcr')";
    CStringArray sArr;
    long nRowCount, nColCount, lIdx;   
    if (FALSE == m_pAdo->GetRecordSet(sql, sArr, nRowCount, nColCount))
    {
        LOGE("sql 执行失败: " << sql.GetBuffer(0));
        types.clear();
        return types;
    }
    CString cstrTmp;
    for (int i = 0; i < nRowCount; ++i)
    {
        lIdx = nColCount * i;

        if (sArr.GetAt(lIdx + 1) != "1")
            continue;


        cstrTmp = sArr.GetAt(lIdx + 0);
        cstrTmp.MakeLower();
        if (cstrTmp == "enableprintocr")
        {
            types.push_back(OcrType::Print);
            LOGD("打印 OCR 已开启");
        }
        else if (cstrTmp == "enablecopyocr")
        {
            types.push_back(OcrType::Copy);
            LOGD("复印留底 OCR 已开启");
        }
        else if (cstrTmp == "enablescanocr")
        {
            types.push_back(OcrType::Scan);
            LOGD("扫描 OCR 已开启");
        }
        else if (cstrTmp == "enablefaxocr")
        {
            types.push_back(OcrType::Fax);
            LOGD("传真 OCR 已开启");
        }
    }


    return types;
}