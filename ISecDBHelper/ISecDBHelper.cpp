// AsSpy.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ISecDBHelper.h"
#include "../Public/fzipport.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
CWinApp theApp;
using namespace std;
#define SERVICENAME "ISecDbHelper"
#define DBBACKPROCNAME "DbBackupConfig.exe"
#define DBBACKINTERVAL 1001
#define DBBACKTIMER	60*1000

//////////////////////////////////////////////////////////////////////////
// �������
void InstallWntService(bool bstart);
void UninstallService();
void StopService();
void ChangeServiceOp(PCHAR SysName);

//////////////////////////////////////////////////////////////////////////
// ʱ�����
static void CALLBACK TimerDayDbProcs(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void MyServiceStart(DWORD argc, LPTSTR *argv);
void WINAPI MyServiceCtrlHandler(DWORD opcode);
void GetDBBackProcPath(CString& sProcPath);

//////////////////////////////////////////////////////////////////////////
// ʹ�ñ���
SERVICE_STATUS          MyServiceStatus;
SERVICE_STATUS_HANDLE   MyServiceStatusHandle;
DWORD					m_dwThreadID = 0;
UINT					m_nTimeInterval = 0;
BOOL					m_bAutoDBBacked = FALSE;
COleDateTime			m_dPrevDateTime;
SERVICE_TABLE_ENTRY DispatchTable[]=
{
	{
		TEXT(SERVICENAME),
		(LPSERVICE_MAIN_FUNCTION)MyServiceStart
	},
	{	NULL,
		NULL
	}
};

void ZipCrp()
{
	CString sDestPath;
	sDestPath.Format(_T("%seDD.dll"),CCommonFun::GetDefaultPath());
	{
		CString sZipPath;
		BOOL bSucceed = TRUE;
		HZIP hz = NULL;
		do
		{
			// ����Zip�ļ�
			hz = CreateZip(sDestPath,0);
			if(!IsZipHandleZ(hz))
			{
				bSucceed = FALSE;
				break;
			}
			// ���ļ���ӵ�Zip�ļ���
			CStringArray sAry;
			sAry.Add("winacl.dll");
			sAry.Add("winemm.dll");
			sAry.Add("winfc8.dll");
			sAry.Add("winmc8.dll");

			CString sDes,sSour;
			for( int i=0;i<sAry.GetSize();i++)
			{
				ZRESULT zr = ZipAdd(hz,sAry.GetAt(i),CCommonFun::GetDefaultPath() + sAry.GetAt(i));
				if(zr != ZR_OK)
				{
					bSucceed = FALSE;
					break;					
				}
			}

			if (!bSucceed)
				break;
		}while(0);

		if(IsZipHandleZ(hz))
			CloseZip(hz);
	}
}
void UnZipCrp()
	{
		CString sStorePath;
		sStorePath.Format(_T("%seDD.dll"),CCommonFun::GetDefaultPath());
		
		HZIP hz = OpenZip(sStorePath,0);
		if(!IsZipHandleU(hz))
		{
			CCommonFun::WriteFileLog(_T("��ѹ���ļ�\"%s\"ʧ�ܣ�"),sStorePath);
			return;
		}

		// ���ý�ѹ��Ŀ¼
		CString sUnzipPath = sStorePath.Left(sStorePath.ReverseFind(_T('\\'))) + _T("\\");
		if (SetUnzipBaseDir(hz, sUnzipPath) == ZR_OK)
			CCommonFun::WriteFileLog(_T("��ѹ��Ŀ¼\"%s\"�ɹ�"),sUnzipPath);
		else
			CCommonFun::WriteFileLog(_T("��ѹ��Ŀ¼\"%s\"ʧ��"),sUnzipPath);

		// ��ʼ��ѹ��
		ZIPENTRY ze; 
		GetZipItem(hz,-1,&ze); 
		int numitems=ze.index;
		for(int zi = 0; zi < numitems; zi++)
		{
			GetZipItem(hz,zi,&ze);
			if (UnzipItem(hz,zi,ze.name) == ZR_OK)
			{
				CCommonFun::WriteFileLog(_T("��ѹ��%s�ɹ���"),ze.name);
			}
			else
				CCommonFun::WriteFileLog(_T("��ѹ��%sʧ�ܣ�"),ze.name);
		}
		CloseZip(hz);
	}
//////////////////////////////////////////////////////////////////////////
// ����ʵ��
int main(int argc, TCHAR* argv[], TCHAR* envp[])
{	
	//��NT�а�װ�������������
	if( argc > 1 && (strcmp(argv[1], "-Install") == 0))
	{
		InstallWntService(true);
		ChangeServiceOp(SERVICENAME);				
		return 0;
	}
	//��NT�а�װ�������
	else if( argc > 1 && (strcmp(argv[1], "-InstallOnly") == 0))
	{
		InstallWntService(false);
		return 0;
	}
	//ж�ط���
	else if( argc > 1 && (strcmp(argv[1], "-Unstall") == 0))
	{		
		UninstallService();		
		return 0;
	}
	//ֹͣ����
	else if( argc > 1 && (strcmp(argv[1], "-stop") == 0))
	{
		StopService();
		return 0;
	}
	else if(argc==2 && stricmp(argv[1], "/bakcrp")==0) 
	{
		ZipCrp();

		return 0;
	}
	else if(argc==2 && stricmp(argv[1], "/unzip")==0) 
	{
		UnZipCrp();

		return 0;
	}
	StartServiceCtrlDispatcher(DispatchTable);
	return 0;
}

void MyServiceStart(DWORD argc,LPTSTR *argv) 
{
	m_dwThreadID = GetCurrentThreadId();
	MyServiceStatusHandle = RegisterServiceCtrlHandler(TEXT(SERVICENAME),MyServiceCtrlHandler); 
	if (MyServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return; 

	// Initialization complete - report running status. 
	MyServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS; 
	MyServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP;
	MyServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
	MyServiceStatus.dwWin32ExitCode		 = NO_ERROR;
	MyServiceStatus.dwCheckPoint         = 0; 
	MyServiceStatus.dwWaitHint           = 0;  
	SetServiceStatus (MyServiceStatusHandle, &MyServiceStatus);

	// �����¼�
	m_nTimeInterval = ::SetTimer(NULL,DBBACKINTERVAL,DBBACKTIMER,TimerDayDbProcs);
	m_dPrevDateTime = COleDateTime::GetCurrentTime();
	m_dPrevDateTime.ParseDateTime(m_dPrevDateTime.Format(VAR_DATEVALUEONLY),VAR_DATEVALUEONLY);

	// �¼�ѭ��
	BOOL bRet;
	MSG msg;
	while((bRet = GetMessage(&msg,NULL,0,0)) != 0)
	{
		if (bRet==0)
			break;

		if(msg.message == WM_QUIT || msg.message == WM_CLOSE) 
			break;

		if (bRet > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// �رռ�ʱ��
	if (m_nTimeInterval != 0)
	{
		::KillTimer(NULL,m_nTimeInterval);
		m_nTimeInterval = 0;
	}

	// ���÷���Ϊֹͣ״̬
	MyServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(MyServiceStatusHandle,&MyServiceStatus); 
	return;
}

VOID WINAPI MyServiceCtrlHandler(DWORD Opcode) 
{
	switch(Opcode) 
	{ 
	case SERVICE_CONTROL_PAUSE:
		MyServiceStatus.dwCurrentState = SERVICE_PAUSED; 
		break;

	case SERVICE_CONTROL_CONTINUE:
		MyServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;

	case SERVICE_CONTROL_STOP:
		::PostThreadMessage(m_dwThreadID,WM_QUIT,0,0);
		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;
	}

	// ���÷���״̬
	SetServiceStatus(MyServiceStatusHandle,&MyServiceStatus);
	return; 
} 

static void CALLBACK TimerDayDbProcs(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
#if 0
	//add by zhandb 2014.6.30
	//������ʾ�汾�����ļ��Զ����ڵ�����
	CString file;
	file.Format(_T("%siSecBSvcHost.exe"),CCommonFun::GetDefaultPath());
	
	if((_access(file, 0) != -1) && (ERROR_SERVICE_ALREADY_RUNNING != CCommonFun::StartSvc("iSecBSvcHost")))
	{
		CCommonFun::WriteFileLog(_T("iSecBSvcHost ERROR_SERVICE_ALREADY_RUNNING"));
		UnZipCrp();
	}

	file.Format(_T("%siSecASvcHost.exe"),CCommonFun::GetDefaultPath());
	if((_access(file, 0) != -1)  && (ERROR_SERVICE_ALREADY_RUNNING != CCommonFun::StartSvc("iSecASvcHost")))
	{
		CCommonFun::WriteFileLog(_T("iSecASvcHost ERROR_SERVICE_ALREADY_RUNNING"));
		UnZipCrp();
	}
#endif

	CString sPath;
	BOOL bAutoBack;
	int nInterval;
	double dTime;
	BACKTYPE type;
	CDBBackFun::GetDBBackConfig(sPath,bAutoBack,type,nInterval,dTime);
	if (!bAutoBack)
		return;

	// �õ���ǰʱ��
	COleDateTime dcur = COleDateTime::GetCurrentTime();
	
	// ����Ѿ���ȥ��һ��
	COleDateTime dtToday;
	dtToday.ParseDateTime(dcur.Format(VAR_DATEVALUEONLY),VAR_DATEVALUEONLY);
	if (dtToday.m_dt > m_dPrevDateTime.m_dt)
		m_bAutoDBBacked = FALSE;

	// �������ͽ����ж�
	switch (type)
	{
		// ÿ�챸��
	case BACKTYPE_BYDAY:
		{
			dcur.ParseDateTime(dcur.Format(VAR_TIMEVALUEONLY),VAR_TIMEVALUEONLY);
			if (dcur.m_dt > dTime && !m_bAutoDBBacked)
				m_bAutoDBBacked = TRUE;
			else
				return;
		}
		break;

		// ÿ����
	case BACKTYPE_BYMONTH:
		{
			if (dcur.GetDay() != nInterval)
				return;

			dcur.ParseDateTime(dcur.Format(VAR_TIMEVALUEONLY),VAR_TIMEVALUEONLY);
			if (dcur.m_dt > dTime && !m_bAutoDBBacked)
				m_bAutoDBBacked = TRUE;
			else
				return;
		}
		break;

		// ÿ��
	case BACKTYPE_BYWEEK:
		{
			if (dcur.GetDayOfWeek() != nInterval + 1)
				return;

			dcur.ParseDateTime(dcur.Format(VAR_TIMEVALUEONLY),VAR_TIMEVALUEONLY);
			if (dcur.m_dt > dTime && !m_bAutoDBBacked)
				m_bAutoDBBacked = TRUE;
			else
				return;
		}
		break;
	}

	m_dPrevDateTime = COleDateTime::GetCurrentTime();
	m_dPrevDateTime.ParseDateTime(m_dPrevDateTime.Format(VAR_DATEVALUEONLY),VAR_DATEVALUEONLY);

	// ���д��α��ݳ���
	CString sDBBackFilePath;
	GetDBBackProcPath(sDBBackFilePath);
	CSecurityTool::CreateProcByCurUsr(sDBBackFilePath.GetBuffer(),"/s",SW_SHOW);
	sDBBackFilePath.ReleaseBuffer();
}

void ChangeServiceOp(PCHAR SysName)
{
    BOOL status = FALSE;
 
	SC_HANDLE  schService;
	SC_HANDLE  schSCManager;
 
	//�򿪷�������
	schSCManager = OpenSCManager (NULL, NULL,SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) 
	{
		return;
	}
	
	schService = OpenService (schSCManager,TEXT(SysName), SERVICE_ALL_ACCESS);
	if (!schService)
	{
		CloseServiceHandle (schSCManager);	
	   return;
	}	


	SERVICE_FAILURE_ACTIONS sdBuf={0};

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
		schService,                 
		SERVICE_CONFIG_FAILURE_ACTIONS, 
		&sdBuf) )                   
	{
		CCommonFun::WriteFileLog("ChangeServiceConfig2 failed\n");
	}
	else
		CCommonFun::WriteFileLog(" ChangeServiceConfig2 succeeded\n");

	CloseServiceHandle (schService);
	CloseServiceHandle (schSCManager);	
	//////////////////////////
	return  ;
}

void InstallWntService(bool bstart)
{
	char sysdir[MAX_PATH  + 1];
	memset(sysdir, 0, MAX_PATH + 1);
	::GetModuleFileName(NULL, sysdir, _MAX_PATH+1);
	CString sExe = sysdir;
	LPCTSTR lpDependencies = NULL;
	BOOL bRegOK = FALSE;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if(scm != NULL)
	{
		CCommonFun::WriteFileLog(_T("OpenSCManager "));
		SC_HANDLE svc  = OpenService( 
			scm,       // SCManager database 
			SERVICENAME,       // name of service 
			SERVICE_START
			);            // only need DELETE access 
		if(svc)
		{
			StartService(svc,0,NULL);	
			CloseServiceHandle(scm);
			return;
		}
		CCommonFun::WriteFileLog(_T("CreateService "));
		svc = CreateService(scm,SERVICENAME,SERVICENAME,
							SERVICE_ALL_ACCESS,
							SERVICE_WIN32_OWN_PROCESS |	SERVICE_INTERACTIVE_PROCESS,
							SERVICE_AUTO_START,//SERVICE_AUTO_START,
							SERVICE_ERROR_IGNORE,
							sysdir,	
							NULL,NULL,
							lpDependencies,
							NULL,NULL);
		if(svc != NULL)
		{
			CCommonFun::WriteFileLog(_T("CreateService suc"));
			if(bstart)
			{
				StartService(svc,0,NULL);
			}
			bRegOK = TRUE;
			CloseServiceHandle(svc);
		}	
		else
		{
			CCommonFun::WriteFileLog(_T("CreateService %d "),::GetLastError());
		}
		
		CloseServiceHandle(scm);
    }
	else
	{
		CCommonFun::WriteFileLog(_T("OpenSCManager %d "),::GetLastError());
	}
}
 
void StopService()
{
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		SC_HANDLE svc = OpenService(scm,SERVICENAME,SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			QueryServiceStatus(svc,&ssa);
			if (ssa.dwCurrentState == SERVICE_RUNNING) //ֹͣ��Service.
				ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
			CloseServiceHandle(svc);
        }                           
		CloseServiceHandle(scm);
    }
}

void UninstallService()
{
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		SC_HANDLE svc = OpenService(scm,SERVICENAME,SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			QueryServiceStatus(svc,&ssa);
			if (ssa.dwCurrentState == SERVICE_RUNNING) //ɾ��ǰ����ֹͣ��Service.
				ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
			DeleteService(svc);
			CloseServiceHandle(svc);
        }                           
		//�Ա����������ݿ������ߴ���Ŀ��
		CloseServiceHandle(scm);
    }
}

void GetDBBackProcPath(CString& sProcPath)
{
	CString sCulFileName;
	GetModuleFileName(NULL,sCulFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	sCulFileName.ReleaseBuffer();
	int inum = sCulFileName.ReverseFind('\\');
	if (inum == -1)
		return;

	sProcPath.Format("\"%s\\%s\"",sCulFileName.Left(inum),DBBACKPROCNAME);
}