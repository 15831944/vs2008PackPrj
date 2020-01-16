#if !defined(AFX_DBBAKSHEET_H__11CF51C1_91E2_11D5_B6F2_0050BAF46898__INCLUDED_)
#define AFX_DBBAKSHEET_H__11CF51C1_91E2_11D5_B6F2_0050BAF46898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "backuppage.h"
#include "Restorepage.h"
#include "LogPage.h"
/////////////////////////////////////////////////////////////////////////////
// CDBBakSheet
class CDBBakSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDBBakSheet)

public:
	CDBBakSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDBBakSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDBBakSheet();

protected:
	//{{AFX_MSG(CDBBakSheet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CBackupPage				m_dlgbak;
    CRestorePage			m_dlgrestore;
	CLogPage				m_target;
	
	HICON					m_hIcon;
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
#endif // !defined(AFX_DBBAKSHEET_H__11CF51C1_91E2_11D5_B6F2_0050BAF46898__INCLUDED_)
