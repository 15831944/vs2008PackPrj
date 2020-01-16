//佳能打印机监视帮助类

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperCANON : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperCANON(void);
	~CSnmpMonitorHelperCANON(void);

protected:
	virtual void CheckFaultCode();	//单独的故障代码检测，从打印机的私有节点获取

protected:
	void InitOID();
	CString m_szCanonFaultInfoEntryOID;
	CString m_szCanonFaultInfoDescOID;
};
