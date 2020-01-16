// SnmpMonitor.cpp: implementation of the CSnmpMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SnmpMonitor.h"

//打印作业监视头文件
#include "SnmpJobMonitorKM.h"
#include "SnmpJobMonitorSL.h"
#include "SnmpJobMonitorLG.h"
#include "SnmpJobMonitorDZ.h"
#include "SnmpJobMonitorJC.h"

//打印机抄表操作类头文件
#include "SnmpPrinterMeterMonitorLM.h"
#include "SnmpPrinterMeterMonitorSL.h"
#include "SnmpPrinterMeterMonitorKM.h"
#include "SnmpPrinterMeterMonitorHP.h"
#include "SnmpPrinterMeterMonitorLG.h"
#include "SnmpPrinterMeterMonitorDZ.h"
#include "SnmpPrinterMeterMonitorJC.h"
#include "SnmpPrinterMeterMonitorSX.h"
#include "SnmpPrinterMeterMonitorXP.h"
#include "SnmpPrinterMeterMonitorEPSON.h"
#include "SnmpPrinterMeterMonitorCANON.h"
#include "SnmpPrinterMeterMonitorJT.h"
#include "SnmpPrinterMeterMonitorBrother.h"
#include "SnmpPrinterMeterMonitorOKI.h"
#include "SnmpPrinterMeterMonitorSEIKO.h"
#include "SnmpPrinterMeterMonitorPT.h"
#include "SnmpPrinterMeterMonitorPRONNIX.h"
#include "SnmpPrinterMeterMonitorTSC.h"
#include "SnmpPrinterMeterMonitorDASCOM.h"

//打印机监视操作类头文件
#include "SnmpMonitorHelperLG.h"
#include "SnmpMonitorHelperSL.h"
#include "SnmpMonitorHelperCANON.h"
#include "SnmpMonitorHelper_JT.h"
#include "SnmpMonitorHelper_SEIKO.h"
#include "SnmpMonitorHelperPT.h"
#include "SnmpMonitorHelperHP.h"
#include "SnmpMonitorHelperDASCOM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSnmpMonitor::CSnmpMonitor() : CSnmpOP()
{
	m_lActive = 0;
	m_hThreadTerm = 0;
	m_hThreadStarted = 0;
	m_hThread = 0;
	m_pJobMonitor = NULL;
	m_pMeterMonitor = NULL;
	m_pPrinterMonitorHelper = NULL;
	m_nA4Pos = 1;
	m_nA3Pos = 2;
	m_nMinPaper = 25;
	m_nMinInk = 10;
	m_bEnablePrinterJobMonitor = FALSE;	//启用打印机作业监视
	m_nCheckJobTimeOutSecond = 1;		//检测打印机作业信息的时间间隔
	m_bEnablePrinterMeterMonitor = FALSE;//启用打印机抄表监视
	m_nCheckMeterTimeOutSecond = 1;		//检测打印机抄表信息的时间间隔
	m_bEnablePrinterInfoMonitor = TRUE;	//启用打印机信息监视
	m_nCheckInfoTimeOutSecond = 1;		//检测打印机信息的时间间隔
	m_bEnableConnectPrinter = FALSE;	//启用打印机连接
	m_nCheckConnectTimeOutSecond = 10;	//检测打印机连接的时间间隔
	m_bEnablePrinterMonitor = FALSE;	//启用打印机状态监视
	m_nCheckMonitorTimeOutSecond = 60;	//检测打印机状态监视的时间间隔

	m_nCheckTick = 0;

//	memset(m_aryInk,0,sizeof(m_aryInk));
	m_nLastPrintCount = 0;
	m_nLastPrintCount_Color = 0;
	m_nLastPrintCount_WB = 0;
	m_nLastCopyCount = 0;
	m_nLastA4CopyCount_Color = 0;
	m_nLastA4CopyCount_WB = 0;
	m_nLastA3CopyCount_Color = 0;
	m_nLastA3CopyCount_WB = 0;
	m_nLastScanCount = 0;
	m_nLastScanCount_Color = 0;
	m_nLastScanCount_WB = 0;
	m_nLastFaxCount = 0;
	m_nLastFaxCount_Color = 0;
	m_nLastFaxCount_WB = 0;
	InitCmmt();
	InitializeCriticalSection (&m_csLock);
	InitializeCriticalSection(&m_csLock4MeterMonitor);
	InitializeCriticalSection(&m_csLock4JobMonitor);
	InitializeCriticalSection(&m_csLock4PrinterMonitorHelper);

	m_nLastCheckJobTimeOutSecond = 0;	//add by zfq,上次检测打印机作业信息的时间,MS
	m_nLastCheckMeterTimeOutSecond = 0;	//add by zfq,Last 检测打印机抄表信息的时间间隔
	m_nLastCheckInfoTimeOutSecond = 0;	//add by zfq,Last 检测打印机信息的时间间隔（校园和企业打印）
	m_nLastCheckMonitorTimeOutSecond = 0;	//add by zfq,Last 检测打印机状态监视的时间间隔（云打印）
	m_nLastCheckConnectTimeOutSecond = 0;	//add by zfq,Last 检测打印机连接的时间间隔
}

CSnmpMonitor::~CSnmpMonitor()
{
	Shutdown();
	DeleteCriticalSection (&m_csLock );
	DeleteCriticalSection (&m_csLock4MeterMonitor);
	DeleteCriticalSection (&m_csLock4JobMonitor);
	DeleteCriticalSection (&m_csLock4PrinterMonitorHelper);	//add by zfq,2016-01-28
}
void CSnmpMonitor::Shutdown()
{
	theLog.Write("CSnmpMonitor::Shutdown,0.1");
	InterlockedExchange(&m_lActive,0);
	theLog.Write("CSnmpMonitor::Shutdown,0.2");
	if (m_hThread && m_hThreadTerm)
	{
		theLog.Write("CSnmpMonitor::Shutdown,1,");
		SetEvent(m_hThreadTerm);
		theLog.Write("CSnmpMonitor::Shutdown,2,");
		WaitForSingleObject(m_hThread,INFINITE);
		theLog.Write("CSnmpMonitor::Shutdown,3,");
		CloseHandle(m_hThread);
		CloseHandle(m_hThreadTerm);
		m_hThreadTerm = 0;
		m_hThread = 0;
	}
	m_nCheckTick = 0;
	m_bEnableConnectPrinter = FALSE;
}

void CSnmpMonitor::CreateTh()
{
	if (!m_hThreadTerm)
	{
		InterlockedExchange(&m_lActive,1);
		m_hThreadTerm = CreateEvent(0,0,0,0);
		m_hThreadStarted = CreateEvent(0,0,0,0);
		m_hThread			= 	(HANDLE)_beginthreadex(0,0,CSnmpMonitor::ThreadFn,(void*)this,0,0 );

		DWORD dwWait = WaitForSingleObject ( m_hThreadStarted , INFINITE );

		ASSERT ( dwWait ==		WAIT_OBJECT_0 );

		CloseHandle(m_hThreadStarted);
		m_hThreadStarted = 0;
	}
}

BOOL CSnmpMonitor::IsRunning()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lActive);
	return lVal;
}

CString CSnmpMonitor::GetPrtIP()
{
	return CSnmpOP::GetPrtIP();
}

void CSnmpMonitor::GetPrinterType(BRAND_TYPE& eType)
{
	eType = m_eType;
}

void CSnmpMonitor::CheckStatus()
{
	if(m_nCheckTick > (24 * 3600))	//最大一天循环
	{
		m_nCheckTick = 0;
	}
	
	DWORD dwCur = GetTickCount();
	DWORD dwTick1 = dwCur;
	DWORD dwTick2 = 0;
	DWORD dwTick3 = 0;
	DWORD dwTick4 = 0;
	DWORD dwTick5 = 0;
	int nSpanMS = 0;
	int nSpanSec = 0;

	//theLog.Write("CSnmpMonitor::CheckStatus,0.1,m_lpMgrSession=%x,m_bEnableConnectPrinter=%d", m_lpMgrSession, m_bEnableConnectPrinter);

	if (m_lpMgrSession && !m_bEnableConnectPrinter)
	{
//		theLog.Write("CSnmpMonitor::CheckStatus,1,PrtIP=%s,begin", m_szIP);

		//--add by zfq,begin
		nSpanMS = dwCur - m_nLastCheckInfoTimeOutSecond;
		nSpanSec = (0 > nSpanMS) ? 0 : (nSpanMS / 1000);
		//--add by zfq,end
//		if ((m_nCheckTick % m_nCheckInfoTimeOutSecond) == 0)	//del by zfq
		if(nSpanSec
			> m_nCheckInfoTimeOutSecond
			|| 0 == m_nLastCheckInfoTimeOutSecond)	//add by zfq
		{
			if (m_bEnablePrinterInfoMonitor)
			{
				CheckDeviceStatus();
				CheckOPStatus();
				if (m_bEnableConnectPrinter)
				{
					//此处将m_nCheckTick与m_nCheckConnectTimeOutSecond设置为相等，以最快进入重连
					m_nCheckTick = m_nCheckConnectTimeOutSecond;
				}
				else
				{
					CheckPrintCount();
					CheckCopyCount();
					CheckScanCount();
					CheckFaxCount();
				}
			}

			m_nLastCheckInfoTimeOutSecond = GetTickCount();	//add by zfq
		}

		dwTick2 = GetTickCount();

		//--add by zfq,begin
		nSpanMS = dwCur - m_nLastCheckJobTimeOutSecond;
		nSpanSec = (0 > nSpanMS) ? 0 : (nSpanMS / 1000);
		//--add by zfq,end
//		if ((m_nCheckTick % m_nCheckJobTimeOutSecond) == 0)	//del by zfq
		if(nSpanSec
			> m_nCheckJobTimeOutSecond
			|| 0 == m_nLastCheckJobTimeOutSecond)	//add by zfq
		{
			CheckJob();	//检测打印作业信息

			m_nLastCheckJobTimeOutSecond = GetTickCount();	//add by zfq
		}

		dwTick3 = GetTickCount();

		//--add by zfq,begin
		nSpanMS = dwCur - m_nLastCheckMonitorTimeOutSecond;
		nSpanSec = (0 > nSpanMS) ? 0 : (nSpanMS / 1000);
		//--add by zfq,end

//		if ((m_nCheckTick % m_nCheckMonitorTimeOutSecond) == 0) //del by zfq
		if((nSpanSec
			> m_nCheckMonitorTimeOutSecond)
			|| 0 == m_nLastCheckMonitorTimeOutSecond)	//add by zfq
		{
			//开始检测预警信息
			CheckBegin();

			CheckPrinterMonitorInfo();	//检测打印机监视信息

			//结束检测预警信息
			CheckEnd();

			m_nLastCheckMonitorTimeOutSecond = GetTickCount();	//add by zfq
		}

		dwTick4 = GetTickCount();

		//--add by zfq,begin
		nSpanMS = dwCur - m_nLastCheckMeterTimeOutSecond;
		nSpanSec = (0 > nSpanMS) ? 0 : (nSpanMS / 1000);
		//--add by zfq,end

// 		theLog.Write("!@CSnmpMonitor::CheckStatus,9.1,PrtIP=%s,nSpanMS=%d,nSpanSec=%d,m_nCheckMeterTimeOutSecond=%d,m_nLastCheckMeterTimeOutSecond=%d,dwCur=%d,Go=%d"
// 			, m_szIP, nSpanMS, nSpanSec, m_nCheckMeterTimeOutSecond
// 			, m_nLastCheckMeterTimeOutSecond, dwCur
// 			, 0 == m_nLastCheckMeterTimeOutSecond);


//		if ((m_nCheckTick % m_nCheckMeterTimeOutSecond) == 0)	//del by zfq
		
		if((nSpanSec
			> m_nCheckMeterTimeOutSecond)
			|| 0 == m_nLastCheckMeterTimeOutSecond)	//add by zfq
		{
			CheckPrinterMeterInfo();	//检测抄表信息,此处抄表信息包含墨粉和纸盒信息，
										//所以第一次要在CheckPrinterMonitorInfo之后调用才能获取到
			m_nLastCheckMeterTimeOutSecond = GetTickCount();	//add by zfq
		}

		dwTick5 = GetTickCount();

		
		DWORD dwCost1 = dwTick2 - dwTick1;
		DWORD dwCost2 = dwTick3 - dwTick2;
		DWORD dwCost3 = dwTick4 - dwTick3;
		DWORD dwCost4 = dwTick5 - dwTick4;
		DWORD dwMax = 15000;	//15秒
		if(dwMax < dwCost1
			|| dwMax < dwCost2
			|| dwMax < dwCost3
			|| dwMax < dwCost4)
		{
			theLog.Write("!@CSnmpMonitor::CheckStatus,10,PrtIP=%s,end,Cost TOO Much Time,dwCost1=%d,dwCost2=%d,dwCost3=%d,dwCost4=%d"
				, m_szIP, dwCost1, dwCost2, dwCost3, dwCost4);
		}
		else
		{
//			theLog.Write("CSnmpMonitor::CheckStatus,10,PrtIP=%s,end,dwCost1=%d,dwCost2=%d,dwCost3=%d,dwCost4=%d"
//				, m_szIP, dwCost1, dwCost2, dwCost3, dwCost4);
		}
	}//if (m_lpMgrSession && !m_bEnableConnectPrinter)
	else
	{
		//--add by zfq,begin
		nSpanMS = dwCur - m_nLastCheckConnectTimeOutSecond;
		nSpanSec = (0 > nSpanMS) ? 0 : (nSpanMS / 1000);
		//--add by zfq,end

//		theLog.Write("CSnmpMonitor::CheckStatus,20.1,PrtIP=%s,nSpanMS=%d,nSpanSec=%d,m_nCheckConnectTimeOutSecond=%d,m_nLastCheckConnectTimeOutSecond=%d,GO=%d"
//			, m_szIP, nSpanMS, nSpanSec, m_nCheckConnectTimeOutSecond
//			, m_nLastCheckConnectTimeOutSecond, 0 == m_nLastCheckConnectTimeOutSecond);


//		if ((m_nCheckTick % m_nCheckConnectTimeOutSecond) == 0)	//del by zfq
		if((nSpanSec
			> m_nCheckConnectTimeOutSecond)
			|| 0 == m_nLastCheckConnectTimeOutSecond)	//add by zfq
		{
			theLog.Write("CSnmpMonitor::CheckStatus,21,PrtIP=%s,begin,nSpanSec=%d,m_nCheckConnectTimeOutSecond=%d,m_bEnableConnectPrinter=%d"
				, m_szIP, nSpanSec , m_nCheckConnectTimeOutSecond, m_bEnableConnectPrinter);

			//开始检测预警信息
			CheckBegin();

			dwTick2 = GetTickCount();

			m_bEnableConnectPrinter = CheckIPActive() ? FALSE : TRUE;	//检测打印机网络连通性

			theLog.Write("CSnmpMonitor::CheckStatus,21.2,PrtIP=%s,nSpanSec=%d,m_nCheckConnectTimeOutSecond=%d,m_bEnableConnectPrinter=%d"
				, m_szIP, nSpanSec , m_nCheckConnectTimeOutSecond, m_bEnableConnectPrinter);


			if (m_bEnableConnectPrinter && m_pCallBack)
			{
				m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::NetWorkDisconnectOfPrinter,"打印网关与打印机的网络不通");
			}

			dwTick3 = GetTickCount();

			//结束检测预警信息
			CheckEnd();

			dwTick4 = GetTickCount();

			DWORD dwCost1 = dwTick2 - dwTick1;
			DWORD dwCost2 = dwTick3 - dwTick2;
			DWORD dwCost3 = dwTick4 - dwTick3;
			DWORD dwMax = 5000;
			if(dwMax < dwCost1
				|| dwMax < dwCost2
				|| dwMax < dwCost3)
			{
				theLog.Write("!@CSnmpMonitor::CheckStatus,20,PrtIP=%s,end,Cost TOO Much Time,dwCost1=%d,dwCost2=%d,dwCost3=%d"
					, m_szIP, dwCost1, dwCost2, dwCost3);
			}
			else
			{
//				theLog.Write("CSnmpMonitor::CheckStatus,20,PrtIP=%s,end,dwCost1=%d,dwCost2=%d,dwCost3=%d"
//					, m_szIP, dwCost1, dwCost2, dwCost3);
			}

			m_nLastCheckConnectTimeOutSecond = GetTickCount();	//add by zfq
		}

	}

	m_nCheckTick++;
}

unsigned __stdcall CSnmpMonitor::ThreadFn(void*pvParam)
{
	CSnmpMonitor* pThis = (CSnmpMonitor*)pvParam;
	
	SetEvent(pThis->m_hThreadStarted);

	theLog.Write("CSnmpMonitor::ThreadFn,begin,PrtIP=%s", pThis->GetPrtIP());

	//开启snmp
	if (!pThis->InitSnmp())
	{
		theLog.Write("!!CSnmpMonitor::ThreadFn,1");
	}

	//开始作业监视
	pThis->StartJobMonitor();

	theLog.Write("CSnmpMonitor::ThreadFn,2");
	//开始打印机抄表操作
	pThis->StartPrinterMeterMonitor(); 

	theLog.Write("CSnmpMonitor::ThreadFn,2.2");
	//开始监视打印机状态
	pThis->StartPrinterMonitorHelper();

	theLog.Write("CSnmpMonitor::ThreadFn,3");
	while (pThis->IsRunning())
	{
		DWORD dwWait = WaitForSingleObject(pThis->m_hThreadTerm,1000);
		switch (dwWait)
		{
		case WAIT_OBJECT_0:
			{
				//_endthreadex(1);
				theLog.Write("CSnmpMonitor::ThreadFn,4");
				goto threadend;
			}
			break;
		case WAIT_TIMEOUT:
			{
				//theLog.Write("CSnmpMonitor::ThreadFn,5");
				pThis->CheckStatus();
			}
			break;
		default:
			{
				theLog.Write("CSnmpMonitor::ThreadFn,6");
			}
			break;
		}//switch
	}
threadend:

	theLog.Write("CSnmpMonitor::ThreadFn,7");

	//结束打印监视
 	pThis->ReleaseSnmp();

	theLog.Write("CSnmpMonitor::ThreadFn,8");
	//结束作业监视
	pThis->StopJobMonitor();


	theLog.Write("CSnmpMonitor::ThreadFn,9");
	//结束打印机抄表操作
	pThis->StopPrinterMeterMonitor();

	theLog.Write("CSnmpMonitor::ThreadFn,10");
	//结束监视打印机状态
	pThis->StopPrinterMonitorHelper();

	theLog.Write("CSnmpMonitor::ThreadFn,end");
	return 0;
}
//设备状态
void CSnmpMonitor::CheckDeviceStatus()
{
	Owner ow(m_csLock);
	char szOid[50] = {".1.3.6.1.2.1.25.3.5.1.1.1"};
	
	int nVal = -1;
	if(GetRequest(szOid,nVal))
	{
		if (nVal == 1)
		{
			//other
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_OTHER;
		}
		else if (nVal == 2)
		{
			//unknown
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_UNKNOWN;
		}
		else if (nVal == 3)
		{
			//空闲
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_IDLE;
			if (m_pCallBack)
			{
				DWORD dw = GetTickCount();
				m_pCallBack->OnSCPIdle((dw - m_lTickCount) /1000);
			}
		}
		else if (nVal == 4)
		{
			//打印
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_PRINTING;
			m_lTickCount = GetTickCount();
		}
		else if (nVal == 5)
		{
			//预热
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_WARMUP;
		}
		else
		{
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_OTHER;
		}
		//theLog.Write("CSnmpMonitor::CheckDeviceStatus,nVal=%d",nVal);
	}
	else
	{
		if (CheckIPActive())
		{
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_OTHER;
			m_bEnableConnectPrinter = FALSE;
		}
		else
		{
			theLog.Write("!!CheckDeviceStatus GetRequest fail,打印网关与打印机的网络不通");
			m_oStatus.m_eDeviceStatus = FX_DC_NET_ERR;
			m_bEnableConnectPrinter = TRUE;
		}
	}
}

//获取打印机状态
void CSnmpMonitor::GetPrinterStatus(CSCPStatus& status)
{
	Owner ow(m_csLock);
	memcpy(&status, &m_oStatus, sizeof(CSCPStatus));
}

//操作状态
void CSnmpMonitor::CheckOPStatus()
{
	if (BT_CANON == m_eType)
	{
		CheckCanonOPStatus();
	}
	else
	{
		Owner ow(m_csLock);
		char szOid[50] = {".1.3.6.1.2.1.25.3.2.1.5.1"};
		int nVal = -1;
		if(GetRequest(szOid,nVal))
		{
			if (nVal == 3)
			{
				m_oStatus.m_eOPStatus = FX_DC_OP_IDLE;
			}
			//theLog.Write("CSnmpMonitor::CheckOPStatus,nVal=%d",nVal);
		}
		else
		{
			theLog.Write("!!CSnmpMonitor::CheckOPStatus,fail,err=%d", GetLastError());
		}
	}
	
}

//佳能操作状态
void CSnmpMonitor::CheckCanonOPStatus()
{
	//佳能有三个devicestatus，每个默认状态是running(2),其它值是异常状态
	Owner ow(m_csLock);
	char szOid1[50] = {".1.3.6.1.2.1.25.3.2.1.5.1"};
	char szOid2[50] = {".1.3.6.1.2.1.25.3.2.1.5.101"};
	char szOid3[50] = {".1.3.6.1.2.1.25.3.2.1.5.102"};
	int nStatus1 = -1;
	int nStatus2 = -1;
	int nStatus3 = -1;
	if(GetRequest(szOid1,nStatus1))
	{
		//theLog.Write("CSnmpMonitor::CheckOPStatus,nVal=%d",nVal);
	}
	else
	{
		//theLog.Write("!!CSnmpMonitor::CheckOPStatus,1,fail,err=%d", GetLastError());
	}
	if(GetRequest(szOid2,nStatus2))
	{
		//theLog.Write("CSnmpMonitor::CheckOPStatus,nVal=%d",nVal);
	}
	else
	{
		//theLog.Write("!!CSnmpMonitor::CheckOPStatus,2,fail,err=%d", GetLastError());
	}
	if(GetRequest(szOid3,nStatus3))
	{
		//theLog.Write("CSnmpMonitor::CheckOPStatus,nVal=%d",nVal);
	}
	else
	{
		//theLog.Write("!!CSnmpMonitor::CheckOPStatus,3,fail,err=%d", GetLastError());
	}
	if (nStatus1 != 2 || nStatus2 != 2 || nStatus3 != 2)
	{
		m_oStatus.m_eOPStatus = FX_DC_DEVICE_OTHER;
	}
	else
	{
		m_oStatus.m_eOPStatus = FX_DC_OP_IDLE;
	}
	
}

void CSnmpMonitor::CheckLmInk()
{
	return CheckInk();
}

//检测墨盒
void CSnmpMonitor::CheckInk()
{
	Owner ow(m_csLock);

	CString sTotalInkOid = ".1.3.6.1.2.1.43.11.1.1.8.1.";
	CString sLeftInkOid = ".1.3.6.1.2.1.43.11.1.1.9.1.";
	for (int n = 1; n < Max_Ink_Index; n++)
	{
		if (m_aryInk[n] > FX_DC_INK_NULL)
		{
			int nTotal = 100;
			int nVal = -1;
			CString sTotalTmp;
			sTotalTmp.Format(_T("%s%d"),sTotalInkOid,n);
			CString sLeftTmp;
			sLeftTmp.Format(_T("%s%d"),sLeftInkOid,n);
			if (GetRequest(CStringToChar(sTotalTmp), nTotal))
			{
				if(GetRequest(CStringToChar(sLeftTmp),nVal))
				{
					double fRate = nVal * 1.0 / nTotal;
					int nLeft = (int)(fRate * 100);
					m_oStatus.m_aryInk[n] = FX_DC_FIR_INK_OK;

					//theLog.Write("CheckInk(%d) total=%d,left=%d rate=%d",n,nTotal,nVal,nLeft);
					if (m_pCallBack)
					{
						CString sDesc;
						if (nLeft <= 0)
						{
							m_oStatus.m_aryInk[n] = FX_DC_FIR_INK_NO;
							sDesc.Format("第%d个墨盒无粉，请安装新墨盒！",n);
							m_pCallBack->OnStatus(FX_DC_FIR_INK_NO,CStringToChar(sDesc));
							theLog.Write("CheckInk(%d) total=%d,left=%d rate=%d,sDesc=%s",n,nTotal,nVal,nLeft,sDesc);
						}
						else if (nLeft <= m_nMinInk)
						{
							m_oStatus.m_aryInk[n] = FX_DC_FIR_INK_LESS;
							sDesc.Format("第%d个墨盒少粉，请准备备用墨盒！",n);
							m_pCallBack->OnStatus(FX_DC_FIR_INK_LESS,CStringToChar(sDesc));
						}
					}
				}
				else
				{
					theLog.Write("!!CSnmpMonitor::CheckInk,1,fail,err=%d", GetLastError());
				}
			}
			else
			{
				theLog.Write("!!CSnmpMonitor::CheckInk,2,fail,err=%d", GetLastError());
			}
		}
	}
}

void CSnmpMonitor::CheckPrinterMeterInfo()
{
	Owner ow(m_csLock4MeterMonitor);

	//theLog.Write("CSnmpMonitor::CheckPrinterMeterInfo,begin");
	if (m_bEnablePrinterMeterMonitor && m_pMeterMonitor)
	{
		m_pMeterMonitor->CheckMeterInfo();
	}
	//theLog.Write("CSnmpMonitor::CheckPrinterMeterInfo,end");
}

void CSnmpMonitor::CheckPrinterMonitorInfo()
{
	Owner ow(m_csLock4PrinterMonitorHelper);

	//theLog.Write("CSnmpMonitor::CheckPrinterMonitorInfo,begin");
	if (m_bEnablePrinterMonitor && m_pPrinterMonitorHelper)
	{
		m_pPrinterMonitorHelper->CheckMonitor();
	}
	//theLog.Write("CSnmpMonitor::CheckPrinterMonitorInfo,end");
}

void CSnmpMonitor::CheckLmPaper()
{
	Owner ow(m_csLock);

	CString sOid = ".1.3.6.1.2.1.43.8.2.1.11.1.";

	for (int n = 1; n < Max_Paper_Index; n++)
	{
		if (m_aryPaper[n] > 0)
		{
			CString sTmp;
			sTmp.Format(_T("%s%d"),sOid,n);
			int nVal = -1;
			CString sDesc;
			if(GetRequest(CStringToChar(sTmp),nVal))
			{
				if (nVal == 17)
				{
					m_oStatus.m_aryPaper[n] = FX_DC_FIR_PAPER_NO;
					sDesc.Format("第%d个纸盒缺纸",n);
					theLog.Write("CheckPaper(%s)(%d) %d ,%s ",sTmp,n,nVal,sDesc);
				}
				else if (nVal == 12)
				{
					m_oStatus.m_aryPaper[n] = FX_DC_FIR_PAPER_LESS;
					sDesc.Format("第%d个纸盒少纸",n);
				}
				else
				{
					m_oStatus.m_aryPaper[n] = FX_DC_FIR_PAPER_OK;
					sDesc.Format("第%d个纸盒正常",n);
				}
				if (m_pCallBack)
				{
					
					m_pCallBack->OnStatus(m_oStatus.m_aryPaper[n],CStringToChar(sDesc));
				}
			}
			else
			{
				theLog.Write("!!CheckLmPaper err,err=%d", GetLastError());
			}
		}
	}
}

void CSnmpMonitor::CheckManualPaper()
{
	Owner ow(m_csLock);

	int nTrayNo = 0;
	if (m_eType == BT_KM)
	{
		nTrayNo = 8;	//利盟纸盒8为手送纸盒
	}
	else
	{
		m_oStatus.m_aryPaper[0] = FX_DC_FIR_PAPER_NO;	//默认为纸盒空
		return;
	}
	
	CString sTotalTemp;
	CString sLeftTemp;
	sTotalTemp.Format(_T(".1.3.6.1.2.1.43.8.2.1.9.1.%d"), nTrayNo);
	sLeftTemp.Format(_T(".1.3.6.1.2.1.43.8.2.1.10.1.%d"), nTrayNo);

	int nTotalVal = -1;
	int nLeftVal = -1;
	if(!GetRequest(CStringToChar(sTotalTemp),nTotalVal))
	{
		theLog.Write("!!CSnmpMonitor::CheckManualPaper,fail.sTotalTemp=[%s]",sTotalTemp);
		return;
	}
	if(!GetRequest(CStringToChar(sLeftTemp),nLeftVal))
	{
		theLog.Write("!!CSnmpMonitor::CheckManualPaper,fail.sLeftTemp=[%s]",sLeftTemp);
		return;
	}

	double fRate = nLeftVal * 1.0 / nTotalVal;
	int nRate = (int)(fRate * 100);

	CString sDesc;
	if (nLeftVal == 0 || nLeftVal == -2)
	{
		m_oStatus.m_aryPaper[0] = FX_DC_FIR_PAPER_NO;
		//sDesc.Format("第%d个纸盒%s缺纸",n,(m_aryPaper[n] == 4 ? "A4":"A3"));
	}
	else if (nLeftVal > 0 && nRate <= m_nMinPaper)
	{
		m_oStatus.m_aryPaper[0] = FX_DC_FIR_PAPER_LESS;
		//sDesc.Format("第%d个纸盒%s少纸",n,(m_aryPaper[n] == 4 ? "A4":"A3"));
	}
	else
	{
		m_oStatus.m_aryPaper[0] = FX_DC_FIR_PAPER_OK;
		//sDesc.Format("第%d个纸盒%s正常",n,(m_aryPaper[n] == 4 ? "A4":"A3"));
	}
	//theLog.Write("CSnmpMonitor::CheckManualPaper,手动进纸盒(%d),总量(%d),余量(%d),%s", nTrayNo, nTotalVal, nLeftVal, sDesc);
	if (m_pCallBack)
	{
		//m_pCallBack->OnStatus(m_oStatus.m_aryPaper[n],CStringToChar(sDesc));
	}
}

//纸盒状态
void CSnmpMonitor::CheckPaper()
{
	Owner ow(m_csLock);

	CString sTotalOid = ".1.3.6.1.2.1.43.8.2.1.9.1.";
	CString sLeftOid = ".1.3.6.1.2.1.43.8.2.1.10.1.";
	for (int n = 1; n < Max_Paper_Index; n++)
	{
		if (m_aryPaper[n] > 0)
		{
			CString sTotalTemp;
			CString sLeftTemp;
			sTotalTemp.Format(_T("%s%d"),sTotalOid,n);
			sLeftTemp.Format(_T("%s%d"),sLeftOid,n);
			int nTotalVal = -1;
			int nLeftVal = -1;
			if(!GetRequest(CStringToChar(sTotalTemp),nTotalVal))
			{
				theLog.Write("!!CSnmpMonitor::CheckPaper,fail.sTotalTemp=[%s]",sTotalTemp);
				return;
			}
			if(!GetRequest(CStringToChar(sLeftTemp),nLeftVal))
			{
				theLog.Write("!!CSnmpMonitor::CheckPaper,fail.sLeftTemp=[%s]",sLeftTemp);
				return;
			}

			double fRate = nLeftVal * 1.0 / nTotalVal;
			int nRate = (int)(fRate * 100);

			CString sDesc;
			if (nLeftVal == 0 || nLeftVal == -2)
			{
				m_oStatus.m_aryPaper[n] = FX_DC_FIR_PAPER_NO;
				sDesc.Format("第%d个纸盒%s缺纸",n,(m_aryPaper[n] == 4 ? "A4":"A3"));
				theLog.Write("CheckPaper,纸盒(%d),总量(%d),余量(%d),%s", n, nTotalVal, nLeftVal, sDesc);
			}
			else if (nLeftVal > 0 && nRate <= m_nMinPaper)
			{
				m_oStatus.m_aryPaper[n] = FX_DC_FIR_PAPER_LESS;
				sDesc.Format("第%d个纸盒%s少纸",n,(m_aryPaper[n] == 4 ? "A4":"A3"));
			}
			else
			{
				m_oStatus.m_aryPaper[n] = FX_DC_FIR_PAPER_OK;
				sDesc.Format("第%d个纸盒%s正常",n,(m_aryPaper[n] == 4 ? "A4":"A3"));
			}
			//theLog.Write("CheckPaper,纸盒(%d),总量(%d),余量(%d),%s", n, nTotalVal, nLeftVal, sDesc);
			if (m_pCallBack)
			{
				m_pCallBack->OnStatus(m_oStatus.m_aryPaper[n],CStringToChar(sDesc));
			}
		}
	}

	//检测手动进纸状态
	//CheckManualPaper();
}
void CSnmpMonitor::CheckLmDeviceStatus()
{
	Owner ow(m_csLock);
	char szOid[50] = {".1.3.6.1.2.1.43.18.1.1.7"};

	int nVal = -1;
	if(GetNextRequest(szOid,nVal))
	{
		CString sDesc;
		if (nVal == 1)
		{
			
		}
		else if (nVal == 2)
		{
			
		}
		//卡纸
		else if (nVal == 8)
		{
			sDesc.Format("打印机卡纸");
		}
		//卡纸
		else if (nVal == 801)
		{
			sDesc.Format("打印机纸盒抽出或者缺少");
		}
		//少纸
		else if (nVal == 807)
		{
			sDesc.Format("打印机少纸");
		}
		//缺纸
		else if (nVal == 808)
		{
			sDesc.Format("打印机缺纸");
		}
		// 纸盒抽出
		else if (nVal == 901)
		{
			sDesc.Format("打印机纸盒抽出");
		}
		if (m_pCallBack && !sDesc.IsEmpty())
		{
			m_pCallBack->OnStatus(FX_DC_ERR,CStringToChar(sDesc));
		}
		//theLog.Write("CheckLmDeviceStatus %d",nVal);
	}
	else
	{
		if (CheckIPActive())
		{
			m_oStatus.m_eDeviceStatus = FX_DC_DEVICE_OTHER;
			m_bEnableConnectPrinter = FALSE;
		}
		else
		{
			theLog.Write("!!CheckLmDeviceStatus fail,打印网关与打印机的网络不通");
			m_oStatus.m_eDeviceStatus = FX_DC_NET_ERR;
			m_bEnableConnectPrinter = TRUE;
		}
	}
}

//打印页数
void CSnmpMonitor::CheckPrintCount()
{
	BOOL bIsPrinting = FALSE;

#if 0
	if (m_pJobMonitor)
	{
		int nPrintColor = m_pJobMonitor->CheckLastPrintCount_Color();
		int nPrintWB = m_pJobMonitor->CheckLastPrintCount_WB();
#else
	PrinterMeterInfo oMeterInfo;
	if (GetPrinterMeterInfo(&oMeterInfo))
	{
		int nPrintColor = oMeterInfo.nA4PrintColorPage + oMeterInfo.nA3PrintColorPage + oMeterInfo.nOtherPrintColorPage;
		int nPrintWB = oMeterInfo.nA4PrintHBPage + oMeterInfo.nA3PrintHBPage + oMeterInfo.nOtherPrintHBPage;

#if 1
		//如果打印纸张全部为0，则表示可能无法获取到打印抄表，尝试将纸张抄表归类为打印抄表
		if ((nPrintColor <= 0) && (nPrintWB <= 0))
		{
			nPrintColor = oMeterInfo.nA4ColorTotalPage + oMeterInfo.nA3ColorTotalPage + oMeterInfo.nOtherOpColorPage;
			nPrintWB = oMeterInfo.nA4HBTotalPage + oMeterInfo.nA3HBTotalPage + oMeterInfo.nOtherOpHBPage;
		}

		//如果打印纸张全部为0，则表示可能无法获取到纸张抄表，尝试将总抄表抄表归类为打印黑白抄表
		if ((nPrintColor <= 0) && (nPrintWB <= 0))
		{
			nPrintColor = 0;
			nPrintWB = oMeterInfo.nAllTotalPage;
		}
#endif

#endif
		if ((m_nLastPrintCount_Color > 0) && (nPrintColor > m_nLastPrintCount_Color))
		{
			theLog.Write("CSnmpMonitor::CheckPrintCount,1,nPrintColor=%d,nPrintWB=%d",nPrintColor,nPrintWB);
			bIsPrinting = TRUE;
		}

		if ((m_nLastPrintCount_WB > 0) && (nPrintWB > m_nLastPrintCount_WB))
		{
			theLog.Write("CSnmpMonitor::CheckPrintCount,2,nPrintColor=%d,nPrintWB=%d",nPrintColor,nPrintWB);
			bIsPrinting = TRUE;
		}
		m_nLastPrintCount_Color = nPrintColor;
		m_nLastPrintCount_WB = nPrintWB;
		m_nLastPrintCount = m_nLastPrintCount_Color + m_nLastPrintCount_WB;
	}
	if (bIsPrinting)
	{
		if (m_pCallBack)
		{
			m_pCallBack->OnStatus(FX_DC_DEVICE_PRINTING,0);
		}
	}
	m_oStatus.m_ePrintStatus = bIsPrinting ? FX_DC_DEVICE_PRINTING : FX_DC_DEVICE_OTHER;
}

void CSnmpMonitor::CheckCopyCount()
{
	BOOL bIsCopying = FALSE;
	CString szCopyInfo = "";	//格式为："PAPER_COLOR|PAPER_TYPE|dwPageCount"

#if 0
	if (m_pJobMonitor)
	{
		int nCopyColor = m_pJobMonitor->CheckLastCopyCount_Color();
		int nCopyWB = m_pJobMonitor->CheckLastCopyCount_WB();
#else
	PrinterMeterInfo oMeterInfo;
	if (GetPrinterMeterInfo(&oMeterInfo))
	{
		int nA4CopyColor = oMeterInfo.nA4CopyColorPage + oMeterInfo.nOtherCopyColorPage;
		int nA3CopyColor = oMeterInfo.nA3CopyColorPage;
		int nA4CopyWB = oMeterInfo.nA4CopyHBPage + oMeterInfo.nOtherCopyHBPage;
		int nA3CopyWB = oMeterInfo.nA3CopyHBPage;
#endif
		if ((m_nLastA4CopyCount_Color > 0) && (nA4CopyColor > m_nLastA4CopyCount_Color))
		{
			int nPageCount = nA4CopyColor - m_nLastA4CopyCount_Color;
			theLog.Write("CSnmpMonitor::CheckCopyCount,1,nA4CopyColor=%d,m_nLastA4CopyCount_Color=%d,nPageCount=%d"
				,nA4CopyColor,m_nLastA4CopyCount_Color,nPageCount);
			bIsCopying = TRUE;
			szCopyInfo.Format("%d|%d|%d", COLOR_PAPER, A4_PAPER, nPageCount);
			if (m_pCallBack)
			{
				m_pCallBack->OnStatus(FX_DC_DEVICE_COPYING,CStringToChar(szCopyInfo));
			}
		}

		if ((m_nLastA4CopyCount_WB > 0) && (nA4CopyWB > m_nLastA4CopyCount_WB))
		{
			int nPageCount = nA4CopyWB - m_nLastA4CopyCount_WB;
			theLog.Write("CSnmpMonitor::CheckCopyCount,2,nA4CopyWB=%d,m_nLastA4CopyCount_WB=%d,nPageCount=%d"
				,nA4CopyWB,m_nLastA4CopyCount_WB,nPageCount);
			bIsCopying = TRUE;
			szCopyInfo.Format("%d|%d|%d", WB_PAPER, A4_PAPER, nPageCount);
			if (m_pCallBack)
			{
				m_pCallBack->OnStatus(FX_DC_DEVICE_COPYING,CStringToChar(szCopyInfo));
			}
		}

		if ((m_nLastA3CopyCount_Color > 0) && (nA3CopyColor > m_nLastA3CopyCount_Color))
		{
			int nPageCount = nA3CopyColor - m_nLastA3CopyCount_Color;
			theLog.Write("CSnmpMonitor::CheckCopyCount,3,nA3CopyColor=%d,m_nLastA3CopyCount_Color=%d,nPageCount=%d"
				,nA3CopyColor,m_nLastA3CopyCount_Color,nPageCount);
			bIsCopying = TRUE;
			szCopyInfo.Format("%d|%d|%d", COLOR_PAPER, A3_PAPER, nPageCount);
			if (m_pCallBack)
			{
				m_pCallBack->OnStatus(FX_DC_DEVICE_COPYING,CStringToChar(szCopyInfo));
			}
		}

		if ((m_nLastA3CopyCount_WB > 0) && (nA3CopyWB > m_nLastA3CopyCount_WB))
		{
			int nPageCount = nA3CopyWB - m_nLastA3CopyCount_WB;
			theLog.Write("CSnmpMonitor::CheckCopyCount,4,nA3CopyWB=%d,m_nLastA3CopyCount_WB=%d,nPageCount=%d"
				,nA3CopyWB,m_nLastA3CopyCount_WB,nPageCount);
			bIsCopying = TRUE;
			szCopyInfo.Format("%d|%d|%d", WB_PAPER, A3_PAPER, nPageCount);
			if (m_pCallBack)
			{
				m_pCallBack->OnStatus(FX_DC_DEVICE_COPYING,CStringToChar(szCopyInfo));
			}
		}		

		m_nLastA4CopyCount_Color = nA4CopyColor;
		m_nLastA3CopyCount_Color = nA3CopyColor;
		m_nLastA4CopyCount_WB = nA4CopyWB;
		m_nLastA3CopyCount_WB = nA3CopyWB;
		m_nLastCopyCount = nA4CopyColor + nA3CopyColor	+ nA4CopyWB + nA3CopyWB;
	}

	m_oStatus.m_eCopyStatus = bIsCopying ? FX_DC_DEVICE_COPYING : FX_DC_DEVICE_OTHER;
}

void CSnmpMonitor::CheckScanCount()
{
	BOOL bIsScaning = FALSE;

#if 0
	if (m_pJobMonitor)
	{
		int nScanColor = m_pJobMonitor->CheckLastScanCount_Color();
		int nScanWB = m_pJobMonitor->CheckLastScanCount_WB();
#else
	PrinterMeterInfo oMeterInfo;
	if (GetPrinterMeterInfo(&oMeterInfo))
	{
		int nScanColor = oMeterInfo.nScanBigColorPage + oMeterInfo.nScanSmallColorPage;
		int nScanWB = oMeterInfo.nScanBigHBPage + oMeterInfo.nScanSmallHBPage;
#endif
		if ((m_nLastScanCount_Color > 0) && (nScanColor > m_nLastScanCount_Color))
		{
			theLog.Write("CSnmpMonitor::CheckScanCount,1,nScanColor=%d,nScanWB=%d",nScanColor,nScanWB);
			bIsScaning = TRUE;
		}

		if ((m_nLastScanCount_WB > 0) && (nScanWB > m_nLastScanCount_WB))
		{
			theLog.Write("CSnmpMonitor::CheckScanCount,2,nScanColor=%d,nScanWB=%d",nScanColor,nScanWB);
			bIsScaning = TRUE;
		}
		m_nLastScanCount_Color = nScanColor;
		m_nLastScanCount_WB = nScanWB;
		m_nLastScanCount = m_nLastScanCount_Color + m_nLastScanCount_WB;
	}

	if (bIsScaning)
	{
		if (m_pCallBack)
		{
			m_pCallBack->OnStatus(FX_DC_DEVICE_SCANING,0);
		}
	}
	m_oStatus.m_eScanStatus = bIsScaning ? FX_DC_DEVICE_SCANING : FX_DC_DEVICE_OTHER;
}

void CSnmpMonitor::CheckFaxCount()
{
	BOOL bIsFaxing = FALSE;

#if 0
	if (m_pJobMonitor)
	{
		int nFaxColor = m_pJobMonitor->CheckLastFaxCount_Color();
		int nFaxWB = m_pJobMonitor->CheckLastFaxCount_WB();
#else
	PrinterMeterInfo oMeterInfo;
	if (GetPrinterMeterInfo(&oMeterInfo))
	{
		int nFaxColor = oMeterInfo.nA4FaxColorPage + oMeterInfo.nA3FaxColorPage + oMeterInfo.nOtherFaxColorPage;
		int nFaxWB = oMeterInfo.nA4FaxHBPage + oMeterInfo.nA3FaxHBPage + oMeterInfo.nOtherFaxHBPage;
#endif
		if ((m_nLastFaxCount_Color > 0) && (nFaxColor > m_nLastFaxCount_Color))
		{
			theLog.Write("CSnmpMonitor::CheckFaxCount,1,nFaxColor=%d,nFaxWB=%d",nFaxColor,nFaxWB);
			bIsFaxing = TRUE;
		}

		if ((m_nLastFaxCount_WB > 0) && (nFaxWB > m_nLastFaxCount_WB))
		{
			theLog.Write("CSnmpMonitor::CheckFaxCount,2,nFaxColor=%d,nFaxWB=%d",nFaxColor,nFaxWB);
			bIsFaxing = TRUE;
		}
		m_nLastFaxCount_Color = nFaxColor;
		m_nLastFaxCount_WB = nFaxWB;
		m_nLastFaxCount = m_nLastFaxCount_Color + m_nLastFaxCount_WB;
	}

	if (bIsFaxing)
	{
		if (m_pCallBack)
		{
			m_pCallBack->OnStatus(FX_DC_DEVICE_FAXING,0);
		}
	}
	m_oStatus.m_eFaxStatus = bIsFaxing ? FX_DC_DEVICE_FAXING : FX_DC_DEVICE_OTHER;
}

void CSnmpMonitor::CheckJob()
{
	Owner ow(m_csLock4JobMonitor);

	if (m_pJobMonitor && m_bEnablePrinterJobMonitor)
	{
		m_pJobMonitor->CheckJob();
	}
}

void CSnmpMonitor::SetPaperConfig(CString sConfig, int nMin)
{
	Owner ow(m_csLock);

	theLog.Write("CSnmpMonitor::SetPaperConfig,sConfig=[%s],nMin=%d", sConfig, nMin);
	m_nMinPaper = nMin;
	memset(m_aryPaper,0,sizeof(m_aryPaper));
	if(sConfig.IsEmpty())
	{
		m_aryPaper[1] = 4;
	}
	else
	{
		CStringArray ary;
		CCommonFun::StringSplit(sConfig,&ary,",");
		for (int n = 0; n < ary.GetCount(); n ++)
		{
			CString s = ary.GetAt(n);
			if (s.GetLength() > 0 )
			{
				int nPos = atoi(s);
				if (nPos >= Max_Paper_Index)
				{
					theLog.Write("!!CSnmpMonitor::SetPaperConfig,超出最大纸盒数,max=%d,current=%d", Max_Paper_Index, nPos);
					continue;
				}
				s.MakeLower();
				int a4 = 0;
				if(s.Find("a4") > 0)
				{
					a4 = 1;
				}
				if (a4)
				{
					m_aryPaper[nPos] = 4;
				}
				else
				{
					m_aryPaper[nPos] = 3;
				}
			}
		}
	}
}

void CSnmpMonitor::SetInkConfig(CString sConfig, int nMin)
{
	Owner ow(m_csLock);

	theLog.Write("CSnmpMonitor::SetInkConfig,sConfig=[%s],nMin=%d",sConfig,nMin);
	m_nMinInk = nMin;
	memset(m_aryInk,FX_DC_INK_NULL,sizeof(m_aryInk));

	if(sConfig.IsEmpty())
	{
		m_aryInk[1] = FX_DC_INK_BLACK;
	}
	else
	{
		CStringArray ary;
		CCommonFun::StringSplit(sConfig,&ary,",");
		for (int n = 0; n < ary.GetCount(); n ++)
		{
			CString s = ary.GetAt(n);
			int nPos = atoi(s.Left(1));
			if (nPos >= Max_Ink_Index)
			{
				theLog.Write("!!CSnmpMonitor::SetPaperConfig,超出最大墨盒数,max=%d,current=%d",Max_Ink_Index, nPos);
				continue;
			}
			m_aryInk[nPos] = (FX_DC_INK_CMYK)atoi(s.Right(1));
		}
	}
}

BOOL CSnmpMonitor::Start(const char* szAddr,ISCPStatusInterface* pCall)
{
	Owner ow(m_csLock);
	theLog.Write("CSnmpMonitor::Start,szAddr=%s,pCall=%p", szAddr, pCall);

	if(0 != m_hThreadTerm)
	{
		theLog.Write("!+CSnmpMonitor::Start,Already has been Started!");
		return TRUE;
	}

	SetCallBack(pCall);
	SetConnect(szAddr);
	CreateTh();
	return TRUE;
}

void CSnmpMonitor::Stop()
{
//	Owner ow(m_csLock);	//del by zfq,2015-08-25

	theLog.Write("CSnmpMonitor::Stop,1");

	InterlockedExchange(&m_lActive,0);

	theLog.Write("CSnmpMonitor::Stop,2");

	Shutdown();

	theLog.Write("CSnmpMonitor::Stop,3");
}

void CSnmpMonitor::StartJobMonitor()
{
	Owner ow(m_csLock4JobMonitor);

	/*
	if (!m_bEnablePrinterJobMonitor)
	{
		theLog.Write("##CSnmpMonitor::StartJobMonitor,PrtIP=%s,m_bEnablePrinterJobMonitor=%d"
			, m_szIP, m_bEnablePrinterJobMonitor);
		return;
	}*/

	StopJobMonitor();
	if (m_eType == BT_SL || m_eType == BT_SL2)
	{
		m_pJobMonitor = new CSnmpJobMonitorSL();
	}
	else if (m_eType == BT_DZ)
	{
		m_pJobMonitor = new CSnmpJobMonitorDZ();
	}
	//del by zxl,20160715,
	//作业监视的目的是打印异常退费,这个监视很不准确,
	//现在采用两种方案:
	//一,通过snmp获取打印历史作业列表,来进行单个校准.
	//二,通过snmp抄表,比较两次登录的抄表差值,多个作业整批校准.
	//三,通过打印机厂商提供的SDK来获取打印历史作业列表.
	//目前只有施乐支持snmp获取打印历史作业列表的功能.
	//东芝支持厂商提供的SDK来获取打印历史作业列表.
#if 0
	else if (m_eType == BT_KM || m_eType == BT_ZD)
	{
		m_pJobMonitor = new CSnmpJobMonitorKM();
	}
	else if (m_eType == BT_LG)
	{
		m_pJobMonitor = new CSnmpJobMonitorLG();
	}
	else if (m_eType == BT_JC)
	{
		m_pJobMonitor = new CSnmpJobMonitorJC();
	}
#endif
	else
	{
		m_pJobMonitor = NULL;
		theLog.Write("##CSnmpMonitor::StartJobMonitor, m_eType=%d, No Support SNMP Job Monitor", m_eType);
	}

	if (m_pJobMonitor)
	{
		m_pJobMonitor->SetCallBack(m_pCallBack);
		m_pJobMonitor->SetConnect(m_szIP, m_szCommunity);
		m_pJobMonitor->InitSnmp();
	}
}

void CSnmpMonitor::StopJobMonitor()
{
	Owner ow(m_csLock4JobMonitor);

	if (m_pJobMonitor)
	{
		m_pJobMonitor->Release();
		delete m_pJobMonitor;
		m_pJobMonitor = NULL;
	}
}

void CSnmpMonitor::StartPrinterMeterMonitor()
{
	Owner ow(m_csLock4MeterMonitor);

	/*
	if (!m_bEnablePrinterMeterMonitor)
	{
		theLog.Write("##CSnmpMonitor::StartPrinterMeterMonitor,PrtIP=%s,m_bEnablePrinterMeterMonitor=%d"
			, m_szIP, m_bEnablePrinterMeterMonitor);
		return;
	}
	*/

	StopPrinterMeterMonitor();
	if (m_eType == BT_LM)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorLM();
	}
	else if (m_eType == BT_SL || m_eType == BT_SL2)
	{
        m_pMeterMonitor = new CSnmpPrinterMeterMonitorSL();
	}
	else if (m_eType == BT_KM || m_eType == BT_ZD)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorKM();
	}
	else if (m_eType == BT_HP)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorHP();
	}
	else if (m_eType == BT_LG)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorLG();
	}
	else if (m_eType == BT_DZ)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorDZ();
	}
	else if (m_eType == BT_JC)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorJC();
	}
	else if (m_eType == BT_SX)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorSX();
	}
	else if (m_eType == BT_XP)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorXP();
	}
	else if (m_eType == BT_EPSON)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorEPSON();
	}
	else if (m_eType == BT_CANON)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorCANON();
	}
	else if(BT_JT == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorJT;
	}
	else if(BT_BROTHER == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorBrother;
	}
	else if(BT_OKI == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorOKI;
	}
	else if(BT_SEIKO == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorSEIKO;
	}
	else if(BT_PANTUM == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorPT;
	}
	else if(BT_PRINTRONIX == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorPRONNIX;
	}
	else if(BT_TSC == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorTSC;
	}
	else if (BT_DASCOM == m_eType)
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitorDASCOM;
	}
	else
	{
		m_pMeterMonitor = new CSnmpPrinterMeterMonitor();
		theLog.Write("##CSnmpMonitor::StartPrinterMeterMonitor, m_eType=%d, No Support, 启用通用操作", m_eType);
	}

	if (m_pMeterMonitor)
	{
		m_pMeterMonitor->SetCallBack(m_pCallBack);
		m_pMeterMonitor->SetBrandType(m_eType);
		m_pMeterMonitor->SetConnect(m_szIP, m_szCommunity);
		m_pMeterMonitor->InitSnmp();
	}
}

void CSnmpMonitor::StopPrinterMeterMonitor()
{
	Owner ow(m_csLock4MeterMonitor);

	if (m_pMeterMonitor)
	{
		m_pMeterMonitor->Release();
		delete m_pMeterMonitor;
		m_pMeterMonitor = NULL;
	}
}

void CSnmpMonitor::StartPrinterMonitorHelper()
{
	Owner ow(m_csLock4PrinterMonitorHelper);

	/*
	if (!m_bEnablePrinterMonitor)
	{
		theLog.Write("##CSnmpMonitor::StartPrinterMonitorHelper, m_bEnablePrinterMonitor=%d", m_bEnablePrinterMonitor);
		return;
	}
	*/

	StopPrinterMonitorHelper();

	if (m_eType == BT_LG)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelperLG;
	}
	else if(m_eType == BT_SL || m_eType == BT_SL2)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelperSL;
	}
	else if(m_eType == BT_CANON)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelperCANON;
	}
	else if (BT_JT == m_eType)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelper_JT;
	}
	else if(BT_SEIKO == m_eType)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelper_SEIKO;
	}
	else if(BT_PANTUM == m_eType)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelperPT;
	}
	else if(BT_HP == m_eType)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelperHP;
	}
	else if(BT_DASCOM == m_eType)
	{
		m_pPrinterMonitorHelper = new CSnmpMonitorHelperDASCOM;
	}
	else
	{
		//默认为使用通用监视操作类
		m_pPrinterMonitorHelper = new CSnmpMonitorHelper;
	}

	if (m_pPrinterMonitorHelper)
	{
		m_pPrinterMonitorHelper->SetCallBack(m_pCallBack);
		m_pPrinterMonitorHelper->SetBrandType(m_eType);
		m_pPrinterMonitorHelper->SetPrinterMeterMonitor(m_pMeterMonitor);
		m_pPrinterMonitorHelper->SetPaperConfig("", m_nMinPaper);
		m_pPrinterMonitorHelper->SetInkConfig("", m_nMinInk);
		m_pPrinterMonitorHelper->SetConnect(m_szIP, m_szCommunity);
		m_pPrinterMonitorHelper->InitSnmp();
	}
}

void CSnmpMonitor::StopPrinterMonitorHelper()
{
	Owner ow(m_csLock4PrinterMonitorHelper);

	if (m_pPrinterMonitorHelper)
	{
		m_pPrinterMonitorHelper->Release();
		delete m_pPrinterMonitorHelper;
		m_pPrinterMonitorHelper = NULL;
	}
}

//监视操作的标记操作
void CSnmpMonitor::CheckBegin()
{
	if (m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterMonitorCheckBegin, "开始检测");
	}
}

void CSnmpMonitor::CheckProcess()
{
	if (m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterMonitorCheckProcess, "正在检测");
	}
}

void CSnmpMonitor::CheckEnd()
{
	if (m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterMonitorCheckEnd, "结束检测");
	}
}

//少纸
BOOL CSnmpMonitor::IsLowerPager(CString& sDesc)
{
	Owner ow(m_csLock);
	sDesc.Empty();
	if(m_hThread)
	{
		for (int n = 1; n < Max_Paper_Index; n++)
		{
			if (m_aryPaper[n] > 0)
			{
				CString sTmp;
				//theLog.Write("CSnmpMonitor::IsLowerPager,paper(%d),%d",n,m_oStatus.m_aryPaper[n]);
				if(m_oStatus.m_aryPaper[n] == FX_DC_FIR_PAPER_LESS)
				{
					sTmp.Format(_T("第（%d）纸盒%s较少；"),n,(m_aryPaper[n] == 4 ? "A4":"A3"));
					sDesc += sTmp;
				}
				else if(m_oStatus.m_aryPaper[n] == FX_DC_FIR_PAPER_NO)
				{
					sTmp.Format(_T("第（%d）纸盒%s缺纸；"),n,(m_aryPaper[n] == 4 ? "A4":"A3"));
					sDesc += sTmp;
				}
			}
		}
	}
	return sDesc.IsEmpty() ? FALSE : TRUE;
}
//少墨
BOOL CSnmpMonitor::IsLowerInk(CString& sDesc)
{
	Owner ow(m_csLock);
	CString sTmp;
	sDesc.Empty();
	if(m_hThread)
	{
		for (int n = 1; n < Max_Ink_Index; n++)
		{
			//theLog.Write("Ink(%d),%d",n,m_oStatus.m_aryInk[n]);
			if(m_oStatus.m_aryInk[n] == FX_DC_FIR_INK_LESS)
			{
				CString sTmp;
				sTmp.Format("第%d个墨盒少粉，请准备备用墨盒；",n);
				sDesc += sTmp;
			}
		}
	}
	return sDesc.IsEmpty() ? FALSE : TRUE;
}

BOOL CSnmpMonitor::IsOutPager()
{
	Owner ow(m_csLock);
	if(m_hThread)
	{
		for (int n = 1; n < Max_Paper_Index; n++)
		{
			if (m_aryPaper[n] > 0)
			{
				//theLog.Write("paper(%d),%d",n,m_oStatus.m_aryPaper[n]);
				if(m_oStatus.m_aryPaper[n] != FX_DC_FIR_PAPER_NO)
				{
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CSnmpMonitor::IsOutPager(CString szPapeType)
{
	theLog.Write("CSnmpMonitor::IsOutPager,0.1");

	Owner ow(m_csLock);

	theLog.Write("CSnmpMonitor::IsOutPager,0.2");

	if(m_hThread)
	{
		theLog.Write("CSnmpMonitor::IsOutPager,0.3");

		for (int n = 1; n < Max_Paper_Index; n++)
		{
			if (m_aryPaper[n] > 0)
			{
				if(m_oStatus.m_aryPaper[n] != FX_DC_FIR_PAPER_NO)
				{
					CString sTmp;
					sTmp.Format("%s",(m_aryPaper[n] == 4 ? "A4":"A3"));
					if (szPapeType.CompareNoCase(sTmp)==0)
					{
//						theLog.Write("!!CSnmpMonitor::IsOutPager,1,szPapeType=%s", szPapeType);
						return FALSE;
					}
				}
			}
		}

		theLog.Write("CSnmpMonitor::IsOutPager,1.4,szPapeType=%s", szPapeType);
 
#pragma message("目前只支持A3,A4两个纸型,其它纸型暂时让它通过缺纸检测.")
		if (szPapeType.CompareNoCase("A4")!=0 && szPapeType.CompareNoCase("A3")!=0)
		{
			return FALSE;
		}
		else
		{
			theLog.Write("!!CSnmpMonitor::IsOutPager,2,szPapeType=%s", szPapeType);
			return TRUE;
		}

		theLog.Write("CSnmpMonitor::IsOutPager,2.1");
	}
	else //if(m_hThread)
	{
		theLog.Write("CSnmpMonitor::IsOutPager,3,m_hThread=0x%x", m_hThread);
	}
	return FALSE;
}

BOOL CSnmpMonitor::IsOutManualPaper()
{
	Owner ow(m_csLock);
	if(m_hThread)
	{
		return (m_oStatus.m_aryPaper[0] == FX_DC_FIR_PAPER_NO);
	}
	return TRUE;
}

// #define DEF_GuiZhouCaiJin	//贵州财经大学
BOOL CSnmpMonitor::IsJamPaper()
{
	Owner ow(m_csLock);
#ifdef DEF_GuiZhouCaiJin
	if (m_oStatus.m_eDeviceStatus == FX_DC_DEVICE_UNKNOWN)
	{
		return TRUE;
	}
#endif
	return FALSE;
}

BOOL CSnmpMonitor::IsOutInk()
{
	Owner ow(m_csLock);
	if(m_hThread)
	{
		for (int n = 1; n < Max_Ink_Index; n++)
		{
			if (m_aryInk[n] > FX_DC_INK_NULL)
			{
				//theLog.Write("ink(%d),%d",n,m_oStatus.m_aryInk[n]);
				if(m_oStatus.m_aryInk[n] != FX_DC_FIR_INK_NO)
				{
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CSnmpMonitor::IsOutNet()
{
	Owner ow(m_csLock);
	if(m_hThread)
	{
		if(m_oStatus.m_eDeviceStatus == FX_DC_NET_ERR)
		{
			return TRUE;
		}
	}
	return FALSE;
}

char* CSnmpMonitor::GetCmmt(FX_DC_SCP_STATUS eState)
{
	return m_aryCmmt[eState].szCmmt;
}

void CSnmpMonitor::InitCmmt()
{
	strcpy(m_aryCmmt[FX_DC_OP_IDLE].szCmmt,"操作空闲");

	strcpy(m_aryCmmt[FX_DC_DEVICE_OTHER].szCmmt,"打印机其他");
	strcpy(m_aryCmmt[FX_DC_DEVICE_UNKNOWN].szCmmt,"打印机未知");
	strcpy(m_aryCmmt[FX_DC_DEVICE_IDLE].szCmmt,"打印机空闲");
	strcpy(m_aryCmmt[FX_DC_DEVICE_PRINTING].szCmmt,"打印机正在打印");
	strcpy(m_aryCmmt[FX_DC_DEVICE_WARMUP].szCmmt,"打印机预热中");

	if(m_nA4Pos == 1)
	{
		strcpy(m_aryCmmt[FX_DC_FIR_PAPER_OK].szCmmt,"A4纸盒正常");
		strcpy(m_aryCmmt[FX_DC_FIR_PAPER_LESS].szCmmt,"A4纸盒少纸");
		strcpy(m_aryCmmt[FX_DC_FIR_PAPER_NO].szCmmt,"A4纸盒缺纸");
	}
	else if (m_nA4Pos == 2)
	{
		strcpy(m_aryCmmt[FX_DC_FIR_PAPER_OK].szCmmt,"A4纸盒正常");
		strcpy(m_aryCmmt[FX_DC_FIR_PAPER_LESS].szCmmt,"A4纸盒少纸");
		strcpy(m_aryCmmt[FX_DC_FIR_PAPER_NO].szCmmt,"A4纸盒缺纸");
	}

	if(m_nA3Pos == 1)
	{
		strcpy(m_aryCmmt[FX_DC_SEC_PAPER_OK].szCmmt,"A3纸盒正常");
		strcpy(m_aryCmmt[FX_DC_SEC_PAPER_LESS].szCmmt,"A3纸盒少纸");
		strcpy(m_aryCmmt[FX_DC_SEC_PAPER_NO].szCmmt,"A3纸盒缺纸");
	}
	else if (m_nA3Pos == 2)
	{
		strcpy(m_aryCmmt[FX_DC_SEC_PAPER_OK].szCmmt,"A3纸盒正常");
		strcpy(m_aryCmmt[FX_DC_SEC_PAPER_LESS].szCmmt,"A3纸盒少纸");
		strcpy(m_aryCmmt[FX_DC_SEC_PAPER_NO].szCmmt,"A3纸盒缺纸");
	}
/*
	strcpy(m_ary[FX_DC_SEC_PAPER_OK].szCmmt,"第二个纸盒正常");
	strcpy(m_ary[FX_DC_SEC_PAPER_LESS].szCmmt,"第二个纸盒少纸");
	strcpy(m_ary[FX_DC_SEC_PAPER_NO].szCmmt,"第二个纸盒缺纸");
*/
	strcpy(m_aryCmmt[FX_DC_FIR_INK_OK].szCmmt,"墨盒正常");
	strcpy(m_aryCmmt[FX_DC_FIR_INK_LESS].szCmmt,"墨盒少墨");
	strcpy(m_aryCmmt[FX_DC_FIR_INK_NO].szCmmt,"墨盒缺墨");

	
}

void CSnmpMonitor::InitBrand(BRAND_TYPE eType)
{
	Owner ow(m_csLock);

	m_eType = eType;
}

void CSnmpMonitor::EnablePrinterInfoMonitor(BOOL bEnable /*= TRUE*/, int nCheckInfoTimeOutSecond /*= 10*/)
{
	Owner ow(m_csLock);

	m_bEnablePrinterInfoMonitor = bEnable;
	m_nCheckInfoTimeOutSecond = nCheckInfoTimeOutSecond;
	theLog.Write("CSnmpMonitor::EnablePrinterInfoMonitor,PrtIP=%s, m_bEnablePrinterInfoMonitor=%d, m_nCheckInfoTimeOutSecond=%d"
		, m_szIP, m_bEnablePrinterInfoMonitor, m_nCheckInfoTimeOutSecond);
}

void CSnmpMonitor::EnablePrinterJobMonitor(BOOL bEnable, int nCheckJobTimeOutSecond /*= 10*/)
{
	Owner ow(m_csLock);

	m_bEnablePrinterJobMonitor = bEnable;
	m_nCheckJobTimeOutSecond = nCheckJobTimeOutSecond;
	theLog.Write("CSnmpMonitor::EnablePrinterJobMonitor,PrtIP=%s, m_bEnablePrinterJobMonitor=%d, m_nCheckJobTimeOutSecond=%d"
		, m_szIP, m_bEnablePrinterJobMonitor, m_nCheckJobTimeOutSecond);
}

void CSnmpMonitor::EnablePrinterMeterMonitor(BOOL bEnable, int nCheckMeterTimeOutSecond /*= 15*/)
{
	Owner ow(m_csLock);

	m_bEnablePrinterMeterMonitor = bEnable;
	m_nCheckMeterTimeOutSecond = nCheckMeterTimeOutSecond;
	theLog.Write("CSnmpMonitor::EnablePrinterMeterMonitor,PrtIP=%s, m_bEnablePrinterMeterMonitor=%d, m_nCheckMeterTimeOutSecond=%d"
		, m_szIP, m_bEnablePrinterMeterMonitor, m_nCheckMeterTimeOutSecond);
}

void CSnmpMonitor::EnablePrinterMonitor(BOOL bEnable, int nCheckMonitorTimeOutSecond /*= 60*/)
{
	Owner ow(m_csLock);

	m_bEnablePrinterMonitor = bEnable;
	m_nCheckMonitorTimeOutSecond = nCheckMonitorTimeOutSecond;
	theLog.Write("CSnmpMonitor::EnablePrinterMonitor,PrtIP=%s, m_bEnablePrinterMonitor=%d, m_nCheckMonitorTimeOutSecond=%d"
		, m_szIP, m_bEnablePrinterMonitor, m_nCheckMonitorTimeOutSecond);
}


//获取打印机纸盒状态信息
/*1.C++端通过snmp获取各个纸盒的信息
格式:            纸盒数;第一纸盒编号;第一纸盒纸型;第一纸盒状态;        示例[1;1;A4;21;] - 第一纸盒为A4,状态正常(FX_DC_FIR_PAPER_OK )
消息代码:        #define WM_PRINT_TRAY_INFO	(WM_USER+1061)      //纸盒信息
*/
CString CSnmpMonitor::GetPrinterPaperInfo()
{
	Owner ow(m_csLock);
	CString szPaperInfo;
	if(m_hThread)
	{
		int nTrayCount = 0;
		CString szTrayInfo;
		for (int n = 1; n < Max_Paper_Index; n++)
		{
			if (m_aryPaper[n] > 0)
			{
				//theLog.Write("paper(%d),%d",n,m_oStatus.m_aryPaper[n]);
				nTrayCount++;
				szTrayInfo.AppendFormat("%d;%s;%d;", n, (m_aryPaper[n] == 4 ? "A4":"A3"), m_oStatus.m_aryPaper[n]);
			}
		}
		if (nTrayCount>0)
		{
			szPaperInfo.Format("%d;%s", nTrayCount, szTrayInfo);
		}
	}
	return szPaperInfo;
}

//获取打印机墨盒状态信息
/*
2.C++端通过snmp获取各个墨盒的信息
格式:            墨盒数;第一墨盒编号;第一墨盒类型;第一墨盒状态;        示例[1;1;4;41;] - 第一墨盒为黑色(FX_DC_INK_BLACK),状态正常(FX_DC_FIR_INK_OK)
消息代码:        #define WM_PRINT_INK_INFO	(WM_USER+1062)      //墨盒信息
*/
CString CSnmpMonitor::GetPrinterInkInfo()
{
	Owner ow(m_csLock);
	CString szInkInfo;
	if(m_hThread)
	{
		int nInkCount = 0;
		CString szInkInfoTemp;
		for (int n = 1; n < Max_Ink_Index; n++)
		{
			if (m_aryInk[n] > FX_DC_INK_NULL)
			{
				//theLog.Write("ink(%d),%d",n,m_oStatus.m_aryInk[n]);
				nInkCount++;
				szInkInfoTemp.AppendFormat("%d;%d;%d;", n, m_aryInk[n], m_oStatus.m_aryInk[n]);
			}
		}
		if (nInkCount>0)
		{
			szInkInfo.Format("%d;%s", nInkCount, szInkInfoTemp);
		}
	}
	return szInkInfo;
}

//获取打印机抄表信息
BOOL CSnmpMonitor::GetPrinterMeterInfo(PrinterMeterInfo* pInfo)
{
	Owner ow(m_csLock4MeterMonitor);

	if (!pInfo || !m_pMeterMonitor)
	{
		theLog.Write("!!CSnmpMonitor::GetPrinterMeterInfo,1,pInfo=%p,m_pMeterMonitor=%p", pInfo, m_pMeterMonitor);
		return FALSE;
	}
	
	return m_pMeterMonitor->GetLastMeterInfo(pInfo);
}
