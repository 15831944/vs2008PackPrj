// PageQueryWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "acmmlib.h"
#include "PageQueryWnd.h"
#include "DlgPageQuery.h"

// CPageQueryWnd

IMPLEMENT_DYNAMIC(CPageQueryWnd, CWnd)

CPageQueryWnd::CPageQueryWnd()
{
	m_pDlg = new CDlgPageQuery;
}

CPageQueryWnd::~CPageQueryWnd()
{
	delete m_pDlg;
}


BEGIN_MESSAGE_MAP(CPageQueryWnd, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CPageQueryWnd 消息处理程序



void CPageQueryWnd::PreSubclassWindow()
{
	if (m_pDlg && !IsWindow(m_pDlg->GetSafeHwnd()))
	{
		HINSTANCE hIns = AfxGetResourceHandle();
		AfxSetResourceHandle(theApp.m_hInstance);
		m_pDlg->Create(IDD_DLG_PAGE_QUERY, this);
		AfxSetResourceHandle(hIns);
		CRect rc;
		GetClientRect(&rc);
		m_pDlg->MoveWindow(rc);
		m_pDlg->ShowWindow(SW_SHOW);
	}
	CWnd::PreSubclassWindow();
}
void CPageQueryWnd::SetPageSize(int nDefault, int nTotal, ...)
{
	ASSERT(m_pDlg);
	ASSERT(IsWindow(m_pDlg->GetSafeHwnd()));
	if (!m_pDlg || !IsWindow(m_pDlg->GetSafeHwnd()))
		return;

	CString strText;
	va_list argList;
	va_start(argList, nTotal);
	int n = 0;
	m_pDlg->m_comboPageSize.ResetContent();
	for (int i = 1; i <= nTotal; i++)
	{
		n = va_arg(argList, int);
		strText.Format("%d", n);
		m_pDlg->m_comboPageSize.AddString(strText);
	}
	strText.Format("%d", nDefault);
	int nIndex = m_pDlg->m_comboPageSize.SelectStringExact(-1, strText);
	ASSERT(nIndex != CB_ERR);
	if (nIndex == CB_ERR)
	{
		m_pDlg->m_comboPageSize.AddString(strText);
		m_pDlg->m_comboPageSize.SelectStringExact(-1, strText);
	}
	m_pDlg->m_nMsgPerPage = nDefault;
	va_end(argList);
}
void CPageQueryWnd::SetPageCount(int nTotalMsg, int nCurrentPage /* = 1 */)
{
	ASSERT(m_pDlg);
	ASSERT(IsWindow(m_pDlg->GetSafeHwnd()));
	if (!m_pDlg || !IsWindow(m_pDlg->GetSafeHwnd()))
		return;
	m_pDlg->SetPageCount(nTotalMsg, nCurrentPage);
}
int CPageQueryWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pDlg && !IsWindow(m_pDlg->GetSafeHwnd()))
	{
		HINSTANCE hIns = AfxGetResourceHandle();
		AfxSetResourceHandle(theApp.m_hInstance);
		m_pDlg->Create(IDD_DLG_PAGE_QUERY, this);
		AfxSetResourceHandle(hIns);
		CRect rc;
		GetClientRect(&rc);
		m_pDlg->MoveWindow(rc);
		m_pDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}
int CPageQueryWnd::GetPageSize()const
{
	if (m_pDlg)
		return m_pDlg->m_nMsgPerPage;
	return 0;
}
int CPageQueryWnd::GetCurrentPage()const
{
	if (m_pDlg)
		return m_pDlg->m_nCurrentPage;
	return 1;
}