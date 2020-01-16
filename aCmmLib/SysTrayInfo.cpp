// SysTrayInfo.cpp: implementation of the CSysTrayInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysTrayInfo.h"
#include "ProcessData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysTrayInfo::CSysTrayInfo()
{
	
}

CSysTrayInfo::~CSysTrayInfo()
{

}

HWND CSysTrayInfo::GetSysTrayIconWnd()
{
	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if(hWnd)
	{
		hWnd = ::FindWindowEx(hWnd,NULL,_T("TrayNotifyWnd"), NULL);
		if(hWnd)
		{
			if(CCommonFun::IsWinXp() || CCommonFun::IsWin2003())
				hWnd = ::FindWindowEx(hWnd,NULL,_T("SysPager"), NULL);
			if(hWnd)
			{				
				hWnd = ::FindWindowEx(hWnd, NULL,_T("ToolbarWindow32"), NULL);
			}
		}
	}
	return hWnd;
}

BOOL CSysTrayInfo::GetIconCount(int &nCount)
{
	ASSERT(m_hTrayWnd);
	nCount = (int)::SendMessage(m_hTrayWnd, TB_BUTTONCOUNT, 0, 0);
	return TRUE;
}

BOOL CSysTrayInfo::GetAllTBtn()
{
	return TRUE;
}

BOOL CSysTrayInfo::UpdateTray(int nCmdID)
{
	m_hTrayWnd = GetSysTrayIconWnd();
	if(!m_hTrayWnd)
		return FALSE;
	GetIconCount(m_nIconCount);

	DWORD dwSize = ::SendMessage(m_hTrayWnd,TB_GETBUTTONSIZE,0,0);
	DWORD dwWidth = LOWORD(dwSize);
	DWORD dwHeight = HIWORD(dwSize);

	RECT rc; 
	BOOL bRet = FALSE;
	::GetWindowRect(m_hTrayWnd,&rc);
	//pt.y = rc.top;

	DWORD dwTrayPid = 0;
	GetWindowThreadProcessId(m_hTrayWnd, &dwTrayPid);
	CProcessData<TBBUTTON> data(dwTrayPid);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo ;
	int nVisbleIndex = 0;

	for(int i = 0; i < m_nIconCount; i++)
	{		
		::SendMessage(m_hTrayWnd, TB_GETBUTTON, i, (LPARAM)data.GetData());		
		data.ReadData(&tb);			
		data.ReadData(&tray,(LPCVOID)tb.dwData);

		if(tray.uID == nCmdID)
		{	
			bRet = (BOOL)SendMessage(m_hTrayWnd,TB_DELETEBUTTON,i,0);
//			pt.x = rc.left + nVisbleIndex * dwWidth;
	//		bRet = TRUE;
			//break;
		}
		if(!(tb.fsState&TBSTATE_HIDDEN))
		{
			nVisbleIndex ++;
		}
	}	
	return bRet;
}
BOOL CSysTrayInfo::GetTrayPos(int nCmdID,POINT &pt)
{
	m_hTrayWnd = GetSysTrayIconWnd();
	if(!m_hTrayWnd)
		return FALSE;
	GetIconCount(m_nIconCount);

	DWORD dwSize = ::SendMessage(m_hTrayWnd,TB_GETBUTTONSIZE,0,0);
	DWORD dwWidth = LOWORD(dwSize);
	DWORD dwHeight = HIWORD(dwSize);

	RECT rc; 
	BOOL bRet = FALSE;
	::GetWindowRect(m_hTrayWnd,&rc);
	pt.y = rc.top;

	DWORD dwTrayPid = 0;
	GetWindowThreadProcessId(m_hTrayWnd, &dwTrayPid);
	CProcessData<TBBUTTON> data(dwTrayPid);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo ;
	int nVisbleIndex = 0;

	for(int i = 0; i < m_nIconCount; i++)
	{		
		::SendMessage(m_hTrayWnd, TB_GETBUTTON, i, (LPARAM)data.GetData());		
		data.ReadData(&tb);			
		data.ReadData(&tray,(LPCVOID)tb.dwData);

		if(tray.uID == nCmdID)
		{	
			pt.x = rc.left + nVisbleIndex * dwWidth;
			bRet = TRUE;
			break;
		}
		if(!(tb.fsState&TBSTATE_HIDDEN))
		{
			nVisbleIndex ++;
		}
	}	
	return bRet;
}

void CSysTrayInfo::UpdateTrayRect()
{
	m_hTrayWnd = GetSysTrayIconWnd();
	if(!m_hTrayWnd)
		return ;
	GetIconCount(m_nIconCount);

	DWORD dwTrayPid = 0;
	GetWindowThreadProcessId(m_hTrayWnd, &dwTrayPid);
	CProcessData<TBBUTTON> data(dwTrayPid);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo ;
	for(int i = 0; i < m_nIconCount; i++)
	{		
		::SendMessage(m_hTrayWnd, TB_GETBUTTON, i, (LPARAM)data.GetData());		
		data.ReadData(&tb);			
		data.ReadData(&tray,(LPCVOID)tb.dwData);

		if(!::IsWindow(tray.hwnd))
		{
			NOTIFYICONDATA nc = {0};
			nc.cbSize = sizeof(nc);
			nc.uID = tray.uID;
			nc.hWnd = tray.hwnd;
			Shell_NotifyIcon(NIM_DELETE,&nc);
		}
	}	
}
HWND CSysTrayInfo::GetToolbarWnd()
{
	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if(hWnd)
	{			
		hWnd = ::FindWindowEx(hWnd, NULL,_T("ReBarWindow32"), NULL);
		if (hWnd)
		{
			hWnd = ::FindWindowEx(hWnd, NULL,_T("MSTaskSwWClass"), NULL);
			if (hWnd)
			{
				hWnd = ::FindWindowEx(hWnd, NULL,_T("ToolbarWindow32"), NULL);
			}
		}	
	}
	return hWnd;
}

BOOL CSysTrayInfo::GetWindowBtnTxt(CStringArray& ary)
{
	HWND hWnd = GetToolbarWnd();
	if(!hWnd)
		return FALSE;
	GetIconCount(m_nIconCount);
	int nCount = (int)::SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);
	DWORD dwTrayPid = 0;
	GetWindowThreadProcessId(hWnd, &dwTrayPid);
	CProcessData<TBBUTTON> data(dwTrayPid);
	TBBUTTON tb = {0};
	CProcessData<tag_TButton_Text> txt(dwTrayPid);
	for(int i = 0; i < nCount; i++)
	{		
		::SendMessage(m_hTrayWnd, TB_GETBUTTON, i, (LPARAM)data.GetData());		
		data.ReadData(&tb);	
		SendMessage(m_hTrayWnd,TB_GETBUTTONTEXT,i,(LPARAM)txt.GetData());
		tag_TButton_Text aa;
		txt.ReadData(&aa);
		if( (tb.fsState & TBSTATE_HIDDEN) )
		{
			//TRACE("hide szBtnTxt = %s,state = %d\n",aa.szTxt,tb.fsState /*& TBSTATE_HIDDEN ? 1 : 0*/);
		}
		else
		{
			//TRACE("show szBtnTxt = %s,state = %d\n",aa.szTxt,tb.fsState /*& TBSTATE_HIDDEN ? 1 : 0*/);
			ary.Add(aa.szTxt);
		}		
	}
	return TRUE;
}