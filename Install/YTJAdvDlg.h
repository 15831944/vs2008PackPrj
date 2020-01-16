#pragma once
#include "afxwin.h"


// CYTJAdvDlg 对话框

class CYTJAdvDlg : public CDialog
{
	DECLARE_DYNAMIC(CYTJAdvDlg)

public:
	CYTJAdvDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CYTJAdvDlg();

// 对话框数据
	enum { IDD = IDD_DLG_YTJ_ADV };
	enum PRINTER_ADV_MODE
	{
		PAM_NONE,
		PAM_Xerox_LongJing
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void InitPrinterAdvModeUI();
	void Load();
	void Save();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbPrinterAdvMode;
	afx_msg void OnBnClickedOk();
	CString m_sPrinterIP;
	CString m_sPid;
	CString m_sSNO;
	CString m_sPName;
};
