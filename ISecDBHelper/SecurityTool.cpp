// SecurityTool.cpp: implementation of the CSecurityTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecurityTool.h"
#include "ProcessExp.h"
#include <Tlhelp32.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSecurityTool::CSecurityTool()
{

}

CSecurityTool::~CSecurityTool()
{

}

//author: yy2better
//email: yy2better@126.com
//得到当前进程用户
BOOL CSecurityTool::GetCurrProcessUser(CString& strName)
{	
	BOOL bRet(TRUE);
	strName = _T("");

	DWORD dwSize = MAX_PATH;
	TCHAR *pszName = new TCHAR[dwSize];
	if (!GetUserName(pszName, &dwSize))
	{
		//if (ERROR_MORE_DATA == GetLastError())	错误码不是这个，MSDN说明有误
		delete[] pszName;
		pszName = new TCHAR[dwSize];
		bRet = GetUserName(pszName, &dwSize);
	}
	
	strName = pszName;
	delete[] pszName;
		
	return bRet;
}


//获取XP登陆用户
BOOL CSecurityTool::GetLogUserXP(CString& strName)
{
	BOOL bRet = FALSE;
	strName = _T("");
/*
	//for xp or above
	TCHAR *szLogName = NULL;
	DWORD dwSize = 0;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSUserName, &szLogName, &dwSize))
	{			
		strName = szLogName;
		WTSFreeMemory(szLogName);
		bRet = TRUE;
	}
	int nError = GetLastError();
*/
	return bRet;
}


//获取win2000登陆用户
BOOL CSecurityTool::GetLogUser2K(CString& strName)
{
	BOOL bRet = FALSE;
	HANDLE hSnapshot = NULL;
	strName = _T("");

    __try
	{
		// Get a snapshot of the processes in the system
        hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == NULL)
		{            
			__leave;
		}

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(pe32);

        // Find the "System" process
        BOOL fProcess = Process32First(hSnapshot, &pe32);
        while (fProcess)
		{
			if (lstrcmpi(pe32.szExeFile, TEXT("explorer.exe")) == 0)
			{	
				TCHAR szUserName[MAX_PATH];
				if (GetProcessUser(pe32.th32ProcessID, szUserName, MAX_PATH))
				{
					bRet = TRUE;
					strName = szUserName;
				}
				
				break;
			}
			fProcess = Process32Next(hSnapshot, &pe32);
		}
        if (!fProcess)
		{			
            __leave;    // Didn't find "System" process
		}
	}
    __finally
	{
		// Cleanup the snapshot
       if (hSnapshot != NULL)
		   CloseHandle(hSnapshot);
    }

	return bRet;	
}

//author: yy2better
//email: yy2better@126.com
//获取进程的用户名
BOOL CSecurityTool::GetProcessUser(DWORD dwProcessID, TCHAR *szUserName, DWORD nNameLen)
{
	BOOL fResult  = FALSE;
    HANDLE hProc  = NULL;
	HANDLE hToken = NULL;
	TOKEN_USER *pTokenUser = NULL;
	
	__try
	{
        // Open the process with PROCESS_QUERY_INFORMATION access
        hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
        if (hProc == NULL)
		{
			__leave;
		}
        fResult = OpenProcessToken(hProc, TOKEN_QUERY, &hToken);
        if(!fResult)  
		{
			__leave;
		}
		
		DWORD dwNeedLen = 0;		
		fResult = GetTokenInformation(hToken,TokenUser, NULL, 0, &dwNeedLen);
		if (dwNeedLen > 0)
		{
			pTokenUser = (TOKEN_USER*)new BYTE[dwNeedLen];
			fResult = GetTokenInformation(hToken,TokenUser, pTokenUser, dwNeedLen, &dwNeedLen);
			if (!fResult)
			{
				__leave;
			}
		}
		else
		{
			__leave;
		}

		SID_NAME_USE sn;
		TCHAR szDomainName[MAX_PATH];
		DWORD dwDmLen = MAX_PATH;
		fResult = LookupAccountSid(NULL, pTokenUser->User.Sid, szUserName, &nNameLen,
			szDomainName, &dwDmLen, &sn);
	}
	__finally
	{
		if (hProc)
			::CloseHandle(hProc);
		if (hToken)
			::CloseHandle(hToken);
		if (pTokenUser)
			delete[] (char*)pTokenUser;

		return fResult;
	}
}


BOOL CSecurityTool::ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken    = NULL;

	// Open the process token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken ))
	{
		TRACE("Failed OpenProcessToken\n");
		return ERROR_FUNCTION_FAILED;
	}

	// Get the local unique id for the privilege.
	if ( !LookupPrivilegeValue( NULL,
		szPrivilege,
		&luid ))
	{
		CloseHandle( hToken );
		TRACE("Failed LookupPrivilegeValue\n");
		return ERROR_FUNCTION_FAILED;
	}

	// Assign values to the TOKEN_PRIVILEGE structure.
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes = 
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);

	// Adjust the token privilege.
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		sizeof(NewState),
		NULL,
		NULL))
	{
		TRACE("Failed AdjustTokenPrivileges\n");
		hr = ERROR_FUNCTION_FAILED;
	}

	// Close the handle.
	CloseHandle(hToken);

	return hr;
}


BOOL static ReadUserEnvirnment(DWORD dwSession, _ENVSTRING_t& stEnvData)
{
	TCHAR szExplorerPath[MAX_PATH + 1];
	GetWindowsDirectory(szExplorerPath, MAX_PATH);
	strcat(szExplorerPath, "\\");
	strcat(szExplorerPath, "explorer.exe");

	BOOL bRet =  CProcessExp::ReadEnvironmentBlock(szExplorerPath, dwSession, stEnvData);

	return bRet;
}

DWORD CSecurityTool::CreateProcByCurUsr(char * pProc,char * pParam,int nShowType,DWORD dwSessionId/* = 0*/)
{
	DWORD bRet = FALSE;
	HANDLE hSnapshot = NULL;
	_ENVSTRING_t stEnvData;
	if(ModifyPrivilege(SE_DEBUG_NAME ,TRUE) != S_OK )
	{
		TRACE("ModifyPrivilege Faile");
	}
	if (dwSessionId == 0)
	{
		dwSessionId = CCommonFun::WTSGetActiveConsoleSessionIdEx();
	}
	do
	{
		// Get a snapshot of the processes in the system
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == NULL)
		{            
			break;
		}

		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);

		// Find the "System" process
		BOOL fProcess = Process32First(hSnapshot, &pe32);
		while (fProcess)
		{
			if (lstrcmpi(pe32.szExeFile, TEXT("explorer.exe")) == 0)
			{	
				DWORD dwSID = 0;
				if(ProcessIdToSessionId(pe32.th32ProcessID,&dwSID) && dwSID == dwSessionId)
				{

					void* pEn = 0;
					if (ReadUserEnvirnment(dwSessionId, stEnvData))
					{
						pEn = stEnvData.pData;
					}

					bRet=(CreateProcUseProcToken(pe32.th32ProcessID,pProc,pParam,nShowType,pEn));

					break;
				}
			}
			fProcess = Process32Next(hSnapshot, &pe32);
		}
		if (!fProcess)
		{			
			break;;    // Didn't find "System" process
		}
	}while(0);


	// Cleanup the snapshot
	if (hSnapshot != NULL)
		CloseHandle(hSnapshot);


	return bRet;
}

DWORD  CSecurityTool::CreateProcUseProcToken(DWORD dwProcessID,char * pProcName,char * pParam,int nShowType,void* pEn)
{

	DWORD fResult  = FALSE;
	HANDLE hProc  = NULL;
	HANDLE hToken = NULL,hTokenSelf=NULL;
	TOKEN_USER *pTokenUser = NULL;
	char* pszPath = NULL;
	char strMsg[256];
	__try
	{
		// Open the process with PROCESS_QUERY_INFORMATION access
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
		if (hProc == NULL)
		{
			__leave;
		}
		fResult = OpenProcessToken(hProc, TOKEN_QUERY 
			| TOKEN_ADJUST_PRIVILEGES | TOKEN_ADJUST_SESSIONID 
			| TOKEN_ADJUST_DEFAULT | TOKEN_ASSIGN_PRIMARY
			| TOKEN_DUPLICATE|TOKEN_IMPERSONATE, &hToken);
		if(!fResult)  
		{
			DWORD dw = ::GetLastError();
			__leave;
		}


		OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY 
			| TOKEN_ADJUST_PRIVILEGES | TOKEN_ADJUST_SESSIONID 
			| TOKEN_ADJUST_DEFAULT | TOKEN_ASSIGN_PRIMARY
			| TOKEN_DUPLICATE|TOKEN_IMPERSONATE, &hTokenSelf);
		TOKEN_PRIVILEGES    tp;

		//if (!LookupPrivilegeValue(NULL, SE_TCB_NAME, 
		if (!LookupPrivilegeValue(NULL, SE_ASSIGNPRIMARYTOKEN_NAME, 
			&tp.Privileges[0].Luid)) 
		{

			//	DisplayError(L"LookupPrivilegeValue");
			__leave;
		}


		// Enable the TCB Name privilege in the process token.
		tp.PrivilegeCount = 1;

		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hTokenSelf, FALSE, &tp, 0, NULL, 0)
			|| GetLastError() != ERROR_SUCCESS)
		{
			sprintf(strMsg,"ERROR=%d",GetLastError());
			//AfxMessageBox(strMsg);
			//DisplayError(L"AdjustTokenPrivileges");
			//	__leave;
		}
		CloseHandle( hTokenSelf );

		int nLen = strlen(pProcName)  + ( pParam ? strlen(pParam) : 0 ) + 10;
		pszPath = new char[nLen];
		ASSERT(pszPath);
		if(!pszPath)
		{
			__leave;
		}
		memset(pszPath,0,nLen);

		sprintf(pszPath,"%s %s",pProcName,pParam ? pParam : "");
		STARTUPINFO si = { sizeof si, 0, "" };
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = nShowType;
		PROCESS_INFORMATION pi;
		if(CreateProcessAsUser( hToken, // the security context
			0, 
			pszPath,// command line
			0, 0,  // security attributes
			FALSE, // handle inheritance
			0,     // creation flags
			0,     // environment block
			0,     // current directory
			&si, &pi ))
		{
			fResult = pi.dwProcessId;
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		int ierror=GetLastError();

		CCommonFun::WriteFileLog(" err %d",ierror);

		//CloseHandle( hToken );
	}
	__finally
	{
		if (hProc)
			::CloseHandle(hProc);
		if (hToken)
			::CloseHandle(hToken);
		if (pTokenUser)
			delete[] (char*)pTokenUser;

		if(pszPath)
		{
			delete []pszPath;
		}

	}
	return fResult;
}