// DbInstaller.h: interface for the CDbInstaller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBINSTALLER_H__94193770_7EFA_4C2E_BD83_50019B5AA4BD__INCLUDED_)
#define AFX_DBINSTALLER_H__94193770_7EFA_4C2E_BD83_50019B5AA4BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ado.h"
enum OLDDB_INSTALL_TYPE
{
	OIT_QUERY,		//询问
	OIT_KEEP,		//保留
	OIT_DELETE		//删除
};

class CDbInstaller  
{
public:
	CDbInstaller(BOOL bShowErr = TRUE);
	virtual ~CDbInstaller();

public:
	static BOOL GetDefaultPort(CString& sPort);
	static BOOL GetMsdeListenPort(CString sInstance,CString& sPort);

public:
	// 从SQL文件中安装数据库
	BOOL InstallDbBySqlFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType);

	// 从备份文件安装数据库
	BOOL InstallDbByDbbackupFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType);

	// 备份数据库
	BOOL BackUpDBToFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath,BOOL bTips = FALSE);

	// 备份数据库
	BOOL BackUpDBToFile2(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sInstanceName,CString sPath,BOOL bTips = FALSE);

	// 还原数据库
	BOOL RestoreDBFromFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath);

	// 还原数据库
	BOOL RestoreDBFromFile2(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sInstanceName,CString sPath);

	// 初始数据库
	BOOL RevertDBFromFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath);

	// 初始数据库
	BOOL RevertDBFromFile2(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sInstanceName,CString sPath);

	// 卸载数据库
	BOOL UnIstallDb(CString sIP,CString sDbName,CString sUser, CString sPsw, CString sPort);

private:
	BOOL ConnectDBServer(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString sPort,BOOL bTips = TRUE);
	BOOL ConnectDBServer2(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString sInstanceName,BOOL bTips = TRUE);
	BOOL BackupDb(CString sDbName,CString sDbPath);// 备份数据库
	BOOL BackupDb2(CString sDbName,CString sDbPath);	//从远程数据库备份到本机的bak
	BOOL RestoreDB(CString sDbName,CString sDbPath);// 还原数据库
	BOOL RestoreDB2(CString sDbName,CString sDbPath);	//从本机的bak还原到远程数据库
	BOOL RevertDB(CString sDbName,CString sDBPath);// 从文件初始化数据库
	BOOL DropDb(CString sDbName,BOOL bDrop = TRUE);// 删除数据库
	BOOL CreateDb(CString sDbName);// 创建数据库
	BOOL ExecuteSqlTxt(CString sIP,CString sDbName,CString sUser, CString sPsw, CString sPort, CString sPath);
	BOOL EnableXpCmdshell();	//启用’xp_cmdshell’
	BOOL DisableXpCmdshell();	//关闭’xp_cmdshell’
	BOOL StartSqlAgent();
	BOOL CheckDbExist(CString sDbName);
	BOOL CheckSP4();
	BOOL CheckVersion(int& nMajor);
	CString GetDbInstallPath(CString sDbName);	//获取数据库安装路径
	BOOL IsLocalDb(CString sDbSvr);	//判断数据库是否在本机
	
public:
	CString m_sErr;

protected:
	CAdo m_ado;
	BOOL m_bLocalDb;	//数据库是否在本地
	BOOL m_bShowErr;
	
};

#endif // !defined(AFX_DBINSTALLER_H__94193770_7EFA_4C2E_BD83_50019B5AA4BD__INCLUDED_)
