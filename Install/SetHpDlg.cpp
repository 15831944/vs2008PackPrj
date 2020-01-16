#include "StdAfx.h"
#include "Install.h"
#include "SetHpDlg.h"

IMPLEMENT_DYNAMIC(CSetHpDlg, CDialog)

CSetHpDlg::CSetHpDlg( CWnd* pParent /*= NULL*/ )
: CDialog(CSetHpDlg::IDD, pParent)
, m_bEnableActivateAccessory(FALSE)
{
	m_dwSetAccTime = 0;
	m_szSetAccUrl = "";
}

CSetHpDlg::~CSetHpDlg()
{

}

void CSetHpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REGACCTIME, m_dwSetAccTime);
	DDV_MinMaxUInt(pDX, m_dwSetAccTime, 0, 4294967295);
	DDX_Text(pDX, IDC_REGACCURL, m_szSetAccUrl);
	DDX_Check(pDX, IDC_CHECK1, m_bEnableActivateAccessory);
}
BEGIN_MESSAGE_MAP(CSetHpDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetHpDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_HPENT, &CSetHpDlg::OnBnClickedRadioHpent)
	ON_BN_CLICKED(IDC_RADIO_HPPRO, &CSetHpDlg::OnBnClickedRadioHppro)
	ON_BN_CLICKED(IDC_CHECK1, &CSetHpDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()
BOOL CSetHpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(theConfig.IsHpPro())
	{
		((CButton*)GetDlgItem(IDC_RADIO_HPPRO))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_HPENT))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_HPPRO))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_HPENT))->SetCheck(1);
	}
	theConfig.LoadEnableHPRegAccessory(m_bEnableActivateAccessory);
	theConfig.LoadHpAtvAccTime(m_dwSetAccTime);
	theConfig.LoadHpAtvAccUrl(m_szSetAccUrl);

	if(m_szSetAccUrl.GetLength() == 0)
	{
		m_szSetAccUrl = theConfig.IsHpPro() ? DEFAULT_PRO_ACCESSORY_URL : DEFAULT_ENT_ACCESSORY_URL;
	}
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_REGACCTIME);
	pWnd->EnableWindow(m_bEnableActivateAccessory);

	pWnd = GetDlgItem(IDC_REGACCURL);
	pWnd->EnableWindow(m_bEnableActivateAccessory);
	UpdateData(FALSE);

	UpdateData(FALSE);
	return TRUE;
}

void CSetHpDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if(IsDlgButtonChecked(IDC_RADIO_HPPRO))
	{
		theConfig.SaveHPIsPro(TRUE);
	}
	else
	{
		theConfig.SaveHPIsPro(FALSE);
	}
	theConfig.SaveEnableHPRegAccessory(m_bEnableActivateAccessory);
	theConfig.SaveHpAtvAccTime(m_dwSetAccTime);
	theConfig.SaveHpAtvAccUrl(m_szSetAccUrl);
	OnOK();
}
void CSetHpDlg::OnBnClickedRadioHpent()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_szSetAccUrl.CompareNoCase(DEFAULT_PRO_ACCESSORY_URL) == 0)
	{
		m_szSetAccUrl = DEFAULT_ENT_ACCESSORY_URL;
	}
	UpdateData(FALSE);
}

void CSetHpDlg::OnBnClickedRadioHppro()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_szSetAccUrl.CompareNoCase(DEFAULT_ENT_ACCESSORY_URL) == 0)
	{
		m_szSetAccUrl = DEFAULT_PRO_ACCESSORY_URL;
	}
	UpdateData(FALSE);
}

void CSetHpDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_REGACCTIME);
	pWnd->EnableWindow(m_bEnableActivateAccessory);

	pWnd = GetDlgItem(IDC_REGACCURL);
	pWnd->EnableWindow(m_bEnableActivateAccessory);
	UpdateData(FALSE);
}
