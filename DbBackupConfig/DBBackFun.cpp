// DBBackFun.cpp: implementation of the CDBBackFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBBackFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDBBackFun::CDBBackFun()
{
}

CDBBackFun::~CDBBackFun()
{
}

// 从注册表中得到数据库的配置信息
void CDBBackFun::GetDatebaseConfig(CString& sServerName,CString& sBackDbName,CString& sPort,CString& sInstanceName,CString& sAccount,CString& sAccountPsw)
{
	do 
	{
		// 从配置文件中读到配置信息
		CIniFile iniFile;
		iniFile.m_sPath.Format("%s%s",CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
		sBackDbName = iniFile.GetVal("DbConfig","DbName","iSec_Print5");
		sServerName = iniFile.GetVal("DbConfig","DbSvr","(local)");
		sPort = iniFile.GetVal("DbConfig","DbPort","");
		sInstanceName = iniFile.GetVal("DbConfig","DbInstanceName","");
		sAccount = iniFile.GetVal("DbConfig","DbUser","sa");
		sAccountPsw = iniFile.GetVal("DbConfig","DbPwd","sa");

		// 从注册表中读取配置信息
		CRegistryEx reg;
		reg.SetParam(TRUE,TRUE);
		if (!reg.Open(PRODUCT_REGISTY_KEY))
			break;

		reg.Read(_T("ServerName"),sServerName);
		reg.Read(_T("DatabaseName"),sBackDbName);
		reg.Read(_T("AccountName"),sAccount);
		reg.Read(_T("AccountPsw"),sAccountPsw);

		reg.Close();
	} while (0);

#ifdef ENABLE_PWD_CRY
	char szPwd[MAX_PATH] = {0};
	int nSize = MAX_PATH;
	CPWDCry cry;
	cry.UnCryptPWD((char*)sAccountPsw.GetBuffer(),szPwd,nSize);
	sAccountPsw.ReleaseBuffer();
	sAccountPsw = szPwd;		
#endif
}

void CDBBackFun::SetDatebaseConfig(CString sServerName,CString sBackDbName,CString sAccount,CString sAccountPsw)
{
	// 从注册表中读取配置信息
	CRegistryEx reg;
	reg.SetParam(TRUE,FALSE);
	if (!reg.Open(PRODUCT_REGISTY_KEY))
	{
		reg.SetParam(TRUE,FALSE);
		if (!reg.CreateKey(PRODUCT_REGISTY_KEY))
		{
			return;
		}
	}

	reg.Write(_T("ServerName"),sServerName);
	reg.Write(_T("DatabaseName"),sBackDbName);
	reg.Write(_T("AccountName"),sAccount);
#ifdef ENABLE_PWD_CRY
	CPWDCry cry;
	CString sPwd;
	char* pPsw  = cry.CryptPWD(sAccountPsw.GetBuffer());
	sAccountPsw.ReleaseBuffer();
	sPwd.Format("%s",pPsw);
	delete pPsw;
	reg.Write(_T("AccountPsw"),sPwd);
#else
	reg.Write(_T("AccountPsw"),sAccountPsw);
#endif
	
	reg.Close();
}

void CDBBackFun::GetDBBackConfig(CString& sBackupDir,BOOL& bAutoBack,BACKTYPE& type,int& nInterVal,double& dTime)
{
	do 
	{
		sBackupDir = _T("C:\\DabaBase\\");
		bAutoBack = FALSE;
		type = BACKTYPE_BYDAY;
		nInterVal = 0;
		dTime = 0;

		// 从注册表中读取配置信息
		CRegistryEx reg;
		reg.SetParam(TRUE,TRUE);
		if (!reg.Open(PRODUCT_REGISTY_KEY))
			break;

		reg.Read(_T("BackupDir"),sBackupDir);
		reg.Read(_T("IsAutoBackOpen"),bAutoBack);
		reg.Read(_T("AutoBackType"),(int&)type);
		reg.Read(_T("AutoBackInterval"),nInterVal);
		CString sTime;
		reg.Read(_T("AutoBackTime"),sTime);
		COleDateTime dt;
		dt.ParseDateTime(sTime,VAR_TIMEVALUEONLY);
		dTime = dt.m_dt;
		reg.Close();
	} while (0);
}

void CDBBackFun::SetDBBackConfig(CString sBackupDir,BOOL bAutoBack,BACKTYPE type,int nInterVal,double dTime)
{
	// 从注册表中读取配置信息
	CRegistryEx reg;
	reg.SetParam(TRUE,FALSE);
	if (!reg.Open(PRODUCT_REGISTY_KEY))
	{
		reg.SetParam(TRUE,FALSE);
		if (!reg.CreateKey(PRODUCT_REGISTY_KEY))
		{
			return;
		}
	}

	reg.Write(_T("BackupDir"),sBackupDir);
	reg.Write(_T("IsAutoBackOpen"),bAutoBack);
	reg.Write(_T("AutoBackType"),(int)type);
	reg.Write(_T("AutoBackInterval"),nInterVal);
	COleDateTime dt;
	dt.m_dt = dTime;
	reg.Write(_T("AutoBackTime"),dt.Format(VAR_TIMEVALUEONLY));
	reg.Close();
}

void CDBBackFun::GetDBRestoreConfig(CString& sRestoreDir,CString& sInitDir)
{
	do 
	{
		sRestoreDir = _T("C:\\DabaBase\\");

		// 从注册表中读取配置信息
		CRegistryEx reg;
		reg.SetParam(TRUE,TRUE);
		if (!reg.Open(PRODUCT_REGISTY_KEY))
			break;

		reg.Read(_T("RestorePath"),sRestoreDir);
		reg.Read(_T("InitPath"),sInitDir);
		reg.Close();
	} while (0);
}

void CDBBackFun::SetDBRestoreConfig(CString sRestoreDir,CString sInitDir)
{
	CRegistryEx reg;
	reg.SetParam(TRUE,FALSE);
	if (!reg.Open(PRODUCT_REGISTY_KEY))
	{
		reg.SetParam(TRUE,FALSE);
		if (!reg.CreateKey(PRODUCT_REGISTY_KEY))
		{
			return;
		}
	}

	reg.Write(_T("RestorePath"),sRestoreDir);
	reg.Write(_T("InitPath"),sInitDir);
	reg.Close();
}