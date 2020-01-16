#include "stdafx.h"
BOOL IsDebug()
{
	LPCTSTR strPath = PROJECT_REGISTY_KEY;
	LPCTSTR strKey=_T("DebugLog");
	DWORD dwRet, dwSize=sizeof(DWORD), dwType=REG_DWORD;
	HKEY hKey;

	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		(LPCTSTR)strPath,
		0,
		KEY_READ,
		&hKey)!= ERROR_SUCCESS)
		return 0;

	if(::RegQueryValueEx(hKey,
		(LPCTSTR)strKey,
		NULL,
		&dwType,
		(LPBYTE)&dwRet,
		&dwSize) != ERROR_SUCCESS)
		dwRet = 0;

	RegCloseKey(hKey);

	return dwRet;
}
void WriteLogEx(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	if (!IsDebug())
	{
		return ;
	}
	va_list argList;
	va_start(argList, lpszFormat);
	CString sMsg;
	sMsg.FormatV(lpszFormat, argList);
	va_end(argList);

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%02d:%02d:%02d",time.GetHour(),time.GetMinute(),time.GetSecond());

	CFile file;
	CString sTemp = strTime;
	sTemp += ":";
	sTemp += sMsg;

	TRACE("%s\n",sTemp);

	CString szFileName;
	szFileName.Format("c:\\xlog\\MulitScreenNews_%02d-%02d-%02d.log",time.GetYear(),time.GetMonth(),time.GetDay());

	if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sTemp,sTemp.GetLength());
		file.Write("\r\n",strlen("\r\n"));
		file.Close();
	}
}
