// KeyMHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "IniFile.h"
//#include "..\APNetOp\RevScreenWnd.h"
/*#include "..\public\include\MessageDefine.h"*/

//===============================================add by zfq,2013.01.22
#pragma data_seg("iSecKeyMHook_data") 
int	 g_nMultiPort = 5030;
char g_bysMultiIP[16] = {0x0};	//组播IP
char g_bysLocalIP[16] = {0x0};	//本地IP
#pragma data_seg() 
#pragma comment(linker,"/Section:iSecKeyMHook_data,rws")
//===============================================add by zfq,2013.01.22


#define WM_APNETOP				WM_USER + 3000 

#define WM_MOUSE_EVENT			WM_APNETOP + 12
#define WM_KEY_ENVENT			WM_APNETOP + 13

#if 0
#ifndef MOUSEHOOKSTRUCTEX
typedef struct {
    MOUSEHOOKSTRUCT MOUSEHOOKSTRUCT;
    DWORD mouseData;
} MOUSEHOOKSTRUCTEX, *PMOUSEHOOKSTRUCTEX;
#endif
#endif
static HHOOK hkbMouse=NULL;
static HHOOK hkbKeyboard=NULL;
HANDLE g_moudle=NULL;
/*HWND   g_FarCtlhWnd=NULL;*/
HWND	g_hNotifyWnd = NULL;
HWND	g_hHookWnd = NULL;

static AFX_EXTENSION_MODULE KeyMHook = { NULL, NULL };

//CRevScreenWnd * g_pRevSrcWnd=NULL;
 
DWORD g_wMosWpm=0;
DWORD cur_x=0,cur_y =0;
/*DWORD g_dNum=0;*/
extern "C" int APIENTRY
DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	 switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:			
			g_moudle=hModule;
			//if (!AfxInitExtensionModule(KeyMHook, (HINSTANCE)hModule))
			//	return 0;
			break;
		case DLL_THREAD_ATTACH:	        
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:           			
			break;
    }

 
    return TRUE;
}


LRESULT  CALLBACK MyMouseProc(int nCode,WPARAM wparam,LPARAM lparam)
{
	if (nCode < 0)  
		return CallNextHookEx(hkbMouse,nCode,wparam,lparam); 
    bool bCanSend=false;
 
	LPMOUSEHOOKSTRUCT pMouseHook=(MOUSEHOOKSTRUCT*)lparam;
	
	if((lparam)&&(g_hHookWnd ==pMouseHook->hwnd))
	{
		
		if(wparam == WM_MOUSEMOVE || wparam == WM_NCMOUSEMOVE)
		{			
			if( cur_x!=pMouseHook->pt.x || cur_y!=pMouseHook->pt.y)
			{				
				BOOL b = PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,wparam, MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			
				ASSERT(b);
				//g_pRevSrcWnd->SendKeyMouseInfo( wparam, lparam,true);
			}
		}
		else
		{
// 			if (WM_MOUSEWHEEL == wparam)
// 			{
// 				MOUSEHOOKSTRUCTEX* pEx = (MOUSEHOOKSTRUCTEX*)lparam;
// 				TRACE("fwKeys = %d,zDelat = %d,x = %d,y = %d \n",LOWORD(pEx->mouseData),(short)HIWORD(pEx->mouseData),pMouseHook->pt.x,pMouseHook->pt.y);
// 				//::PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,WM_MOUSEMOVE, MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			
// 				
// 				BOOL b = ::PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,wparam, pEx->mouseData);//MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			
// 				TRACE("b = %d, fwKeys = %d,zDelat = %d,x = %d,y = %d \n",b,LOWORD(pEx->mouseData),(short)HIWORD(pEx->mouseData),pMouseHook->pt.x,pMouseHook->pt.y);
// 		
// 			}
// 			else
// 			{
// 				if(g_wMosWpm!=wparam)
// 				{
// 					g_wMosWpm=wparam;
// 					PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,wparam, MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			
// 				}
// 			}

			if(g_wMosWpm!=wparam)
			{
							
				if(WM_MOUSEWHEEL!=wparam)
				{
					g_wMosWpm=wparam;
					PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,wparam, MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			
				}
				else
				{
					MOUSEHOOKSTRUCTEX* pEx = (MOUSEHOOKSTRUCTEX*)lparam;
				//	TRACE("fwKeys = %d,zDelat = %d,x = %d,y = %d \n",LOWORD(pEx->mouseData),(short)HIWORD(pEx->mouseData),pMouseHook->pt.x,pMouseHook->pt.y);
					//::PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,WM_MOUSEMOVE, MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			

				//	BOOL b = ::PostMessage(g_hNotifyWnd,WM_MOUSE_EVENT,wparam, pEx->mouseData);//MAKELONG((WORD)pMouseHook->pt.x,(WORD)pMouseHook->pt.y));			
				//	TRACE("b = %d, fwKeys = %d,zDelat = %d,x = %d,y = %d \n",b,LOWORD(pEx->mouseData),(short)HIWORD(pEx->mouseData),pMouseHook->pt.x,pMouseHook->pt.y);
				}
			}
		}
		TRACE("wparam=%d----\n",wparam);
		cur_x = pMouseHook->pt.x;
		cur_y = pMouseHook->pt.y;		
		
	}
	 
 

 
	return CallNextHookEx(hkbMouse,nCode,wparam,lparam);
}

//截取键盘消息	
LRESULT  CALLBACK MyKeyboardProcDll(int nCode,WPARAM wParam,LPARAM lParam)
{	
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;
	if (nCode < 0)  
		return CallNextHookEx( hkbKeyboard, nCode, wParam, lParam );	
	 
	bool bNotNeedSend=false;
	DWORD dFlag = pkbhs->flags;
	DWORD dwScanCode = 0;
	BYTE keyState[256];			
	dwScanCode = pkbhs->scanCode;


		 
		 

	bool bUpDownLeftRightKey=false;
	switch (pkbhs->vkCode)//处理方向键
	{	
	case 0x2d:
	case 0x2e:

	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:

	case 0x25:										
	case 0x26:										
	case 0x27:										
	case 0x28:
		bUpDownLeftRightKey=true;
// 		{
// 			char cstr[256];
// 			sprintf(cstr,"key==%x dcode=%x",dFlag,pkbhs->vkCode);
// 			HDC hdc=::GetDC(NULL);
// 			::TextOut(hdc,40,10,cstr,strlen(cstr));
// 			::ReleaseDC(NULL,hdc);
// 		}
		break;					
	}
// 	{
// 		char cstr[256];
// 		sprintf(cstr,"key==%x bUpDownLeftRightKey = %d ,dcode=%x",keyState[VK_NUMLOCK] & 1,bUpDownLeftRightKey,dwScanCode);
// 		HDC hdc=::GetDC(NULL);
// 		::TextOut(hdc,10,10,cstr,strlen(cstr));
// 		::ReleaseDC(NULL,hdc);
// 			}
	//获取键盘当前状态，判断小键盘是否开启
	GetKeyboardState((LPBYTE)&keyState);	
	if ((keyState[VK_NUMLOCK] & 1)&&!bUpDownLeftRightKey)
	{															
		switch (dwScanCode)
		{
		case 0x52:										
		case 0x4f:										
		case 0x50:										
		case 0x51:										
		case 0x4b:										
		case 0x4c:										
		case 0x4d:										
		case 0x47:										
		case 0x48:										
		case 0x49:										
		case 0x53:																		
			dwScanCode += 0x2c;
// 			{
// 				char cstr[256];
// 				sprintf(cstr,"key==%x dcode=%x",dFlag,dwScanCode);
// 				HDC hdc=::GetDC(NULL);
// 				::TextOut(hdc,10,10,cstr,strlen(cstr));
// 				::ReleaseDC(NULL,hdc);
// 			}
			break;					
		}
	}
	
	switch(dwScanCode)
	{
	case 0x37:
		bNotNeedSend=true;
		break;
	}
	
	if(!bNotNeedSend && g_hNotifyWnd)
	{
		WPARAM w = MAKEWPARAM(pkbhs->vkCode,pkbhs->flags);
		::PostMessage(g_hNotifyWnd,WM_KEY_ENVENT,w,dwScanCode);
	}
	
	LRESULT RetVal = 1;
	RetVal = CallNextHookEx( hkbKeyboard, nCode, wParam, lParam );	
	return  RetVal;
}

extern "C" BOOL __declspec(dllexport) SendCtrlAltDel()
{
	//SendCtrlAltDelOp();
	return TRUE;
}



extern "C" BOOL __declspec(dllexport) Installhook(HWND hHookWnd,HWND hNotifyWnd,DWORD dwThreadID)
{
	ASSERT(hNotifyWnd);
	if (!hNotifyWnd)
	{
		return FALSE;
	}
	g_hHookWnd = hHookWnd;
	g_hNotifyWnd = hNotifyWnd;
 
	hkbKeyboard=SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)MyKeyboardProcDll,(HINSTANCE__ *)g_moudle,0);
	//hkbKeyboard=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)MyKeyboardProcDll,/*(HINSTANCE__ *)g_moudle*/NULL,dThreadid);
 
	hkbMouse=SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MyMouseProc,/*(HINSTANCE__ *)g_moudle*/NULL,dwThreadID);
	//hkbMouse=SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MyMouseProc,KeyMHook.hModule,dThreadid);
	//KeyMHook
  
	return hkbKeyboard && hkbMouse;
}
extern "C" void  __declspec(dllexport)   UnHook()
{   	
	 if (hkbKeyboard)
	 {
		BOOL unhooked1 = UnhookWindowsHookEx(hkbKeyboard);
	 }
	 if (hkbMouse)
	 {
		 BOOL unhooked2 = UnhookWindowsHookEx(hkbMouse);
	 }
} 


//==========================add by zfq,2013.01.21,begin
#include "ParseKey.h"

CParseKey theParseKey;

extern void WriteLogExKH(LPCTSTR lpszFormat, ...);

//截取键盘消息	
LRESULT  CALLBACK ISecKeyboardProcDll(int nCode,WPARAM wParam,LPARAM lParam)
{	
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;

//	WriteLogExKH("##ISecKeyboardProcDll,1,nCode=%d,lParam=0x%x", nCode, lParam);


	if (nCode < 0)  
		return CallNextHookEx( hkbKeyboard, nCode, wParam, lParam );

#if 1
	if(((DWORD)lParam&0x40000000) && (HC_ACTION==nCode))
	{
		WriteLogExKH("##ISecKeyboardProcDll,2,before SetKey,wParam=0x%x", wParam);
		theParseKey.SaveKey(wParam);
	}
#else


	WriteLogExKH("##ISecKeyboardProcDll,2");

	DWORD dwScanCode = pkbhs->scanCode;

	WriteLogExKH("##ISecKeyboardProcDll,3,dwScanCode=%d", dwScanCode);

	theParseKey.SaveKey(dwScanCode);
#endif

	LRESULT RetVal = 1;
	RetVal = CallNextHookEx( hkbKeyboard, nCode, wParam, lParam );	
	return  RetVal;
}

//截取键盘消息	
LRESULT  CALLBACK ISecKeyboardProcDll_LL(int nCode,WPARAM wParam,LPARAM lParam)
{	
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;

	//	WriteLogExKH("##ISecKeyboardProcDll,1,nCode=%d,lParam=0x%x", nCode, lParam);


	if (nCode < 0)  
		return CallNextHookEx( hkbKeyboard, nCode, wParam, lParam );

#if 0
	if(((DWORD)lParam&0x40000000) && (HC_ACTION==nCode))
	{
		WriteLogExKH("##ISecKeyboardProcDll,2,before SetKey,wParam=0x%x", wParam);
		theParseKey.SaveKey(wParam);
	}
#else
//	WriteLogExKH("##ISecKeyboardProcDll,2");

	DWORD dwScanCode = pkbhs->scanCode;

//	WriteLogExKH("##ISecKeyboardProcDll,3,dwScanCode=%d", dwScanCode);

	theParseKey.SaveKey(dwScanCode);
#endif

	LRESULT RetVal = 1;
	RetVal = CallNextHookEx( hkbKeyboard, nCode, wParam, lParam );	
	return  RetVal;
}

#define SYS_CONFIG_FILE _T("config\\config.ini")
extern CString GetDefaultPath();
extern "C" BOOL __declspec(dllexport) InstallKeyHook2(char *pMultiIP, int nMultiPort, char *pLocalIP)
{
	WriteLogExKH("##InstallKeyHook2,0.1,test=%d", 7);
	if(!pMultiIP || 0 >= nMultiPort || !pLocalIP)
	{
		WriteLogExKH("!!InstallKeyHook,1,pMultiIP=0x%x,nMultiPort=%d,pLocalIP=0x%x", pMultiIP, nMultiPort, pLocalIP);
		return FALSE;
	}
//	theParseKey.SetCallBack(pfnOnReadCard, pContext);
	WriteLogExKH("##InstallKeyHook2,0.2,pMultiIP=%s,nMultiPort=%d,pLocalIP=%s", pMultiIP, nMultiPort, pLocalIP);
	strcpy(g_bysMultiIP, pMultiIP);
	g_nMultiPort = nMultiPort;
	strcpy(g_bysLocalIP, pLocalIP);

	BOOL BHookLL = FALSE;
	{
		CIniFile file;
		file.m_sPath.Format("%s%s",GetDefaultPath(),SYS_CONFIG_FILE);
		int nHookLL = file.GetVal("PrtCtrlConfig","HookLL", 0);
		BHookLL = (0 == nHookLL) ? FALSE : TRUE;
	}


	if(BHookLL)
	{
		hkbKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)ISecKeyboardProcDll_LL,(HINSTANCE__ *)g_moudle,0);
	}
	else
	{
		hkbKeyboard = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)ISecKeyboardProcDll,(HINSTANCE__ *)g_moudle,0);	
	}

	WriteLogExKH("##InstallKeyHook,1.2,test=%d,BHookLL=%d", 9, BHookLL);

	BOOL BRet = FALSE;
	if(hkbKeyboard)
	{
		WriteLogExKH("##InstallKeyHook,2,SetWindowsHookEx ok");
		BRet = TRUE;
	}
	else
	{
		WriteLogExKH("##InstallKeyHook,3,SetWindowsHookEx fail,err=%d", GetLastError());
	}

	return BRet;
}

extern "C" BOOL __declspec(dllexport) InstallKeyHook(PFN_OnReadCardByKeyHook pfnOnReadCard, void *pContext)
{
	{
		return InstallKeyHook2("224.0.0.85", 5031, "192.168.2.45");
	}

	WriteLogExKH("##InstallKH,0.1,test=%d", 7);
	if(!pfnOnReadCard || !pContext)
	{
		WriteLogExKH("!!InstallKH,1,pfnOnReadCard=0x%x, pContext=0x%x", pfnOnReadCard, pContext);
		return FALSE;
	}
	theParseKey.SetCallBack(pfnOnReadCard, pContext);
	//	hkbKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)ISecKeyboardProcDll,(HINSTANCE__ *)g_moudle,0);
	hkbKeyboard = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)ISecKeyboardProcDll,(HINSTANCE__ *)g_moudle,0);	

	WriteLogExKH("##InstallKH,1.2,test=%d", 9);

	BOOL BRet = FALSE;
	if(hkbKeyboard)
	{
		WriteLogExKH("##InstallKH,2,SetHK ok");
		BRet = TRUE;
	}
	else
	{
		WriteLogExKH("##InstallKH,3,SetHK fail,err=%d", GetLastError());
	}

	return BRet;
}


extern "C" void  __declspec(dllexport)   UnHookKey()
{
	WriteLogExKH("##UnHK,1,hkbKeyboard=0x%x", hkbKeyboard);
	theParseKey.SetCallBack(NULL, NULL);
	if (hkbKeyboard)
	{
		BOOL unhooked1 = UnhookWindowsHookEx(hkbKeyboard);
		hkbKeyboard = NULL;	//add by zfq
	}
}
//==========================add by zfq,2013.01.21,end
