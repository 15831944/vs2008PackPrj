// ComboBox2.cpp : 实现文件
//

#include "stdafx.h"
#include "aCmmLib.h"
#include "ComboBox2.h"


// CComboBox2

IMPLEMENT_DYNAMIC(CComboBox2, CComboBox)

CComboBox2::CComboBox2()
{

}

CComboBox2::~CComboBox2()
{
}


BEGIN_MESSAGE_MAP(CComboBox2, CComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CComboBox2::OnCbnDropdown)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, &CComboBox2::OnCbnEditchange)
END_MESSAGE_MAP()



// CComboBox2 消息处理程序



void CComboBox2::OnCbnDropdown()
{
	CString      str;
	CSize      sz;
	int      dx = 0;
	TEXTMETRIC   tm;
	CDC*      pDC =  GetDC();
	CFont*      pFont =  GetFont();

	// Select the listbox font, save the old font
	CFont* pOldFont = pDC->SelectObject(pFont);
	// Get the text metrics for avg char width
	pDC->GetTextMetrics(&tm);

	for (int i = 0; i <  GetCount(); i++)
	{
		GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		// Add the avg width to prevent clipping
		sz.cx += tm.tmAveCharWidth;

		if (sz.cx > dx)
			dx = sz.cx;
	}
	// Select the old font back into the DC
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	// Adjust the width for the vertical scroll bar and the left and right border.
	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	// Set the horizontal extent so every character of all strings can 
	// be scrolled to.
	SetDroppedWidth(dx);
}

void CComboBox2::OnCbnEditchange()
{
#if 0
	if (!(GetStyle() & CBS_DROPDOWN))
		return;
	CEdit* pEdit = (CEdit*) GetWindow(GW_CHILD);
	if (pEdit == NULL)
		return;
	CString str;
	pEdit->GetWindowText(str);
	int nIndex  = FindString(-1, str);
	if (nIndex != CB_ERR)
	{
		if (!GetDroppedState())
		{
			ShowDropDown(TRUE);
		}
		SetTopIndex(nIndex);
		pEdit->SetSel(str.GetLength(), -1);
	}
#endif
}
CString CComboBox2::GetEditText()
{
	CString str;
	CWnd* pWnd = GetWindow(GW_CHILD);
	if (pWnd != NULL)
		pWnd->GetWindowText(str);
	return str;
}

CString CComboBox2::GetSelText()
{
	CString str;
	int nIndex = GetCurSel();
	if (nIndex != CB_ERR)
		GetLBText(nIndex, str);
	return str;
}
int	CComboBox2::SelectStringExact(int nStartAfter, LPCTSTR lpszString)
{
	if (nStartAfter == 0)
	{
		TRACE("U forget the first???\n");
		ASSERT(0);
	}
	int nIndex = FindString(nStartAfter, lpszString);
	if (nIndex != CB_ERR)
	{
		SetCurSel(nIndex);
	}
	return nIndex;
}