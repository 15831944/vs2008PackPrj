//��ͼ��ӡ�����Ӱ�����

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperPT : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperPT(void);
	~CSnmpMonitorHelperPT(void);

protected:
	//ͨ��SNMP��ȡOID��Ϣ
	virtual BOOL GetMarkerSupplies();
	virtual BOOL GetAlert();
    //virtual BOOL GetCover(); // del by zmm, 2019.1.10 �����º�����ʵ����������һ��, ��������Ҳ�����������״̬

    virtual BOOL CheckPrinterTable(); // add by zmm, 2019.1.10
	//virtual void CheckMarkerSupplies(); // del by zmm, 2019.1.10

    BOOL CheckDynamicInfo();
	// void PrinterStatus(); // del by zmm, 2019.1.10, ���� CheckPrinterTable ��ʵ��
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
