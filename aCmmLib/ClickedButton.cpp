// ClickedButton.cpp : 实现文件
//

#include "stdafx.h"
#include "ClickedButton.h"
#include "FontDialog2.h"
#include "acmmlib.h"
// CClickedButton

IMPLEMENT_DYNAMIC(CClickedButton, CButton)

CClickedButton::CClickedButton()
{
	m_cr = RGB(0, 0, 0);
	m_bOver = FALSE;
	m_hMyCursor = LoadCursor(NULL, IDC_HAND);
	m_nMode = MODE_UNKNOWN;
	m_strText = "Forget to initial?";
}

CClickedButton::~CClickedButton()
{
}


BEGIN_MESSAGE_MAP(CClickedButton, CButton)
	//ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(BN_CLICKED, &CClickedButton::OnBnClicked)
END_MESSAGE_MAP()



// CClickedButton 消息处理程序
void CClickedButton::SetMode(int nMode)
{
	m_nMode = nMode;
}
void CClickedButton::SetMode(COLORREF cr, int nMode /* = MODE_COLOR */)
{
	m_cr = cr;
	m_nMode = MODE_COLOR;
	if (IsWindow(m_hWnd))
		Invalidate();
}
void CClickedButton::SetMode(RICHEDITFONT rf, CString strText /* =  */, int nMode /* = MODE_FONT */)
{
	m_Font = rf;
	if (strText.GetLength() > 0)
		m_strText = strText;
	m_nMode = MODE_FONT;
	if (IsWindow(m_hWnd))
		Invalidate();
}
void CClickedButton::SetColor(COLORREF cr)
{
	m_cr = cr;
	Invalidate();
}
COLORREF CClickedButton::GetColor()
{
	return m_cr;
}
void CClickedButton::SetRFont(RICHEDITFONT& rf, CString strText /* =  */)
{
	m_Font = rf;
	if (strText.GetLength() > 0)
		m_strText = strText;
	if (IsWindow(m_hWnd))
		Invalidate();
}
void CClickedButton::SetFontText(CString strText)
{
	m_strText = strText;
	if (IsWindow(m_hWnd))
		Invalidate();
}
RICHEDITFONT CClickedButton::GetRFont()
{
	return m_Font;
}
void CClickedButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	::Rectangle(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom);
	if (m_nMode == MODE_COLOR)
	{
		HBRUSH hBrush =	CreateSolidBrush(m_cr);
		FillRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem), hBrush);
		DeleteObject(hBrush);
		return;
	}
	else if (m_nMode == MODE_FONT)
	{
		//字体
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		RICHEDITFONT::RICHEDITFONT2LOGFONT(m_Font, lf);
		CFont font;
		font.CreateFontIndirect(&lf);
		dc.SelectObject(&font);
		dc.SetTextColor(m_Font.color);
		dc.SetBkMode(TRANSPARENT);
		///SetFont()
		CRect rc;
		GetClientRect(&rc);
		dc.DrawText( m_strText, m_strText.GetLength(),rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		dc.Detach();
		//SelectObject(lpDrawItemStruct->hDC, hOld);
	}
}
void CClickedButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bOver)        // Cursor is currently over control
	{
		CRect rect;
		GetClientRect(rect);
		if (!rect.PtInRect(point))
		{
			m_bOver = FALSE;
			ReleaseCapture();
			return;
		}
		if (GetCursor() != m_hMyCursor)
		{
			m_bOver = FALSE;
		}
	}
	else                      // Cursor has just moved over control
	{
		if (!(GetState() & 0x0004))
		{
			m_bOver = TRUE;
			SetCapture();
			::SetCursor(m_hMyCursor);
		}
	}
	CButton::OnMouseMove(nFlags, point);
}

void CClickedButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	m_bOver = FALSE;
}

void CClickedButton::OnBnClicked()
{
	if (m_nMode == MODE_COLOR)
	{
		CColorDialog dlg(m_cr);
		if (dlg.DoModal() == IDCANCEL)
			return;
		m_cr = dlg.GetColor();
		Invalidate();
	}
	else if (m_nMode == MODE_FONT)
	{
		HINSTANCE hIns = AfxGetResourceHandle();
		AfxSetResourceHandle(theApp.m_hInstance);
		CFontDialog2 dlg(m_Font);
		if (dlg.DoModal() == IDCANCEL)
		{
			AfxSetResourceHandle(hIns);
			return;
		}
		AfxSetResourceHandle(hIns);
		dlg.GetFont(m_Font);
		Invalidate();
	}
	GetParent()->SendMessage(WM_COMMAND, GetDlgCtrlID());
}
