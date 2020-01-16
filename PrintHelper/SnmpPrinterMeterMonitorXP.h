//ÏÄÆÕ´òÓ¡»ú³­±í

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorXP : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorXP(void);
	~CSnmpPrinterMeterMonitorXP(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szSharpA4PrintTotalPageOID;
	CString m_szSharpA4PrintColorPageOID;
	CString m_szSharpA4PrintHBPageOID;
	CString m_szSharpA4CopyHBPageOID;
	CString m_szSharpA4CopyHBPage2OID;
	CString m_szSharpA4CopyHBPage3OID;
	CString m_szSharpA4CopyFullColorPageOID;
	CString m_szSharpA4CopyDoubleColorPageOID;
	CString m_szSharpA4CopySingleColorPageOID;
	CString m_szSharpOtherOpHBPageOID;
	CString m_szSharpOtherOpColorPageOID;
	CString m_szSharpScanTotalPageOID;
	CString m_szSharpScanTotalPage2OID;
	CString m_szSharpA4FaxHBPageOID;
};
