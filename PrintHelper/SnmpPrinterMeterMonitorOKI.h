#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorOKI : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorOKI(void);
	virtual ~CSnmpPrinterMeterMonitorOKI(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szOkiA4PrintTotalPageOID;
	CString m_szOkiA4PrintColorPageOID;
	CString m_szOkiA4PrintHBPageOID;
};
