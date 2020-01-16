#include "StdAfx.h"
#include "SnmpJobMonitorLG.h"
#include "RicohMIB_Def.h"

CSnmpJobMonitorLG::CSnmpJobMonitorLG()
{
	InitOID();
}

CSnmpJobMonitorLG::~CSnmpJobMonitorLG(void)
{
}

void CSnmpJobMonitorLG::InitOID()
{
	m_szRicohJobCurrentCountOID = DecryptOID(RicohJobCurrentCountOID);
	m_szRicohJobMaxCountOID = DecryptOID(RicohJobMaxCountOID);

	m_szRicohJobInfo2OID = DecryptOID(RicohJobInfo2OID);
	m_szRicohJobInfo3OID = DecryptOID(RicohJobInfo3OID);
	m_szRicohJobInfoDocNameOID = DecryptOID(RicohJobInfoDocNameOID);
	m_szRicohJobInfo5OID = DecryptOID(RicohJobInfo5OID);
	m_szRicohJobInfoPageOID = DecryptOID(RicohJobInfoPageOID);
	m_szRicohJobInfoIDOID = DecryptOID(RicohJobInfoIDOID);
	m_szRicohJobInfoCreateTimeOID = DecryptOID(RicohJobInfoCreateTimeOID);
	m_szRicohJobInfoUserNameOID = DecryptOID(RicohJobInfoUserNameOID);
	m_szRicohJobInfoStateStrOID = DecryptOID(RicohJobInfoStateStrOID);
	m_szRicohJobInfoStateOID = DecryptOID(RicohJobInfoStateOID);
	m_szRicohJobInfo12OID = DecryptOID(RicohJobInfo12OID);
	m_szRicohJobInfo13OID = DecryptOID(RicohJobInfo13OID);
}

void CSnmpJobMonitorLG::CheckJob()
{
	if (!CheckIPActive())
	{
		return;
	}

	char cOidStr[50] = {0};
	unsigned char cValue[256] = {0};

	int nCurrentJobCount = 0;
	if(GetRequest(CStringToChar(m_szRicohJobCurrentCountOID), nCurrentJobCount))
	{
		if (nCurrentJobCount > 0)
		{
			//theLog.Write("CSnmpJobMonitorLG::CheckJob,1,nCurrentJobCount = %d", nCurrentJobCount);
			for (int nJobIndex=1; nJobIndex<=nCurrentJobCount; nJobIndex++)
			{
				do 
				{
					//打印作业ID
					sprintf(cOidStr, "%s.%d", m_szRicohJobInfoIDOID, nJobIndex);
					if (!GetRequest(cOidStr, cValue, sizeof(cValue)))
					{
						theLog.Write("!!CSnmpJobMonitorLG::CheckJob,2,GetRequest fail");
						break;
					}

					int nJobId = GetJobPID((char*)cValue);
					if (nJobId <= 0)
					{
						theLog.Write("!!CSnmpJobMonitorLG::CheckJob,3,nJobId=%d, cValue = %s", nJobId, cValue);
						break;
					}

					//打印作业状态：0-无作业，1-打印完成，6-处理中，7-错误，8-已取消
					int nJobInfoState = 0;
					sprintf(cOidStr, "%s.%d", m_szRicohJobInfoStateOID, nJobIndex);
					if (!GetRequest(cOidStr, nJobInfoState))
					{
						theLog.Write("!!CSnmpJobMonitorLG::CheckJob,4,GetRequest fail");
						break;
					}

					if (nJobInfoState <= 0)
					{
						theLog.Write("!!CSnmpJobMonitorLG::CheckJob,5,nJobInfoState=%d", nJobInfoState);
						break;
					}

					//打印作业打印页数
					sprintf(cOidStr, "%s.%d", m_szRicohJobInfoPageOID, nJobIndex);
					if (!GetRequest(cOidStr, cValue, sizeof(cValue)))
					{
						theLog.Write("!!CSnmpJobMonitorLG::CheckJob,6,GetRequest fail");
						break;
					}

					int nJobInfoPage = GetJobPage((char*)cValue);

					PSCP_JobInfo pJobInfo = NULL;
					if (!m_oJobMap.Lookup(nJobId, pJobInfo))
					{
						//添加作业
						pJobInfo = new SCP_JobInfo;
						memset(pJobInfo, 0x0, sizeof(SCP_JobInfo));
						pJobInfo->nJobId = nJobId;
						pJobInfo->nPrintedPage = nJobInfoPage;
						pJobInfo->JobType = SCP_JOB_TYPE_PRINT; 
						pJobInfo->JobStatus = SCP_JOB_STATUS_START;
						m_oJobMap.SetAt(nJobId, pJobInfo);
					}	
					else
					{
						if (pJobInfo)
						{	
							//更新作业状态
							pJobInfo->nPrintedPage = nJobInfoPage;
							if (pJobInfo->JobStatus == SCP_JOB_STATUS_ABORTED)
							{

							}
							else
							{
								if (nJobInfoState == 1 || nJobInfoState == 8)
								{
									pJobInfo->JobStatus = SCP_JOB_STATUS_COMPLETED;
								}
								else
								{
									pJobInfo->JobStatus = SCP_JOB_STATUS_PRINTING;
								}
							}
						}
						else
						{
							//删除异常作业
							m_oJobMap.RemoveKey(nJobId);
							theLog.Write("!!CSnmpJobMonitorLG::CheckJob,7,RemoveKey 异常作业,nJobId = %d", nJobId);
						}
					}
				} while (FALSE);
			}
		}
	}
	else
	{
		//theLog.Write("!!CSnmpJobMonitorLG::CheckJob,7,fail,SnmpJobMonitor_JobCurrentCount_LG=%s", SnmpJobMonitor_JobCurrentCount_LG);
		return;
	}

	POSITION pos = m_oJobMap.GetStartPosition();
	while(pos)
	{
		int nJobId = 0;
		PSCP_JobInfo pJobInfo = NULL;
		m_oJobMap.GetNextAssoc(pos, nJobId, pJobInfo);
		if (pJobInfo)
		{
			if (pJobInfo->JobStatus != SCP_JOB_STATUS_ABORTED)
			{
				if (m_pCallBack)
				{
					m_pCallBack->OnJobInfo(pJobInfo);
				}
			}

			if (pJobInfo->JobStatus == SCP_JOB_STATUS_COMPLETED)
			{
				pJobInfo->JobStatus = SCP_JOB_STATUS_ABORTED;
				//m_oJobMap.RemoveKey(nJobId);
				//delete pJobInfo;
			}
		}
	}
}

int CSnmpJobMonitorLG::CheckLastPrintCount()
{
	//CheckLastPrintCount_Color和CheckLastPrintCount_WB一定要先于CheckLastPrintCount调用
	CheckLastPrintCount_Color();
	CheckLastPrintCount_WB();
	m_nLastPrintCount = m_nLastPrintCount_Color + m_nLastPrintCount_WB;
	return m_nLastPrintCount;
}

int CSnmpJobMonitorLG::CheckLastPrintCount_Color()
{
	m_nLastPrintCount_Color = 0;
	return m_nLastPrintCount_Color;
}

int CSnmpJobMonitorLG::CheckLastPrintCount_WB()
{
	m_nLastPrintCount_WB = 0;
	return m_nLastPrintCount_WB;
}

int CSnmpJobMonitorLG::CheckLastCopyCount()
{
	//CheckLastCopyCount_Color和CheckLastCopyCount_WB一定要先于CheckLastCopyCount调用
	CheckLastCopyCount_Color();
	CheckLastCopyCount_WB();
	m_nLastCopyCount = m_nLastCopyCount_Color + m_nLastCopyCount_WB;
	return m_nLastCopyCount;
}

int CSnmpJobMonitorLG::CheckLastCopyCount_Color()
{
	m_nLastCopyCount_Color = 0;
	return m_nLastCopyCount_Color;
}

int CSnmpJobMonitorLG::CheckLastCopyCount_WB()
{
	m_nLastCopyCount_WB = 0;
	return m_nLastCopyCount_WB;
}

int CSnmpJobMonitorLG::CheckLastScanCount()
{
	//CheckLastScanCount_WB和CheckLastScanCount_Color一定要先于CheckLastScanCount调用
	CheckLastScanCount_WB();
	CheckLastScanCount_Color();
	m_nLastScanCount = m_nLastScanCount_Color + m_nLastScanCount_WB;
	return m_nLastScanCount;
}

int CSnmpJobMonitorLG::CheckLastScanCount_Color()
{
	m_nLastScanCount_Color = 0;
	return m_nLastScanCount_Color;
}

int CSnmpJobMonitorLG::CheckLastScanCount_WB()
{
	m_nLastScanCount_WB = 0;
	return m_nLastScanCount_WB;
}

int CSnmpJobMonitorLG::CheckLastFaxCount()
{
	CheckLastFaxCount_Color();
	CheckLastFaxCount_WB();
	m_nLastFaxCount = m_nLastFaxCount_Color + m_nLastFaxCount_WB;
	return m_nLastFaxCount;
}

int CSnmpJobMonitorLG::CheckLastFaxCount_Color()
{
	m_nLastFaxCount_Color = 0;
	return m_nLastFaxCount_Color;
}

int CSnmpJobMonitorLG::CheckLastFaxCount_WB()
{
	m_nLastFaxCount_WB = 0;
	return m_nLastFaxCount_WB;
}

int CSnmpJobMonitorLG::CheckLastPageCount_A3()
{
	m_nA3PageCount = 0;
	return m_nA3PageCount;
}

int CSnmpJobMonitorLG::CheckLastPageCount_A4()
{
	m_nA4PageCount = 0;
	return m_nA4PageCount;
}

//Value=pid=1,submit=(NONE),recv=1		//pid=打印作业ID
int CSnmpJobMonitorLG::GetJobPID(CString szValue)
{
	int nPID = 0;
	szValue.MakeLower();
	CStringArray ary;
	CCommonFun::StringSplit(szValue, &ary, ",", TRUE);
	if (ary.GetCount() == 3)
	{
		szValue = ary.GetAt(0);
		szValue.Trim("pid=");
		nPID = atoi(szValue);
	}
	return nPID;	
}

//Value=proc=1,prt=1	//proc=处理的页数，prt=打印的页数
int CSnmpJobMonitorLG::GetJobPage(CString szValue)
{
	int nPage = 0;
	szValue.MakeLower();
	CStringArray ary;
	CCommonFun::StringSplit(szValue, &ary, ",", TRUE);
	if (ary.GetCount() == 2)
	{		
		szValue = ary.GetAt(1);
		szValue.Trim("prt=");
		nPage = atoi(szValue);
	}
	return nPage;	
}
