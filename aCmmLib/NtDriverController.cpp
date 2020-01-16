//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NtDriverController.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNtDriverController::CNtDriverController(CString sName,BOOL bStart,BOOL bNeedDelStopDriver)
{
	m_bNeedDelStopDriver = bNeedDelStopDriver;
	m_hSCM = NULL;
	m_hDriver = NULL;
	if (TRUE == Open()	// 赋值 m_hSCM
		&& Init(sName))	// 检测 *.sys 文件并复制到%system32%\drivers目录下
	{
		if (bStart)
			m_bDriverStarted = InstallAndStart(bStart);	// 启动服务
	} // if
}

CNtDriverController::CNtDriverController()
{
	m_hSCM = NULL;
	m_hDriver = NULL;
	m_bDriverStarted = FALSE;
	m_bNeedDelStopDriver = FALSE;
}

CNtDriverController::~CNtDriverController()
{
	if(m_bNeedDelStopDriver)
	{
		StopAndRemove();
	}
	Close();
}

BOOL CNtDriverController::Init(CString sName)
{
	TCHAR szPath[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szPath,MAX_PATH + 1);

	_tcscpy(m_szName, sName);
	_tcscpy(m_szInfo, sName);
	_stprintf(m_szFullFileName,_T("%s\\Drivers\\%s.sys"),szPath,sName);
	_stprintf(m_szInfo,sName);
	return FileExist();
}

BOOL CNtDriverController::Open()
{
	if (NULL == m_hSCM)
		m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	return (m_hSCM != NULL);
}

BOOL CNtDriverController::InstallAndStart(CString sName,BOOL bStart,BOOL bNeedDelStopDriver)
{
	BOOL bResult = FALSE;
	m_bNeedDelStopDriver = bNeedDelStopDriver;
	if (Open() && Init(sName))
	{
		bResult = InstallAndStart(bStart);	
	} // if

	return bResult;
}

BOOL CNtDriverController::InstallAndStart(BOOL bStart)
{
	BOOL bResult = FALSE;

	if (Open())
	{
		m_hDriver = ::CreateService(
			m_hSCM, 
			m_szName, 
			m_szInfo,
			SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			m_szFullFileName, 
			NULL, 
			NULL,
			NULL, 
			NULL, 
			NULL
			);

		ERROR_PATH_NOT_FOUND;
		if (NULL == m_hDriver)
		{
			if ( (::GetLastError() == ERROR_SERVICE_EXISTS) ||
			     (::GetLastError() == ERROR_SERVICE_MARKED_FOR_DELETE) )
				m_hDriver = ::OpenService(
					m_hSCM,
					m_szName,
					SERVICE_ALL_ACCESS
					);
		}
		if (NULL != m_hDriver && bStart )
		{
			SERVICE_STATUS serviceStatus = { 0 };
			bResult = ::StartService(m_hDriver, 0, NULL);
			//DWORD dw = ::GetLastError();
	
			if (bResult)
			{		
				bResult = WaitForState(SERVICE_RUNNING, &serviceStatus);	
			}
			else
				bResult = (::GetLastError() == ERROR_SERVICE_ALREADY_RUNNING);
			// We should call DeleteService() if the SCM reports an error
			// on StartService(). Otherwise, the service will remain loaded
			// in an undesired state
			if (!bResult)
			{
				// Mark the service for deletion.
				//::DeleteService(m_hDriver);
				if (m_hDriver != NULL)
				{
					::CloseServiceHandle(m_hDriver);
					m_hDriver = NULL;
				}
				m_bErrorOnStart = TRUE;
			}
		} // if
	} // if

	return bResult;
}
BOOL CNtDriverController::Stop()
{
	BOOL bResult=TRUE;
	if ((NULL != m_hDriver) && (!m_bErrorOnStart))
	{	
		SERVICE_STATUS serviceStatus = { 0 };
		// Notifies a service that it should stop. 
		bResult = ::ControlService(m_hDriver, SERVICE_CONTROL_STOP, &serviceStatus);
		
		::Sleep(100);
		Close();
	} // if
	
	return bResult;
}

BOOL CNtDriverController::StopAndRemove()
{	
	BOOL bResult=TRUE;
	if ((NULL != m_hDriver) && (!m_bErrorOnStart))
	{	
		SERVICE_STATUS serviceStatus = { 0 };
		// Notifies a service that it should stop. 
		bResult = ::ControlService(m_hDriver, SERVICE_CONTROL_STOP, &serviceStatus);
		if (bResult)
		//	bResult = WaitForState(SERVICE_STOPPED, &serviceStatus);	
		// Mark the service for deletion.
		::DeleteService(m_hDriver);
	} // if
	else
	{
		RemoveService();
	}
	return bResult;
}

BOOL CNtDriverController::StopAndRemove(CString sName)
{	
	BOOL bResult=TRUE;

	if(Open())
	{
		SC_HANDLE hSvc = ::OpenService(m_hSCM,sName,DELETE);
		if(hSvc)
		{
			::DeleteService(hSvc);
			::CloseServiceHandle(hSvc);
		}
	}

	return bResult;
}
void CNtDriverController::Close()
{
	if (m_hDriver != NULL)
	{
		::CloseServiceHandle(m_hDriver);
		m_hDriver = NULL;
	}
	if (m_hSCM != NULL)
	{
		::CloseServiceHandle(m_hSCM);
		m_hSCM = NULL;
	} 
}

BOOL CNtDriverController::WaitForState(
	DWORD           dwDesiredState, 
	SERVICE_STATUS* pss
	) 
{
	BOOL bResult = FALSE;
	if (NULL != m_hDriver)
	{
		// Loop until driver reaches desired state or error occurs
		while (1)
		{
			// Get current state of driver
			bResult = ::QueryServiceStatus(m_hDriver, pss);
			// If we can't query the driver, we're done
			if (!bResult) 
				break;
			// If the driver reaches the desired state
			if (pss->dwCurrentState == dwDesiredState) 
				break;
			// We're not done, wait the specified period of time
			DWORD dwWaitHint = pss->dwWaitHint / 10;    // Poll 1/10 of the wait hint
			if (dwWaitHint <  1000) dwWaitHint = 1000;  // At most once a second
			if (dwWaitHint > 10000) dwWaitHint = 10000; // At least every 10 seconds
			::Sleep(dwWaitHint);
		} // while
	} // if

	return bResult;
}

BOOL CNtDriverController::FileExist()
{
	// [12/7/2011 Songqx]
	// 与drvInstall中的统一

	BOOL bRet = FALSE;
	char szPath[MAX_PATH] = {0};
	::GetSystemDirectory(szPath,MAX_PATH);
	CString sPath(szPath);
	sPath.TrimRight("\\");
	sPath += "\\Drivers\\";
	sPath += m_szName;
	sPath += ".sys";

	CFileStatus st;
	if(CFile::GetStatus(sPath,st))
	{
		CString sDefaultPath;
#ifdef _AMD64_
		sDefaultPath.Format("%s\\x64\\%s.sys", CCommonFun::GetDefaultPath(),m_szName);
#else
		sDefaultPath.Format("%s\\x86\\%s.sys", CCommonFun::GetDefaultPath(),m_szName);
#endif
		if(!CCommonFun::IsSameFile(sPath,sDefaultPath))
		{
			bRet = CopyFile(sDefaultPath,sPath,FALSE);
		}
		bRet = TRUE;
	}
	else
	{
		CString sDefaultPath ;
#ifdef _AMD64_
		sDefaultPath.Format("%s\\x64\\%s.sys", CCommonFun::GetDefaultPath(),m_szName);
#else
		sDefaultPath.Format("%s\\x86\\%s.sys", CCommonFun::GetDefaultPath(),m_szName);
#endif
		bRet = CopyFile(sDefaultPath,sPath,TRUE);
	}
	return bRet; 
}

BOOL CNtDriverController::StartDriver(CString sName /* = "" */)
{
	BOOL bResult = FALSE;

	if(!sName.IsEmpty())
	{
		strcpy(m_szName,sName);
	}
	if (Open())
	{
		m_hDriver = ::OpenService(
					m_hSCM,
					m_szName,
					SERVICE_ALL_ACCESS
					);
		
		if (NULL != m_hDriver)
		{
			SERVICE_STATUS serviceStatus = { 0 };
			bResult = ::StartService(m_hDriver, 0, NULL);
			//DWORD dw = ::GetLastError();
	
			if (bResult)
			{		
				bResult = WaitForState(SERVICE_RUNNING, &serviceStatus);	
			}
			else
				bResult = (::GetLastError() == ERROR_SERVICE_ALREADY_RUNNING);
			// We should call DeleteService() if the SCM reports an error
			// on StartService(). Otherwise, the service will remain loaded
			// in an undesired state
			if (!bResult)
			{
				// Mark the service for deletion.
				//::DeleteService(m_hDriver);
				if (m_hDriver != NULL)
				{
					::CloseServiceHandle(m_hDriver);
					m_hDriver = NULL;
				}
				m_bErrorOnStart = TRUE;
			}
		} // if
	} // if
	return bResult;
}

//--------------------------------------------
// 说明: AddDevice函数，可以设置驱动路径、启动类型
// 时间: 2011/07/18
// Add by SongQX
//--------------------------------------------

DWORD CNtDriverController::AddDevice(LPCTSTR pszServiceBinary, DWORD dwStartType)
{
	DWORD status = ERROR_SUCCESS;
	SC_HANDLE schSCManager = OpenSCManager(
		NULL,                 // computer name
		NULL,                 // SCM database name
		SC_MANAGER_CREATE_SERVICE // access type
		);

	if (schSCManager)
	{
		SC_HANDLE schService = CreateService(
			schSCManager,          // SCManager database
			m_szName, // name of service
			m_szInfo,         // name to display
			SERVICE_ALL_ACCESS,    // desired access
			SERVICE_KERNEL_DRIVER, // service type
			dwStartType,//SERVICE_DEMAND_START,  // start type
			SERVICE_ERROR_NORMAL,  // error control type
			pszServiceBinary,      // service's binary
			NULL,                  // no load ordering group
			NULL,                  // no tag identifier
			NULL,                  // no dependencies
			NULL,                  // LocalSystem account
			NULL                   // no password
			);
		if (schService)
		{
			CloseServiceHandle(schService);
		}
		else
		{
			status = GetLastError();
			//ERROR_SERVICE_MARKED_FOR_DELETE
			if(ERROR_SERVICE_MARKED_FOR_DELETE == status)
			{
				schService=OpenService(schSCManager,
					pszServiceBinary,
					SERVICE_ALL_ACCESS);
				if(schService)
				{			
					if(ChangeServiceConfig(schService,
						SERVICE_KERNEL_DRIVER,
						0,
						SERVICE_ERROR_NORMAL,
						pszServiceBinary,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL))
					{
						CloseServiceHandle(schService);
						status = ERROR_SUCCESS;
					}
					else
					{
						status = GetLastError();
					}
				}
			}
		}
		CloseServiceHandle(schSCManager);
	}
	else
	{
		status = GetLastError();
	}
	return status;
}
BOOL CNtDriverController::RemoveService()
{
	BOOL bRet = FALSE;
	if(Open())
	{
		SC_HANDLE hSvc = ::OpenService(m_hSCM,m_szName,DELETE);
		if(hSvc)
		{
			bRet = ::DeleteService(hSvc);
			::CloseServiceHandle(hSvc);
		}
	}
	return bRet;
}