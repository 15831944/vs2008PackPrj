//�ֵܴ�ӡ�����Ӱ�����

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperBrother : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperBrother(void);
	~CSnmpMonitorHelperBrother(void);

protected:
	//ͨ��SNMP��ȡOID��Ϣ
	virtual BOOL GetMarkerSupplies();	//��д��ȡ�ĲĵĽӿ�

	CString GetBrotherModel();
protected:
	void InitOID();
	CString m_szPantumTonerLevelPercentOID;
	CString m_szBrotherModelOID;
	CString m_szModel;
	CString m_szBrotherMarkerSuppliesMaxCapacityOID_LenovoLJ3650DN;
	CString m_szBrotherMarkerSuppliesLevelOID_LenovoLJ3650DN;

};
