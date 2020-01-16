//精图工程打印机
#pragma once
#include "snmpinterface.h"

class CSnmpMonitorHelper_JT : public CSnmpMonitorHelper
{
public:
	CSnmpMonitorHelper_JT(void);
	virtual ~CSnmpMonitorHelper_JT(void);

	virtual BOOL GetMarkerSupplies();

	virtual BOOL GetInput();
	virtual void CheckInput();

	virtual BOOL GetAlert();

	virtual BOOL GetCover();

	CString GetPageSize(int code);
	int GetPaperLevel(int code);	//模拟百分比量

protected:
	void InitOID();
};
