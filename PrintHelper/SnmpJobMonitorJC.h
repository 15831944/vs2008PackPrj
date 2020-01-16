//SNMP监视京瓷打印机作业

#pragma once
#include "SnmpInterface.h"

class CSnmpJobMonitorJC : public CSnmpJobMonitor
{
public:
	CSnmpJobMonitorJC();
	~CSnmpJobMonitorJC(void);
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
	int GetPrintedCountByJobId(int nJobId);

protected:
	void InitOID();

	//京瓷当前作业信息节点
	CString m_szKyoceraActiveJobInfoJobCountOID;

	CString m_szKyoceraActiveJobInfoJobIDOID;
	CString m_szKyoceraActiveJobInfo3OID;
	CString m_szKyoceraActiveJobInfoJobTypeOID;
	CString m_szKyoceraActiveJobInfoJobNameOID;
	CString m_szKyoceraActiveJobInfoJobStateOID;
	CString m_szKyoceraActiveJobInfoJobBeginTimeOID;
	CString m_szKyoceraActiveJobInfo8OID;
	CString m_szKyoceraActiveJobInfoPrintedCountOID;
	CString m_szKyoceraActiveJobInfoPrintedCopyOID;
	CString m_szKyoceraActiveJobInfoTotalCopyOID;
	CString m_szKyoceraActiveJobInfoUserNameOID;
	CString m_szKyoceraActiveJobInfo13OID;
	CString m_szKyoceraActiveJobInfoFromOID;
	CString m_szKyoceraActiveJobInfoOrgDocPageCountOID;
	CString m_szKyoceraActiveJobInfo16OID;
	CString m_szKyoceraActiveJobInfoColorTypeOID;

	//京瓷历史作业信息节点
	CString m_szKyoceraHistoryJobInfoJobCountOID;

	CString m_szKyoceraHistoryJobInfoJobIDOID;
	CString m_szKyoceraHistoryJobInfoJobTypeOID;
	CString m_szKyoceraHistoryJobInfoJobNameOID;
	CString m_szKyoceraHistoryJobInfoJobStateOID;
	CString m_szKyoceraHistoryJobInfoBeginTimeOID;
	CString m_szKyoceraHistoryJobInfoEndTimeOID;
	CString m_szKyoceraHistoryJobInfo8OID;
	CString m_szKyoceraHistoryJobInfoPrintedCountOID;
	CString m_szKyoceraHistoryJobInfoPrintedCopyOID;
	CString m_szKyoceraHistoryJobInfoTotalCopyOID;
	CString m_szKyoceraHistoryJobInfoUserNameOID;
	CString m_szKyoceraHistoryJobInfo13OID;
	CString m_szKyoceraHistoryJobInfo14OID;
	CString m_szKyoceraHistoryJobInfoOrgDocPageCountOID;
	CString m_szKyoceraHistoryJobInfo16OID;
	CString m_szKyoceraHistoryJobInfoColorTypeOID;
};
