// SnmpMonitor.h: interface for the CSnmpMonitor class.
//

/////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNMPMONITOR_H__72D628C2_3438_40B1_8628_271517537FAC__INCLUDED_)
#define AFX_SNMPMONITOR_H__72D628C2_3438_40B1_8628_271517537FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SnmpInterface.h"

class _AA_DLL_EXPORT_ CSnmpMonitor : public CSnmpOP
{
public:
	CSnmpMonitor();
	virtual ~CSnmpMonitor();

	BOOL Start(const char* szAddr,ISCPStatusInterface* pCall);
	void Stop();
	char* GetCmmt(FX_DC_SCP_STATUS eState);
	BOOL IsOutPager();
	BOOL IsOutPager(CString szPapeType);
	BOOL IsOutManualPaper();
	BOOL IsJamPaper();
	BOOL IsOutInk();
	BOOL IsOutNet();
	//少纸
	BOOL IsLowerPager(CString& sDesc);
	BOOL IsLowerInk(CString& sDesc);
	void SetPaperConfig(CString sConfig, int nMin);
	void SetInkConfig(CString sConfig, int nMin);
	
	//设置品牌
	void InitBrand(BRAND_TYPE eType);
	void EnablePrinterInfoMonitor(BOOL bEnable = TRUE, int nCheckInfoTimeOutSecond = 10);
	void EnablePrinterJobMonitor(BOOL bEnable, int nCheckJobTimeOutSecond = 10);
	void EnablePrinterMeterMonitor(BOOL bEnable, int nCheckMeterTimeOutSecond = 15);
	void EnablePrinterMonitor(BOOL bEnable, int nCheckMonitorTimeOutSecond = 60);

	//获取打印机状态
	void GetPrinterStatus(CSCPStatus& status);
	//获取打印机纸盒状态信息
	CString GetPrinterPaperInfo();
	//获取打印机墨盒状态信息
	CString GetPrinterInkInfo();
	//add by zxl,20150721,获取打印机抄表信息
	BOOL GetPrinterMeterInfo(PrinterMeterInfo* pInfo);
	BOOL IsRunning();

	CString GetPrtIP();

	void GetPrinterType(BRAND_TYPE& eType);

private:
	void InitCmmt();

	void CheckManualPaper();
	void CheckStatus();
	void CheckPaper();
	void CheckOPStatus();
	void CheckCanonOPStatus();
	void CheckDeviceStatus();
	void CheckInk();
	void CheckPrinterMeterInfo();
	void CheckPrinterMonitorInfo();

	void CheckLmPaper();
	void CheckLmInk();
	void CheckLmDeviceStatus();

	void CheckPrintCount();
	void CheckCopyCount();
	void CheckScanCount();
	void CheckFaxCount();
	void CheckJob();

	void CreateTh();
	void Shutdown();
	void StartJobMonitor();
	void StopJobMonitor();
	void StartPrinterMeterMonitor();
	void StopPrinterMeterMonitor();
	void StartPrinterMonitorHelper();
	void StopPrinterMonitorHelper();

	//监视操作的标记操作
	void CheckBegin();
	void CheckProcess();
	void CheckEnd();

protected:
	CSCPStatus m_oStatus;

	long m_lActive;
	HANDLE	m_hThreadTerm ;
	HANDLE	m_hThread;
	HANDLE	m_hThreadStarted;

	int m_nA4Pos;
	int m_nA3Pos;
	int m_aryPaper[Max_Paper_Index];
	int m_nMinPaper;

	FX_DC_INK_CMYK m_aryInk[Max_Ink_Index];
	int m_nMinInk;

	struct tag_Snmp_Desc m_aryCmmt[SNMP_CMMT_COUNT];

	CRITICAL_SECTION m_csLock;
	CSnmpJobMonitor* m_pJobMonitor;
	CRITICAL_SECTION m_csLock4JobMonitor;
	CSnmpPrinterMeterMonitor* m_pMeterMonitor;
	CRITICAL_SECTION m_csLock4MeterMonitor;	//add by lzj

	CSnmpMonitorHelper* m_pPrinterMonitorHelper;
	CRITICAL_SECTION m_csLock4PrinterMonitorHelper;

	DWORD m_lTickCount;
	static unsigned __stdcall ThreadFn(void*pvParam);

	BRAND_TYPE m_eType;

	BOOL m_bEnablePrinterJobMonitor;	//启用打印机作业监视
	DWORD m_nCheckJobTimeOutSecond;		//检测打印机作业信息的时间间隔
	DWORD m_nLastCheckJobTimeOutSecond;	//add by zfq,上次检测打印机作业信息的时间,MS

	BOOL m_bEnablePrinterMeterMonitor;	//启用打印机抄表监视
	DWORD m_nCheckMeterTimeOutSecond;	//检测打印机抄表信息的时间间隔
	DWORD m_nLastCheckMeterTimeOutSecond;	//add by zfq,Last 检测打印机抄表信息的时间间隔

	BOOL m_bEnablePrinterInfoMonitor;	//启用打印机信息监视（校园和企业打印）
	DWORD m_nCheckInfoTimeOutSecond;	//检测打印机信息的时间间隔（校园和企业打印）
	DWORD m_nLastCheckInfoTimeOutSecond;	//add by zfq,Last 检测打印机信息的时间间隔（校园和企业打印）

	BOOL m_bEnablePrinterMonitor;		//启用打印机状态监视（云打印）
	DWORD m_nCheckMonitorTimeOutSecond;	//检测打印机状态监视的时间间隔（云打印）
	DWORD m_nLastCheckMonitorTimeOutSecond;	//add by zfq,Last 检测打印机状态监视的时间间隔（云打印）

	BOOL m_bEnableConnectPrinter;		//启用打印机连接
	DWORD m_nCheckConnectTimeOutSecond;	//检测打印机连接的时间间隔
	DWORD m_nLastCheckConnectTimeOutSecond;	//add by zfq,Last 检测打印机连接的时间间隔

	int m_nLastPrintCount;
	int m_nLastPrintCount_Color;
	int m_nLastPrintCount_WB;
	int m_nLastCopyCount;
	int m_nLastA4CopyCount_Color;
	int m_nLastA4CopyCount_WB;
	int m_nLastA3CopyCount_Color;
	int m_nLastA3CopyCount_WB;
	int m_nLastScanCount;
	int m_nLastScanCount_Color;
	int m_nLastScanCount_WB;
	int m_nLastFaxCount;
	int m_nLastFaxCount_Color;
	int m_nLastFaxCount_WB;

	int m_nCheckTick;
};

#endif // !defined(AFX_SNMPMONITOR_H__72D628C2_3438_40B1_8628_271517537FAC__INCLUDED_)
