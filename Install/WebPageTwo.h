#pragma once


// CWebPageTwo 对话框

class CWebPageTwo : public CPropertyPage
{
	DECLARE_DYNAMIC(CWebPageTwo)

public:
	CWebPageTwo();
	virtual ~CWebPageTwo();

// 对话框数据
	enum { IDD = IDD_PAGE_TWO };
	CComboBox	m_ctWeb;
	CString	m_szWebPort;
	CString m_szNewWebName;
	CString m_szNewWebIp;
	CString m_szNewWebPort;
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboZhan();
	afx_msg void OnTest();
	BOOL InitWebSiteInfo();
	BOOL FinishWebSiteSet(BOOL bQuiet = FALSE);
	void ShowWebPara(int index);
	void OnTestIIS();
	void MakeHttpCon(char *cinfo, int imax);
	void StartWebServer(int nNum);
	BOOL IsCheck(int uID);
	void EnableAddNewWebSite(BOOL bEanble);

public:
	CIISWeb m_Web;
	CPtrList m_WebList;
	bool m_bInitSuc;
	CString m_szVirtualPath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckNewWeb();
};
