#pragma once


// CWebPageTwo 对话框

class CWebPageThree : public CPropertyPage
{
	DECLARE_DYNAMIC(CWebPageThree)

public:
	CWebPageThree();
	virtual ~CWebPageThree();

// 对话框数据
	enum { IDD = IDD_PAGE_THREE };
	CComboBox	m_ctWeb;
	CString	m_szWebPort;
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboZhan();
	afx_msg void OnTest();
	BOOL InitWebSiteInfo();
	BOOL FinishWebSiteSet(BOOL bQuiet = FALSE);
	void ShowWebPara(int index);
	void OnTestIIS();
	void MakeHttpCon(char *cinfo, int imax);
	void StartWebServer(int nNum);

public:
	CIISFTP m_Web;
	CPtrList m_WebList;
	bool m_bInitSuc;
	CString m_szVirtualPath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
