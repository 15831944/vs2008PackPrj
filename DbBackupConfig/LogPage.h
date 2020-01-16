#pragma once


// CLogPage 对话框

class CLogPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLogPage)

public:
	CLogPage();
	virtual ~CLogPage();

// 对话框数据
	enum { IDD = IDD_PAGE_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
};
