//奔图打印机监视帮助类

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperHP : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperHP(void);
	~CSnmpMonitorHelperHP(void);

protected:
	//通过SNMP获取OID信息
	//virtual BOOL GetMarkerSupplies();	//重写获取耗材的接口
// 
// 	virtual BOOL GetInput();
// 	virtual void CheckInput();
	
protected:
	CString GetHPModel();
	void InitOID();
	CString m_szHPTonerLevelPercentOID_4345;
	CString m_szHpPrinterModelOID;

	BOOL GetMarkerSupplies_M4345();
	BOOL GetMarkerSupplies_M1216();
	BOOL GetMarkerSupplies_M4650();
	BOOL GetMarkerSupplies_CP3505();


	BOOL GetMarkerSupplies_M525();
	CString GetBlackInfo_M525(CString& szContent);

	CString GetBlackInfo_CP3505(CString& szContent);
	CString GetCyanInfo_CP3505(CString& szContent);
	CString GetMagentaInfo_CP3505(CString& szContent);
	CString GetYellowInfo_CP3505(CString& szContent);

	int DoubleToInt(double fValue);
	CString GetMeterByUrl(CString szUrl);
	CString GetHttpsMeterByUrl(CString szUrl);

	BOOL GetMarkerSupplies_E77822();
	void GetTonerInfo_E77822(CString& szContent,CString &szBlackPercent,CString &szCyanPercent,CString &szMagentPercent,CString &szYellowPercent);


	BOOL GetMarkerSupplies_E72530();
	void GetTonerInfo_E72530(CString& szContent,CString &szBlackPercent);

};
