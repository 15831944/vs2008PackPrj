//���Ǵ�ӡ������

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorSX : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorSX(void);
	~CSnmpPrinterMeterMonitorSX(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szScmHrDevCountEntryOID;
};
