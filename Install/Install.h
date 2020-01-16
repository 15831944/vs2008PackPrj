
// Install.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CInstallApp:
// 有关此类的实现，请参阅 Install.cpp
//

class CInstallApp : public CWinAppEx
{
public:
	CInstallApp();

	
	void DeleteInstallKey();
	void DeleteInstallKey2();
	void DeleteInstallKey3();

	// 创建key
	void CreateServerKey(LPCTSTR pszData, LPCTSTR pszData2);
	void DeleteServerKey(); 

	int ParseParam(void);
	CString m_sDefaultPath;
	CString m_sInstallSourcePath;
	int m_nExitCode;
	BOOL m_bQueit;
	void SetErrCode(int nCode,CString sErr);
// 重写
	public:
	virtual BOOL InitInstance();

	void IntallDogDrv();
	BOOL EncryptCheck();
	BOOL CopyEnCryptFiles();
	BOOL CopyEnCryptFilesC();
	void DelEncrypFile();
	void InstallSvr(CString sSvcName);
	void UninstallSvr();

	void DealDbFunctionTree();	//删除精简版中的多余web树项
// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

class CDrvInstallHelper
{
private:
	HMODULE m_hModule;
	TCHAR m_szPath[MAX_PATH + 1];
public:
	CDrvInstallHelper()
	{
		m_hModule = NULL;
		int nLen = GetModuleFileName(NULL, m_szPath, MAX_PATH);
		if (nLen == 0)
		{
			ASSERT(0);
		}
		TCHAR* pszDest = strrchr(m_szPath,'\\');
		if (pszDest) 
			*pszDest = '\0';
	}
	~CDrvInstallHelper()
	{
		if(m_hModule)
		{
			::FreeLibrary(m_hModule);
		}
	}

	bool CallFun(TCHAR* szFun);

private:
	BOOL Load();

};

#define PRODUCT_CODE	0
class CProductInfo
{
public:

	static CString GetEduPrintProductName();
	static CString GetPrintProductName();

	//add by zfq,从程序目录中的到产品目录,即含有“打印复印管理系统”的目录
	static BOOL GetProductDirNameInProgramDir(CString &szProgramDir, CString &szProductDir);	
};


class CConfigSettingHelper
{
public:
	CConfigSettingHelper()
	{
		SetCurrentConfig("");
		CString sPath;
		sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR);
		CreateDirectory(sPath,0);
	}
	void LoadConnStr(CString& sDbSvr,CString& sDbName,CString& sDbUser,CString& szDbPort,int& nData )
	{
		sDbSvr = theIni.GetVal("DBConfig","DbSvr",_T(""));
		sDbName = theIni.GetVal("DBConfig","DbName",_T(""));
		sDbUser = theIni.GetVal("DBConfig","DbUser",_T(""));
		//theIni.SetVal("DBConfig","DbPwd",sDbPwd);
		szDbPort = theIni.GetVal("DBConfig","DbPort",_T("1433"));
		nData = theIni.GetVal("DBConfig","DbType",SQLSERVER);
	}


	void SaveConnStr(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort =  _T("1433"),int nData = SQLSERVER)
	{
		theIni.SetVal("DBConfig","DbSvr",sDbSvr);
		theIni.SetVal("DBConfig","DbName",sDbName);
		theIni.SetVal("DBConfig","DbUser",sDbUser);
		
		theIni.SetVal("DBConfig","DbPort",szDbPort);
		theIni.SetVal("DBConfig","DbType",/*nData*/SQLSERVER);
#pragma message("暂时默认为数据库SQLSERVER，后续扩展为其它数据库时，要求修改配置界面")

#ifdef ENABLE_PWD_CRY
		CPWDCry cry;
		CString sPwd;
		char* pPsw  = cry.CryptPWD(sDbPwd.GetBuffer());
		sPwd.Format("%s",pPsw);
		delete pPsw;
		theIni.SetVal("DBConfig","DbPwd",sPwd);
#else
		theIni.SetVal("DBConfig","DbPwd",sDbPwd);
#endif
	}
	

    void LoadEmbedWebConnStr(CString& sDbSvr,CString& sDbName,CString& sDbPwd, CString& sDbUser,CString& szDbPort,int& nData )
    {
        sDbSvr = theIni.GetVal("EmbedWebDbConfig","DbSvr",_T(""));
        sDbName = theIni.GetVal("EmbedWebDbConfig","DbName",_T(""));
        sDbPwd = theIni.GetVal("EmbedWebDbConfig","DbPwd",_T(""));
        sDbUser = theIni.GetVal("EmbedWebDbConfig","DbUser",_T(""));
        szDbPort = theIni.GetVal("EmbedWebDbConfig","DbPort",_T("1433"));
        nData = theIni.GetVal("EmbedWebDbConfig","DbType",SQLSERVER);
    }
    

    void SaveEmbedWebConnStr(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort =  _T("1433"),int nData = SQLSERVER)
    {
        theLog.Write("SaveEmbedWebConnStr,sDbSvr=%s, sDbName=%s, sDbUser=%s, sDbPwd=%s", sDbSvr, sDbName, sDbUser, sDbPwd);
        theIni.SetVal("EmbedWebDbConfig","DbSvr",sDbSvr);
        theIni.SetVal("EmbedWebDbConfig","DbName",sDbName);
        theIni.SetVal("EmbedWebDbConfig","DbUser",sDbUser);
        theIni.SetVal("EmbedWebDbConfig","DbPwd",sDbPwd);
        theIni.SetVal("EmbedWebDbConfig","DbPort",szDbPort);
        theIni.SetVal("EmbedWebDbConfig","DbType",/*nData*/SQLSERVER);
#pragma message("暂时默认为数据库SQLSERVER，后续扩展为其它数据库时，要求修改配置界面")

    }

	void SaveSyxConnStr(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort =  _T("1433"))
	{
		theIni.SetVal("SyxDBConfig","DbSvr",sDbSvr);
		theIni.SetVal("SyxDBConfig","DbName",sDbName);
		theIni.SetVal("SyxDBConfig","DbUser",sDbUser);

		theIni.SetVal("SyxDBConfig","DbPort",szDbPort);
#ifdef ENABLE_PWD_CRY
		CPWDCry cry;
		CString sPwd;
		char* pPsw  = cry.CryptPWD(sDbPwd.GetBuffer());
		sPwd.Format("%s",pPsw);
		delete pPsw;
		theIni.SetVal("SyxDBConfig","DbPwd",sPwd);
#else
		theIni.SetVal("SyxDBConfig","DbPwd",sDbPwd);
#endif
	}

	void SaveClientConfig(CString sIP,CString sPort = _T(""),DWORD dwValue = 0)
	{
		theIni.SetVal("ClientConfig","SvrIP",sIP);
		if (!sPort.IsEmpty())
			theIni.SetVal("ClientConfig","SvrPort",sPort);
		if (dwValue != 0)
			theIni.SetVal("ClientConfig","iSec",dwValue);
	}


	void SaveUpdaterServerConfig(CString sPath,UINT uPort,int nChecked = 1)
	{
		theIni.SetVal("Updater","PatchDir",sPath);
		theIni.SetVal("Updater","CheckKB",nChecked);
		theIni.SetVal("Updater","HttpPort",uPort);

		{
			//保存更新配置信息到注册表中,以便webinf.dll中的接口获取到升级配置信息.
			CRegistryEx pReg;		
			pReg.SetParam(false);
			CString	szPath;
			szPath.Format(_T("%s%s"),PRODUCT_REGISTY_KEY,"Updater");
			pReg.SetParam(false);	
			if(!pReg.Open((LPCTSTR)szPath) )
			{
				pReg.SetParam(false);
				if (!pReg.CreateKey(szPath) ) 
				{
					return;					
				}  	
			}
			pReg.Write("PatchDir", sPath);
			pReg.Write("CheckKB", nChecked);
			pReg.Write("HttpPort", (int)uPort);
			pReg.Close();
		}
	}

	void SaveUpdaterClientConfig(CString sPath,CString sAddr,UINT uPort)
	{
		theIni.SetVal("UpdaterClt","LocalAddr",sPath);
		theIni.SetVal("UpdaterClt","SvrAddr",sAddr);
		theIni.SetVal("UpdaterClt","SvrPort",uPort);
	}

	void SavePrtStationConfig(CString sIP,CString sPort = _T(""))
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}

		theIni.SetVal(sKey,"SvrIP",sIP);
		if (!sPort.IsEmpty())
		{
			theIni.SetVal(sKey,"SvrPort",sPort);
		}
		
	}
	void SaveScanDir(CString& sDir)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"ScanDir",sDir);
	}
	void SaveScanData(CString& sDir)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"ScanDataType",sDir);
	}
	void LoadScanDir(CString& sDir)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		sDir = theIni.GetVal(sKey,"ScanDir","");
	}
	void LoadScanData(CString& sDir)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		sDir = theIni.GetVal(sKey,"ScanDataType","");
	}
	void LoadBindIP(CString& sIP)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		sIP = theIni.GetVal(sKey,"BindIP","");
	}

	void SavePrtYTJ(BOOL bEnable)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"EnableYTJ",bEnable);
	}
	void LoadPrtYTJ(BOOL& b)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		b = theIni.GetVal(sKey,"EnableYTJ",0);
	}
	void SavePrtComIndex(int nIndex)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"ComIndex",nIndex);
	}
	void SavePrtBindIP(CString& s)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"BindIP",s);
	}
	void SaveBrand(int n)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"Brand",n);
	}
	void LoadBrand(int& b)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		b = theIni.GetVal(sKey,"Brand",0);
	}

    void SaveSignalCtrlSet(BOOL boo)
    {
        CString sKey = _T("PrtStationConfig");
        if(IsWithOutPrintStation())
        {
            sKey = _T("PrtCtrlConfig");
        }
        theIni.SetVal(sKey,"SignalCtrl", boo ? 1 : 0);
    }
    void LoadSignalCtrlSet(BOOL &boo)
    {
        CString sKey = _T("PrtStationConfig");
        if(IsWithOutPrintStation())
        {
            sKey = _T("PrtCtrlConfig");
        }
        boo = theIni.GetVal(sKey,"SignalCtrl", 0);
    }
	void SaveSNMPPrintMon(BOOL bEnable)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"SnmpPrintMonitor", bEnable ? 1 : 0);
	}
	void LoadSNMPPrintMon(BOOL &bEnable)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		bEnable = theIni.GetVal(sKey,"SnmpPrintMonitor", 0);
	}
	void SaveSNMPPrinterMeter(BOOL bEnable)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"SnmpPrinterMeterMonitor", bEnable ? 1 : 0);
	}
	void LoadSNMPPrinterMeter(BOOL &bEnable)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		bEnable = theIni.GetVal(sKey,"SnmpPrinterMeterMonitor", TRUE);
	}
	void SetPrtPaperConfig(CString& s)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"PaperConfig",s);
	}
	void LoadPrtPaperConfig(CString& s)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		s = theIni.GetVal(sKey,"PaperConfig","");
	}
	void SetPrtAddr(CString& s)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		theIni.SetVal(sKey,"PrinterAddr",s);
	}
	void LoadPrtAddr(CString& s)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		s = theIni.GetVal(sKey,"PrinterAddr","");
	}
	void LoadPrtComIndex(int& nIndex)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		nIndex = theIni.GetVal(sKey,"ComIndex",1);
	}
	void SaveMgrConfig(CString sIP,CString sPort = _T(""))
	{
		theIni.SetVal("MgrConfig","SvrIP",sIP);
		if (!sPort.IsEmpty())
		{
			theIni.SetVal("MgrConfig","SvrPort",sPort);
		}
	}

	void LoadPrtStationConfig(CString &sIP,CString& sPort)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		sIP = theIni.GetVal(sKey,"SvrIP","");
		sPort = theIni.GetVal(sKey,"SvrPort","");
	}
	BOOL IsWithOutPrintStation()
	{
		CString sTmpPath;
		sTmpPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),"LmPrinterSvcHost.exe");
		if (PathFileExists(sTmpPath))
		{
			return TRUE;
		}
		return FALSE;
	}

	//add by zxl, 20130618
	BOOL IsPrintStationCloud()
	{
		if (!IsWithOutPrintStation())
		{
			CString sTmpPath;
			sTmpPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),"PrintStationCloud.exe");
			if (PathFileExists(sTmpPath))
			{
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

	//===================================================add by zfq,2013.02.16,打印集中管理端,begin
	void LoadPrtCenterSvrConfig(CString &szSvrIP, CString &szSvrPort, CString &szLocalIP)
	{
		CString sKey = _T("PrtCenterSvr");
		szSvrIP = theIni.GetVal(sKey, "SvrIP", "");
		szSvrPort = theIni.GetVal(sKey, "SvrPort", "");
		szLocalIP = theIni.GetVal(sKey, "BindIP", "");
		return;
	}

	void SavePrtCenterSvrConfig(CString &szSvrIP, CString &szSvrPort, CString &szLocalIP)
	{
		CString sKey = _T("PrtCenterSvr");
		if(!szSvrIP.IsEmpty())
		{
			theIni.SetVal(sKey, "SvrIP", szSvrIP);
		}
		if(!szSvrPort.IsEmpty())
		{
			theIni.SetVal(sKey, "SvrPort", szSvrPort);
		}
		if(!szLocalIP.IsEmpty())
		{
			theIni.SetVal(sKey, "BindIP", szLocalIP);
		}
		return;
	}
	//===================================================add by zfq,2013.02.16,打印集中管理端,end
	
	//add by szy 20150421 begin
	void SaveEmbedWebCfg(CString &szTitle,CString &szColor,CString &szSize)
	{
        CString sKey = _T("PrinterEmbedWeb");
        if(!szTitle.IsEmpty())
        {
            theIni.SetVal(sKey, "Title", szTitle);
        }
        if(!szColor.IsEmpty())
        {
            theIni.SetVal(sKey, "Color", szColor);
        }
        if(!szSize.IsEmpty())
        {
            theIni.SetVal(sKey, "Size", szSize);
        }
	}
	void LoadEmbedWebCfg(CString &szTitle,CString &szColor,CString &szSize)
	{
	    CString sKey = _T("PrinterEmbedWeb");
	    szTitle = theIni.GetVal(sKey, "Title", "");
	    szColor = theIni.GetVal(sKey, "Color", "#000000");
	    szSize = theIni.GetVal(sKey, "Size", "40px");
	}
	
	void SaveEmbedDBCfg(int &PrtStationID,int & jump)
	{
        CString sKey = _T("PrinterEmbedWeb");
        if(PrtStationID > 0)
        {
            theIni.SetVal(sKey, "PrtStationID", PrtStationID);
        }
        if(jump > 0)
        {
            theIni.SetVal(sKey, "Jump", jump);
        }
	}
	void LoadEmbedDBCfg(int &PrtStationID,int & jump)
	{
        CString sKey = _T("PrinterEmbedWeb");
        PrtStationID = theIni.GetVal(sKey, "PrtStationID", 1);
        jump = theIni.GetVal(sKey, "Jump", 3000);
	}
	//add by szy 20150421 end

	//--add by zfq,2015-07-22 begin
	void LoadIsEmbed(BOOL &bEmbed)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}
		
		int nRet = theIni.GetVal(sKey, "IsEmbed", 0);
		bEmbed = (0 == nRet) ? FALSE : TRUE;
		return;
	}

	void SaveIsEmbed(BOOL bEmbed)
	{
		CString sKey = _T("PrtStationConfig");
		if(IsWithOutPrintStation())
		{
			sKey = _T("PrtCtrlConfig");
		}

		theIni.SetVal(sKey, "IsEmbed", bEmbed ? 1 : 0);
		return;
	}
	//--add by zfq,2015-07-22 end

	//设置当前配置文件
	void SetCurrentConfig(CString szBindPrinter /*= ""*/)
	{
		if (szBindPrinter.GetLength()>0)
		{
			theIni.m_sPath.Format(_T("%s%s\\config_%s.ini"),CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR,szBindPrinter);
		}
		else
		{
			theIni.m_sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
		}
	}

protected:
	CIniFile theIni;

};
extern CInstallApp theApp;
extern CConfigSettingHelper theConfig;