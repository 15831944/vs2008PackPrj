// DlgPageQuery.cpp : 实现文件
//

#include "stdafx.h"
#include "aCmmLib.h"
#include "DlgPageQuery.h"
#include "PageQueryWnd.h"
#include "threadmgr.h"
#include <cmath>
using namespace std;

// CDlgPageQuery 对话框

IMPLEMENT_DYNAMIC(CDlgPageQuery, CDialog)

CDlgPageQuery::CDlgPageQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPageQuery::IDD, pParent)
{
	m_nPageCount = 1;
	m_nCurrentPage = 1;
	m_nMsgCount = 0;
//	m_nCurrentMsg = 0;
	m_nMsgPerPage = 0;
}

CDlgPageQuery::~CDlgPageQuery()
{
}

void CDlgPageQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE, m_StCount);
	DDX_Control(pDX, IDC_FIRST, m_btFirst);
	DDX_Control(pDX, IDC_PRE, m_btPre);
	DDX_Control(pDX, IDC_NEXT, m_btNext);
	DDX_Control(pDX, IDC_LAST, m_btLast);
	DDX_Control(pDX, IDC_COMBO5, m_comboPageSize);
}


BEGIN_MESSAGE_MAP(CDlgPageQuery, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPageQuery::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPageQuery::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_FIRST, &CDlgPageQuery::OnBnClickedFirst)
	ON_BN_CLICKED(IDC_PRE, &CDlgPageQuery::OnBnClickedPre)
	ON_BN_CLICKED(IDC_NEXT, &CDlgPageQuery::OnBnClickedNext)
	ON_BN_CLICKED(IDC_LAST, &CDlgPageQuery::OnBnClickedLast)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlgPageQuery::OnCbnSelchangeCombo5)
END_MESSAGE_MAP()


// CDlgPageQuery 消息处理程序

void CDlgPageQuery::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CDlgPageQuery::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}
void CDlgPageQuery::OnBnClickedFirst()
{
	if (m_nCurrentPage == 1)
		return;
	m_nCurrentPage = 1;
	GotoCurrentPage();
}

void CDlgPageQuery::OnBnClickedPre()
{
	if (m_nCurrentPage > 1)
	{
		m_nCurrentPage--;
		GotoCurrentPage();
	}
}

void CDlgPageQuery::OnBnClickedNext()
{
	if (m_nCurrentPage < m_nPageCount)
	{
		m_nCurrentPage++;
		GotoCurrentPage();
	}
}

void CDlgPageQuery::OnBnClickedLast()
{
	if (m_nCurrentPage == m_nPageCount)
		return;
	m_nCurrentPage = m_nPageCount;
	GotoCurrentPage();
}


void CDlgPageQuery::OnCbnSelchangeCombo5()
{
	int nIndex = m_comboPageSize.GetCurSel();
	if (nIndex == LB_ERR)
		return;
	CString str;
	m_comboPageSize.GetLBText(nIndex, str);
	if (m_nMsgPerPage == atoi(str))
		return;
	m_nMsgPerPage = atoi(str);
	m_nCurrentPage = 1;
	GotoCurrentPage();
}
void CDlgPageQuery::GotoCurrentPage()
{
	CWnd* pParent = GetParent();//pagequerywnd
	if (pParent == NULL)
		return;
	pParent = pParent->GetParent();
	if (pParent == NULL)
		return;
	if (::SendMessage(pParent->GetSafeHwnd(), WM_PQW_GOTO_PAGE, m_nCurrentPage, m_nMsgPerPage))
		SetPageCount(m_nMsgCount, m_nCurrentPage);
	return;
}
BOOL CDlgPageQuery::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strText;
	m_btFirst.SetIcon(IDI_FIRST);
	m_btFirst.DrawTransparent();
	strText.LoadString(IDS_FIRSTPAGE);
	m_btFirst.SetTooltipText(strText);

	m_btPre.SetIcon(IDI_PRE);
	m_btPre.DrawTransparent();
	strText.LoadString((IDS_PREPAGE));
	m_btPre.SetTooltipText(strText);

	m_btNext.SetIcon(IDI_NEXT);
	m_btNext.DrawTransparent();
	strText.LoadString((IDS_NEXTPAGE));
	m_btNext.SetTooltipText(strText);

	m_btLast.SetIcon(IDI_LAST);
	m_btLast.DrawTransparent();
	strText.LoadString((IDS_LASTPAGE));
	m_btLast.SetTooltipText(strText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgPageQuery::SetPageCount(int nTotalMsg, int nCurrentPage /* = 1 */)
{
	ASSERT(this);
	ASSERT(IsWindow(this->GetSafeHwnd()));
	if (!this || !IsWindow(this->GetSafeHwnd()))
		return;
	this->m_nCurrentPage = nCurrentPage;

	//this->m_nPageCount = nTotalPage;
	//nPageCount = (int)ceil((double)nMsgCount / m_wndPageQuery.GetMsgPerPage());//统共多少页
	this->m_nPageCount = (int)ceil((double)nTotalMsg / m_nMsgPerPage);
	if (this->m_nPageCount  <= 0 )
		this->m_nPageCount = 1;
	this->m_nMsgCount = nTotalMsg;
	CString strText, strTemp;

	HINSTANCE hIns = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	strTemp.LoadString(IDS_PAGECOUNT);
	AfxSetResourceHandle(hIns);

	strText.Format(strTemp, this->m_nCurrentPage, this->m_nPageCount, this->m_nMsgCount);
	this->m_StCount.SetWindowText(strText);
	
	//根据长度改变控件大小
	TEXTMETRIC   tm;
	CDC*      pDC =  GetDC();
	CFont*      pFont =  GetFont();
	CFont* pOldFont = pDC->SelectObject(pFont);

	CSize      sz;
	int      dxNow = 0;
	int		 dxNormal = 0;
	pDC->GetTextMetrics(&tm);
	sz = pDC->GetTextExtent(strText);
	sz.cx += tm.tmAveCharWidth;
	dxNow = sz.cx;
	strText.Format(strTemp, 100, 100, 10000);
	sz = pDC->GetTextExtent(strText);
	sz.cx += tm.tmAveCharWidth;
	dxNormal = sz.cx;
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	//整个控件
	CRect rc;
	int nWidth = 0;
	m_StCount.GetWindowRect(&rc);
	ScreenToClient(&rc);
	nWidth = dxNow;
	rc.right = rc.left + nWidth;
	m_StCount.MoveWindow(&rc);
	nWidth = rc.right;
#if 1
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);
	//nWidth = rc.Width() - dxNormal + dxNow;
	rc.right = nWidth;
	MoveWindow(&rc);
	ClientToScreen(&rc);
	GetParent()->GetParent()->ScreenToClient(&rc);
	GetParent()->MoveWindow(&rc);
	GetParent()->Invalidate();
#endif
}