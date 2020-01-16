// SaturnDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBHelp.h"
#include "SaturnDBDlg.h"

#include "RegistryEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "SecText.h"
/////////////////////////////////////////////////////////////////////////////
// CSaturnDBDlg dialog


CSaturnDBDlg::CSaturnDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaturnDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaturnDBDlg)
	m_szDBIP = _T("");
	m_szPwd = _T("");
	m_szUID = _T("");
	m_szMsg = _T("");
	m_szDbName = _T("iSec_Print");
	//}}AFX_DATA_INIT
}


void CSaturnDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaturnDBDlg)
	DDX_Text(pDX, IDC_DBIP, m_szDBIP);
	DDX_Text(pDX, IDC_Pwd, m_szPwd);
	DDX_Text(pDX, IDC_UID, m_szUID);
	DDX_Text(pDX, IDC_DBName, m_szDbName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaturnDBDlg, CDialog)
	//{{AFX_MSG_MAP(CSaturnDBDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaturnDBDlg message handlers

void CSaturnDBDlg::OnOK() 
{
	UpdateData();

	CSecText sec;
	WriteReg("ServIP",m_szDBIP);
	WriteReg("DbName",m_szDbName);
	CString sUser,sPwd;

#ifdef ENABLE_PWD_CRY
	CPWDCry cry;
	char* pUser  = cry.CryptPWD(m_szUID.GetBuffer());	// 第一个参数又跌一次，打印一定要传-1！ qilu@2012-2-3 16:35
	sUser.Format("%s",pUser);
	delete pUser;

	char* pPwd  = cry.CryptPWD(m_szPwd.GetBuffer());	// 第一个参数又跌一次，打印一定要传-1！ qilu@2012-2-3 16:35
	sPwd.Format("%s",pPwd);
	delete pPwd;

	WriteReg("username",sUser);
	WriteReg("password",sPwd);
#else
	WriteReg("username",m_szUID);
	WriteReg("password",m_szPwd);
#endif
	
	
	CDialog::OnOK();
}

bool CSaturnDBDlg::WriteReg(char *szName, LPCTSTR szValue)
{
	CRegistryEx reg;
	reg.SetParam();
	reg.CreateKey("Software\\iSecStar\\Print\\database");
	if(reg.m_hKey)
	{
		reg.Write(CString(szName),CString(szValue));
		reg.Close();
	}
	return true;
}

BOOL CSaturnDBDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSecText sec;
	
	CRegistryEx reg;
	reg.SetParam();
	reg.CreateKey("Software\\iSecStar\\Print\\database");
	if(reg.m_hKey)
	{
		reg.Read("ServIP",m_szDBIP);
		reg.Read("username",m_szUID);
		reg.Read("password",m_szPwd);
		reg.Read("DbName",m_szDbName);
		//m_szUID = sec.unsecretstr(m_szUID);
		//m_szPwd = sec.unsecretstr(m_szPwd);

#ifdef ENABLE_PWD_CRY

		char szUser[MAX_PATH] = {0};
		int nSize = MAX_PATH;
		CPWDCry cry;
		cry.UnCryptPWD((char*)m_szUID.GetBuffer(),szUser,nSize);
		m_szUID = szUser;

		char szPwd[MAX_PATH] = {0};
		nSize = MAX_PATH;
		cry.UnCryptPWD((char*)m_szPwd.GetBuffer(),szPwd,nSize);
		m_szPwd = szPwd;
#endif
		reg.Close();
	}


	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSaturnDBDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}
