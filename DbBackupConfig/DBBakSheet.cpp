// DBBakSheet.cpp : implementation file
//

#include "stdafx.h"
#include "DBBakSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern  bool  commitsql(CString sqlexe);
/////////////////////////////////////////////////////////////////////////////
// CDBBakSheet

IMPLEMENT_DYNAMIC(CDBBakSheet, CPropertySheet)

CDBBakSheet::CDBBakSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	// 添加页面
	AddPage(&m_dlgbak);
	AddPage(&m_dlgrestore);
	AddPage(&m_target);

	// 取消“应用”及“帮助”按钮
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP; 
}

CDBBakSheet::CDBBakSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// 添加页面
	AddPage(&m_dlgbak);
	AddPage(&m_dlgrestore);
	AddPage(&m_target);

	// 取消“应用”及“帮助”按钮
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP; 
}

CDBBakSheet::~CDBBakSheet()
{
}

BEGIN_MESSAGE_MAP(CDBBakSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CDBBakSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDBBakSheet::OnInitDialog() 
{
	// 基本初始化
	CPropertySheet::OnInitDialog();
	CWnd * pButton = (CWnd *)GetDlgItem(IDOK);
	if(pButton != NULL)
		pButton->ShowWindow(false);
	CWnd * pButtonH = (CWnd *)GetDlgItem(IDHELP);
	if(pButtonH!=NULL)
		pButtonH->ShowWindow(false);
	CWnd * pButtonCancle = (CWnd *)GetDlgItem(IDCANCEL);
	if(pButtonCancle!=NULL)
	{
		CRect rect,rectsheet;
		pButtonCancle->GetWindowRect(&rect);
		this->GetWindowRect(&rectsheet);
		rect.left = rectsheet.right - rect.Width() - 10;
		rect.right = rectsheet.right - 10;
		rect.top = rectsheet.bottom - rect.Height()-10;
		rect.bottom = rectsheet.bottom - 10;
		ScreenToClient(rect);
		pButtonCancle->MoveWindow(rect);
		pButtonCancle->SetWindowText(LoadS(IDS_STRING_Cancel));
		pButtonCancle->ShowWindow(true);
	}

	m_hIcon = AfxGetApp()->LoadIcon(IDI_BAK);
    SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);
	return TRUE;
}
BOOL CDBBakSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (wParam == IDCANCEL)
	{
		m_dlgbak.OnOK();
		m_dlgrestore.OnOK();
	}

	return CPropertySheet::OnCommand(wParam, lParam);
}
