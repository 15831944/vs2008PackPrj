#include "StdAfx.h"
#include "SnmpJobMonitorJC.h"
#include "KyoceraMIB_Def.h"

CSnmpJobMonitorJC::CSnmpJobMonitorJC()
{
	InitOID();
}

CSnmpJobMonitorJC::~CSnmpJobMonitorJC(void)
{
}

void CSnmpJobMonitorJC::InitOID()
{
	m_szKyoceraActiveJobInfoJobCountOID = DecryptOID(KyoceraActiveJobInfoJobCountOID);

	m_szKyoceraActiveJobInfoJobIDOID = DecryptOID(KyoceraActiveJobInfoJobIDOID);
	m_szKyoceraActiveJobInfo3OID = DecryptOID(KyoceraActiveJobInfo3OID);
	m_szKyoceraActiveJobInfoJobTypeOID = DecryptOID(KyoceraActiveJobInfoJobTypeOID);
	m_szKyoceraActiveJobInfoJobNameOID = DecryptOID(KyoceraActiveJobInfoJobNameOID);
	m_szKyoceraActiveJobInfoJobStateOID = DecryptOID(KyoceraActiveJobInfoJobStateOID);
	m_szKyoceraActiveJobInfoJobBeginTimeOID = DecryptOID(KyoceraActiveJobInfoJobBeginTimeOID);
	m_szKyoceraActiveJobInfo8OID = DecryptOID(KyoceraActiveJobInfo8OID);
	m_szKyoceraActiveJobInfoPrintedCountOID = DecryptOID(KyoceraActiveJobInfoPrintedCountOID);
	m_szKyoceraActiveJobInfoPrintedCopyOID = DecryptOID(KyoceraActiveJobInfoPrintedCopyOID);
	m_szKyoceraActiveJobInfoTotalCopyOID = DecryptOID(KyoceraActiveJobInfoTotalCopyOID);
	m_szKyoceraActiveJobInfoUserNameOID = DecryptOID(KyoceraActiveJobInfoUserNameOID);
	m_szKyoceraActiveJobInfo13OID = DecryptOID(KyoceraActiveJobInfo13OID);
	m_szKyoceraActiveJobInfoFromOID = DecryptOID(KyoceraActiveJobInfoFromOID);
	m_szKyoceraActiveJobInfoOrgDocPageCountOID = DecryptOID(KyoceraActiveJobInfoOrgDocPageCountOID);
	m_szKyoceraActiveJobInfo16OID = DecryptOID(KyoceraActiveJobInfo16OID);
	m_szKyoceraActiveJobInfoColorTypeOID = DecryptOID(KyoceraActiveJobInfoColorTypeOID);

	m_szKyoceraHistoryJobInfoJobCountOID = DecryptOID(KyoceraHistoryJobInfoJobCountOID);

	m_szKyoceraHistoryJobInfoJobIDOID = DecryptOID(KyoceraHistoryJobInfoJobIDOID);
	m_szKyoceraHistoryJobInfoJobTypeOID = DecryptOID(KyoceraHistoryJobInfoJobTypeOID);
	m_szKyoceraHistoryJobInfoJobNameOID = DecryptOID(KyoceraHistoryJobInfoJobNameOID);
	m_szKyoceraHistoryJobInfoJobStateOID = DecryptOID(KyoceraHistoryJobInfoJobStateOID);
	m_szKyoceraHistoryJobInfoBeginTimeOID = DecryptOID(KyoceraHistoryJobInfoBeginTimeOID);
	m_szKyoceraHistoryJobInfoEndTimeOID = DecryptOID(KyoceraHistoryJobInfoEndTimeOID);
	m_szKyoceraHistoryJobInfo8OID = DecryptOID(KyoceraHistoryJobInfo8OID);
	m_szKyoceraHistoryJobInfoPrintedCountOID = DecryptOID(KyoceraHistoryJobInfoPrintedCountOID);
	m_szKyoceraHistoryJobInfoPrintedCopyOID = DecryptOID(KyoceraHistoryJobInfoPrintedCopyOID);
	m_szKyoceraHistoryJobInfoTotalCopyOID = DecryptOID(KyoceraHistoryJobInfoTotalCopyOID);
	m_szKyoceraHistoryJobInfoUserNameOID = DecryptOID(KyoceraHistoryJobInfoUserNameOID);
	m_szKyoceraHistoryJobInfo13OID = DecryptOID(KyoceraHistoryJobInfo13OID);
	m_szKyoceraHistoryJobInfo14OID = DecryptOID(KyoceraHistoryJobInfo14OID);
	m_szKyoceraHistoryJobInfoOrgDocPageCountOID = DecryptOID(KyoceraHistoryJobInfoOrgDocPageCountOID);
	m_szKyoceraHistoryJobInfo16OID = DecryptOID(KyoceraHistoryJobInfo16OID);
	m_szKyoceraHistoryJobInfoColorTypeOID = DecryptOID(KyoceraHistoryJobInfoColorTypeOID);
}

/************************************************************************/
/* 作业监视说明
1.京瓷作业分为【当前激活作业】和【已经完成作业】两个分类。
2.检测打印作业时，先将所有上次获取到的【当前激活作业】全部设置状态为【完成】。
3.再将检测【当前激活作业】，在MAP中没有，则添加，否则则更新信息信息为【打印中】。
4.再次检测MAP，如果作业状态为【完成】，则在实际的【当前激活作业】中本作业没有找到，
则认为已经完成。
5.在【已经完成作业】中获取指定完成的作业ID的作业完成信息。
*/
/************************************************************************/
void CSnmpJobMonitorJC::CheckJob()
{
	if (!CheckIPActive())
	{
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
			pJobInfo->JobStatus = SCP_JOB_STATUS_COMPLETED;
		}
		else
		{
			theLog.Write("##CSnmpJobMonitorJC::CheckJob,1,RemoveKey 异常作业,nJobId = %d", nJobId);
			m_oJobMap.RemoveKey(nJobId);
		}
	}

	char cOidStr[128] = {0};
	char cValue[256] = {0};

	int nCurrentJobCount = 0;
	if(GetRequest(CStringToChar(m_szKyoceraActiveJobInfoJobCountOID), nCurrentJobCount))
	{
		if (nCurrentJobCount > 0)
		{
			//theLog.Write("CSnmpJobMonitorJC::CheckJob,1,nCurrentJobCount = %d", nCurrentJobCount);
			for (int nJobIndex=1; nJobIndex<=nCurrentJobCount; nJobIndex++)
			{
				do 
				{
					//打印作业ID
					int nJobId = 0;
					sprintf(cOidStr, "%s.%d", m_szKyoceraActiveJobInfoJobIDOID, nJobIndex);
					if (!GetRequest(cOidStr, nJobId))
					{
						//theLog.Write("!!CSnmpJobMonitorJC::CheckJob,2,GetRequest fail, cOidStr = %s", cOidStr);
						break;
					}

					
					//作业类型，1-打印，2-复印
					int nJobType = 0;
					sprintf(cOidStr, "%s.%d", m_szKyoceraActiveJobInfoJobTypeOID, nJobIndex);
					if (!GetRequest(cOidStr, nJobId))
					{
						//theLog.Write("!!CSnmpJobMonitorJC::CheckJob,3,GetRequest fail, cOidStr = %s", cOidStr);
						break;
					}

					if (nJobType != 1)
					{
						//theLog.Write("##CSnmpJobMonitorJC::CheckJob,4,nJobType = %d, Not Print Job(1)", nJobType);
						continue;
					}

					//作业状态，1-处理中，2-打印中，4-暂停，5-等待中
					int nJobInfoState = 0;
					sprintf(cOidStr, "%s.%d", m_szKyoceraActiveJobInfoJobStateOID, nJobIndex);
					if (!GetRequest(cOidStr, nJobInfoState))
					{
						//theLog.Write("!!CSnmpJobMonitorJC::CheckJob,5,GetRequest fail, cOidStr = %s", cOidStr);
						break;
					}

					//打印作业打印页数
					int nJobInfoPage = 0;
					sprintf(cOidStr, "%s.%d", m_szKyoceraActiveJobInfoPrintedCountOID, nJobIndex);
					if (!GetRequest(cOidStr, nJobInfoPage))
					{
						//theLog.Write("!!CSnmpJobMonitorJC::CheckJob,6,GetRequest fail, cOidStr = %s", cOidStr);
						break;
					}

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
							//更新作业信息
							pJobInfo->nPrintedPage = nJobInfoPage;
							pJobInfo->JobStatus = SCP_JOB_STATUS_PRINTING;
						}
						else
						{
							//删除异常作业
							m_oJobMap.RemoveKey(nJobId);
							theLog.Write("!!CSnmpJobMonitorJC::CheckJob,7,RemoveKey 异常作业,nJobId = %d", nJobId);
						}
					}
				} while (FALSE);
			}
		}
	}
	else
	{
		//theLog.Write("!!CSnmpJobMonitorJC::CheckJob,7,fail,KyoceraActiveJobInfoJobCountOID=%s", m_szKyoceraActiveJobInfoJobCountOID);
		return;
	}

	pos = m_oJobMap.GetStartPosition();
	while(pos)
	{
		int nJobId = 0;
		PSCP_JobInfo pJobInfo = NULL;
		m_oJobMap.GetNextAssoc(pos, nJobId, pJobInfo);
		if (pJobInfo)
		{
			if (m_pCallBack)
			{
				if (pJobInfo->JobStatus == SCP_JOB_STATUS_COMPLETED)
				{
					int nPrintedCount = GetPrintedCountByJobId(pJobInfo->nJobId);
					if (pJobInfo->nPrintedPage < nPrintedCount )
					{
						pJobInfo->nPrintedPage = nPrintedCount;
					}
					else
					{
						theLog.Write("##CSnmpJobMonitorJC::CheckJob,8,nJobId=%d,nPrintedPage=%d,nPrintedCount=%d"
							, pJobInfo->nJobId, pJobInfo->nPrintedPage, nPrintedCount);
					}
				}
				m_pCallBack->OnJobInfo(pJobInfo);
			}

			if (pJobInfo->JobStatus == SCP_JOB_STATUS_COMPLETED)
			{
				m_oJobMap.RemoveKey(nJobId);
				delete pJobInfo;
			}
		}
	}
}

int CSnmpJobMonitorJC::CheckLastPrintCount()
{
	//CheckLastPrintCount_Color和CheckLastPrintCount_WB一定要先于CheckLastPrintCount调用
	CheckLastPrintCount_Color();
	CheckLastPrintCount_WB();
	m_nLastPrintCount = m_nLastPrintCount_Color + m_nLastPrintCount_WB;
	return m_nLastPrintCount;
}

int CSnmpJobMonitorJC::CheckLastPrintCount_Color()
{
	m_nLastPrintCount_Color = 0;
	return m_nLastPrintCount_Color;
}

int CSnmpJobMonitorJC::CheckLastPrintCount_WB()
{
	m_nLastPrintCount_WB = 0;
	return m_nLastPrintCount_WB;
}

int CSnmpJobMonitorJC::CheckLastCopyCount()
{
	//CheckLastCopyCount_Color和CheckLastCopyCount_WB一定要先于CheckLastCopyCount调用
	CheckLastCopyCount_Color();
	CheckLastCopyCount_WB();
	m_nLastCopyCount = m_nLastCopyCount_Color + m_nLastCopyCount_WB;
	return m_nLastCopyCount;
}

int CSnmpJobMonitorJC::CheckLastCopyCount_Color()
{
	m_nLastCopyCount_Color = 0;
	return m_nLastCopyCount_Color;
}

int CSnmpJobMonitorJC::CheckLastCopyCount_WB()
{
	m_nLastCopyCount_WB = 0;
	return m_nLastCopyCount_WB;
}

int CSnmpJobMonitorJC::CheckLastScanCount()
{
	m_nLastScanCount = 0;
	return m_nLastScanCount;
}

int CSnmpJobMonitorJC::CheckLastScanCount_Color()
{
	m_nLastScanCount_Color = 0;
	return m_nLastScanCount_Color;
}

int CSnmpJobMonitorJC::CheckLastScanCount_WB()
{
	m_nLastScanCount_WB = 0;
	return m_nLastScanCount_WB;
}

int CSnmpJobMonitorJC::CheckLastFaxCount()
{
	CheckLastFaxCount_Color();
	CheckLastFaxCount_WB();
	m_nLastFaxCount = m_nLastFaxCount_Color + m_nLastFaxCount_WB;
	return m_nLastFaxCount;
}

int CSnmpJobMonitorJC::CheckLastFaxCount_Color()
{
	m_nLastFaxCount_Color = 0;
	return m_nLastFaxCount_Color;
}

int CSnmpJobMonitorJC::CheckLastFaxCount_WB()
{
	m_nLastFaxCount_WB = 0;
	return m_nLastFaxCount_WB;
}


int CSnmpJobMonitorJC::CheckLastPageCount_A3()
{
	m_nA3PageCount = 0;
	return m_nA3PageCount;
}

int CSnmpJobMonitorJC::CheckLastPageCount_A4()
{
	m_nA4PageCount = 0;
	return m_nA4PageCount;
}

//本函数获取打印结束的任务
int CSnmpJobMonitorJC::GetPrintedCountByJobId(int nJobId)
{
	//theLog.Write("CSnmpJobMonitorJC::GetPrintedCountByJobId,1,nJobId=%d", nJobId);

	int nPrintedCount = 0;
	//作业已经出纸页数
	CString szPrintedCount;
	szPrintedCount.Format("%s.%d", m_szKyoceraHistoryJobInfoPrintedCountOID, nJobId);
	if (!GetRequest(CStringToChar(szPrintedCount), nPrintedCount))
	{
		theLog.Write("!!CSnmpJobMonitorJC::GetPrintedCountByJobId,2,GetRequest fail");
	}

	return nPrintedCount;
}
