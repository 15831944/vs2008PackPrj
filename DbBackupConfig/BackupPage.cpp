// BackupPage.cpp : 实现文件
//

#include "stdafx.h"
#include "DbBackupConfig.h"
#include "BackupPage.h"

// CBackupPage 对话框
IMPLEMENT_DYNAMIC(CBackupPage, CPropertyPage)

CBackupPage::CBackupPage()
	: CPropertyPage(CBackupPage::IDD)
{

}

CBackupPage::~CBackupPage()
{

}

void CBackupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ComboWeek, m_weekCombox);
	DDX_Control(pDX, IDC_DATE_DAY, m_EveryDayTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_WeekDayTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_MonthDayTime);
}

BEGIN_MESSAGE_MAP(CBackupPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CBackupPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_UseTarget, &CBackupPage::EnabledWindowItem)
	ON_BN_CLICKED(IDC_RADIOD, &CBackupPage::EnabledWindowItem)
	ON_BN_CLICKED(IDC_RADIOW, &CBackupPage::EnabledWindowItem)
	ON_BN_CLICKED(IDC_RADIOM, &CBackupPage::EnabledWindowItem)
	ON_EN_CHANGE(IDC_EDIT_MONTH, &CBackupPage::OnEnChangeEditMonth)
	ON_BN_CLICKED(IDOK, &CBackupPage::OnBnClickedOk)
END_MESSAGE_MAP()

void CBackupPage::OnBnClickedButton1()
{
	CString sPath = CCommonFun::GetDirFromShell(FALSE);
	if (sPath.IsEmpty())
		return;

	// 设置当前目录
	if (sPath.Right(1) != '\\' && sPath.Right(1) != '/')
		sPath += "\\";

	SetDlgItemText(IDC_EDIT4,sPath);
}

void CBackupPage::EnabledWindowItem()
{
	m_bAutoBackupOpen = ((CButton*)GetDlgItem(IDC_UseTarget))->GetCheck() == BST_CHECKED ? TRUE : FALSE;
	int nChecked = GetCheckedRadioButton(IDC_RADIOM,IDC_RADIOD);
	if (nChecked == 0)
	{
		nChecked = IDC_RADIOD;
		CheckRadioButton(IDC_RADIOM,IDC_RADIOD,IDC_RADIOD);
	}

	GetDlgItem(IDC_RADIOD)->EnableWindow(m_bAutoBackupOpen);
	GetDlgItem(IDC_RADIOW)->EnableWindow(m_bAutoBackupOpen);
	GetDlgItem(IDC_RADIOM)->EnableWindow(m_bAutoBackupOpen);
	GetDlgItem(IDC_DATE_DAY)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOD);
	GetDlgItem(IDC_ComboWeek)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOW);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOW);
	GetDlgItem(IDC_EDIT_MONTH)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOM);
	GetDlgItem(IDC_SPIN1)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOM);
	GetDlgItem(IDC_STATIC_AA)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOM);
	GetDlgItem(IDC_STATIC_AA2)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOM);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(m_bAutoBackupOpen && nChecked == IDC_RADIOM);
}

BOOL CBackupPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// 得到默认信息在界面上显示
	CDBBackFun::GetDatebaseConfig(m_sDBServerName,m_sDBName,m_sPort,m_sInstanceName,m_sUserAccount,m_sUserPsw);
	SetDlgItemText(IDC_EDIT9,m_sDBServerName);
	SetDlgItemText(IDC_EDIT1,m_sDBName);
	SetDlgItemText(IDC_EDIT5,m_sUserAccount);
	SetDlgItemText(IDC_EDIT6,m_sUserPsw);

	// 得到备份配置信息
	CDBBackFun::GetDBBackConfig(m_sBackDBPath,m_bAutoBackupOpen,m_nAutoBackType,m_nInterval,m_dAutoBackTime);
	if (!m_sBackDBPath.IsEmpty())
		CCommonFun::CreateDir(m_sBackDBPath);
	SetDlgItemText(IDC_EDIT4,m_sBackDBPath);
	SetDlgItemText(IDC_EDIT_MONTH,_T("1"));
	m_weekCombox.SetCurSel(0);
	if (m_bAutoBackupOpen)
	{
		((CButton*)GetDlgItem(IDC_UseTarget))->SetCheck(BST_CHECKED);
		COleDateTime dt;
		dt.m_dt = m_dAutoBackTime;
		switch (m_nAutoBackType)
		{
		case BACKTYPE_BYDAY:
			m_EveryDayTime.SetTime(dt);
			break;

		case BACKTYPE_BYWEEK:
			m_WeekDayTime.SetTime(dt);
			m_weekCombox.SetCurSel(m_nInterval);
			break;
		case BACKTYPE_BYMONTH:
			m_MonthDayTime.SetTime(dt);
			SetDlgItemInt(IDC_EDIT_MONTH,m_nInterval);
			break;
		}
	}
	EnabledWindowItem();
	return TRUE; 
}

void CBackupPage::OnEnChangeEditMonth()
{
	CString sText;
	GetDlgItemText(IDC_EDIT_MONTH,sText);
	if (atoi(sText) >= 31)
		SetDlgItemText(IDC_EDIT_MONTH,_T("31"));
	else if (atoi(sText) <= 0)
		SetDlgItemText(IDC_EDIT_MONTH,_T("1"));
}	

void CBackupPage::OnBnClickedOk()
{
	UpdateData();
	GetDlgItemText(IDC_EDIT9,m_sDBServerName);
	GetDlgItemText(IDC_EDIT1,m_sDBName);
	GetDlgItemText(IDC_EDIT5,m_sUserAccount);
	GetDlgItemText(IDC_EDIT6,m_sUserPsw);
	GetDlgItemText(IDC_EDIT4,m_sBackDBPath);
	if (m_sDBServerName.IsEmpty() || m_sDBName.IsEmpty() || m_sBackDBPath.IsEmpty())
	{
		MessageBox(_T(LoadS(IDS_STRING_Text1)),_T(LoadS(IDS_STRING_Tip)),MB_OK | MB_ICONINFORMATION);
		if (m_sDBServerName.IsEmpty())
			GetDlgItem(IDC_EDIT9)->SetFocus();
		if (m_sDBName.IsEmpty())
			GetDlgItem(IDC_EDIT1)->SetFocus();
		if (m_sBackDBPath.IsEmpty())
			GetDlgItem(IDC_EDIT4)->SetFocus();
		return;
	}

	CString sBackSavePath;
	COleDateTime dt = COleDateTime::GetCurrentTime();
	sBackSavePath.Format(_T("%s%s_%04d-%02d-%02d-%02d-%02d-%02d.bak"),m_sBackDBPath,m_sDBName,
		dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());

	// 数据库的备份
	CDbInstaller dbins;
	if (m_sInstanceName.IsEmpty())
	{
		if (m_sPort.IsEmpty())
			dbins.GetDefaultPort(m_sPort);
		dbins.BackUpDBToFile(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw,m_sPort,sBackSavePath,TRUE);
	}
	else
	{
		dbins.BackUpDBToFile2(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw,m_sInstanceName,sBackSavePath,TRUE);
	}
}

void CBackupPage::OnOK()
{
	UpdateData();
	GetDlgItemText(IDC_EDIT9,m_sDBServerName);
	GetDlgItemText(IDC_EDIT1,m_sDBName);
	GetDlgItemText(IDC_EDIT5,m_sUserAccount);
	GetDlgItemText(IDC_EDIT6,m_sUserPsw);
	CDBBackFun::SetDatebaseConfig(m_sDBServerName,m_sDBName,m_sUserAccount,m_sUserPsw);

	GetDlgItemText(IDC_EDIT4,m_sBackDBPath);
	m_bAutoBackupOpen = ((CButton*)GetDlgItem(IDC_UseTarget))->GetCheck() == BST_CHECKED ? TRUE : FALSE;
	BACKTYPE type = BACKTYPE_BYDAY;
	int nInterVal = 1;
	double dTime = 0;
	COleDateTime dtTime;
	if (m_bAutoBackupOpen)
	{
		int nChecked = GetCheckedRadioButton(IDC_RADIOM,IDC_RADIOD);
		if (nChecked == 0)
		{
			nChecked = IDC_RADIOD;
			CheckRadioButton(IDC_RADIOM,IDC_RADIOD,IDC_RADIOD);
		}
		switch (nChecked)
		{
		case IDC_RADIOD:
			type = BACKTYPE_BYDAY;
			m_EveryDayTime.GetTime(dtTime);
			dTime = dtTime.m_dt;
			break;

		case IDC_RADIOM:
			type = BACKTYPE_BYMONTH;
			GetDlgItemInt(IDC_EDIT_MONTH,&nInterVal);
			m_MonthDayTime.GetTime(dtTime);
			dTime = dtTime.m_dt;
			break;

		case IDC_RADIOW:
			type = BACKTYPE_BYWEEK;
			nInterVal = m_weekCombox.GetCurSel();
			m_WeekDayTime.GetTime(dtTime);
			dTime = dtTime.m_dt;
			break;
		}
	}	
	CDBBackFun::SetDBBackConfig(m_sBackDBPath,m_bAutoBackupOpen,type,nInterVal,dtTime);

	// 处理服务
	CString sCulFileName;
	CString sProcPath;
	GetModuleFileName(NULL,sCulFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	sCulFileName.ReleaseBuffer();
	int inum = sCulFileName.ReverseFind('\\');
	if (inum == -1)
		return;
	if (m_bAutoBackupOpen)
		sProcPath.Format("\"%s\\%s\" -Install",sCulFileName.Left(inum),DBBACKSERVERPROC);
	else
		sProcPath.Format("\"%s\\%s\" -Unstall",sCulFileName.Left(inum),DBBACKSERVERPROC);
	CCommonFun::CreateProcessAndWait(sProcPath.GetBuffer());
	sProcPath.ReleaseBuffer();
}
