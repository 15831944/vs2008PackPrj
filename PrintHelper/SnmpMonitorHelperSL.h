//ʩ�ִ�ӡ�����Ӱ�����

#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperSL : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperSL(void);
	~CSnmpMonitorHelperSL(void);

protected:
	//ͨ��SNMP��ȡOID��Ϣ
	virtual BOOL GetMarkerSupplies();	//��д��ȡ�ĲĵĽӿ�

protected:
	void GetMarkerSuppliesByHtml();
	void Parse(string& strData);
	void ParseEx(string& strData, int nMarkerSuppliesType);
	void AddOneMarkerSupplies(string& strMarkerSuppliesName, int nMarkerSuppliesStatus, int nMarkerSuppliesLevel, int nMarkerSuppliesType);
	void ConvertColorDesc(string& strColor);
	int ConvertMarkerSuppliesType(int nType);
	int ConvertMarkerSuppliesLevel(int nMarkerSuppliesStatus, int nMarkerSuppliesLevel, int nMarkerSuppliesType);
};
