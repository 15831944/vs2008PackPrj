// SaturnDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBHelp.h"
#include "OracleDbDlg.h"
#include "RegistryEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COracleDbDlg dialog


COracleDbDlg::COracleDbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COracleDbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COracleDbDlg)
	m_nHourSpan = 1;
	m_szPwd2 = _T("");
	m_szUID2 = _T("");
	m_szMsg = _T("");
	m_szDBOwner = _T("");
	m_szDataSource = _T("");
	//}}AFX_DATA_INIT
}


void COracleDbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COracleDbDlg)
	DDX_Text(pDX, IDC_Frequency, m_nHourSpan);
	DDX_Text(pDX, IDC_Pwd, m_szPwd2);
	DDX_Text(pDX, IDC_UID, m_szUID2);
	DDX_Text(pDX, IDC_DBOwner, m_szDBOwner);
	DDX_Text(pDX, IDC_DataSource, m_szDataSource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COracleDbDlg, CDialog)
	//{{AFX_MSG_MAP(COracleDbDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COracleDbDlg message handlers
bool COracleDbDlg::WriteReg(char* szName,LPCTSTR szValue)
{
	CRegistryEx reg;
	reg.SetParam();
	reg.CreateKey("Software\\iSecStar\\Print");
	if(reg.m_hKey)
	{
		reg.Write(CString(szName),CString(szValue));
		reg.Close();
	}
	return true;
}

void COracleDbDlg::OnOK() 
{
	UpdateData();
	if(m_nHourSpan < 1)
	{
		AfxMessageBox("同步频率必须大于1小时!");
		return ;
	}
	CString szTemp;
	szTemp.Format("%d",m_nHourSpan);
	CString szAutoDep;
	//szAutoDep.Format("%d",m_bAutoDep);
	WriteReg("Oracle_UID",m_szUID2);
	WriteReg("Oracle_PWD",m_szPwd2);
	WriteReg("Oracle_TimeSpan",szTemp);
	WriteReg("Oracle_DBOwner",m_szDBOwner);
	WriteReg("Oracle_DataSource",m_szDataSource);
	WriteReg("Oracle_AutoDep",szAutoDep);
	
	MessageBox("设置成功!","提示",MB_OK | MB_ICONINFORMATION);

	CDialog::OnOK();
}


BOOL COracleDbDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetOracleConfig(m_szUID2,m_szPwd2,m_szDBOwner,m_szDataSource,m_nHourSpan);

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COracleDbDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}
