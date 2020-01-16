#pragma once

//====================================================
// 1.继承类
// 2.重载所需函数
// 3.必须调用CService：：OnStop（）；
//===================================================

#include <winsvc.h>
#define SERVICE_CONTROL_USER_OP				 128 
#define SERVICE_CONTROL_RELEASEPROC_HOOK	SERVICE_CONTROL_USER_OP + 1
#define SERVICE_CONTROL_CHANGE_TIME			SERVICE_CONTROL_USER_OP + 2

#define WM_WTSSESSION_CHANGE			   0x02B1
#define WTS_CONSOLE_CONNECT                0x1
#define WTS_CONSOLE_DISCONNECT             0x2
#define WTS_REMOTE_CONNECT                 0x3
#define WTS_REMOTE_DISCONNECT              0x4
#define WTS_SESSION_LOGON                  0x5
#define WTS_SESSION_LOGOFF                 0x6

class _AA_DLL_EXPORT_ CService
{
public:
	CService(TCHAR* szServiceName, BOOL bAcceptStop = TRUE);
	~CService(void);
public:
	int main(int argc, TCHAR* argv[], TCHAR* envp[]);
	BOOL Start();
	BOOL Stop();
	BOOL Install();
	BOOL UnInstall();
	void LogEventEx(int id, LPCTSTR pszMessage=NULL, WORD type = EVENTLOG_INFORMATION_TYPE);
	BOOL IsInstalled();
	void WriteLog(LPCTSTR lpszFormat, ...);
	void SvcDebugOut(LPSTR String, DWORD Status);
	BOOL SetServiceStatus(DWORD dwState);
	
	
	//====================================================
	//override
public:
	virtual void ProcessMsg(MSG& msg){}
	virtual void OnStart() = 0;
	virtual void OnStop();
	virtual void OnPause(){}
	virtual void OnContinue(){}
	virtual void OnInterrogate(){}
	virtual void OnShutdown();
	virtual void OnSessionChange(DWORD dwEventType,LPVOID lpEventData){}
	virtual void OnUnknownRequest(DWORD /*dwOpcode*/){}
	virtual void OnUnknownRequestEx(DWORD dwControl,     // requested control code
		DWORD dwEventType,   // event type
		LPVOID lpEventData,  // event data
		LPVOID lpContext     // user-defined context data
		){}

	virtual BOOL OnServiceInstalled(SC_HANDLE svc){return 0;}
	virtual BOOL OnServiceUninstalled(SC_HANDLE svc){return 0;}
	//====================================================

public:
	 void ServiceMain (DWORD argc, LPTSTR *argv);
	 DWORD WINAPI HandlerEx(
		DWORD dwControl,     // requested control code
		DWORD dwEventType,   // event type
		LPVOID lpEventData,  // event data
		LPVOID lpContext     // user-defined context data
		);
	 VOID WINAPI Handler(DWORD Opcode);

	static void WINAPI _ServiceMain (DWORD argc, LPTSTR *argv);
	static DWORD WINAPI _HandlerEx(
		DWORD dwControl,     // requested control code
		DWORD dwEventType,   // event type
		LPVOID lpEventData,  // event data
		LPVOID lpContext     // user-defined context data
		);
	static VOID WINAPI _Handler(DWORD Opcode) ;
	static DWORD _ServiceInitialization(DWORD   argc, LPTSTR  *argv, DWORD *specificError);
	static BOOL IsWinXpOrLater();
protected:
	virtual BOOL ProcessCommandLine(int argc, TCHAR* argv[], TCHAR* envp[]);
	BOOL SetFailedAction(SC_HANDLE svc,SC_ACTION_TYPE eType = SC_ACTION_RESTART);
	BOOL SetDelayedAutoStart(SC_HANDLE svc);
public:
	TCHAR m_szServiceName[MAX_PATH];
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_status;
	BOOL m_bService;
	DWORD m_dwThreadID;
	
//	static CService* pService;
	static BOOL		 bXpOrLater;
};
