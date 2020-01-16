#include "StdAfx.h"
#include "PrinterInstall.h"
#include "NtDriverController.h"
#include "PrinterUtils.h"
#include <atlsecurity.h>

CPrinterInstallHelper::CPrinterInstallHelper(void)
{
	SetDefaultPath(NULL);
}

CPrinterInstallHelper::~CPrinterInstallHelper(void)
{
}

void CPrinterInstallHelper::SetDefaultPath(char* pPath)
{
	if (pPath)
	{
		m_sDefaultPath.Format("%s",pPath);
	}

	if (m_sDefaultPath.IsEmpty())
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(0,path,MAX_PATH);
		TCHAR *pdest = 0;
		pdest=_tcsrchr(path,_T('\\'));
		if(pdest) 
			*pdest=0;
		m_sDefaultPath=path;
	}
	int nFind = 0;
	if ((nFind = m_sDefaultPath.Find("x64")) > 1)
	{
		m_sDefaultPath = m_sDefaultPath.Left(nFind - 1);
	}

	theIniFile.m_sPath.Format("%s\\config\\config.ini", m_sDefaultPath);
	CString szPrinterName = theIniFile.GetVal("ClientConfig","PrinterName",IST_PRINTER_NAME);	//默认打印机名
	strcpy_s(m_szPrinterName,sizeof(m_szPrinterName),szPrinterName);
	sprintf_s(m_szDriverName,sizeof(m_szDriverName),IST_PRINTER_DRIVER,szPrinterName);
	sprintf_s(m_szPortName,sizeof(m_szPortName),IST_PRINTER_PORT,szPrinterName);
	sprintf_s(m_szProcName,sizeof(m_szProcName),IST_PRINTER_PROC,szPrinterName);
	sprintf_s(m_szProcDll,sizeof(m_szProcDll),IST_PRINTER_PROC_DLL,szPrinterName);
	sprintf_s(m_szPrinterKeyPath,sizeof(m_szPrinterKeyPath),XAB_PRINTER_KEY_PATH,szPrinterName);
}

BOOL CPrinterInstallHelper::UninstallPortMon(const MONITOR_INFO_2 &mi2)
{
	WCHAR szBuffer[4096];
	DWORD dwNeeded;
	DWORD dwReturned;
	if (::EnumMonitors(NULL, 1, (LPBYTE)szBuffer, sizeof(szBuffer), &dwNeeded, &dwReturned)) 
	{
		PMONITOR_INFO_1 mi = reinterpret_cast<PMONITOR_INFO_1>(szBuffer);
		for (DWORD i = 0; i < dwReturned; ++i) 
		{
			WriteLogEx("UninstallPortMon %s",mi[i].pName);
			if (::lstrcmp(mi[i].pName, mi2.pName) == 0)
			{
				if ( !DeleteMonitor(0, XAB_MONITORENVNT, mi2.pName) )
				{
					WriteLogEx("UninstallPortMon DeleteMonitor %d",::GetLastError());
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

int CPrinterInstallHelper::InstallPrinterMonitor(LPCTSTR portMonName, LPCTSTR portMonFileName)
{
	MONITOR_INFO_2 mi2 = {0};

	mi2.pName =  (LPSTR)portMonName;
	mi2.pEnvironment = XAB_MONITORENVNT;
	mi2.pDLLName = (LPSTR)portMonFileName;
	if ( !UninstallPortMon(mi2) )
	{
		//return GetLastError();
	}
	WriteLogEx("InstallPrinterMonitor %s,%s",portMonName,portMonFileName);


	if ( !AddMonitor(NULL, 2, (LPBYTE)&mi2) )
	{
		int nErr = GetLastError();
		WriteLogEx("InstallPrinterMonitor err %s,%s,%d",portMonName,portMonFileName,nErr);
		return nErr;
	}
	WCHAR szBuffer[4096];
	DWORD dwNeeded;
	DWORD dwReturned;
	if (::EnumMonitors(NULL, 1, (LPBYTE)szBuffer, sizeof(szBuffer), &dwNeeded, &dwReturned)) 
	{
		PMONITOR_INFO_1 mi = reinterpret_cast<PMONITOR_INFO_1>(szBuffer);
		for (DWORD i = 0; i < dwReturned; ++i) 
		{
			WriteLogEx("InstallPrinterMonitor %s",mi[i].pName);
			
		}
	}
	return 0;
}

BOOL CPrinterInstallHelper::AddPortX()
{
	BOOL bRet = TRUE;
	DWORD cbneed,cbstate;
	PBYTE pOutputData;
	HANDLE hXcv = INVALID_HANDLE_VALUE;
	PRINTER_DEFAULTS Defaults = { NULL,NULL,SERVER_ACCESS_ADMINISTER};//PRINTER_ACCESS_ADMINISTER   SERVER_ACCESS_ADMINISTER 		

	WCHAR pszPortName[MAX_PATH*2] = {0};
	MultiByteToWideChar(CP_GB2312,0,m_szPortName,strlen(m_szPortName)+1,pszPortName,260);

	int nLen = 2*(wcslen(pszPortName)+1);

	pOutputData=(PBYTE)malloc(MAX_PATH);
	memset(pOutputData,0,MAX_PATH);
	WriteLogEx("CPrinterInstallHelper::AddPortX 11 %S",(LPCTSTR)pszPortName);
	if(!OpenPrinter(",XcvMonitor Local Port", &hXcv, &Defaults))
	//if(!OpenPrinter(",XcvMonitor DDK Port Monitor", &hXcv, &Defaults))
	{
		LPVOID lpMsgBuf; 
		GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL );
		//::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);
		WriteLogEx("CPrinterInstallHelper::AddPortX 11 %s",(LPCTSTR)lpMsgBuf);
		free(pOutputData);
		LocalFree( lpMsgBuf );
		return FALSE;

	}
	if(!XcvData(hXcv,L"AddPort",(PBYTE)pszPortName,nLen,(PBYTE)pOutputData,MAX_PATH,&cbneed,&cbstate))
	{
		int nError = GetLastError();
		LPVOID lpMsgBuf; 
		SetLastError(cbstate);
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
		//::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::AddPortX 22 %s",(LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf ); 
		bRet = FALSE;
	}
	ERROR_SUCCESS;
	WriteLogEx("AddPort = %d,%s",cbstate,(char*)pOutputData);
	free(pOutputData);
	ClosePrinter(hXcv);

	return bRet;
}

BOOL CPrinterInstallHelper::AddPrinterDriverX()
{
	DRIVER_INFO_3 di3; 

	char lpDriverPath[MAX_PATH] = {0}; 
	char lpBuffer[MAX_PATH] = {0}; 
	DWORD uSize=0; 

	BOOL b=::GetPrinterDriverDirectory(NULL,_PrintEnvironment,1,(LPBYTE)lpBuffer,MAX_PATH,&uSize);
	WriteLogEx("AddPrinterDriverX %s",lpBuffer);
#ifdef _AMD64_
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT5.DLL")); 

	char lpDriverConfigFile[MAX_PATH]; 
	lstrcpy(TEXT(lpDriverConfigFile),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverConfigFile),TEXT("\\PS5UI.DLL")); 

	char lpDataFilePath[MAX_PATH]; 
	lstrcpy(TEXT(lpDataFilePath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDataFilePath),TEXT("\\XabVPrinter.PPD")); 

	char lpHelpFilePath[MAX_PATH]; 
	lstrcpy(TEXT(lpHelpFilePath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpHelpFilePath),TEXT("\\PSCRIPT.HLP")); 

	ZeroMemory(&di3, sizeof(DRIVER_INFO_3)); 
	di3.cVersion = 0x03; 
	di3.pConfigFile =lpDriverConfigFile; 
	di3.pDataFile =lpDataFilePath; 	
	di3.pDependentFiles = TEXT("PSCRIPT.NTF\0PS_SCHM.GDL\0gsdll64.dll\0XabVPrinter.dll\0XabVPrinter.ini\0\0");
	di3.pDriverPath = lpDriverPath; 
	di3.pEnvironment = _PrintEnvironment;
	di3.pHelpFile = lpHelpFilePath;
	di3.pMonitorName =  NULL;//_T("Sample Port Monitor"); 
	di3.pName = m_szDriverName;
	di3.pDefaultDataType = IST_PRINTER_DATATYPE;

	if(!::AddPrinterDriver(NULL, 3, (LPBYTE)&di3))
	{
		LPVOID lpMsgBuf; 
		DWORD dwError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwError, NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
		//::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::AddPrinterDriverX %s,%u",(LPCTSTR)lpMsgBuf,dwError);
		LocalFree( lpMsgBuf ); 
		return FALSE;
	}
#else
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT5.DLL")); 

	char lpDriverConfigFile[MAX_PATH]; 
	lstrcpy(TEXT(lpDriverConfigFile),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverConfigFile),TEXT("\\PS5UI.DLL")); 

	char lpDataFilePath[MAX_PATH]; 
	lstrcpy(TEXT(lpDataFilePath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDataFilePath),TEXT("\\XabVPrinter.PPD")); 

	char lpHelpFilePath[MAX_PATH]; 
	lstrcpy(TEXT(lpHelpFilePath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpHelpFilePath),TEXT("\\PSCRIPT.HLP")); 

	ZeroMemory(&di3, sizeof(DRIVER_INFO_3)); 
	di3.cVersion = 0x03; 
	di3.pConfigFile =lpDriverConfigFile; 
	di3.pDataFile =lpDataFilePath; 	
	di3.pDependentFiles = TEXT("PSCRIPT.NTF\0PS_SCHM.GDL\0gsdll32.dll\0XabVPrinter.dll\0XabVPrinter.ini\0\0");
	di3.pDriverPath = lpDriverPath; 
	di3.pEnvironment = _PrintEnvironment;
	di3.pHelpFile = lpHelpFilePath;
	di3.pMonitorName =  NULL;//_T("Sample Port Monitor"); 
	di3.pName = m_szDriverName;
	di3.pDefaultDataType = IST_PRINTER_DATATYPE;

	if(!::AddPrinterDriver(NULL, 3, (LPBYTE)&di3))
	{
		LPVOID lpMsgBuf; 
		DWORD dwError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwError, NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
		//::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::AddPrinterDriverX %s,%u", (LPCTSTR)lpMsgBuf, dwError);
		LocalFree( lpMsgBuf ); 
		return FALSE;
	}
#endif
	return TRUE;
}

BOOL CPrinterInstallHelper::AddPrinterX()
{
	DWORD  dwValue =0;
	PRINTER_INFO_2 pi2;
	HANDLE pHD;

	ZeroMemory(&pi2, sizeof(PRINTER_INFO_2)); 
	pi2.pServerName= NULL; 
	pi2.pPrinterName = m_szPrinterName; 
	pi2.pPortName = m_szPortName;
	pi2.pDriverName = m_szDriverName;
	pi2.pPrintProcessor = TEXT("WinPrint"); //IST_PRINTER_PROC
	pi2.pDatatype = IST_PRINTER_DATATYPE; 
	//启用高级打印功能，否则在win10系统中，打印监视会出问题，导致无法提交作业。
	pi2.Attributes = PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS/*|PRINTER_ATTRIBUTE_QUEUED|PRINTER_ATTRIBUTE_DO_COMPLETE_FIRST|PRINTER_ATTRIBUTE_RAW_ONLY*/;

	if((pHD=::AddPrinter(NULL, 2, (LPBYTE)&pi2))==NULL)
	{
		LPVOID lpMsgBuf; 
		int nError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 

		//::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::AddPrinterX %s",(LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf ); 
		return FALSE;
	}
	ClosePrinter(pHD);

	return TRUE;
}
BOOL ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken    = NULL;

	// Open the process token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken ))
	{
		WriteLogEx(_T("Failed OpenProcessToken\n"));
		return ERROR_FUNCTION_FAILED;
	}

	// Get the local unique id for the privilege.
	if ( !LookupPrivilegeValue( NULL,
		szPrivilege,
		&luid ))
	{
		CloseHandle( hToken );
		WriteLogEx(_T("Failed LookupPrivilegeValue\n"));
		return ERROR_FUNCTION_FAILED;
	}

	// Assign values to the TOKEN_PRIVILEGE structure.
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes = 
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);

	// Adjust the token privilege.
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		sizeof(NewState),
		NULL,
		NULL))
	{
		WriteLogEx(_T("Failed AdjustTokenPrivileges\n"));
		hr = ERROR_FUNCTION_FAILED;
	}

	// Close the handle.
	CloseHandle(hToken);

	return hr;
}

BOOL CPrinterInstallHelper::Install(bool bOnlyInstallDrv)
{
	bool bRet = false;

	//win7 x86情况默认情况此服务是不开启的
	CNtDriverController nt;
	nt.SetStartSvcType(WINDOWS_SPOOL_SVR_NAME,SERVICE_AUTO_START);
	//【xp和2003系统中】停止spooler服务，因为在打印客户端卸载后，不重启电脑直接重新安装的时候会出现虚拟打印机脱机。
	nt.Stop(WINDOWS_SPOOL_SVR_NAME);
	::Sleep(1000);
	nt.StartDriver(WINDOWS_SPOOL_SVR_NAME);
	::Sleep(1000);

	WriteLogEx("CPrinterInstallHelper::Install,1,bOnlyInstallDrv=%d", bOnlyInstallDrv);

	//添加处理器
	//AddPrinterProcess();
	CString sCustom;
	CString sDriver;
	CString s;
	CRegistryEx reg(2,TRUE);
	if(reg.Open(PRODUCT_CUSTOM_REGISTY_KEY))
	{
		if (reg.Read("Custom",sCustom))
		{
			//WriteLogEx("sCustom = %s",sCustom);
		}
		else
		{
			WriteLogEx("!!CPrinterInstallHelper::Install,read Custom fail=[%s]",PRODUCT_REGISTY_KEY);
		}
	}
	else
	{
		WriteLogEx("!!CPrinterInstallHelper::Install,open fail=[%s]",PRODUCT_REGISTY_KEY);
	}

	WriteLogEx("sCustom=%s",sCustom);

	//--------------------
	//判断路径
	BOOL bCustom = FALSE;
	if(!sCustom.IsEmpty())
	{
		char lpBuffer[MAX_PATH + 1] = {0}; 
		DWORD uSize=0; 
		CString szDriverFile;
		CString sDefaultDir;
		sDefaultDir.Format(_T("%s\\%s\\%s"),m_sDefaultPath,PTINTER_BIN_DIR,sCustom);
		bCustom = ::PathFileExists(sDefaultDir);
	}
	
	ModifyPrivilege(SE_LOAD_DRIVER_NAME,TRUE);
	
	if (!bCustom)
	{
		//拷贝文件到system32/spool
		CopyDriverFile();
		if(AddPortX())
		{
			if(AddPrinterDriverX())
			{
				if(!bOnlyInstallDrv)
				{
					if(AddPrinterX())
					{
						bRet = true;
					}
				}
			}
		}
	}
	else
	{
		//拷贝文件到system32/spool
		CopyDriverFileY(sCustom);
		if(AddPortY())
		{
			if(AddPrinterDriverY(sCustom))
			{
				if(!bOnlyInstallDrv)
				{
					if(AddPrinterY())
					{
						bRet = true;
					}
				}
			}
		}
	}
	


	//设置工作目录
	SetWorkDir();
	//暂时先注掉，add by zhandb 2012 03.05
	//SetDefaultParam();

#if 0	//del by zxl,20170620
	//安装监视端口
	CString sMonPath;
	sMonPath.Format(_T("%s\\%s\\%s"),m_sDefaultPath,PTINTER_BIN_DIR,XAB_PORTMON_DLL);
	char szSystem[MAX_PATH] = {0};
	char szFile[MAX_PATH] = {0};
	GetSystemDirectory(szSystem,MAX_PATH);
	sprintf_s(szFile,sizeof(szFile),"%s\\%s",szSystem,XAB_PORTMON_DLL);

	BOOL b = CopyFile(sMonPath,szFile,	FALSE);
	WriteLogEx("1111 copy file from (%s) to (%s) ret(%d)",sMonPath,szFile,b);
	if (!b)
	{
		char szTmpFile[MAX_PATH] = {0};
		sprintf_s(szTmpFile,sizeof(szTmpFile),"%s\\%d_%s",szSystem,::GetTickCount(),XAB_PORTMON_DLL);
		rename(szFile,szTmpFile);
		BOOL b = CopyFile(sMonPath,szFile,	FALSE);
		WriteLogEx("2222 copy file from (%s) to (%s) ret(%d)",sMonPath,szFile,b);
	}
	InstallPrinterMonitor(XAB_PORTMON_NAME,XAB_PORTMON_DLL);
#endif

	//重启打印服务
	nt.Stop(WINDOWS_SPOOL_SVR_NAME);
	Sleep(1000);

	SetPrinterDefaultColorMode(DMCOLOR_MONOCHROME);	//设置打印机的默认色彩模式为黑白

	nt.StartDriver(WINDOWS_SPOOL_SVR_NAME);
	Sleep(1000);
	if(bOnlyInstallDrv)//只安装打印驱动
		return true;

	SetDefaultPrinter();
//	DoSharePrinterNT(IST_PRINTER_NAME,"RemotePrint",true);
	return bRet;
}

BOOL CPrinterInstallHelper::DeletePortX()
{
	DWORD cbneed,cbstate;
	PBYTE pOutputData;
	HANDLE hXcv = INVALID_HANDLE_VALUE;
	PRINTER_DEFAULTS Defaults = { NULL,NULL,SERVER_ACCESS_ADMINISTER};//PRINTER_ACCESS_ADMINISTER   SERVER_ACCESS_ADMINISTER 		

	WCHAR pszPortName[260];
	MultiByteToWideChar(CP_GB2312,0,m_szPortName,strlen(m_szPortName)+1,pszPortName,260);

	pOutputData=(PBYTE)new BYTE[MAX_PATH];

	if(!OpenPrinter(",XcvMonitor Local Port", &hXcv, &Defaults))
	{
		LPVOID lpMsgBuf; 
		GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL );
	//	::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);
		WriteLogEx("CPrinterInstallHelper::DeletePortX AA %s",(LPCTSTR)lpMsgBuf);
		delete[] pOutputData;
		LocalFree( lpMsgBuf );
		return FALSE;

	}
	if(!XcvData(hXcv,L"DeletePort",(PBYTE)pszPortName,sizeof(pszPortName),(PBYTE)pOutputData,MAX_PATH,&cbneed,&cbstate))
	{
		int nError = GetLastError();

		LPVOID lpMsgBuf; 
		SetLastError(cbstate);
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
	//	::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::DeletePortX DeletePort %s,%S",(LPCTSTR)lpMsgBuf,pszPortName);
		LocalFree( lpMsgBuf ); 
	}

	delete[] pOutputData;
	ClosePrinter(hXcv);

	return TRUE;

}

BOOL CPrinterInstallHelper::DeletePrinterDriverX()
{
	BOOL bRet = TRUE;
	if(!DeletePrinterDriver(NULL,_PrintEnvironment,m_szDriverName)) 
	{ 
		LPVOID lpMsgBuf; 
		GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
	//	::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::DeletePrinterDriverX %s",(LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf ); 
		bRet = FALSE;
	} 
	return bRet;
}

BOOL CPrinterInstallHelper::DeletePrinterX()
{
	BOOL bRet = FALSE;
	PRINTER_DEFAULTS  pd;
	HANDLE  pHd;

	ZeroMemory(&pd, sizeof(PRINTER_DEFAULTS)); 
	pd.DesiredAccess = PRINTER_ALL_ACCESS;

	bRet = OpenPrinter(m_szPrinterName, &pHd, &pd);
	if ( !bRet )
		return bRet;
	bRet = ::DeletePrinter( pHd );
	if ( !bRet)
	{
		LPVOID lpMsgBuf; 
		GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
		WriteLogEx("CPrinterInstallHelper::DeletePrinterX %s",(LPCTSTR)lpMsgBuf);
	//	::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);		 
		LocalFree( lpMsgBuf ); 
	}
	ClosePrinter( pHd );
	return bRet;
}

BOOL CPrinterInstallHelper::Uninstall()
{
	WriteLogEx("CPrinterInstallHelper::Uninstall");
	DeleteAllJob();
#if 1
	CNtDriverController nt;
	nt.Stop(WINDOWS_SPOOL_SVR_NAME);
	::Sleep(1000);
	nt.StartDriver(WINDOWS_SPOOL_SVR_NAME);
	::Sleep(1000);
#endif
	//之前最好停止一下Print Spool服务，否则可能删除不成功
	BOOL bRet1 = DeletePrinterX();
	Sleep(1000);
	BOOL bRet2 = DeletePrinterDriverX();
	Sleep(1000);
	BOOL bRet3 = DeletePortX();
	Sleep(1000);
	DelDriverFile();
	Sleep(1000);

	//add by zxl,20150821,删除Windows图片打印向导配置信息
	DeletePhotoPrintWizardPram();

#if 0	//del by zxl,20170620
	MONITOR_INFO_2 mi2 = {0};

	mi2.pName =  (LPSTR)XAB_PORTMON_NAME;
	mi2.pEnvironment = NULL;
	mi2.pDLLName = (LPSTR)XAB_PORTMON_DLL;
	if ( !UninstallPortMon(mi2) )
	{
		return GetLastError();
	}
#endif
	//return bRet1 && bRet2 && bRet3;
	return true;
}
void CPrinterInstallHelper::DelDriverFile()
{
	char lpDriverPath[MAX_PATH]; 
	char lpBuffer[MAX_PATH + 1] = {0}; 
	DWORD uSize=0; 

	CString szDriverFile;
	CString sDefaultDir;

	sDefaultDir.Format(_T("%s\\%s"),m_sDefaultPath,PTINTER_BIN_DIR);
	WriteLogEx("default path = %s",sDefaultDir);

	BOOL bRet = ::GetPrinterDriverDirectory(NULL,_PrintEnvironment,1,(LPBYTE)lpBuffer,MAX_PATH + 1,&uSize);

	BOOL b = FALSE;
#if 0
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT5.DLL")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PS5UI.DLL")); 
	b = DeleteFile(lpDriverPath);//直接覆盖
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT.HLP")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PS_SCHM.GDL")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT.NTF")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);
#endif

#ifdef _AMD64_
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\gsdll64.dll")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);
#else
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\gsdll32.dll")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);
#endif

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\XabVPrinter.PPD")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\XabVPrinter.dll")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);

	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\XabVPrinter.ini")); 
	b = DeleteFile(lpDriverPath);
	WriteLogEx("DeleteFile (%d) %s",b,lpDriverPath);
}

BOOL CPrinterInstallHelper::DeleteOrRenameFile(CString szFilePath)
{
	if (PathFileExists(szFilePath))
	{
		if (!DeleteFile(szFilePath))
		{
			CString szFilePathBak = szFilePath + ".bak";
			DeleteFile(szFilePathBak);
			return MoveFile(szFilePath, szFilePathBak);
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
}

void CPrinterInstallHelper::CopyDriverFile()
{
	char lpDriverPath[MAX_PATH]; 
	char lpBuffer[MAX_PATH + 1] = {0}; 
	DWORD uSize=0; 

	CString szDriverFile;
	CString sDefaultDir;

	sDefaultDir.Format(_T("%s\\%s"),m_sDefaultPath,PTINTER_BIN_DIR);
	WriteLogEx("default path = %s",sDefaultDir);

	BOOL bRet = ::GetPrinterDriverDirectory(NULL,_PrintEnvironment,1,(LPBYTE)lpBuffer,MAX_PATH + 1,&uSize);

	//-----------
	//szDriverFile.Format("%s\\3\\PSCRIPT5.DLL",lpBuffer);
	//DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT5.DLL")); 
	BOOL b = CopyFile(sDefaultDir+"\\PSCRIPT5.DLL",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);

	//-----------
	//szDriverFile.Format("%s\\3\\PS5UI.DLL",lpBuffer);
	//DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PS5UI.DLL")); 
	b = CopyFile(sDefaultDir+"\\PS5UI.DLL",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);
	
	//-------------------
	//szDriverFile.Format("%s\\3\\PSCRIPT.HLP",lpBuffer);
	//DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT.HLP")); 
	b = CopyFile(sDefaultDir+"\\PSCRIPT.HLP",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);

	//-------------------
	//szDriverFile.Format("%s\\3\\PSCRIPT.NTF",lpBuffer);
	//DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PSCRIPT.NTF")); 
	b = CopyFile(sDefaultDir+"\\PSCRIPT.NTF",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);

	//-------------------
	//szDriverFile.Format("%s\\3\\PS_SCHM.GDL",lpBuffer);
	//DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\PS_SCHM.GDL")); 
	b = CopyFile(sDefaultDir+"\\PS_SCHM.GDL",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);

#ifdef _AMD64_
	//-------------------
	szDriverFile.Format("%s\\3\\gsdll64.dll",lpBuffer);
	DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\gsdll64.dll")); 
	b = CopyFile(sDefaultDir+"\\gsdll64.dll",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);
#else
	//-------------------
	szDriverFile.Format("%s\\3\\gsdll32.dll",lpBuffer);
	DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\gsdll32.dll")); 
	b = CopyFile(sDefaultDir+"\\gsdll32.dll",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);
#endif

	//------------------
	szDriverFile.Format("%s\\3\\XabVPrinter.PPD",lpBuffer);
	DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\XabVPrinter.PPD")); 
	b = CopyFile(sDefaultDir+"\\XabVPrinter.PPD",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);

	//-------------------
	szDriverFile.Format("%s\\3\\XabVPrinter.dll",lpBuffer);
	DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\XabVPrinter.dll")); 
	b = CopyFile(sDefaultDir+"\\XabVPrinter.dll",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);

	//-------------------
	szDriverFile.Format("%s\\3\\XabVPrinter.ini",lpBuffer);
	DeleteOrRenameFile(szDriverFile);
	lpDriverPath[0] = '\0';
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),TEXT("\\XabVPrinter.ini")); 
	b = CopyFile(sDefaultDir+"\\XabVPrinter.ini",lpDriverPath,FALSE);//直接覆盖
	WriteLogEx("copy file from (%d) %s",b,lpDriverPath);
}

void CPrinterInstallHelper::SetDefaultPrinter()
{
	::SetDefaultPrinter(m_szPrinterName);
}

void CPrinterInstallHelper::WriteExePath(CString szFilePath)
{
// 	CRegistry Reg;
// 	CString	szPath;
// 	szPath="SYSTEM\\CurrentControlSet\\Control\\Print\\Printers";		
// 	Reg.SetParam(true);
// 	if(Reg.Open((LPCTSTR)szPath))
// 	{
// 		Reg.Write("ASPrintExePath",szFilePath);
// 		Reg.Close();
// 	}
}

void CPrinterInstallHelper::AddPrinterProcessX()
{
	char lpBuffer[MAX_PATH]; 
	DWORD uSize=0; 

	BOOL bRet = ::GetPrintProcessorDirectory(NULL,_PrintEnvironment,1,(LPBYTE)lpBuffer,MAX_PATH,&uSize);

	CString szProcessPath = lpBuffer;
	szProcessPath += "\\";
	szProcessPath += + m_szProcDll;

	CopyFile(m_sDefaultPath+"\\PrintDrivers\\"+m_szProcDll,szProcessPath,false);

	//bRet = AddPrintProcessor(NULL,_PrintEnvironment,szProcessPath.GetBuffer(szProcessPath.GetLength()),"AsProc");
	//szProcessPath.ReleaseBuffer();

	bRet = AddPrintProcessor(NULL,_PrintEnvironment,m_szProcDll,m_szProcName);//用全路径会错误


	if(!bRet)
	{
		CString szMsg;
		szMsg.Format("AddPrinterProcess----error:%d",GetLastError());
		//AfxMessageBox(szMsg);
	}
}

BOOL CPrinterInstallHelper::DoSharePrinterNT(LPTSTR szPrinterName, LPTSTR szShareName, BOOL bShare)
{
	HANDLE            hPrinter;
	PRINTER_DEFAULTS   pd;
	DWORD            dwNeeded;
	PRINTER_INFO_2      *pi2;

	// Fill in the PRINTER_DEFAULTS struct to get full permissions.
	ZeroMemory( &pd, sizeof(PRINTER_DEFAULTS) );
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	if( ! OpenPrinter( szPrinterName, &hPrinter, &pd ) )
	{
		// OpenPrinter() has failed - bail out.
		return FALSE;
	}
	// See how big a PRINTER_INFO_2 structure is.
	if( ! GetPrinter( hPrinter, 2, NULL, 0, &dwNeeded ) )
	{
		if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
		{
			// GetPrinter() has failed - bail out.
			ClosePrinter( hPrinter );
			return FALSE;
		}
	}
	// Allocate enough memory for a PRINTER_INFO_2 and populate it.
	pi2 = (PRINTER_INFO_2*)malloc( dwNeeded );
	if( pi2 == NULL )
	{
		// malloc() has failed - bail out.
		ClosePrinter( hPrinter );
		return FALSE;
	}
	if( ! GetPrinter( hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded ) )
	{
		// Second call to GetPrinter() has failed - bail out.
		free( pi2 );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// We won't mess with the security on the printer.
	pi2->pSecurityDescriptor = NULL;
	// If you want to share the printer, set the bit and the name.
	if( bShare )
	{
		pi2->pShareName = szShareName;
		pi2->Attributes |= PRINTER_ATTRIBUTE_SHARED;
	}
	else // Otherwise, clear the bit.
	{
		pi2->Attributes = pi2->Attributes & (~PRINTER_ATTRIBUTE_SHARED);
	}
	// Make the change.
	if( ! SetPrinter( hPrinter, 2, (LPBYTE)pi2, 0 ) )
	{
		// SetPrinter() has failed - bail out
		free( pi2 );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Clean up.
	free( pi2 );
	ClosePrinter( hPrinter );
	return TRUE;
}

void CPrinterInstallHelper::DeleteAllJob()
{
	JOB_INFO_2			*pJobStorage = NULL;
	PRINTER_INFO_2		*pPrinterInfo = NULL;

	DWORD	cByteNeeded, nReturned=0, cByteUsed, i;

	HANDLE hPrint;
	PRINTER_DEFAULTS   pd;
	ZeroMemory( &pd, sizeof(PRINTER_DEFAULTS) );
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	if(OpenPrinter(IST_PRINTER_NAME,&hPrint,&pd))
	{
		CString sPrinterInfo(""), sTask;;
		if (!GetPrinter(hPrint, 2, NULL, 0, &cByteNeeded))
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				ClosePrinter(hPrint);
				return ;
			}
		}
		pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
		if (!(pPrinterInfo))
		{
			ClosePrinter(hPrint);
			return ;
		}

		if (!GetPrinter(hPrint, 2, (LPBYTE)pPrinterInfo, cByteNeeded, &cByteUsed))
		{
			free(pPrinterInfo);
			ClosePrinter(hPrint);
			return ;
		} 
		if (pPrinterInfo->cJobs <= 0)
		{
			free(pPrinterInfo);
			ClosePrinter(hPrint);
			return ;
		}
		if (!EnumJobs(hPrint, 
			0, 
			(pPrinterInfo)->cJobs, 
			2, 
			NULL, 
			0,
			(LPDWORD)&cByteNeeded,
			(LPDWORD)&nReturned))
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				free(pPrinterInfo);
				ClosePrinter(hPrint);
				return ;
			}
		}

		pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
		if (!pJobStorage)
		{
			free(pPrinterInfo);
			ClosePrinter(hPrint);
			return ;
		}


		ZeroMemory(pJobStorage, cByteNeeded);


		if (!EnumJobs(hPrint, 
			0, 
			(pPrinterInfo)->cJobs, 
			2, 
			(LPBYTE)pJobStorage, 
			cByteNeeded,
			(LPDWORD)&cByteUsed,
			(LPDWORD)&nReturned))
		{
			free(pPrinterInfo);
			free(pJobStorage);
			ClosePrinter(hPrint);
			return ;
		}

		bool bAllPrinted = true;
		CString sPrinterName;
		WriteLogEx("cJobs=%d,nReturned=%d", pPrinterInfo->cJobs, nReturned);
		for (i = 0; i < nReturned; i++)
		{
			int nTempJobID = pJobStorage[i].JobId;
			DWORD  dwStatus = pJobStorage[i].Status;

			WriteLogEx("当前打印机［%s］作业ID：%d   状态：%d\n",pJobStorage[i].pPrinterName,nTempJobID,dwStatus);

			if(SetJob(hPrint,nTempJobID,0,NULL,JOB_CONTROL_DELETE))
				WriteLogEx("删除JOB:%d 成功",nTempJobID);
			else
				WriteLogEx("删除JOB:%d 失败：%d",nTempJobID,GetLastError());
		}

		free(pPrinterInfo);
		free(pJobStorage);
		ClosePrinter(hPrint);
	}	
	::Sleep(100);
}
void RemoveUsers( ATL::CDacl &pDacl)
{
	for(UINT i = 0; i < pDacl.GetAceCount(); i++)
	{
		ATL::CSid pSid;
		ACCESS_MASK pMask = 0;
		BYTE pType = 0, pFlags = 0;
		/* Get the ith ACL */

		const_cast<ATL::CDacl &>(pDacl).GetAclEntry(i, &pSid, &pMask, &pType, &pFlags);

		CString str(pSid.AccountName());

		if (str.CompareNoCase("Users") == 0 )
		{
			pDacl.RemoveAce(i);
		}
	}
}
BOOL AddObjectAccess(CString strFileName, const CSid &rSid, ACCESS_MASK accessmask,SE_OBJECT_TYPE eType /*= SE_OBJECT_TYPE*/)
{
	ATL::CSecurityDesc OutSecDesc;
	ATL::AtlGetSecurityDescriptor(strFileName, eType, &OutSecDesc);
	OutSecDesc.MakeAbsolute();
	bool pbPresent = 0;

	ATL::CDacl pDacl;
	if (!OutSecDesc.GetDacl(&pDacl, &pbPresent))
		return FALSE;

	RemoveUsers(pDacl);

	if (!pDacl.AddAllowedAce(rSid, accessmask,  CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE))
	{
		//已经把已有的删掉了！
		return FALSE;
	}

	OutSecDesc.SetControl(SE_DACL_AUTO_INHERITED | SE_DACL_PROTECTED, SE_DACL_AUTO_INHERITED);
	ATL::AtlSetDacl(strFileName, eType, pDacl);
	ATL::CSacl pSacl;
	/* We've already set the Dacl. Now set the SACL. */
	OutSecDesc.GetSacl(&pSacl, &pbPresent);
	if(pbPresent)
	{
		ATL::AtlSetSacl(strFileName, eType, pSacl);
	}
	return TRUE;
}

void CPrinterInstallHelper::SetWorkDir()
{
	CRegistryEx reg(2,FALSE);
	if(reg.CreateKey(m_szPrinterKeyPath))
	{
#if 0
		TCHAR szDir[MAX_PATH + 1] = {0};
		GetWindowsDirectory(szDir,MAX_PATH + 1);
		strcat_s(szDir,sizeof(szDir),"\\temp");
		CreateDirectory(szDir,NULL) ;
		strcat_s(szDir,sizeof(szDir),"\\Xab");
		CreateDirectory(szDir,NULL) ;
		reg.Write(XAB_JOB_DIR,szDir);
		WriteLogEx("jobDir = %s",szDir);
		AddObjectAccess(szDir,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);
#else
		CString sTmpPath;
		sTmpPath.Format(_T("%s\\temp"),m_sDefaultPath);
		CreateDirectory(sTmpPath,NULL) ;
		reg.Write(XAB_JOB_DIR,sTmpPath);
		WriteLogEx("jobDir = %s",sTmpPath);
		AddObjectAccess(sTmpPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		CString sExePath;
		sExePath.Format(_T("%s\\iCltPrintHost.exe"),m_sDefaultPath);
		reg.Write(XAB_JOB_EXE,sExePath);
		WriteLogEx("exepath = %s",sExePath);

#endif
	}
	//设置注册表权限
	CString sPath;
	sPath.Format(_T("MACHINE\\%s"),m_szPrinterKeyPath);
	AddObjectAccess(sPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_REGISTRY_KEY);
}

int CPrinterInstallHelper::SetDefaultParam()
{
	//Your printer name
	CString testPrinter(IST_PRINTER_NAME);    

	//The DEVMODE data structure contains information about the initialization and environment of a printer     
	DEVMODE   devmodeIn    = {0};                    
	PDEVMODE  pDevmodeWork = NULL;              

	int n = sizeof(DEVMODE);
	// Get current printer properties
	pDevmodeWork = PrinterUtils::ChangePrinterProperties(testPrinter, &devmodeIn, NULL);

	if( !pDevmodeWork )
	{
		WriteLogEx(_T("Can't get printer settings"));
		return 0;
	}

	/*if (pDevmodeWork->dmFields & DM_ICMMETHOD)
	{
		pDevmodeWork->dmICMMethod = DMICMMETHOD_SYSTEM;
		pDevmodeWork->dmFields |= DM_ICMMETHOD;
	}*/

	if (pDevmodeWork->dmFields & DM_COLOR)
	{
		pDevmodeWork->dmColor = DMCOLOR_MONOCHROME;
		pDevmodeWork->dmFields |= DM_COLOR;
	}


	//Setting printer settings 
	if( !PrinterUtils::SetPrinterSetting(testPrinter, *pDevmodeWork) )
	{
		WriteLogEx(_T("Can't set printer settings"));
	}
	else
	{
		WriteLogEx(_T("SetPrinterSetting Ok"));
	}
	::free(pDevmodeWork);
	return 0;
}

//add by zxl, 20150821,删除Windows图片打印向导配置信息
//在安装了第三方驱动的打印客户端之后，如果重新安装了其它驱动的打印客户端，
//Windows图片打印向导配置信息会是保留原来的驱动配置信息，
//所以卸载驱动时要删除本驱动的配置信息。
BOOL CPrinterInstallHelper::DeletePhotoPrintWizardPram()
{
	//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\PhotoPrintingWizard\XabVPrinter
	BOOL bRet = TRUE;
	do 
	{
		CRegistryEx reg(0, FALSE);
		bRet = reg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\PhotoPrintingWizard");
		if (!bRet)
		{
			WriteLogEx("!!DeletePhotoPrintWizardPram,1, reg.Open fail");
			break;
		}

		bRet = reg.DeleteTree("XabVPrinter");
		if (!bRet)
		{
			WriteLogEx("!!DeletePhotoPrintWizardPram,2, reg.DeleteValue fail");
			break;
		}

		reg.Close();
	} while (FALSE);
	
	return bRet;
}

void CPrinterInstallHelper::SetPrinterDefaultColorMode(WORD wColor)
{
	WriteLogEx("CPrinterInstallHelper::SetPrinterDefaultColorMode,wColor=%d", wColor);
	CRegistryEx reg(2, FALSE);
	if (reg.Open(m_szPrinterKeyPath))
	{
		LPCTSTR pKey = "Default DevMode";
		BYTE* pData = NULL;
		UINT uBytes = 0;
		if (reg.Read(pKey, &pData, &uBytes))
		{
			if (pData || uBytes < sizeof(DEVMODEW))
			{
				DEVMODEW* pDevModeW = (DEVMODEW*)pData;
				pDevModeW->dmColor = wColor;
				if (!reg.Write(pKey, pData, uBytes))
				{
					WriteLogEx("##CPrinterInstallHelper::SetPrinterDefaultColorMode,Write fail,pData=%p,uBytes=%u", pData, uBytes);
				}
				delete[] pData;
			}
			else
			{
				WriteLogEx("##CPrinterInstallHelper::SetPrinterDefaultColorMode,Read fail,pData=%p,uBytes=%u,destsize=%d", pData, uBytes, sizeof(DEVMODEW));
			}
		}
		else
		{
			WriteLogEx("##CPrinterInstallHelper::SetPrinterDefaultColorMode,Read fail,[Default DevMode]");
		}
	}
	else
	{
		WriteLogEx("##CPrinterInstallHelper::SetPrinterDefaultColorMode,Open fail,[%s]", m_szPrinterKeyPath);
	}
}

//---------------
//安装理光驱动
void CPrinterInstallHelper::CopyDriverFileY(CString sCustom)
{
	char lpBuffer[MAX_PATH + 1] = {0}; 
	DWORD uSize=0; 

	CString szDriverFile;
	CString sDefaultDir;

	sDefaultDir.Format(_T("%s\\%s\\%s"),m_sDefaultPath,PTINTER_BIN_DIR,sCustom);
	WriteLogEx("default path = %s",sDefaultDir);

	BOOL bRet = ::GetPrinterDriverDirectory(NULL,_PrintEnvironment,1,(LPBYTE)lpBuffer,MAX_PATH + 1,&uSize);

	CFileFind	finder;				
	CString		sFullPath = _T("");					
	CString		strWildcard;

	sDefaultDir.TrimRight(_T("\\"));

	strWildcard = sDefaultDir + _T("\\*.*");
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
					
		}
		else
		{
			CString sName = finder.GetFileName();
			CString sPath = finder.GetFilePath();
			CString sDest;
			sDest.Format(_T("%s\\3\\%s"),lpBuffer,sName);
			BOOL b = CopyFile(sPath,sDest,FALSE);
			WriteLogEx("copy %s to %s ret = %d",sPath,sDest,b);
			sDest.Format(_T("%s\\%s"),lpBuffer,sName);
			b = CopyFile(sPath,sDest,FALSE);
			WriteLogEx("copy %s to %s ret = %d",sPath,sDest,b);

		}
	}
	finder.Close();	

}
BOOL CPrinterInstallHelper::AddPrinterY()
{
	DWORD  dwValue =0;
	PRINTER_INFO_2 pi2;
	HANDLE pHD;

	ZeroMemory(&pi2, sizeof(PRINTER_INFO_2)); 
	pi2.pServerName= NULL; 
	pi2.pPrinterName = m_szPrinterName; 
	pi2.pPortName = m_szPortName;
	pi2.pDriverName = m_szDriverName;
	pi2.pPrintProcessor = TEXT("WinPrint"); //IST_PRINTER_PROC
	pi2.pDatatype = TEXT("RAW"); 
	pi2.Attributes = PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS|PRINTER_ATTRIBUTE_QUEUED|PRINTER_ATTRIBUTE_DO_COMPLETE_FIRST;

	WriteLogEx("CPrinterInstallHelper::AddPrinterY PrinterName=[%s],PortName=[%s],DriverName=[%s]",pi2.pPrinterName,pi2.pPortName,pi2.pDriverName);
	if((pHD=::AddPrinter(NULL, 2, (LPBYTE)&pi2))==NULL)
	{
		LPVOID lpMsgBuf; 
		int nError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, nError, NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 

		//::MessageBox(NULL,(LPCTSTR)lpMsgBuf,"ERROR",MB_OK|MB_ICONINFORMATION);	
		WriteLogEx("CPrinterInstallHelper::AddPrinterY %s, err=%d",(LPCTSTR)lpMsgBuf, nError);
		LocalFree( lpMsgBuf ); 
		return FALSE;
	}
	ClosePrinter(pHD);

	return TRUE;
}
BOOL CPrinterInstallHelper::AddPrinterDriverY(CString sCustom)
{
	DRIVER_INFO_3 di3; 
	ZeroMemory(&di3, sizeof(DRIVER_INFO_3)); 

	char lpBuffer[MAX_PATH] = {0}; 
	DWORD uSize=0; 

	BOOL b=::GetPrinterDriverDirectory(NULL,_PrintEnvironment,1,(LPBYTE)lpBuffer,MAX_PATH,&uSize);
	WriteLogEx("AddPrinterDriverY %s",lpBuffer);

	char cDriverFileName[MAX_PATH] = {0};
	char cDriverConfigFileName[MAX_PATH] = {0};
	char cDependentFiles[MAX_PATH*10] = {0};
	char cDataFile[MAX_PATH] = {0};
	char cHelpFile[MAX_PATH] = {0};

	if (0 == sCustom.CompareNoCase(TEXT("ricoh_pcl5")))	//理光PCL5驱动
	{
		lstrcpy(cDriverFileName,TEXT("\\RIC55Ic.DLL"));
		lstrcpy(cDriverConfigFileName,TEXT("\\chs55Iu.DLL"));
		lstrcpy(cDataFile,TEXT("\\XabVPrinter.gpd"));
		di3.pDependentFiles = TEXT("chs55Ih.DLL\0RIC55IWM.EXE\0chs55Il.DLL\0RIC55I2K.RCF\0RIC55Ibd.DLL\0"
			"RIC55Ict.DLL\0RIC55Icj.DLL\0RIC55Ib.XPI\0RIC55Ib.INI\0\0");
	}
	else if (0 == sCustom.CompareNoCase(TEXT("ricoh")))	//理光PCL6驱动
	{
		lstrcpy(cDriverFileName,TEXT("\\ricu00gr.dll"));
		lstrcpy(cDriverConfigFileName,TEXT("\\ricu00ui.dll"));
		lstrcpy(cDataFile,TEXT("\\XabVPrinter.gpd"));
		di3.pDependentFiles = TEXT("ricu00ui.irj\0ricu00ui.rdj\0ricu00ui.rcf\0ricu00ug.dll\0ricu00ug.miz\0ricu00ur.dll\0"
			"ricu00ci.dll\0ricu00cd.dll\0ricu00cf.cfz\0ricu00cl.ini\0ricu00cz.dlz\0ricu00cj.dll\0ricu00ct.dll\0ricu00cb.dll\0\0");
	}
	else if (0 == sCustom.CompareNoCase(TEXT("toshiba")))	//东芝PCL驱动
	{
		lstrcpy(cDriverFileName,TEXT("\\UNIDRV.DLL"));
		lstrcpy(cDriverConfigFileName,TEXT("\\UNIDRVUI.DLL"));
#ifdef _AMD64_
		lstrcpy(cDataFile,TEXT("\\eSf6u.gpd"));
		di3.pDependentFiles = TEXT("eSf6ufw.dll\0eSf6ufwdsdk.dll\0eSf6upclxl.dll\0eSf6ups.dll\0eSf6u.dll\0eSf6uui.dll\0"
			"eSf6u.ini\0eSf6u.xml\0eSf6u.ver\0eSf6ucap.xml\0eSf6uval.xml\0eSf6udevSettings.xml\0eSf6upjl.xml\0eSf6ubase.zip\0"
			"eSf6ubase.dll\0eSf6udevicesettings.zip\0eSf6udevicesettings.dll\0eSf6udefpr.xml\0eSfWM.exe\0eSf6uHSP.exe\0"
			"eSfOVL.exe\0eSfpnp.dll\0locale.gpd\0pclxl.dll\0pclxl.gpd\0pcl5eres.dll\0pcl5ures.dll\0pcl4res.dll\0p6font.gpd\0"
			"pjl.gpd\0p6disp.gpd\0stdnames.gpd\0stddtype.gdl\0stdschem.gdl\0stdschmx.gdl\0ttfsub.gpd\0unires.dll\0eSf6usf_builtin.dll\0"
			"eSf6ueffect.zip\0eSf6ueffect.dll\0eSf6uhsp.zip\0eSf6uhsp.dll\0eSf6uimagequality.zip\0eSf6uimagequality.dll\0"
			"eSf6utandemPrint.zip\0eSf6utandemprint.dll\0eSf6upagehandling.zip\0eSf6upagehandling.dll\0eSf6uaddprinter.zip\0"
			"eSf6uaddprinter.dll\0eSf6uonetouch.zip\0eSf6uonetouch.dll\0UNIRES.DLL\0STDNAMES.GPD\0STDDTYPE.GDL\0STDSCHEM.GDL\0STDSCHMX.GDL\0\0");
#else
		lstrcpy(cDataFile,TEXT("\\eSf3u.gpd"));
		di3.pDependentFiles = TEXT("eSf3ufw.dll\0eSf3ufwdsdk.dll\0eSf3upclxl.dll\0eSf3ups.dll\0eSf3u.dll\0eSf3uui.dll\0"
			"eSf3u.ini\0eSf3u.xml\0eSf3u.ver\0eSf3ucap.xml\0eSf3uval.xml\0eSf3udevSettings.xml\0eSf3upjl.xml\0eSf3ubase.zip\0"
			"eSf3ubase.dll\0eSf3udevicesettings.zip\0eSf3udevicesettings.dll\0eSf3udefpr.xml\0eSfWM.exe\0eSf3uHSP.exe\0eSfOVL.exe\0"
			"eSfpnp.dll\0locale.gpd\0pclxl.dll\0pclxl.gpd\0pcl5eres.dll\0pcl5ures.dll\0pcl4res.dll\0p6font.gpd\0pjl.gpd\0"
			"p6disp.gpd\0stdnames.gpd\0stddtype.gdl\0stdschem.gdl\0stdschmx.gdl\0ttfsub.gpd\0unires.dll\0eSf3usf_builtin.dll\0"
			"eSf3ueffect.zip\0eSf3ueffect.dll\0eSf3uhsp.zip\0eSf3uhsp.dll\0eSf3uimagequality.zip\0eSf3uimagequality.dll\0"
			"eSf3utandemPrint.zip\0eSf3utandemprint.dll\0eSf3upagehandling.zip\0eSf3upagehandling.dll\0eSf3uaddprinter.zip\0"
			"eSf3uaddprinter.dll\0eSf3uonetouch.zip\0eSf3uonetouch.dll\0\0");
#endif
	}
	else if (0 == sCustom.CompareNoCase(TEXT("Xerox")))	//施乐PCL6驱动-(3373pcl6)
	{
		lstrcpy(cDriverFileName,TEXT("\\UNIDRV.DLL"));
		lstrcpy(cDriverConfigFileName,TEXT("\\UNIDRVUI.DLL"));
		lstrcpy(cDataFile,TEXT("\\FX6KNAL-1.GPD"));
#ifdef _AMD64_
		di3.pDependentFiles = TEXT("FX6KNAL-1.INI\0FX6KNAL-1.CFG\0FX6KNAL-1.DXDS\0FX6KNAL-1.DXDC\0"
			"FX6KNAL-1.DXDM\0FX6KNALM-1.DXDB\0FX6KNALD-1.DXDB\0FX6KNALU-1.DLL\0"
			"FX6KNALK-1.DLL\0FX6KNALS-1.XRS\0FX6KNALT-1.XRS\0FX6KNALL-1.XRS\0"
			"FX6KNALF-1.XRS\0FX6KNAL-1.CHM\0FX6KNA-1.CDD\0FX6KNA-1.DDD\0"
			"FX6KNAXA5-1.DLL\0FX6KNAADC3-1.DLL\0FX6KNAARL32-1.DLL\0"
			"FX6KNADHB32-1.DLL\0FX6KNAF01-1.EXE\0FX6KNAF01-1.DLL\0FX6KNANIE2-1.DLL\0"
			"FX6KNAJNIE-1.DLL\0fxzddmif.dll\0fxzdmifk.dll\0LOCALE.GPD\0P6DISP.GPD\0P6FONT.GPD\0"
			"PCL4RES.DLL\0PCL5ERES.DLL\0PCL5URES.DLL\0PCLXL.DLL\0PCLXL.GPD\0PJL.GPD\0"
			"STDDTYPE.GDL\0STDNAMES.GPD\0STDSCHEM.GDL\0STDSCHMX.GDL\0TTFSUB.GPD\0"
			"UNIRES.DLL\0FX6KNAL-1.DXDT\0\0");
#else
		di3.pDependentFiles = TEXT("FX6KNAL-1.INI\0FX6KNAL-1.CFG\0FX6KNAL-1.DXDS\0FX6KNAL-1.DXDC\0"
			"FX6KNAL-1.DXDM\0FX6KNALM-1.DXDB\0FX6KNALD-1.DXDB\0FX6KNALU-1.DLL\0"
			"FX6KNALK-1.DLL\0FX6KNALS-1.XRS\0FX6KNALT-1.XRS\0FX6KNALL-1.XRS\0"
			"FX6KNALF-1.XRS\0FX6KNAL-1.CHM\0FX6KNA-1.CDD\0FX6KNA-1.DDD\0"
			"FX6KNAXA5-1.DLL\0FX6KNAADC3-1.DLL\0FX6KNAARL32-1.DLL\0"
			"FX6KNADHB32-1.DLL\0FX6KNAF01-1.EXE\0FX6KNAF01-1.DLL\0FX6KNANIE2-1.DLL\0"
			"FX6KNAJNIE-1.DLL\0fxzddmif.dll\0LOCALE.GPD\0P6DISP.GPD\0P6FONT.GPD\0"
			"PCL4RES.DLL\0PCL5ERES.DLL\0PCL5URES.DLL\0PCLXL.DLL\0PCLXL.GPD\0PJL.GPD\0"
			"STDDTYPE.GDL\0STDNAMES.GPD\0STDSCHEM.GDL\0STDSCHMX.GDL\0TTFSUB.GPD\0"
			"UNIRES.DLL\0FX6KNAL-1.DXDT\0\0");
#endif
	}
	else if (sCustom.GetLength()>0)
	{
		CIniFile iniFile;
		iniFile.m_sPath.Format(_T("%s\\%s\\%s\\data.ini"),m_sDefaultPath,PTINTER_BIN_DIR,sCustom);
		WriteLogEx("data.ini = %s",iniFile.m_sPath);

		lstrcpy(cDriverFileName,"\\"+iniFile.GetVal("data", "DriverFileName", ""));
		lstrcpy(cDriverConfigFileName,"\\"+iniFile.GetVal("data", "DriverConfigFileName", ""));
		lstrcpy(cDataFile,"\\"+iniFile.GetVal("data", "DataFile", ""));
		lstrcpy(cHelpFile,"\\"+iniFile.GetVal("data", "HelpFile", ""));
		CString szDependentFiles = iniFile.GetVal("data", "DependentFiles", "");
		WriteLogEx("szDependentFiles = %s",szDependentFiles);
		CStringArray ary;
		StringSplit(szDependentFiles, &ary, "|", TRUE);
		int nPos = 0;
		for (int i=0; i<ary.GetCount(); i++)
		{
			CString szFile = ary.GetAt(i);
			int nLen = szFile.GetLength();
			if (nLen > 0)
			{
				lstrcat(TEXT(cDependentFiles+nPos),szFile);
				nPos += (nLen + 1);
			}
		}	
	}
	else
	{
		WriteLogEx("!!AddPrinterDriverY fail,no dirver match [%s]",sCustom);
		return FALSE;
	}

	char lpDriverPath[MAX_PATH] = {0}; 
	lstrcpy(TEXT(lpDriverPath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverPath),cDriverFileName); 
	WriteLogEx("driver = %s",lpDriverPath);

	char lpDriverConfigFile[MAX_PATH] = {0}; 
	lstrcpy(TEXT(lpDriverConfigFile),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDriverConfigFile),cDriverConfigFileName); 
	WriteLogEx("driver = %s",lpDriverConfigFile);

	char lpDataFilePath[MAX_PATH] = {0}; 
	lstrcpy(TEXT(lpDataFilePath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpDataFilePath),cDataFile); 
	WriteLogEx("driver = %s",lpDataFilePath);

	char lpHelpFilePath[MAX_PATH] = {0}; 
	lstrcpy(TEXT(lpHelpFilePath),TEXT(lpBuffer)); 
	lstrcat(TEXT(lpHelpFilePath),cHelpFile); 
	WriteLogEx("driver = %s",lpHelpFilePath);

	di3.cVersion = 0x03; 
	di3.pConfigFile =lpDriverConfigFile; 
	di3.pDataFile =lpDataFilePath; 	
	di3.pDependentFiles = cDependentFiles;
	di3.pDriverPath = lpDriverPath; 
	di3.pEnvironment = _PrintEnvironment;
	di3.pHelpFile = lpHelpFilePath;
	di3.pMonitorName =  NULL;//_T("Sample Port Monitor"); 
	di3.pName = m_szDriverName;
	di3.pDefaultDataType = TEXT("RAW");

	if(!::AddPrinterDriver(NULL, 3, (LPBYTE)&di3))
	{
		LPVOID lpMsgBuf; 
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, NULL,(LPTSTR) &lpMsgBuf, 0, NULL ); 
		WriteLogEx("CPrinterInstallHelper::AddPrinterDriverY %s,err=%d",(LPCTSTR)lpMsgBuf, dw);
		LocalFree( lpMsgBuf ); 
		return FALSE;
	}

	return TRUE;
}
BOOL CPrinterInstallHelper::AddPortY()
{
	return AddPortX();
}