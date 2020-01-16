#include "StdAfx.h"
#include "SnmpJobMonitorSL.h"
#include "XeroxMIB_Def.h"

CSnmpJobMonitorSL::CSnmpJobMonitorSL(void)
{
	InitOID();
}

CSnmpJobMonitorSL::~CSnmpJobMonitorSL(void)
{
}

void CSnmpJobMonitorSL::InitOID()
{
	//施乐作业类型OID
	m_szXeroxJobTypeScanToFileOID = DecryptOID(XeroxJobTypeScanToFileOID);
	m_szXeroxJobTypeScanToPrintOID = DecryptOID(XeroxJobTypeScanToPrintOID);
	m_szXeroxJobTypeScanToFaxOID = DecryptOID(XeroxJobTypeScanToFaxOID);
	m_szXeroxJobTypeScanToMailListOID = DecryptOID(XeroxJobTypeScanToMailListOID);
	m_szXeroxJobTypeFaxToFileOID = DecryptOID(XeroxJobTypeFaxToFileOID);
	m_szXeroxJobTypeFaxToPrintOID = DecryptOID(XeroxJobTypeFaxToPrintOID);
	m_szXeroxJobTypeFaxToMailListOID = DecryptOID(XeroxJobTypeFaxToMailListOID);
	m_szXeroxJobTypePrintOID = DecryptOID(XeroxJobTypePrintOID);
	m_szXeroxJobTypeFileToFaxOID = DecryptOID(XeroxJobTypeFileToFaxOID);
	m_szXeroxJobTypeFileToMailListOID = DecryptOID(XeroxJobTypeFileToMailListOID);
	m_szXeroxJobTypeCopyOID = DecryptOID(XeroxJobTypeCopyOID);
	m_szXeroxJobTypeFileToFileOID = DecryptOID(XeroxJobTypeFileToFileOID);

	//施乐当前作业信息
	m_szXeroxCurentJobListBeginOID = DecryptOID(XeroxCurentJobListBeginOID);
	m_szXeroxCurentJobTypeOID = DecryptOID(XeroxCurentJobTypeOID);
	m_szXeroxCurentJobDocNameIndexOID = DecryptOID(XeroxCurentJobDocNameIndexOID);
	m_szXeroxCurentJobOwnerIndexOID = DecryptOID(XeroxCurentJobOwnerIndexOID);
	m_szXeroxCurentJobSenderIndexOID = DecryptOID(XeroxCurentJobSenderIndexOID);
	m_szXeroxCurentJobDocNameOID = DecryptOID(XeroxCurentJobDocNameOID);

	//施乐历史作业信息
	m_szXeroxHistoryJobListBeginOID = DecryptOID(XeroxHistoryJobListBeginOID);
	m_szXeroxHistoryDocNameFormat1OID = DecryptOID(XeroxHistoryDocNameFormat1OID);
	m_szXeroxHistoryDocNameFormat151OID = DecryptOID(XeroxHistoryDocNameFormat151OID);
	m_szXeroxHistoryDocNameFormatOID = "";	//初始化为空

	//施乐替换字符串
	m_szXeroxPrivateEnterprisesOID = DecryptOID(XeroxPrivateEnterprisesOID);
	m_szXeroxPrivateEnterprises2OID = DecryptOID(XeroxPrivateEnterprises2OID);
	m_szXeroxPrintStatusOID = DecryptOID(XeroxPrintStatusOID);
	m_szXeroxPrintStatus2OID = DecryptOID(XeroxPrintStatus2OID);
	m_szXeroxPrintedPagesOID = DecryptOID(XeroxPrintedPagesOID);
	m_szXeroxPrintedPages2OID = DecryptOID(XeroxPrintedPages2OID);
	m_szXeroxPrintTimeOID = DecryptOID(XeroxPrintTimeOID);
	m_szXeroxPrintTime2OID = DecryptOID(XeroxPrintTime2OID);
}

void CSnmpJobMonitorSL::CheckJob()
{
	if (!CheckIPActive())
	{
		return;
	}

	GetSLPrintJobList();
}

int CSnmpJobMonitorSL::CheckLastPrintCount()
{
	//CheckLastPrintCount_Color和CheckLastPrintCount_WB一定要先于CheckLastPrintCount调用
	CheckLastPrintCount_Color();
	CheckLastPrintCount_WB();
	m_nLastPrintCount = m_nLastPrintCount_Color + m_nLastPrintCount_WB;
	return m_nLastPrintCount;
}

int CSnmpJobMonitorSL::CheckLastPrintCount_Color()
{
	m_nLastPrintCount_Color = 0;
	return m_nLastPrintCount_Color;
}

int CSnmpJobMonitorSL::CheckLastPrintCount_WB()
{
	m_nLastPrintCount_WB = 0;
	return m_nLastPrintCount_WB;
}

int CSnmpJobMonitorSL::CheckLastCopyCount()
{
	//CheckLastCopyCount_Color和CheckLastCopyCount_WB一定要先于CheckLastCopyCount调用
	CheckLastCopyCount_Color();
	CheckLastCopyCount_WB();
	m_nLastCopyCount = m_nLastCopyCount_Color + m_nLastCopyCount_WB;
	return m_nLastCopyCount;
}

int CSnmpJobMonitorSL::CheckLastCopyCount_Color()
{
	m_nLastCopyCount_Color = 0;
	return m_nLastCopyCount_Color;
}

int CSnmpJobMonitorSL::CheckLastCopyCount_WB()
{
	m_nLastCopyCount_WB = 0;
	return m_nLastCopyCount_WB;
}

int CSnmpJobMonitorSL::CheckLastScanCount()
{
	//CheckLastScanCount_Color和CheckLastScanCount_WB一定要先于CheckLastScanCount调用
	CheckLastScanCount_Color();
	CheckLastScanCount_WB();
	m_nLastScanCount = m_nLastScanCount_Color + m_nLastScanCount_WB;
	return m_nLastScanCount;
}

int CSnmpJobMonitorSL::CheckLastScanCount_Color()
{
	m_nLastScanCount_Color = 0;
	return m_nLastScanCount_Color;
}

int CSnmpJobMonitorSL::CheckLastScanCount_WB()
{
	m_nLastScanCount_WB = 0;
	return m_nLastScanCount_WB;
}

int CSnmpJobMonitorSL::CheckLastFaxCount()
{
	CheckLastFaxCount_Color();
	CheckLastFaxCount_WB();
	m_nLastFaxCount = m_nLastFaxCount_Color + m_nLastFaxCount_WB;
	return m_nLastFaxCount;
}

int CSnmpJobMonitorSL::CheckLastFaxCount_Color()
{
	m_nLastFaxCount_Color = 0;
	return m_nLastFaxCount_Color;
}

int CSnmpJobMonitorSL::CheckLastFaxCount_WB()
{
	m_nLastFaxCount_WB = 0;
	return m_nLastFaxCount_WB;
}

int CSnmpJobMonitorSL::CheckLastPageCount_A3()
{
	m_nA3PageCount = 0;
	return m_nA3PageCount;
}

int CSnmpJobMonitorSL::CheckLastPageCount_A4()
{
	m_nA4PageCount = 0;
	return m_nA4PageCount;
}

BOOL CSnmpJobMonitorSL::GetCurrentJobIndex(CJobList& oCurrentJobList)	//获取当前打印队列信息
{
	//theLog.Write("CSnmpJobMonitorSL::GetCurrentJobIndex,1");
	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(CStringToChar(m_szXeroxCurentJobListBeginOID), &asnOid))	//施乐打印机当前作业列表起始oid=".1.3.6.1.4.1.253.8.59.6.1.1.2.101.8.48.48.48.48"
	{
		theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,2,err=%u", GetLastError());
		return FALSE;
	}

	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	

	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	while(TRUE) 
	{
		// initiates the GET request
		if(!SnmpMgrRequest(m_lpMgrSession,SNMP_PDU_GETNEXT,&snmpVarList,&errorStatus,&errorIndex))
		{	
			theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,4,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,5,err=%u,errorStatus=%d", GetLastError(), errorStatus);
			break;
		}
		if(SnmpUtilOidNCmp(&snmpVarList.list[0].name,&asnOid,asnOid.idLength))   
		{   
			//theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,6,err=%u", GetLastError());
			break;   
		}   

		char *cOidCurrent = NULL;
		if (SnmpMgrOidToStr(&snmpVarList.list[0].name,&cOidCurrent))
		{
			//.1.3.6.1.4.1.253.8.59.6.1.1.4.101.8.48.48.48.48.xx.xx.xx.xx	//作业类型的OID
			//.1.3.6.1.4.1.253.8.59.6.1.1.5.101.8.48.48.48.48.xx.xx.xx.xx	//文档名Index
			//.1.3.6.1.4.1.253.8.59.6.1.1.6.101.8.48.48.48.48.xx.xx.xx.xx	//作业用户名Index
			//.1.3.6.1.4.1.253.8.59.10.1.1.1.101.8.48.48.48.48.xx.xx.xx.xx	//作业发起者Index
			//.1.3.6.1.4.1.253.8.51.5.1.1.4.101.Index.3		//Index指示的字符串值

			int nActiveJobIndex = 0;
			CString szOidCurrent(cOidCurrent);
			CString szJobOid = szOidCurrent.Right(11);
			CString szOidJobType;
			szOidJobType.Format("%s.%s", m_szXeroxCurentJobTypeOID, szJobOid);	//.1.3.6.1.4.1.253.8.59.6.1.1.4.101.8.48.48.48.48
			unsigned char cJobTypeOid[128] = {0};
			if (!GetRequest(CStringToChar(szOidJobType), cJobTypeOid, sizeof(cJobTypeOid)))
			{
				theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,7,err=%u", GetLastError());
				SnmpUtilMemFree(cOidCurrent);
				break;
			}

			//获取并判断作业类型,只选择打印作业
			CString szJobTypeOid(cJobTypeOid);
			szJobTypeOid.Trim(".");
			szJobTypeOid = "." + szJobTypeOid;
			if (szJobTypeOid.CompareNoCase(m_szXeroxJobTypePrintOID) == 0)
			{
				//获取作业的文档名Index,并将此Index作为作业ID
				CString szOidJobNameIndex;
				szOidJobNameIndex.Format("%s.%s", m_szXeroxCurentJobDocNameIndexOID, szJobOid);	//.1.3.6.1.4.1.253.8.59.6.1.1.5.101.8.48.48.48.48
				if (!GetRequest(CStringToChar(szOidJobNameIndex), nActiveJobIndex))
				{
					theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,8,err=%u", GetLastError());
					SnmpUtilMemFree(cOidCurrent);
					break;
				}

				PSCP_JobInfo pJobInfo = new SCP_JobInfo;
				memset(pJobInfo, 0x0, sizeof(SCP_JobInfo));
				pJobInfo->nJobId = nActiveJobIndex;
				strcpy_s(pJobInfo->cDesc, sizeof(pJobInfo->cDesc), szJobOid.GetString());
				pJobInfo->JobType = SCP_JOB_TYPE_PRINT; 
				pJobInfo->JobStatus = SCP_JOB_STATUS_START;
				GetJobInfo(pJobInfo);
				oCurrentJobList.AddTail(pJobInfo);
			}
			else
			{
				theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,9");
			}
		}
		else
		{
			theLog.Write("!!CSnmpJobMonitorSL::GetCurrentJobIndex,10,err=%u,cOidCurrent=%p", GetLastError(), cOidCurrent);
		}
		SnmpUtilMemFree(cOidCurrent);
	}

	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);

	//theLog.Write("CSnmpJobMonitorSL::GetCurrentJobIndex,11,count=%d", oCurrentJobList.GetCount());
	return TRUE;
}

int CSnmpJobMonitorSL::GetJobIndexOld(CString& szJobOid)
{
	theLog.Write("CSnmpJobMonitorSL::GetJobIndexOld,1");
	int nActiveJobIndexOld = 0;

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(CStringToChar(m_szXeroxCurentJobListBeginOID), &asnOid))	//施乐打印机当前作业列表起始oid=".1.3.6.1.4.1.253.8.59.6.1.1.2.101.8.48.48.48.48"
	{
		theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,2,err=%u", GetLastError());
		return nActiveJobIndexOld;
	}

	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	

	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	while(TRUE) 
	{
		// initiates the GET request
		if(!SnmpMgrRequest(m_lpMgrSession,SNMP_PDU_GETNEXT,&snmpVarList,&errorStatus,&errorIndex))
		{	
			theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,4,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,5,err=%u,errorStatus=%d", GetLastError(), errorStatus);
			break;
		}
		if(SnmpUtilOidNCmp(&snmpVarList.list[0].name,&asnOid,asnOid.idLength))   
		{   
			theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,6,err=%u", GetLastError());
			break;   
		}   

		char *cOidCurrent = NULL;
		if (SnmpMgrOidToStr(&snmpVarList.list[0].name,&cOidCurrent))
		{
			//.1.3.6.1.4.1.253.8.59.6.1.1.4.101.8.48.48.48.48.xx.xx.xx.xx	//作业类型的OID
			//.1.3.6.1.4.1.253.8.59.6.1.1.5.101.8.48.48.48.48.xx.xx.xx.xx	//文档名Index
			//.1.3.6.1.4.1.253.8.59.6.1.1.6.101.8.48.48.48.48.xx.xx.xx.xx	//作业用户名Index
			//.1.3.6.1.4.1.253.8.59.10.1.1.1.101.8.48.48.48.48.xx.xx.xx.xx	//作业发起者Index
			//.1.3.6.1.4.1.253.8.51.5.1.1.4.101.Index.3		//Index指示的字符串值

			CString szOidCurrent(cOidCurrent);
			szJobOid = szOidCurrent.Right(11);
			CString szOidJobType;
			szOidJobType.Format("%s.%s", m_szXeroxCurentJobTypeOID, szJobOid);	//.1.3.6.1.4.1.253.8.59.6.1.1.4.101.8.48.48.48.48
			unsigned char cJobTypeOid[128] = {0};
			if (!GetRequest(CStringToChar(szOidJobType), cJobTypeOid, sizeof(cJobTypeOid)))
			{
				theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,7,err=%u", GetLastError());
				SnmpUtilMemFree(cOidCurrent);
				break;
			}

			//获取并判断作业类型,只选择打印作业
			CString szJobTypeOid(cJobTypeOid);
			szJobTypeOid.Trim(".");
			szJobTypeOid = "." + szJobTypeOid;
			if (szJobTypeOid.CompareNoCase(m_szXeroxJobTypePrintOID) == 0)
			{
				//获取作业的文档名Index,并将此Index作为作业ID
				CString szOidJobNameIndex;
				szOidJobNameIndex.Format("%s.%s", m_szXeroxCurentJobDocNameIndexOID, szJobOid);	//.1.3.6.1.4.1.253.8.59.6.1.1.5.101.8.48.48.48.48
				if (!GetRequest(CStringToChar(szOidJobNameIndex), nActiveJobIndexOld))
				{
					theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,8,err=%u", GetLastError());
					SnmpUtilMemFree(cOidCurrent);
					break;
				}
			}
			else
			{
				theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,9");
			}
		}
		else
		{
			theLog.Write("!!CSnmpJobMonitorSL::GetJobIndexOld,10,err=%u,cOidCurrent=%p", GetLastError(), cOidCurrent);
		}
		SnmpUtilMemFree(cOidCurrent);
	}

	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);

	theLog.Write("CSnmpJobMonitorSL::GetJobIndexOld,11,nActiveJobIndexOld=%d", nActiveJobIndexOld);
	return nActiveJobIndexOld;
}

bool CSnmpJobMonitorSL::GetJobInfo(PSCP_JobInfo pJobInfo)
{
	bool bRet = true;
	if (!pJobInfo)
	{
		theLog.Write("!!CSnmpJobMonitorSL::GetJobInfo,1,pJobInfo=%p", pJobInfo);
		return bRet;
	}

	//.1.3.6.1.4.1.253.8.59.6.1.1.4.101.8.48.48.48.48.xx.xx.xx.xx	//作业类型的OID
	//.1.3.6.1.4.1.253.8.59.6.1.1.5.101.8.48.48.48.48.xx.xx.xx.xx	//文档名Index
	//.1.3.6.1.4.1.253.8.59.6.1.1.6.101.8.48.48.48.48.xx.xx.xx.xx	//作业用户名Index
	//.1.3.6.1.4.1.253.8.59.10.1.1.1.101.8.48.48.48.48.xx.xx.xx.xx	//作业发起者Index
	//.1.3.6.1.4.1.253.8.51.5.1.1.4.101.Index.3		//Index指示的字符串值

	do 
	{
		//获取作业的文档名
		CString szOidJobName;
		szOidJobName.Format("%s.%d.3", m_szXeroxCurentJobDocNameOID, pJobInfo->nJobId);	//.1.3.6.1.4.1.253.8.51.5.1.1.4.101
		unsigned char cJobName[MAX_PATH] = {0};
		if (GetRequest(CStringToChar(szOidJobName), cJobName, sizeof(cJobName)))
		{
			strcpy_s(pJobInfo->cJobName, sizeof(pJobInfo->cJobName), (char*)cJobName);
		}
		else
		{
			theLog.Write("!!CSnmpJobMonitorSL::GetJobInfo,2,err=%u", GetLastError());
			//break;
		}
	} while (false);

	return bRet;
}

//只针对施乐打印机,获取已经打印列表
void CSnmpJobMonitorSL::GetSLPrintJobList()
{
	SCP_JobInfo_List PrintedJobList;

	BOOL bRet = FALSE;
	char *asciiStr = NULL;


	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(CStringToChar(m_szXeroxHistoryJobListBeginOID), &asnOid))	//施乐打印机打印列表起始oid=".1.3.6.1.4.1.253.8.59.6.1.1.5"
	{
		return;
	}

	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	

	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	while(TRUE) 
	{
		// initiates the GET request
		if(!SnmpMgrRequest(m_lpMgrSession,SNMP_PDU_GETNEXT,&snmpVarList,&errorStatus,&errorIndex))
		{			
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		if(SnmpUtilOidNCmp(&snmpVarList.list[0].name,&asnOid,asnOid.idLength))
		{   
			break;   
		}   

		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);

		//theLog.Write("%s",asciiStr);
		if (asciiStr)
		{
			SCP_JobInfo info;
			memset(&info,0x0,sizeof(SCP_JobInfo));
			info.nJobId = atoi(asciiStr);
			char *szOidCurrent = NULL;
			if (SnmpMgrOidToStr(&snmpVarList.list[0].name,&szOidCurrent))
			{
				//CString szOidPrivate(_T(".iso.org.dod.internet.private.enterprises."));	//.1.3.6.1.4.1.
				//CString szOidPrivateS(_T(".1.3.6.1.4.1."));
				CString szOidCurrentTmp(szOidCurrent);
				szOidCurrentTmp.Replace(m_szXeroxPrivateEnterprisesOID, m_szXeroxPrivateEnterprises2OID);
				strcpy(info.szOid, szOidCurrentTmp);
				//theLog.Write("==szOidCurrentTmp=[%s]",info.szOid);
				PrintedJobList.push_back(info);
			}
			else
			{
				//
			}
			SnmpUtilMemFree(szOidCurrent);
			SnmpUtilMemFree(asciiStr);
		}
	}

	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	

	//开始获取打印页数和打印文档名称
	//	标准启始oid
	//	OID=.1.3.6.1.4.1.253.8.59.6.1.1.5.151.8.48.48.48.48.52.54.53.68, Type=Integer, Value=268453469

	//	文档打印页数[1]
	//  OID=.1.3.6.1.4.1.253.8.59.16.1.1.9.151.8.48.48.48.48.52.54.53.68, Type=Counter32, Value=1	

	//	打印文档时间[2013 09 29 15-12-32-00]
	//	OID=.1.3.6.1.4.1.253.8.59.10.1.1.10.151.8.48.48.48.48.52.54.53.68, Type=OctetString, Value=  07 DD 09 1D 0F 0C 20 00 

	//  打印文档名称[test_zxl.txt - 记事本]
	// 	OID=.1.3.6.1.4.1.253.8.51.5.1.1.4.151.268453469.3, Type=OctetString, Value=  74 65 73 74 5F 7A 78 6C 2E 74 78 74 20 2D 20 BC      C7 CA C2 B1 BE                                     


	//	暂时求和数据
	// 	OID=.1.3.6.1.4.1.253.8.51.5.1.1.3.151.268453469.3, Type=Integer, Value=1

	theLog.Write("Job Count = %d", PrintedJobList.size());
	SCP_JobInfo_List_I iter;
	for (iter=PrintedJobList.begin(); iter!=PrintedJobList.end();iter++)
	{
		if (m_szXeroxHistoryDocNameFormatOID.GetLength() <= 0)
		{
			//目前发现XeroxHistoryDocNameFormat有以下2种格式
			//".1.3.6.1.4.1.253.8.51.5.1.1.4.1.Index.106"
			//".1.3.6.1.4.1.253.8.51.5.1.1.4.151.Index.3"
			//XeroxHistoryJobListBeginOID与有以下2种格式与它对应
			//".1.3.6.1.4.1.253.8.59.6.1.1.5.1.8.48.48.48.48.48.50.53.54"
			//".1.3.6.1.4.1.253.8.59.6.1.1.5.151.8.48.48.48.48.54.65.67.54"
			CString szOid(iter->szOid);
			CString szTemp = szOid.Mid(m_szXeroxHistoryJobListBeginOID.GetLength()+1, 3);
			if (szTemp.Compare("151") == 0)
			{
				m_szXeroxHistoryDocNameFormatOID = m_szXeroxHistoryDocNameFormat151OID;
			}
			else
			{
				m_szXeroxHistoryDocNameFormatOID = m_szXeroxHistoryDocNameFormat1OID;
			}
		}
		//作业状态
		CString szOidJobStatus(iter->szOid);
		//szOidJobStatus.Replace(_T("253.8.59.6.1.1.5"),_T("253.8.59.6.1.1.9"));
		szOidJobStatus.Replace(m_szXeroxPrintStatusOID, m_szXeroxPrintStatus2OID);
		//theLog.Write("%s",szOidJobStatus);
		int nJobStatus = 0;
		GetRequest(szOidJobStatus.GetBuffer(), nJobStatus);
		szOidJobStatus.ReleaseBuffer();
		iter->JobStatus = (nJobStatus == XeroxJobStatusFinished) ? SCP_JOB_STATUS_COMPLETED : SCP_JOB_STATUS_START;
		if (iter->JobStatus != SCP_JOB_STATUS_COMPLETED)
		{
			//作业未结束，跳过
			continue;
		}

		//文档打印页数
		CString szOidPrintedPages(iter->szOid);
		//szOidPrintedPages.Replace(_T("253.8.59.6.1.1.5"),_T("253.8.59.16.1.1.5"));
		szOidPrintedPages.Replace(m_szXeroxPrintedPagesOID, m_szXeroxPrintedPages2OID);
		//theLog.Write("%s",szOidPrintedPages);
		GetRequest(CStringToChar(szOidPrintedPages), iter->nPrintedPage);

		/*
		//打印文档时间
		CString szOidPrintTime(iter->szOid);
		//szOidPrintTime.Replace(_T("253.8.59.6.1.1.5"),_T("253.8.59.10.1.1.10"));
		szOidPrintTime.Replace(m_szXeroxPrintTimeOID, m_szXeroxPrintTime2OID);
		//theLog.Write("%s",szOidPrintTime);
		GetRequestStr(CStringToChar(szOidPrintTime), iter->szPrintTime, sizeof(iter->szPrintTime));
		int nYear = iter->szPrintTime[0];
		int nYear2 = (unsigned char)iter->szPrintTime[1];
		nYear = (nYear<<8) + nYear2;
		int nMonth = iter->szPrintTime[2];
		int nDay = iter->szPrintTime[3];
		int nHour = iter->szPrintTime[4];
		int nMin = iter->szPrintTime[5];
		int nSec = iter->szPrintTime[6];
		COleDateTime printtime(nYear, nMonth, nDay, nHour, nMin, nSec);
		iter->fJobEndTime = printtime.m_dt;
		sprintf(iter->szPrintTime, "%04d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMin, nSec);
		*/

		//打印文档名称
		CString szOidDocName;
		szOidDocName.Format(m_szXeroxHistoryDocNameFormatOID, iter->nJobId);
		//theLog.Write("%s",szOidDocName);
		GetRequest(CStringToChar(szOidDocName),(unsigned char*)iter->cJobName,sizeof(iter->cJobName));

		//输出日志
		//theLog.Write("nIdentifier=[%u],nDocPrintedPages=[%d],szDocName=[%s],time=[%s]"
		//	,iter->nJobId, iter->nPrintedPage, iter->cJobName, iter->szPrintTime);
	}
	if (PrintedJobList.size() > 0 && m_pCallBack)
	{
		m_pCallBack->OnJobInfoList(&PrintedJobList);
	}
}
