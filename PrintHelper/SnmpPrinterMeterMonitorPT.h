#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorPT : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorPT(void);
	virtual ~CSnmpPrinterMeterMonitorPT(void);

protected:
	BOOL GetMeterInfo();

    BOOL StaticInfo();
protected:
	void InitOID();
	CString m_szPantumA4PrintTotalPageOID;
	CString m_szPantumA4PrintHBPageOID;
	CString m_szPantumTotalPrintOID;
};
