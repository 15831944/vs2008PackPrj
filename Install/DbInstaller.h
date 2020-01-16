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
	OIT_DELETE,		//删除
	OIT_BACKANDNEW	//备份并新建
};

class CDbInstaller  
{
public:
	CDbInstaller(BOOL bShowErr = TRUE);
	virtual ~CDbInstaller();

	static BOOL GetDefaultPort(CString& sPort);
	static BOOL GetMsdeListenPort(CString sInstance,CString& sPort);
	BOOL BackupDb(CString sDbName,CString sDbPath);
	BOOL DropDb(CString sDbName,BOOL bDrop = TRUE);
	BOOL InstallDbBySqlFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType);
	BOOL InstallDbByDbbackupFile(CString sDbSvr,CString sDbName,CString sDbUser,
		CString sDbPwd,CString sPort,CString sPath,OLDDB_INSTALL_TYPE& eType);
	BOOL CreateDb(CString sDbName);
	BOOL ExecuteSqlTxt(CString sIP,CString sDbName,CString sUser, CString sPsw, CString sPort, CString sPath);
	BOOL StartSqlAgent();
	BOOL CheckDbExist(CString sDbName);
	BOOL CheckSP4();
	BOOL CheckVersion(int& nMajor);
	BOOL RestoreDB(CString sDbName,CString sDbPath);
	BOOL UnIstallDb(CString sIP,CString sDbName,CString sUser, CString sPsw, CString sPort);
	
public:
	CString m_sErr;
protected:
	CAdo m_ado;
	BOOL m_bShowErr;
	
};

#endif // !defined(AFX_DBINSTALLER_H__94193770_7EFA_4C2E_BD83_50019B5AA4BD__INCLUDED_)
