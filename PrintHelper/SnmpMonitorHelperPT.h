//奔图打印机监视帮助类

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperPT : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperPT(void);
	~CSnmpMonitorHelperPT(void);

protected:
	//通过SNMP获取OID信息
	virtual BOOL GetMarkerSupplies();
	virtual BOOL GetAlert();
    //virtual BOOL GetCover(); // del by zmm, 2019.1.10 该重新函数的实现与基类基本一致, 无需重新也可以正常获得状态

    virtual BOOL CheckPrinterTable(); // add by zmm, 2019.1.10
	//virtual void CheckMarkerSupplies(); // del by zmm, 2019.1.10

    BOOL CheckDynamicInfo();
	// void PrinterStatus(); // del by zmm, 2019.1.10, 已在 CheckPrinterTable 中实现
	void TonerStatus();
	void DrumStatus();
	void CoverStatus();
	void TrayPaperError();
	//void TrayPaperError_P3500DN(); del by zmm, 2019.1.10
	void FatalError();

private:
    void appendPrtAlert(CString sAlertDesc, ENUM_STRUCT_TYPE(PrtAlertGroupTC) eAlertType, ENUM_STRUCT_TYPE(PrtAlertCodeTC) eAlertCode);
    void getOpcInfo();
    void getToneInfo();
protected:
	void InitOID();
	CString m_szPantumTonerLevelPercentOID;
	CString m_szPantumCoverStatusOID;
	CString m_szPantumPrinterStatus;
	CString m_szPantumTrayPaperErrorOID;
	CString m_szPantumAlertBaseOID;
	CString m_szPantumTonerStatus;
	CString m_szPantumDrumStatusOID;
	CString m_szPantumFatalErrorOID;
	CString m_szPantumModelOID;
};
