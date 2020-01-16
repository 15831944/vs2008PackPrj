#include "stdafx.h"
#include "FileMap.h"


CFileMap::CFileMap(void)
{
	m_hMapHandle = NULL;
	m_pMapMem = NULL;
	m_hMutex = 0;
	m_pPipe = 0;
	m_dwTimeout = 1000;
	InitializeCriticalSection(&m_cs);
}

CFileMap::~CFileMap(void)
{
	Close();
}

BOOL CFileMap::Create(CString sName,int nMapSize)
{
	if (m_hMapHandle && m_pMapMem)
	{
		return TRUE;
	}
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
	SECURITY_ATTRIBUTES sa = { sizeof(sa), &sd, FALSE };

	
	m_hMapHandle = CreateFileMapping( (HANDLE)0xFFFFFFFF, &sa,PAGE_READWRITE,0,nMapSize,(LPCTSTR)sName);
	if(!m_hMapHandle)
		return FALSE;
	m_pMapMem = MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS,0,0,0);

	if (!m_pMapMem) 
	{
		CloseHandle(m_hMapHandle);
		return  FALSE;
	}
	WriteFileLog("CreateFileMapping suc");
	return TRUE;
}

BOOL CFileMap::Open(CString sName,int nMapSize)
{
	m_hMapHandle = ::OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE,FALSE,(LPCTSTR)sName);

	if(!m_hMapHandle)
	{
		int ierror=::GetLastError();
		return  FALSE;
	}

	m_pMapMem = MapViewOfFile(m_hMapHandle, FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);
	if (!m_pMapMem) 
	{
		CloseHandle(m_hMapHandle);
		return  FALSE;
	}
	WriteFileLog("OpenFileMapping suc");
	return TRUE;
}

void CFileMap::Close()
{
	if (m_pMapMem)
	{
		::UnmapViewOfFile(m_pMapMem);
		m_pMapMem = NULL;
	}

	if (m_hMapHandle)
	{
		::CloseHandle(m_hMapHandle);
		m_hMapHandle = NULL;
	}
	if (m_hMutex)
	{
		::CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
	if (m_pPipe)
	{
		delete m_pPipe;
		m_pPipe = 0;
	}
	DeleteCriticalSection(&m_cs);
}

BOOL CFileMap::Create(char* szDoc)
{
	CLockCS ow(m_cs);
	int nSize = sizeof(CMapData) + sizeof(CDocumentLogNode) * DOC_LOG_SIZE;
	if(Create(ISEC_HOOK_PRINT_MAP_NAME,nSize))
	{
		if(m_pMapMem)
		{
			memset(m_pMapMem,0,nSize);
			CMapData* pData = (CMapData*)m_pMapMem;
			pData->m_nSize = DOC_LOG_SIZE;
			strcpy_s(pData->m_szDocType,sizeof(pData->m_szDocType),szDoc);
		}

		return InitMutex();
	}
	return FALSE;
}

CString CFileMap::GetDocType()
{
	CString sDocType;
	if (!Open())
	{
		WriteFileLog("GetDocType err");
		return _T(""); 
	}
	CLockCS ow(m_cs);
	DWORD dw = 0;
	CAutoMutex mtx(m_hMutex,m_dwTimeout,dw);
	if (dw != WAIT_OBJECT_0)
	{
		WriteFileLog("GetDocType err %d",dw);
	}
	else
	{
		//HANDLE hMapHandle = ::OpenFileMapping(FILE_MAP_WRITE | FILE_MAP_READ,FALSE,(LPCTSTR)ISEC_HOOK_MAP_NAME);
		if(m_hMapHandle)
		{
			void* pMem = MapViewOfFile(m_hMapHandle, FILE_MAP_WRITE | FILE_MAP_READ,0,0,0);
			CMapData* pData = (CMapData*)pMem;
			sDocType.Format(_T("%s"),pData->m_szDocType);
			UnmapViewOfFile(pMem);
		}
	}
	return sDocType;
}

void CFileMap::OnCloseFile(HANDLE h,CString sProcess)
{
	return ;
	CLockCS ow(m_cs);
	CString sVal;
	if(m_oHandleMap.Lookup(h,sVal))
	{
		//WriteFileLog("OnCloseFile %s",sVal);
		m_oHandleMap.RemoveKey(h);
	}
}

void CFileMap::OnOpenFile(HANDLE h,CString sPath,CString sProcess)
{
	CString sDocType = GetDocType();
	sPath.MakeLower();
	CString sName;
	CString sSuffix;
	int n = sPath.ReverseFind('\\');
	if (n > 0)
	{
		int nLen = sPath.GetLength();
		sName = sPath.Right(nLen - n - 1);
	}

	//---------------
	//检测是否是自己的文档
	if (sProcess.Find("isafeclient.exe") > 0 ||
		sProcess.Find("icltprinthost.exe") > 0 ||
		sProcess.Find("ictlsvchost.exe") > 0)
	{
		return ;
	}

	//if (sProcess.Find("acrord32info.exe") > 0 && sName.Find(".pdf") > 0)
	//{
		//WriteFileLog("handle(%x),process(%s) open %s",h,sProcess,sName);
	//}
	n = sName.ReverseFind('.');
	if (n > 0)
	{
		int nLen = sName.GetLength();
		sSuffix = sName.Right(nLen - n - 1);
	}
	if (sDocType.IsEmpty())
	{
		sDocType.Format(_T("doc|docx|xsl|xlsx|ppt|pptx|rtf|pdf|txt|dwg"));
	}
	if(sSuffix.GetLength() > 0 && CheckDocType(sSuffix,sDocType))
	/*if(sSuffix.CompareNoCase(".doc") == 0 ||
		sSuffix.CompareNoCase(".docx") == 0 ||
		sSuffix.CompareNoCase(".xsl") == 0 ||
		sSuffix.CompareNoCase(".xlsx") == 0 ||
		sSuffix.CompareNoCase(".ppt") == 0 ||
		sSuffix.CompareNoCase(".pptx") == 0 ||
		sSuffix.CompareNoCase(".rtf") ==0 ||
		sSuffix.CompareNoCase(".xls") == 0 ||
		sSuffix.CompareNoCase(".pdf") == 0 ||
		sSuffix.CompareNoCase(".txt") == 0 ||
		//sSuffix.CompareNoCase(".log") == 0 ||
		sSuffix.CompareNoCase(".dwg") == 0)*/
	{
		CLockCS ow(m_cs);
		CString sVal;
		if(m_oHandleMap.Lookup(h,sVal) && sVal.CompareNoCase(sPath) == 0)
		{

		}
		else
		{
			WriteFileLog("handle(%x),process(%s) open %s",h,sProcess,sPath);
			m_oHandleMap[h] = sPath;
			AddNode(sProcess,sPath,h,GetCurrentProcessId());
		}
	}
	else
	{
		//WriteFileLog("OnOpenFile err handle(%x),process(%s) open %s,%s",h,sProcess,sPath,sDocType);
	}
}

BOOL CFileMap::CheckDocType(CString s,CString sDoxType)
{
	BOOL bRet = FALSE;
	
	sDoxType.MakeLower();
	s.MakeLower();
	//WriteFileLog("CheckDocType:%s,%s",s,sDoxType);
	if(sDoxType.Find(s) >= 0 )
	{
		bRet = TRUE;
	}
	return bRet;
}

BOOL CFileMap::Open()
{
	CLockCS ow(m_cs);
	if(m_hMapHandle && m_hMutex)
	{
		return TRUE;
	}
	int nSize = sizeof(CMapData) + sizeof(CDocumentLogNode) * 2000;
	if(Open(ISEC_HOOK_PRINT_MAP_NAME,nSize))
	{
		return InitMutex();
	}
	return FALSE;
}

void CFileMap::OnProcessClosed(DWORD dwPid)
{
	if (!Open())
	{
		WriteFileLog("OnProcessClosed err (%d)",dwPid);
		return ; 
	}
	CLockCS ow(m_cs);
	DWORD dw = 0;
	CAutoMutex mtx(m_hMutex,m_dwTimeout,dw);
	if (dw != WAIT_OBJECT_0)
	{
		WriteFileLog("OnProcessClosed err %d",dw);
	}
	else
	{
		//HANDLE hMapHandle = ::OpenFileMapping(FILE_MAP_WRITE | FILE_MAP_READ,FALSE,(LPCTSTR)ISEC_HOOK_MAP_NAME);
		if(m_hMapHandle)
		{
			void* pMem = MapViewOfFile(m_hMapHandle, FILE_MAP_WRITE | FILE_MAP_READ,0,0,0);
			CMapData* pData = (CMapData*)pMem;
			CDocumentLogNode* pNode = (CDocumentLogNode*)(pData+1);
			WriteFileLog("OnProcessClosed(%d,%d,%d)",pData->m_nUsed,pData->m_nSize,dwPid);
			for (int n = 0; n < pData->m_nSize; n++)
			{
				if(pData->m_nUsed < 1)
					break;
				if (pData->m_bIndexAry[n])
				{
					//文件已经不存在
					if(!PathFileExists(pNode[n].m_szPath))
					{
						pData->m_nUsed --;
						pData->m_bIndexAry[n] = 0;
					}
					else if(pNode[n].m_dwPId == dwPid)
					{
						WriteFileLog("OnProcessClosed(%s,%s,%s)",pNode[n].m_szPath,pNode[n].m_szProcess,pNode[n].m_szDocName);
						pData->m_nUsed --;
						pData->m_bIndexAry[n] = 0;
					}
				}
			}
			UnmapViewOfFile(pMem);
		}
	}
}

void CFileMap::OnPrinted(CString sPrintedDocName,CString sDocName,CString sProcess)
{
	if (!Open())
	{
		WriteFileLog("OnPrinted err (%s,%s)",sDocName,sProcess);
		return ; 
	}
	CLockCS ow(m_cs);
	DWORD dw = 0;
	CAutoMutex mtx(m_hMutex,m_dwTimeout,dw);
	if (dw != WAIT_OBJECT_0)
	{
		WriteFileLog("OnPrinted err %d",dw);
	}
	else
	{
		//HANDLE hMapHandle = ::OpenFileMapping(FILE_MAP_WRITE | FILE_MAP_READ,FALSE,(LPCTSTR)ISEC_HOOK_MAP_NAME);
		if(m_hMapHandle)
		{
			void* pMem = MapViewOfFile(m_hMapHandle, FILE_MAP_WRITE | FILE_MAP_READ,0,0,0);
			CMapData* pData = (CMapData*)pMem;
			CDocumentLogNode* pNode = (CDocumentLogNode*)(pData+1);
			int nCount = 0;
			WriteFileLog("OnPrinted(%d,%d,%s)",pData->m_nUsed,pData->m_nSize,sDocName);
			for (int n = 0; n < pData->m_nSize; n++)
			{
				if (nCount >= pData->m_nUsed)
				{
					break;
				}
				//WriteFileLog("OnPrinted(%d,%d,%s)",n,pData->m_bIndexAry[n],pNode[n].m_szPath);
				//文件已经不存在
				if(!PathFileExists(pNode[n].m_szPath))
				{
					pData->m_nUsed --;
					pData->m_bIndexAry[n] = 0;
					continue;
				}
				if (pData->m_bIndexAry[n])
				{
					nCount ++;
					CString sPath = pNode[n].m_szPath;
					if(sPath.Find(sDocName) >= 0)
					{
						UpdateNode(&pNode[n],sPrintedDocName);
						NotifyPrinted(&pNode[n]);
						WriteFileLog("OnPrinted find the path (%s,%s)",sDocName,sPath);
						break;
					}
				}
			}
			UnmapViewOfFile(pMem);
		}
	}
}
BOOL CFileMap::NotifyPrinted(CDocumentLogNode* pNode)
{
	BOOL bRet = 0;
	if(!m_pPipe)
	{
		m_pPipe = new CPipeClt();
		if(m_pPipe->Open(ISEC_HOOK_PRINT_PIPE))
		{
			WriteFileLog("open pipe suc");
		}
		else
		{
			WriteFileLog("open pipe err");
		}
	}
	if (m_pPipe)
	{
		if(!m_pPipe->Write(WM_PRINT_DOC_EVENT,0,(BYTE*)pNode,sizeof(CDocumentLogNode)))
		{
			//ASSERT(0);
			m_pPipe->Shutdown();
			if(m_pPipe->Open(ISEC_HOOK_PRINT_PIPE))
			{
				bRet = m_pPipe->Write(WM_PRINT_DOC_EVENT,0,(BYTE*)pNode,sizeof(CDocumentLogNode));
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			bRet = TRUE;
		}
	}
	WriteFileLog("PostToPipe ret =%d cmd(%d)",bRet,0);
	return bRet;
}

void CFileMap::FillNode(CDocumentLogNode* pNode,CString sProcess,CString sPath,HANDLE h,DWORD dwPid)
{
	strcpy_s(pNode->m_szProcess,sizeof(pNode->m_szProcess),sProcess);
	strcpy_s(pNode->m_szPath,sizeof(pNode->m_szPath),sPath);
	pNode->m_hFile = h;
	pNode->m_dwPId = dwPid;
}

void CFileMap::UpdateNode(CDocumentLogNode* pNode,CString sDocName)
{
	strcpy_s(pNode->m_szDocName,sizeof(pNode->m_szDocName),sDocName);
}
void CFileMap::AddNode(CString sProcess,CString sPath,HANDLE hFile,DWORD dwPid)
{
	CLockCS ow(m_cs);
	DWORD dw = 0;
	CAutoMutex mtx(m_hMutex,m_dwTimeout,dw);
	if (dw != WAIT_OBJECT_0)
	{
		WriteFileLog("AddNode err %d",dw);
	}
	else
	{
		if(m_hMapHandle)
		{
			void* pMem = MapViewOfFile(m_hMapHandle, FILE_MAP_WRITE | FILE_MAP_READ,0,0,0);
			if(pMem)
			{
				BOOL bSuc = FALSE;
				CMapData* pData = (CMapData*)pMem;
				CDocumentLogNode* pNode = (CDocumentLogNode*)(pData+1);
				for (int n = 0; n < pData->m_nSize; n++)
				{
					if (pData->m_bIndexAry[n] == 0)
					{
						FillNode(&pNode[n],sProcess,sPath,hFile,dwPid);
						pData->m_bIndexAry[n] = 1;
						pData->m_nUsed ++;
						bSuc = TRUE;
						break;
					}
				}
				if (bSuc)
				{
					WriteFileLog("AddNode suc (%d,%d)(%s,%s)",pData->m_nUsed,pData->m_nSize,sProcess,sPath);
				}
				else
				{
					WriteFileLog("AddNode err (%d,%d)(%s,%s)",pData->m_nUsed,pData->m_nSize,sProcess,sPath);
					//可以释放一些文档
				}
				
				UnmapViewOfFile(pMem);
			}

		}
	}
}

BOOL CFileMap::InitMutex()
{
	if (m_hMutex)
	{
		return TRUE;
	}
	DWORD dwRes;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea[2];
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld =
		SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	SECURITY_ATTRIBUTES sa;

	// Create a well-known SID for the Everyone group.
	if(!AllocateAndInitializeSid(&SIDAuthWorld, 1,
		SECURITY_WORLD_RID,
		0, 0, 0, 0, 0, 0, 0,
		&pEveryoneSID))
	{
		//			Err(_T("AllocateAndInitializeSid Error %u\n"), GetLastError());
		goto Cleanup;
	}

	// Initialize an EXPLICIT_ACCESS structure for an ACE.
	// The ACE will allow Everyone read access to the key.
	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
	ea[0].grfAccessPermissions = KEY_ALL_ACCESS|MUTEX_ALL_ACCESS;
	ea[0].grfAccessMode = SET_ACCESS;
	ea[0].grfInheritance= NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName  = (LPTSTR) pEveryoneSID;

	// Create a SID for the BUILTIN\Administrators group.
	if(! AllocateAndInitializeSid(&SIDAuthNT, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdminSID)) 
	{
		//Err(_T("AllocateAndInitializeSid Error %u\n"), GetLastError());
		goto Cleanup; 
	}

	// Initialize an EXPLICIT_ACCESS structure for an ACE.
	// The ACE will allow the Administrators group full access to
	// the key.
	ea[1].grfAccessPermissions = KEY_ALL_ACCESS|MUTEX_ALL_ACCESS;
	ea[1].grfAccessMode = SET_ACCESS;
	ea[1].grfInheritance= NO_INHERITANCE;
	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea[1].Trustee.ptstrName  = (LPTSTR) pAdminSID;

	// Create a new ACL that contains the new ACEs.
	dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
	if (ERROR_SUCCESS != dwRes) 
	{
		//Err(_T("SetEntriesInAcl Error %u\n"), GetLastError());
		goto Cleanup;
	}

	// Initialize a security descriptor.  
	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, 
		SECURITY_DESCRIPTOR_MIN_LENGTH); 
	if (NULL == pSD) 
	{ 
		//Err(_T("LocalAlloc Error %u\n"), GetLastError());
		goto Cleanup; 
	} 

	if (!InitializeSecurityDescriptor(pSD,
		SECURITY_DESCRIPTOR_REVISION)) 
	{  
		//Err(_T("InitializeSecurityDescriptor Error %u\n"),
		//	GetLastError());
		goto Cleanup; 
	} 

	// Add the ACL to the security descriptor. 
	if (!SetSecurityDescriptorDacl(pSD, 
		TRUE,     // bDaclPresent flag   
		pACL, 
		FALSE))   // not a default DACL 
	{  
		//Err(_T("SetSecurityDescriptorDacl Error %u\n"),
		//	GetLastError());
		goto Cleanup; 
	} 

	// Initialize a security attributes structure.
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	m_hMutex = CreateMutex(&sa, FALSE, ISEC_HOOK_PRINT_MUTEX_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		WriteFileLog("CreateMutex ERROR_ALREADY_EXISTS");
	}
Cleanup:

	if (pEveryoneSID) 
		FreeSid(pEveryoneSID);
	if (pAdminSID) 
		FreeSid(pAdminSID);
	if (pACL) 
		LocalFree(pACL);
	if (pSD) 
		LocalFree(pSD);


	return m_hMutex!=NULL;
}