#include "stdafx.h"
#include "Registry2.h"


#define READ_ONLY_KEYS	(KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY)

CRegistry2::CRegistry2(int nMode, BOOL bReadOnly) :
	m_bReadOnly (bReadOnly)
{
	switch(nMode)
	{
	case 0:
		m_hKey=HKEY_CURRENT_USER;
		break;
	case 1:
		m_hKey=HKEY_CURRENT_CONFIG;
		break;
	case 2:
		m_hKey=HKEY_LOCAL_MACHINE;
		break;
	case 3:
		m_hKey = HKEY_USERS;
		break;
	}
}

CRegistry2::~CRegistry2()
{
	Close();
}

BOOL CRegistry2::Open (LPCTSTR pszPath)
{
	ASSERT (pszPath != NULL);

	LONG ReturnValue = RegOpenKeyEx (m_hKey, pszPath, 0L,
		m_bReadOnly ? READ_ONLY_KEYS : KEY_ALL_ACCESS, &m_hKey);

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

void CRegistry2::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;  //edit by dongxs 3.31
	}
}

BOOL CRegistry2::Read(LPCTSTR pszKey, int& iVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistry2::Read (LPCTSTR pszKey, BYTE** ppData, UINT* pBytes)
{
	ASSERT (m_hKey != NULL);
	ASSERT (pszKey != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);
	*ppData = NULL;
	*pBytes = 0;

	DWORD dwType, dwCount;
	LONG lResult = ::RegQueryValueEx (m_hKey, pszKey, NULL, &dwType,
		NULL, &dwCount);

	if (lResult == ERROR_SUCCESS)
	{
		*pBytes = dwCount;
		ASSERT (dwType == REG_BINARY);

		*ppData = new BYTE [*pBytes];
		::ZeroMemory(*ppData, *pBytes);	//add by zfq,2013.01.06

		lResult = ::RegQueryValueEx (m_hKey, pszKey, NULL, &dwType,
			*ppData, &dwCount);
		
		delete [] *ppData;
		*ppData = NULL;

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT (dwType == REG_BINARY);
		}
	}
	return (lResult == ERROR_SUCCESS);
}

BOOL CRegistry2::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistry2::Read (LPCTSTR pszKey, CString& sVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType = REG_SZ;
	DWORD dwSize = 200;
	char  szString[255];
	::ZeroMemory(szString, sizeof(szString)/sizeof(szString[0]));	//add by zfq,2013.01.06,不清空，就会读失败，183

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	if(lReturn == ERROR_SUCCESS)
	{
		sVal = szString;
		return TRUE;
	}

	theLog.Write("!!CRegistry2::Read,10,READ FAIL,err=%d,lReturn=%d,pszKey=%s", GetLastError(), lReturn, pszKey);
	return FALSE;
}

BOOL CRegistry2::CreateKey (LPCTSTR pszPath,int nMode)
{
	switch(nMode)
	{
	case 0:
		m_hKey=HKEY_CURRENT_USER;
		break;
	case 1:
		m_hKey=HKEY_CURRENT_CONFIG;
		break;
	case 2:
		m_hKey=HKEY_LOCAL_MACHINE;
		break;
	case 3:
		m_hKey = HKEY_USERS;
		break;
	}

	if (m_bReadOnly)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT (pszPath != NULL);

	DWORD dw;

	LONG ReturnValue = RegCreateKeyEx (m_hKey, pszPath, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
		&m_hKey, &dw);

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	else
	{
		TRACE("Can't create registry key: %s\n", pszPath);
		return FALSE;
	}
}

BOOL CRegistry2::Write (LPCTSTR pszKey, int iVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	DWORD dwValue;

	ASSERT(m_hKey);
	ASSERT(pszKey);
	
	dwValue = (DWORD)iVal;
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwValue, sizeof(DWORD));

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry2::Write (LPCTSTR pszKey, DWORD dwVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD)) == ERROR_SUCCESS ? TRUE : FALSE;
}

BOOL CRegistry2::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pszData);
	ASSERT(AfxIsValidAddress(pszData, strlen(pszData), FALSE));

	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, strlen(pszData) + 1);

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}
BOOL CRegistry2::Delete(LPCTSTR pszVal)
{
	ASSERT(pszVal);
	ASSERT(m_hKey);
	LONG lReturn = RegDeleteValue(m_hKey,pszVal);
	if(lReturn == ERROR_SUCCESS)
	{
		return TRUE;
	}

	return FALSE;
}
BOOL CRegistry2::QuerySubKeyCount(DWORD& dwCount)
{
	ASSERT(m_hKey);

	long retCode = RegQueryInfoKey(
		m_hKey,                    // key handle 
		NULL,                // buffer for class name 
		0,           // size of class string 
		NULL,                    // reserved 
		&dwCount,               // number of subkeys 
		NULL,            // longest subkey size 
		NULL,            // longest class string 
		NULL,                // number of values for this key 
		NULL,            // longest value name 
		NULL,         // longest value data 
		NULL,   // security descriptor 
		NULL);       // last write time 

	return retCode == ERROR_SUCCESS;
}

BOOL CRegistry2::FindSubKey(CString sSubKey)
{
	BOOL b = FALSE;
	HKEY hKey = NULL;
	LONG ReturnValue = RegOpenKeyEx (m_hKey, sSubKey, 0L,
		READ_ONLY_KEYS , &hKey);

	if(ReturnValue == ERROR_SUCCESS)
	{
		::RegCloseKey(hKey);
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistry2::FindSubKeyDataVal(CString sValName,CString sVal) 
{ 
	ASSERT(m_hKey);

	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	BOOL bFind = FALSE;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		m_hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.
	if ((retCode == ERROR_SUCCESS) && cSubKeys)
	{	
		for (i=0; i<cSubKeys; i++) 
		{ 
			bFind = FALSE;
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(m_hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				HKEY hSubKey = NULL;
				if(RegOpenKeyEx(m_hKey,achKey,0L,KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)
				{
					DWORD dwType;
					DWORD dwSize = 200;
					char  szString[255] = {0};

					LONG lReturn = RegQueryValueEx (hSubKey, sValName, NULL,
						&dwType, (BYTE *) szString, &dwSize);
					if (lReturn == ERROR_SUCCESS)
					{
						if (sVal.CompareNoCase(szString) == 0)
						{
							bFind = TRUE;	
						}
					}
					::RegCloseKey(hSubKey);
				}
			}

			if (bFind)
			{
				break;
			}
		}
	} 

	return bFind;
}