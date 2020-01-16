#include "stdafx.h"
#include "IniFile.h"
// #include "FileLog.h"
CIniFile::CIniFile(void)
: m_sPath(_T(""))
{
	//m_sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
}

CIniFile::~CIniFile(void)
{
}

CString CIniFile::GetVal(CString sSection, CString sKey, CString sDefault)
{
	TCHAR inBuf[MAX_PATH] = {0};
	DWORD dw = GetPrivateProfileString (sSection,sKey, sDefault, inBuf, MAX_PATH, m_sPath); 
// 	ASSERT(dw > 0);
// 	CFileLog log;
// 	log.SetLogName("CIniFile");
// 	log.Write("GetVal sSection = %s,skey = %s,sDefault = %s,val(%s),dw = %d,err = %d,path(%s)",
// 		sSection,sKey, sDefault,inBuf,dw,::GetLastError(),m_sPath);
	return CString(inBuf);
}

int CIniFile::GetVal(CString sSection, CString sKey,int nDefault)
{
	TCHAR inBuf[MAX_PATH] = {0};
	CString s;
	s.Format(_T("%d"),nDefault);
	GetPrivateProfileString (sSection,sKey, s, inBuf, MAX_PATH, m_sPath); 
	return _tstoi(inBuf);
}

int CIniFile::SetVal(CString sSection, CString sKey, CString sVal)
{
	return WritePrivateProfileString (sSection,sKey, sVal, m_sPath); 
}

int CIniFile::SetVal(CString sSection, CString sKey, int nVal)
{
	TCHAR inBuf[MAX_PATH] = {0};
	CString sVal;
	sVal.Format(_T("%d"),nVal);
	return WritePrivateProfileString (sSection,sKey, sVal, m_sPath); 
}