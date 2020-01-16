// SQliteDataSvc.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SQliteDataSvc.h"
#include <aclapi.h>

#define SQLITE_MUTEX_NAME TEXT("Global\\@#$SQLITEDATASVC$#@%")
CFileLog theLog;
#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)
#define FILE_AND_LINE __FILE__ "@" QUOTE(__LINE__)

#if 0
#define Err theLog.Write(FILE_AND_LINE"Err:");theLog.Write
#define Log theLog.Write(FILE_AND_LINE"log:");theLog.Write
#else
void Err(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	theLog.Write("Err");
	theLog.Write(sTmp);
}

void Log(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	theLog.Write("Log");
	theLog.Write(sTmp);
}
#endif

CSQliteDataSvc::CSQliteDataSvc()
{
	m_hDB = NULL;
	m_hMutex = NULL;
	m_dwTimeout = 10 * 1000;
	m_szMutexName = SQLITE_MUTEX_NAME;
	memset(m_szDbFile,0,MAX_PATH);
	memset(m_szPwd,0,MAX_PATH);
	theLog.SetLogName("sqlite");
}

CSQliteDataSvc::~CSQliteDataSvc()
{
	Release();
}

CSQliteDataSvc& CSQliteDataSvc::GetInstance()
{
	static CSQliteDataSvc theOne;
	return theOne;
}

BOOL CSQliteDataSvc::CreateDB(char* szDb)
{
	if(!Connect() || !szDb)
		return FALSE;

	return ExecuteSQL(szDb);
}

void CSQliteDataSvc::SetsMutexName(CString szMutexName)
{
	m_szMutexName = szMutexName;
}

void CSQliteDataSvc::InitConnt(char* szDbFile /*= NULL*/,char* szPwd/* = NULL*/,DWORD dwTimeout /*= 10 * 1000*/)
{
	ASSERT(szDbFile);
	//ASSERT(szPwd);
	if(szDbFile)
	{
		strcpy_s(m_szDbFile,sizeof(m_szDbFile),szDbFile);
	}
	if(szPwd)
	{
		strcpy_s(m_szPwd,sizeof(m_szPwd),szPwd);
	}
	m_dwTimeout = dwTimeout;
	Log("CSQliteDataSvc::InitConnt,szDbFile=[%s], version=[%s]",szDbFile, sqlite3_libversion());

	//add by zxl,20160626,检测本地数据表变更
	CheckCloudDbTables();	
}
BOOL CSQliteDataSvc::Connect()
{
	if(m_hDB && m_hMutex)
		return TRUE;
#if 0
	TCHAR szPath[MAX_PATH];
	if(!GetModuleFileName(GetModuleHandle(_T("SQLiteDataSvc.dll")), szPath, MAX_PATH ))
		return FALSE;
	TCHAR* psz = (TCHAR*)_tcsrchr(szPath, _T('\\'));
	if(psz)
		_tcscpy_s(psz, MAX_PATH-(psz-szPath)*sizeof(WCHAR), _T("\\ABInfoB.dll")); // 伪装的DB名称
#endif
	//int n = sqlite3_open(m_szDbFile, &m_hDB);
	//int n = sqlite3_open16(L":memory:", &g_db);	// 内存数据库，效率差别非常大，参考profile测试用例
	int n = sqlite3_open(CCommonFun::MultiByteToUTF8(m_szDbFile), &m_hDB);	//传入参数是UTF8格式，否则中文路径会打开失败
	if(n!=SQLITE_OK) {
		Err(TEXT("!!CSQliteDataSvc::Connect,1,open db fail(%s)! err:%d\n"), m_szDbFile, n);
		return FALSE;
	}
	//加密
	if(strlen(m_szPwd))
	{
		sqlite3_key(m_hDB,m_szPwd,strlen(m_szPwd));
	}

	if(m_hMutex==NULL && !InitMutex()) 
	{
		Err(TEXT("!!CSQliteDataSvc::Connect,2,ad_create_mutex失败，err:%d\n"), GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL CSQliteDataSvc::InitMutex()
{
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
		Err(_T("!!CSQliteDataSvc::InitMutex,1,AllocateAndInitializeSid Error %u\n"), GetLastError());
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
		Err(_T("!!CSQliteDataSvc::InitMutex,2,AllocateAndInitializeSid Error %u\n"), GetLastError());
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
		Err(_T("!!CSQliteDataSvc::InitMutex,3,SetEntriesInAcl Error %u\n"), GetLastError());
		goto Cleanup;
	}

	// Initialize a security descriptor.  
	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, 
		SECURITY_DESCRIPTOR_MIN_LENGTH); 
	if (NULL == pSD) 
	{ 
		Err(_T("!!CSQliteDataSvc::InitMutex,4,LocalAlloc Error %u\n"), GetLastError());
		goto Cleanup; 
	} 

	if (!InitializeSecurityDescriptor(pSD,
		SECURITY_DESCRIPTOR_REVISION)) 
	{  
		Err(_T("!!CSQliteDataSvc::InitMutex,5,InitializeSecurityDescriptor Error %u\n"),
			GetLastError());
		goto Cleanup; 
	} 

	// Add the ACL to the security descriptor. 
	if (!SetSecurityDescriptorDacl(pSD, 
		TRUE,     // bDaclPresent flag   
		pACL, 
		FALSE))   // not a default DACL 
	{  
		Err(_T("!!CSQliteDataSvc::InitMutex,6,SetSecurityDescriptorDacl Error %u\n"),
			GetLastError());
		goto Cleanup; 
	} 

	// Initialize a security attributes structure.
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	m_hMutex = CreateMutex(&sa, FALSE, /*SQLITE_MUTEX_NAME*/m_szMutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		Log("##CSQliteDataSvc::InitMutex,7,CreateMutex ERROR_ALREADY_EXISTS");
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

void CSQliteDataSvc::Release()
{
	if(m_hDB) 
	{
		sqlite3_close(m_hDB);
		m_hDB=NULL;
	}

	if(m_hMutex) 
	{
		CloseHandle(m_hMutex);
		m_hMutex=NULL;
	}

}

BOOL CSQliteDataSvc::Insert(char* szSQL)
{
	return 0;
}

BOOL CSQliteDataSvc::ExecuteSQL(char* szSQL)
{
	if(!Connect() || !szSQL)
		return FALSE;

	int nCode=-1;

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		for(int i=0; i<10; i++) 
		{
			nCode = sqlite3_exec(m_hDB, szSQL, 0, 0, 0);
			if(nCode != SQLITE_BUSY)
				break;

			Sleep(20);

		}
		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::ExecuteSQL,WaitForSingleObject, err:%d\n"), GetLastError());
	}

	if(nCode != SQLITE_OK)
		Err(_T("!!CSQliteDataSvc::ExecuteSQL,exec_sql FAILED! sql:%s code:%d\n"), szSQL, nCode);

	return nCode==SQLITE_OK;
}

BOOL CSQliteDataSvc::GetRecordStream(char* szSQL,BYTE*& pBuf,int &nBufSize)
{
	if(!szSQL|| !Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	isafetec::datastream data;
	int nRowSize = 0;
	int nColSize = 0;
	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_step(pst);
			if(code==SQLITE_ROW) 
			{
				nColSize = sqlite3_column_count(pst);
				for(int n = 0; n < nColSize; n ++)
				{
					const char* pRs = (const char*)sqlite3_column_text(pst,n);
					data.add_val(pRs);
				}
				nRowSize++;
				
			}
			else if(code != SQLITE_DONE)
			{
				Err(_T("!!CSQliteDataSvc::GetRecordStream,get_value, sqlite3_step, err:%d\n"), code);
			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetRecordStream,get_value, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetRecordStream,get_value, sqlite3_finalize, err:%d\n"), code);
			}
			
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetRecordStream,get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetRecordStream,WaitForSingleObject, err:%d\n"), GetLastError());
	}
	data.set_col(nColSize);
	data.set_row(nRowSize);
	nBufSize = data.ByteSize();
	pBuf = new BYTE[nBufSize];
	ASSERT(pBuf);
	
	if(pBuf)
	{
		memset(pBuf,0,nBufSize);
		bRet = data.SerializeToArray(pBuf,nBufSize);
	}
	return bRet;
}

BOOL CSQliteDataSvc::GetRecordStream(char* szSQL,isafetec::datastream& ary)
{
	if(!szSQL|| !Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	isafetec::datastream data;
	int nRowSize = 0;
	int nColSize = 0;
	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_step(pst);
			if(code==SQLITE_ROW) 
			{
				nColSize = sqlite3_column_count(pst);
				for(int n = 0; n < nColSize; n ++)
				{
					const char* pRs = (const char*)sqlite3_column_text(pst,n);
					data.add_val(pRs);
				}
				nRowSize++;

			}
			else if(code != SQLITE_DONE)
			{
				Err(_T("!!CSQliteDataSvc::GetRecordStream,B,get_value, sqlite3_step, err:%d\n"), code);
			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetRecordStream,B,get_value, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetRecordStream,B,get_value, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetRecordStream,B,get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetRecordStream,B,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}
	data.set_col(nColSize);
	data.set_row(nRowSize);
	return bRet;
}

BOOL CSQliteDataSvc::GetSimplePolicy(POLICY_TYPE type,SimplePolicyList& list)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL),"select id,last_modified_time from  t_c_policy where type = %d order by id;",type);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					int nID = sqlite3_column_int(pst,0);
					double dtime = sqlite3_column_double(pst,1);
					SimplePolicy* node = list.add_list();
					node->set_id(nID);
					node->set_type(type);
					node->set_last_time(dtime);
				}
				else
					break;
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetSimplePolicy,get_value, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetSimplePolicy,get_value, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetSimplePolicy,get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetSimplePolicy,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}
	return bRet;
}

BOOL CSQliteDataSvc::CheckSimplePolicy(SimplePolicyList* list,SimplePolicyList& rs,SimplePolicyList& rs2)
{
	if(!Connect())
		return FALSE;

	CString sWhere;
	CString sTmp;
	char szSQL[1024] = {0};
	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		for (int n = 0 ; n < list->list_size(); n++)
		{
			sTmp.Format("%d",list->list(n).id());
			sWhere += sTmp;
			sWhere += ",";
			sprintf_s(szSQL,sizeof(szSQL)," select last_modified_time from  t_c_policy where id = %d",list->list(n).id());
		
			pst = NULL;
			pc = NULL;
			int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
			if(code==SQLITE_OK) 
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{	
					double dtime = sqlite3_column_double(pst,0);
					COleDateTime dtLocal,dtRemote;
					dtLocal.m_dt = dtime;
					dtRemote.m_dt = list->list(n).last_time();
					COleDateTimeSpan sp = dtRemote - dtLocal;
					theLog.Write("CSQliteDataSvc::CheckSimplePolicy,B, %f",sp.GetTotalSeconds());
					//存在，需要更新
					if(sp.GetTotalSeconds() > 1)
					{
						theLog.Write("CSQliteDataSvc::CheckSimplePolicy,B, need update %d,%f,%f",list->list(n).id(),dtime,list->list(n).last_time());
						SimplePolicy* node = rs.add_list();
						node->set_id(list->list(n).id());
						node->set_type(list->list(n).type());
					}
					else
					{
						theLog.Write("CSQliteDataSvc::CheckSimplePolicy,B, can use %d",list->list(n).id());
						//已经是最新的，可以立即生效
						SimplePolicy* node = rs2.add_list();
						node->set_id(list->list(n).id());
						node->set_type(list->list(n).type());
					}
				}
				else if(code == SQLITE_DONE)
				{
					theLog.Write("CSQliteDataSvc::CheckSimplePolicy,B, need get new ,not exist %d",list->list(n).id());
					//不存在，需要更新
					SimplePolicy* node = rs.add_list();
					node->set_id(list->list(n).id());
					node->set_type(list->list(n).type());
				}
				else
				{
					ASSERT(0);
				}
				code = sqlite3_reset(pst);
				if (code!=SQLITE_OK)
				{
					Err(_T("!!CSQliteDataSvc::GetSimplePolicy,B, sqlite3_reset, err:%d\n"), code);
				}
				code = sqlite3_finalize(pst);
				if (code!=SQLITE_OK)
				{
					Err(_T("!!CSQliteDataSvc::GetSimplePolicy,B, sqlite3_finalize, err:%d\n"), code);
				}
				bRet = TRUE;
			}
			else
			{
				Err(_T("!!CSQliteDataSvc::CheckSimplePolicy,B,get_value, sqlite3_prepare, err:%d\n"), code);
			}
		}

		sWhere.TrimRight(",");
		//清除不需要的策略
		if (!sWhere.IsEmpty())
		{
			sprintf_s(szSQL,sizeof(szSQL)," delete from t_c_Policy where id not in (%s);",sWhere);
			ExecuteSQL(szSQL);
		}
		
		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::CheckSimplePolicy,B,get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

//获取本地策略信息
BOOL CSQliteDataSvc::GetPolicy(int nId,BYTE*& pData,int& nSize)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select detail_info from t_c_policy where id = %d; ",nId);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_step(pst);
			if(code == SQLITE_ROW) 
			{
				const void* pTmp = sqlite3_column_blob(pst,0);
				nSize = sqlite3_column_bytes(pst,0);
				if(nSize > 0)
				{
					pData = new BYTE[nSize];
					if(pData)
					{
						memcpy(pData,pTmp,nSize);
					}
				}
				bRet = TRUE;
			}
			else if(code != SQLITE_DONE)
			{
				Err(_T("CSQliteDataSvc::GetPolicy,get_value, step, err:%d\n"), code);
			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetPolicy, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetPolicy, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("CSQliteDataSvc::GetPolicy,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("CSQliteDataSvc::GetPolicy,get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::UpdatePolicy(int nId,int nType,double time,BYTE* pData,int nSize)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," insert or replace into t_c_policy (id,type,detail_info,last_modified_time) values (%d,%d,@AA , %f)",nId,nType,time);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout) == WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_bind_blob(pst,1,pData,nSize,0);
			if(code == SQLITE_OK)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_DONE)
				{
					bRet = TRUE;
				}
			}

// 			code = sqlite3_clear_bindings(pst);
// 			if (code!=SQLITE_OK)
// 			{
// 				Err(_T("!!CSQliteDataSvc::UpdatePolicy, sqlite3_clear_bindings, err:%d\n"), code);
// 			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::UpdatePolicy, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::UpdatePolicy, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::UpdatePolicy,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::UpdatePolicy,get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::UpdateAssets(SYSTEM_INFO_TYPE eType,double time,BYTE* pData,int nSize,int nUploadSuc /*= 1*/)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	char szSQL[1024] = {0};

	sprintf_s(szSQL,sizeof(szSQL)," insert or replace into t_c_assets (type,last_scan_time,desc,Uploadsuc) values (%d,%f,@AA,%d )",
		eType,time,nUploadSuc);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout) == WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_bind_blob(pst,1,pData,nSize,0);
			if(code == SQLITE_OK)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_DONE)
				{
					bRet = TRUE;
				}
			}
			
			code = sqlite3_finalize(pst);

		}
		else
		{
			Err(_T("get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

//获取硬件信息
BOOL CSQliteDataSvc::GetAssets(SYSTEM_INFO_TYPE eType,BYTE*& pData,int &nSize,BOOL& bUpSucced)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL),"select desc,Uploadsuc from t_c_assets where type = %d",eType);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_step(pst);
			if(code == SQLITE_ROW) 
			{
				const void* pTmp = sqlite3_column_blob(pst,0);
				nSize = sqlite3_column_bytes(pst,0);
				pData = new BYTE[nSize];
				if(pData)
				{
					memcpy(pData,pTmp,nSize);
				}
				bUpSucced = sqlite3_column_int(pst,1);
				bRet = TRUE;
			}
			else if(code != SQLITE_DONE)
			{
				Err(_T("get_value, step, err:%d\n"), code);
			}
			sqlite3_finalize(pst);
			bRet = TRUE;
		}
		else
		{
			Err(_T("get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::CleanAssets()
{
	if(!Connect())
		return FALSE;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL),"delete from t_c_assets");
	return ExecuteSQL(szSQL);
}

BOOL CSQliteDataSvc::InsertLog(LOG_TYPE eType,BYTE* pData,int nSize,int &nId)
{
	if(!Connect())
		return FALSE;

	nId = 0;
	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," insert into t_c_log (type,data) values (%d,@AA)",eType);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout) == WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_bind_blob(pst,1,pData,nSize,0);
			if(code == SQLITE_OK)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_DONE)
				{
					bRet = TRUE;
				}
				else
				{
					Err(_T("!!CSQliteDataSvc::InsertLog,B,1 sqlite3_step, err:%d\n"), code);
				}
			}
			else
			{
				Err(_T("!!CSQliteDataSvc::InsertLog,B,2 sqlite3_bind_blob, err:%d\n"), code);
			}

// 			code = sqlite3_clear_bindings(pst);
// 			if (code!=SQLITE_OK)
// 			{
// 				Err(_T("!!CSQliteDataSvc::InsertLog,B,3 sqlite3_clear_bindings, err:%d\n"), code);
// 			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::InsertLog,B,4 sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::InsertLog,B,5 sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::InsertLog,B,6 sqlite3_prepare, err:%d\n"), code);
		}

		//插入成功,获取最好插入的id
		if (bRet)
		{
			sprintf_s(szSQL,sizeof(szSQL)," select id from t_c_log where type = %d order by id desc limit 0,1;",eType);
			int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
			if(code==SQLITE_OK) 
			{
				
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					nId = sqlite3_column_int(pst,0);
					
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::InsertLog,B,7,get_value, step, err:%d\n"), code);
					
				}
				code = sqlite3_reset(pst);
				if (code!=SQLITE_OK)
				{
					Err(_T("!!CSQliteDataSvc::InsertLog,B,8 sqlite3_reset, err:%d\n"), code);
				}
				code = sqlite3_finalize(pst);
				if (code!=SQLITE_OK)
				{
					Err(_T("!!CSQliteDataSvc::InsertLog,B,9 sqlite3_finalize, err:%d\n"), code);
				}
				bRet = TRUE;
			}
			else
			{
				Err(_T("!!CSQliteDataSvc::InsertLog,B,10 sqlite3_prepare, err:%d\n"), code);
			}
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::InsertLog,B,11 wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::InsertLog(LOG_TYPE eType,BYTE* pData,int nSize)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," insert into t_c_log (type,data) values (%d,@AA)",eType);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout) == WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_bind_blob(pst,1,pData,nSize,0);
			if(code == SQLITE_OK)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_DONE)
				{
					bRet = TRUE;
				}
				else
				{
					Err(_T("!!CSQliteDataSvc::InsertLog, sqlite3_step, err:%d\n"), code);
				}
			}
			else
			{
				Err(_T("!!CSQliteDataSvc::InsertLog, sqlite3_bind_blob, err:%d\n"), code);
			}

// 			code = sqlite3_clear_bindings(pst);
// 			if (code!=SQLITE_OK)
// 			{
// 				Err(_T("!!CSQliteDataSvc::InsertLog, sqlite3_clear_bindings, err:%d\n"), code);
// 			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::InsertLog, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::InsertLog, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::InsertLog, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::InsertLog,  wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::DelLog(LOG_TYPE eType,int nId /*= 0*/)
{
	char szSQL[1024] = {0};
	if(nId > 0)
	{
		sprintf_s(szSQL,sizeof(szSQL)," delete from t_c_log where id = %d and type = %d; ",nId,eType);
	}
	else
	{
		sprintf_s(szSQL,sizeof(szSQL)," delete from t_c_log where type = %d; ",eType);
	}
	return ExecuteSQL(szSQL);
}

BOOL CSQliteDataSvc::GetOnePrintLog(SubmitPrintJob* job,int &nId)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	ASSERT(job);
	if (!job)
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	CString sTmp;
	CString sWhere;
	if (nId == 0)
	{
		sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log where type = %d order by id limit 0,1; ",PRINT_LOG);

	}
	else
	{
		sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log where type = %d and id = %d ; ",PRINT_LOG,nId);

	}

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					nId = sqlite3_column_int(pst,0);
					int nType = sqlite3_column_int(pst,1);
					const void* pTmp = sqlite3_column_blob(pst,2);
					int nSize = sqlite3_column_bytes(pst,2);					
					job->ParseFromArray((BYTE*)pTmp,nSize);
					bRet = TRUE;
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::GetOnePrintLog, sqlite3_step, err:%d\n"), code);
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetOnePrintLog, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetOnePrintLog, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetOnePrintLog, get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetOnePrintLog, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}
BOOL CSQliteDataSvc::QueryLog(ICltLogCallback* pCall,LOG_TYPE eType)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	CString sTmp;
	CString sWhere;
	sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log where type = %d; ",eType);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					int nID = sqlite3_column_int(pst,0);
					int nType = sqlite3_column_int(pst,1);
					const void* pTmp = sqlite3_column_blob(pst,2);
					int nSize = sqlite3_column_bytes(pst,2);
					if(pCall )
					{
						pCall->OnLogCallback(nID,nType,pTmp,nSize);
						
					}
					bRet = TRUE;
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::QueryLog, sqlite3_step, err:%d\n"), code);
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::QueryLog, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::QueryLog, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::QueryLog,get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::QueryLog, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}
int CSQliteDataSvc::GetLogCount(LOG_TYPE eType /*= NULL_LOG*/)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nCount = 0;
	char szSQL[1024] = {0};
	CString sTmp;
	CString sWhere;
	if (eType == NULL_LOG)
	{
		sprintf_s(szSQL,sizeof(szSQL)," select count(*) from t_c_log ; ");
	}
	else
	{
		sprintf_s(szSQL,sizeof(szSQL)," select count(*) from t_c_log where type = %d; ",eType);
	}

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					nCount = sqlite3_column_int(pst,0);
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::GetLogCount,get_value, step, err:%d\n"), code);
					break;
				}
			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetLogCount, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetLogCount, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetLogCount,get_value, prepare, err:%d\n"), code);
		}

		
		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetLogCount,get_value, wait, err:%d\n"), GetLastError());
	}

	return nCount;
}
BOOL CSQliteDataSvc::PopOnePrintLog(ICltLogCallback* pCall,int nid,void* context /*= NULL*/)
{

	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	CString sTmp;
	CString sWhere;
	
	sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log where type = %d and id = %d order by id ; ",PRINT_LOG,nid);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					int nID = sqlite3_column_int(pst,0);
					int nType = sqlite3_column_int(pst,1);
					const void* pTmp = sqlite3_column_blob(pst,2);
					int nSize = sqlite3_column_bytes(pst,2);
					if(pCall )
					{
						if(pCall->OnLogUpload(nID,nType,pTmp,nSize,context))
						{
							sTmp.Format("%d,",nID);
							sWhere += sTmp;
						}
					}
					bRet = TRUE;
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::PopOnePrintLog,  step, err:%d\n"), code);
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::PopOnePrintLog, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::PopOnePrintLog, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;

		}
		else
		{
			Err(_T("!!CSQliteDataSvc::PopOnePrintLog,  prepare, err:%d\n"), code);
		}

		//更新状态
		theLog.Write("CSQliteDataSvc::PopOnePrintLog, after poplog,then del log where = %s ",sWhere);
		sWhere.TrimRight(",");
		if(!sWhere.IsEmpty())
		{
			int nLen = sWhere.GetLength() + 100;
			char *pSql = new char[nLen];
			ASSERT(pSql);
			if (pSql)
			{
				sprintf_s(pSql,nLen," delete from t_c_log  where id in (%s);",sWhere);

				ExecuteSQL(pSql);

				delete pSql;
			}
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("CSQliteDataSvc::PopOnePrintLog, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}
BOOL CSQliteDataSvc::PopOneLog(ICltLogCallback* pCall,LOG_TYPE eType/* = NULL_LOG*/,void* context /*= NULL*/)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	CString sTmp;
	CString sWhere;
	if (eType == NULL_LOG)
	{
		sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log order by id limit 0,1; ");
	}
	else
	{
		sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log where type = %d order by id limit 0,1; ",eType);
	}

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					int nID = sqlite3_column_int(pst,0);
					int nType = sqlite3_column_int(pst,1);
					const void* pTmp = sqlite3_column_blob(pst,2);
					int nSize = sqlite3_column_bytes(pst,2);
					if(pCall )
					{
						if(pCall->OnLogUpload(nID,nType,pTmp,nSize,context))
						{
							sTmp.Format("%d,",nID);
							sWhere += sTmp;
						}
					}
					bRet = TRUE;
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::PopOneLog, step, err:%d\n"), code);
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::PopOneLog, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::PopOneLog, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;


		}
		else
		{
			Err(_T("!!CSQliteDataSvc::PopOneLog, prepare, err:%d\n"), code);
		}

		//更新状态
		theLog.Write("CSQliteDataSvc::PopOneLog, after poplog,then del log where = %s ",sWhere);
		sWhere.TrimRight(",");
		if(!sWhere.IsEmpty())
		{
			int nLen = sWhere.GetLength() + 100;
			char *pSql = new char[nLen];
			ASSERT(pSql);
			if (pSql)
			{
				sprintf_s(pSql,nLen," delete from t_c_log  where id in (%s);",sWhere);

				ExecuteSQL(pSql);

				delete pSql;
			}
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::PopOneLog, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}
BOOL CSQliteDataSvc::PopLog(ICltLogCallback* pCall,LOG_TYPE eType,void* context /*= NULL*/)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	int nRowSize = 0;
	int nColSize = 0;
	char szSQL[1024] = {0};
	CString sTmp;
	CString sWhere;
	if (eType == NULL_LOG)
	{
		sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log order by id; ");
	}
	else
	{
		sprintf_s(szSQL,sizeof(szSQL)," select id, type,data from t_c_log where type = %d order by id; ",eType);
	}
	
	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					int nID = sqlite3_column_int(pst,0);
					int nType = sqlite3_column_int(pst,1);
					const void* pTmp = sqlite3_column_blob(pst,2);
					int nSize = sqlite3_column_bytes(pst,2);
					if(pCall )
					{
						if(pCall->OnLogUpload(nID,nType,pTmp,nSize,context))
						{
							sTmp.Format("%d,",nID);
							sWhere += sTmp;
						}
					}
					bRet = TRUE;
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::PopLog, sqlite3_step, err:%d\n"), code);
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::PopLog, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::PopLog, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::PopLog, sqlite3_prepare, err:%d\n"), code);
		}

		//更新状态
		theLog.Write("CSQliteDataSvc::PopLog,after poplog,then del log where = %s ",sWhere);
		sWhere.TrimRight(",");
		if(!sWhere.IsEmpty())
		{
			int nLen = sWhere.GetLength() + 100;
			char *pSql = new char[nLen];
			ASSERT(pSql);
			if (pSql)
			{
				sprintf_s(pSql,nLen," delete from t_c_log  where id in (%s);",sWhere);

				ExecuteSQL(pSql);

				delete pSql;
			}
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::PopLog, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::SaveConfig(CString sName,CString sVal,BYTE* pData,int nSize)
{
	if(!Connect())
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," insert or replace into t_c_LocalConfig (name,val,data) values ('%s','%s',@AA)",sName,sVal);
	if(WaitForSingleObject(m_hMutex,m_dwTimeout) == WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			code = sqlite3_bind_blob(pst,1,pData,nSize,0);
			if(code == SQLITE_OK)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_DONE)
				{
					bRet = TRUE;
				}
				else
				{
					Err(_T("!!CSQliteDataSvc::SaveConfig, sqlite3_step, err:%d\n"), code);
				}
			}
			else
			{
				Err(_T("!!CSQliteDataSvc::SaveConfig, sqlite3_bind_blob, err:%d\n"), code);
			}

// 			code = sqlite3_clear_bindings(pst);
// 			if (code!=SQLITE_OK)
// 			{
// 				Err(_T("!!CSQliteDataSvc::SaveConfig, sqlite3_clear_bindings, err:%d\n"), code);
// 			}
			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::SaveConfig, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::SaveConfig, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::SaveConfig, get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::SaveConfig, get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::GetConfigVal(CString sName,CString& sVal)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;
	char szSQL[1024] = {0};

	sprintf_s(szSQL,sizeof(szSQL)," select val from t_c_LocalConfig where name = '%s'; ",sName);
	

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					char* pVal = (char*)sqlite3_column_text(pst,0);
					sVal.Format(_T("%s"),pVal);

					bRet = TRUE;
					break;
				}
				else 
				{
					Err(_T("get_value, step, err:%d\n"), code);
					break;
				}
			}

			sqlite3_finalize(pst);
			bRet = TRUE;


		}
		else
		{
			Err(_T("get_value, prepare, err:%d\n"), code);
		}

		
		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::GetConfigData(CString sName,CBuffer* pBuf)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select data from t_c_LocalConfig where name = '%s'; ",sName);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					const void* pTmp = sqlite3_column_blob(pst,0);
					int nSize = sqlite3_column_bytes(pst,0);
					pBuf->AddData((BYTE*)pTmp,nSize);
					bRet = TRUE;
					break;
				}
				else 
				{
					Err(_T("get_value, step, err:%d\n"), code);
					break;
				}
			}

			sqlite3_finalize(pst);
			bRet = TRUE;


		}
		else
		{
			Err(_T("get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::GetConfigData(CString sName,BYTE*& pData,int& nSize)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select data from t_c_LocalConfig where name = '%s'; ",sName);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					const void* pTmp = sqlite3_column_blob(pst,0);
					nSize = sqlite3_column_bytes(pst,0);
					pData = new BYTE[nSize];
					ASSERT(pData);
					if (pData)
					{
						CopyMemory(pData,pTmp,nSize);
					}
					bRet = TRUE;
					break;
				}
				else 
				{
					Err(_T("get_value, step, err:%d\n"), code);
					break;
				}
			}

			sqlite3_finalize(pst);
			bRet = TRUE;


		}
		else
		{
			Err(_T("get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::GetEquipInfo(EQUIP_INFO* pInfo)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	ASSERT(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select data from t_c_LocalConfig where name = '%s'; ","equip");

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					const void* pTmp = sqlite3_column_blob(pst,0);
					int nSize = sqlite3_column_bytes(pst,0);
					CopyMemory(pInfo,pTmp,nSize);
					bRet = TRUE;
					break;
				}
				else 
				{
					Err(_T("!!CSQliteDataSvc::GetEquipInfo,get_value, sqlite3_step, err:%d\n"), code);
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetEquipInfo, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetEquipInfo, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetEquipInfo,get_value, sqlite3_prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetEquipInfo,get_value, wait, err:%d\n"), GetLastError());
	}

	return bRet;
}
BOOL CSQliteDataSvc::GetProductInfo(CString sName, ProductInfo* pInfo)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	ASSERT(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select data from t_c_LocalConfig where name = '%s'; ",sName);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					const void* pTmp = sqlite3_column_blob(pst,0);
					int nSize = sqlite3_column_bytes(pst,0);
					pInfo->ParseFromArray((BYTE*)pTmp,nSize);
					bRet = TRUE;
					break;
				}
				else 
				{
					if (code != SQLITE_DONE)
					{
						Err(_T("!!CSQliteDataSvc::GetProductInfo,get_value, sqlite3_step, err:%d\n"), code);
					}

					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetProductInfo, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetProductInfo, sqlite3_finalize, err:%d\n"), code);
			}
			bRet = TRUE;
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetProductInfo,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetProductInfo,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::AddOnePrinter(PPrinterInfo pInfo)
{
	if (!pInfo)
	{
		Err(_T("!!CSQliteDataSvc::AddOnePrinter,pInfo=%p\n"), pInfo);
		return FALSE;
	}

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," insert into t_c_CloudPrintStation("
		"PrinterName, Brand, Manufacturer, Manufacturer2, IPAddress, MAC, Port, Status, StatusDesc, "	//1-9
		"StationId, Model, EnableCloudPrint, EnableCloudOPS, QrCodePath, DNS, SerialNumber, ServiceTag, "	//10-17
		"Location, Type, TypeDesc, ColorCapable, ProtocolVersion, PINCapable, PINEnabled, EquipId, EntId, " //18-26
		"EntKey, EntName, PrinterCode, Flag, DoublePrintCapable, DPI, PPM) "	//27-33
		"values("
		"'%s',%d,'%s','%s','%s','%s','%s',%d,'%s',"	//1-9
		"%d,'%s',%d,%d,'%s','%s','%s','%s',"	//10-17
		"'%s',%d,'%s',%d,'%s',%d,%d,%d,%d,"		//18-26
		"'%s','%s','%s','%s',%d,%d,%d); "	//27-33
		,pInfo->cPrinterName, pInfo->nBrand, pInfo->cManufacturer, pInfo->cManufacturer2, pInfo->cIPAddress, pInfo->cMAC, pInfo->cPort, pInfo->dwStatus, pInfo->cStatus
		,pInfo->nStationId, pInfo->cModel, pInfo->bEnableCloudPrint, pInfo->bEnableCloudOPS, pInfo->cQrCodePath, pInfo->cDNS, pInfo->cSerialNumber, pInfo->cServiceTag
		,pInfo->cLocation, pInfo->nType, pInfo->cType, pInfo->bColorCapable, pInfo->cProtocolVersion, pInfo->bPINCapable, pInfo->bPINEnabled, pInfo->nEquipId, pInfo->nEntId
		,pInfo->cEntKey, pInfo->cEntName, pInfo->cPrinterCode, pInfo->cFlag, pInfo->bDoublePrintCapable, (int)pInfo->fDPI, pInfo->nPPM);

	if (!ExecuteSQL(szSQL))
	{
		Err(_T("!!CSQliteDataSvc::AddOnePrinter,fail,szSQL=%s\n"), szSQL);
		return FALSE;
	}

	return TRUE;
}

BOOL CSQliteDataSvc::GetOnePrinter(PPrinterInfo pInfo, CString szFlag)
{
	if (!pInfo)
	{
		Err(_T("!!CSQliteDataSvc::GetOnePrinter,pInfo=%p\n"), pInfo);
		return FALSE;
	}

	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select Flag, "	//0
		"PrinterName, "		//1
		"Brand, "			//2
		"Manufacturer, "	//3
		"Manufacturer2, "	//4
		"IPAddress, "		//5
		"MAC, "				//6
		"Port, "			//7
		"Status, "			//8
		"StatusDesc, "		//9
		"StationId, "		//10
		"Model, "			//11
		"EnableCloudPrint, "//12
		"EnableCloudOPS, "	//13
		"QrCodePath, "		//14
		"DNS, "				//15
		"SerialNumber, "	//16
		"ServiceTag, "		//17
		"Location, "		//18
		"Type, "			//19
		"TypeDesc, "		//20
		"ColorCapable, "	//21
		"ProtocolVersion, "	//22
		"PINCapable, "		//23
		"PINEnabled, "		//24
		"EquipId, "			//25
		"EntId, "			//26
		"EntKey, "			//27
		"EntName, "			//28
		"PrinterCode, "		//29
		"DoublePrintCapable, "	//30
		"DPI, "				//31
		"PPM "				//32
		"from t_c_CloudPrintStation where Flag='%s'; ", szFlag);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					const char *pText = (const char *)sqlite3_column_text(pst, 0);
					if (pText)
						strcpy(pInfo->cFlag, pText);
					pText = (const char *)sqlite3_column_text(pst, 1);
					if (pText)
						strcpy(pInfo->cPrinterName, pText);
					pInfo->nBrand = sqlite3_column_int(pst, 2);
					pText = (const char *)sqlite3_column_text(pst, 3);
					if (pText)
						strcpy(pInfo->cManufacturer, pText);
					pText = (const char *)sqlite3_column_text(pst, 4);
					if (pText)
						strcpy(pInfo->cManufacturer2, pText);
					pText = (const char *)sqlite3_column_text(pst, 5);
					if (pText)
						strcpy(pInfo->cIPAddress, pText);
					pText = (const char *)sqlite3_column_text(pst, 6);
					if (pText)
						strcpy(pInfo->cMAC, pText);
					pText = (const char *)sqlite3_column_text(pst, 7);
					if (pText)
						strcpy(pInfo->cPort, pText);
					pInfo->dwStatus = sqlite3_column_int(pst, 8);
					pText = (const char *)sqlite3_column_text(pst, 9);
					if (pText)
						strcpy(pInfo->cStatus, pText);
					pInfo->nStationId = sqlite3_column_int(pst, 10);
					pText = (const char *)sqlite3_column_text(pst, 11);
					if (pText)
						strcpy(pInfo->cModel, pText);
					pInfo->bEnableCloudPrint = sqlite3_column_int(pst, 12);
					pInfo->bEnableCloudOPS = sqlite3_column_int(pst, 13);
					pText = (const char *)sqlite3_column_text(pst, 14);
					if (pText)
						strcpy(pInfo->cQrCodePath, pText);
					pText = (const char *)sqlite3_column_text(pst, 15);
					if (pText)
						strcpy(pInfo->cDNS, pText);
					pText = (const char *)sqlite3_column_text(pst, 16);
					if (pText)
						strcpy(pInfo->cSerialNumber, pText);
					pText = (const char *)sqlite3_column_text(pst, 17);
					if (pText)
						strcpy(pInfo->cServiceTag, pText);
					pText = (const char *)sqlite3_column_text(pst, 18);
					if (pText)
						strcpy(pInfo->cLocation, pText);
					pInfo->nType = sqlite3_column_int(pst, 19);
					pText = (const char *)sqlite3_column_text(pst, 20);
					if (pText)
						strcpy(pInfo->cType, pText);
					pInfo->bColorCapable = sqlite3_column_int(pst, 21);
					pText = (const char *)sqlite3_column_text(pst, 22);
					if (pText)
						strcpy(pInfo->cProtocolVersion, pText);
					pInfo->bPINCapable = sqlite3_column_int(pst, 23);
					pInfo->bPINEnabled = sqlite3_column_int(pst, 24);
					pInfo->nEquipId = sqlite3_column_int(pst, 25);
					pInfo->nEntId = sqlite3_column_int(pst, 26);
					pText = (const char *)sqlite3_column_text(pst, 27);
					if (pText)
						strcpy(pInfo->cEntKey, pText);
					pText = (const char *)sqlite3_column_text(pst, 28);
					if (pText)
						strcpy(pInfo->cEntName, pText);
					pText = (const char *)sqlite3_column_text(pst, 29);
					if (pText)
						strcpy(pInfo->cPrinterCode, pText);
					pInfo->bDoublePrintCapable = sqlite3_column_int(pst, 30);
					pInfo->fDPI = sqlite3_column_int(pst, 31);
					pInfo->nPPM = sqlite3_column_int(pst, 32);

					bRet = TRUE;
					break;	//只有一个
				}
				else 
				{
					if (code != SQLITE_DONE)
					{
						Err(_T("!!CSQliteDataSvc::GetOnePrinter,get_value, sqlite3_step, err:%d\n"), code);
					}
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetOnePrinter, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetOnePrinter, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetOnePrinter,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetOnePrinter,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::RemoveOnePrinter(CString szFlag)
{
	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," delete from t_c_CloudPrintStation where Flag='%s'; ", szFlag);

	if (!ExecuteSQL(szSQL))
	{
		Err(_T("!!CSQliteDataSvc::RemoveOnePrinter,fail,szSQL=%s\n"), szSQL);
		return FALSE;
	}

	return TRUE;
}

BOOL CSQliteDataSvc::UpdateOnePrinter(PPrinterInfo pInfo)
{
	if (!pInfo)
	{
		Err(_T("!!CSQliteDataSvc::UpdateOnePrinter,pInfo=%p\n"), pInfo);
		return FALSE;
	}

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," update t_c_CloudPrintStation "
		"set PrinterName='%s', Brand=%d, Manufacturer='%s', Manufacturer2='%s', IPAddress='%s', MAC='%s', Port='%s', Status=%d, StatusDesc='%s', StationId=%d, "	//1-10
		"Flag='%s', Model='%s', EnableCloudPrint=%d, EnableCloudOPS=%d, QrCodePath='%s', DNS='%s', SerialNumber='%s', ServiceTag='%s', "	//11-18
		"Location='%s', Type=%d, TypeDesc='%s', ColorCapable=%d, ProtocolVersion='%s', PINCapable=%d, PINEnabled=%d, EquipId=%d, EntId=%d, EntKey='%s', "	//19-28
		"EntName='%s', PrinterCode='%s', DoublePrintCapable=%d, DPI=%d, PPM=%d "	//29-33
		"where Flag='%s'; "	//34
		,pInfo->cPrinterName, pInfo->nBrand, pInfo->cManufacturer, pInfo->cManufacturer2, pInfo->cIPAddress, pInfo->cMAC, pInfo->cPort, pInfo->dwStatus, pInfo->cStatus, pInfo->nStationId
		,pInfo->cFlag, pInfo->cModel, pInfo->bEnableCloudPrint, pInfo->bEnableCloudOPS, pInfo->cQrCodePath, pInfo->cDNS, pInfo->cSerialNumber, pInfo->cServiceTag
		,pInfo->cLocation, pInfo->nType, pInfo->cType, pInfo->bColorCapable, pInfo->cProtocolVersion, pInfo->bPINCapable, pInfo->bPINEnabled, pInfo->nEquipId, pInfo->nEntId, pInfo->cEntKey
		,pInfo->cEntName, pInfo->cPrinterCode, pInfo->bDoublePrintCapable, (int)pInfo->fDPI, pInfo->nPPM
		,pInfo->cFlag);

	if (!ExecuteSQL(szSQL))
	{
		Err(_T("!!CSQliteDataSvc::UpdateOnePrinter,fail\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CSQliteDataSvc::GetAllPrinter(CMapStringToPtr& PrinterMap)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select Flag, "	//0
		"PrinterName, "		//1
		"Brand, "			//2
		"Manufacturer, "	//3
		"Manufacturer2, "	//4
		"IPAddress, "		//5
		"MAC, "				//6
		"Port, "			//7
		"Status, "			//8
		"StatusDesc, "		//9
		"StationId, "		//10
		"Model, "			//11
		"EnableCloudPrint, "//12
		"EnableCloudOPS, "	//13
		"QrCodePath, "		//14
		"DNS, "				//15
		"SerialNumber, "	//16
		"ServiceTag, "		//17
		"Location, "		//18
		"Type, "			//19
		"TypeDesc, "		//20
		"ColorCapable, "	//21
		"ProtocolVersion, "	//22
		"PINCapable, "		//23
		"PINEnabled, "		//24
		"EquipId, "			//25
		"EntId, "			//26
		"EntKey, "			//27
		"EntName, "			//28
		"PrinterCode, "		//29
		"DoublePrintCapable, "	//30
		"DPI, "				//31
		"PPM "				//32
		"from t_c_CloudPrintStation; ");

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					PrinterInfo* pInfo = new PrinterInfo;
					memset(pInfo, 0x0, sizeof(PrinterInfo));
					const char *pText = (const char *)sqlite3_column_text(pst, 0);
					if (pText)
						strcpy(pInfo->cFlag, pText);
					pText = (const char *)sqlite3_column_text(pst, 1);
					if (pText)
						strcpy(pInfo->cPrinterName, pText);
					pInfo->nBrand = sqlite3_column_int(pst, 2);
					pText = (const char *)sqlite3_column_text(pst, 3);
					if (pText)
						strcpy(pInfo->cManufacturer, pText);
					pText = (const char *)sqlite3_column_text(pst, 4);
					if (pText)
						strcpy(pInfo->cManufacturer2, pText);
					pText = (const char *)sqlite3_column_text(pst, 5);
					if (pText)
						strcpy(pInfo->cIPAddress, pText);
					pText = (const char *)sqlite3_column_text(pst, 6);
					if (pText)
						strcpy(pInfo->cMAC, pText);
					pText = (const char *)sqlite3_column_text(pst, 7);
					if (pText)
						strcpy(pInfo->cPort, pText);
					pInfo->dwStatus = sqlite3_column_int(pst, 8);
					pText = (const char *)sqlite3_column_text(pst, 9);
					if (pText)
						strcpy(pInfo->cStatus, pText);
					pInfo->nStationId = sqlite3_column_int(pst, 10);
					pText = (const char *)sqlite3_column_text(pst, 11);
					if (pText)
						strcpy(pInfo->cModel, pText);
					pInfo->bEnableCloudPrint = sqlite3_column_int(pst, 12);
					pInfo->bEnableCloudOPS = sqlite3_column_int(pst, 13);
					pText = (const char *)sqlite3_column_text(pst, 14);
					if (pText)
						strcpy(pInfo->cQrCodePath, pText);
					pText = (const char *)sqlite3_column_text(pst, 15);
					if (pText)
						strcpy(pInfo->cDNS, pText);
					pText = (const char *)sqlite3_column_text(pst, 16);
					if (pText)
						strcpy(pInfo->cSerialNumber, pText);
					pText = (const char *)sqlite3_column_text(pst, 17);
					if (pText)
						strcpy(pInfo->cServiceTag, pText);
					pText = (const char *)sqlite3_column_text(pst, 18);
					if (pText)
						strcpy(pInfo->cLocation, pText);
					pInfo->nType = sqlite3_column_int(pst, 19);
					pText = (const char *)sqlite3_column_text(pst, 20);
					if (pText)
						strcpy(pInfo->cType, pText);
					pInfo->bColorCapable = sqlite3_column_int(pst, 21);
					pText = (const char *)sqlite3_column_text(pst, 22);
					if (pText)
						strcpy(pInfo->cProtocolVersion, pText);
					pInfo->bPINCapable = sqlite3_column_int(pst, 23);
					pInfo->bPINEnabled = sqlite3_column_int(pst, 24);
					pInfo->nEquipId = sqlite3_column_int(pst, 25);
					pInfo->nEntId = sqlite3_column_int(pst, 26);
					pText = (const char *)sqlite3_column_text(pst, 27);
					if (pText)
						strcpy(pInfo->cEntKey, pText);
					pText = (const char *)sqlite3_column_text(pst, 28);
					if (pText)
						strcpy(pInfo->cEntName, pText);
					pText = (const char *)sqlite3_column_text(pst, 29);
					if (pText)
						strcpy(pInfo->cPrinterCode, pText);
					pInfo->bDoublePrintCapable = sqlite3_column_int(pst, 30);
					pInfo->fDPI = sqlite3_column_int(pst, 31);
					pInfo->nPPM = sqlite3_column_int(pst, 32);

					PrinterMap.SetAt(pInfo->cFlag, pInfo);
					bRet = TRUE;
				}
				else 
				{
					if (code != SQLITE_DONE)
					{
						Err(_T("!!CSQliteDataSvc::GetAllPrinter,get_value, sqlite3_step, err:%d\n"), code);
					}
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetAllPrinter, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetAllPrinter, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetAllPrinter,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetAllPrinter,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

//获取指定nStationId的打印机信息
BOOL CSQliteDataSvc::GetOnePrinterById(PPrinterInfo pInfo, int nStationId)
{
	if (!pInfo)
	{
		Err(_T("!!CSQliteDataSvc::GetOnePrinterById,pInfo=%p\n"), pInfo);
		return FALSE;
	}

	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select Flag, "	//0
		"PrinterName, "		//1
		"Brand, "			//2
		"Manufacturer, "	//3
		"Manufacturer2, "	//4
		"IPAddress, "		//5
		"MAC, "				//6
		"Port, "			//7
		"Status, "			//8
		"StatusDesc, "		//9
		"StationId, "		//10
		"Model, "			//11
		"EnableCloudPrint, "//12
		"EnableCloudOPS, "	//13
		"QrCodePath, "		//14
		"DNS, "				//15
		"SerialNumber, "	//16
		"ServiceTag, "		//17
		"Location, "		//18
		"Type, "			//19
		"TypeDesc, "		//20
		"ColorCapable, "	//21
		"ProtocolVersion, "	//22
		"PINCapable, "		//23
		"PINEnabled, "		//24
		"EquipId, "			//25
		"EntId, "			//26
		"EntKey, "			//27
		"EntName, "			//28
		"PrinterCode, "		//29
		"DoublePrintCapable, "	//30
		"DPI, "				//31
		"PPM "				//32
		"from t_c_CloudPrintStation where StationId=%d; ", nStationId);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					const char *pText = (const char *)sqlite3_column_text(pst, 0);
					if (pText)
						strcpy(pInfo->cFlag, pText);
					pText = (const char *)sqlite3_column_text(pst, 1);
					if (pText)
						strcpy(pInfo->cPrinterName, pText);
					pInfo->nBrand = sqlite3_column_int(pst, 2);
					pText = (const char *)sqlite3_column_text(pst, 3);
					if (pText)
						strcpy(pInfo->cManufacturer, pText);
					pText = (const char *)sqlite3_column_text(pst, 4);
					if (pText)
						strcpy(pInfo->cManufacturer2, pText);
					pText = (const char *)sqlite3_column_text(pst, 5);
					if (pText)
						strcpy(pInfo->cIPAddress, pText);
					pText = (const char *)sqlite3_column_text(pst, 6);
					if (pText)
						strcpy(pInfo->cMAC, pText);
					pText = (const char *)sqlite3_column_text(pst, 7);
					if (pText)
						strcpy(pInfo->cPort, pText);
					pInfo->dwStatus = sqlite3_column_int(pst, 8);
					pText = (const char *)sqlite3_column_text(pst, 9);
					if (pText)
						strcpy(pInfo->cStatus, pText);
					pInfo->nStationId = sqlite3_column_int(pst, 10);
					pText = (const char *)sqlite3_column_text(pst, 11);
					if (pText)
						strcpy(pInfo->cModel, pText);
					pInfo->bEnableCloudPrint = sqlite3_column_int(pst, 12);
					pInfo->bEnableCloudOPS = sqlite3_column_int(pst, 13);
					pText = (const char *)sqlite3_column_text(pst, 14);
					if (pText)
						strcpy(pInfo->cQrCodePath, pText);
					pText = (const char *)sqlite3_column_text(pst, 15);
					if (pText)
						strcpy(pInfo->cDNS, pText);
					pText = (const char *)sqlite3_column_text(pst, 16);
					if (pText)
						strcpy(pInfo->cSerialNumber, pText);
					pText = (const char *)sqlite3_column_text(pst, 17);
					if (pText)
						strcpy(pInfo->cServiceTag, pText);
					pText = (const char *)sqlite3_column_text(pst, 18);
					if (pText)
						strcpy(pInfo->cLocation, pText);
					pInfo->nType = sqlite3_column_int(pst, 19);
					pText = (const char *)sqlite3_column_text(pst, 20);
					if (pText)
						strcpy(pInfo->cType, pText);
					pInfo->bColorCapable = sqlite3_column_int(pst, 21);
					pText = (const char *)sqlite3_column_text(pst, 22);
					if (pText)
						strcpy(pInfo->cProtocolVersion, pText);
					pInfo->bPINCapable = sqlite3_column_int(pst, 23);
					pInfo->bPINEnabled = sqlite3_column_int(pst, 24);
					pInfo->nEquipId = sqlite3_column_int(pst, 25);
					pInfo->nEntId = sqlite3_column_int(pst, 26);
					pText = (const char *)sqlite3_column_text(pst, 27);
					if (pText)
						strcpy(pInfo->cEntKey, pText);
					pText = (const char *)sqlite3_column_text(pst, 28);
					if (pText)
						strcpy(pInfo->cEntName, pText);
					pText = (const char *)sqlite3_column_text(pst, 29);
					if (pText)
						strcpy(pInfo->cPrinterCode, pText);
					pInfo->bDoublePrintCapable = sqlite3_column_int(pst, 30);
					pInfo->fDPI = sqlite3_column_int(pst, 31);
					pInfo->nPPM = sqlite3_column_int(pst, 32);

					bRet = TRUE;
					break;	//只有一个
				}
				else 
				{
					if (code != SQLITE_DONE)
					{
						Err(_T("!!CSQliteDataSvc::GetOnePrinterById,get_value, sqlite3_step, err:%d\n"), code);
					}
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetOnePrinterById, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::GetOnePrinterById, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::GetOnePrinter,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::GetOnePrinterById,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

//更新指定nStationId的打印机信息
BOOL CSQliteDataSvc::UpdateOnePrinterById(PPrinterInfo pInfo, int nStationId)
{
	if (!pInfo)
	{
		Err(_T("!!CSQliteDataSvc::UpdateOnePrinterById,pInfo=%p\n"), pInfo);
		return FALSE;
	}

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," update t_c_CloudPrintStation "
		"set PrinterName='%s', Brand=%d, Manufacturer='%s', Manufacturer2='%s', IPAddress='%s', MAC='%s', Port='%s', Status=%d, StatusDesc='%s', StationId=%d, "	//1-10
		"Flag='%s', Model='%s', EnableCloudPrint=%d, EnableCloudOPS=%d, QrCodePath='%s', DNS='%s', SerialNumber='%s', ServiceTag='%s', "	//11-18
		"Location='%s', Type=%d, TypeDesc='%s', ColorCapable=%d, ProtocolVersion='%s', PINCapable=%d, PINEnabled=%d, EquipId=%d, EntId=%d, EntKey='%s', "	//19-28
		"EntName='%s', PrinterCode='%s', DoublePrintCapable=%d, DPI=%d, PPM=%d " //29-33
		"where StationId=%d; "	//31
		,pInfo->cPrinterName, pInfo->nBrand, pInfo->cManufacturer, pInfo->cManufacturer2, pInfo->cIPAddress, pInfo->cMAC, pInfo->cPort, pInfo->dwStatus, pInfo->cStatus, pInfo->nStationId
		,pInfo->cFlag, pInfo->cModel, pInfo->bEnableCloudPrint, pInfo->bEnableCloudOPS, pInfo->cQrCodePath, pInfo->cDNS, pInfo->cSerialNumber, pInfo->cServiceTag
		,pInfo->cLocation, pInfo->nType, pInfo->cType, pInfo->bColorCapable, pInfo->cProtocolVersion, pInfo->bPINCapable, pInfo->bPINEnabled, pInfo->nEquipId, pInfo->nEntId, pInfo->cEntKey
		,pInfo->cEntName, pInfo->cPrinterCode, pInfo->bDoublePrintCapable, (int)pInfo->fDPI, pInfo->nPPM
		,nStationId);

	if (!ExecuteSQL(szSQL))
	{
		Err(_T("!!CSQliteDataSvc::UpdateOnePrinterById,fail\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CSQliteDataSvc::IsTableExist(CString szTableName)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," SELECT COUNT(*) FROM sqlite_master where type='table' and name='%s'; ", szTableName);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					int nCount = sqlite3_column_int(pst, 0);
					if (nCount > 0)
					{
						bRet = TRUE;
					}
				}
				else 
				{
					if (code != SQLITE_DONE)
					{
						Err(_T("!!CSQliteDataSvc::IsTableExist,get_value, sqlite3_step, err:%d\n"), code);
					}
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::IsTableExist, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::IsTableExist, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::IsTableExist,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::IsTableExist,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

BOOL CSQliteDataSvc::IsColumnExist(CString szTableName, CString szColumn)
{
	if(!Connect() /*|| !pCall*/)
		return FALSE;

	BOOL bRet = FALSE;
	sqlite3_stmt* pst = NULL;
	const char* pc = NULL;

	char szSQL[1024] = {0};
	sprintf_s(szSQL,sizeof(szSQL)," select sql from sqlite_master where type='table' and tbl_name='%s'; ", szTableName);

	if(WaitForSingleObject(m_hMutex,m_dwTimeout)==WAIT_OBJECT_0) 
	{
		int code=sqlite3_prepare(m_hDB, szSQL, strlen(szSQL)*sizeof(CHAR), &pst, &pc);
		if(code==SQLITE_OK) 
		{
			while (1)
			{
				code = sqlite3_step(pst);
				if(code == SQLITE_ROW) 
				{
					CString szRet = "";
					const char *pText = (const char *)sqlite3_column_text(pst, 0);
					if (pText)
						szRet = pText;

					szRet.MakeUpper();
					szColumn.MakeUpper();
					if (szRet.Find(szColumn) >= 0)
					{
						bRet = TRUE;
					}

					//只取第一行
					break;
				}
				else 
				{
					if (code != SQLITE_DONE)
					{
						Err(_T("!!CSQliteDataSvc::IsColumnExist,get_value, sqlite3_step, err:%d\n"), code);
					}
					break;
				}
			}

			code = sqlite3_reset(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::IsColumnExist, sqlite3_reset, err:%d\n"), code);
			}
			code = sqlite3_finalize(pst);
			if (code!=SQLITE_OK)
			{
				Err(_T("!!CSQliteDataSvc::IsColumnExist, sqlite3_finalize, err:%d\n"), code);
			}
		}
		else
		{
			Err(_T("!!CSQliteDataSvc::IsColumnExist,get_value, prepare, err:%d\n"), code);
		}

		ReleaseMutex(m_hMutex);
	}
	else
	{
		Err(_T("!!CSQliteDataSvc::IsColumnExist,get_value, WaitForSingleObject, err:%d\n"), GetLastError());
	}

	return bRet;
}

void CSQliteDataSvc::CheckCloudDbTables()
{
	if (IsTableExist("t_c_CloudPrintStation"))
	{
		//表[t_c_CloudPrintStation]添加EquipId
		if (!IsColumnExist("t_c_CloudPrintStation", "EquipId"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [EquipId] INTEGER;");
		}
		
		//表[t_c_CloudPrintStation]添加EntId
		if (!IsColumnExist("t_c_CloudPrintStation", "EntId"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [EntId] INTEGER;");
		}

		//表[t_c_CloudPrintStation]添加EntKey
		if (!IsColumnExist("t_c_CloudPrintStation", "EntKey"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [EntKey] VARCHAR(20);");
		}

		//表[t_c_CloudPrintStation]添加EntName
		if (!IsColumnExist("t_c_CloudPrintStation", "EntName"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [EntName] VARCHAR(100);");
		}

		//表[t_c_CloudPrintStation]添加PrinterCode
		if (!IsColumnExist("t_c_CloudPrintStation", "PrinterCode"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [PrinterCode] VARCHAR(50);");
		}

		//表[t_c_CloudPrintStation]添加DoublePrintCapable
		if (!IsColumnExist("t_c_CloudPrintStation", "DoublePrintCapable"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [DoublePrintCapable] INTEGER;");
		}

		//表[t_c_CloudPrintStation]添加DPI
		if (!IsColumnExist("t_c_CloudPrintStation", "DPI"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [DPI] INTEGER;");
		}

		//表[t_c_CloudPrintStation]添加PPM
		if (!IsColumnExist("t_c_CloudPrintStation", "PPM"))
		{
			ExecuteSQL("ALTER TABLE [t_c_CloudPrintStation] ADD COLUMN [PPM] INTEGER;");
		}
	}
}
