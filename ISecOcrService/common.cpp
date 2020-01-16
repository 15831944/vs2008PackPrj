#include "stdafx.h"
#include "common.h"


MY_NAMESPACE_BEGIN

const int OcrPeriod = 10;
const std::string OutRootDir("OcrDir\\");

MY_NAMESPACE_END


void WriteLog(LPCTSTR szMsg,LPCTSTR szFileName)
{

    CTime time = CTime::GetCurrentTime();
    CString strTime;
    strTime = time.Format("%Y-%m-%d  %H:%M:%S ");

    try
    {
        CFile file;
        CString sTemp = strTime;
        //sTemp += "----->>>";
        sTemp += szMsg;
        if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
        {
            file.SeekToEnd();
            file.Write(sTemp,sTemp.GetLength());
            file.Write("\r\n",strlen("\r\n"));
            file.Close();
        }
    }
    catch ( CFileException* ex ) 
    {
        ex->Delete();
    }
    catch (...) 
    {
        ;
    }
}

void WriteLogEx(LPCTSTR lpszFormat, ...)
{
    //日志开关
    // 	if(!CGlobalConfig::GetInstance().m_bEnableDebugLog)
    // 	{
    // 		return ;
    // 	}
    ASSERT(AfxIsValidString(lpszFormat));

    va_list argList;
    va_start(argList, lpszFormat);
    CString sTmp;
    sTmp.FormatV(lpszFormat, argList);
    va_end(argList);

    TRACE("%s\n",sTmp);

    CString sSysDir;
    sSysDir = "C:";	

    CFileFind fd;
    sSysDir += "\\Xlog";	

    if( !fd.FindFile(sSysDir) )
    {
        if( !::CreateDirectory(sSysDir,NULL) )
            return;
    }
    CTime time = CTime::GetCurrentTime();
    CString sDate = time.Format("%y%m%d");

    CString sFileName;
    sFileName.Format("%s\\generatetrancript_%s.Log",sSysDir,sDate);

    WriteLog(sTmp,sFileName);
}