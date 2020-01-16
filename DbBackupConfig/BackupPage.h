#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CBackupPage 对话框

class CBackupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBackupPage)

public:
	CBackupPage();
	virtual ~CBackupPage();

// 对话框数据
	enum { IDD = IDD_PAGE_BACKUP };

public:
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedButton1();
	void EnabledWindowItem();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditMonth();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();

public:
	virtual void OnOK();

public:
	CString			m_sDBServerName;
	CString			m_sDBName;
	CString			m_sUserAccount;
	CString			m_sUserPsw;
	CString			m_sBackDBPath;
	CString			m_sPort;
	CString			m_sInstanceName;

	BOOL			m_bAutoBackupOpen;
	BACKTYPE		m_nAutoBackType;
	int				m_nInterval;
	double			m_dAutoBackTime;

private:
	CComboBox		m_weekCombox;
	CDateTimeCtrl	m_EveryDayTime;
	CDateTimeCtrl	m_WeekDayTime;
	CDateTimeCtrl	m_MonthDayTime;
};
