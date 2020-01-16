#pragma once
#include "afxwin.h"


// CDlgCreateAppDB �Ի���

class CDlgCreateAppDB : public CDialog
{
	DECLARE_DYNAMIC(CDlgCreateAppDB)

public:
	CDlgCreateAppDB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCreateAppDB();

// �Ի�������
	enum { IDD = IDD_DLG_CREATE_APP_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_wndCmb;
	CString m_sDbAddr;
	CString m_sDbUser;
	CString m_sDbPwd;
	CString m_sDbPort;
	BOOL m_bInstalledSuc;
	CStringArray m_aryDb;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedTest2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboDbtype2();
	BOOL CreateDbOnLocalMsde(CString sDbName);
};
