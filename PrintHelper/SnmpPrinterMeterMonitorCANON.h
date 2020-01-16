#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorCANON : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorCANON(void);
	virtual ~CSnmpPrinterMeterMonitorCANON(void);

protected:
	BOOL GetMeterInfo();
	BOOL GetMeterInfo_LBP6300n();
	BOOL GetMeterInfo_iR1024();

protected:
	void InitOID();
	CString m_szCanonAllTotalPageOID;
	CString m_szCanonA3HBTotalPageOID;
	CString m_szCanonA4HBTotalPageOID;
	CString m_szCanonA3ColorTotalPageOID;
	CString m_szCanonA4ColorTotalPageOID;
	CString m_szCanonScanTotalPageOID;

	//通过Html分析抄表信息
	void GetMeterInfoByHtml();
	void write_value(int nType, int nValue);
	CString	GetCANONModel();
	CString m_szCANONPrinterModelOID;
	CString m_szCanonAllTotalPageOID_LBP6300dn;
	CString m_szCanonAllTotalPageOID_iR1024;
};
