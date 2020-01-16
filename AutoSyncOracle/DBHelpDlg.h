// DBHelpDlg.h : header file
//

#if !defined(AFX_DBHELPDLG_H__71086B3C_A993_4E4D_8899_6CC6400B767A__INCLUDED_)
#define AFX_DBHELPDLG_H__71086B3C_A993_4E4D_8899_6CC6400B767A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "trayicon.h"
/////////////////////////////////////////////////////////////////////////////
// CDBHelpDlg dialog

class CDBHelpDlg : public CDialog
{
// Construction
public:
	CDBHelpDlg(CWnd* pParent = NULL);	// standard constructor
	CTrayIcon	m_trayIcon;		// my tray icon
// Dialog Data
	//{{AFX_DATA(CDBHelpDlg)
	enum { IDD = IDD_DBHELP_DIALOG };
	UINT	m_nHourSpan;
	CString	m_szPwd2;
	CString	m_szUID2;
	CString	m_szDBOwner;
	CString	m_szDataSource;
	BOOL	m_bAutoDep;
	BOOL	m_bRunOnceExit;
	//}}AFX_DATA

	CString	m_szMsg;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBHelpDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void TestOracleDB();
	void Release();
	void UpdateSaturnDbInfo();
	CString GetDefaultPath();
	void BackupCardInfo();
	void BeginSyncDB();
	bool WriteReg(char* szName,LPCTSTR szValue);
	bool m_bCanClose;
	void OnExit();
	LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
	bool CheckUser(CString& szCertID,CString &szName,CString &szPwd,CString &szReadFlag,CString &szCertFlag,CString &szDep);
	bool CheckUserEx(CString& szCertID,CString &szName,CString &szPwd,CString &szReadFlag,CString &szCertFlag,CString &szDep);
	int m_nTimerSpan;

	CString m_szOracleUID;
	CString m_szOraclePwd;


	bool OperateDB();
	CString m_szUserName;
	CString m_szDbName;
	CString m_szServerIP;
	CString m_szPwd;
	CString m_szPort;

	bool Connect();
	CAdo m_OracleAdo;
	CAdo m_MSAdo;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDBHelpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnSet();
	afx_msg void OnSetSaturn();
	afx_msg void OnRightNow();
	afx_msg void OnCHECK1RunOnce();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBHELPDLG_H__71086B3C_A993_4E4D_8899_6CC6400B767A__INCLUDED_)
