//精工工程打印机
#pragma once
#include "SnmpMonitorHelper_JT.h"

class CSnmpMonitorHelper_SEIKO : public CSnmpMonitorHelper_JT
{
public:
	CSnmpMonitorHelper_SEIKO(void);
	virtual ~CSnmpMonitorHelper_SEIKO(void);

protected:
	void InitOID();
};
