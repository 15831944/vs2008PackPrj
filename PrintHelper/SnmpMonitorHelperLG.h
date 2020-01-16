//����ӡ�����Ӱ�����

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperLG : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperLG(void);
	~CSnmpMonitorHelperLG(void);

protected:
	//ͨ��SNMP��ȡOID��Ϣ
	virtual BOOL GetMarkerSupplies();	//��д��ȡ�ĲĵĽӿ�
	virtual BOOL GetAlert();			//��д��ȡԤ����Ϣ
	virtual BOOL IsFalutInfo(CString szDetial);	//��д�ж���Ϣ�Ƿ�Ϊ������Ϣ
	virtual CString GetFaultCode(CString szDetial);	//��д��ȡ���ϴ���

protected:
	void InitOID();
	CString m_szRicohEngPrtAlertEntryOID;
	CString m_szRicohEngFAXAlertEntryOID;
	CString m_szRicohEngCpyAlertEntryOID;
	CString m_szRicohEngScnAlertEntryOID;
	CString m_szRicohEngCounterEntryOID;
	CString m_szRicohTonerColorOID;
	CString m_szRicohTonerDescOID;
	CString m_szRicohTonerColorTypeOID;
	CString m_szRicohTonerLevelPercentOID;

	CString m_szRicohAlertEntryOID;
	CString m_szRicohAlertIndexOID;
	CString m_szRicohAlertSeverityLevelOID;
	CString m_szRicohAlertTrainingLevelOID;
	CString m_szRicohAlertGroupOID;
	CString m_szRicohAlertGroupIndexOID;
	CString m_szRicohAlertLocationOID;
	CString m_szRicohAlertCodeOID;
	CString m_szRicohAlertDescriptionOID;
	CString m_szRicohAlertTimeOID;

};
