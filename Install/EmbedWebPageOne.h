#pragma once


// CEmbedWebPageOne 对话框

class CEmbedWebPageOne : public CPropertyPage
{
	DECLARE_DYNAMIC(CEmbedWebPageOne)

public:
	CEmbedWebPageOne();
	virtual ~CEmbedWebPageOne();
    CString	m_szDbName;
    CString	m_szIP;
    CString	m_szPort;
    CString	m_szUserName;
    CString	m_szUserPwd;
    CComboBox m_wndCmb;
    int m_nDbType;
// 对话框数据
	enum { IDD = IDD_EMBED_PAGE_ONE };
	
    virtual BOOL OnSetActive();
    virtual LRESULT OnWizardNext();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    virtual BOOL OnInitDialog();
    afx_msg void OnTest();

    void InitDbSet();
    BOOL WriteDbSet();
    BOOL TestConnect();
    
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnWizardFinish();
    afx_msg void OnSelchangeComboDbType();
};
