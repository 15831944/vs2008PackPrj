#pragma once
#include "afxwin.h"

#include "expandingdialog.h"
// CConfigPrtDlg 对话框

class CConfigPrtDlg : public /*CDialog*/CExpandingDialog
{
	DECLARE_DYNAMIC(CConfigPrtDlg)

public:
	CConfigPrtDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigPrtDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CONFIG_PRT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL Save();
	void Load();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void InitComUI();
	void InitBrandUI();
	CComboBox m_wndCmb;
	CString m_sIP;
	int m_nComIndex;
	BOOL m_bEnableYTJ;
	CString m_sBindIp;
	afx_msg void OnBnClickedButton2();
	CString m_sScanDir;
	CComboBox m_wndCmbIP;
	CString m_sPrinterIP;
	int m_nBrandType;
	BOOL m_bUseSignalCtrl;
	BOOL m_bEnableSnmpPrintMonitor;
	BOOL m_bEnableSnmpPrinterMeter;
	BOOL m_bEnableEmbedWeb;
	CComboBox m_cmbBrand;
};
