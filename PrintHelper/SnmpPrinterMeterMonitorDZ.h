//东芝打印机抄表

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorDZ : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorDZ(void);
	~CSnmpPrinterMeterMonitorDZ(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szToshibaA4PrintFullColorPageOID;
	CString m_szToshibaA4PrintSingleColorPageOID;
	CString m_szToshibaA4PrintHBPageOID;
	CString m_szToshibaA3PrintFullColorPageOID;
	CString m_szToshibaA3PrintSingleColorPageOID;
	CString m_szToshibaA3PrintHBPageOID;
	CString m_szToshibaA4CopyFullColorPageOID;
	CString m_szToshibaA4CopySingleColorPageOID;
	CString m_szToshibaA4CopyHBPageOID;
	CString m_szToshibaA3CopyFullColorPageOID;
	CString m_szToshibaA3CopySingleColorPageOID;
	CString m_szToshibaA3CopyHBPageOID;
	CString m_szToshibaScanSmallFullColorPageOID;
	CString m_szToshibaScanSmallSingleColorPageOID;
	CString m_szToshibaScanSmallHBPageOID;
	CString m_szToshibaScanBigFullColorPageOID;
	CString m_szToshibaScanBigSingleColorPageOID;
	CString m_szToshibaScanBigHBPageOID;
	CString m_szToshibaOtherOpFullColorA4PageOID;
	CString m_szToshibaOtherOpSinglelColorA4PageOID;
	CString m_szToshibaOtherOpHBA4PageOID;
	CString m_szToshibaOtherOpFullColorA3PageOID;
	CString m_szToshibaOtherOpSingleColorA3PageOID;
	CString m_szToshibaOtherOpHBA3PageOID;
};
