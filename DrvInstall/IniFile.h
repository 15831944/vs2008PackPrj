#pragma once

class CIniFile
{
public:
	CIniFile(void);
public:
	~CIniFile(void);
public:
	CString GetVal(CString sSection, CString sKey, CString sDefault);
	int SetVal(CString sSection, CString sKey, CString sVal);
	int GetVal(CString sSection, CString sKey,int nDefault);
	int SetVal(CString sSection, CString sKey, int nVal);
public:
	// Â·¾¶
	CString m_sPath;
};
