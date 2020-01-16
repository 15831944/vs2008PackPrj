#pragma once


// CRestorePage 对话框

class CRestorePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CRestorePage)

public:
	CRestorePage();
	virtual ~CRestorePage();

// 对话框数据
	enum { IDD = IDD_PAGET_RESTORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedButton3();
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();

public:
	CString			m_sDBServerName;
	CString			m_sDBName;
	CString			m_sUserAccount;
	CString			m_sUserPsw;
	CString			m_sRestoreDBPath;
	CString			m_sInitDBPath;
	CString			m_sPort;
	CString			m_sInstanceName;
};
