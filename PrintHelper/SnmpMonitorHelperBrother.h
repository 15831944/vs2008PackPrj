//兄弟打印机监视帮助类

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperBrother : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperBrother(void);
	~CSnmpMonitorHelperBrother(void);

protected:
	//通过SNMP获取OID信息
	virtual BOOL GetMarkerSupplies();	//重写获取耗材的接口

	CString GetBrotherModel();
protected:
	void InitOID();
	CString m_szPantumTonerLevelPercentOID;
	CString m_szBrotherModelOID;
	CString m_szModel;
	CString m_szBrotherMarkerSuppliesMaxCapacityOID_LenovoLJ3650DN;
	CString m_szBrotherMarkerSuppliesLevelOID_LenovoLJ3650DN;

};
