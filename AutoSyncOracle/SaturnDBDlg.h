#if !defined(AFX_SATURNDBDLG_H__2596EF39_A9AF_4926_9849_DE9474B90879__INCLUDED_)
#define AFX_SATURNDBDLG_H__2596EF39_A9AF_4926_9849_DE9474B90879__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaturnDBDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaturnDBDlg dialog

class CSaturnDBDlg : public CDialog
{
// Construction
public:
	bool WriteReg(char* szName,LPCTSTR szValue);
	CSaturnDBDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaturnDBDlg)
	enum { IDD = IDD_SaturnDBDlg };
	CString	m_szDBIP;
	CString	m_szPwd;
	CString	m_szUID;
	CString	m_szMsg;
	CString	m_szDbName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaturnDBDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaturnDBDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SATURNDBDLG_H__2596EF39_A9AF_4926_9849_DE9474B90879__INCLUDED_)
