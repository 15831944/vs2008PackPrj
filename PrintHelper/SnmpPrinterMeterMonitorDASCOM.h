//得实打印机（小票打印机）抄表

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorDASCOM : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorDASCOM(void);
	~CSnmpPrinterMeterMonitorDASCOM(void);

protected:
	BOOL GetMeterInfo();

protected:
	CCriticalSection2 m_cs4MeterInfo;
};

