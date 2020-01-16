#pragma once


// CEncryptDlg 对话框

class CEncryptDlg : public CDialog
{
	DECLARE_DYNAMIC(CEncryptDlg)

public:
	CEncryptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEncryptDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ENCYPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sSerial;
	int m_nTry ;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
