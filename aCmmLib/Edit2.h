#pragma once


// CEdit2
class CEdit2;
typedef BOOL (*PFCHECKDATA)(CEdit2&, BOOL);

//#define WM_EDIT_CHANGE WM_USER + 666

//注意初始化提示信息在CDialog::OnInitDialog();之前
class _AA_DLL_EXPORT_ CEdit2 : public CEdit
{
	DECLARE_DYNAMIC(CEdit2)

public:
	CEdit2();
	virtual ~CEdit2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void InitInfo(CString strInfo);//初始化提示
	void ChangeInfo(CString strNewInfo);//更改提示

	BOOL GetValue(CString& strValue);
	BOOL SetValue(CString& strValue);
	BOOL GetValue(int& nValue);
	BOOL SetValue(int nValue);
	void GetWindowText(CString& rString) const;
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
	void EnableLimit(int minVal, int maxVal);
	void EnableLimit(int nMaxLen);
	BOOL   CheckData(BOOL bMaxLimit = FALSE);

private:
	CString m_strInfo;
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void PreSubclassWindow();
	friend BOOL CheckDataMaxLen(CEdit2& pEdit, BOOL bMaxLimit);
	friend BOOL   CheckDataMinMax(CEdit2& pEdit, BOOL bMaxLimit);
private:
	BOOL   m_bEnableLimit;
	BOOL   m_bAvoidRecurse;
	BOOL   m_bUseInfo;
	int    m_nMaxLen;
	PFCHECKDATA  m_pFCheckData;
protected:
	int    m_nMinVal;
	int    m_nMaxVal;
};


