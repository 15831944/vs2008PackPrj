#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorEPSON : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorEPSON(void);
	virtual ~CSnmpPrinterMeterMonitorEPSON(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szEpsonA4PrintHBPageOID;
	CString m_szEpsonA4PrintTotalPageOID;
	CString m_szEpsonA4PrintColorPageOID;
	CString m_szEpsonA4CopyTotalPageOID;
	CString m_szEpsonA4CopyColorPageOID;
	CString m_szEpsonOtherOpTotalPageOID;
	CString m_szEpsonOtherOpColorPageOID;
};
