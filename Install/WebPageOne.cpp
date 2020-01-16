// WebPageOne.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "WebPageOne.h"


// CWebPageOne 对话框

IMPLEMENT_DYNAMIC(CWebPageOne, CPropertyPage)

CWebPageOne::CWebPageOne()
	: CPropertyPage(CWebPageOne::IDD)
{
	//{{AFX_DATA_INIT(CWebPageOne)
    m_szDbName = PRINT_DB_NAME;
    m_szIP = CCommonFun::GetLocalIP();
    m_szPort = _T("1433");
    m_szUserName = _T("sa");
    m_szUserPwd = _T("");
    //}}AFX_DATA_INIT
    m_nDbType = 1;
}

CWebPageOne::~CWebPageOne()
{
}

void CWebPageOne::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebPageOne)
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_szDbName);
	DDX_Text(pDX, IDC_EDIT_IP, m_szIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_szPort);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_szUserName);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_szUserPwd);
	DDX_Control(pDX,IDC_COMBO_DBTYPE,m_wndCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebPageOne, CPropertyPage)
	//{{AFX_MSG_MAP(CDialogPageOne)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_CBN_SELCHANGE(IDC_COMBO_DBTYPE, &CWebPageOne::OnSelchangeComboDbType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CWebPageOne 消息处理程序
BOOL CWebPageOne::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CPropertySheet *pMyWizard = (CPropertySheet *)GetParent();
	pMyWizard->SetWindowText("数据库连接设置");
	// TODO: Add extra initialization here
	//theConfig.LoadConnStr(m_szIP,m_szDbName,m_szUserName,m_szPort);

	theConfig.LoadConnStr(m_szIP,m_szDbName,m_szUserName,m_szPort,m_nDbType);
	if(m_szDbName.IsEmpty())
	{
		m_szDbName = PRINT_DB_NAME;
	}
	CString sPort;
	int nSel = 0;
	if(CDbInstaller::GetMsdeListenPort("iSecStar",sPort))
	{
		int n = m_wndCmb.AddString("桌面数据库（msde）");
		m_wndCmb.SetItemData(n,0);
		if (m_nDbType == 0)
		{
			nSel = n;
		}
	}
	int n = m_wndCmb.AddString("Microsoft SQL Server 2000及以后");
	m_wndCmb.SetItemData(n,1);
	if (m_nDbType == 1)
	{
		nSel = n;
	}

	m_wndCmb.SetCurSel(nSel);
	OnSelchangeComboDbType();


	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CWebPageOne::OnSelchangeComboDbType()
{
	int nSel = m_wndCmb.GetCurSel();
	int nData = m_wndCmb.GetItemData(nSel);
	if (nData == 0)
	{
		m_szIP = CCommonFun::GetLocalIP();
		m_szPort;
		CDbInstaller::GetMsdeListenPort("iSecStar",m_szPort);
		m_szUserName = "sa";
		m_szUserPwd = "sa";
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_USERPWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
	}
	else
	{
		m_szPort = "1433";
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USERPWD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CWebPageOne::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet *pMyWizard = (CPropertySheet *)GetParent();
	if (pMyWizard->GetPageCount() > 1)
	{
		pMyWizard->SetWizardButtons(PSWIZB_NEXT);
	}
	else
		pMyWizard->SetWizardButtons(PSWIZB_FINISH);
	


	CWnd *pWnd = pMyWizard->GetDlgItem(ID_WIZBACK);		//将上一步按钮隐藏掉
	if(NULL != pWnd)
		pWnd->ShowWindow(SW_HIDE);

	return CPropertyPage::OnSetActive();
}

LRESULT CWebPageOne::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class

	theLog.Write("CWebPageOne::OnWizardNext,1");

	if(!WriteDbSet())
	{
		theLog.Write("!!CWebPageOne::OnWizardNext,2,WriteDbSet fail");
		return 1;
	}

	theLog.Write("CWebPageOne::OnWizardNext,3");

	return CPropertyPage::OnWizardNext();
}

BOOL CWebPageOne::WriteDbSet()
{
	if(!UpdateData(TRUE))
		return FALSE;

	if(m_szIP == "")
	{
		MessageBox("数据库IP地址不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	if(m_szUserName == "")
	{
		MessageBox("数据库用户名不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	if(m_szPort == "")
	{
		MessageBox("连接端口不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	if(!TestConnect())
	{
		MessageBox("连接数据库失败，请确认输入的数据库连接信息是否正确?","提示",MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	theLog.Write("CWebPageOne::WriteDbSet,1");

	theConfig.SaveConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd);

	theLog.Write("CWebPageOne::WriteDbSet,2");

	CString szMsg;
	szMsg.Format("Data Source=%s,%s;Initial Catalog=%s;User ID=%s;Password=%s",
		m_szIP,m_szPort,m_szDbName,m_szUserName,m_szUserPwd);

	theLog.Write("CWebPageOne::WriteDbSet,3,szMsg=%s", szMsg);

	HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
	typedef void (* PFSetDoNetDBStr)(char* pStr,int nSize);
	PFSetDoNetDBStr fSetDoNetDBStr = NULL;

	theLog.Write("CWebPageOne::WriteDbSet,4,hModule=%x,Err=%d", hModule, GetLastError());

	if(hModule)
	{
		fSetDoNetDBStr = 
			(PFSetDoNetDBStr)::GetProcAddress(hModule,"SetDoNetDBStr");

		theLog.Write("CWebPageOne::WriteDbSet,5,fSetDoNetDBStr=%x", fSetDoNetDBStr);

		if (fSetDoNetDBStr)
		{
			fSetDoNetDBStr(szMsg.GetBuffer(), szMsg.GetLength());
			szMsg.ReleaseBuffer();
		}

		theLog.Write("CWebPageOne::WriteDbSet,6");

		BOOL BRet = ::FreeLibrary(hModule);

		theLog.Write("CWebPageOne::WriteDbSet,7,BRet=%d,err=%d", BRet, GetLastError());
	}

	theLog.Write("CWebPageOne::WriteDbSet,8");

	int nSel = m_wndCmb.GetCurSel();
	int nData = m_wndCmb.GetItemData(nSel);

	theLog.Write("CWebPageOne::WriteDbSet,9");

	theConfig.SaveConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd,m_szPort,nData);

	theLog.Write("CWebPageOne::WriteDbSet,10");

	return TRUE;
}

BOOL CWebPageOne::TestConnect()
{
	if(!UpdateData(TRUE))
		return FALSE;

	BOOL bRet = TRUE;

	CoInitialize(0);
	CAdo ado;


	ado.SetConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd,m_szPort);
	bRet = ado.Connect();


	CoUninitialize();

	return bRet;
}

void CWebPageOne::OnTest() 
{
	if(!UpdateData(TRUE))
		return ;
	if(m_szIP == "")
	{
		MessageBox("数据库IP地址不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if(m_szUserName == "")
	{
		MessageBox("数据库用户名不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if(m_szPort == "")
	{
		MessageBox("连接端口不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if(TestConnect())
		MessageBox("连接数据库成功！","提示",MB_OK | MB_ICONINFORMATION);
	else
		MessageBox("连接数据库失败！","提示",MB_OK | MB_ICONINFORMATION);
}

BOOL CWebPageOne::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!WriteDbSet())
		return FALSE;

	return CPropertyPage::OnWizardFinish();
}
