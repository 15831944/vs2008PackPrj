// XControl.cpp

#include "stdafx.h"
#include "XControl.h"
#include "XThemeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXControl::CXControl(CWnd *pParent) :
	m_pParent(pParent),
	m_bMouseOver(FALSE),
	m_bPressed(FALSE),
	m_bEnableTheming(TRUE),
	m_bEnabled(TRUE),
	m_rect(0,0,0,0)
{
}

CXControl::~CXControl()
{
}

BOOL CXControl::IsThemed(HTHEME hTheme)
{
	BOOL bAppThemed = ThemeHelper.IsAppThemed();	// user can disable theming
													// for app by right-clicking
													// on exe, then clicking on
													// Properties | Compatibility |
													// Disable visual themes
	return (hTheme && m_bEnableTheming && bAppThemed);
}
