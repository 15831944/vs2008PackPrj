//SNMP���ӿ�����ӡ����ҵ

#pragma once
#include "SnmpInterface.h"

class CSnmpJobMonitorKM : public CSnmpJobMonitor
{
public:
	CSnmpJobMonitorKM();
	~CSnmpJobMonitorKM(void);
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
	int CheckLastPrintCount2();
	void GetJobList();	//��ȡ��ʷ��ҵ�б�
	void GetPrintJobList();	//��ȡ�Ѿ���ӡ�б�

protected:
	void InitOID();
	CString m_szKmJmGeneralEntryOID;
	CString m_szKmJmJobEntryOID;
	CString m_szKmJobCountOID;
	CString m_szKmJobIndexOldOID;

	CString m_szKmTotalPageCountOID;
};
