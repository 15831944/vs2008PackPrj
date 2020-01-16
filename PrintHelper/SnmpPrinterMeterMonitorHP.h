//»ÝÆÕ´òÓ¡»ú³­±í

#pragma once
#include "snmpinterface.h"

class CSnmpPrinterMeterMonitorHP : public CSnmpPrinterMeterMonitor

{
public:
	CSnmpPrinterMeterMonitorHP(void);
	~CSnmpPrinterMeterMonitorHP(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szHpA4PrintHBPageOID;
	CString m_szHpA4PrintColorPageOID;
	CString m_szHpA4CopyTotalPageOID;
	CString m_szHpA4CopyHBPageOID;
	CString m_szHpA4CopyColorPageOID;
	CString m_szHpPrinterModelOID;
	CString m_szszHpPrintTotalPageOID_M4345;
	CString m_szHpA4PrintHBPageOID_4345;
	CString m_szHpPrintTotalPageOID_M1216;

	//Equivalent OID
	CString m_szHpEquivalentPrintHBOID;
	CString m_szHpEquivalentPrintColorOID;
	CString m_szHpEquivalentCopyHBOID;
	CString m_szHpEquivalentCopyColorOID;
	CString m_szHpEquivalentFaxHBOID;

	CString m_szHpA4TotalPrintOID_M401;
	CString m_szHpA4HBPrintOID_M401;
	CString m_szHpA4ColorPrintOID_M401;

	CString m_szHpA4TotalPrintOID_M401n;

	CString m_szHpScanHBOID;

	CString m_szHpEquivalentPrintHBOID_3015dn;
	
	

protected:
	
	CString GetHPModel();
	BOOL GetMeterInfo_M4345();
	BOOL GetMeterInfo_M1216();
	BOOL GetMeterInfo_M4650();
	BOOL GetMeterInfo_M725();
	BOOL GetMeterInfo_M525();
	BOOL GetMeterInfo_E77822();
	BOOL GetMeterInfo_E72530();
	BOOL GetMeterInfo_M401();
	BOOL GetMeterInfo_M401n();

	BOOL GetMeterInfo_3015dn();

	
	CString GetEquivalentPrint_M725(CString& szContent);
	CString GetEquivalentCopy_M725(CString& szContent);
	CString GetEquivalentFax_M725(CString& szContent);

	CString GetEquivalentPrint_M525(CString& szContent);
	CString GetEquivalentCopy_M525(CString& szContent);
	CString GetEquivalentFax_M525(CString& szContent);


	BOOL GetMeterInfo_CP3505();
	CString GetEquivalentTotalPrint_CP3505(CString& szContent);
	CString GetEquivalentA4TotalPrint_CP3505(CString& szContent);
	CString GetEquivalentA4HBTotalPrint_CP3505(CString& szContent);
	CString GetEquivalentA4ColorTotalPrint_CP3505(CString& szContent);
	CString GetEquivalentColorTotalPrint_CP3505(CString& szContent);
	CString GetEquivalentHBTotalPrint_CP3505(CString& szContent);

	BOOL GetMeterInfo_CP5520();
	CString GetEquivalentTotalPrint_CP5520(CString& szContent);
	CString GetEquivalentA4HBTotalPrint_CP5520(CString& szContent);
	CString GetEquivalentA4ColorTotalPrint_CP5520(CString& szContent);

	int DoubleToInt(double fValue);


	CString GetMeterByUrl(CString szUrl);
	CString GetHttpsMeterByUrl(CString szUrl);

	

	CString GetA3HBPrint_E77822(CString& szContent);
	CString GetA3ColorPrint_E77822(CString& szContent);

	CString GetA4HBPrint_E77822(CString& szContent);
	CString GetA4ColorPrint_E77822(CString& szContent);

	CString GetA4HBCopy_E77822(CString& szContent);
	CString GetA4ColorCopy_E77822(CString& szContent);

	CString GetA4HBFax_E77822(CString& szContent);
	CString GetA4ColorFax_E77822(CString& szContent);

	CString GetEquivalentTotalPrint_E77822(CString& szContent);
	CString GetEquivalentTotalCopy_E77822(CString& szContent);
	CString GetEquivalentTotalFx_E77822(CString& szContent);

	CString GetA4HBPrint_E72530(CString& szContent);
	CString GetA4HBCopy_E72530(CString& szContent);
	CString GetEquivalentTotalPrint_E72530(CString& szContent);
	CString GetEquivalentTotalCopy_E72530(CString& szContent);
	CString GetA4HBFax_E72530(CString& szContent);

	double ConvertIEEE754(const char* pBin);


	BOOL GetMeterInfo_CM3530_MFP();
	CString GetEquivalentTotalPrint_CM3530_MFP(CString& szContent);
	CString GetEquivalentA4TotalPrint_CM3530_MFP(CString& szContent);
	CString GetEquivalentA4HBTotalPrint_CM3530_MFP(CString& szContent);
	CString GetEquivalentA4ColorTotalPrint_CM3530_MFP(CString& szContent);
	CString GetEquivalentColorTotalPrint_CM3530_MFP(CString& szContent);
	CString GetEquivalentHBTotalPrint_CM3530_MFP(CString& szContent);

	





};
