#pragma once


// CCeleritySettingDlg 对话框

class CCeleritySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CCeleritySettingDlg)

public:
	CCeleritySettingDlg(CString sServerIP,UINT uUpport = 8080,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCeleritySettingDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CELERITYSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();

private:
	CString				m_sSerIP;
	CString				m_sUpdatePath;
	UINT				m_nUpdatePort;
};
