#pragma once
#include "snmpinterface.h"
class CSnmpPrinterMeterMonitorPRONNIX : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorPRONNIX(void);
	~CSnmpPrinterMeterMonitorPRONNIX(void);
protected:
	BOOL GetMeterInfo();
protected:
	void InitOID();
	CString m_szPRINTRONIX_TotalPageOID;
};
