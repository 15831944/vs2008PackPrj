// SetDbDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "SetDbDlg.h"


// CSetDbDlg 对话框

IMPLEMENT_DYNAMIC(CSetDbDlg, CDialog)

CSetDbDlg::CSetDbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDbDlg::IDD, pParent)
{
	m_szDbName = PRINT_DB_NAME;
	m_szIP = CCommonFun::GetLocalIP();
	m_szPort = _T("1433");
	m_szUserName = _T("sa");
	m_szUserPwd = _T("");
}

CSetDbDlg::~CSetDbDlg()
{
}

void CSetDbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDbDlg)
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_szDbName);
	DDX_Text(pDX, IDC_EDIT_IP, m_szIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_szPort);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_szUserName);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_szUserPwd);
	DDX_Control(pDX,IDC_COMBO_DBTYPE,m_wndCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDbDlg, CDialog)
	ON_BN_CLICKED(IDC_TEST, &CSetDbDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDOK, &CSetDbDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DBTYPE, &CSetDbDlg::OnSelchangeComboDbType)
END_MESSAGE_MAP()

void CSetDbDlg::OnSelchangeComboDbType()
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

// CSetDbDlg 消息处理程序
BOOL CSetDbDlg::TestConnect()
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

void CSetDbDlg::OnBnClickedTest()
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


void CSetDbDlg::OnBnClickedOk()
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

	if(!TestConnect())
	{
		int nRet = MessageBox("连接数据库失败，是否要保存输入的数据库连接信息？","提示",MB_YESNO | MB_ICONINFORMATION);
		if (nRet == IDNO)
		{
			return ;
		}
	}
	int nSel = m_wndCmb.GetCurSel();
	int nData = m_wndCmb.GetItemData(nSel);
	theConfig.SaveConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd,m_szPort,nData);

	OnOK();
}

BOOL CSetDbDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	// 异常: OCX 属性页应返回 FALSE
}
