//��ʵ��ӡ����СƱ��ӡ��������

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

