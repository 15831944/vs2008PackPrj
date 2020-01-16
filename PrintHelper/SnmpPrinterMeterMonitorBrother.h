#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorBrother :	public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorBrother(void);
	virtual ~CSnmpPrinterMeterMonitorBrother(void);

protected:
	BOOL GetMeterInfo();
	BOOL GetMeterInfo_LenovoLJ3650DN();
	CString GetBrotherModel();

protected:
	void InitOID();
	CString m_szBrotherCounterInfo1OID;
	CString m_szBrotherCounterInfo2OID;
	CString m_szBrotherCounterInfo3OID;
	CString m_szBrotherCounterInfo4OID;
	CString m_szBrotherCounterInfo21OID;
	CString m_szBrotherCounterInfo22OID;
	CString m_szBrotherCounterInfo31ID;
	CString m_szBrotherCounterInfo41ID;
	CString m_szBrotherModelOID;
	CString m_szModel;
	CString m_szBrotherA4HBpPagesOID_LenovoLJ3650DN;
	CString m_szBrotherTotalHBPagesOID_LenovoLJ3650DN;
	
};
