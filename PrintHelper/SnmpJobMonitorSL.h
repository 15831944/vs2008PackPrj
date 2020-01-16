//SNMP����ʩ�ִ�ӡ����ҵ

#pragma once
#include "SnmpInterface.h"

class CSnmpJobMonitorSL : public CSnmpJobMonitor
{
public:
	CSnmpJobMonitorSL(void);
	~CSnmpJobMonitorSL(void);
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
	BOOL GetCurrentJobIndex(CJobList& oCurrentJobList);	//��ȡ��ǰ��ӡ������Ϣ
	int GetJobIndexOld(CString& szJobOid);	//��ȡ��ɵ�һ����ӡ��ҵIndex����ҵ��ʶOID
	bool GetJobInfo(PSCP_JobInfo pJobInfo); //��ȡ��ҵ��Ϣ
	void GetSLPrintJobList();	//ֻ���ʩ�ִ�ӡ��,��ȡ�Ѿ���ӡ�б�

protected:
	void InitOID();
	//ʩ����ҵ����OID
	CString m_szXeroxJobTypeScanToFileOID;
	CString m_szXeroxJobTypeScanToPrintOID;
	CString m_szXeroxJobTypeScanToFaxOID;
	CString m_szXeroxJobTypeScanToMailListOID;
	CString m_szXeroxJobTypeFaxToFileOID;
	CString m_szXeroxJobTypeFaxToPrintOID;
	CString m_szXeroxJobTypeFaxToMailListOID;
	CString m_szXeroxJobTypePrintOID;
	CString m_szXeroxJobTypeFileToFaxOID;
	CString m_szXeroxJobTypeFileToMailListOID;
	CString m_szXeroxJobTypeCopyOID;
	CString m_szXeroxJobTypeFileToFileOID;

	//ʩ�ֵ�ǰ��ҵ��Ϣ
	CString m_szXeroxCurentJobListBeginOID;
	CString m_szXeroxCurentJobTypeOID;
	CString m_szXeroxCurentJobDocNameIndexOID;
	CString m_szXeroxCurentJobOwnerIndexOID;
	CString m_szXeroxCurentJobSenderIndexOID;
	CString m_szXeroxCurentJobDocNameOID;

	//ʩ����ʷ��ҵ��Ϣ
	CString m_szXeroxHistoryJobListBeginOID;
	CString m_szXeroxHistoryDocNameFormat1OID;
	CString m_szXeroxHistoryDocNameFormat151OID;
	CString m_szXeroxHistoryDocNameFormatOID;

	//ʩ���滻�ַ���
	CString m_szXeroxPrivateEnterprisesOID;
	CString m_szXeroxPrivateEnterprises2OID;
	CString m_szXeroxPrintStatusOID;
	CString m_szXeroxPrintStatus2OID;
	CString m_szXeroxPrintedPagesOID;
	CString m_szXeroxPrintedPages2OID;
	CString m_szXeroxPrintTimeOID;
	CString m_szXeroxPrintTime2OID;
};
