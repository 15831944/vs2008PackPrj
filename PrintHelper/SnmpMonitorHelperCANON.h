//���ܴ�ӡ�����Ӱ�����

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperCANON : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperCANON(void);
	~CSnmpMonitorHelperCANON(void);

protected:
	virtual void CheckFaultCode();	//�����Ĺ��ϴ����⣬�Ӵ�ӡ����˽�нڵ��ȡ

protected:
	void InitOID();
	CString m_szCanonFaultInfoEntryOID;
	CString m_szCanonFaultInfoDescOID;
};
