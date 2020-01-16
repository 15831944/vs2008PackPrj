//理光打印机监视帮助类

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperLG : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperLG(void);
	~CSnmpMonitorHelperLG(void);

protected:
	//通过SNMP获取OID信息
	virtual BOOL GetMarkerSupplies();	//重写获取耗材的接口
	virtual BOOL GetAlert();			//重写获取预警信息
	virtual BOOL IsFalutInfo(CString szDetial);	//重写判断信息是否为故障信息
	virtual CString GetFaultCode(CString szDetial);	//重写获取故障代码

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
