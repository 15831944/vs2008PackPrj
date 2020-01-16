//��ʵ��СƱ����ӡ��
#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelperDASCOM : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelperDASCOM(void);
	virtual ~CSnmpMonitorHelperDASCOM(void);

protected:
	//���ظ���CSnmpMonitorHelper�Ľӿڣ���ֹ������ʹ��snmpȥ��ȡ��ʵ��ӡ����Ϣ��
	//��Ϊ��ʵ��ӡ����֧��snmp��ֻ��ͨ���򵥵�tcp����������ʵ�ִ�ӡ����Ϣ�Ļ�ȡ��
	//ͨ��SNMP��ȡOID��Ϣ
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
	virtual BOOL GetAlert();	//��д��ȡԤ����Ϣ

	virtual void CheckAlertEx() { } //��չ����(�Ǳ�׼PrintMib)�����˹��ϴ��������Ԥ����Ϣ

protected:
	//ģ��SNMP��GetAlert���������һ��ģ��Ԥ����Ϣ
	void AddAlert(int nIndex, int nAlertGroup, int nAlertCode, CString prtAlertDescription);
};
