#pragma once

//通知 每页显示条数改变，暂时没有用到，直接调用gotopage
#define WM_PQW_COMBO_CHANGE		WM_USER + 3401

//通知 转到某一页,返回非零自动更新页数显示，零不更新[WPARAM:PageIndex　LPARAM:Page size]
#define WM_PQW_GOTO_PAGE		WM_USER + 3402

// CPageQueryWnd
class CDlgPageQuery;
class _AA_DLL_EXPORT_ CPageQueryWnd : public CWnd
{
	DECLARE_DYNAMIC(CPageQueryWnd)

public:
	CPageQueryWnd();
	virtual ~CPageQueryWnd();
public:
	// 设置combobox中的数字，第一个为默认使用，第二个为总数，之后为每个数字
	// 例：SetPageSize(100, 3, 50, 100, 150);
	void SetPageSize(int nDefault, int nTotal, ...);

	//设置总条数，当前页数
	void SetPageCount(int nTotalMsg, int nCurrentPage = 1);
	int	 GetPageSize()const;
	int  GetCurrentPage()const;
protected:
	DECLARE_MESSAGE_MAP()
	CDlgPageQuery* m_pDlg;
	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


