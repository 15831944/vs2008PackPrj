#include "StdAfx.h"
#include "ISecPrintLogMonitor.h"
#include "comm.h"


using namespace Xab;

IsecPrintLogMonitor* IsecPrintLogMonitor:: _instance  = NULL;//初始化在主线程之前


IsecPrintLogMonitor::IsecPrintLogMonitor()
{
}
IsecPrintLogMonitor::~IsecPrintLogMonitor()
{
	m_bInitOk = FALSE;
	theLog.Write("run ~IsecPrintLogMonitor");
}

IsecPrintLogMonitor* IsecPrintLogMonitor::getInstance()
{
	if (_instance == NULL) {
		_instance = new IsecPrintLogMonitor();
	}
	return _instance;
}

bool IsecPrintLogMonitor::init(std::string cmd)
{  
	m_sCmd = cmd;
	if (m_sCmd.empty())
		return FALSE;
	m_sSavePath = "./temp.xml";
	m_bInitOk = TRUE;
	return TRUE;
}

void IsecPrintLogMonitor::saveLogToPath(std::string path)
{ 
	m_sSavePath = path;
	return;
}

bool IsecPrintLogMonitor::getLog()
{    
	if (m_bInitOk){
		bool bGetLogSucc = FALSE;
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		CString tmpCmd;
		tmpCmd.Format("%s %s", m_sCmd.c_str(), m_sSavePath.c_str());
		theLog.Write("IsecPrintLogMonitor::getLog, exec command [%s]", tmpCmd.GetString());

		if(!CreateProcess(0, tmpCmd.GetBuffer(), 0, 0, FALSE, 0, 0, 0, &si, &pi))
		{
			theLog.Write("!!IsecPrintLogMonitor::getLog,CreateProcess fail,error=%u,szCmdLine=%s", GetLastError(), m_sCmd.c_str());
			return FALSE;
		}

		theLog.Write("IsecPrintLogMonitor::getLog,CreateProcess succ,dwProcessId=%u", pi.dwProcessId);
		DWORD dwTimeoutSecond = 60*3;
		DWORD dwRet = WaitForSingleObject(pi.hProcess, dwTimeoutSecond*1000);
		if (dwRet == WAIT_TIMEOUT)
		{
			theLog.Write("!!IsecPrintLogMonitor::getLog,WaitForSingleObject WAIT_TIMEOUT 作业处理超时(%d秒)，强制结束打印作业,dwProcessId=%u"
				, dwTimeoutSecond, pi.dwProcessId);
			CCommonFun::TerminaPID(pi.dwProcessId);
		}
		else
		{
			theLog.Write("IsecPrintLogMonitor::getLog,WaitForSingleObject succ,dwProcessId=%u", pi.dwProcessId);
			DWORD dwExitCode = 0;	//STILL_ACTIVE,EXCEPTION_INT_DIVIDE_BY_ZERO
			GetExitCodeProcess(pi.hProcess, &dwExitCode);
			if (dwExitCode == 1)
			{
				bGetLogSucc = TRUE;
			}
			else
			{
				theLog.Write("!!IsecPrintLogMonitor::getLog,fail,dwExitCode=%u(0x%X),dwProcessId=%u"
					, dwExitCode, dwExitCode, pi.dwProcessId);
			}
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return bGetLogSucc;

	} else{
		theLog.Write("!!IsecPrintLogMonitor::getLog,fail,IsecPrintLogMonitor not inited!m_sCmd=[%s],m_sSavePath=[%s]"
			, m_sCmd.c_str(), m_sSavePath.c_str());
		return FALSE;
	}
}


void IsecPrintLogMonitor::getPrintJobList(SCP_JobInfo_List& jobList)
{

	CXmlParse xml;
	if (xml.ParseXml(CString(m_sSavePath.c_str())))
	{
		do 
		{
			MSXML2::IXMLDOMDocument2Ptr doc = xml.GetXmlDocment();
			MSXML2::IXMLDOMNodePtr Log = xml.SelectSingleNode(doc, "Log");
			MSXML2::IXMLDOMNodeListPtr PrintLogEntryList = xml.SelectNodes(Log, "LogRec");
			if (!PrintLogEntryList)
			{
				theLog.Write("##IsecPrintLogMonitor::getPrintJobList,PrintLogEntryList=%p", PrintLogEntryList);
				break;
			}
			long nCount = PrintLogEntryList->Getlength();
			theLog.Write("IsecPrintLogMonitor::getPrintJobList,nCount=%d", nCount);
			for (long index=0; index<nCount; index++)
			{
				CString szId, szJobType, szColorMode, szPaperSize, szSets, szOriginalPages, szTotalPages,
					szTotalPapers, szPrintedPages, szDocumentName;
				CString szStartTime, szFinishTime, szStatus, szStatusCode;
				MSXML2::IXMLDOMNodePtr LogRec = PrintLogEntryList->Getitem(index);
				MSXML2::IXMLDOMNodePtr JobInf = xml.SelectSingleNode(LogRec, "JobInf");			
				xml.GetNodeValue(JobInf, "No", szId);
				xml.GetNodeValue(JobInf, "Typ", szJobType);
				//只留下打印的日志
				if(atoi(szJobType) != 2)
					continue;
				xml.GetNodeValue(JobInf, "EntTim", szStartTime);
				xml.GetNodeValue(JobInf, "FinTim", szFinishTime);

				//xml.GetNodeValue(BasicInfo, "Status", szStatus);
				//xml.GetNodeValue(BasicInfo, "StatusCode", szStatusCode);


				MSXML2::IXMLDOMNodePtr PrtProc = xml.SelectSingleNode(JobInf, "PrtProc");
				MSXML2::IXMLDOMNodePtr PrtProcRX = xml.SelectSingleNode(JobInf, "PrtProcRX");
				MSXML2::IXMLDOMNodePtr JobResInf = xml.SelectSingleNode(JobInf, "JobResInf");
				xml.GetNodeValue(JobResInf, "Res", szStatus);
				
				

				MSXML2::IXMLDOMNodePtr ActInf = xml.SelectSingleNode(PrtProc, "ActInf");
				xml.GetNodeValue(ActInf, "CompSets", szSets);
				xml.GetNodeValue(ActInf, "InPg", szOriginalPages);
				xml.GetNodeValue(ActInf, "CompPg", szPrintedPages);

				MSXML2::IXMLDOMNodePtr PrPgInf = xml.SelectSingleNode(PrtProc, "PrPgInf");
				xml.GetNodeValue(PrPgInf, "ColMode", szColorMode);
				xml.GetNodeValue(PrPgInf, "Size", szPaperSize);
				xml.GetNodeValue(PrPgInf, "Pg", szTotalPages);
				

				MSXML2::IXMLDOMNodePtr Finishing = xml.SelectSingleNode(PrtProc, "Finishing");
				MSXML2::IXMLDOMNodePtr OutTray = xml.SelectSingleNode(Finishing, "OutTray");
				xml.GetNodeValue(OutTray, "Pg", szTotalPapers);
				
				MSXML2::IXMLDOMNodePtr PrtProcRX_ActInf = xml.SelectSingleNode(PrtProcRX, "ActInf");
				xml.GetNodeValue(PrtProcRX_ActInf, "FileNam", szDocumentName);
				
			
				theLog.Write("IsecPrintLogMonitor::GetPrintJobList,szId=%s,szJobType=%s,szColorMode=%s,"
					"szPaperSize=%s,szSets=%s,szOriginalPages=%s,szTotalPapers=%s,szPrintedPages=%s,szTotalPages=%s,"
					"szDocumentName=%s,szStartTime=%s,szFinishTime=%s,szStatus=%s,szStatusCode=%s"
					, szId, szJobType, szColorMode, szPaperSize, szSets, szOriginalPages, szTotalPapers
					, szPrintedPages, szTotalPages, szDocumentName, szStartTime, szFinishTime, szStatus, szStatusCode);
				

				SCP_JobInfo info;
				memset(&info,0x0,sizeof(SCP_JobInfo));
				info.nJobId = atoi(szId);
				info.nPrintedPage = atoi(szPrintedPages);
				info.nTotalPage = atoi(szTotalPages);
				info.nPage = atoi(szOriginalPages);
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
				//int nStatusCode = atoi(szStatusCode);
				info.JobStatus = SCP_JOB_STATUS_COMPLETED;
				
				jobList.push_back(info);
			}			
		} while (FALSE);
	}
	else
	{
		theLog.Write("!!IsecPrintLogMonitor::GetPrintJobList,ParseXml fail，m_sSavePath=[%s]",m_sSavePath.c_str());
	}

}
void IsecPrintLogMonitor::release()
{
	if(_instance)
		delete _instance;

}
