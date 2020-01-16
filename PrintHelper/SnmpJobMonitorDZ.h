//SNMP监视东芝打印机作业

#pragma once
#include "SnmpInterface.h"

class CSnmpJobMonitorDZ : public CSnmpJobMonitor
{
public:
	CSnmpJobMonitorDZ();
	~CSnmpJobMonitorDZ(void);
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
	void GetPrintJobList();	//获取已经打印列表

	//清理校验成功的打印机上的打印记录，防止打印机内部的打印记录随着使用量的增加从而导致查询日志的效率降低
	void ClearPrintJobList(SCP_JobInfo_List& PrintedJobList);

	//清理复印记录日志
	void ClearCopyLog(CXmlParse& xml);

	//作业类型
	enum ToshibaLogType
	{
		ToshibaLogType_Device,
		ToshibaLogType_Operation,
		ToshibaLogType_Access,
		ToshibaLogType_Message,
		ToshibaLogType_Job,
		ToshibaLogType_Scan,
		ToshibaLogType_Copy,
		ToshibaLogType_Fax,
		ToshibaLogType_FaxReception,
		ToshibaLogType_FaxSent,
		ToshibaLogType_Print,
		ToshibaLogType_Connector,
		ToshibaLogType_SaveAsFileConnector,
		ToshibaLogType_EFilingConnector,
		ToshibaLogType_MailConnector,
		ToshibaLogType_OtherConnector,
		ToshibaLogType_Application,
		ToshibaLogType_Maintenance,
		ToshibaLogType_Erase
	};

	//清理指定类型和ID的日志记录
	void ClearJobLog(ToshibaLogType nLogType, int nLogId);


protected:
	void InitOID();
	CString m_szToshibaTotalPageCountOID;
	SCP_JobInfo_List m_PrintedJobList;
};
