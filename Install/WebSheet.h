#if !defined(AFX_WEBSHEET_H__B3C8A467_1D67_4A0A_A1A8_4E97AE914ABF__INCLUDED_)
#define AFX_WEBSHEET_H__B3C8A467_1D67_4A0A_A1A8_4E97AE914ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebSheet.h : header file
//
#include "webPageOne.h"
#include "webPageTwo.h"
#include "WebPageThree.h"

/////////////////////////////////////////////////////////////////////////////
// CWebSheet

class CWebSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CWebSheet)

// Construction
public:
	CWebSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CWebSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	CWebSheet(UINT iPage ,LPCTSTR pszCaption);
// Attributes
public:

// Operations
public:
	CWebPageOne page1;
	CWebPageTwo page2;
	CWebPageThree page3;
	void WriteTheURL(BOOL bQueit = FALSE, CString szSerIp = "", CString szSerPort = "80");
	void WriteTheFtp(BOOL bQueit = FALSE);
	BOOL m_bSuc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWebSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWebSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBSHEET_H__B3C8A467_1D67_4A0A_A1A8_4E97AE914ABF__INCLUDED_)
