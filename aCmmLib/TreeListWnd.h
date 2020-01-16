#pragma once

#include "newtreelistctrl.h"

// CTreeListWnd

class _AA_DLL_EXPORT_ CTreeListWnd : public CStatic
{
	DECLARE_DYNAMIC(CTreeListWnd)

public:
	CTreeListWnd();
	virtual ~CTreeListWnd();
public:
	CNewTreeListCtrl m_treelist;
	CImageList		m_cImageList;

protected:
	DECLARE_MESSAGE_MAP()
public:
	CScrollBar m_horScrollBar;
	void Initialize(BOOL bHasCheckBox = FALSE);

	void SortTree(int nCol, BOOL bAscending, HTREEITEM hParent);
	void ResetScrollBar();

	BOOL VerticalScrollVisible();
	BOOL HorizontalScrollVisible();

	int StretchWidth(int m_nWidth, int m_nMeasure);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


