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
	CoInitialize(NULL);
	m_bShowErr = bShowErr;
	m_bLocalDb = TRUE;
}

CDbInstaller::~CDbInstaller()
{
	CoUninitialize();
}

BOOL CDbInstaller::BackupDb(CString sDbName,CString sDbPath)
{
	if (!m_bLocalDb)
	{
		return BackupDb2(sDbName, sDbPath);
	}

	if (!CheckDbExist(sDbName))
	{
		theLog.Write("!!CDbInstaller::BackupDb,sDbName=%s,数据库不存在。", sDbName);
		return FALSE;
	}

	CString sSQL;
	sSQL.Format(" EXEC sp_addumpdevice 'disk', 'XabDbBackDisk', '%s' "
				" BACKUP DATABASE %s TO XabDbBackDisk with format "
				" exec sp_dropdevice 'XabDbBackDisk' ",sDbPath,sDbName);
	return m_ado.ExecuteSQL(sSQL);
}

BOOL CDbInstaller::BackupDb2(CString sDbName,CString sDbPath)
{
	theLog.Write("CDbInstaller::BackupDb2,1,sDbName=%s,sDbPaht=%s", sDbName, sDbPath);

	//在备份机建立共享文件夹
	CString szShareName("TempDataBaseShareDir");
	CString szShareDir;
	CString szSharePath;
	BOOL bRet = CCommonFun::GetSharePath(sDbPath, szShareName, szShareDir, szSharePath);
	theLog.Write("CDbInstaller::BackupDb2,2,bRet=%d,szSharePath=%s", bRet, szSharePath);
	if (!bRet)
	{
		theLog.Write("!!CDbInstaller::BackupDb2,3,bRet=%d,szSharePath=%s", bRet, szSharePath);
		return FALSE;
	}

	//添加临时共享用户
	CString szAddUser = "XabTempUser";
	CString szAddUserPassword = "XabTempPwd123456@*Abc";
	CString szAddUserComment = "Xab Temp User, Will Delete.";
	CString szAddUserToGroup = "Administrators";
	if (!CCommonFun::AddUser(szAddUser, szAddUserPassword, szAddUserComment))
	{
		theLog.Write("!!CDbInstaller::BackupDb2,4, AddUser, failed, szAddUser=%s", szAddUser);
	}

	if (!CCommonFun::AddUserToGroup(szAddUser, szAddUserToGroup))
	{
		theLog.Write("!!CDbInstaller::BackupDb2,5, AddUserToGroup, failed, szAddUser=%s,"
			"szAddUserToGroup=%s", szAddUser, szAddUserToGroup);
	}

	//开启’xp_cmdshell’，否则xp_cmdshell失败
	CString sSQL;
	sSQL.Format(" USE master "
		" EXEC sp_configure 'show advanced options', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure 'xp_cmdshell', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure   'show advanced options', 0 "
		" RECONFIGURE WITH OVERRIDE ");

	//建立共享信用关系
	sSQL.AppendFormat(" exec master..xp_cmdshell 'net use %s %s /user:%s\\%s' "
		, szShareDir, szAddUserPassword, CCommonFun::GetLocalIP(), szAddUser);

	//还原数据库
	sSQL.AppendFormat(" EXEC sp_addumpdevice 'disk', 'XabDbBackDisk', '%s' "
		" BACKUP DATABASE %s TO XabDbBackDisk with format "
		" exec sp_dropdevice 'XabDbBackDisk' ",szSharePath,sDbName);

	//执行SQL
	bRet = m_ado.ExecuteSQL(sSQL);
	if(!bRet)
	{
		theLog.Write("!!CDbInstaller::BackupDb2,6");
	}

	//清理现场
	CCommonFun::DelNetShare(szShareName);
	CCommonFun::DeleteUser(szAddUser);

	return bRet;
}

// 从文件初始化数据库
BOOL CDbInstaller::RevertDB(CString sDbName,CString sDBPath)
{
	CString sSQL;
	sSQL.Format("EXEC sp_attach_db @dbname = N'%s',@filename1 = N'%s%s.mdf',@filename2 = N'%s%s.ldf'",
		sDbName,sDBPath,sDbName,sDBPath,sDbName);
	return m_ado.ExecuteSQL(sSQL);
}

CString CDbInstaller::GetDbInstallPath(CString sDbName)
{
	CString sInstallPath;
	CString sSQL;
	sSQL.Format("select filename from master.dbo.sysdatabases where name = '%s'",sDbName);
	sInstallPath = m_ado.GetOneString(sSQL);
	theLog.Write("CDbInstaller::GetDbInstallPath,sInstallPath=%s", sInstallPath);
	int nFind = sInstallPath.ReverseFind('\\');
	if(nFind >= 0)
		sInstallPath = sInstallPath.Left(nFind);
	return sInstallPath;
}

BOOL CDbInstaller::IsLocalDb(CString sDbSvr)
{
	if ((sDbSvr.Find("localhost") >= 0) || (sDbSvr.Find("127.0.0.1") >= 0))
	{
		return TRUE;
	}

	CStringArray ary;
	CCommonFun::GetLocalIPList(ary);
	for (int i=0; i<ary.GetCount(); i++)
	{
		if (sDbSvr.Find(ary.GetAt(i)) >= 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDbInstaller::RestoreDB(CString sDbName,CString sDbPath)
{
	if (!m_bLocalDb)
	{
		return RestoreDB2(sDbName, sDbPath);
	}

	CString sInstallPath = GetDbInstallPath(sDbName);
	if (sInstallPath.GetLength() <= 0)
	{
		theLog.Write("!!CDbInstaller::RestoreDB,GetDbInstallPath fail,sInstallPath=%s", sInstallPath);
		return FALSE;
	}

	CString sSQL;
	sSQL.Format(" Restore Database %s from disk = '%s' "
				 " WITH REPLACE,MOVE '%s_Data' TO '%s\\%s.mdf',"
				 " MOVE '%s_log' TO '%s\\%s.ldf'",
				sDbName,sDbPath,
				sDbName,sInstallPath,sDbName,
				sDbName,sInstallPath,sDbName);

	BOOL BRet = m_ado.ExecuteSQL(sSQL);
	if(!BRet)
	{
		theLog.Write("!!CDbInstaller::RestoreDB,5,sSQL=%s", sSQL);
	}
	return BRet;
}

//参考
//http://blog.csdn.net/cpu_2/article/details/46481735
//http://www.cnblogs.com/iinsnian/archive/2008/06/06/1214821.html
//SQLSERVER服务实例名称:192.168.0.2
//需要备份的数据库名称: a
//备份机器名称（Client端）：192.168.0.3
//备份机用户:zf 密码：123
//备份机域名：domain
//备份机提供备份需求的文件夹:D:\sqlbak
//exec master..xp_cmdshell 'net use  file://192.168.0.3/sqlbak 123 /user:domain\zf'
BOOL CDbInstaller::RestoreDB2(CString sDbName,CString sDbPath)
{
	theLog.Write("CDbInstaller::RestoreDB2,1,sDbName=%s,sDbPaht=%s", sDbName, sDbPath);

	//获取数据库安装路径
	CString sInstallPath = GetDbInstallPath(sDbName);
	if (sInstallPath.GetLength() <= 0)
	{
		theLog.Write("!!CDbInstaller::RestoreDB2,2,GetDbInstallPath fail,sInstallPath=%s", sInstallPath);
		return FALSE;
	}

	//在备份机建立共享文件夹
	CString szShareName("TempDataBaseShareDir");
	CString szShareDir;
	CString szSharePath;
	BOOL bRet = CCommonFun::GetSharePath(sDbPath, szShareName, szShareDir, szSharePath);
	theLog.Write("CDbInstaller::RestoreDB2,3,bRet=%d,szSharePath=%s", bRet, szSharePath);
	if (!bRet)
	{
		theLog.Write("!!CDbInstaller::RestoreDB2,4,bRet=%d,szSharePath=%s", bRet, szSharePath);
		return FALSE;
	}

	//添加临时共享用户
	CString szAddUser = "XabTempUser";
	CString szAddUserPassword = "XabTempPwd123456@*Abc";
	CString szAddUserComment = "Xab Temp User, Will Delete.";
	CString szAddUserToGroup = "Administrators";
	if (!CCommonFun::AddUser(szAddUser, szAddUserPassword, szAddUserComment))
	{
		theLog.Write("!!CDbInstaller::RestoreDB2,5, AddUser, failed, szAddUser=%s", szAddUser);
	}

	if (!CCommonFun::AddUserToGroup(szAddUser, szAddUserToGroup))
	{
		theLog.Write("!!CDbInstaller::RestoreDB2,6, AddUserToGroup, failed, szAddUser=%s,"
			"szAddUserToGroup=%s", szAddUser, szAddUserToGroup);
	}

	//开启’xp_cmdshell’，否则xp_cmdshell失败
	CString sSQL;
	sSQL.Format(" USE master "
		" EXEC sp_configure 'show advanced options', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure 'xp_cmdshell', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure   'show advanced options', 0 "
		" RECONFIGURE WITH OVERRIDE ");

	//建立共享信用关系
	sSQL.AppendFormat(" exec master..xp_cmdshell 'net use %s %s /user:%s\\%s' "
		, szShareDir, szAddUserPassword, CCommonFun::GetLocalIP(), szAddUser);

	//还原数据库
	if(0 == sDbName.CompareNoCase("iSec_PrinterApp"))
	{//App数据库
		sSQL.AppendFormat(" Restore Database %s from disk = '%s' "
			" WITH REPLACE,MOVE '%s' TO '%s\\%s_Data.mdf',"
			" MOVE '%s_log' TO '%s\\%s_Log.ldf'"
			, sDbName, szSharePath
			, sDbName, sInstallPath, sDbName
			, sDbName, sInstallPath, sDbName);    
	}
	else
	{
		sSQL.AppendFormat(" Restore Database %s from disk = '%s' "
			" WITH REPLACE,MOVE '%s_Data' TO '%s\\%s.mdf',"
			" MOVE '%s_log' TO '%s\\%s.ldf'",
			sDbName,szSharePath,
			sDbName,sInstallPath,sDbName,
			sDbName,sInstallPath,sDbName);
	}

	//执行SQL
	bRet = m_ado.ExecuteSQL(sSQL);
	if(!bRet)
	{
		theLog.WriteEx("!!CDbInstaller::RestoreDB2,7");
	}

	//清理现场
	CCommonFun::DelNetShare(szShareName);
	CCommonFun::DeleteUser(szAddUser);

	return bRet;
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

BOOL CDbInstaller::EnableXpCmdshell()
{
	CString sSQL;
	sSQL.Format(" USE master "
		" EXEC sp_configure 'show advanced options', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure 'xp_cmdshell', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure   'show advanced options', 0 "
		" RECONFIGURE WITH OVERRIDE ");
	return m_ado.ExecuteSQL(sSQL);
}

BOOL CDbInstaller::DisableXpCmdshell()
{
	CString sSQL;
	sSQL.Format(" USE master "
		" EXEC sp_configure 'show advanced options', 1 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure 'xp_cmdshell', 0 "
		" RECONFIGURE WITH OVERRIDE "
		" EXEC sp_configure   'show advanced options', 0 "
		" RECONFIGURE WITH OVERRIDE ");
	return m_ado.ExecuteSQL(sSQL);
}

BOOL CDbInstaller::StartSqlAgent()
{
	EnableXpCmdshell();	//启用xp_cmdshell，否则xp_cmdshell失败。
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

BOOL CDbInstaller::ConnectDBServer(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString sPort,BOOL bTips)
{
	do 
	{
		m_ado.SetConnStr(sDbSvr,"master",sDbUser,sDbPwd,sPort);
		if(!m_ado.Connect())
		{
			m_sErr = _T(LoadS(IDS_STRING_Db_ConnetFailed));
			break;
		}

		int nMajor = 0;
		if(!CheckVersion(nMajor))
		{
			m_sErr = _T(LoadS(IDS_STRING_Test_Dbversion_failed));
			break;
		}

		//mssql 2000
		if(nMajor == 8 && !CheckSP4())
		{
			//m_sErr = _T("数据库没有打SP4补丁！");
			m_sErr = _T(LoadS(IDS_STRING_Text4));
			break;
		}

		//启动代理
		StartSqlAgent();
		return TRUE;
	} while (0);
	theLog.Write(m_sErr);
	if (bTips)
		::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
	return FALSE;
}

BOOL CDbInstaller::ConnectDBServer2(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString sInstanceName,BOOL bTips)
{
	do 
	{
		m_ado.SetConnStr3(sDbSvr,"master",sDbUser,sDbPwd,sInstanceName);
		if(!m_ado.Connect())
		{
			m_sErr = _T(LoadS(IDS_STRING_Db_ConnetFailed));
			break;
		}

		int nMajor = 0;
		if(!CheckVersion(nMajor))
		{
			m_sErr = _T(LoadS(IDS_STRING_Test_Dbversion_failed));
			break;
		}

		//mssql 2000
		if(nMajor == 8 && !CheckSP4())
		{
			m_sErr = _T(LoadS(IDS_STRING_Text4));
			break;
		}

		//启动代理
		StartSqlAgent();
		return TRUE;
	} while (0);
	theLog.Write(m_sErr);
	if (bTips)
		::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
	return FALSE;
}

BOOL CDbInstaller::InstallDbByDbbackupFile(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,
										   CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		if (!ConnectDBServer(sDbSvr,sDbName,sDbUser,sDbPwd,sPort))
		{
			bRet = FALSE;
			break;
		}

		//检测数据库还存在
		if(CheckDbExist(sDbName))
		{
			if (eType == OIT_QUERY)
			{
				CString sMsg;
				sMsg.Format(_T(LoadS(IDS_STRING_Text5)));
				int nId = ::MessageBox(0,sMsg,LoadS(IDS_STRING_Tip),MB_ICONQUESTION |MB_YESNO);
				if (nId == IDYES)
				{
					CString sBackPath;
					COleDateTime dt = COleDateTime::GetCurrentTime();
					sBackPath.Format(_T("%s%s_%04d-%02d-%02d-%02d-%02d-%02d.bak"),"c:\\",sDbName,
						dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());
					if(!BackupDb(sDbName,sBackPath))
					{
						m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Failed)),sDbName,sBackPath);
						theLog.Write(m_sErr);
						::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
						bRet = FALSE;
						break;
					}
					eType = OIT_DELETE;
				}
				else
				{
					eType = OIT_KEEP;
					break;
				}
			}
			else if (OIT_KEEP == eType)
			{
				break;
			}
			else if (OIT_DELETE == eType)
			{
				CString sBackPath;
				COleDateTime dt = COleDateTime::GetCurrentTime();
				sBackPath.Format(_T("%s%s_%04d-%02d-%02d-%02d-%02d-%02d.bak"),"c:\\",sDbName,
					dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());
				if(!BackupDb(sDbName,sBackPath))
				{
					m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Failed)),sDbName,sBackPath);
					theLog.Write(m_sErr);
					::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
					bRet = FALSE;
					break;
				}
			}
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
			m_sErr.Format(_T(""),sDbName);
			theLog.Write(m_sErr);
			::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
			bRet = FALSE;
			break;
		}

		//恢复数据库
		if(!RestoreDB(sDbName,sPath))
		{
			m_sErr.Format(_T(IDS_STRING_Restore_DB_Failed),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
			bRet = FALSE;
			break;
		}
		else
		{
			m_sErr.Format(_T(IDS_STRING_Restore_DB_Suc),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
		}
	} while (0);
	return bRet;
}

BOOL CDbInstaller::InstallDbBySqlFile(CString sDbSvr,CString sDbName,CString sDbUser,
									  CString sDbPwd,CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		if (!ConnectDBServer(sDbSvr,sDbName,sDbUser,sDbPwd,sPort))
		{
			bRet = FALSE;
			break;
		}

		//检测数据库还存在
		if(CheckDbExist(sDbName))
		{
 			if (eType == OIT_QUERY)
 			{
 				CString sMsg;
 				/*sMsg.Format(_T("系统检测到数据库服务器上已经存在该产品的数据库。\n"
 					"建议您先备份数据库，然后再做进一步操作。是否继续？\n\n"
 					"如果选择“是”，系统将自动将数据备份到“C:\\”目录，然后重新安装数据库，原有数据将被删除！\n"
 					"如果选择“否”，系统将继续使用现有数据库，原有数据将保留。"));*/
				sMsg.Format(_T(IDS_STRING_Text5));
				int nId = ::MessageBox(0,sMsg,LoadS(IDS_STRING_Tip),MB_ICONQUESTION |MB_YESNO);
				if (nId == IDYES)
				{
					CString sBackPath;
					COleDateTime dt = COleDateTime::GetCurrentTime();
					sBackPath.Format(_T("%s%s_%04d-%02d-%02d-%02d-%02d-%02d.bak"),"c:\\",sDbName,
						dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());
					if(!BackupDb(sDbName,sBackPath))
					{
						m_sErr.Format(_T(IDS_STRING_Db_Bakup_Failed),sDbName,sBackPath);
						theLog.Write(m_sErr);
						::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
						bRet = FALSE;
						break;
					}
					eType = OIT_DELETE;
				}
				else
				{
					eType = OIT_KEEP;
					break;
				}
			}
			else if (OIT_KEEP == eType)
			{
				break;
			}
			else if (OIT_DELETE == eType)
			{
				CString sBackPath;
				COleDateTime dt = COleDateTime::GetCurrentTime();
				sBackPath.Format(_T("%s%s_%04d-%02d-%02d-%02d-%02d-%02d.bak"),"c:\\",sDbName,
					dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());
				if(!BackupDb(sDbName,sBackPath))
				{
					//m_sErr.Format(_T("将已存在的数据库\"%s\"备份到\"%s\"失败！"),sDbName,sBackPath);
					m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Failed)),sDbName,sBackPath);
					theLog.Write(m_sErr);
					::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
					bRet = FALSE;
					break;
				}
			}
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
			//m_sErr.Format(_T("创建数据库\"%s\"失败！"),sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_CreateDB_Failed)),sDbName);
			theLog.Write(m_sErr);
			::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
			bRet = FALSE;
			break;
		}

		//脚本初始化数据库
		if(!ExecuteSqlTxt(sDbSvr,sDbName,sDbUser,sDbPwd,sPort,sPath))
		{
			m_sErr.Format(_T(LoadS(IDS_STRING_CreateDBbyScriptFailed)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
			bRet = FALSE;
			break;
		}
		else
		{
			m_sErr.Format(_T(LoadS(IDS_STRING_CreateDBbyScriptSucess)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(NULL,m_sErr,_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
		}
	} while (0);
	return bRet;
}

// 备份数据库
BOOL CDbInstaller::BackUpDBToFile(CString sDbSvr,CString sDbName,CString sDbUser,
					CString sDbPwd,CString sPort,CString sPath,BOOL bTips)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		// 连接数据库
		if (!ConnectDBServer(sDbSvr,sDbName,sDbUser,sDbPwd,sPort,bTips))
		{
			bRet = FALSE;
			break;
		}

		// 如果数据库不存在
		if(!CheckDbExist(sDbName))
		{
			//m_sErr.Format(_T("需要备份的数据库:\"%s\"不存在!"),sDbName);
			m_sErr.Format(_T(IDS_STRING_BakUpFileNotExit),sDbName);
			theLog.Write(m_sErr);
			if (bTips)
				::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}

		// 备份数据库
		if(!BackupDb(sDbName,sPath))
		{
			//m_sErr.Format(_T("备份数据库:\"%s\"到\"%s\"失败!"),sDbName,sPath);
			m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Failed)),sDbName,sPath);
			theLog.Write(m_sErr);
			if (bTips)
				::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}
		else
		{
			//m_sErr.Format(_T("备份数据库:\"%s\"到\"%s\"成功!"),sDbName,sPath);
			m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Sucess)),sDbName,sPath);
			theLog.Write(m_sErr);
			if (bTips)
				::MessageBox(0,m_sErr,"提示",MB_ICONINFORMATION |MB_OK);
		}
	} while (0);
	return bRet;
}

// 备份数据库
BOOL CDbInstaller::BackUpDBToFile2(CString sDbSvr,CString sDbName,CString sDbUser,
					CString sDbPwd,CString sInstanceName,CString sPath,BOOL bTips)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		// 连接数据库
		if (!ConnectDBServer2(sDbSvr,sDbName,sDbUser,sDbPwd,sInstanceName,bTips))
		{
			bRet = FALSE;
			break;
		}

		// 如果数据库不存在
		if(!CheckDbExist(sDbName))
		{
			//m_sErr.Format(_T("需要备份的数据库:\"%s\"不存在!"),sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_BakUpFileNotExit)),sDbName);
			theLog.Write(m_sErr);
			if (bTips)
				::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}

		// 备份数据库
		if(!BackupDb(sDbName,sPath))
		{
			//m_sErr.Format(_T("备份数据库:\"%s\"到\"%s\"失败!"),sDbName,sPath);
			m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Failed)),sDbName,sPath);
			theLog.Write(m_sErr);
			if (bTips)
				::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}
		else
		{
			//m_sErr.Format(_T("备份数据库:\"%s\"到\"%s\"成功!"),sDbName,sPath);
			m_sErr.Format(_T(LoadS(IDS_STRING_Db_Bakup_Sucess)),sDbName,sPath);
			theLog.Write(m_sErr);
			if (bTips)
				::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
		}
	} while (0);
	return bRet;
}

// 还原数据库
BOOL CDbInstaller::RestoreDBFromFile(CString sDbSvr,CString sDbName,CString sDbUser,
					   CString sDbPwd,CString sPort,CString sPath)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		if (!ConnectDBServer(sDbSvr,sDbName,sDbUser,sDbPwd,sPort))
		{
			bRet = FALSE;
			break;
		}
		if (!DropDb(sDbName,FALSE))
		{
			bRet = FALSE;
			break;
		}

		//恢复数据库
		if(!RestoreDB(sDbName,sPath))
		{
			//m_sErr.Format(_T("从\"%s\"还原数据库:\"%s\"失败!"),sPath,sDbName);
			m_sErr.Format(_T(IDS_STRING_Restore_DB_Failed),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}
		else
		{
			//m_sErr.Format(_T("从\"%s\"还原数据库:\"%s\"成功!"),sPath,sDbName);
			m_sErr.Format(_T(IDS_STRING_Restore_DB_Suc),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
		}
	} while (0);
	return bRet;
}

// 还原数据库
BOOL CDbInstaller::RestoreDBFromFile2(CString sDbSvr,CString sDbName,CString sDbUser,
					   CString sDbPwd,CString sInstanceName,CString sPath)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		if (!ConnectDBServer2(sDbSvr,sDbName,sDbUser,sDbPwd,sInstanceName))
		{
			bRet = FALSE;
			break;
		}
		if (!DropDb(sDbName,FALSE))
		{
			bRet = FALSE;
			break;
		}

		//恢复数据库
		if(!RestoreDB(sDbName,sPath))
		{
			//m_sErr.Format(_T("从\"%s\"还原数据库:\"%s\"失败!"),sPath,sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_Restore_DB_Failed)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}
		else
		{
			//m_sErr.Format(_T("从\"%s\"还原数据库:\"%s\"成功!"),sPath,sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_Restore_DB_Suc)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
		}
	} while (0);
	return bRet;
}

// 初始数据库
BOOL CDbInstaller::RevertDBFromFile(CString sDbSvr,CString sDbName,CString sDbUser,
					  CString sDbPwd,CString sPort,CString sPath)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		if (!ConnectDBServer(sDbSvr,sDbName,sDbUser,sDbPwd,sPort))
		{
			bRet = FALSE;
			break;
		}

		//恢复数据库
		if(!RevertDB(sDbName,sPath))
		{
			//m_sErr.Format(_T("从\"%s\"初始数据库:\"%s\"失败!"),sPath,sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_Restore_DB_Failed)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}
		else
		{
			//m_sErr.Format(_T("从\"%s\"初始数据库:\"%s\"成功!"),sPath,sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_Restore_DB_Suc)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
		}
	} while (0);
	return bRet;
}

// 初始数据库
BOOL CDbInstaller::RevertDBFromFile2(CString sDbSvr,CString sDbName,CString sDbUser,
					  CString sDbPwd,CString sInstanceName,CString sPath)
{
	m_bLocalDb = IsLocalDb(sDbSvr);	//设置数据库是否在本地

	BOOL bRet = TRUE;
	do 
	{
		if (!ConnectDBServer2(sDbSvr,sDbName,sDbUser,sDbPwd,sInstanceName))
		{
			bRet = FALSE;
			break;
		}

		//恢复数据库
		if(!RevertDB(sDbName,sPath))
		{
			//m_sErr.Format(_T("从\"%s\"初始数据库:\"%s\"失败!"),sPath,sDbName);
			m_sErr.Format(_T(LoadS(IDS_STRING_Restore_DB_Failed)),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
			bRet = FALSE;
			break;
		}
		else
		{
			//m_sErr.Format(_T("从\"%s\"初始数据库:\"%s\"成功!"),sPath,sDbName);
			m_sErr.Format(_T(IDS_STRING_Restore_DB_Suc),sPath,sDbName);
			theLog.Write(m_sErr);
			::MessageBox(0,m_sErr,LoadS(IDS_STRING_Tip),MB_ICONINFORMATION |MB_OK);
		}
	} while (0);
	return bRet;
}

BOOL CDbInstaller::GetDefaultPort(CString& sPort)
{
	sPort.Empty();
	CRegistryEx reg(2,TRUE);
	CString sRegPath;
	sRegPath.Format("SOFTWARE\\Microsoft\\MSSQLServer\\MSSQLServer\\SuperSocketNetLib\\Tcp");
	if(reg.Open(sRegPath))
	{
		if(!reg.Read("TcpPort",sPort))			
			return FALSE;
		else
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
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}