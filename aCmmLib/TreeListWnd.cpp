// TreeListWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "aCmmLib.h"
#include "TreeListWnd.h"
#include "resource.h"

// CTreeListWnd
#define ID_TREE_LIST_HEADER 370
#define ID_TREE_LIST_CTRL   373
#define ID_TREE_LIST_SCROLLBAR   377

IMPLEMENT_DYNAMIC(CTreeListWnd, CStatic)

CTreeListWnd::CTreeListWnd()
{
}

CTreeListWnd::~CTreeListWnd()
{
}


BEGIN_MESSAGE_MAP(CTreeListWnd, CStatic)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// CTreeListWnd 消息处理程序


void CTreeListWnd::Initialize(BOOL bHasCheckBox /* = FALSE */)
{
	// creates all the objects in frame -
	// header, tree, horizontal scroll bar

	CRect m_wndRect;
	GetWindowRect(&m_wndRect);

	//加了WS_BORDER属性
	if (GetExStyle() & WS_EX_CLIENTEDGE)
		m_wndRect.bottom -= 4;

	CRect m_headerRect;

	// create the header
	{
		m_headerRect.left = m_headerRect.top = -1;
		m_headerRect.right = m_wndRect.Width();

		m_treelist.m_wndHeader.Create(WS_CHILD | WS_VISIBLE | HDS_BUTTONS | HDS_HORZ, m_headerRect, this, ID_TREE_LIST_HEADER);
	}

	CSize textSize;
	// set header's pos, dimensions and image list
	{
		LOGFONT logfont;

		CFont *pFont = GetParent()->GetFont();
		pFont->GetLogFont( &logfont );

		m_treelist.m_headerFont.CreateFontIndirect( &logfont );
		m_treelist.m_wndHeader.SetFont(&m_treelist.m_headerFont);

		CDC *pDC = m_treelist.m_wndHeader.GetDC();
		pDC->SelectObject(&m_treelist.m_headerFont);
		textSize = pDC->GetTextExtent("A");

		m_treelist.m_wndHeader.SetWindowPos(&wndTop, 0, 0, m_headerRect.Width(), textSize.cy+4, SWP_SHOWWINDOW);

		//	m_treelist.m_cImageList.Create(IDB_HEADER, 16, 10, 0);
		//	m_treelist.m_wndHeader.SetImageList(&m_treelist.m_cImageList);

		m_treelist.m_wndHeader.UpdateWindow();
	}

	CRect m_treeRect;

	// create the tree itself
	{
		m_treeRect.left=0;
		m_treeRect.top = textSize.cy+4;
		m_treeRect.right = m_headerRect.Width()-5;
		m_treeRect.bottom = m_wndRect.Height()-GetSystemMetrics(SM_CYHSCROLL)-4;

		m_treelist.Create(WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS /*| TVS_CHECKBOXES*/, m_treeRect, this, ID_TREE_LIST_CTRL);
	}

	// finally, create the horizontal scroll bar
	{
		CRect m_scrollRect;
		m_scrollRect.left=0;
		m_scrollRect.top = m_treeRect.bottom + 4;
		m_scrollRect.right = m_treeRect.Width()-GetSystemMetrics(SM_CXVSCROLL);
		m_scrollRect.bottom = m_wndRect.bottom;

		m_horScrollBar.Create(WS_CHILD | WS_VISIBLE | WS_DISABLED | SBS_HORZ | SBS_TOPALIGN, m_scrollRect, this, ID_TREE_LIST_SCROLLBAR);

		SCROLLINFO si;
		si.fMask = SIF_PAGE;
		si.nPage = m_treeRect.Width();
		m_horScrollBar.SetScrollInfo(&si, FALSE);
	}
	if (bHasCheckBox)
	{
		m_cImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 0);
		HICON hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_UNCHECK);
		m_cImageList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_cImageList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHILDCHECK);
		m_cImageList.Add(hIcon);
		m_treelist.SetImageList(&m_cImageList, TVSIL_NORMAL);
	}
	else
	{
		// added by jarry
		// 这里按需要进行修改
		m_cImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 0);
		HICON hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_cImageList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_cImageList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_cImageList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_cImageList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_cImageList.Add(hIcon);
		m_treelist.SetImageList(&m_cImageList, TVSIL_NORMAL);
	}
	
#if 0
	m_treelist.InsertColumn(0, "题目", LVCFMT_LEFT, 150);
	m_treelist.InsertColumn(1, "张三", LVCFMT_LEFT, 100);
	m_treelist.InsertColumn(2, "李四", LVCFMT_RIGHT, 100);

	HTREEITEM hType = m_treelist.InsertItem("单选题", 2, 2);
	m_treelist.SetItemText(hType, 1, "");
	m_treelist.SetItemText(hType, 2, "");
	HTREEITEM hTmp = m_treelist.InsertItem("第一题", 0, 0, hType);
	m_treelist.SetItemText(hTmp, 1, "A");
	m_treelist.SetItemText(hTmp, 2, "B");
	hTmp = m_treelist.InsertItem("第二题", 0, 0, hType);
	m_treelist.SetItemText(hTmp, 1, "A");
	m_treelist.SetItemText(hTmp, 2, "C");
	hTmp = m_treelist.InsertItem("第三题", 0, 0, hType);
	m_treelist.SetItemText(hTmp, 1, "B");
	m_treelist.SetItemText(hTmp, 2, "C");
	hTmp = m_treelist.InsertItem("多选题", 1, 1);
	m_treelist.SetItemText(hTmp, 1, "");
	m_treelist.SetItemText(hTmp, 2, "");
#endif
	//m_ed.SubclassDlgItem(IDC_EDIT2, this);

	//SortTree(0, TRUE, m_treelist.GetRootItem());
}

BOOL CTreeListWnd::VerticalScrollVisible()
{
	int sMin, sMax;
	m_treelist.GetScrollRange(SB_VERT, &sMin, &sMax);
	return sMax!=0;
}

BOOL CTreeListWnd::HorizontalScrollVisible()
{
	int sMin, sMax;
	m_horScrollBar.GetScrollRange(&sMin, &sMax);
	return sMax!=0;
}

int CTreeListWnd::StretchWidth(int m_nWidth, int m_nMeasure)
{
	return ((m_nWidth/m_nMeasure)+1)*m_nMeasure;
}


void CTreeListWnd::ResetScrollBar()
{
	// resetting the horizontal scroll bar

	int m_nTotalWidth=0, m_nPageWidth;

	CRect m_treeRect;
	m_treelist.GetClientRect(&m_treeRect);

	CRect m_wndRect;
	GetClientRect(&m_wndRect);

	CRect m_headerRect;
	m_treelist.m_wndHeader.GetClientRect(&m_headerRect);

	CRect m_barRect;
	m_horScrollBar.GetClientRect(m_barRect);

	m_nPageWidth = m_treeRect.Width();

	m_nTotalWidth = m_treelist.GetColumnsWidth();

	if(m_nTotalWidth > m_nPageWidth)
	{
		// show the scroll bar and adjust it's size
		{
			m_horScrollBar.EnableWindow(TRUE);

			m_horScrollBar.ShowWindow(SW_SHOW);

			// the tree becomes smaller
			CRect TreeRect;
			m_treelist.GetWindowRect(&TreeRect);
			if(TreeRect.Width() != m_wndRect.Width() || TreeRect.Height() != m_wndRect.Height()-m_barRect.Height()-m_headerRect.Height())
				m_treelist.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_barRect.Height()-m_headerRect.Height(), SWP_NOMOVE);

			CRect ScrollRect;
			m_horScrollBar.GetWindowRect(&ScrollRect);

			if(!VerticalScrollVisible())
				// i.e. vertical scroll bar isn't visible
			{
				m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_barRect.Height(), SWP_NOMOVE);
			}
			else
			{
				m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width() - GetSystemMetrics(SM_CXVSCROLL), m_barRect.Height(), SWP_NOMOVE);
			}
		}

		SCROLLINFO si;
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = m_treeRect.Width();
		si.nMin = 0;
		si.nMax = m_nTotalWidth;
		m_horScrollBar.SetScrollInfo(&si, FALSE);

		// recalculate the offset
		{
			CRect m_wndHeaderRect;
			m_treelist.m_wndHeader.GetWindowRect(&m_wndHeaderRect);
			ScreenToClient(&m_wndHeaderRect);

			m_treelist.m_nOffset = m_wndHeaderRect.left;
			m_horScrollBar.SetScrollPos(-m_treelist.m_nOffset);
		}
	}
	else
	{
		m_horScrollBar.EnableWindow(FALSE);

		// we no longer need it, so hide it!
		{
			m_horScrollBar.ShowWindow(SW_HIDE);

			// the tree takes scroll's place
			CRect TreeRect;
			m_treelist.GetClientRect(&TreeRect);
			m_treelist.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height() - m_headerRect.Height(), SWP_NOMOVE);
		}

		m_horScrollBar.SetScrollRange(0, 0);

		// set scroll offset to zero
		{
			m_treelist.m_nOffset = 0;
			m_treelist.Invalidate();
			CRect m_headerRect;
			m_treelist.m_wndHeader.GetWindowRect(&m_headerRect);
			CRect m_wndRect;
			GetClientRect(&m_wndRect);
			m_treelist.m_wndHeader.SetWindowPos(&wndTop, m_treelist.m_nOffset, 0, max(StretchWidth(m_treelist.GetColumnsWidth(),m_wndRect.Width()),m_wndRect.Width()), m_headerRect.Height(), SWP_SHOWWINDOW);
		}
	}
}

void CTreeListWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CRect m_treeRect;
	m_treelist.GetClientRect(&m_treeRect);

	if(pScrollBar == &m_horScrollBar)
	{
		int m_nCurPos = m_horScrollBar.GetScrollPos();
		int m_nPrevPos = m_nCurPos;
		switch(nSBCode)
		{
		case SB_LEFT:			m_nCurPos = 0;
			break;
		case SB_RIGHT:			m_nCurPos = m_horScrollBar.GetScrollLimit()-1;
			break;
		case SB_LINELEFT:		m_nCurPos = max(m_nCurPos-6, 0);
			break;
		case SB_LINERIGHT:		m_nCurPos = min(m_nCurPos+6, m_horScrollBar.GetScrollLimit()-1);
			break;
		case SB_PAGELEFT:		m_nCurPos = max(m_nCurPos-m_treeRect.Width(), 0);
			break;
		case SB_PAGERIGHT:		m_nCurPos = min(m_nCurPos+m_treeRect.Width(), m_horScrollBar.GetScrollLimit()-1);
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:  if(nPos==0)
									m_nCurPos = 0;
								else
									m_nCurPos = min(StretchWidth(nPos, 6), m_horScrollBar.GetScrollLimit()-1);
			break;
		}
		// 6 is Microsoft's step in a CListCtrl for example

		m_horScrollBar.SetScrollPos(m_nCurPos);
		m_treelist.m_nOffset = -m_nCurPos;

		// smoothly scroll the tree control
		{
			CRect m_scrollRect;
			m_treelist.GetClientRect(&m_scrollRect);
			m_treelist.ScrollWindow(m_nPrevPos - m_nCurPos, 0, &m_scrollRect, &m_scrollRect);
		}

		CRect m_headerRect;
		m_treelist.m_wndHeader.GetWindowRect(&m_headerRect);
		CRect m_wndRect;
		GetClientRect(&m_wndRect);

		m_treelist.m_wndHeader.SetWindowPos(&wndTop, m_treelist.m_nOffset, 0, max(StretchWidth(m_treelist.GetColumnsWidth(),m_treeRect.Width()),m_wndRect.Width()), m_headerRect.Height(), SWP_SHOWWINDOW);
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTreeListWnd::SortTree(int nCol, BOOL bAscending, HTREEITEM hParent)
{
	return;
	HTREEITEM hChild;
	HTREEITEM hPa = hParent;
	while(hPa!=NULL)
	{
		if(m_treelist.ItemHasChildren(hPa))
		{
			hChild = m_treelist.GetChildItem(hPa);
			m_treelist.SortItems(nCol, bAscending, hPa);
			SortTree(nCol, bAscending, hChild);
		}

		hPa = m_treelist.GetNextSiblingItem(hPa);
	}
}
BOOL CTreeListWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

	if((wParam == ID_TREE_LIST_HEADER) && (pHDN->hdr.code == HDN_ITEMCLICK))
	{
		int nCol = pHDN->iItem;

		BOOL bAscending = FALSE;

		if(m_treelist.m_wndHeader.GetItemImage(nCol)==-1)
			bAscending = TRUE;
		else
			if(m_treelist.m_wndHeader.GetItemImage(nCol)==1)
				bAscending = TRUE;

		for(int i=0;i<m_treelist.GetColumnsNum();i++)
		{
			m_treelist.m_wndHeader.SetItemImage(i, -1);
		}

		if(bAscending)
			m_treelist.m_wndHeader.SetItemImage(nCol, 0);
		else
			m_treelist.m_wndHeader.SetItemImage(nCol, 1);

		m_treelist.SortItems(nCol, bAscending, NULL);
		SortTree(nCol, bAscending, m_treelist.GetRootItem());

		m_treelist.UpdateWindow();
	}
	else
		if((wParam == ID_TREE_LIST_HEADER) && (pHDN->hdr.code == HDN_ITEMCHANGED))
		{
			int m_nPrevColumnsWidth = m_treelist.GetColumnsWidth();
			m_treelist.RecalcColumnsWidth();
			ResetScrollBar();

			// in case we were at the scroll bar's end,
			// and some column's width was reduced,
			// update header's position (move to the right).
			CRect m_treeRect;
			m_treelist.GetClientRect(&m_treeRect);

			CRect m_headerRect;
			m_treelist.m_wndHeader.GetClientRect(&m_headerRect);

			if((m_nPrevColumnsWidth > m_treelist.GetColumnsWidth()) &&
				(m_horScrollBar.GetScrollPos() == m_horScrollBar.GetScrollLimit()-1) &&
				(m_treeRect.Width() < m_treelist.GetColumnsWidth()))
			{
				m_treelist.m_nOffset = -m_treelist.GetColumnsWidth()+m_treeRect.Width();
				m_treelist.m_wndHeader.SetWindowPos(&wndTop, m_treelist.m_nOffset, 0, 0, 0, SWP_NOSIZE);
			}

			m_treelist.Invalidate();
		}
		return CStatic::OnNotify(wParam, lParam, pResult);
}