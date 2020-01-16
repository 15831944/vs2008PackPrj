#pragma once


// CExportDataDlg 对话框

class CExportDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportDataDlg)

public:
	CExportDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportDataDlg();

// 对话框数据
	enum { IDD = IDD_DLG_EXPORT };
	CString	m_szDbName;
	CString	m_szIP;
	CString	m_szPort;
	CString	m_szUserName;
	CString	m_szUserPwd;
	BOOL TestConnect();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCnnt();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnSave();
};
