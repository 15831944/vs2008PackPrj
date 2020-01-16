//SNMP监视理光打印机作业

#pragma once
#include "SnmpInterface.h"

class CSnmpJobMonitorLG : public CSnmpJobMonitor
{
public:
	CSnmpJobMonitorLG();
	~CSnmpJobMonitorLG(void);
	virtual void CheckJob();
	virtual int CheckLastPrintCount();
	virtual int CheckLastPrintCount_Color();
	virtual int CheckLastPrintCount_WB();
	virtual int CheckLastCopyCount();
	virtual int CheckLastCopyCount_Color();
	virtual int CheckLastCopyCount_WB();
	virtual int CheckLastScanCount();
	virtual int CheckLastScanCount_Color();
	virtual int CheckLastScanCount_WB();
	virtual int CheckLastFaxCount();
	virtual int CheckLastFaxCount_Color();
	virtual int CheckLastFaxCount_WB();
	virtual int CheckLastPageCount_A3();
	virtual int CheckLastPageCount_A4();

protected:
	int GetJobPID(CString szValue);
	int GetJobPage(CString szValue);

protected:
	void InitOID();
	CString m_szRicohJobCurrentCountOID;
	CString m_szRicohJobMaxCountOID;

	CString m_szRicohJobInfo2OID;
	CString m_szRicohJobInfo3OID;
	CString m_szRicohJobInfoDocNameOID;
	CString m_szRicohJobInfo5OID;
	CString m_szRicohJobInfoPageOID;
	CString m_szRicohJobInfoIDOID;
	CString m_szRicohJobInfoCreateTimeOID;
	CString m_szRicohJobInfoUserNameOID;
	CString m_szRicohJobInfoStateStrOID;
	CString m_szRicohJobInfoStateOID;
	CString m_szRicohJobInfo12OID;
	CString m_szRicohJobInfo13OID;
};
