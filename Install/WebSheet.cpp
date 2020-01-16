// WebSheet.cpp : implementation file
//

#include "stdafx.h"
#include "WebSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebSheet

IMPLEMENT_DYNAMIC(CWebSheet, CPropertySheet)

CWebSheet::CWebSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&page1);
	AddPage(&page2);
	m_bSuc = FALSE;

	m_psh.dwFlags &= ~(PSH_HASHELP);
	m_psh.dwFlags |= PSH_WIZARD;

	page1.m_psp.dwFlags &= ~(PSP_HASHELP);
	page2.m_psp.dwFlags &= ~(PSP_HASHELP);
}

CWebSheet::CWebSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&page1);
	AddPage(&page2);
	m_bSuc = FALSE;

	m_psh.dwFlags &= ~(PSH_HASHELP);
	m_psh.dwFlags |= PSH_WIZARD;

	page1.m_psp.dwFlags &= ~(PSP_HASHELP);
	page2.m_psp.dwFlags &= ~(PSP_HASHELP);
}

CWebSheet::CWebSheet( UINT iSelectPage,LPCTSTR pszCaption)
:CPropertySheet(pszCaption, NULL, 0)
{
	m_bSuc = FALSE;
	if (iSelectPage == 0)
	{
		AddPage(&page1);
		page1.m_psp.dwFlags &= ~(PSP_HASHELP);
	}
	else if (iSelectPage == 1)
	{
		AddPage(&page2);
		page2.m_psp.dwFlags &= ~(PSP_HASHELP);
	}
	else if (iSelectPage == 2)
	{
		AddPage(&page3);
		page3.m_psp.dwFlags &= ~(PSP_HASHELP);
	}
	else if (iSelectPage == 3)
	{
		AddPage(&page1);
		AddPage(&page2);
		AddPage(&page3);
		page1.m_psp.dwFlags &= ~(PSP_HASHELP);
		page2.m_psp.dwFlags &= ~(PSP_HASHELP);
		page3.m_psp.dwFlags &= ~(PSP_HASHELP);
	}
	//AddPage(&page2);

	m_psh.dwFlags &= ~(PSH_HASHELP);
	m_psh.dwFlags |= PSH_WIZARD;
}

CWebSheet::~CWebSheet()
{
}


BEGIN_MESSAGE_MAP(CWebSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CWebSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebSheet message handlers
void CWebSheet::WriteTheURL(BOOL bQueit,CString szSerIp/* = ""*/, CString szSerPort/* = "80"*/)
{
	CString szSql;
	CString sConnStr;
	CoInitialize(0);
	CAdo ado;
	if(bQueit)
	{
		HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
		typedef void (* PFGetDoNetDBStr)(char* pStr,int nSize);
		PFGetDoNetDBStr fGetDoNetDBStr = NULL;
		if(hModule)
		{
			fGetDoNetDBStr = 
				(PFGetDoNetDBStr)::GetProcAddress(hModule,"GetDoNetDBStr");

			char szMsg[MAX_PATH + 1] = {0};
			if (fGetDoNetDBStr)
				fGetDoNetDBStr(szMsg, MAX_PATH);

			sConnStr.Format(_T("Provider=sqloledb; %s"),szMsg);
			ado.SetConnStr(sConnStr);
			::FreeLibrary(hModule);
		}
	}
	else
	{
		CString szIP = page1.m_szIP;
		CString szDbName = page1.m_szDbName;
		CString szUserName = page1.m_szUserName;
		CString szUserPwd = page1.m_szUserPwd;
		CString szPort = page1.m_szPort;

		theLog.Write("WriteTheURL---szIP:%s---szDbName:%s---szUserName:%s---szUserPwd:%s---szPort:%s",
			szIP,szDbName,szUserName,szUserPwd,szPort);
		ado.SetConnStr(szIP,szDbName,szUserName,szUserPwd,szPort);
	}
	
	if (ado.Connect())
	{
		//添加扫描默认目录
		CString sHttpScanDir;
		CString sScanDir;
		CString sDownload;
		CString sPath = CCommonFun::GetDefaultPath();
		sPath.MakeLower();
		int nFind = sPath.Find("bin");
		if (nFind > 0)
		{
			sScanDir.Format(_T("%s%s"),sPath.Left(nFind),SCAN_DIR);
			sDownload.Format(_T("%s%s"),sPath.Left(nFind),"down");
		}
		CCommonFun::CreateDir(sScanDir);
		AddObjectAccess(sScanDir,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);
		
		CCommonFun::CreateDir(sDownload);
		AddObjectAccess(sDownload,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		if (szSerIp.IsEmpty())
		{
			szSerIp = CCommonFun::GetLocalIP();
		}

		sHttpScanDir.Format(_T("http://%s:%s/%s/%s"),szSerIp,szSerPort,VIRTUALNAME,SCAN_DIR);
		szSql.Format(" DELETE FROM t_s_Systemset WHERE SetName = 'ScanDir' "
			" INSERT INTO t_s_Systemset (SetName,SetValue1,SetValue2) VALUES('ScanDir','%s','%s') ",sScanDir,sHttpScanDir);

		if(!ado.ExecuteSQL(szSql))
		{
			theLog.Write("初始化scandir失败！");
		}
		theLog.Write(szSql);

		//添加打印目录
		CString sHttpPrintDir;
		CString sPrintDir;
		nFind = sPath.Find("bin");
		if (nFind > 0)
		{
			sPrintDir.Format(_T("%s%s"),sPath.Left(nFind),PRINT_DIR);
		}
		CCommonFun::CreateDir(sPrintDir);
		AddObjectAccess(sPrintDir,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		sHttpPrintDir.Format(_T("http://%s:%s/%s/%s"),szSerIp,szSerPort,VIRTUALNAME,PRINT_DIR);
		szSql.Format(" DELETE FROM t_s_Systemset WHERE SetName = 'PrintDir' "
			" INSERT INTO t_s_Systemset (SetName,SetValue1,SetValue2) VALUES('PrintDir','%s','%s') ",sPrintDir,sHttpPrintDir);

		if(!ado.ExecuteSQL(szSql))
		{
			theLog.Write("初始化sPrintDir失败！");
		}
		theLog.Write(szSql);

		//添加成绩打印图片目录
		CString sHttpTranscriptDir;
		CString sTranscriptDir;
		nFind = sPath.Find("bin");
		if (nFind > 0)
		{
			sTranscriptDir.Format(_T("%s%s"),sPath.Left(nFind),TRANSCRIPT_DIR);
		}
		CCommonFun::CreateDir(sTranscriptDir);
		AddObjectAccess(sTranscriptDir,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		sHttpTranscriptDir.Format(_T("http://%s:%s/%s/%s"),szSerIp,szSerPort,VIRTUALNAME,TRANSCRIPT_DIR);
		szSql.Format(" DELETE FROM t_s_Systemset WHERE SetName = 'TranscriptDir' "
			" INSERT INTO t_s_Systemset (SetName,SetValue1,SetValue2) VALUES('TranscriptDir','%s','%s') ",sTranscriptDir,sHttpTranscriptDir);

		if(!ado.ExecuteSQL(szSql))
		{
			theLog.Write("初始化sTranscriptDir失败！");
		}
		theLog.Write(szSql);

		CString sHttpSnapshootURL;
		sHttpSnapshootURL.Format(_T("http://%s:%s/%s/ShowImg.aspx"),szSerIp,szSerPort,VIRTUALNAME);	//http://192.168.4.142/XabPrint/ShowImg.aspx
		szSql.Format(" DELETE FROM t_s_Systemset WHERE SetName = 'PaperScanCertification' "
			" INSERT INTO t_s_Systemset (SetName,SetValue1,SetValue2,Description) VALUES('PaperScanCertification','180','%s','%s') "
			,sHttpSnapshootURL, "说明：SetValue1=扫码验证的打印快照保存天数，默认180天；SetVale2=扫码验证二维码BaseUrl；");

		if(!ado.ExecuteSQL(szSql))
		{
			theLog.Write("初始化sHttpSnapshootURL失败！");
		}
		theLog.Write(szSql);

		HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
		typedef void (* PFNotifyFromWeb)(int);
		PFNotifyFromWeb pf = NULL;
		if(hModule)
		{
			pf = (PFNotifyFromWeb)::GetProcAddress(hModule,"NotifyFromWeb");

			char szMsg[MAX_PATH + 1] = {0};
			if (pf)
				pf(0);

			::FreeLibrary(hModule);
		}
	}

	CoUninitialize();
	
}

void CWebSheet::WriteTheFtp(BOOL bQueit)
{
	CString szSql;
	CString sConnStr;
	CoInitialize(0);
	CAdo ado;
	if(bQueit)
	{
		HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
		typedef void (* PFGetDoNetDBStr)(char* pStr,int nSize);
		PFGetDoNetDBStr fGetDoNetDBStr = NULL;
		if(hModule)
		{
			fGetDoNetDBStr = 
				(PFGetDoNetDBStr)::GetProcAddress(hModule,"GetDoNetDBStr");

			char szMsg[MAX_PATH + 1] = {0};
			if (fGetDoNetDBStr)
				fGetDoNetDBStr(szMsg, MAX_PATH);

			sConnStr.Format(_T("Provider=sqloledb; %s"),szMsg);
			ado.SetConnStr(sConnStr);
			::FreeLibrary(hModule);
		}
	}
	else
	{
		CString szIP = page1.m_szIP;
		CString szDbName = page1.m_szDbName;
		CString szUserName = page1.m_szUserName;
		CString szUserPwd = page1.m_szUserPwd;
		CString szPort = page1.m_szPort;

		theLog.Write("WriteTheURL---szIP:%s---szDbName:%s---szUserName:%s---szUserPwd:%s---szPort:%s",
			szIP,szDbName,szUserName,szUserPwd,szPort);
		ado.SetConnStr(szIP,szDbName,szUserName,szUserPwd,szPort);
	}

	if (ado.Connect())
	{
		//添加扫描默认目录
		CString sHttpScanDir;
		CString sScanDir;
		CString sPath = CCommonFun::GetDefaultPath();
		sPath.MakeLower();
		int nFind = sPath.Find("bin");
		if (nFind > 0)
		{
			sScanDir.Format(_T("%s%s"),sPath.Left(nFind),SCAN_DIR);
		}
		CCommonFun::CreateDir(sScanDir);

		sHttpScanDir.Format(_T("http://%s/%s/%s"),CCommonFun::GetLocalIP(),VIRTUALNAME,SCAN_DIR);
		szSql.Format(" DELETE FROM t_s_Systemset WHERE SetName = 'Ftp' "
			" INSERT INTO t_s_Systemset (SetName,SetValue1,SetValue2) VALUES('ftp','%s','%s') ",FTP_USERNAME,FTP_PASSWORD);

		if(!ado.ExecuteSQL(szSql))
		{
			theLog.Write("初始化scandir失败！");
		}
		theLog.Write(szSql);

		HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
		typedef void (* PFNotifyFromWeb)(int);
		PFNotifyFromWeb pf = NULL;
		if(hModule)
		{
			pf = (PFNotifyFromWeb)::GetProcAddress(hModule,"NotifyFromWeb");

			char szMsg[MAX_PATH + 1] = {0};
			if (pf)
				pf(1);

			::FreeLibrary(hModule);
		}

	}

	CoUninitialize();
}