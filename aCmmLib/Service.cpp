#include "StdAfx.h"
#include "Service.h"

static CService* pService = NULL;
BOOL      CService::bXpOrLater = IsWinXpOrLater();

#pragma warning(push)
#pragma warning(disable:4996)

CService::CService(TCHAR* szServiceName, BOOL bAcceptStop /* = TRUE */)
{
	strncpy(m_szServiceName, szServiceName,  MAX_PATH);
	// set up the initial service status 
	m_hServiceStatus = NULL;
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_status.dwCurrentState = SERVICE_STOPPED;
	if (bAcceptStop)
		m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	else
		m_status.dwControlsAccepted = 0;
	m_status.dwWin32ExitCode = 0;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;
	pService = this;
}

CService::~CService(void)
{
}
int CService::main(int argc, TCHAR *argv[], TCHAR *envp[])
{
	for (int n = 0; n < argc ; n ++)
	{
		WriteLog(argv[n]);
	}
	
	if( argc > 1 && (stricmp(argv[1], "-install") == 0))
	{
		Install();
		Start();
		return 0;
	}
	else if( argc > 1 && (stricmp(argv[1], "-installOnly") == 0))
	{
		Install();
		return 0;
	}
	else if( argc > 1 && (stricmp(argv[1], "-UnInstall") == 0))
	{
		WriteLog("UnInstall");
		UnInstall();
		return 0;
	}
	else if( argc > 1 && (stricmp(argv[1], "-stop") == 0))
	{
		Stop();
		return 0;
	}
	else if (ProcessCommandLine(argc, argv, envp))
	{
		return 0;
	}
	SERVICE_TABLE_ENTRY DispatchTable[]=
	{
		{m_szServiceName, _ServiceMain}, {NULL, NULL}
	};
	if ( !StartServiceCtrlDispatcher (DispatchTable))
	{
		SvcDebugOut ("[SRPSM_Service] StartServiceCtrlDispatcher error = %d\n", GetLastError());
	}
	return 0;
}
BOOL  CService::ProcessCommandLine(int argc, TCHAR* argv[], TCHAR* envp[])
{
	return FALSE;
}
void CService::SvcDebugOut(LPSTR String, DWORD Status)
{ 
	CHAR  Buffer[1024] = {0};

	if (strlen(String) < 1000)
	{
		sprintf(Buffer, String, Status);
		WriteLog(Buffer);
	}
#ifndef _DEBUG
	return;
#endif
	OutputDebugStringA(Buffer);
}

void CService::ServiceMain(DWORD argc, LPTSTR* argv)
{
	DWORD status; 
	DWORD specificError; 
	m_dwThreadID = GetCurrentThreadId();

	bXpOrLater = IsWinXpOrLater();

	WriteLog("MyServiceStart 服务启动 %d",bXpOrLater);	
	if(bXpOrLater)
		m_hServiceStatus = RegisterServiceCtrlHandlerEx(m_szServiceName, _HandlerEx,NULL); 
	else
		m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler); 

	if (pService != NULL)
	{
		pService->OnStart();
	}

	if (m_hServiceStatus == (SERVICE_STATUS_HANDLE)0)
	{
		SvcDebugOut(" [SRPSM_Service] RegisterServiceCtrlHandler failed %d\n", GetLastError()); 
		return; 
	}

	status = _ServiceInitialization(argc, argv, &specificError);	
	if (status != NO_ERROR)
	{
		SetServiceStatus (SERVICE_STOPPED); 
		return; 
	} 

	//DWORD dwControlsAccepted = SERVICE_ACCEPT_STOP;
	if(bXpOrLater)
		m_status.dwControlsAccepted |= 	SERVICE_ACCEPT_SESSIONCHANGE;
	else
		m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

		//dwControlsAccepted = dwControlsAccepted | SERVICE_ACCEPT_SESSIONCHANGE;
	
	m_status.dwServiceType        = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
	//m_status.dwControlsAccepted   = dwControlsAccepted;
	m_status.dwWin32ExitCode	  = NO_ERROR;
	m_status.dwCheckPoint         = 0; 
	m_status.dwWaitHint           = 0;  
	if (!SetServiceStatus (SERVICE_RUNNING)) 
	{ 
		status = GetLastError(); 
		SvcDebugOut(" [SRPSM_Service] SetServiceStatus error %ld\n",status); 
	}


	BOOL bRet;
	MSG msg;
	while((bRet = GetMessage(&msg, NULL, 0, 0 )) != 0)
	{
		if (bRet==0)
			break;
		if(msg.message == WM_QUIT || msg.message == WM_CLOSE) 
		{
			break;
		}
		if (bRet>0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (pService != NULL)
			{
				pService->ProcessMsg(msg);
			}
		}
	}
	SetServiceStatus (SERVICE_STOPPED); 
	return;
}

void CService::OnStop()
{
	SetServiceStatus(SERVICE_STOP_PENDING);
	PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
}

void CService::OnShutdown()
{
	SetServiceStatus(SERVICE_STOP_PENDING);
	PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
}

void CService::Handler(DWORD dwOpcode) throw()
{
	if (pService == NULL)
		return;
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		pService->OnStop();
		break;
	case SERVICE_CONTROL_PAUSE:
		pService->OnPause();
		break;
	case SERVICE_CONTROL_CONTINUE:
		pService->OnContinue();
		break;
	case SERVICE_CONTROL_INTERROGATE:
		pService->OnInterrogate();
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		pService->OnShutdown();
		break;
	default:
		pService->OnUnknownRequest(dwOpcode);
	}
}

BOOL CService::IsInstalled()
{
	BOOL bResult = FALSE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}
BOOL CService::Install()
{
	if (IsInstalled())
	{
		WriteLog("%s has installed",m_szServiceName);
		return TRUE;
	}
	BOOL bRet = FALSE;
	char szPath[MAX_PATH  + 1] = {0};
	::GetModuleFileName(NULL, szPath, MAX_PATH+1);

	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if(scm != NULL)
	{
		SC_HANDLE svc  = OpenService(scm, m_szServiceName, SERVICE_CHANGE_CONFIG);
		if(svc)
		{
			//2009.12.1
			//只有marscltsvc失败后重启
			//SetFailedAction(svc);
			OnServiceInstalled(svc);
			CloseServiceHandle(scm);
			return TRUE;
		}
		DWORD dwAccess = SERVICE_ALL_ACCESS;
		if(bXpOrLater)
			dwAccess =  SERVICE_ALL_ACCESS|SERVICE_ACCEPT_SESSIONCHANGE;
		svc = CreateService(scm, m_szServiceName, m_szServiceName, dwAccess,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,	SERVICE_ERROR_IGNORE, szPath,	NULL, NULL, NULL, NULL, NULL);
		if(svc != NULL)
		{
			//2009.12.1
			//只有marscltsvc失败后重启
		//	SetFailedAction(svc);
			OnServiceInstalled(svc);
			CloseServiceHandle(svc);
			bRet = TRUE;
		}		
		CloseServiceHandle(scm);
	}
	return bRet;
}
BOOL CService::Start()
{
	BOOL bRet = FALSE;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if(scm != NULL)
	{
		SC_HANDLE svc  = OpenService(scm, m_szServiceName, SERVICE_ALL_ACCESS);
		if(svc)
		{
			bRet = StartService(svc, 0, NULL);	
			if (!bRet)
			{
				WriteLog("!!CService::Start,StartService fail,err=%u",GetLastError());
			}
			CloseServiceHandle(svc);	//add by zfq,2013.06.08
			CloseServiceHandle(scm);
		}
		else
		{
			CloseServiceHandle(scm);	//add by zfq,2013.06.08
		}
	}
	else
	{
		WriteLog("!!CService::Start,OpenSCManager fail,err=%u",GetLastError());
	}
	return bRet;
}
BOOL CService::Stop()
{
	BOOL bRet = FALSE;
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		SC_HANDLE svc = OpenService(scm, m_szServiceName, SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			QueryServiceStatus(svc,&ssa);
			if (ssa.dwCurrentState == SERVICE_RUNNING) //停止此Service.
			{
				bRet = ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
			}
			CloseServiceHandle(svc);
		}                           
		CloseServiceHandle(scm);
	}
	return bRet;
}

BOOL CService::UnInstall()
{
	WriteLog("come in UnInstall");
	//if (!IsInstalled())
	//	return TRUE;
	BOOL bRet = FALSE;
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		WriteLog("come in UnInstall aa");
		SC_HANDLE svc = OpenService(scm, m_szServiceName, SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			WriteLog("come in UnInstall bb");
			OnServiceUninstalled(svc);

			QueryServiceStatus(svc,&ssa);
			
			if (ssa.dwCurrentState == SERVICE_RUNNING) //删除前，先停止此Service.
			{
				bRet = ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
				::Sleep(20);
			}
			
			BOOL b = DeleteService(svc);
			WriteLog("come in UnInstall cc %d",b);
			CloseServiceHandle(svc);
		}                           
		//以便立即从数据库中移走此条目。
		CloseServiceHandle(scm);
	}
	return bRet;
}

void CService::LogEventEx(int id, LPCTSTR pszMessage, WORD type)
{
	HANDLE hEventSource;
	if (m_szServiceName)
	{
		/* Get a handle to use with ReportEvent(). */
		hEventSource = RegisterEventSource(NULL, m_szServiceName);
		if (hEventSource != NULL)
		{
			/* Write to event log. */
			ReportEvent(hEventSource, 
				type,
				(WORD)0,
				id,
				NULL,
				(WORD)(pszMessage != NULL ? 1 : 0),
				0,
				pszMessage != NULL ? &pszMessage : NULL,
				NULL);
			DeregisterEventSource(hEventSource);
		}
	}
}


BOOL CService::SetServiceStatus(DWORD dwState)
{
	m_status.dwCurrentState = dwState;
	return ::SetServiceStatus(m_hServiceStatus, &m_status);
}

DWORD WINAPI CService::HandlerEx(DWORD dwControl,     // requested control code
								 DWORD dwEventType,   // event type
								 LPVOID lpEventData,  // event data
								 LPVOID lpContext     // user-defined context data
								 )
{
	if (pService == NULL)
		return 0;
	switch(dwControl) 
	{ 
	case SERVICE_CONTROL_SESSIONCHANGE:
		pService->OnSessionChange(dwEventType,lpEventData);
		break;
	case SERVICE_CONTROL_PAUSE:
		pService->OnPause();
		break;
	case SERVICE_CONTROL_CONTINUE:
		pService->OnContinue();
		break;
	case SERVICE_CONTROL_STOP:
		pService->OnStop();
		break;
	default:
		pService->OnUnknownRequestEx(dwControl, dwEventType, lpEventData, lpContext);
	}
	return 0;  
}
void WINAPI CService::_ServiceMain (DWORD argc, LPTSTR *argv)
{
	if (pService != NULL)
		pService->ServiceMain(argc, argv);
}
DWORD WINAPI CService::_HandlerEx(		DWORD dwControl,     // requested control code
								  DWORD dwEventType,   // event type
								  LPVOID lpEventData,  // event data
								  LPVOID lpContext     // user-defined context data
								  )
{
	if (pService != NULL)
		return pService->HandlerEx(dwControl, dwEventType, lpEventData, lpContext);
	return 0;
}
VOID WINAPI CService::_Handler(DWORD Opcode)
{
	if (pService != NULL)
		pService->Handler(Opcode);
}
DWORD CService::_ServiceInitialization(DWORD   argc, LPTSTR  *argv, DWORD *specificError) 
{ 

	argv; 
	argc; 
	specificError; 
	return(0); 
}

void CService::WriteLog(LPCTSTR lpszFormat, ...)
{
 	if (!CCommonFun::IsDebug())
 		return;
	va_list argList;
	va_start(argList, lpszFormat);
	CString sLog;
	sLog.FormatV(lpszFormat, argList);
	va_end(argList);

// 	SYSTEMTIME curTime;
// 	GetLocalTime(&curTime);
// 
// 	CString strTime;
// 	strTime.Format("[%d/%d/%d %d:%d:%d.%d]",curTime.wYear,curTime.wMonth,curTime.wDay,
// 		curTime.wHour,curTime.wMinute,curTime.wSecond,curTime.wMilliseconds);
// 
//  	CString sTemp;
//  	sTemp.Format("%-25s  %s", strTime, sTmp);

	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format(_T("%y%m%d"));

	CFile file;
	CString strFileName;
	strFileName.Format("%s\\%s_%s.Log", CCommonFun::GetLogDir(), m_szServiceName, sDate);
	if(file.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		CString sTime = time.Format( _T("%Y-%m-%d %H:%M:%S") );
		CString sTmp;

		sTmp.Format(_T("%s,	 %s"),sTime,sLog);
		sTmp += _T("\r\n");
		file.SeekToEnd();
		file.Write(sTmp,sTmp.GetLength());
		file.Close();
	}
}

BOOL CService::IsWinXpOrLater()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if( osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		theLog.Write("osv.dwMajorVersion %d",osv.dwMajorVersion);
		theLog.Write("osv.dwMinorVersion %d",osv.dwMinorVersion);

		if (osv.dwMajorVersion == 5 && osv.dwMinorVersion >= 1)
			return TRUE;
		// vista(6.0), 2008, win7(6.1)
		if (osv.dwMajorVersion > 5)
			return TRUE;
		if (osv.dwMajorVersion == 4 || (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 0))
		{
			theLog.Write("IsWinXpOrLater osv.dwMajorVersion 4");
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CService::SetFailedAction(SC_HANDLE svc,SC_ACTION_TYPE eType)
{
	SERVICE_FAILURE_ACTIONS sdBuf={0};
	BOOL bSuccess=TRUE;

	sdBuf.lpRebootMsg=NULL;
	sdBuf.dwResetPeriod=3600*24;

	SC_ACTION action[3];

	action[0].Delay=5*1000;
	action[0].Type=SC_ACTION_RESTART;

	action[1].Delay=5*1000;
	action[1].Type=SC_ACTION_RESTART;
	action[2].Delay=5*1000;
	action[2].Type=SC_ACTION_RESTART;

	sdBuf.cActions=3;
	sdBuf.lpsaActions=action;
	sdBuf.lpCommand=NULL;

	if( !ChangeServiceConfig2(
		svc,                 
		SERVICE_CONFIG_FAILURE_ACTIONS, 
		&sdBuf) )                   
	{
		WriteLog("ChangeServiceConfig2 failed\n");
		bSuccess = FALSE;
	}
	else
		WriteLog(" ChangeServiceConfig2 succeeded\n");
	return bSuccess;
}

BOOL CService::SetDelayedAutoStart(SC_HANDLE svc)
{
	BOOL bSuccess=TRUE;
	//Client Requires Windows Vista.
	//Server Requires Windows Server 2008.
	SERVICE_DELAYED_AUTO_START_INFO sdBuf={0};
	sdBuf.fDelayedAutostart = TRUE;
	if( !ChangeServiceConfig2(
		svc,                 
		SERVICE_CONFIG_DELAYED_AUTO_START_INFO, 
		&sdBuf) )                   
	{
		WriteLog("SetDelayedAutoStart failed\n");
		bSuccess = FALSE;
	}
	else
		WriteLog("SetDelayedAutoStart succeeded\n");
	return bSuccess;
}

#pragma warning(pop)
