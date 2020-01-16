// DBHelp.h : main header file for the DBHELP application
//

#if !defined(AFX_DBHELP_H__6A0EF9B8_CB14_4D20_A7CA_8CE0E4CD9629__INCLUDED_)
#define AFX_DBHELP_H__6A0EF9B8_CB14_4D20_A7CA_8CE0E4CD9629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDBHelpApp:
// See DBHelp.cpp for the implementation of this class
//

class CDBHelpApp : public CWinApp
{
public:
	CDBHelpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBHelpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDBHelpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBHELP_H__6A0EF9B8_CB14_4D20_A7CA_8CE0E4CD9629__INCLUDED_)
