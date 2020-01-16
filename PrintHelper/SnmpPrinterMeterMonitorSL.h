//施乐打印机抄表

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorSL : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorSL(void);
	~CSnmpPrinterMeterMonitorSL(void);

protected:
	BOOL GetMeterInfo();
	BOOL GetMeterInfo_ApeosPort_V_C3373();
	BOOL GetMeterInfo_DocuCentre_S2110();

	CString GetSLModel();

	CString m_szSLPrintModel;
	CString GetMeterByUrl(CString szUrl);
	CString GetDetailMeterInfoByFlag(CString szFlag,CString szWebContent);
protected:
	void InitOID();
	CString m_szXeroxA4PrintHBPageOID;
	CString m_szXeroxA4PrintColorPageOID;
	CString m_szXeroxA4CopyHBPageOID;
	CString m_szXeroxA4CopyColorPageOID;
	CString m_szXeroxScannedImagesStoredOID;
	CString m_szXeroxNetworkScanningImagesSentOID;
	CString m_szXeroxA4FaxTotalPageOID;
	CString m_szXeroxA4ColorTotalPageOID;
	CString m_szXeroxA4HBTotalPageOID;
	CString m_szXeroxA3ColorTotalPageOID;
	CString m_szXeroxA3HBTotalPageOID;

	CString m_szXeroxA3HBTotalPageOID_DocuCentre_S2110;

	CString m_szSLPrinterModelOID;
};
