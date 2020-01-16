#pragma once
#include "afxwin.h"


// CDlgCfgPrtCenterSvr 对话框

class CDlgCfgPrtCenterSvr : public CDialog
{
	DECLARE_DYNAMIC(CDlgCfgPrtCenterSvr)

public:
	CDlgCfgPrtCenterSvr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCfgPrtCenterSvr();

// 对话框数据
	enum { IDD = IDD_DLG_CFG_PrtCenterSvr };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
protected:
	CComboBox m_wndCmbIP;

	CString m_sBindIp;
};
