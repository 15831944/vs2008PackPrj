#pragma once


// CComboBox2

class _AA_DLL_EXPORT_ CComboBox2 : public CComboBox
{
	DECLARE_DYNAMIC(CComboBox2)

public:
	CComboBox2();
	virtual ~CComboBox2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdown();
	afx_msg void OnCbnEditchange();
public:
	CString		GetEditText();
	CString		GetSelText();
	int			SelectStringExact(int nStartAfter, LPCTSTR lpszString);
};


