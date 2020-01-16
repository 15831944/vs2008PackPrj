#pragma once


// ConfigEmbedWeb 对话框

class ConfigEmbedWeb : public CDialog
{
	DECLARE_DYNAMIC(ConfigEmbedWeb)

public:
	ConfigEmbedWeb(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ConfigEmbedWeb();

// 对话框数据
	enum { IDD = IDD_DLG_EMBEDWEB };
	
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_szTitle;
	CString m_szColor;
	CString m_szSize;
	int m_nJump;
	int m_nPrtStationId;
	afx_msg void OnBnClickedOk();
};
