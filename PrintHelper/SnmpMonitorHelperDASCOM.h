//得实（小票）打印机
#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperDASCOM : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperDASCOM(void);
	virtual ~CSnmpMonitorHelperDASCOM(void);

protected:
	//重载父类CSnmpMonitorHelper的接口，防止父类中使用snmp去获取得实打印机信息，
	//因为得实打印机不支持snmp，只能通过简单的tcp数据请求来实现打印机信息的获取。
	//通过SNMP获取OID信息
	//virtual BOOL GetAll();
	virtual BOOL GetGeneral() { return TRUE; }
	virtual BOOL GetStorageRef() { return TRUE; }
	virtual BOOL GetDeviceRef() { return TRUE; }
	virtual BOOL GetCover() { return TRUE; }
	virtual BOOL GetLocalization() { return TRUE; }
	virtual BOOL GetInput() { return TRUE; }
	virtual BOOL GetOutput() { return TRUE; }
	virtual BOOL GetMarker() { return TRUE; }
	virtual BOOL GetMarkerSupplies() { return TRUE; }
	virtual BOOL GetMarkerColorant() { return TRUE; }
	virtual BOOL GetMediaPath() { return TRUE; }
	virtual BOOL GetChannel() { return TRUE; }
	virtual BOOL GetInterpreter() { return TRUE; }
	virtual BOOL GetConsoleDisplayBuffer() { return TRUE; }
	virtual BOOL GetConsoleLight() { return TRUE; }
	virtual BOOL GetAlert();	//重写获取预警信息

	virtual void CheckAlertEx() { } //扩展功能(非标准PrintMib)，过滤故障代码和其它预警信息

protected:
	//模拟SNMP的GetAlert操作，添加一个模拟预警信息
	void AddAlert(int nIndex, int nAlertGroup, int nAlertCode, CString prtAlertDescription);
};
