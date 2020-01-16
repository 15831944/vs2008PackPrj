// SysTrayInfo.h: interface for the CSysTrayInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTRAYINFO_H__7E31BD93_F5BE_46C8_91D2_8BE4CCCD6C25__INCLUDED_)
#define AFX_SYSTRAYINFO_H__7E31BD93_F5BE_46C8_91D2_8BE4CCCD6C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct TRAYDATA
{
	HWND hwnd;				
	UINT uID;				
	UINT uCallbackMessage;	
	DWORD Reserved[2];		
	HICON hIcon;				
};

struct TrayItemInfo
{
	HWND hwnd;				
	UINT uID;				
	UINT uCallbackMessage;
	CString sTip;
	CString sProcessPath;
	bool bVisible;
};

class _AA_DLL_EXPORT_ CSysTrayInfo  
{
public:
	void UpdateTrayRect();
	BOOL GetTrayPos(int nCmdID,POINT &pt);
	BOOL UpdateTray(int nCmdID);
	BOOL GetAllTBtn();
	BOOL GetIconCount(int& nCount);
	
	HWND m_hTrayWnd;
	int  m_nIconCount;
//	CList<TBBUTTON_INFO,TBBUTTON_INFO &> m_tbtList;

	HWND GetSysTrayIconWnd();
	HWND GetToolbarWnd();
	BOOL GetWindowBtnTxt(CStringArray& ary);
	CSysTrayInfo();
	virtual ~CSysTrayInfo();

};

#endif // !defined(AFX_SYSTRAYINFO_H__7E31BD93_F5BE_46C8_91D2_8BE4CCCD6C25__INCLUDED_)
