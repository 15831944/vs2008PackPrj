#pragma once

class _AA_DLL_EXPORT_ CHtmlHelp
{
public:
	CHtmlHelp(void);
	~CHtmlHelp(void);

	CString m_sWndName;

	void SetHelpWndTitle(CString sName);
	void OpenHelp(CString sIndex);
};
