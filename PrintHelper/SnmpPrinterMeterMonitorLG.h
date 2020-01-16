//理光打印机抄表

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorLG : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorLG(void);
	~CSnmpPrinterMeterMonitorLG(void);

protected:
	BOOL GetMeterInfo();
	CString GetTonerPercent(int value);

protected:
	void InitOID();
	CString m_szRicohEngCounterEntryOID;
	CString m_szRicohTonerColorOID;
	CString m_szRicohTonerDescOID;
	CString m_szRicohTonerColorTypeOID;
	CString m_szRicohTonerLevelPercentOID;
};
