// NtDriverController.h: interface for the CNtDriverController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTDRIVERCONTROLLER_H__729AF2DC_BD54_4FB8_8ACF_0206A9F1F075__INCLUDED_)
#define AFX_NTDRIVERCONTROLLER_H__729AF2DC_BD54_4FB8_8ACF_0206A9F1F075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsvc.h>
class _AA_DLL_EXPORT_ CNtDriverController  
{
public:
	BOOL FileExist();
	BOOL Init(CString sName);
	
	BOOL InstallAndStart(CString sName,BOOL bStart = TRUE,BOOL bNeedDelStopDriver = FALSE);
	BOOL StopAndRemove(CString sName);
	BOOL Open();
	BOOL InstallAndStart(BOOL bStart = TRUE);
	BOOL StopAndRemove();
	BOOL Stop();
	void Close();
	BOOL WaitForState(DWORD  dwDesiredState, SERVICE_STATUS* pss);

	//--------------------------------------------
	// 说明: AddDevice函数，可以设置驱动路径、启动类型
	// 时间: 2011/07/18
	// Add by SongQX
	//--------------------------------------------
	DWORD AddDevice(LPCTSTR pszServiceBinary, DWORD dwStartType = SERVICE_DEMAND_START);
	BOOL RemoveService();

	CNtDriverController(CString sName,BOOL bStart = TRUE,BOOL bNeedDelStopDriver = FALSE);
	CNtDriverController();
	virtual ~CNtDriverController();
public:
	BOOL	  m_bNeedDelStopDriver;

protected:
	SC_HANDLE m_hSCM;
	SC_HANDLE m_hDriver;
	BOOL      m_bDriverStarted;
	
	BOOL      m_bErrorOnStart;
	TCHAR      m_szName[MAX_PATH];
	TCHAR      m_szInfo[MAX_PATH];
	TCHAR      m_szFullFileName[MAX_PATH];
public:
	BOOL StartDriver(CString sName = _T(""));
};

#endif // !defined(AFX_NTDRIVERCONTROLLER_H__729AF2DC_BD54_4FB8_8ACF_0206A9F1F075__INCLUDED_)
