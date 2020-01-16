
// InstallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "InstallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CInstallDlg 对话框




CInstallDlg::CInstallDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInstallDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CInstallDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInstallDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInstallDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CInstallDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CInstallDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CInstallDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON10, &CInstallDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CInstallDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CInstallDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CInstallDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CInstallDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CInstallDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CInstallDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CInstallDlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CInstallDlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CInstallDlg::OnBnClickedButton19)
END_MESSAGE_MAP()


// CInstallDlg 消息处理程序

BOOL CInstallDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInstallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInstallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInstallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInstallDlg::OnBnClickedButton1()
{
	CDrvInstallHelper helper;
	helper.CallFun("InstallNdis");
}

void CInstallDlg::OnBnClickedButton2()
{
	CDrvInstallHelper helper;
	helper.CallFun("UnInstallNdis");
}

void CInstallDlg::OnBnClickedButton3()
{
	CDrvInstallHelper helper;
	helper.CallFun("InstallPrinter");

}

void CInstallDlg::OnBnClickedButton4()
{

	CDrvInstallHelper helper;
	helper.CallFun("UnInstallPrinter");
}

void CInstallDlg::OnBnClickedButton5()
{
	CDrvInstallHelper helper;
	helper.CallFun("InstallMirrDisp");
}

void CInstallDlg::OnBnClickedButton6()
{
	CDrvInstallHelper helper;
	helper.CallFun("UnInstallMirrDisp");
}

void CInstallDlg::OnBnClickedButton10()
{
	CDrvInstallHelper helper;
	helper.CallFun("InstallDeviceFilter");
}

void CInstallDlg::OnBnClickedButton11()
{
	CDrvInstallHelper helper;
	helper.CallFun("UnInstallDeviceFilter");
}

void CInstallDlg::OnBnClickedButton12()
{
	CDrvInstallHelper helper;
	helper.CallFun("InstallFileFilter");
}

void CInstallDlg::OnBnClickedButton13()
{
	CDrvInstallHelper helper;
	helper.CallFun("UnInstallFileFilter");
}

void CInstallDlg::OnBnClickedButton14()
{
	CDbInstaller db;
//	db.InstallDbBySqlFile("192.168.2.12","iSec_Platform","sa","sa","1433","e:\\iSec_Platform.sql");
}

void CInstallDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CInstallDlg::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
}

LPDEVMODE GetLandscapeDevMode(HWND hWnd, char *pDevice)
{

	HANDLE      hPrinter;
	LPDEVMODE   pDevMode;
	DWORD       dwNeeded, dwRet;

	/* Start by opening the printer */ 
	if (!OpenPrinter(pDevice, &hPrinter, NULL))
		return NULL;

	/*
	* Step 1:
	* Allocate a buffer of the correct size.
	*/ 
	dwNeeded = DocumentProperties(hWnd,
		hPrinter,       /* Handle to our printer. */ 
		pDevice,        /* Name of the printer. */ 
		NULL,           /* Asking for size, so */ 
		NULL,           /* these are not used. */ 
		0);             /* Zero returns buffer size. */ 
	pDevMode = (LPDEVMODE)malloc(dwNeeded);

	/*
	* Step 2:
	* Get the default DevMode for the printer and
	* modify it for your needs.
	*/ 
	dwRet = DocumentProperties(hWnd,
		hPrinter,
		pDevice,
		pDevMode,       /* The address of the buffer to fill. */ 
		NULL,           /* Not using the input buffer. */ 
		DM_OUT_BUFFER); /* Have the output buffer filled. */ 
	if (dwRet != IDOK)
	{
		/* If failure, cleanup and return failure. */ 
		free(pDevMode);
		ClosePrinter(hPrinter);
		return NULL;
	}

	/*
	* Make changes to the DevMode which are supported.
	*/ 
	if (pDevMode->dmFields & DM_ORIENTATION)
	{
		/* If the printer supports paper orientation, set it.*/ 
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	}

	if (pDevMode->dmFields & DM_DUPLEX)
	{
		/* If it supports duplex printing, use it. */ 
		pDevMode->dmDuplex = DMDUP_HORIZONTAL;
	}
	if (pDevMode->dmFields & DM_ICMMETHOD)
	{
		//pDevMode->dmICMMethod = ;
	}
	if (pDevMode->dmFields & DM_ICMINTENT)
	{
		pDevMode->dmICMIntent = DMICM_COLORIMETRIC;
	}
	pDevMode->dmColor = DMCOLOR_MONOCHROME;
	/*
	* Step 3:
	* Merge the new settings with the old.
	* This gives the driver an opportunity to update any private
	* portions of the DevMode structure.
	*/ 
	dwRet = DocumentProperties(hWnd,
		hPrinter,
		pDevice,
		pDevMode,       /* Reuse our buffer for output. */ 
		pDevMode,       /* Pass the driver our changes. */ 
		DM_IN_BUFFER |  /* Commands to Merge our changes and */ 
		DM_OUT_BUFFER); /* write the result. */ 

	/* Finished with the printer */ 
	ClosePrinter(hPrinter);

	if (dwRet != IDOK)
	{
		/* If failure, cleanup and return failure. */ 
		free(pDevMode);
		return NULL;
	}

	/* Return the modified DevMode structure. */ 
	return pDevMode;

}


void CInstallDlg::OnBnClickedButton17()
{
	DMICMMETHOD_SYSTEM;
	DM_INTERLACED;
#if 1
	HANDLE h = 0;
	OpenPrinter("XabVPrinter",&h,0);
	BYTE szBuf[4096] = {0};
	DWORD dwSize = 4096;
	DWORD dwOut = 0;
	GetPrinter(h,2,szBuf,dwSize,&dwOut);
	PRINTER_INFO_2* pInfo = (PRINTER_INFO_2*)szBuf;
	LPDEVMODE pIn = (LPDEVMODE)pInfo->pDevMode;
	BYTE szOut[4096] = {0};
	LPDEVMODE pOut = (LPDEVMODE)pIn;
	//CopyMemory(pOut,)
//	AdvancedDocumentProperties(m_hWnd,h,"XabVPrinter",pOut,pIn);
	PRINTER_INFO_8 p8;
	p8.pDevMode = pOut;
	DMICMMETHOD_SYSTEM;
	if (pOut->dmFields & DM_ICMMETHOD)
	{
		pOut->dmICMIntent = DMICMMETHOD_DRIVER;
	}
	
	
	SetPrinter(h,8,(BYTE*)&p8,0);
	ClosePrinter(h);
#endif
	//GetLandscapeDevMode(m_hWnd,"XabVPrinter");
}

void CInstallDlg::OnBnClickedButton18()
{
	CIISWeb web;
	CString sPath;
	web.GetWebSitePath(1,sPath);
	
	AfxMessageBox(sPath);
}


#include <atlsecurity.h>
void CInstallDlg::OnBnClickedButton19()
{

}
