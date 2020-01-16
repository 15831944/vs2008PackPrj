#include "StdAfx.h"
#include "SnmpJobMonitorDZ.h"
#include "ToshibaMIB_Def.h"

CSnmpJobMonitorDZ::CSnmpJobMonitorDZ()
{
	InitOID();
}

CSnmpJobMonitorDZ::~CSnmpJobMonitorDZ(void)
{
}

void CSnmpJobMonitorDZ::InitOID()
{
	m_szToshibaTotalPageCountOID = DecryptOID(ToshibaTotalPageCountOID);
}

void CSnmpJobMonitorDZ::CheckJob()
{
	if (!CheckIPActive())
	{
		return;
	}

	GetPrintJobList();
}

int CSnmpJobMonitorDZ::CheckLastPrintCount()
{
	//CheckLastPrintCount_Color和CheckLastPrintCount_WB一定要先于CheckLastPrintCount调用
	CheckLastPrintCount_Color();
	CheckLastPrintCount_WB();
	m_nLastPrintCount = m_nLastPrintCount_Color + m_nLastPrintCount_WB;
	return m_nLastPrintCount;
}

int CSnmpJobMonitorDZ::CheckLastPrintCount_Color()
{
	m_nLastPrintCount_Color = 0;
	return m_nLastPrintCount_Color;
}

int CSnmpJobMonitorDZ::CheckLastPrintCount_WB()
{
	m_nLastPrintCount_WB = 0;
	return m_nLastPrintCount_WB;
}

int CSnmpJobMonitorDZ::CheckLastCopyCount()
{
	//CheckLastCopyCount_Color和CheckLastCopyCount_WB一定要先于CheckLastCopyCount调用
	CheckLastCopyCount_Color();
	CheckLastCopyCount_WB();
	m_nLastCopyCount = m_nLastCopyCount_Color + m_nLastCopyCount_WB;
	return m_nLastCopyCount;
}

int CSnmpJobMonitorDZ::CheckLastCopyCount_Color()
{
	m_nLastCopyCount_Color = 0;
	return m_nLastCopyCount_Color;
}

int CSnmpJobMonitorDZ::CheckLastCopyCount_WB()
{
	m_nLastCopyCount_WB = 0;
	return m_nLastCopyCount_WB;
}

int CSnmpJobMonitorDZ::CheckLastScanCount()
{
	m_nLastScanCount = 0;
	return m_nLastScanCount;
}

int CSnmpJobMonitorDZ::CheckLastScanCount_Color()
{
	m_nLastScanCount_Color = 0;
	return m_nLastScanCount_Color;
}

int CSnmpJobMonitorDZ::CheckLastScanCount_WB()
{
	m_nLastScanCount_WB = 0;
	return m_nLastScanCount_WB;
}

int CSnmpJobMonitorDZ::CheckLastFaxCount()
{
	CheckLastFaxCount_Color();
	CheckLastFaxCount_WB();
	m_nLastFaxCount = m_nLastFaxCount_Color + m_nLastFaxCount_WB;
	return m_nLastFaxCount;
}

int CSnmpJobMonitorDZ::CheckLastFaxCount_Color()
{
	m_nLastFaxCount_Color = 0;
	return m_nLastFaxCount_Color;
}

int CSnmpJobMonitorDZ::CheckLastFaxCount_WB()
{
	m_nLastFaxCount_WB = 0;
	return m_nLastFaxCount_WB;
}

int CSnmpJobMonitorDZ::CheckLastPageCount_A3()
{
	m_nA3PageCount = 0;
	return m_nA3PageCount;
}

int CSnmpJobMonitorDZ::CheckLastPageCount_A4()
{
	m_nA4PageCount = 0;
	return m_nA4PageCount;
}

//本函数只能获取打印正常结束的任务
int CSnmpJobMonitorDZ::GetPrintedCountByJobId(int nJobId)
{
	return 0;
}

int CSnmpJobMonitorDZ::CheckLastPrintCount2()
{
	int nAllPages = 0;

	//总计数：原稿张数
	if (!GetRequest(CStringToChar(m_szToshibaTotalPageCountOID), nAllPages))	//AllPages=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.3.1.4"
	{
		theLog.Write("!!CSnmpJobMonitorDZ::CheckLastPrintCount2,GetRequest fail");
	}

	m_nLastPrintCount = nAllPages;
	return m_nLastPrintCount;
}

//获取已经打印列表
void CSnmpJobMonitorDZ::GetPrintJobList()
{
	/* 通过调用一个外部工具来获取打印日志，外部工具是基于.Net4.0运行环境的。
	使用命令行调用，按照顺序传递10个参数。
	示例：OperatePrinterLog.exe GetLog "192.168.4.180" "user" "password" 10 "SearchKey" "SearchValue" 1 100 "C:\printlog.xml"
	其中：
	参数1：GetLog，标识是获取日志
	参数2：打印机的IP
	参数3：打印机的登录用户,默认为admin
	参数4：打印机的登录密码,默认为123456
	参数5：获取的日志类型，5 - 扫描，6 - 复印，7 - 传真，8 - 传真接收，9 - 传真发送，10 - 打印，17 - Maintenance
	参数6：日志过滤类型，< 空 >，< UserName > 或者 < DepartmentCode >
	参数7：日志过滤内容
	参数8：日志的起始记录
	参数9：获取日志的最大个数
	参数10：日志保留的路径，一般是XML格式
	*/

	theLog.Write("CSnmpJobMonitorDZ::GetPrintJobList,1,begin");

	CString szPrinterIP = GetPrtIP();
	CString szUser = "admin";
	CString szPassword = "123456";
	CString szDefaultPath = CCommonFun::GetDefaultPath();
	CString szToolPath;
	szToolPath.Format("%sToshiba\\OperatePrinterLog.exe", szDefaultPath);
	CString szLogPath;
	szLogPath.Format("%sToshiba\\printlog_%s.xml", szDefaultPath, szPrinterIP);

	//删除原来的日志文件
	DeleteFile(szLogPath);

	CString szCmdLine;
	szCmdLine.Format("\"%s\" \"%s\" \"%s\" \"%s\" \"%s\" %d \"%s\" \"%s\" %d %d \"%s\" "
		, szToolPath, "GetLog", szPrinterIP, szUser, szPassword, ToshibaLogType_Print, "", "", 1, 100, szLogPath);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(0, szCmdLine.GetBuffer(), 0, 0, FALSE, 0, 0, 0, &si, &pi))
	{
		theLog.Write("!!CSnmpJobMonitorDZ::GetPrintJobList,2,CreateProcess fail,error=%u,szCmdLine=%s", GetLastError(), szCmdLine);
		return;
	}

	BOOL bGetLogSucc = FALSE;
	theLog.Write("CSnmpJobMonitorDZ::GetPrintJobList,3,CreateProcess succ,dwProcessId=%u", pi.dwProcessId);
	DWORD dwTimeoutSecond = 60*3;
	DWORD dwRet = WaitForSingleObject(pi.hProcess, dwTimeoutSecond*1000);
	if (dwRet == WAIT_TIMEOUT)
	{
		theLog.Write("!!CSnmpJobMonitorDZ::GetPrintJobList,4,WaitForSingleObject WAIT_TIMEOUT 作业处理超时(%d秒)，强制结束打印作业,dwProcessId=%u"
			, dwTimeoutSecond, pi.dwProcessId);
		CCommonFun::TerminaPID(pi.dwProcessId);
	}
	else
	{
		theLog.Write("CSnmpJobMonitorDZ::GetPrintJobList,5,WaitForSingleObject succ,dwProcessId=%u", pi.dwProcessId);
		DWORD dwExitCode = 0;	//STILL_ACTIVE,EXCEPTION_INT_DIVIDE_BY_ZERO
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		if (dwExitCode == 1)
		{
			bGetLogSucc = TRUE;
		}
		else
		{
			theLog.Write("!!CSnmpJobMonitorDZ::GetPrintJobList,6,fail,dwExitCode=%u(0x%X),dwProcessId=%u", dwExitCode, dwExitCode, pi.dwProcessId);
		}
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (bGetLogSucc)
	{
		SCP_JobInfo_List PrintedJobList;

		CXmlParse xml;
		if (xml.ParseXml(szLogPath))
		{
			//首先清理复印日志（因为获取打印日志的接口中，实际会得到打印和复印两个日志）
			ClearCopyLog(xml);

			//然后处理打印日志
			do 
			{
				MSXML2::IXMLDOMDocument2Ptr doc = xml.GetXmlDocment();
				MSXML2::IXMLDOMNodePtr getLogResponse = xml.SelectSingleNode(doc, "getLogResponse");
				MSXML2::IXMLDOMNodePtr Log = xml.SelectSingleNode(getLogResponse, "Log");
				MSXML2::IXMLDOMNodePtr OperationLog = xml.SelectSingleNode(Log, "OperationLog");
				MSXML2::IXMLDOMNodePtr JobLog = xml.SelectSingleNode(OperationLog, "JobLog");
				MSXML2::IXMLDOMNodePtr PrintLog = xml.SelectSingleNode(JobLog, "PrintLog");
				MSXML2::IXMLDOMNodeListPtr PrintLogEntryList = xml.SelectNodes(PrintLog, "PrintLogEntry");
				if (!PrintLogEntryList)
				{
					theLog.Write("##CSnmpJobMonitorDZ::GetPrintJobList,7.1,PrintLogEntryList=%p", PrintLogEntryList);
					break;
				}
				long nCount = PrintLogEntryList->Getlength();
				theLog.Write("CSnmpJobMonitorDZ::GetPrintJobList,7,nCount=%d", nCount);
				for (long index=0; index<nCount; index++)
				{
					CString szId, szJobType, szColorMode, szPaperSize, szSets, szOriginalPages, 
						szTotalPapers, szPrintedPages, szDocumentName;
					MSXML2::IXMLDOMNodePtr PrintLogEntry = PrintLogEntryList->Getitem(index);
					xml.GetNodeValue(PrintLogEntry, "Id", szId);
					xml.GetNodeValue(PrintLogEntry, "JobType", szJobType);
					xml.GetNodeValue(PrintLogEntry, "ColorMode", szColorMode);
					xml.GetNodeValue(PrintLogEntry, "PaperSize", szPaperSize);
					xml.GetNodeValue(PrintLogEntry, "Sets", szSets);
					xml.GetNodeValue(PrintLogEntry, "OriginalPages", szOriginalPages);
					xml.GetNodeValue(PrintLogEntry, "TotalPapers", szTotalPapers);
					xml.GetNodeValue(PrintLogEntry, "PrintedPages", szPrintedPages);
					xml.GetNodeValue(PrintLogEntry, "DocumentName", szDocumentName);

					//获取BasicInfo
					CString szStartTime, szFinishTime, szStatus, szStatusCode;
					MSXML2::IXMLDOMNodePtr BasicInfo = xml.SelectSingleNode(PrintLogEntry, "BasicInfo");
					xml.GetNodeValue(BasicInfo, "StartTime", szStartTime);
					xml.GetNodeValue(BasicInfo, "FinishTime", szFinishTime);
					xml.GetNodeValue(BasicInfo, "Status", szStatus);
					xml.GetNodeValue(BasicInfo, "StatusCode", szStatusCode);

					/*
					theLog.Write("CSnmpJobMonitorDZ::GetPrintJobList,8,szId=%s,szJobType=%s,szColorMode=%s,"
						"szPaperSize=%s,szSets=%s,szOriginalPages=%s,szTotalPapers=%s,szPrintedPages=%s,"
						"szDocumentName=%s,szStartTime=%s,szFinishTime=%s,szStatus=%s,szStatusCode=%s"
						, szId, szJobType, szColorMode, szPaperSize, szSets, szOriginalPages, szTotalPapers
						, szPrintedPages, szDocumentName, szStartTime, szFinishTime, szStatus, szStatusCode);
					*/

					SCP_JobInfo info;
					memset(&info,0x0,sizeof(SCP_JobInfo));
					info.nJobId = atoi(szId);
					info.nPrintedPage = atoi(szPrintedPages);
					info.nTotalPage = atoi(szOriginalPages);
					info.JobType = SCP_JOB_TYPE_PRINT;
					info.nCopies = atoi(szSets);
					//info.bColor = (szColorMode.CompareNoCase("Monochrome") == 0) ? false : true;
					//info.nMediaSize = (szColorMode.CompareNoCase("A4") == 0) ? 9 : 8;
					//COleDateTime time;
					//time.ParseDateTime(szStartTime);
					//info.fJobStartTime = time.m_dt;
					//time.ParseDateTime(szFinishTime);
					//info.fJobEndTime = time.m_dt;
					strcpy_s(info.cJobName, szDocumentName);
					int nStatusCode = atoi(szStatusCode);
					info.JobStatus = SCP_JOB_STATUS_COMPLETED;
					/*
					if (nStatusCode == 4000)
					{
						info.JobStatus = SCP_JOB_STATUS_COMPLETED;
					}
					else if (nStatusCode == 4011)
					{
						info.JobStatus = SCP_JOB_STATUS_CANCELED;
					}
					else
					{
						info.JobStatus = SCP_JOB_STATUS_PRINTING;
					}
					*/

					PrintedJobList.push_back(info);
				}			
			} while (FALSE);
		}
		else
		{
			theLog.Write("!!CSnmpJobMonitorDZ::GetPrintJobList,9,ParseXml fail");
		}

		if (PrintedJobList.size() > 0 && m_pCallBack)
		{
			m_pCallBack->OnJobInfoList(&PrintedJobList);

			//清理打印记录
			ClearPrintJobList(PrintedJobList);
		}
	}

	theLog.Write("CSnmpJobMonitorDZ::GetPrintJobList,100,end");
}

//清理打印机上的打印记录
void CSnmpJobMonitorDZ::ClearPrintJobList(SCP_JobInfo_List& PrintedJobList)
{
	SCP_JobInfo_List_I it;
	for (it=PrintedJobList.begin(); it!=PrintedJobList.end(); it++)
	{
		if (it->JobVerifyResult == SCP_JOB_VERIFY_RESULT_SUCC)
		{
			theLog.Write("CSnmpJobMonitorDZ::ClearPrintJobList,nJobId=%d,cJobName=%s"
				, it->nJobId, it->cJobName);
			ClearJobLog(ToshibaLogType_Print, it->nJobId);
		}
	}
}

//清理复印记录日志
void CSnmpJobMonitorDZ::ClearCopyLog(CXmlParse& xml)
{
	MSXML2::IXMLDOMDocument2Ptr doc = xml.GetXmlDocment();
	MSXML2::IXMLDOMNodePtr getLogResponse = xml.SelectSingleNode(doc, "getLogResponse");
	MSXML2::IXMLDOMNodePtr Log = xml.SelectSingleNode(getLogResponse, "Log");
	MSXML2::IXMLDOMNodePtr OperationLog = xml.SelectSingleNode(Log, "OperationLog");
	MSXML2::IXMLDOMNodePtr JobLog = xml.SelectSingleNode(OperationLog, "JobLog");
	MSXML2::IXMLDOMNodePtr CopyLog = xml.SelectSingleNode(JobLog, "CopyLog");
	MSXML2::IXMLDOMNodeListPtr CopyLogEntryList = xml.SelectNodes(CopyLog, "CopyLogEntry");
	if (!CopyLogEntryList)
	{
		theLog.Write("##CSnmpJobMonitorDZ::ClearCopyLog,1,CopyLogEntryList=%p", CopyLogEntryList);
		return;
	}
	long nCount = CopyLogEntryList->Getlength();
	theLog.Write("CSnmpJobMonitorDZ::ClearCopyLog,2,nCount=%d", nCount);
	for (long index=0; index<nCount; index++)
	{
		CString szId, szJobType, szColorMode, szPaperSize, szSets, szOriginalPages, 
			szTotalPapers, szPrintedPages, szDocumentName;
		MSXML2::IXMLDOMNodePtr CopyLogEntry = CopyLogEntryList->Getitem(index);
		xml.GetNodeValue(CopyLogEntry, "Id", szId);
		/*
		xml.GetNodeValue(CopyLogEntry, "JobType", szJobType);
		xml.GetNodeValue(CopyLogEntry, "ColorMode", szColorMode);
		xml.GetNodeValue(CopyLogEntry, "PaperSize", szPaperSize);
		xml.GetNodeValue(CopyLogEntry, "Sets", szSets);
		xml.GetNodeValue(CopyLogEntry, "OriginalPages", szOriginalPages);
		xml.GetNodeValue(CopyLogEntry, "TotalPapers", szTotalPapers);
		xml.GetNodeValue(CopyLogEntry, "PrintedPages", szPrintedPages);
		xml.GetNodeValue(CopyLogEntry, "DocumentName", szDocumentName);

		//获取BasicInfo
		CString szStartTime, szFinishTime, szStatus, szStatusCode;
		MSXML2::IXMLDOMNodePtr BasicInfo = xml.SelectSingleNode(CopyLogEntry, "BasicInfo");
		xml.GetNodeValue(BasicInfo, "StartTime", szStartTime);
		xml.GetNodeValue(BasicInfo, "FinishTime", szFinishTime);
		xml.GetNodeValue(BasicInfo, "Status", szStatus);
		xml.GetNodeValue(BasicInfo, "StatusCode", szStatusCode);

		theLog.Write("CSnmpJobMonitorDZ::ClearCopyLog,3,szId=%s,szJobType=%s,szColorMode=%s,"
			"szPaperSize=%s,szSets=%s,szOriginalPages=%s,szTotalPapers=%s,szPrintedPages=%s,"
			"szDocumentName=%s,szStartTime=%s,szFinishTime=%s,szStatus=%s,szStatusCode=%s"
			, szId, szJobType, szColorMode, szPaperSize, szSets, szOriginalPages, szTotalPapers
			, szPrintedPages, szDocumentName, szStartTime, szFinishTime, szStatus, szStatusCode);
		*/

		ClearJobLog(ToshibaLogType_Copy, atoi(szId));
	}	

	theLog.Write("CSnmpJobMonitorDZ::ClearCopyLog,100,end");
}

//清理指定类型和ID的日志记录
void CSnmpJobMonitorDZ::ClearJobLog(ToshibaLogType nLogType, int nLogId)
{
	/* 通过调用一个外部工具来清理日志，外部工具是基于.Net4.0运行环境的。
	使用命令行调用，按照顺序传递6个参数。
	示例：OperatePrinterLog.exe DeleteLog "192.168.4.180" "user" "password" 10 8024
	其中：
	参数1：ClearLog，标识是清理日志
	参数2：打印机的IP
	参数3：打印机的登录用户,默认为admin
	参数4：打印机的登录密码,默认为123456
	参数5：清理的日志类型，5 - 扫描，6 - 复印，7 - 传真，8 - 传真接收，9 - 传真发送，10 - 打印，17 - Maintenance
	参数6：清理的日志ID
	*/

	theLog.Write("CSnmpJobMonitorDZ::ClearJobLog,1,begin,nLogType=%d,nLogId=%d", nLogType, nLogId);

	CString szPrinterIP = GetPrtIP();
	CString szUser = "admin";
	CString szPassword = "123456";
	CString szDefaultPath = CCommonFun::GetDefaultPath();
	CString szToolPath;
	szToolPath.Format("%sToshiba\\OperatePrinterLog.exe", szDefaultPath);

	CString szCmdLine;
	szCmdLine.Format("\"%s\" \"%s\" \"%s\" \"%s\" \"%s\" %d %d "
		, szToolPath, "ClearLog", szPrinterIP, szUser, szPassword, nLogType, nLogId);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
// 	si.dwFlags = STARTF_USESHOWWINDOW;
// 	si.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(0, szCmdLine.GetBuffer(), 0, 0, FALSE, 0, 0, 0, &si, &pi))
	{
		theLog.Write("!!CSnmpJobMonitorDZ::ClearJobLog,2,CreateProcess fail,error=%u,szCmdLine=%s", GetLastError(), szCmdLine);
		return;
	}

	BOOL bGetLogSucc = FALSE;
	//theLog.Write("CSnmpJobMonitorDZ::ClearJobLog,3,CreateProcess succ,dwProcessId=%u", pi.dwProcessId);
	DWORD dwTimeoutSecond = 60*3;
	DWORD dwRet = WaitForSingleObject(pi.hProcess, dwTimeoutSecond*1000);
	if (dwRet == WAIT_TIMEOUT)
	{
		theLog.Write("!!CSnmpJobMonitorDZ::ClearJobLog,4,WaitForSingleObject WAIT_TIMEOUT 作业处理超时(%d秒)，强制结束打印作业,dwProcessId=%u"
			, dwTimeoutSecond, pi.dwProcessId);
		CCommonFun::TerminaPID(pi.dwProcessId);
	}
	else
	{
		//theLog.Write("CSnmpJobMonitorDZ::ClearJobLog,5,WaitForSingleObject succ,dwProcessId=%u", pi.dwProcessId);
		DWORD dwExitCode = 0;	//STILL_ACTIVE,EXCEPTION_INT_DIVIDE_BY_ZERO
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		if (dwExitCode == 1)
		{
			bGetLogSucc = TRUE;
		}
		else
		{
			theLog.Write("!!CSnmpJobMonitorDZ::ClearJobLog,6,fail,dwExitCode=%u(0x%X),dwProcessId=%u", dwExitCode, dwExitCode, pi.dwProcessId);
		}
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	theLog.Write("CSnmpJobMonitorDZ::ClearJobLog,100,end");
}
