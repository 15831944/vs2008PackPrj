#pragma once
#include "rf.h"

// CClickedButton
class CClickedButton : public CButton
{
	DECLARE_DYNAMIC(CClickedButton)

public:
	enum{MODE_UNKNOWN, MODE_COLOR, MODE_FONT};
	CClickedButton();
	virtual ~CClickedButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetColor(COLORREF cr);
	COLORREF	GetColor();
	void SetFontText(CString strText);
	void SetRFont(RICHEDITFONT& rf, CString strText = "");
	RICHEDITFONT	GetRFont();
	void        SetMode(int nMode);
	void        SetMode(COLORREF cr, int nMode = MODE_COLOR);
	void        SetMode(RICHEDITFONT rf, CString strText = "", int nMode = MODE_FONT);
private:
	COLORREF m_cr;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	HCURSOR	m_hPreCursor;
	HCURSOR m_hMyCursor;
	BOOL	m_bOver;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	int    m_nMode;
	RICHEDITFONT m_Font;
	afx_msg void OnBnClicked();
	CString m_strText;
};


