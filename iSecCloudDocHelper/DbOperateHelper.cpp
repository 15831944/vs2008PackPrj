#include "StdAfx.h"
#include "DbOperateHelper.h"

CDbOperateHelper::CDbOperateHelper(void)
{
	m_pAdo = CAdoFactory::CreateAdoEx();
}

CDbOperateHelper::~CDbOperateHelper(void)
{
}

BOOL CDbOperateHelper::InitDataBase()
{
	theLog.Write("CDbOperateHelper::InitDataBase");
	//´òÓ¡Êý¾Ý¿âÅäÖÃ
	int nPrintDbType = theIniFile.GetVal("DbConfig","DbType", MYSQL);
	CString szPrintDbIP = theIniFile.GetVal("DbConfig","DbSvr", "192.168.4.240");
	CString szPrintDbPort = theIniFile.GetVal("DbConfig","DbPort", "3306");
	CString szPrintDbName = theIniFile.GetVal("DbConfig","DbName", "iSecCloud");
	CString szPrintDbUser = theIniFile.GetVal("DbConfig","DbUser", "root");
	CString szPrintDbPwd = theIniFile.GetVal("DbConfig","DbPwd", "");
#ifdef ENABLE_PWD_CRY
	char szPwd[MAX_PATH] = {0};
	int nSize = MAX_PATH;
	CPWDCry cry;
	cry.UnCryptPWD(szPrintDbPwd.GetBuffer(),szPwd,nSize);
	szPrintDbPwd.ReleaseBuffer();
	szPrintDbPwd = szPwd;
#endif

	ADO_INFO ado;
	strcpy(ado.szIP, szPrintDbIP);
	strcpy(ado.szPort, szPrintDbPort);
	strcpy(ado.szDbName, szPrintDbName);
	strcpy(ado.szUser, szPrintDbUser);
	strcpy(ado.szPwd, szPrintDbPwd);

	CAdoFactory::SetDBType(nPrintDbType);
	CAdo* pAdo = CAdoFactory::CreateAdoEx(&ado);
	if (!pAdo)
	{
		theLog.Write("!!CDbOperateHelper::InitDataBase, CreateAdo fail");
		return FALSE;
	}
	else
	{
		CAdoFactory::Release(pAdo);
		return TRUE;
	}
}

void CDbOperateHelper::ReleaseDataBase()
{
	theLog.Write("CDbOperateHelper::ReleaseDataBase");
	CAdoFactory::Release();
}

BOOL CDbOperateHelper::IsNeedDealCloudJob()
{
	return TRUE;
}

BOOL CDbOperateHelper::GetNeedDealCloudJob(CListCloudJobInfo& list)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(m_pAdo);
	if (!m_pAdo)
	{
		theLog.Write("!!CDbOperateHelper::GetNeedDealCloudJob,m_pAdo=%p", m_pAdo);
		return FALSE;
	}

	CStringArray ary;
	long lRow = 0;
	long lCol = 0;
	CString szSql;
	szSql.Format(" SELECT  "
		" id, "				//0
		" entid, "			//1
		" equip_id, "		//2
		" userid, "			//3
		" submit_time, "	//4
		" doc_name, "		//5
		" doc_size, "		//6
		" org_job_path, "	//7
		" org_job_size, "	//8
		" org_job_is_gz, "	//9
		" dir_path, "		//10
		" page_type, "		//11
		" page_size, "		//12
		" paper_source, "	//13
		" orientation, "	//14
		" copies, "			//15
		" color, "			//16
		" double_print, "	//17
		" total_page, "		//18
		" state, "			//19
		" file_state, "		//20
		" file_deleted "	//21
		" FROM t_log_submitjob WHERE state=%d order by id ", JOB_CLOUD_DEAL_WAIT);

	if (m_pAdo->GetRecordset(szSql, ary, lRow, lCol) && lRow>0)
	{
		for (long l=0; l<lRow; l++)
		{
			long lIndex = l * lCol;
			PCloudJobInfo job = new CloudJobInfo;
			memset(job, 0x0, sizeof(CloudJobInfo));
			job->nJobId = atoi(ary.GetAt(lIndex + 0));
			job->nEntId = atoi(ary.GetAt(lIndex + 1));
			job->nEquipId = atoi(ary.GetAt(lIndex + 2));
			job->nUserId = atoi(ary.GetAt(lIndex + 3));
			COleDateTime time;
			time.ParseDateTime(ary.GetAt(lIndex + 4));
			job->fSubmitTime = time.m_dt;
			strcpy(job->cDocName, ary.GetAt(lIndex + 5));
			job->nDocSize = atoi(ary.GetAt(lIndex + 6));
			strcpy(job->cOrgJobName, ary.GetAt(lIndex + 7));
			job->nOrgJobSize = atoi(ary.GetAt(lIndex + 8));
			job->nOrgJobIsGz = atoi(ary.GetAt(lIndex + 9));
			strcpy(job->cJobDir, ary.GetAt(lIndex + 10));
			strcpy(job->cPageType, ary.GetAt(lIndex + 11));
			job->nPaperSize = atoi(ary.GetAt(lIndex + 12));
			job->nPaperSource = atoi(ary.GetAt(lIndex + 13));
			job->nOrientation = atoi(ary.GetAt(lIndex + 14));
			job->nCopies = atoi(ary.GetAt(lIndex + 15));
			job->nColor = atoi(ary.GetAt(lIndex + 16));
			job->nDoublePrint = atoi(ary.GetAt(lIndex + 17));
			job->nTotalPage = atoi(ary.GetAt(lIndex + 18));
			job->nJobState = atoi(ary.GetAt(lIndex + 19));
			job->nFileState = atoi(ary.GetAt(lIndex + 20));
			job->nFileDeleted = atoi(ary.GetAt(lIndex + 21));

			list.AddTail(job);
		}

		return (list.GetCount()>0 ? TRUE : FALSE);
	}
	else
	{
		return FALSE;
	}
}

BOOL CDbOperateHelper::UpdateCloudJob(PCloudJobInfo job)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(m_pAdo);
	if (!m_pAdo || !job)
	{
		theLog.Write("!!CDbOperateHelper::UpdateCloudJob,m_pAdo=%p,job=%p", m_pAdo, job);
		return FALSE;
	}

	CString szJobDir(job->cJobDir);
	szJobDir.Replace(Cloud_Doc_Path_Slash_Windows, Cloud_Doc_Path_Slash_Windows_Double);
	CString szSql;
	szSql.Format(
		" UPDATE t_log_submitjob SET "
		" total_page=%d, "		//1
		" state=%d, "			//2
		" new_job_path='%s', "	//3
		" new_job_size=%d, "	//4
		" new_job_is_gz=%d "	//5
		" WHERE id=%d "			//6
		, job->nTotalPage
		, job->nJobState
		, job->cNewJobName
		, job->nNewJobSize
		, job->nNewJobIsGz
		, job->nJobId);
	BOOL bRet = m_pAdo->ExecuteSQL(szSql);
	if (!bRet)
	{
		theLog.Write("!!CDbOperateHelper::UpdateCloudJob,fail,szSql=%s", szSql);
	}
	return bRet;
}

BOOL CDbOperateHelper::UpdateCloudJobState(PCloudJobInfo job, int nState)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(m_pAdo);
	if (!m_pAdo || !job)
	{
		theLog.Write("!!CDbOperateHelper::UpdateCloudJobState,m_pAdo=%p,job=%p", m_pAdo, job);
		return FALSE;
	}

	CString szSql;
	szSql.Format(" UPDATE t_log_submitjob SET state=%d WHERE id=%d ;", nState, job->nJobId);
	BOOL bRet = m_pAdo->ExecuteSQL(szSql);
	if (!bRet)
	{
		theLog.Write("!!CDbOperateHelper::UpdateCloudJobState,fail,szSql=%s", szSql);
	}
	return bRet;
}

BOOL CDbOperateHelper::GetSystemSetting(CString& sName,CStringArray& ary)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(m_pAdo);
	if (!m_pAdo)
	{
		theLog.Write("!!CDbOperateHelper::UpdateCloudJob,m_pAdo=%p", m_pAdo);
		return FALSE;
	}
	CString szSql;
	szSql.Format(_T(" SELECT setname, setvalue1, setvalue2, setvalue3, setvalue4, setvalue5, setvalue6, "
		"setvalue7, setvalue8, comment, state FROM t_s_system WHERE setname='%s' "), sName);
	return m_pAdo->GetAry(szSql,ary);
}


CString CDbOperateHelper::GetTimeStr(double fTime)
{
	COleDateTime time;
	time.m_dt = fTime;
	return time.Format("%Y-%m-%d %H:%M:%S");
}

BOOL CDbOperateHelper::CheckDbConn()
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(m_pAdo);
	if (!m_pAdo)
	{
		theLog.Write("!!CDbOperateHelper::CheckDbConn,m_pAdo=%p", m_pAdo);
		return FALSE;
	}
	CString szSql;
	szSql.Format(_T(" SELECT NOW() "));

	BOOL bRet = m_pAdo->ExecuteSQL(szSql);
	if (!bRet)
	{
		theLog.Write("!!CDbOperateHelper::CheckDbConn,2,fail,m_pAdo=%p", m_pAdo);
	}
	return bRet;
}

BOOL CDbOperateHelper::ReConnDb()
{
	CCriticalSection2::Owner ow(m_cs);

	if (!m_pAdo)
	{
		CAdoFactory::Release(m_pAdo);
	}

	m_pAdo = CAdoFactory::CreateAdoEx();

	return (m_pAdo != NULL);
}

