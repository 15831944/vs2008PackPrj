#pragma once


// CSetHpDlg 对话框


#define DEFAULT_ENT_ACCESSORY_URL _T("http://localhost/HpPrint/RegisterAccessory.aspx")
#define DEFAULT_PRO_ACCESSORY_URL _T("http://localhost:8686/rest/Accessories/checkoutAccessory")

class CSetHpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetHpDlg)

public:
	CSetHpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetHpDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SETHP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DWORD m_dwSetAccTime;
	CString m_szSetAccUrl;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioHpent();
	afx_msg void OnBnClickedRadioHppro();
	afx_msg void OnBnClickedCheck1();
	BOOL m_bEnableActivateAccessory;
};
