//柯美打印机抄表

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorKM : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorKM(void);
	~CSnmpPrinterMeterMonitorKM(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	BOOL IsBigPaperDoubleCounter();	//检测打印机抄表计数大纸计数是否是双倍
	CString m_szKmA4CopyHBPageOID;
	CString m_szKmA4CopyFullColorPageOID;
	CString m_szKmA4CopySingleColorPageOID;
	CString m_szKmA4CopyDoubleColorPage;
	CString m_szKmA3CopyHBPageOID;
	CString m_szKmA3CopyFullColorPageOID;
	CString m_szKmA3CopySingleColorPageOID;
	CString m_szKmA3CopyDoubleColorPageOID;
	CString m_szKmA4PrintHBPageOID;
	CString m_szKmA4PrintFullColorPageOID;
	CString m_szKmA4PrintSingleColorPageOID;
	CString m_szKmA4PrintDoubleColorPageOID;
	CString m_szKmA3PrintHBPageOID;
	CString m_szKmA3PrintFullColorPageOID;
	CString m_szKmA3PrintSingleColorPageOID;
	CString m_szKmA3PrintDoubleColorPageOID;
	CString m_szKmScanTotalPageOID;
	CString m_szKmScanBigColorPageOID;

	//system
	CString m_szSysDescrOID;
};
