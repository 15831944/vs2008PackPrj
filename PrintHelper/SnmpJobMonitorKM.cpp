#include "StdAfx.h"
#include "SnmpJobMonitorKM.h"
#include "KonicaMinoltaMIB_Def.h"
#include "ISecPrintLogMonitor.h"

#define KM_CHECK_STATUS_WITH_LOG 0 //用OpenAPI的获取日志功能来抄表

CSnmpJobMonitorKM::CSnmpJobMonitorKM()
{
	InitOID();
}

CSnmpJobMonitorKM::~CSnmpJobMonitorKM(void)
{
}

void CSnmpJobMonitorKM::InitOID()
{
	m_szKmJmGeneralEntryOID = DecryptOID(KmJmGeneralEntryOID);
	m_szKmJmJobEntryOID = DecryptOID(KmJmJobEntryOID);
	m_szKmJobCountOID.Format("%s.2.1", m_szKmJmGeneralEntryOID);
	m_szKmJobIndexOldOID.Format("%s.3.1", m_szKmJmGeneralEntryOID);

	m_szKmTotalPageCountOID = DecryptOID(KmTotalPageCountOID);
}

void CSnmpJobMonitorKM::CheckJob()
{
	if (!CheckIPActive())
	{
		return;
	}
	//Get log with KM openAPI
	//GetPrintJobList();


#if KM_CHECK_STATUS_WITH_LOG
	//Get log with KM openAPI
	GetPrintJobList();
	//GetJobList();
#else
	int nActiveJobCount = 0;
	int nActiveJobIndexOld = 0;
	int nActiveJobIndexNew = 0;
	if(GetRequest(CStringToChar(m_szKmJobCountOID), nActiveJobCount))	//".1.3.6.1.4.1.18334.1.1.4.1.2.1.1.2.1"
	{
		if (nActiveJobCount>0)
		{
			//theLog.Write("CSnmpJobMonitorKM::CheckJob,1,nActiveJobCount = %d", nActiveJobCount);
			if(GetRequest(CStringToChar(m_szKmJobIndexOldOID), nActiveJobIndexOld) && nActiveJobIndexOld>0)	//".1.3.6.1.4.1.18334.1.1.4.1.2.1.1.3.1"
			{
				//theLog.Write("CSnmpJobMonitorKM::CheckJob,2,nActiveJobIndexOld = %d", nActiveJobIndexOld);
				int nValue;
				CString szValue;

				//作业功能
				//1-copy; 2-print; 3-receiveJob; 4-sendJob; 7-other; 10-multi
				CString szJobFunction;	
				szJobFunction.Format("%s.3.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);	//.1.3.6.1.4.1.18334.1.1.4.1.3.1.1
				if(GetRequest(CStringToChar(szJobFunction), nValue) && nValue!=2)
				{
					//theLog.Write("CSnmpJobMonitorKM::CheckJob,3,JobFunction = %d", nValue);
					return;
				}

				//作业类型
				//10-scanSend; 11-email; 13-scanToServer; 14-scanToPc; 15-scanToHDD; 16-twain; 17-faxSend;
				//20-normalPrint; 22-securePrint; 24-printToHDD; 32-FaxReceive; 40-copy; 60-boxStore;
				//100-scanToUSB; 110-scanToWebDAV;	111-scanServer;
				CString szJobType;
				szJobType.Format("%s.11.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				if(GetRequest(CStringToChar(szJobType), nValue) && nValue!=20)
				{
					//theLog.Write("CSnmpJobMonitorKM::CheckJob,3,JobType = %d", nValue);
					return;
				}

				//作业状态
				//2-cancelRequest; 10-pending; 20-processing; 21-sending; 22-receviving
				//23-printing; 24-scanning; 25-procesingStopped; 30-completed; 31-caution
				//32-aborted; 33-canceled; 34:sendCompleteed;
				CString szJobStatus;	
				szJobStatus.Format("%s.4.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				if(!GetRequest(CStringToChar(szJobStatus), nValue))
				{
					//theLog.Write("CSnmpJobMonitorKM::CheckJob,3.2,JobStatus = %d", nValue);
					return;
				}
				//theLog.Write("CSnmpJobMonitorKM::CheckJob,3.3,JobStatus = %d", nValue);

				/*
				//作业状态详情
				CString szJobStatusDetials;
				szJobStatusDetials.Format("%s.5.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				if(!GetRequestStr(CStringToChar(szJobStatusDetials), szValue))
				{
					return;
				}
				CString szJobName;
				szJobName.Format("%s.12.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				CString szJobDocPageNumbers;
				szJobDocPageNumbers.Format("%s.13.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				CString szJobDocCopyNumbers;
				szJobDocCopyNumbers.Format("%s.14.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				CString szJobPageSize;
				szJobPageSize.Format("%s.15.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);
				CString szJobDestination;
				szJobDestination.Format("%s.16.1.%d", m_szKmJmJobEntryOID, nActiveJobIndexOld);			
				*/
			}
			else
			{
				//theLog.Write("!!CSnmpJobMonitorKM::CheckJob,4,fail");
				//return;
			}
		}
	}
	else
	{
		//theLog.Write("CSnmpJobMonitorKM::CheckJob,5,fail");
		return;
	}

	//m_oJobMap.RemoveAll();
	BOOL bNewJobAdd = (nActiveJobIndexOld>0) ? TRUE : FALSE;
	CJobMap::CPair* pFirstVal = m_oJobMap.PGetFirstAssoc();
	if (pFirstVal && pFirstVal->value)
	{
		PSCP_JobInfo value = pFirstVal->value;
		int nJobId = value->nJobId;
		if (nJobId != nActiveJobIndexOld)
		{
			int nPreLastPrintCount = m_nLastPrintCount;
			CheckLastPrintCount2();
			if (m_pCallBack)
			{
				value->nPrintedPage = m_nLastPrintCount - nPreLastPrintCount;
				theLog.Write("CSnmpJobMonitorKM::CheckJob,6,SCP_JOB_STATUS_COMPLETED,nJobId=%d,nPrintedPage=%d", nJobId, value->nPrintedPage);
				value->JobStatus = SCP_JOB_STATUS_COMPLETED;
				m_pCallBack->OnJobInfo(value);
			}
			delete value;
			m_oJobMap.RemoveKey(nJobId);
		}
		else
		{
			bNewJobAdd = FALSE;
			if (m_pCallBack)
			{
				theLog.Write("CSnmpJobMonitorKM::CheckJob,7,SCP_JOB_STATUS_PROCESSING,nJobId=%d", nActiveJobIndexOld);
				value->JobStatus = SCP_JOB_STATUS_PROCESSING;
				m_pCallBack->OnJobInfo(value);
			}
		}
	}
	else if (pFirstVal)
	{
		theLog.Write("!!CSnmpJobMonitorKM::CheckJob,8,pFirstVal=%p,pFirstVal->value=%p", pFirstVal, pFirstVal->value);
	}
	
	if (bNewJobAdd)
	{
		CheckLastPrintCount2();
		PSCP_JobInfo pJobInfo = new SCP_JobInfo;
		memset(pJobInfo, 0x0, sizeof(SCP_JobInfo));
		pJobInfo->nJobId = nActiveJobIndexOld;
		pJobInfo->JobType = SCP_JOB_TYPE_PRINT; 
		pJobInfo->JobStatus = SCP_JOB_STATUS_START;
		m_oJobMap.SetAt(nActiveJobIndexOld, pJobInfo);
		if (m_pCallBack)
		{
			theLog.Write("CSnmpJobMonitorKM::CheckJob,9,SCP_JOB_STATUS_START,nJobId=%d", nActiveJobIndexOld);
			m_pCallBack->OnJobInfo(pJobInfo);
		}
	}
#endif
}

int CSnmpJobMonitorKM::CheckLastPrintCount()
{
	//CheckLastPrintCount_Color和CheckLastPrintCount_WB一定要先于CheckLastPrintCount调用
	CheckLastPrintCount_Color();
	CheckLastPrintCount_WB();
	m_nLastPrintCount = m_nLastPrintCount_Color + m_nLastPrintCount_WB;
	return m_nLastPrintCount;
}

int CSnmpJobMonitorKM::CheckLastPrintCount_Color()
{
	m_nLastPrintCount_Color = 0;
	return m_nLastPrintCount_Color;
}

int CSnmpJobMonitorKM::CheckLastPrintCount_WB()
{
	m_nLastPrintCount_WB = 0;
	return m_nLastPrintCount_WB;
}

int CSnmpJobMonitorKM::CheckLastCopyCount()
{
	//CheckLastCopyCount_Color和CheckLastCopyCount_WB一定要先于CheckLastCopyCount调用
	CheckLastCopyCount_Color();
	CheckLastCopyCount_WB();
	m_nLastCopyCount = m_nLastCopyCount_Color + m_nLastCopyCount_WB;
	return m_nLastCopyCount;
}

int CSnmpJobMonitorKM::CheckLastCopyCount_Color()
{
	m_nLastCopyCount_Color = 0;
	return m_nLastCopyCount_Color;
}

int CSnmpJobMonitorKM::CheckLastCopyCount_WB()
{
	m_nLastCopyCount_WB = 0;
	return m_nLastCopyCount_WB;
}

int CSnmpJobMonitorKM::CheckLastScanCount()
{
	m_nLastScanCount = 0;
	return m_nLastScanCount;
}

int CSnmpJobMonitorKM::CheckLastScanCount_Color()
{
	m_nLastScanCount_Color = 0;
	return m_nLastScanCount_Color;
}

int CSnmpJobMonitorKM::CheckLastScanCount_WB()
{
	m_nLastScanCount_WB = 0;
	return m_nLastScanCount_WB;
}

int CSnmpJobMonitorKM::CheckLastFaxCount()
{
	CheckLastFaxCount_Color();
	CheckLastFaxCount_WB();
	m_nLastFaxCount = m_nLastFaxCount_Color + m_nLastFaxCount_WB;
	return m_nLastFaxCount;
}

int CSnmpJobMonitorKM::CheckLastFaxCount_Color()
{
	m_nLastFaxCount_Color = 0;
	return m_nLastFaxCount_Color;
}

int CSnmpJobMonitorKM::CheckLastFaxCount_WB()
{
	m_nLastFaxCount_WB = 0;
	return m_nLastFaxCount_WB;
}

int CSnmpJobMonitorKM::CheckLastPageCount_A3()
{
	m_nA3PageCount = 0;
	return m_nA3PageCount;
}

int CSnmpJobMonitorKM::CheckLastPageCount_A4()
{
	m_nA4PageCount = 0;
	return m_nA4PageCount;
}

//本函数只能获取打印正常结束的任务
int CSnmpJobMonitorKM::GetPrintedCountByJobId(int nJobId)
{
	theLog.Write("CSnmpJobMonitorKM::GetPrintedCountByJobId,1,nJobId=%d", nJobId);

	int nPages = 0;
	int nJobPageNumbers = 0;
	int nJobDocCopyNumbers = 0;

	//作业每份页数
	CString szJobDocPageNumbers;
	szJobDocPageNumbers.Format("%s.13.1.%d", m_szKmJmJobEntryOID, nJobId);	//.1.3.6.1.4.1.18334.1.1.4.1.3.1.1
	if (!GetRequest(CStringToChar(szJobDocPageNumbers), nJobPageNumbers))
	{
		theLog.Write("!!CSnmpJobMonitorKM::GetPrintedCountByJobId,2,GetRequest fail");
		return nPages;
	}

	//作业份数
	CString szJobDocCopyNumbers;
	szJobDocCopyNumbers.Format("%s.14.1.%d", m_szKmJmJobEntryOID, nJobId);
	if (!GetRequest(CStringToChar(szJobDocCopyNumbers), nJobDocCopyNumbers))
	{
		theLog.Write("!!CSnmpJobMonitorKM::GetPrintedCountByJobId,3,GetRequest fail");
		return nPages;
	}

	nPages = nJobPageNumbers * nJobDocCopyNumbers;
	theLog.Write("CSnmpJobMonitorKM::GetPrintedCountByJobId,4,GetRequest succ,nJobId=%d,nPages=%d"
		",nJobPageNumbers=%d,nJobDocCopyNumbers=%d", nJobId, nPages, nJobPageNumbers, nJobDocCopyNumbers);

	return nPages;
}

int CSnmpJobMonitorKM::CheckLastPrintCount2()
{
	int nAllPages = 0;

	//总计数：原稿张数
	if (!GetRequest(CStringToChar(m_szKmTotalPageCountOID), nAllPages))	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.1.8.0"
	{
		theLog.Write("!!CSnmpJobMonitorKM::CheckLastPrintCount2,GetRequest fail");
	}

	m_nLastPrintCount = nAllPages;
	return m_nLastPrintCount;
}


//获取历史作业列表
void CSnmpJobMonitorKM::GetJobList()
{
	//1.查找所有打印作业记录，通过kmJmJobType来判断
	KmJmJobEntryList oKmJmJobList;
	char cOidBegin[128] =  {0};
	sprintf(cOidBegin, "%s.11", m_szKmJmJobEntryOID);	//".iso.org.dod.internet.private.enterprises.18334.1.1.4.1.3.1.1.11";	//kmJmJobType=11
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			ENUM_STRUCT_TYPE(KmJmJobType) kmJmJobType = (ENUM_STRUCT_TYPE(KmJmJobType))atoi(pszValue);
			if (kmJmJobType == ENUM_STRUCT_VALUE(KmJmJobType)::normalPrint)
			{
				int nIndex = GetOidEndNumber(cOidCurrent);
				PKmJmJobEntry pEntry = new KmJmJobEntry;
				memset(pEntry, 0x0, sizeof(KmJmJobEntry));
				oKmJmJobList.push_back(pEntry);
				pEntry->kmJmJobJobSetIndex = 1;
				pEntry->kmJmJobIndex = nIndex;
				pEntry->kmJmJobType = kmJmJobType;
			}
		}
		else
		{
			break;
		}
	}

	//2.根据打印面的纸型计数索引，获取具体的打印计数信息，如纸型、色彩、拾取面数和实际出纸面数。
	KmJmJobEntryList::iterator it;
	for (it=oKmJmJobList.begin(); it!=oKmJmJobList.end(); it++)
	{
		PKmJmJobEntry pEntry = (PKmJmJobEntry)(*it);
		if (pEntry)
		{
			int nIndex = pEntry->kmJmJobIndex;

			char cOidStr[128] = {0};
			//sprintf(cOidStr, "%s.1.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobJobSetIndex=1
			//GetRequest(cOidStr, pEntry->kmJmJobJobSetIndex);

			//sprintf(cOidStr, "%s.2.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobIndex=2
			//GetRequest(cOidStr, pEntry->kmJmJobIndex);

			//sprintf(cOidStr, "%s.3.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobFunction=3
			//GetRequest(cOidStr, (int&)pEntry->kmJmJobFunction);

			sprintf(cOidStr, "%s.4.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobStatus=4
			GetRequest(cOidStr, (int&)pEntry->kmJmJobStatus);

			//sprintf(cOidStr, "%s.5.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobStatusDetails=5
			//GetRequest(cOidStr, pEntry->kmJmJobStatusDetails, sizeof(pEntry->kmJmJobStatusDetails));

			//sprintf(cOidStr, "%s.6.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobReceivedTime=6
			//GetRequest(cOidStr, pEntry->kmJmJobReceivedTime, sizeof(pEntry->kmJmJobReceivedTime));

			//sprintf(cOidStr, "%s.7.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobCompleteTime=7
			//GetRequest(cOidStr, pEntry->kmJmJobCompleteTime, sizeof(pEntry->kmJmJobCompleteTime));

			//sprintf(cOidStr, "%s.8.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobPriority=8
			//GetRequest(cOidStr, pEntry->kmJmJobPriority);

			//sprintf(cOidStr, "%s.9.1.%d", m_szKmJmJobEntryOID, nIndex);			//kmJmJobCompleteTime=9
			//GetRequest(cOidStr, pEntry->kmJmJobDivNumber, sizeof(pEntry->kmJmJobDivNumber));

			//sprintf(cOidStr, "%s.10.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobOwner=10
			//GetRequest(cOidStr, pEntry->kmJmJobOwner, sizeof(pEntry->kmJmJobOwner));

			//sprintf(cOidStr, "%s.11.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobType=11
			//GetRequest(cOidStr, (int&)pEntry->kmJmJobType);

			sprintf(cOidStr, "%s.12.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobName=12
			GetRequest(cOidStr, pEntry->kmJmJobName, sizeof(pEntry->kmJmJobName));

			sprintf(cOidStr, "%s.13.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobDocPageNumbers=13
			GetRequest(cOidStr, pEntry->kmJmJobDocPageNumbers);

			sprintf(cOidStr, "%s.14.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobDocCopyNumbers=14
			GetRequest(cOidStr, pEntry->kmJmJobDocCopyNumbers);

			sprintf(cOidStr, "%s.15.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobPageSize=15
			GetRequest(cOidStr, (int&)pEntry->kmJmJobPageSize);

			//sprintf(cOidStr, "%s.16.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJokmJmJobDestinationbName=16
			//GetRequest(cOidStr, pEntry->kmJmJobDestination, sizeof(pEntry->kmJmJobDestination));

			//sprintf(cOidStr, "%s.17.1.%d", m_szKmJmJobEntryOID, nIndex);		//kmJmJobDriverJobID=17
			//GetRequest(cOidStr, pEntry->kmJmJobDriverJobID, sizeof(pEntry->kmJmJobDriverJobID));
		}
	}
}


//获取已经打印列表
void CSnmpJobMonitorKM::GetPrintJobList()
{
	/* 通过调用一个外部工具来获取打印日志，外部工具是基于.Net4.5运行环境的。
	GetLog.exe 192.168.4.185 Admin 1 2 0 ./temp.xml

	参数含义:
	参数1 :打印机IP
	参数2 :账号
	参数3 :密码
	参数4 :日志类型 1-统计日志，2-所有日志
	参数5 :是否获取最新的记录  1-获取最新的日志（会清除打印机中之前日志获取的记录） 0-获取旧的日志文件
	参数6 :保存的日志文件路径

	*/

	theLog.Write("CSnmpJobMonitorKM::GetPrintJobList,1,begin");

	//init parameters
	CString szPrinterIP = GetPrtIP();
	CString szUser = "Admin";
	CString szPassword = "12345678";//默认密码是12345678
	CString szDefaultPath = CCommonFun::GetDefaultPath();
	CString szToolPath;
	szToolPath.Format("%sKM\\tool\\GetLog.exe", szDefaultPath);
	CString szLogPath;
	szLogPath.Format("%sKM\\Temp_%s.xml", szDefaultPath, szPrinterIP);
	CString szLogType = "2";
	CString needCreateLog = "1";

	//删除原来的日志文件
	//DeleteFile(szLogPath);

	CString szCmdLine;
	szCmdLine.Format("\"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\""
		, szToolPath, szPrinterIP, szUser, szPassword, szLogType, needCreateLog);

	theLog.Write("CSnmpJobMonitorKM::GetPrintJobList,szCmdLine=[%s]", szCmdLine);
	IsecPrintLogMonitor* logMonitor = IsecPrintLogMonitor::getInstance();
	BOOL bGetLogSucc = FALSE;
	if(logMonitor){
		bGetLogSucc = logMonitor->init(std::string(szCmdLine));
		logMonitor->saveLogToPath(std::string(szLogPath));
		bGetLogSucc = logMonitor->getLog();
		if (bGetLogSucc)
		{
			SCP_JobInfo_List PrintedJobList;

			logMonitor->getPrintJobList(PrintedJobList);

			if (PrintedJobList.size() > 0 && m_pCallBack)
			{
				m_pCallBack->OnJobInfoList(&PrintedJobList);

				//清理打印记录
				//ClearPrintJobList(PrintedJobList);
			}
		} else {
			theLog.Write("CSnmpJobMonitorKM::GetPrintJobList getLog fail.");
		}
	}
	
	

	theLog.Write("CSnmpJobMonitorKM::GetPrintJobList,100,end");
}