// EncryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "EncryptDlg.h"


// CEncryptDlg 对话框

IMPLEMENT_DYNAMIC(CEncryptDlg, CDialog)

CEncryptDlg::CEncryptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEncryptDlg::IDD, pParent)
	, m_sSerial(_T(""))
{
	m_nTry = 1;
}

CEncryptDlg::~CEncryptDlg()
{
}

void CEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sSerial);
	DDV_MaxChars(pDX, m_sSerial, 15);
}


BEGIN_MESSAGE_MAP(CEncryptDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEncryptDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEncryptDlg 消息处理程序

void CEncryptDlg::OnBnClickedOk()
{
	if( m_nTry >= 3 )
	{
		CDialog::OnCancel();
		return;
	}
	m_nTry++;

	UpdateData();
	if( m_sSerial.IsEmpty() )
	{
		MBW(IDS_EMPTY_SERIAL);
		return;
	}
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	OnOK();
}

BOOL CEncryptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT1)->SetFocus();

	SetForegroundWindow();
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
