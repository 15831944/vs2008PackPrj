#pragma once
#include "clickedbutton.h"
#include "afxwin.h"

//============================================
//resorce.h
#define IDD_DLG_FONT_SETTING            129
#define IDC_COMBO_COLOR                 1012
#define IDC_CHECK_UNDERLINE             1013
#define IDC_CHECK_ITALIC                1014
#define IDC_CHECK_STRICKOUT             1015
#define IDC_LIST_FONT                   1016
#define IDC_EDIT_FONT                   1017
#define IDC_BUTTON_COLOR                1018
#define IDC_CHECK_BOLD                  1019
#define IDC_LIST_SIZE                   1020
#define IDC_BUTTON_EXP                  1021
#define IDC_EDIT_SIZE                   1022

//============================================
// CFontDialog2 对话框

class CFontDialog2 : public CDialog
{
	DECLARE_DYNAMIC(CFontDialog2)

public:
	CFontDialog2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFontDialog2();
	CFontDialog2(RICHEDITFONT& rc,CWnd* pParent = NULL);
	// 对话框数据
	enum { IDD = IDD_DLG_FONT_SETTING };

public:
	void GetFont(RICHEDITFONT& rf);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedCheckUnderline();
	afx_msg void OnBnClickedCheckStrickout();
	afx_msg void OnBnClickedCheckItalic();
	afx_msg void OnBnClickedCheckBold();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnSelchangeListFont();
	afx_msg void OnEnChangeEditFont();
	virtual BOOL OnInitDialog();
private:
	CClickedButton m_btColor;
	CListBox m_listFont;
	CEdit m_editFont;
	CButton m_checkUnderline;
	CButton m_checkStrikeout;
	CButton m_checkItalic;
	CButton m_checkBold;
	CClickedButton m_btExp;
	RICHEDITFONT m_Font;
	CMap< float, float,CString, CString> m_FontSizeMap;
	CListBox m_listSize;
	CEdit m_editSize;
	afx_msg void OnLbnSelchangeListSize();
	afx_msg void OnEnChangeEditSize();
};
