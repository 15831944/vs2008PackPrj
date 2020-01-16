//施乐打印机监视帮助类

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperSL : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperSL(void);
	~CSnmpMonitorHelperSL(void);

protected:
	//通过SNMP获取OID信息
	virtual BOOL GetMarkerSupplies();	//重写获取耗材的接口

protected:
	void GetMarkerSuppliesByHtml();
	void Parse(string& strData);
	void ParseEx(string& strData, int nMarkerSuppliesType);
	void AddOneMarkerSupplies(string& strMarkerSuppliesName, int nMarkerSuppliesStatus, int nMarkerSuppliesLevel, int nMarkerSuppliesType);
	void ConvertColorDesc(string& strColor);
	int ConvertMarkerSuppliesType(int nType);
	int ConvertMarkerSuppliesLevel(int nMarkerSuppliesStatus, int nMarkerSuppliesLevel, int nMarkerSuppliesType);
};
