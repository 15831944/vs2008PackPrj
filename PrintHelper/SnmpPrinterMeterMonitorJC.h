//京瓷打印机抄表

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorJC : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorJC(void);
	~CSnmpPrinterMeterMonitorJC(void);

protected:
	BOOL GetMeterInfo();
	BOOL GetMeterInfo_ECOSYS_P5021cdn();
	BOOL GetMeterInfo_ECOSYS_P2135dn();
	BOOL GetMeterInfo_FS_1060DN();
	BOOL GetMeterInfo_TASKalfa6002i();
	
	CString GetModel();

protected:
	void InitOID();
	CString m_szKyoceraA4PrintTotalPageOID;
	CString m_szKyoceraA4PrintHBPageOID;
	CString m_szKyoceraA4PrintSingleColorPageOID;
	CString m_szKyoceraA4PrintFullColorPageOID;
	CString m_szKyoceraA4CopyTotalPageOID;
	CString m_szKyoceraA4CopyHBPageOID;
	CString m_szKyoceraA4CopySingleColorPageOID;
	CString m_szKyoceraA4CopyFullColorPageOID;
	CString m_szKyoceraScanTotalPageOID;
	CString m_szKyoceraScanTotalPage2OID;
	CString m_szKyoceraTotalPageInfo6OID;
	CString m_szKyoceraTotalPageInfo7OID;
	CString m_szKyoceraTotalPageInfo8OID;
	CString m_szKyoceraTotalPageInfo9OID;

	CString m_szKyoceraModelOID;						
	CString m_szECOSYS_P5021cdn_A4PrintHBOID;			
	CString m_szECOSYS_P5021cdn_A4PrintColorOID;		
	CString m_szECOSYS_P5021cdn_A4PrintTotalOID;
	CString m_szFS_1060DN_A4PrintHBOID;

	CString m_szTASKalfa_6002iA4HBOID;
	CString m_szTASKalfa_6002iTotalOID;

};
