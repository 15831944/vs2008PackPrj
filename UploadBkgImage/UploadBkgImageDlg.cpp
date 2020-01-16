
// UploadBkgImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UploadBkgImage.h"
#include "UploadBkgImageDlg.h"

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


// CUploadBkgImageDlg 对话框




CUploadBkgImageDlg::CUploadBkgImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadBkgImageDlg::IDD, pParent)
	, m_szFilePath(_T(""))
	, m_nPort(0)
	, m_szPicSize(_T("800*480"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUploadBkgImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UBT_FILEDIR, m_szFilePath);
	DDV_MaxChars(pDX, m_szFilePath, 1024);
	DDX_Control(pDX, IDC_UBT_COMBO, m_CBPicType);
	DDX_Text(pDX, IDC_UBT_PORT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 0, 65535);
	DDX_Control(pDX, IDC_UBT_UPLOAD, m_BtnUpload);
	DDX_Control(pDX, IDC_UBT_IPADDRESS, m_IPCtrl);
	DDX_Text(pDX, IDC_UBT_PICSIZE, m_szPicSize);
}

BEGIN_MESSAGE_MAP(CUploadBkgImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_UBT_OVERVIEW, &CUploadBkgImageDlg::OnBnClickedUbtOverview)
	ON_BN_CLICKED(IDC_UBT_UPLOAD, &CUploadBkgImageDlg::OnBnClickedUbtUpload)
	ON_CBN_SELCHANGE(IDC_UBT_COMBO, &CUploadBkgImageDlg::OnCbnSelchangeUbtCombo)
END_MESSAGE_MAP()


// CUploadBkgImageDlg 消息处理程序

BOOL CUploadBkgImageDlg::OnInitDialog()
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
	m_CBPicType.InsertString(0, _T("刷卡登录首页背景图"));
	m_CBPicType.InsertString(1, _T("刷卡登录首页LOGO"));
	m_CBPicType.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUploadBkgImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUploadBkgImageDlg::OnPaint()
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
HCURSOR CUploadBkgImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUploadBkgImageDlg::OnBnClickedUbtOverview()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
	dlg.SetDefExt(".jpg");
	if(dlg.DoModal()==IDOK)
	{
		CString FileName=dlg.GetFileName();//获取文件名，在打开文件对话框中选择文件
		CString FileExt = dlg.GetFileExt();//文件后缀名
		CString FileFloder = dlg.GetFolderPath();
		//获取文路径及文件名后打开
		//把文件内容输出到列表控件
		m_szFilePath = FileFloder+"\\"+FileName;
		UpdateData(FALSE);
	}
}

void CUploadBkgImageDlg::OnBnClickedUbtUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BYTE nField0=0,nField1=0,nField2=0,nField3=0;
	CString szIP;
	m_IPCtrl.GetAddress(nField0,nField1,nField2,nField3);
	szIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
	CString URL;
	if(m_CBPicType.GetCurSel() == 0)
	{
		//URL.Format("http://%s:%d/webservice/pctool/bgImage", szIP, m_nPort);
		URL.Format("http://%s:8080/webservice/pctool/bgImage", szIP);
	}
	else
	{
		//URL.Format("http://%s:%d/webservice/pctool/logoImage", szIP, m_nPort);
		URL.Format("http://%s:8080/webservice/pctool/logoImage", szIP);
	}
	
	if(UploadPic(URL, m_szFilePath))
	{
		MessageBox("上传成功");
	}
	else
	{
		MessageBox("上传失败");
	}
	UpdateData(FALSE);
}

BOOL CUploadBkgImageDlg::RequestHttp( CString szHttp, CString &szResponse )
{
	//WriteLogEx("CUploadBkgImageDlg::RequestHttp,1,szHttp=[%s].", szHttp);
	//根据内网中经验，第一次经常会出现未知错误
	for(int i = 0; i < 2; ++i)
	{
		string sRequestHttp = szHttp.GetString();	//请求的网页url
		string sHeaderSend;			//定义http头
		string sHeaderReceive;		//返回头
		string sMessage="";			//返回页面内容
		bool IsPost = false;		//是否Post提交

		int iRet = m_pRequest->SendRequest(IsPost, sRequestHttp, sHeaderSend, sHeaderReceive, sMessage);
		if (iRet)
		{
			szResponse = sMessage.c_str();
			// 			WriteLogEx("CDaShiCardTayg::RequestHttp,2, succ. szResponse=[%s].", szResponse);
			return TRUE;
		}
		else
		{
			//WriteLogEx("!!CUploadBkgImageDlg::RequestHttp,3, SendRequest fail ret=%d", iRet);
			continue;
		}
	}
	//WriteLogEx("!!CUploadBkgImageDlg::RequestHttp,4, fail.");
	return FALSE;
}
BOOL CUploadBkgImageDlg::UploadPic(LPCTSTR strURL, LPCTSTR strLocalFileName)
{
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	CString strServer;
	CString strObject;
	INTERNET_PORT wPort = 0;
	DWORD dwFileLength = 0;
	char * pFileBuff = NULL;

	CHttpConnection * pHC = NULL;
	CHttpFile * pHF = NULL;
	CInternetSession cis;

	bResult = AfxParseURL(strURL, dwType, strServer, strObject, wPort);
	if(!bResult)
		return FALSE;
	CFile file;
	try
	{
		if(!file.Open(strLocalFileName, CFile::shareDenyNone | CFile::modeRead))
			return FALSE;
		dwFileLength = file.GetLength();
		if(dwFileLength <= 0)
			return FALSE;
		pFileBuff = new char[dwFileLength];
		memset(pFileBuff, 0, sizeof(char) * dwFileLength);
		file.Read(pFileBuff, dwFileLength);

		const int nTimeOut = 5000;
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //联接超时设置
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); //重试1次
		pHC = cis.GetHttpConnection(strServer, wPort); //取得一个Http联接

		pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
		if(!pHF->SendRequest(NULL, 0, pFileBuff, dwFileLength))
		{
			delete[]pFileBuff;
			pFileBuff = NULL;
			pHF->Close();
			pHC->Close();
			cis.Close();
			return FALSE;
		}
		DWORD dwStateCode = 0;
		pHF->QueryInfoStatusCode(dwStateCode);

		if(dwStateCode == HTTP_STATUS_OK)
			bResult = TRUE;
	}

	catch(CInternetException * pEx)
	{
		char sz[256] = "";
		pEx->GetErrorMessage(sz, 25);
		CString str;
		str.Format("InternetException occur!\r\n%s", sz);
		AfxMessageBox(str);
		bResult = FALSE;
	}
	catch(CFileException& fe)
	{
		CString str;
		str.Format("FileException occur!\r\n%d", fe.m_lOsError);
		AfxMessageBox(str);
		bResult = FALSE;
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		CString str;
		str.Format("Unknow Exception occur!\r\n%d", dwError);
		AfxMessageBox(str);
		bResult = FALSE;
	}

	delete[]pFileBuff;
	pFileBuff = NULL;
	file.Close();
	pHF->Close();
	pHC->Close();
	cis.Close();
	return bResult;
}

void CUploadBkgImageDlg::OnCbnSelchangeUbtCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_CBPicType.GetCurSel() == 0)
	{
		m_szPicSize = "800*480";
	}
	else if(m_CBPicType.GetCurSel() == 1)
	{
		m_szPicSize = "200*100左右";
	}
	UpdateData(FALSE);
}
