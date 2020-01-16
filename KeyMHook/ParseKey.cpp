#include "StdAfx.h"
#include "ParseKey.h"
//#include "FileLog.h"
#include "KeyHkUdpSock.h"
#include "IniFile.h"
#define SYS_CONFIG_FILE _T("config\\config.ini")

void WriteLogKH(LPCTSTR szMsg,LPCTSTR szFileName)
{
	FILE *pFile = fopen(szFileName, "a+");
	if(!pFile)
	{
		return;
	}

	char *pRet = "\r\n";
	fwrite(szMsg, 1, strlen(szMsg), pFile);
	fwrite(pRet, 1, strlen(pRet), pFile);

	fclose(pFile);
}

//获取日志目录
CString& GetLogDir()
{
	static CString g_szLogDir = "";
	if (g_szLogDir.IsEmpty())
	{
		OutputDebugString("##CCommonFun::GetLogDir");

		TCHAR path[MAX_PATH];
		CString sDefaultPath;
		if(GetModuleFileName(0,path,MAX_PATH))
		{
			TCHAR *pdest;
			pdest=_tcsrchr(path,_T('\\'));
			if(pdest) 
				*pdest=0;
			sDefaultPath=path;
			sDefaultPath += "\\xlog";
		}

		if (sDefaultPath.IsEmpty())
		{
			sDefaultPath = ".\\xlog";
		}

		g_szLogDir = sDefaultPath;
	}

	return g_szLogDir;
}

void WriteLogExKH(LPCTSTR lpszFormat, ...)
{
	//	ASSERT(AfxIsValidString(lpszFormat));

	char sTmp[2048];
	memset(sTmp, 0x0, 2048);

	CString szTmp;
	CTime time = CTime::GetCurrentTime();
	CString strTime;
	szTmp = time.Format("%Y-%m-%d  %H:%M:%S ");
	szTmp += "====>";
	sprintf(sTmp, szTmp);
	char *pPos = sTmp + (szTmp.GetLength());


	va_list argList;
	va_start(argList, lpszFormat);
#if 0
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
#else
//	_snprintf(sTmp, 2048, lpszFormat, argList);
//	vsprintf(sTmp, lpszFormat, argList);
	vsprintf(pPos, lpszFormat, argList);
#endif
	va_end(argList);

	//	TRACE("%s\n",sTmp);

	CString szLogFilePath;
	szLogFilePath.Format("%s\\KeyMHook.log", GetLogDir());

	WriteLogKH(sTmp,szLogFilePath);
}

CString GetDefaultPath()
{
	TCHAR path[MAX_PATH];
	CString sDefaultPath;
	if(GetModuleFileName(0,path,MAX_PATH)==0) 
		return _T("");
	TCHAR *pdest;
	pdest=_tcsrchr(path,_T('\\'));
	if(pdest) 
		*pdest=0;
	sDefaultPath=path;
	sDefaultPath+=_T("\\");
	return sDefaultPath;
}

BOOL ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable)
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
		TRACE(_T("Failed OpenProcessToken\n"));
		return ERROR_FUNCTION_FAILED;
	}

	// Get the local unique id for the privilege.
	if ( !LookupPrivilegeValue( NULL,
		szPrivilege,
		&luid ))
	{
		CloseHandle( hToken );
		TRACE(_T("Failed LookupPrivilegeValue\n"));
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
		TRACE(_T("Failed AdjustTokenPrivileges\n"));
		hr = ERROR_FUNCTION_FAILED;
	}

	// Close the handle.
	CloseHandle(hToken);

	return hr;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

BOOL IsWow64()
{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")),"IsWow64Process");

	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		//AfxMessageBox(_T("aa"));
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
			//AfxMessageBox(_T("errr"));
		}
	}
	return bIsWow64;
}

BOOL IsWin9X()
{
	OSVERSIONINFO osv={sizeof(OSVERSIONINFO)};
	// osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if (!bSuc)
	{
	}
	if(osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		return TRUE;
	}
	return FALSE;
}

CString GetProcessFullPath(DWORD dwPID)
{
	if(!IsWin9X())
	{
		if(ModifyPrivilege(SE_DEBUG_NAME ,TRUE) != S_OK )
		{
			TRACE(_T(""));
		}
	}

	HANDLE hProcess = NULL;
	if (IsWow64())
		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPID);
	else
		hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if(!hProcess)
		return _T("");

	HINSTANCE hLib = LoadLibrary(_T("PSAPI.DLL"));
	if(!hLib)
	{
		if(hProcess)
		{
			::CloseHandle(hProcess);
			hProcess = NULL;
		}
		return _T("");
	}

	CString sRet = _T("");
	TCHAR szFilename[MAX_PATH + 1] = {0};
	HMODULE hModule[1024];
	DWORD	dwSize = MAX_PATH + 1;
	BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *, DWORD, LPDWORD);
	DWORD (WINAPI *lpfGetModuleFileNameEx)( HANDLE, HMODULE, LPTSTR, DWORD);			
	DWORD (WINAPI *lpfGetProcessImageFileName)(HANDLE,LPTSTR,DWORD);
	if (IsWow64())
	{
		lpfGetProcessImageFileName = (DWORD(WINAPI *)(HANDLE,LPTSTR,DWORD))GetProcAddress(hLib, "GetProcessImageFileNameA") ;
		if(lpfGetProcessImageFileName)
		{
			if(lpfGetProcessImageFileName(hProcess,szFilename,dwSize))
			{
				sRet = szFilename;	

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
							if (sRet.Left(sName.GetLength()).Compare(sName) == 0)
							{
								sRet.Replace(sName,szDrive);
								bFound = TRUE;
								break;
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			else
				WriteLogExKH("LastError = %d,",::GetLastError());
		}
	}
	else
	{
		lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *, DWORD, LPDWORD)) GetProcAddress(hLib, "EnumProcessModules" ) ;
		lpfGetModuleFileNameEx =(DWORD (WINAPI *)(HANDLE, HMODULE, LPTSTR, DWORD )) GetProcAddress(hLib, "GetModuleFileNameExA") ;	    
		if(lpfEnumProcessModules)
		{
			if(lpfEnumProcessModules(hProcess, hModule, 1024, &dwSize))
			{
				if(lpfGetModuleFileNameEx)
				{
					if (lpfGetModuleFileNameEx(hProcess, hModule[0],szFilename, MAX_PATH))
					{
						sRet = szFilename;	
						//CCommonFun::WriteFileLog(_T("lpfGetModuleFileNameEx = %s,"),szFilename);
					}
				}
			}
			else
			{
				WriteLogExKH("LastError = %d,",::GetLastError());
			}
		}
	}

	if(hProcess)
	{
		::CloseHandle(hProcess);
		hProcess = NULL;
	}
	if(hLib)
	{
		::FreeLibrary(hLib);
		hLib = NULL;
	}
	return sRet;
}


//	获取进程带路径名称和不带路径名称
BOOL GetProcessPathAndName(DWORD dwProcessID, CString &sFullName, CString &sName)
{
	sFullName= GetProcessFullPath(dwProcessID);
	if(sFullName.IsEmpty()) // 程序名为空
	{
		WriteLogExKH("!!GetProcessPathAndName 根据进程ID获取路径名失败!,err=%d", GetLastError());
		return FALSE;
	}
	sFullName.MakeLower();

	int nReverseIndex = sFullName.ReverseFind('\\');
	if(nReverseIndex>0)	// 程序名中包含路径
		sName = sFullName.Right(sFullName.GetLength()-nReverseIndex-1);
	return TRUE;
}

CParseKey::CParseKey(void)
{
	m_pKeyCard = NULL;
	m_dwTickCountLastInput = 0;
#if 0	//modify by zxl,2013.04.22
	m_dwMaxSpan = 30;
	m_dwMaxCount = 10;
#else
	CIniFile file;
	file.m_sPath.Format("%s%s",GetDefaultPath(),SYS_CONFIG_FILE);
	m_dwMaxSpan = file.GetVal("PrtCtrlConfig","MaxSpan",100);
	m_dwMaxCount = file.GetVal("PrtCtrlConfig","MaxCount",10);
#endif

	m_pfnOnReadCard = NULL;
	m_pContext = NULL;

	m_pUdpSock = NULL;

	m_pKeyCard = new BYTE[m_dwMaxCount + 1];
	memset(m_pKeyCard, 0x0, m_dwMaxCount + 1);

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		err = WSAStartup(wsaData.wVersion,&wsaData);
		WriteLogExKH("!!CParseKey::CParseKey, WSAStartup fail,err=%d", GetLastError());
	}

	CString szProPath = "";
	CString szProName = "";
	DWORD dwProID = GetCurrentProcessId();
	if(!GetProcessPathAndName(dwProID, szProPath, szProName))
	{
		WriteLogExKH("##CParseKey::CParseKey,0.7,GetProcessPathAndName fail,err=%d,curProcID=%d", GetLastError(), dwProID);
	}
	
	WriteLogExKH("##CParseKey::CParseKey,1,构造,curProcID=%d,szProName=[%s],szProPath=[%s]", dwProID, szProName, szProPath);
}

CParseKey::~CParseKey(void)
{
	if(m_pKeyCard)
	{
		delete []m_pKeyCard;
		m_pKeyCard = NULL;
	}

	if(m_pUdpSock)
	{
		delete m_pUdpSock;
		m_pUdpSock = NULL;
	}

	WSACleanup();

	WriteLogExKH("##CParseKey::~CParseKey,1,析构,curProcID=%d", GetCurrentProcessId());
}

void CParseKey::SaveKey(WPARAM wp)
{
	DWORD dwCurTick = ::GetTickCount();
	DWORD dwSpan = dwCurTick - m_dwTickCountLastInput;
	BOOL bValid = IsThisKeyValid(wp);
	BOOL bEndFlag = IsEndFlag(wp);

	
	if(!bValid)
	{//当前键不是有效输入，清空缓存
		memset(m_pKeyCard, 0x0, m_dwMaxCount + 1);
		m_dwTickCountLastInput = dwCurTick;
		return;
	}

	if((m_dwMaxSpan < dwSpan) && !bEndFlag)
	{//输入间隔超时 但不是结束符
//		WriteLogExKH("##CParseKey::SaveKey,1,超时,dwCurTick=%d,last=%d,m_dwMaxSpan=%d,dwSpan=%d"
//			, dwCurTick, m_dwTickCountLastInput, m_dwMaxSpan, dwSpan);
		if(!bEndFlag)
		{//此时不是结束符
//			m_szKeyCard.Format("%c", wp);
			m_pKeyCard[0] = (BYTE)wp;
		}
		else
		{//是结束符，清空
//			m_szKeyCard = "";
			memset(m_pKeyCard, 0x0, m_dwMaxCount + 1);
		}
	}
	else
	{
//		WriteLogExKH("##CParseKey::SaveKey,10,m_szKeyCard=%s,len=%d,bEndFlag=%d"
//			, m_pKeyCard, strlen((char*)m_pKeyCard), bEndFlag);

		if(!bEndFlag)
		{//此时不是结束符
#if 0
			CString szTmp;
			szTmp.Format("%c", wp);
			m_szKeyCard += szTmp;
#else
			int nOriLen = strlen((char*)m_pKeyCard);
			m_pKeyCard[nOriLen] = (BYTE)wp;
#endif
		}
		else
		{//是结束符，清空

//			WriteLogExKH("##CParseKey::SaveKey,20,结束符,m_szKeyCard=%s,len=%d,m_dwMaxCount=%d"
//				, m_pKeyCard, strlen((char*)m_pKeyCard), m_dwMaxCount);

			int nOriLen = strlen((char*)m_pKeyCard);
			if((m_dwMaxSpan << 1) < dwSpan)
			{//结束符超时
//				if(m_dwMaxCount == m_szKeyCard.GetLength())
				if(m_dwMaxCount == nOriLen)
				{
#if 0
					theLog.Write("##CParseKey::SaveKey,21,结束符,m_szKeyCard=%s,len=%d,m_dwMaxCount=%d,m_dwMaxSpan<<1=%d,dwSpan=%d"
						, m_szKeyCard, m_szKeyCard.GetLength(), m_dwMaxCount, m_dwMaxSpan<<1, dwSpan);
#else
					WriteLogExKH("##CParseKey::SaveKey,21,结束符,m_szKeyCard=%s,len=%d,m_dwMaxCount=%d,m_dwMaxSpan<<1=%d,dwSpan=%d"
						, m_pKeyCard, nOriLen, m_dwMaxCount, m_dwMaxSpan<<1, dwSpan);
#endif
				}
			}
			else
			{
				//查看当前位数
//				if(m_dwMaxCount == m_szKeyCard.GetLength())
				if(m_dwMaxCount >= nOriLen)
				{
//					OnReadCardByKeyBoard(m_szKeyCard);
					OnReadCardByKeyBoard(m_pKeyCard);
				}
			}
//			m_szKeyCard = "";
			memset(m_pKeyCard, 0x0, m_dwMaxCount + 1);
		}
	}

	m_dwTickCountLastInput = dwCurTick;
	return;
}

BOOL CParseKey::IsThisKeyValid(WPARAM wp)
{
	if(('0' <= wp && '9' >= wp)	//是数字
		|| '\r' == wp)
	{
		return TRUE;
	}
#if 0 //铁匠的刷卡器模拟键盘输入，只可能是数字与'r'
		|| ('a' <= wp && 'f' >= wp)	//是小写字母,十六进制字母只能是A~F
		|| ('A' <= wp && 'F' >= wp)	//是小写字母,十六进制只能是A~F
#endif

	return FALSE;
}

BOOL CParseKey::IsEndFlag(WPARAM wp)
{
	if('\r' == wp)
	{
		return TRUE;
	}

	return FALSE;
}

#if 0
void WriteLog(LPCTSTR szMsg,LPCTSTR szFileName)
{

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime = time.Format("%Y-%m-%d  %H:%M:%S");

	CFile file;
	CString sTemp = strTime;
	sTemp += "----->>>";
	sTemp += szMsg;
	if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sTemp,sTemp.GetLength());
		file.Write("\r\n",strlen("\r\n"));
		file.Close();
	}
}
#endif


extern int	 g_nMultiPort;
extern char g_bysMultiIP[16];	//组播IP
extern char g_bysLocalIP[16];	//本地IP

//void CParseKey::OnReadCardByKeyBoard(CString &szCardNum)
void CParseKey::OnReadCardByKeyBoard(BYTE *pszCardNum)
{

	WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,2,szCardNum=%s", pszCardNum);

#if 1
	if(!m_pUdpSock)
	{
		m_pUdpSock = new CKeyHkUdpSock;
		if(!m_pUdpSock)
		{
			WriteLogExKH("!!CParseKey::OnReadCardByKeyBoard,3,szCardNum=%s,new CKeyHkUdpSock fail,err=%d"
				, pszCardNum, GetLastError());
			return;
		}

//		WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,4,szCardNum=%s,multiIP=%s,multiPort=%d,LocalIP=%s"
//			, pszCardNum, g_bysMultiIP, g_nMultiPort, g_bysLocalIP);


		if(!m_pUdpSock->Init(g_bysMultiIP, g_nMultiPort, g_bysLocalIP))
		{
			WriteLogExKH("!!CParseKey::OnReadCardByKeyBoard,5,szCardNum=%s,multiIP=%s,multiPort=%d,LocalIP=%s,Init fail,err=%d"
				, pszCardNum, g_bysMultiIP, g_nMultiPort, g_bysLocalIP, GetLastError());
			return;
		}

		WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,6,szCardNum=%s,multiIP=%s,multiPort=%d,LocalIP=%s,Init OK"
			, pszCardNum, g_bysMultiIP, g_nMultiPort, g_bysLocalIP);
	}

	BOOL BRet = m_pUdpSock->SendCardNum(pszCardNum);
	if(!BRet)
	{
		WriteLogExKH("!!CParseKey::OnReadCardByKeyBoard,7,szCardNum=%s,multiIP=%s,multiPort=%d,LocalIP=%s,pszCardNum=%s,Send fail,err=%d"
			, pszCardNum, g_bysMultiIP, g_nMultiPort, g_bysLocalIP, pszCardNum, GetLastError());
	}
	else
	{
		WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,8,szCardNum=%s,multiIP=%s,multiPort=%d,LocalIP=%s,pszCardNum=%s,Send OK,curProcID=%d"
			, pszCardNum, g_bysMultiIP, g_nMultiPort, g_bysLocalIP, pszCardNum, GetCurrentProcessId());
	}

#else
	WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,2.1,test=%d,m_pfnOnReadCard=0x%x,test2=%d", 6, m_pfnOnReadCard, 328);
	int i = 0;
	int nLen = strlen((char*)pszCardNum);
	for(; i < nLen; i++)
	{
		WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,2,buf[%d]=%c", i, pszCardNum[i]);
	}

	if(m_pfnOnReadCard)
	{
		m_pfnOnReadCard((char*)pszCardNum, m_pContext);
	}
	else
	{
		WriteLogExKH("##CParseKey::OnReadCardByKeyBoard,6,m_pfnOnReadCard=0x%x", m_pfnOnReadCard);
		ASSERT(0);
	}
#endif
}

void CParseKey::SetCallBack(PFN_OnReadCardByKeyHook pfn, void *pContext)
{
	m_pfnOnReadCard = pfn;
	m_pContext = pContext;

	WriteLogExKH("##CParseKey::SetCallBack,1,m_pfnOnReadCard=0x%x,m_pContext=0x%x,test=%d", m_pfnOnReadCard, m_pContext,132);
}