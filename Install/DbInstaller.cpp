// DbInstaller.cpp: implementation of the CDbInstaller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DbInstaller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbInstaller::CDbInstaller(BOOL bShowErr)
{
	m_bShowErr = bShowErr;
}

CDbInstaller::~CDbInstaller()
{

}

BOOL CDbInstaller::BackupDb(CString sDbName,CString sDbPath)
{
	ASSERT(!sDbName.IsEmpty());
	ASSERT(!sDbPath.IsEmpty());

	CString sSQL;

	_variant_t vt;
	vt.Clear();

	sSQL.Format("select filename from master.dbo.sysdatabases where name = '%s'",sDbName);
	if(!m_ado.GetOneData(sSQL,vt))
	{
		//	AfxMessageBox("表sysdatabases不存在刚创建的数据库!");
		return FALSE;
	}
	sSQL.Format(" EXEC sp_addumpdevice 'disk', 'XabDbBackDisk', '%s' "
				" BACKUP DATABASE %s TO XabDbBackDisk with format "
				" exec sp_dropdevice 'XabDbBackDisk' ",sDbPath,sDbName);
	return m_ado.ExecuteSQL(sSQL);
}

BOOL CDbInstaller::RestoreDB(CString sDbName,CString sDbPath)
{
	CString sSQL;

	_variant_t vt;
	vt.Clear();

	sSQL.Format("select filename from master.dbo.sysdatabases where name = '%s'",sDbName);
	if(!m_ado.GetOneData(sSQL,vt))
	{
	//	AfxMessageBox("表sysdatabases不存在刚创建的数据库!");
		theLog.WriteEx("!!CDbInstaller::RestoreDB,3,GetOneData fail,err=%d,sql=%s", GetLastError(), sSQL);
		return FALSE;
	}
	
	CString sInstallPath = CCommonFun::GetVariantStr(vt);

	if(sInstallPath.IsEmpty())
	{
		//AfxMessageBox("读取数据库路径失败!");
		theLog.WriteEx("!!CDbInstaller::RestoreDB,4,sInstallPath=%s", sInstallPath);
		return FALSE;
	}
	else
	{
		int nFind = sInstallPath.ReverseFind('\\');
		if(nFind >= 0)
			sInstallPath = sInstallPath.Left(nFind);
	}

	sSQL.Format("RESTORE FILELISTONLY from disk = '%s'",sDbPath);

    if(0 == sDbName.CompareNoCase("iSec_PrinterApp"))
    {//App数据库
        sSQL.Format(" Restore Database %s from disk = '%s' "
            " WITH REPLACE,MOVE '%s' TO '%s\\%s_Data.mdf',"
            " MOVE '%s_log' TO '%s\\%s_Log.ldf'"
            , sDbName, sDbPath
            , sDbName, sInstallPath, sDbName
            , sDbName, sInstallPath, sDbName);    
    }
    else
    {
    	sSQL.Format(" Restore Database %s from disk = '%s' "
    		" WITH REPLACE,MOVE '%s_Data' TO '%s\\%s.mdf',"
    		" MOVE '%s_log' TO '%s\\%s.ldf'",
    		sDbName,sDbPath,
    		sDbName,sInstallPath,sDbName,
    		sDbName,sInstallPath,sDbName);
    }
	BOOL BRet = m_ado.ExecuteSQL(sSQL);
	if(!BRet)
	{
		theLog.WriteEx("!!CDbInstaller::RestoreDB,5,sInstallPath=%s", sInstallPath);
	}
	return BRet;
}

BOOL CDbInstaller::CheckVersion(int& nMajor)
{
	CString sSQL;
	sSQL.Format("select Level = serverproperty('ProductVersion')");

	_variant_t vt;
	vt.Clear();
	BOOL bRet = m_ado.GetOneData(sSQL,vt);
	if(!bRet)
	{
		theLog.Write("读取数据库系统表失败!");
		return FALSE;
	}
	CString sVal = CCommonFun::GetVariantStr(vt);
	CStringArray ary;
	CCommonFun::StringSplit(sVal,&ary,'.');
	if (ary.GetCount() > 0)
	{
		nMajor = atoi(ary.GetAt(0));
	}
	
	return TRUE;
}

BOOL CDbInstaller::CheckSP4()
{
	CString sSQL;
	sSQL.Format("select Level = serverproperty('productlevel')");
	
	_variant_t vt;
	vt.Clear();
	BOOL bRet = m_ado.GetOneData(sSQL,vt);
	if(!bRet)
	{
		theLog.Write("读取数据库系统表失败!");
		return FALSE;
	}
	CString sVal = CCommonFun::GetVariantStr(vt);
	if(sVal.CompareNoCase("SP4") != 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDbInstaller::CheckDbExist(CString sDbName)
{
	CString sSQL;
	_variant_t vt;
	vt.Clear();
	sSQL.Format("SELECT name FROM master.dbo.sysdatabases WHERE name = N'%s'",sDbName);
	BOOL bRet = m_ado.GetOneData(sSQL,vt);
	return bRet;
}

BOOL CDbInstaller::StartSqlAgent()
{
	CString sSQL = "exec xp_cmdshell 'net start sqlserveragent'";
	return m_ado.ExecuteSQL(sSQL);
}

BOOL CDbInstaller::ExecuteSqlTxt(CString sIP,CString sDbName,CString sUser, CString sPsw, CString sPort, CString sPath)
{
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	PROCESS_INFORMATION pi;
	memset(&pi,0,sizeof(pi));

	//------------------------
	//设置参数
	char szShortPath[MAX_PATH + 1] = {0};
	CString sParam;
	GetShortPathName(sPath,szShortPath,MAX_PATH + 1);
	if(sPort == "1433")
		sParam.Format(" -S%s -l 300 -d%s -U%s -P%s -i%s",sIP,sDbName,sUser,sPsw,szShortPath);
	else
		sParam.Format(" -S%s,%s -l 300 -d%s -U%s -P%s -i%s",sIP,sPort,sDbName,sUser,sPsw,szShortPath);
	
	//获取执行路径
	CString sExePath;
	sExePath.Format("%sosql.exe",CCommonFun::GetDefaultPath());
	char szSqlPath[MAX_PATH + 1] = {0};
	GetShortPathName(sExePath,szSqlPath,MAX_PATH + 1);
	
	//
	BOOL bRet = CreateProcess(szSqlPath,sParam.GetBuffer(0),NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
	if(bRet == 0)
	{
		return FALSE;
	}
	HANDLE handle = pi.hProcess;
	DWORD dwRet = ::WaitForSingleObject(handle,1000*200);

	if(dwRet == WAIT_OBJECT_0 )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDbInstaller::CreateDb(CString sDbName)
{
	CString sSQL;
	sSQL.Format("create database %s ",sDbName);
	return m_ado.ExecuteSQL(sSQL);
}
BOOL CDbInstaller::UnIstallDb(CString sDbSvr,CString sDbName,CString sUser, CString sPsw, CString sPort)
{
	BOOL bRet = FALSE;
	m_ado.SetConnStr(sDbSvr,"master",sUser,sPsw,sPort);
	if (m_ado.Connect())
	{
		bRet = DropDb(sDbName);
	}
	else
	{
		theLog.Write("UnIstallDb connect faile");
		ASSERT(0);
	}
	return bRet;
}
BOOL CDbInstaller::DropDb(CString sDbName,BOOL bDrop /* = TRUE*/)
{
	CString sSQL;
	_variant_t varspid;
	BOOL bRet = FALSE;
	sSQL.Format(" select spid from sysprocesses where dbid=db_id('%s') ",sDbName);
	_RecordsetPtr pRs = NULL;
	_bstr_t bsKillSql;
	try
	{
		bRet = m_ado.GetRecordset(sSQL,&pRs);
		if(!bRet || pRs == NULL)
			return FALSE;

		while(!pRs->EndOfFile)
		{
			_variant_t varspid;
			varspid = pRs->Fields->GetItem(_variant_t("spid"))->Value;
			
			bsKillSql = "kill  " + _bstr_t(varspid);
			m_ado.ExecuteSQL(bsKillSql);
			pRs->MoveNext();

		}

		if(bDrop)
		{
			sSQL.Format("DROP Database %s",sDbName);
			bRet = m_ado.ExecuteSQL(sSQL);
		}
		else
		{
			bRet = TRUE;
		}
	}
	catch (...)
	{
	}
	return bRet;
}


BOOL CDbInstaller::InstallDbByDbbackupFile(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,
										   CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType)
{
	m_ado.SetConnStr(sDbSvr,"master",sDbUser,sDbPwd,sPort);
	if(!m_ado.Connect())
	{
		theLog.Write("连接数据库失败！");
		m_sErr = _T("连接数据库失败！");
		return FALSE;
	}
	int nMajor = 0;
	if(!CheckVersion(nMajor))
	{
		theLog.Write("检测数据库版本失败！");
		m_sErr = _T("检测数据库版本失败！");
		return FALSE;
	}
	//mssql 2000
	if(nMajor == 8)
	{
		//检测sp4
		if(!CheckSP4())
		{
			theLog.Write("数据库没有打SP4补丁！");
			m_sErr = _T("数据库没有打SP4补丁！");
			return FALSE;
		}
	}
	//sql2005 以上
	else if (nMajor >= 9)
	{

	}
	else
	{
		ASSERT(0);
	}

	//启动代理
	StartSqlAgent();

	//检测数据库还存在
 	if(CheckDbExist(sDbName))
 	{
		m_sErr = _T("要创建的数据库已经存在，请先备份后，再删除此数据库！");
 		//AfxMessageBox("要创建的数据库已经存在，请先备份后，再删除此数据库！");
		
		if (eType == OIT_QUERY)
		{
			CString sMsg;
			sMsg.Format(_T("系统检测到数据库服务器上已经存在该产品的数据库。\n"
				"建议您先备份数据库，然后再做进一步操作。是否继续？\n\n"
				"如果选择“是”，系统将自动将数据备份到“C:\\”目录，然后重新安装数据库，原有数据将被删除！\n"
				"如果选择“否”，系统将继续使用现有数据库，原有数据将保留。"));

			int nId = ::MessageBox(0,sMsg,"提示",MB_ICONQUESTION |MB_YESNO);
			if (nId == IDYES)
			{
				CString sBackPath;
				COleDateTime dt = COleDateTime::GetCurrentTime();
				sBackPath.Format(_T("%s%s_%d%d%d%d%d%d.bak"),"c:\\",sDbName,
					dt.GetYear(),
					dt.GetMonth(),
					dt.GetDay(),
					dt.GetHour(),
					dt.GetMinute(),
					dt.GetSecond());
				if(!BackupDb(sDbName,sBackPath))
				{
					return FALSE;
				}
	
				eType = OIT_DELETE;
			}
			else
			{
				eType = OIT_KEEP;
				return TRUE;
			}
		}
		else if (OIT_BACKANDNEW == eType)
		{
			CString sBackPath;
			COleDateTime dt = COleDateTime::GetCurrentTime();
			sBackPath.Format(_T("%s%s_%d%d%d%d%d%d.bak"),"c:\\",sDbName,
				dt.GetYear(),
				dt.GetMonth(),
				dt.GetDay(),
				dt.GetHour(),
				dt.GetMinute(),
				dt.GetSecond());
			if(!BackupDb(sDbName,sBackPath))
			{
				return FALSE;
			}

			eType = OIT_DELETE;
		}
		else if (OIT_KEEP == eType)
		{
			return TRUE;
		}
		else if (OIT_DELETE == eType)
		{
			CString sBackPath;
			COleDateTime dt = COleDateTime::GetCurrentTime();
			sBackPath.Format(_T("%s%s_%d%d%d%d%d%d.bak"),"c:\\",sDbName,
				dt.GetYear(),
				dt.GetMonth(),
				dt.GetDay(),
				dt.GetHour(),
				dt.GetMinute(),
				dt.GetSecond());
			if(!BackupDb(sDbName,sBackPath))
			{
				return FALSE;
			}
		}
 		//return FALSE;
 	}

	//检测数据库还存在,如果存在直接删除
	while(CheckDbExist(sDbName))
	{
		DropDb(sDbName);
		::Sleep(100);
	}

	int nIndex = 0;
	//创建数据库
	while(!CheckDbExist(sDbName) && nIndex < 2)
	{
		::Sleep(100);
		if(!CreateDb(sDbName))
		{
			theLog.Write("创建数据库失败！");
			m_sErr = _T("创建数据库失败！");
			return FALSE;
		}
		nIndex ++;
		theLog.Write("CheckDbExist CreateDb %s,%d",sDbName,nIndex);

	}
	//恢复数据库
	if(!RestoreDB(sDbName,sPath))
	{
		theLog.Write("恢复数据库失败！sDbName=[%s], sPath=[%s]", sDbName, sPath);
		m_sErr = _T("恢复数据库失败！");
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDbInstaller::InstallDbBySqlFile(CString sDbSvr,CString sDbName,CString sDbUser,
									  CString sDbPwd,CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType)
{
	m_ado.SetConnStr(sDbSvr,"master",sDbUser,sDbPwd,sPort);
	if(!m_ado.Connect())
	{
		m_sErr = _T("连接数据库失败！");
		theLog.Write("连接数据库失败！");
		
		return FALSE;
	}

	int nMajor = 0;
	if(!CheckVersion(nMajor))
	{
		theLog.Write("检测数据库版本失败！");
		m_sErr = _T("检测数据库版本失败！");
		return FALSE;
	}
	//mssql 2000
	if(nMajor == 8)
	{
		//检测sp4
		if(!CheckSP4())
		{
			theLog.Write("数据库没有打SP4补丁！");
			m_sErr = _T("数据库没有打SP4补丁！");
			return FALSE;
		}
	}
	//sql2005 以上
	else if (nMajor >= 9)
	{

	}
	else
	{
		ASSERT(0);
	}

	//启动代理
	StartSqlAgent();

	if(CheckDbExist(sDbName))
	{
		theLog.Write("要创建的数据库已经存在，请先备份后，再删除此数据库！");
		m_sErr = _T("要创建的数据库已经存在，请先备份后，再删除此数据库！");
		if (eType == OIT_QUERY)
		{
			CString sMsg;
			sMsg.Format(_T("系统检测到数据库服务器上已经存在该产品的数据库。\n"
				"建议您先备份数据库，然后再做进一步操作。是否继续？\n\n"
				"如果选择“是”，系统将自动将数据备份到“C:\\”目录，然后重新安装数据库，原有数据将被删除！\n"
				"如果选择“否”，系统将继续使用现有数据库，原有数据将保留。"));

			int nId = ::MessageBox(0,sMsg,"提示",MB_ICONQUESTION |MB_YESNO);
			if (nId == IDYES)
			{
				CString sBackPath;
				COleDateTime dt = COleDateTime::GetCurrentTime();
				sBackPath.Format(_T("%s%s_%d%d%d%d%d%d.bak"),"c:\\",sDbName,
					dt.GetYear(),
					dt.GetMonth(),
					dt.GetDay(),
					dt.GetHour(),
					dt.GetMinute(),
					dt.GetSecond());
				if(!BackupDb(sDbName,sBackPath))
				{
					return FALSE;
				}

				eType = OIT_DELETE;
			}
			else
			{
				eType = OIT_KEEP;
				return TRUE;
			}
		}
		else if (eType == OIT_BACKANDNEW)
		{
			CString sBackPath;
			COleDateTime dt = COleDateTime::GetCurrentTime();
			sBackPath.Format(_T("%s%s_%d%d%d%d%d%d.bak"),"c:\\",sDbName,
				dt.GetYear(),
				dt.GetMonth(),
				dt.GetDay(),
				dt.GetHour(),
				dt.GetMinute(),
				dt.GetSecond());
			if(!BackupDb(sDbName,sBackPath))
			{
				return FALSE;
			}

			eType = OIT_DELETE;
			
		}
		else if (OIT_KEEP == eType)
		{
			return TRUE;
		}
		else if (OIT_DELETE == eType)
		{
			CString sBackPath;
			COleDateTime dt = COleDateTime::GetCurrentTime();
			sBackPath.Format(_T("%s%s_%d%d%d%d%d%d.bak"),"c:\\",sDbName,
				dt.GetYear(),
				dt.GetMonth(),
				dt.GetDay(),
				dt.GetHour(),
				dt.GetMinute(),
				dt.GetSecond());
			if(!BackupDb(sDbName,sBackPath))
			{
				return FALSE;
			}
		}
		//return FALSE;
	}

	//检测数据库还存在,如果存在直接删除
	while(CheckDbExist(sDbName))
	{
		DropDb(sDbName);
		::Sleep(100);
	}

	//创建数据库
	if(!CreateDb(sDbName))
	{
		theLog.Write("创建数据库失败！");
		m_sErr = _T("创建数据库失败！");
		return FALSE;
	}

	//脚本初始化数据库
	if(!ExecuteSqlTxt(sDbSvr,sDbName,sDbUser,sDbPwd,sPort,sPath))
	{
		theLog.Write("初始化数据库失败！");
		m_sErr = _T("初始化数据库失败！");
		return FALSE;
	}
	theLog.Write("数据库安装成功");
	return TRUE;
}

BOOL CDbInstaller::GetDefaultPort(CString& sPort)
{
	sPort.Empty();
	CRegistryEx reg(2,TRUE);
	CString sRegPath;
	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\MSSQLServer\MSSQLServer\SuperSocketNetLib\Tcp
	sRegPath.Format("SOFTWARE\\Microsoft\\MSSQLServer\\MSSQLServer\\SuperSocketNetLib\\Tcp");
	if(reg.Open(sRegPath))
	{

		if(!reg.Read("TcpPort",sPort))
		{
			
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
BOOL CDbInstaller::GetMsdeListenPort(CString sInstance,CString& sPort)
{
	sPort.Empty();
	CRegistryEx reg(2,TRUE);
	CString sRegPath;
	sRegPath.Format("SOFTWARE\\Microsoft\\Microsoft SQL Server\\%s\\MSSQLServer\\SuperSocketNetLib\\Tcp",sInstance);
	if(reg.Open(sRegPath))
	{

		if(!reg.Read("TcpPort",sPort))
		{
			
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}