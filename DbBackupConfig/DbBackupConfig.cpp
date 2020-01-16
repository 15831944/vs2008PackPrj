// DbBackupConfig.cpp : 定义应用程序的类行为。
#include "stdafx.h"
#include "DbBackupConfig.h"
#include "DBBakSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDbBackupConfigApp
BEGIN_MESSAGE_MAP(CDbBackupConfigApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CDbBackupConfigApp 构造
CDbBackupConfigApp::CDbBackupConfigApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

CFileLog theLog;

// 唯一的一个 CDbBackupConfigApp 对象
CDbBackupConfigApp theApp;

// CDbBackupConfigApp 初始化
BOOL CDbBackupConfigApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	theLog.SetLogName(_T("database"));

	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CString o1;
	AfxExtractSubString(o1,m_lpCmdLine,0,' ');
	o1.MakeLower();
	if(o1 == "-s" || o1 == "/s") 
	{
		CString sDBServerName;
		CString sDBName;
		CString sUserAccount;
		CString sUserPsw;
		CString sPort;
		CString sInstanceName;
		CDBBackFun::GetDatebaseConfig(sDBServerName,sDBName,sPort,sInstanceName,sUserAccount,sUserPsw);
		
		CString sBackDBPath;
		BOOL bAutoBackupOpen;
		BACKTYPE nAutoBackType;
		int nInterval;
		double dAutoBackTime;
		CDBBackFun::GetDBBackConfig(sBackDBPath,bAutoBackupOpen,nAutoBackType,nInterval,dAutoBackTime);

		CString sBackSavePath;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		sBackSavePath.Format(_T("%s%s_%04d-%02d-%02d-%02d-%02d-%02d.bak"),sBackDBPath,sDBName,
			dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());
		CDbInstaller dbins;
		if (sInstanceName.IsEmpty())
		{
			if (sPort.IsEmpty())
				dbins.GetDefaultPort(sPort);
			dbins.BackUpDBToFile(sDBServerName,sDBName,sUserAccount,sUserPsw,sPort,sBackSavePath);
		}
		else
		{
			dbins.BackUpDBToFile2(sDBServerName,sDBName,sUserAccount,sUserPsw,sInstanceName,sBackSavePath);
		}
	} 
	else
	{
		CDBBakSheet dlg(LoadS(IDS_STRINGBackupAndRestore),NULL,NULL);
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
	}
	return FALSE;
}
