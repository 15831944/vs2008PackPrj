//精工工程打印机
#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorSEIKO : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorSEIKO(void);
	virtual ~CSnmpPrinterMeterMonitorSEIKO(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szSeikoAllTotalDistanceOID;
	CString m_szSeikoAllTotalPageOID;
	CString m_szSeikoPrintDistanceOID;
	CString m_szSeikoPrintAreaOID;
	CString m_szSeikoCopyDistanceOID;
	CString m_szSeikoCopyAreaOID;
	CString m_szSeikoPaper1LevelPercentOID;
	CString m_szSeikoPaper2LevelPercentOID;
	CString m_szSeikoPaper3LevelPercentOID;
	CString m_szSeikoPaper4LevelPercentOID;
	CString m_szSeikoTonerBlackLevelPercentOID;
};
