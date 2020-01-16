// RestorePage.cpp : 实现文件
#include "stdafx.h"
#include "DbBackupConfig.h"
#include "RestorePage.h"

// CRestorePage 对话框
IMPLEMENT_DYNAMIC(CRestorePage, CPropertyPage)
CRestorePage::CRestorePage()
	: CPropertyPage(CRestorePage::IDD)
{

}

CRestorePage::~CRestorePage()
{
}

void CRestorePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRestorePage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CRestorePage::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CRestorePage::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CRestorePage::OnBnClickedOk2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRestorePage::OnBnClickedButton3)
END_MESSAGE_MAP()


// CRestorePage 消息处理程序
void CRestorePage::OnBnClickedButton1()
{
	CFileDialog dlgsearch(true,LoadS(IDS_STRING_Path),NULL,OFN_READONLY,"*.bak|*.bak||");
	if(dlgsearch.DoModal() != IDOK)
		return;

	SetDlgItemText(IDC_EDIT4,dlgsearch.GetPathName());
}

void CRestorePage::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT9,m_sDBServerName);
	GetDlgItemText(IDC_EDIT1,m_sDBName);
	GetDlgItemText(IDC_EDIT5,m_sUserAccount);
	GetDlgItemText(IDC_EDIT6,m_sUserPsw);
	GetDlgItemText(IDC_EDIT4,m_sRestoreDBPath);
	if (m_sDBServerName.IsEmpty() || m_sDBName.IsEmpty() || m_sRestoreDBPath.IsEmpty())
	{
		MessageBox(_T(LoadS(IDS_STRING_Text1)),_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
		if (m_sDBServerName.IsEmpty())
			GetDlgItem(IDC_EDIT9)->SetFocus();
		if (m_sDBName.IsEmpty())
			GetDlgItem(IDC_EDIT1)->SetFocus();
		if (m_sRestoreDBPath.IsEmpty())
			GetDlgItem(IDC_BUTTON1)->SetFocus();
		return;
	}
	else if (PathIsDirectory(m_sRestoreDBPath))
	{
		MessageBox(_T(LoadS(IDS_STRING_Text2)),_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_BUTTON1)->SetFocus();
		return;
	}

	if (MessageBox(_T(LoadS(IDS_STRING_Text3)),_T(LoadS(IDS_STRING_Tip)),MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	CDbInstaller dbins;
	if (m_sInstanceName.IsEmpty())
	{
		if (m_sPort.IsEmpty())
			dbins.GetDefaultPort(m_sPort);
		dbins.RestoreDBFromFile(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw,m_sPort,m_sRestoreDBPath);
	}
	else
	{
		dbins.RestoreDBFromFile2(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw,m_sInstanceName,m_sRestoreDBPath);
	}
}

BOOL CRestorePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CDBBackFun::GetDatebaseConfig(m_sDBServerName,m_sDBName,m_sPort,m_sInstanceName,m_sUserAccount,m_sUserPsw);
	SetDlgItemText(IDC_EDIT9,m_sDBServerName);
	SetDlgItemText(IDC_EDIT1,m_sDBName);
	SetDlgItemText(IDC_EDIT5,m_sUserAccount);
	SetDlgItemText(IDC_EDIT6,m_sUserPsw);
	CDBBackFun::GetDBRestoreConfig(m_sRestoreDBPath,m_sInitDBPath);
	if (!m_sRestoreDBPath.IsEmpty())
		CCommonFun::CreateDir(m_sRestoreDBPath);
	SetDlgItemText(IDC_EDIT4,m_sRestoreDBPath);

	if (!m_sInitDBPath.IsEmpty())
		CCommonFun::CreateDir(m_sInitDBPath);
	SetDlgItemText(IDC_EDIT10,m_sInitDBPath);

	//CString sParam = _T("所指路径为文件*.mdf和*.ldf所在的路径，当使用COPY文件来进行数据库在两台机器间的转移时使用本 \n功能。建议此两个文件COPY到MSSQLSERVER下的DATA目录中。         \r\nSql Server 路径 \r\n     $:\\Program Files\\Microsoft SQL Server\\MSSQL\\Data");
	CString sParam = _T(LoadS(IDS_STRING_ExpainTex));
	SetDlgItemText(IDC_EDIT2,sParam);
	return TRUE; 
}

void CRestorePage::OnBnClickedOk2()
{
	UpdateData();
	GetDlgItemText(IDC_EDIT9,m_sDBServerName);
	GetDlgItemText(IDC_EDIT1,m_sDBName);
	GetDlgItemText(IDC_EDIT5,m_sUserAccount);
	GetDlgItemText(IDC_EDIT6,m_sUserPsw);
	GetDlgItemText(IDC_EDIT10,m_sInitDBPath);
	if (m_sDBServerName.IsEmpty() || m_sDBName.IsEmpty() || m_sInitDBPath.IsEmpty())
	{
		MessageBox(_T(LoadS(IDS_STRING_Text1)),_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
		if (m_sDBServerName.IsEmpty())
			GetDlgItem(IDC_EDIT9)->SetFocus();
		if (m_sDBName.IsEmpty())
			GetDlgItem(IDC_EDIT1)->SetFocus();
		if (m_sInitDBPath.IsEmpty())
			GetDlgItem(IDC_EDIT10)->SetFocus();
		return;
	}

	CDbInstaller dbins;
	if (m_sInstanceName.IsEmpty())
	{
		if (m_sPort.IsEmpty())
			dbins.GetDefaultPort(m_sPort);
		dbins.RevertDBFromFile(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw,m_sPort,m_sInitDBPath);
	}
	else
	{
		dbins.RevertDBFromFile2(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw,m_sInstanceName,m_sInitDBPath);
	}
}

void CRestorePage::OnBnClickedButton3()
{
	CString sPath = CCommonFun::GetDirFromShell(FALSE);
	if (sPath.IsEmpty())
		return;

	// 设置当前目录
	if (sPath.Right(1) != '\\' && sPath.Right(1) != '/')
		sPath += "\\";

	SetDlgItemText(IDC_EDIT10,sPath);
}

void CRestorePage::OnOK()
{
	UpdateData();
	GetDlgItemText(IDC_EDIT10,m_sInitDBPath);
	GetDlgItemText(IDC_EDIT4,m_sRestoreDBPath);
	CDBBackFun::SetDBRestoreConfig(m_sRestoreDBPath,m_sInitDBPath);
}
