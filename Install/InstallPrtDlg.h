#pragma once
#include "afxwin.h"

#include "../Public/Include/printersvctDef.h"
// CInstallPrtDlg �Ի���

class CInstallPrtDlg : public CDialog
{
	DECLARE_DYNAMIC(CInstallPrtDlg)

public:
	CInstallPrtDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInstallPrtDlg();

// �Ի�������
	enum { IDD = IDD_DLG_INSTALL_PRT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL UpdateToSvr(CPrinterAtri& oPrint);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_wndCmbPrinter;
	CString m_szPrinterIP;
	CString m_sSpeed;
	CString m_sResolution;
	CString m_sDesc;
	float m_fA3HB;
	float m_fA3Color;
	float m_fA4HB;
	float m_fA4Color;
	afx_msg void OnCbnSelchangeComboPrt();
	CString m_sColor;

	CPrinterAtri m_oAri;
};
