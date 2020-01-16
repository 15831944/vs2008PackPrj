
// UploadBkgImageDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "Request.h"

// CUploadBkgImageDlg �Ի���
class CUploadBkgImageDlg : public CDialog
{
// ����
public:
	CUploadBkgImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPLOADBKGIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
