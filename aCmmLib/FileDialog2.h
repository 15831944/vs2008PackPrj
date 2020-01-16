#pragma once

// CFileDialog2

class _AA_DLL_EXPORT_ CFileDialog2 : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialog2)

public:
	CFileDialog2(BOOL bOpenFileDialog, // 对于 FileOpen 为 TRUE，对于 FileSaveAs 为 FALSE
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CFileDialog2();

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void SetMaxLen(ULONGLONG lMaxLen);
public:
	virtual INT_PTR DoModal();
private:
	DWORD		 m_dwMaxLen;
private:
	void		 RemoveNextPathName(POSITION& pos);
	CString		FormatFileSize(ULONGLONG uSize);
	CString		m_strFT;
	CString		m_strFileBuffer;
};


