
// UploadBkgImageDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "Request.h"

// CUploadBkgImageDlg 对话框
class CUploadBkgImageDlg : public CDialog
{
// 构造
public:
	CUploadBkgImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPLOADBKGIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUbtOverview();
	CString m_szFilePath;
	CStatic m_CPictureCtrl;
	CComboBox m_CBPicType;
	int m_nPort;
	CButton m_BtnUpload;
	CIPAddressCtrl m_IPCtrl;
	Request* m_pRequest;
	afx_msg void OnBnClickedUbtUpload();
	BOOL RequestHttp( CString szHttp, CString &szResponse );
	BOOL UploadPic(LPCTSTR strURL, LPCTSTR strLocalFileName);
	afx_msg void OnCbnSelchangeUbtCombo();
	CString m_szPicSize;
};
