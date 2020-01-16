// NdisInstall.h: interface for the CNdisInstall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDISINSTALL_H__332A53A1_8E43_44DE_852D_F8D2577C0052__INCLUDED_)
#define AFX_NDISINSTALL_H__332A53A1_8E43_44DE_852D_F8D2577C0052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class  CNdisInstall  
{
public:
	CNdisInstall();
	virtual ~CNdisInstall();
	static DWORD WINAPI ThreadFun(LPVOID lpParam);
	static void Install(DWORD dwVerson,CString sName = _T("ISafeTec"));	// dwVersion: 0代表win2000, 1代表winXP
	static void UnInstall(DWORD dwVerson,CString sName = _T("ISafeTec"));	// dwVersion: 0代表win2000, 1代表winXP
	static BOOL RemoveProductSign(CString sName);
	static void SetProductSign(CString sName);
};

#endif // !defined(AFX_NDISINSTALL_H__332A53A1_8E43_44DE_852D_F8D2577C0052__INCLUDED_)
