// DBBackFun.h: interface for the CDBBackFun class.
#if !defined(AFX_DBBACKFUN_H__A2444F30_62B6_443E_94EE_7956639BD322__INCLUDED_)
#define AFX_DBBACKFUN_H__A2444F30_62B6_443E_94EE_7956639BD322__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum
{
	BACKTYPE_BYDAY = 0,
	BACKTYPE_BYMONTH = 1,
	BACKTYPE_BYWEEK = 2
}BACKTYPE;

class CDBBackFun  
{
public:
	CDBBackFun();
	virtual ~CDBBackFun();
	
public:
	// 得到当前文件的路径
	void static GetLocalFilePath(CString& sLocalFilePath);

	/* 
	从注册表中得到/设置数据库的配置信息
	sServerName:数据库服务器名称
	sBackDbName:备份数据库名称
	sPort:数据库访问端口
	sAccount:登录数据库的用户名
	sAccountPsw:登录数据库的密码
	*/
	void static GetDatebaseConfig(CString& sServerName,CString& sBackDbName,CString& sPort,CString& sInstanceName,CString& sAccount,CString& sAccountPsw);
	void static SetDatebaseConfig(CString sServerName,CString sBackDbName,CString sAccount,CString sAccountPsw);

	/*
	从注册表中得到/设置数据库备份的信息
	sBackupDir: 备份数据库保存目录
	sDesc:备份数据库的描述
	bAutoBack:是否自动备份
	type:备份时间类型
	nInterVal:备份时间差
	dTime:备份时间
	*/
	void static GetDBBackConfig(CString& sBackupDir,BOOL& bAutoBack,BACKTYPE& type,int& nInterVal,double& dTime);
	void static SetDBBackConfig(CString sBackupDir,BOOL bAutoBack,BACKTYPE type,int nInterVal,double dTime);

	/*
	从注册表中得到/设置数据库还原信息
	sRestoreDir:还原数据库目录
	*/
	void static GetDBRestoreConfig(CString& sRestoreDir,CString& sInitDir);
	void static SetDBRestoreConfig(CString sRestoreDir,CString sInitDir);
};
#endif // !defined(AFX_DBBACKFUN_H__A2444F30_62B6_443E_94EE_7956639BD322__INCLUDED_)
