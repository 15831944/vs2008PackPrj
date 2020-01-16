#pragma once
#include "clickedbutton.h"


// CDlgReSetting 对话框

class CDlgReSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgReSetting)

public:
	CDlgReSetting(COLORREF bgcr
		, RICHEDITFONT rfTime, RICHEDITFONT rfName,
				RICHEDITFONT rfNormal, RICHEDITFONT rfInfo, RICHEDITFONT rfError,
		
		CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgReSetting();

// 对话框数据
	enum { IDD = IDD_DLG_RE_SETTING };

public:
	void GetSettings(COLORREF& bgcr
		, RICHEDITFONT& rfTime, RICHEDITFONT& rfName,
				RICHEDITFONT& rfNormal, RICHEDITFONT& rfInfo, RICHEDITFONT& rfError
		);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CClickedButton m_btBgColor;
public:
	CClickedButton m_btTimeFont;
public:
	CClickedButton m_btNameFont;
public:
	CClickedButton m_btNormalFont;
public:
	CClickedButton m_btInfoFont;
public:
	CClickedButton m_btErrorFont;
public:
	afx_msg void OnBnClickedOk();
private:
// 	COLORREF    m_bgCr;
// 	RICHEDITFONT m_rfTime;
// 	RICHEDITFONT m_rfName;
// 	RICHEDITFONT m_rfInfo;
// 	RICHEDITFONT m_rfNormal;
// 	RICHEDITFONT m_rfError;
};
