#include "stdafx.h"
#include "ProcessExp.h"

CProcessExp::CProcessExp(void)
{
}

CProcessExp::~CProcessExp(void)
{
}

NTSTATUS CProcessExp::QueryInformationProcesss(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	)
{
	typedef NTSTATUS ( __stdcall *QueryInfoProcess) (
		IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL
		);

	HMODULE hModNTDll = LoadLibrary( _T("ntdll.dll" ));

	if( !hModNTDll )
	{
		CCommonFun::WriteFileLog(_T("Error Loading library"));
	}

	QueryInfoProcess QueryProcInfo = (QueryInfoProcess) GetProcAddress( hModNTDll ,
		"NtQueryInformationProcess" );
	if( !QueryProcInfo )
	{
		CCommonFun::WriteFileLog(_T("Can't find NtQueryInformationProcess in ntdll.dll"));
		return FALSE;
	}

	NTSTATUS ntStat =  QueryProcInfo( ProcessHandle,
		ProcessInformationClass, 
		ProcessInformation,
		ProcessInformationLength,
		ReturnLength );

	FreeLibrary( hModNTDll );

	return ntStat;
}

BOOL CProcessExp::ReadEnvironmentBlock(CString strProcess, DWORD dwSession, _ENVSTRING_t& stEnvData )
{
	DWORD dwID = GetProcessIdByName(strProcess, dwSession);
	HANDLE hProcess = OpenProcessToRead(dwID);
	if( hProcess )
	{
		if( !ReadEnvironmentBlock( hProcess, stEnvData ))
		{
			CloseHandle(hProcess);
			return FALSE; 
		}
	}
	CloseHandle( hProcess );
	return TRUE;
}

BOOL CProcessExp::ReadEnvironmentBlock( HANDLE hProcess, _ENVSTRING_t& stEnvData )
{
	// Buffer to hold the string read from process	
	UCHAR* pchBuffEnvString = 0;
	stEnvData.Clear();
	__try
	{
		PROCESS_BASIC_INFORMATION stBasiProcInfo = { 0 };
		ULONG uReturnLength = 0;
		NTSTATUS ntStat =  QueryInformationProcesss( hProcess,
			ProcessBasicInformation, 
			&stBasiProcInfo,
			sizeof(stBasiProcInfo),
			&uReturnLength );

		// Read the process environment block
		PEB peb = { 0 };
		SIZE_T nReturnNumBytes = 0;
		// Check read access of specified location in the processs and get the size of block
		ReadProcessMemory( hProcess,(LPCVOID)stBasiProcInfo.PebBaseAddress, &peb, sizeof(peb) , &nReturnNumBytes );	


		// Get the address of RTL_USER_PROCESS_PARAMETERS structure 
		UCHAR* puPEB = (UCHAR*)&peb;
		UCHAR* pRTLUserInfo = (UCHAR*) *((long*)(puPEB+0x10));

		int nReadbleSize = 0;
		if( !HasReadAccess( hProcess, pRTLUserInfo, nReadbleSize ))
		{
			CCommonFun::WriteFileLog(_T("Error Reading Process Memory1"));
			return FALSE;
		}

		// Get the first 0x64 bytes of RTL_USER_PROCESS_PARAMETERS strcuture
		char cBuffRTLUserInfo[0x64] = {0};
		ReadProcessMemory( hProcess,(LPCVOID)pRTLUserInfo, cBuffRTLUserInfo, 0x64 , &nReturnNumBytes );	

		// Validate the read operation
		if( !nReturnNumBytes )
		{
			CCommonFun::WriteFileLog(_T("Error Reading Process Memory2"));
			return FALSE;
		}

		// Get the value at offset 0x48 to get the pointer to environment string block
		UCHAR* pAddrEnvStrBlock = (UCHAR*) *((long*)(&cBuffRTLUserInfo[0]+0x48));

		if( !HasReadAccess( hProcess, pAddrEnvStrBlock, nReadbleSize ))
		{
			CCommonFun::WriteFileLog(_T("Error Reading Process Memory3"));
			return FALSE;
		}

		// Allocate buffer for to copy the block
		pchBuffEnvString = new UCHAR[nReadbleSize];
		memset( pchBuffEnvString, 0, sizeof( UCHAR )* nReadbleSize);

		// Read the environment block
		ReadProcessMemory( hProcess,(LPCVOID)pAddrEnvStrBlock,
			pchBuffEnvString, nReadbleSize , &nReturnNumBytes );

		// Cleanup existing data if any

		if( nReturnNumBytes )
		{
			// Set the values in the return pointer
			stEnvData.pData =  pchBuffEnvString;
			stEnvData.nSize = nReturnNumBytes;
			return TRUE;
		}
		else
		{
			//add by zfq,2015-04-29,begin
			delete []pchBuffEnvString;
			pchBuffEnvString = NULL;
			stEnvData.pData = NULL;
			stEnvData.nSize = 0;
			CCommonFun::WriteFileLog(_T("Error Reading Process Memory4"));
			return FALSE;
			//add by zfq,2015-04-29,end
		}
	}
	__except( EXCEPTION_CONTINUE_SEARCH)
	{
		CCommonFun::WriteFileLog( _T("Exception occured on reading process memory\n"));
		if( pchBuffEnvString )
			delete []pchBuffEnvString;
	}

	//add by zfq,2015-04-29,begin
	delete []pchBuffEnvString;
	pchBuffEnvString = NULL;
	stEnvData.pData = NULL;
	stEnvData.nSize = 0;
	//add by zfq,2015-04-29,end

	return FALSE;
}

HANDLE CProcessExp::OpenProcessToRead( DWORD dwPID )
{
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, dwPID );
	return hProcess;
}

BOOL CProcessExp::HasReadAccess( HANDLE hProcess,
								void* pAddress, int& nSize )
{
	MEMORY_BASIC_INFORMATION memInfo;
	__try
	{
		VirtualQueryEx( hProcess, pAddress,&memInfo,sizeof(memInfo));
		if( PAGE_NOACCESS == memInfo.Protect ||
			PAGE_EXECUTE == memInfo.Protect )
		{
			nSize = 0;
			return FALSE;
		}

		nSize = memInfo.RegionSize;
		return TRUE;
	}
	__except(EXCEPTION_CONTINUE_SEARCH)
	{
		CCommonFun::WriteFileLog( _T("Failed to close Handle"));
	}
	return FALSE;
}

DWORD CProcessExp::GetProcessIdByName(CString strExeFullPath, DWORD dwSession)
{
	strExeFullPath.MakeUpper();
	CString strExe2;
	strExe2.Format(_T("\\??\\%s"), strExeFullPath);
	BOOL bIsXp = FALSE;

	HANDLE hProcess=NULL;
	DWORD ArrayProcess[1000];
	DWORD cb=1000, cbNeeded=1000;
	CString strt;

	HINSTANCE	hLib = LoadLibrary(_T("PSAPI.DLL"));
	BOOL (WINAPI *lpEnumProcesses)(DWORD*, DWORD, DWORD*);
	lpEnumProcesses = (BOOL (WINAPI *)(DWORD*, DWORD, DWORD*))GetProcAddress(hLib, "EnumProcesses");
	if(lpEnumProcesses)
	{
		if(lpEnumProcesses(ArrayProcess, cb, &cbNeeded))
		{
			for(DWORD i = 0; i < cbNeeded; i++)
			{
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ArrayProcess[i]);
				if(hProcess)
				{
					strt = GetProcessFullPath(hProcess);
					CloseHandle(hProcess);
					strt.MakeUpper();
					if((strExeFullPath == strt)||(strExe2 == strt))
					{
						if (CCommonFun::GetProcessSessionID(ArrayProcess[i]) == dwSession)
							return ArrayProcess[i];
					}
				}

			}
		}
	}
	else
	{
		CCommonFun::WriteFileLog(_T("isNULL\n"));
	}
	return 0;	
}
CString CProcessExp::GetProcessFullPath(HANDLE hProcess)
{
	HINSTANCE	hLib = LoadLibrary(_T("PSAPI.DLL"));
	if((!hLib)||(!hProcess))
		return _T("");

	TCHAR szFilename[MAX_PATH];
	HMODULE hModule;
	DWORD	dwSize = MAX_PATH + 1;
	BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *, DWORD, LPDWORD);
	DWORD (WINAPI *lpfGetModuleFileNameEx)( HANDLE, HMODULE, LPTSTR, DWORD);			
	DWORD (WINAPI *lpfGetProcessImageFileName)(HANDLE,LPTSTR,DWORD);
	if (CCommonFun::IsWow64())
	{
		lpfGetProcessImageFileName = (DWORD(WINAPI *)(HANDLE,LPTSTR,DWORD))GetProcAddress(hLib, "GetProcessImageFileNameA") ;
		if(lpfGetProcessImageFileName)
		{
			if(lpfGetProcessImageFileName(hProcess,szFilename,dwSize))
			{
				CString str = szFilename;

				// 得到路径
				int nSize = MAX_PATH;
				TCHAR szTemp[MAX_PATH];
				szTemp[0] = '\0';
				if (GetLogicalDriveStrings(MAX_PATH - 1, szTemp)) 
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do 
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							UINT uNameLen = _tcslen(szName);
							CString sName(szName);
							if (str.Left(sName.GetLength()).Compare(sName) == 0)
							{
								str.Replace(sName,szDrive);
								bFound = TRUE;
								break;
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
				return str;	
			}
		}
	}
	else
	{
		lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *, DWORD, LPDWORD)) GetProcAddress( hLib, "EnumProcessModules" ) ;
		lpfGetModuleFileNameEx = (DWORD (WINAPI *)(HANDLE, HMODULE, LPTSTR, DWORD )) GetProcAddress( hLib, "GetModuleFileNameExA" ) ;	    
		if(lpfEnumProcessModules)
		{
			if(lpfEnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwSize))
			{
				CString str;
				if(lpfGetModuleFileNameEx)
				{
					if (lpfGetModuleFileNameEx(hProcess, hModule,szFilename, 100))
					{
						str.Format(_T("%s"),szFilename);
						str.MakeUpper();
						return str;			
					}
					else
					{
						return _T("");
					}
				}

			}	
		}
	}
	int Ierror=::GetLastError();
	return _T("");
}

BOOL CProcessExp::ProcessExist(CString strProcessName, DWORD dwSession)
{
	DWORD dwRet = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hSnapshot != INVALID_HANDLE_VALUE)
	{
		strProcessName.MakeUpper();
		PROCESSENTRY32 ProcessEntry;  
		ZeroMemory(&ProcessEntry,sizeof(PROCESSENTRY32));
		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		BOOL bFlag = Process32First(hSnapshot,&ProcessEntry);
		//TRACE("----------------checkprocess begin-----------------------------");
		while(bFlag)
		{
			CString strExePath = ProcessEntry.szExeFile;
			strExePath.MakeUpper();
			//TRACE("%s--%s\n", strExePath, strProcessName);
			if(strExePath.CompareNoCase(strProcessName) == 0)
			{
				dwRet = ProcessEntry.th32ProcessID;
				DWORD dwProcSession = 0;
				if (ProcessIdToSessionId(dwRet, &dwProcSession))
				{
					//WriteLog("Process:%s, pID:%d, session:%d", strExePath, dwRet, dwProcSession);
					//WriteLog("CheckProcess:ProcSession:%d, should be:%d",dwProcSession, dwSession);
					if (dwProcSession == dwSession)
						break;
					else
						dwRet = 0;
				}
			}
			ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
			bFlag = Process32Next(hSnapshot,&ProcessEntry);
		}
		CloseHandle(hSnapshot);
	}
	//WriteLog("----------------checkprocess end-----------------------------");
	if (dwRet == 0)
		return FALSE;
	return CCommonFun::IsProcAlive(dwRet);
}

BOOL CProcessExp::GetProcessUser(DWORD dwProcessID, CString& strUser)
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
			ASSERT(pTokenUser);
			if(!pTokenUser)
				__leave;
			memset(pTokenUser,0,dwNeedLen);
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
		DWORD nNameLen = MAX_PATH;
		TCHAR szUserName[MAX_PATH] = _T("");
		fResult = LookupAccountSid(NULL, pTokenUser->User.Sid, szUserName, &nNameLen,
			szDomainName, &dwDmLen, &sn);
		strUser = szUserName;
	}
	__finally
	{
		if (hProc)
			::CloseHandle(hProc);
		if (hToken)
			::CloseHandle(hToken);
		if (pTokenUser)
			delete[] (char*)pTokenUser;


	}
	return fResult;	
}
BOOL CProcessExp::GetProcessUser(CString strProcessName, DWORD dwSession, CString& strUser)
{
	BOOL bResult = FALSE;
	HANDLE          hToken = NULL;
	PTOKEN_USER     pTokenUser = NULL;
	char            szName[256];
	char            szDomain[256];
	DWORD           dwSize = 0 ;
	DWORD           dwName = MAX_PATH;
	DWORD           dwDomain = MAX_PATH;   
	SID_NAME_USE    SNU;

	do
	{
		if(!GetProcessToken(strProcessName, dwSession, hToken))
		{
			CCommonFun::WriteFileLog(_T("failed to get process token"));
			break;
		}
		if(!GetTokenInformation(hToken,TokenUser,pTokenUser,dwSize, &dwSize))
		{
			DWORD dwErr = GetLastError();
			//CCommonFun::WriteFileLog("GetTokenInformation, err = %d", dwErr);
			if(dwErr != ERROR_INSUFFICIENT_BUFFER)
			{
				break;
			}
		}
		pTokenUser = (PTOKEN_USER)new BYTE[dwSize];
		if(pTokenUser == NULL)
		{
			CCommonFun::WriteFileLog("pTokenUser = NULL");
			break;
		}
		if(!GetTokenInformation(hToken,TokenUser,pTokenUser,dwSize,&dwSize))
		{
			CCommonFun::WriteFileLog(_T("failed to GetTokenInformation"));
			break;
		}
		if(LookupAccountSid(NULL,pTokenUser->User.Sid,szName,&dwName,szDomain,&dwDomain,&SNU) != 0)
		{
			strUser = szName;
			bResult = TRUE;
		}
	}while(0);
	if (pTokenUser != NULL)
	{
		delete pTokenUser;
		pTokenUser = NULL;
	}
	return bResult;
}

BOOL CProcessExp::GetProcessToken(CString strProcessName, DWORD dwActiveSessionId, HANDLE& hProcessToken)
{
	BOOL bRet = FALSE;
	HANDLE hToken = NULL;
	HANDLE hSnapshot = NULL;
	do 
	{
		// Get a snapshot of the processes in the system
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == NULL)
		{    
			CCommonFun::WriteFileLog(_T("CreateToolhelp32Snapshot falie(%d)"),::GetLastError());
			break;
		}
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);
		// Find the "System" process
		BOOL fProcess = Process32First(hSnapshot, &pe32);
		while (fProcess)
		{
			if (strProcessName.CompareNoCase(pe32.szExeFile) == 0)
			{
				CCommonFun::WriteFileLog(_T("find winlogon.exe"));
				DWORD dwSessionID = -1;
				DWORD dwProcessID = pe32.th32ProcessID;

				if(ProcessIdToSessionId(dwProcessID,&dwSessionID))
				{
					if(dwSessionID == dwActiveSessionId /*GetCurrentSessionID()*/)
					{
						CCommonFun::WriteFileLog(_T("%s,%d: Session ID(%d) is equal"), strProcessName,pe32.th32ProcessID,dwSessionID);
						if (GetProcessToken(dwProcessID, hProcessToken))
						{
							bRet = TRUE;
							CCommonFun::WriteFileLog(_T("token = %x"),(DWORD)hProcessToken);


							break;
						}
						else
						{
							CCommonFun::WriteFileLog(_T("error open proc token"));
						}
					}
					else
						CCommonFun::WriteFileLog(_T("Session ID not equal, found session%d, to find Sessionid%d"), dwSessionID, dwActiveSessionId);
				}
				else
					CCommonFun::WriteFileLog(_T("ProcessIdToSessionId错误"));
			}
			CCommonFun::WriteFileLog(pe32.szExeFile);
			fProcess = Process32Next(hSnapshot, &pe32);
		}
		if (!fProcess)
		{			
			break;    
		}
	}while (0);
	// Cleanup the snapshot
	if (hSnapshot != NULL)
		CloseHandle(hSnapshot);
	return bRet;
}
BOOL CProcessExp::GetProcessToken(DWORD dwProcessID, HANDLE& hProcessToken)
{
	BOOL bRet = FALSE;
	HANDLE hToken = NULL;
	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS,false,dwProcessID);
	if(hprocess)
	{
		if( ::OpenProcessToken(hprocess,TOKEN_ALL_ACCESS, &hToken) )
		{
			bRet = TRUE;
			hProcessToken = hToken;
			return TRUE;
		}
	}
	else
		CCommonFun::WriteFileLog(_T("Open process error"));
	return FALSE;
}
LPVOID CProcessExp::WriteRemoteMemory(HANDLE hProcess, LPVOID pData, UINT nSize)
{
	LPVOID pRemote = VirtualAllocEx(hProcess, NULL, nSize,
		MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if(NULL != pRemote)
	{
		if(WriteProcessMemory(hProcess, pRemote, pData, nSize, NULL))
		{
			//不要忘记Free哦
			return pRemote;
		}
		else
		{
			CCommonFun::WriteFileLog(_T("ProcessExp:Failed to write process memory, err = %d."), GetLastError());
		}
	}
	else
	{
		CCommonFun::WriteFileLog(_T("ProcessExp:pRemote == NULL, err = %d"), GetLastError());
	}
	return NULL;
}

PTHREAD_START_ROUTINE CProcessExp::GetStartRoutine(char *szProc, char* szLibrary /*= _T("Kernel32.dll")*/)
{
	HMODULE hModule = LoadLibrary(szLibrary);
	if (hModule == NULL)
		return NULL;
	PTHREAD_START_ROUTINE pStart = (PTHREAD_START_ROUTINE)GetProcAddress(hModule, szProc);
	return pStart;
}

typedef NTSTATUS (WINAPI *LPFUN_NtCreateThreadEx) 
(
 OUT PHANDLE hThread,
 IN ACCESS_MASK DesiredAccess,
 IN LPVOID ObjectAttributes,
 IN HANDLE ProcessHandle,
 IN LPTHREAD_START_ROUTINE lpStartAddress,
 IN LPVOID lpParameter,
 IN BOOL CreateSuspended, 
 IN ULONG StackZeroBits,
 IN ULONG SizeOfStackCommit,
 IN ULONG SizeOfStackReserve,
 OUT LPVOID lpBytesBuffer
 );

//Buffer argument passed to NtCreateThreadEx function
struct NtCreateThreadExBuffer
{
	ULONG Size;
	ULONG Unknown1;
	ULONG Unknown2;
	PULONG Unknown3;
	ULONG Unknown4;
	ULONG Unknown5;
	ULONG Unknown6;
	PULONG Unknown7;
	ULONG Unknown8;
};

BOOL CProcessExp::InjectDll(DWORD dwTargetPID, char* szDllPath)
{
	int iSize = (_tcslen(szDllPath) + 1) * sizeof(char);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS/*PROCESS_VM_WRITE | PROCESS_VM_OPERATION*/,
		FALSE, dwTargetPID);
	if (hProcess == NULL)
	{
		CCommonFun::WriteFileLog(_T("handleTarget == NULL, err = %d"), GetLastError());
		return FALSE;
	}
	CCommonFun::WriteFileLog("handle = 0x%x", hProcess);
	BOOL bRet = FALSE;
	LPVOID szRemotePath = WriteRemoteMemory(hProcess, (LPVOID)szDllPath, iSize);
	if(szRemotePath != NULL)
	{
		PTHREAD_START_ROUTINE pStart = GetStartRoutine(_T("LoadLibraryA"));
		if (pStart == NULL)
		{
			CCommonFun::WriteFileLog(_T("GetProcAddress Failed."));
		}
		else
		{

			if (CCommonFun::IsWinVistaOrLater())
			{
				do 
				{
					// [1/9/2012 Songqx]
					// vista之后的跨session注入有问题，使用未公开函数 NtCreateThreadEx
					HMODULE modNtDll = GetModuleHandle("ntdll.dll"); 

					if( !modNtDll )
					{
						CCommonFun::WriteFileLog("failed to get module handle for ntdll.dll, Error=0x%.8x", GetLastError());
						break;
					}

					LPFUN_NtCreateThreadEx funNtCreateThreadEx = 
						(LPFUN_NtCreateThreadEx) GetProcAddress(modNtDll, "NtCreateThreadEx");

					if( !funNtCreateThreadEx )
					{
						CCommonFun::WriteFileLog("failed to get funtion address from ntdll.dll, Error=0x%.8x", GetLastError());
						break;
					}
					//setup and initialize the buffer
					NtCreateThreadExBuffer ntbuffer;

					memset (&ntbuffer,0,sizeof(NtCreateThreadExBuffer));
					DWORD temp1 = 0;
					DWORD temp2 = 0;

					ntbuffer.Size = sizeof(NtCreateThreadExBuffer);
					ntbuffer.Unknown1 = 0x10003;
					ntbuffer.Unknown2 = 0x8;
					ntbuffer.Unknown3 = &temp2;
					ntbuffer.Unknown4 = 0;
					ntbuffer.Unknown5 = 0x10004;
					ntbuffer.Unknown6 = 4;
					ntbuffer.Unknown7 = &temp1;
					ntbuffer.Unknown8 = 0;

					HANDLE hThread = NULL;

					NTSTATUS status = funNtCreateThreadEx( 
						&hThread, 
						0x1FFFFF, 
						NULL, 
						hProcess,
						(LPTHREAD_START_ROUTINE) pStart,
						szRemotePath,
						FALSE, //start instantly
						NULL,
						NULL,
						NULL,
						&ntbuffer
						);
					if (hThread == NULL)
					{
						CCommonFun::WriteFileLog("NtCreateThreadEx failed, Error=0x%.8x", GetLastError());
						break;
					}

					//Wait for thread to complete....
					WaitForSingleObject(hThread, INFINITE);

					//Check the return code from remote thread function 
					int dwExitCode;
					if( GetExitCodeThread(hThread, (DWORD*) &dwExitCode) )
					{
						CCommonFun::WriteFileLog("Remote thread returned with status = %d", dwExitCode);
					}

					CloseHandle(hThread);
					bRet = TRUE;
				} while (0);
			}// vista or later
			else
			{
				HANDLE hHandle = CreateRemoteThread(hProcess, NULL, 
					0, pStart, szRemotePath, 0, NULL);
				WaitForSingleObject(hHandle, INFINITE);
				if (hHandle != NULL)
				{
					CCommonFun::WriteFileLog(_T("注入成功。"));
					bRet = TRUE;
				}
				else
				{
					CCommonFun::WriteFileLog(_T("creatert failed, err = %d"), GetLastError());
				}
			}// before vista
		}//end of get loadlibrary function addr.
		VirtualFreeEx(hProcess, szRemotePath, iSize, MEM_DECOMMIT);
	}
	else
	{
		CCommonFun::WriteFileLog("szRemotePath == NULL");
	}
	CloseHandle(hProcess);
	hProcess = NULL;
	return bRet;
}
BOOL CProcessExp::UnloadDll(DWORD dwTargetPID, char* szDllPath)
{
	int iSize = (_tcslen(szDllPath) + 1) * sizeof(char);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS/*PROCESS_VM_WRITE | PROCESS_VM_OPERATION*/,
		FALSE, dwTargetPID);
	if (hProcess == NULL)
	{
		CCommonFun::WriteFileLog(_T("handleTarget == NULL, err = %d"), GetLastError());
		return FALSE;
	}
	BOOL bRet = FALSE;
	LPVOID szRemotePath = WriteRemoteMemory(hProcess, (LPVOID)szDllPath, iSize);
	if(szRemotePath != NULL)
	{
		LPTHREAD_START_ROUTINE pStart = GetStartRoutine(_T("GetModuleHandleA"));
		if (pStart != NULL)
		{
			HANDLE hThread = CreateRemoteThread( hProcess,
				NULL, 0, (LPTHREAD_START_ROUTINE)pStart, szRemotePath, 0, NULL);
			WaitForSingleObject(hThread, INFINITE);
			// 获得GetModuleHandle的返回值, 也就是目标dll的句柄转换为DWORD后的值
			HMODULE  hLib = NULL;
			GetExitCodeThread(hThread, (LPDWORD)&hLib);
			pStart = GetStartRoutine(_T("FreeLibrary"));
			if (pStart != NULL)
			{
				hThread = CreateRemoteThread( hProcess, NULL, 0, 
					(LPTHREAD_START_ROUTINE)pStart, (LPVOID)hLib, 0, NULL);
				// 等待FreeLibrary卸载完毕
				WaitForSingleObject( hThread, INFINITE );
				bRet = TRUE;
			}
		}
		VirtualFreeEx(hProcess, szRemotePath, iSize, MEM_DECOMMIT);
	}
	CloseHandle(hProcess);
	hProcess = NULL;
	return bRet;
}

BOOL FindToken(HANDLE& hProcessToken,DWORD dwActiveSessionId, BOOL bInDefaultDesktop)
{
	TCHAR szExeName[MAX_PATH];
	
	if (bInDefaultDesktop)
		strcpy(szExeName, "explorer.exe");
	else
		strcpy(szExeName, "winlogon.exe");

	CCommonFun::WriteFileLog("Find Token %s, bDefaultDesktop:%d, session:%d.", szExeName, bInDefaultDesktop, dwActiveSessionId);
	return CProcessExp::GetProcessToken(szExeName, dwActiveSessionId, hProcessToken);	
}
//读取某一用户的环境，explorer进程不存在时失败
BOOL ReadUserEnvirnment(DWORD dwSession, _ENVSTRING_t& stEnvData)
{
	TCHAR szExplorerPath[MAX_PATH + 1];
	GetWindowsDirectory(szExplorerPath, MAX_PATH);
	strcat(szExplorerPath, "\\");
	strcat(szExplorerPath, "explorer.exe");
	CCommonFun::WriteFileLog("Read session:%d, %s", dwSession, szExplorerPath);
	BOOL bRet =  CProcessExp::ReadEnvironmentBlock(szExplorerPath, dwSession, stEnvData);
	CCommonFun::WriteFileLog("Read env over");
	return bRet;
}
BOOL	CProcessExp::CreateProcessByCurUser(CString sPath,CString sParam,DWORD dwActiveSessionId)
{
	bool bCreatSuccess = false;
	if (dwActiveSessionId == 0)
	{
		dwActiveSessionId = CCommonFun::WTSGetActiveConsoleSessionIdEx();
	}
	try
	{
		char cFileName[MAX_PATH*2] = {0};
		sprintf(cFileName,"%s %s", sPath,sParam);

		BOOL   bDefaultDesktop = TRUE;
		HANDLE hToken = NULL;
		HANDLE hToken2= NULL;
		if(!FindToken(hToken, dwActiveSessionId, bDefaultDesktop) || !hToken)
		{
			CCommonFun::WriteFileLog("Find Token error");
			return FALSE;
		}
		else
		{
			//CCommonFun::WriteFileLog("find token successfully");
			BOOL bRet = DuplicateTokenEx(hToken,0,NULL,SecurityAnonymous,TokenPrimary,&hToken2);//复制一下Token才能创建用户进程成功
			if(bRet)
			{
				CCommonFun::WriteFileLog("DuplicateTokenEx success");
			}
			else
			{
				//sprintf(cMsg,);
				CCommonFun::WriteFileLog("DuplicateTokenEx error:%d",GetLastError());
				return FALSE;
			}
		}

		void* pEn = NULL;
		_ENVSTRING_t stEnvData;
		if (bDefaultDesktop)
		{
			if (!ReadUserEnvirnment(dwActiveSessionId, stEnvData))
			{
				CCommonFun::WriteFileLog("default: failed to read user envirnment.");
				
			}
			else
			{
				pEn = stEnvData.pData;
			}
			//bUnicode = 1;(*(LPBYTE(stEnvData.pData) + 1) == 0);
		}
		CCommonFun::WriteFileLog("env size:%d", stEnvData.nSize);

		WCHAR wcFileName[MAX_PATH * 2] = {0};
		int nRet = MultiByteToWideChar(CP_ACP,0,cFileName,strlen(cFileName)+1,wcFileName,MAX_PATH * 2);
		if(nRet)
		{
			STARTUPINFOW StartInfo;
			memset(&StartInfo,0,sizeof(StartInfo));
			PROCESS_INFORMATION pinfo;
			memset(&pinfo,0,sizeof(pinfo));
			StartInfo.cb = sizeof(STARTUPINFOW);
			if (bDefaultDesktop)
			{
				StartInfo.lpDesktop = L"winsta0\\default";	
			}
			else
			{
				StartInfo.lpDesktop = L"winsta0\\winlogon";	
			}
			if(!CreateProcessAsUserW(
				hToken2,
				(WCHAR*)wcFileName,
				0,
				NULL,
				NULL,
				FALSE,
				NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT,
				//| IsTextUnicode((LPVOID)stEnvData.pData, stEnvData.nSize, NULL) ? CREATE_UNICODE_ENVIRONMENT : NULL ,
				pEn,
				NULL,
				&StartInfo,
				&pinfo) )
			{
				CCommonFun::WriteFileLog("use wide char to create failed  error:%d 文件名：%s",::GetLastError(),cFileName);
			}
			else
			{
				CCommonFun::WriteFileLog("use wide char to create  successfully, sessionid: %d", dwActiveSessionId);
				bCreatSuccess = true;
			}
		}

		//如果创建失败，则使用多字节字符创建
		if(!bCreatSuccess)
		{
			STARTUPINFO StartInfo;
			memset(&StartInfo,0,sizeof(StartInfo));
			PROCESS_INFORMATION pinfo;
			memset(&pinfo,0,sizeof(pinfo));
			if (bDefaultDesktop)
			{
				StartInfo.lpDesktop = "winsta0\\default";	
			}
			else
			{
				StartInfo.lpDesktop = "winsta0\\winlogon";	
			}	
			StartInfo.cb = sizeof(STARTUPINFO);

			if(!CreateProcessAsUser(
				hToken2,
				(LPCSTR)cFileName,
				0,
				NULL,
				NULL,
				FALSE,
				NORMAL_PRIORITY_CLASS,
				NULL,
				NULL,
				&StartInfo,
				&pinfo) )
			{
				//sprintf(cMsg,);
				CCommonFun::WriteFileLog("------user multi char to create error:%d 文件名：%s",::GetLastError(),cFileName);
				bCreatSuccess = false;
			}
			else
			{
				bCreatSuccess = true;
				CloseHandle(pinfo.hThread);
				CloseHandle(pinfo.hProcess);
				CCommonFun::WriteFileLog("------user multi char to create eguard.exe successfully");
				//AddProcessInfo(dwActiveSessionId, 0);
			}
		}
	}
	catch(...)
	{
		CCommonFun::WriteFileLog("catch2 error:%d",::GetLastError());
		return FALSE;
	}


	return TRUE;
}