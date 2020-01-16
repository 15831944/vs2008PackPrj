#include <winreg.h>

#define REG_RECT	0x0001
#define REG_POINT	0x0002

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#ifndef CRegistry2_H
#define CRegistry2_H

class _AA_DLL_EXPORT_ CRegistry2 : public CObject
{
// Construction
public:
	CRegistry2 (int nMode, BOOL bReadOnly);
	virtual ~CRegistry2();

// Operations
public:
	BOOL Open(LPCTSTR pszPath);
	BOOL Read(LPCTSTR pszKey, int& iVal);
	BOOL Read(LPCTSTR pszKey,BYTE** ppData,UINT* pBytes);
	BOOL Read (LPCTSTR pszKey, DWORD& dwVal);
	BOOL Read (LPCTSTR pszKey, CString& sVal);
	BOOL Delete(LPCTSTR pszVal);

	BOOL CreateKey(LPCTSTR pszPath,int nMode = -1);
	BOOL Write(LPCTSTR pszKey, int iVal);
	BOOL Write(LPCTSTR pszKey,BYTE* pData,UINT uBytes);
	BOOL Write (LPCTSTR pszKey, DWORD dwVal);
	BOOL Write (LPCTSTR pszKey, LPCTSTR pszVal);

	void Close();
	BOOL QuerySubKeyCount(DWORD& dwCount);
	BOOL FindSubKeyDataVal(CString sValName,CString sVal) ;
	BOOL FindSubKey(CString sSubKey);
protected:

	HKEY		m_hKey;
	const BOOL	m_bReadOnly;
};

#endif