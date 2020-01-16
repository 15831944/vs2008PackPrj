#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorTSC : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorTSC(void);
	~CSnmpPrinterMeterMonitorTSC(void);
protected:

	BOOL GetMeterInfo();
	void InitOID();
	CString GetModel();
	BOOL GetMeterInfo_T_4502E();
	CString GetMeterByUrl(CString szUrl);
	CString GetPrintLen(CString szHtml);

protected:
	CCriticalSection2 m_cs4MeterInfo;
	CString m_szModelOID;
};
