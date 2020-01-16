#pragma once


class COracleDbDlg : public CDialog
{
// Construction
public:
	COracleDbDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CSaturnDBDlg)
	enum { IDD = IDD_DLG_ORACLE };
	//}}AFX_DATA

	UINT	m_nHourSpan;
	CString	m_szPwd2;
	CString	m_szUID2;
	CString	m_szMsg;
	CString	m_szDBOwner;
	CString	m_szDataSource;

	bool WriteReg(char* szName,LPCTSTR szValue);
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

