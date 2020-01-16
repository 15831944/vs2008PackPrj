// RegistryEx.cpp: implementation of the CRegistryEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SetIP.h"
#include "RegistryEx.h"

#pragma warning(disable : 4244)
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define READ_ONLY_KEYS	(KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY)

CRegistryEx::CRegistryEx(BOOL bAdmin, BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;
	if(bAdmin)
		m_hKey =HKEY_LOCAL_MACHINE;
	else
		m_hKey =HKEY_CURRENT_USER;
	//
}

CRegistryEx::~CRegistryEx()
{
	
	Close();
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRegistryEx, CObject)
	//{{AFX_MSG_MAP(CRegistryEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRegistryEx member functions

BOOL CRegistryEx::CreateKey (LPCTSTR pszPath)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (pszPath != NULL);

	DWORD dw;

     	LONG ReturnValue = RegCreateKeyEx (m_hKey, pszPath, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
		&m_hKey, &dw);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	TRACE(_T("Can't create registry key: %s\n"), pszPath);
	return FALSE;
}

BOOL CRegistryEx::Open (LPCTSTR pszPath)
{
	ASSERT (pszPath != NULL);

	m_sPath = pszPath;

	LONG ReturnValue = RegOpenKeyEx (m_hKey, pszPath, 0L,
		   KEY_READ | KEY_WRITE, &m_hKey);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	PrintErrMsg(ReturnValue);
	return FALSE;
}

void CRegistryEx::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, int iVal)
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

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = sizeof(DWORD);
	m_Info.dwType = REG_DWORD;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, DWORD dwVal)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	LONG ReturnValue =  RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	if(!m_hKey)
		return false;
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pszData);
	ASSERT(AfxIsValidAddress(pszData, strlen(pszData), FALSE));

	if (!m_hKey || !pszKey || !pszData)
		return FALSE;

	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, strlen(pszData) + 1);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = strlen(pszData) + 1;
	m_Info.dwType = REG_SZ;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CStringList& scStringList)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(scStringList.IsSerializable());
	scStringList.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CObList& list)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (!m_hKey || !pszKey)
		return FALSE;

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			list.Serialize (ar);
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CObject& obj)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (!m_hKey || !pszKey)
		return FALSE;

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			obj.Serialize (ar);
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CByteArray& bcArray)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(bcArray.IsSerializable());
	bcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CDWordArray& dwcArray)
{
	ASSERT (m_hKey != NULL);
	ASSERT (pszKey != NULL);
	if (!m_hKey || !pszKey)
		return FALSE;

	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);
			dwcArray.Serialize(ar);
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Write ()!\n"));
		return FALSE;
	}

	return bRes;
/*
	const int iMaxChars = 4096;
	LPBYTE lpData = new BYTE [iMaxChars];
	ASSERT (lpData != NULL);

	CMemFile file (lpData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());

	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	delete lpData;
	return (lReturn == ERROR_SUCCESS);*/
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CWordArray& wcArray)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (!m_hKey || !pszKey)
		return FALSE;

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			ar << wcArray.GetSize ();
			for (int i = 0; i < wcArray.GetSize (); i ++)
			{
				ar << wcArray [i];
			}

			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete();
		TRACE(_T("Memory exception in CRegistryEx::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CStringArray& scArray)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(scArray.IsSerializable());
	scArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write(LPCTSTR pszKey, const CRect& rect)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}
	if (!m_hKey || !pszKey)
		return FALSE;

	BOOL bRes = FALSE;
	try
	{
		CMemFile file;

		{
			CArchive ar (&file, CArchive::store);

			ar << rect;
			ar.Flush ();
		}

		DWORD dwDataSize = file.GetLength ();
		LPBYTE lpbData = file.Detach ();

		if (lpbData == NULL)
		{
			return FALSE;
		}

		bRes = Write (pszKey, lpbData, (UINT) dwDataSize);
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Write ()!\n"));
		return FALSE;
	}

	return bRes;
}

BOOL CRegistryEx::Write(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 20;
	CDWordArray dwcArray;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, lpPoint->x);
	dwcArray.SetAt(1, lpPoint->y);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_POINT;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, LPBYTE pData, UINT nBytes)
{
	if (m_bReadOnly)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (m_hKey != NULL);
	ASSERT (pszKey != NULL);
	ASSERT (pData != NULL);
	ASSERT (AfxIsValidAddress (pData, nBytes, FALSE));
	if (!m_hKey || !pszKey)
		return FALSE;

	LONG lResult = ::RegSetValueEx (m_hKey, pszKey, NULL, REG_BINARY,
									pData, nBytes);

	m_Info.lMessage = lResult;
	m_Info.dwSize = nBytes;
	m_Info.dwType = REG_BINARY;

	return (lResult == ERROR_SUCCESS);
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, int& iVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CString& sVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
	{
		theLog.Write("!!CRegistryEx::Read,1,m_hKey=0x%x,pszKey=%p", m_hKey, pszKey);
		return FALSE;
	}

	DWORD dwType;
	DWORD dwSize = 200;
	char  szString[255];
	::ZeroMemory(szString, sizeof(szString)/sizeof(szString[0]));	//add by zfq,2013.01.06,不清空，就会读失败，183

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		sVal = szString;
		return TRUE;
	}

	theLog.Write("!!CRegistryEx::Read,2,read fail,lRet=%d,err=%d,pszKey=%s", lReturn, GetLastError(), pszKey);
	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CStringList& scStringList)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(scStringList.IsSerializable());
		scStringList.RemoveAll();
		scStringList.Serialize(ar);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CByteArray& bcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
	int OldSize = bcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(bcArray.IsSerializable());
		bcArray.RemoveAll();
		bcArray.SetSize(10);
		bcArray.Serialize(ar);
		bcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CDWordArray& dwcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
//	int OldSize = dwcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
//		dwcArray.SetSize(10);
		dwcArray.SetSize(0);
		dwcArray.Serialize(ar);
//		dwcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CWordArray& wcArray)
{
	if (!m_hKey || !pszKey)
		return FALSE;

	wcArray.SetSize (0);

	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		int iSize;
		ar >> iSize;

		wcArray.SetSize (iSize);
		for (int i = 0; i < iSize; i ++)
		{
			ar >> wcArray [i];
		}

		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CRegistryEx::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 4096;
	int OldSize = scArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(scArray.IsSerializable());
		scArray.RemoveAll();
		scArray.SetSize(10);
		scArray.Serialize(ar);
		scArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, CRect& rect)
{
	if (!m_hKey || !pszKey)
		return FALSE;

	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		ar >> rect;
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CRegistryEx::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	if (!m_hKey || !pszKey)
		return FALSE;

	const int iMaxChars = 20;
	CDWordArray dwcArray;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(5);
		dwcArray.Serialize(ar);
		ar.Close();
		file.Close();
		lpPoint->x = dwcArray.GetAt(0);
		lpPoint->y = dwcArray.GetAt(1);
	}

	m_Info.lMessage = lReturn;
	m_Info.dwType = REG_POINT;
	m_Info.dwSize = sizeof(POINT);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, BYTE** ppData, UINT* pBytes)
{
	ASSERT (m_hKey != NULL);
	ASSERT (pszKey != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);
	*ppData = NULL;
	*pBytes = 0;

	if (!m_hKey || !pszKey)
		return FALSE;

	DWORD dwType, dwCount;
	LONG lResult = ::RegQueryValueEx (m_hKey, pszKey, NULL, &dwType,
		NULL, &dwCount);

	if (lResult == ERROR_SUCCESS)
	{
		*pBytes = dwCount;
		ASSERT (dwType == REG_BINARY);

		*ppData = new BYTE [*pBytes];

		lResult = ::RegQueryValueEx (m_hKey, pszKey, NULL, &dwType,
			*ppData, &dwCount);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT (dwType == REG_BINARY);
		}
		else
		{
			delete [] *ppData;
			*ppData = NULL;
		}
	}

	m_Info.lMessage = lResult;
	m_Info.dwType = REG_BINARY;
	m_Info.dwSize = *pBytes;

	return (lResult == ERROR_SUCCESS);
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CObList& list)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!m_hKey || !pszKey)
		return FALSE;

	while (!list.IsEmpty ())
	{
		delete list.RemoveHead ();
	}

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		list.Serialize (ar);
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CRegistryEx::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CObject& obj)
{
	BOOL	bSucess = FALSE;
	BYTE*	pData = NULL;
	UINT	uDataSize;

	if (!m_hKey || !pszKey)
		return FALSE;

	if (!Read (pszKey, &pData, &uDataSize))
	{
		ASSERT (pData == NULL);
		return FALSE;
	}

	ASSERT (pData != NULL);

	try
	{
		CMemFile file (pData, uDataSize);
		CArchive ar (&file, CArchive::load);

		obj.Serialize (ar);
		bSucess = TRUE;
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("Memory exception in CRegistryEx::Read ()!\n"));
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		TRACE(_T("CArchiveException exception in CRegistryEx::Read ()!\n"));
	}

	delete pData;
	return bSucess;
}

BOOL CRegistryEx::DeleteValue (LPCTSTR pszValue)
{
	ASSERT(m_hKey);
	if (!m_hKey)
		return FALSE;

	LONG lReturn = RegDeleteValue(m_hKey, pszValue);


	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::DeleteKey (LPCTSTR pszPath, BOOL bAdmin)
{
	ASSERT(pszPath);

	LONG lReturn = ::RegDeleteKey (HKEY_LOCAL_MACHINE,
									pszPath);
	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

int CRegistryEx::SetParam(BOOL bAdmin, BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;
	Close();
	if(bAdmin)
		m_hKey =HKEY_LOCAL_MACHINE;
    else	
		m_hKey=HKEY_CURRENT_USER;
	return 1;
}

int CRegistryEx::SetParamHKUSER()
{
    m_bReadOnly = FALSE;
	Close();
	m_hKey=HKEY_USERS;
	return 1;
}
void CRegistryEx::SetRootKey(HKEY DHkey)
{
  	Close();
    m_hKey =DHkey;
	return  ;
}

void CRegistryEx::PrintErrMsg(DWORD dwMessageID)
{
	//add by onin ,display error.
	
	LPVOID lpMsgBuf;
	FormatMessage( 
	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	FORMAT_MESSAGE_FROM_SYSTEM | 
	FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
	dwMessageID,
	0, // Default language
	(LPTSTR) &lpMsgBuf,
	0,
	NULL 
	);

	TRACE("%s\n",(LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}


CIniFile::CIniFile(void)
: m_sPath(_T(""))
{
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