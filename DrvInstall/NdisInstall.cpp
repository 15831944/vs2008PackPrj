// NdisInstall.cpp: implementation of the CNdisInstall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NdisInstall.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static bool g_bWindowScanThread = true;

#define NDIS_DRV_FILE_NAME	_T("eNdisFilter.Sys")
#define NDIS_DRV_ID			_T("ms_eNdisFilter")
#define NDIS_INSTALL_TOOL	_T("snetcfg.exe")
#define NDIS_SVC_NAME		_T("eNdisFilter")
#ifdef _AMD64_
#define NDIS_BIN_DIR		_T("x64\\Ndis")
#else
#define NDIS_BIN_DIR		_T("x86\\Ndis")
#endif
#define IDS_NOT_FOUND_DIGITAL_SIGN ""
#define IDS_HARDWARE_INSTALL	""
#define IDS_SOFTWARE_INSTALL	""

#define CFFS(a) _T("")
CNdisInstall::CNdisInstall()
{

}

CNdisInstall::~CNdisInstall()
{

}


DWORD WINAPI CNdisInstall::ThreadFun(LPVOID lpParam)
{
	while(g_bWindowScanThread)
	{
		HWND g_hWnd = ::FindWindow(NULL,CFFS(IDS_NOT_FOUND_DIGITAL_SIGN));
		if(g_hWnd)
		{
		//	::ShowWindow(g_hWnd,SW_HIDE);
		//	keybd_event(67, /*1376257*/0,1,0);
			::PostMessage(g_hWnd,WM_KEYDOWN,13,0);
		}
		g_hWnd = ::FindWindow(NULL,CFFS(IDS_HARDWARE_INSTALL));
		if(g_hWnd)
		{
		//	::ShowWindow(g_hWnd,SW_HIDE);
		//	keybd_event(67, /*1376257*/0,1,0);
		//	::PostMessage(g_hWnd,WM_KEYDOWN,13,0);
			keybd_event(67, /*1376257*/0,1,0);
		}
		g_hWnd = ::FindWindow(NULL,CFFS(IDS_SOFTWARE_INSTALL));
		if(g_hWnd)
		{
		//	::ShowWindow(g_hWnd,SW_HIDE);
		//	keybd_event(67, /*1376257*/0,1,0);
		//	::PostMessage(g_hWnd,WM_KEYDOWN,13,0);
			keybd_event(67, /*1376257*/0,1,0);
		}
		Sleep(1);
	}
	return 0;
}

void CNdisInstall::Install(DWORD dwVersion,CString sName /*= "AsMars"*/)
{
	g_bWindowScanThread = true;
	char cPath[MAX_PATH]={0};	// 当前程序所在路径
	::GetModuleFileName(NULL,(char *)cPath,MAX_PATH);
	TCHAR* pEnd=_tcsrchr(cPath,_T('\\'));
	if(pEnd)
	{
		*pEnd=0;	// 去掉结尾的'\\'字符
	}

	char cinf[MAX_PATH] = {0},
		ctemp[MAX_PATH] = {0};
 
	sprintf(ctemp,"%s\\%s\\netsf.inf",cPath,NDIS_BIN_DIR);    
	GetShortPathName(ctemp,cinf,MAX_PATH);

	char pCmd[MAX_PATH]={0};
	sprintf(pCmd,"-l %s -c s -i %s",cinf,NDIS_DRV_ID);	

	STARTUPINFO StartInfo;
	PROCESS_INFORMATION pinfo;

	CString sPath;
	sPath.Format(_T("%s\\%s\\%s"),cPath,NDIS_BIN_DIR,NDIS_INSTALL_TOOL);

	memset(&StartInfo,0,sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_HIDE;
	char multicmd[MAX_PATH];
	sprintf(multicmd,"\"%s\" %s",sPath,pCmd);

	WriteLogEx("path = %s",multicmd);
	if(!CreateProcess(
		0,//(LPCSTR)sPath,
		multicmd,//(LPTSTR)pCmd,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&pinfo))//创建进程失败
	{
		CString strError;
		strError.Format("找不到文件%s！",sPath);
		Beep(100,100);
		WriteLogEx(strError);
	}
	::WaitForSingleObject (pinfo.hProcess, INFINITE);
	::CloseHandle (pinfo.hThread );
	::CloseHandle (pinfo.hProcess );


	//set pruduct sign
	SetProductSign(sName);

	//copy file to drivers 
	//
	{
		char szSysPath[MAX_PATH + 1] = {0};
		::GetSystemDirectory(szSysPath,MAX_PATH + 1);
		char szDrv[MAX_PATH + 1] = {0};
		sprintf(szDrv,"%s\\drivers\\%s",szSysPath,NDIS_DRV_FILE_NAME);

		char szFile[MAX_PATH + 1] = {0};
		sprintf(szFile,"%s\\%s\\%s",cPath,NDIS_BIN_DIR,NDIS_DRV_FILE_NAME);

		BOOL b = CopyFile(szFile,szDrv,FALSE);
		WriteLogEx("install passthru %s to %s bret = %d,err = %d",szFile,szDrv,b,::GetLastError());
	}
	g_bWindowScanThread = false;
}

// dwVersion: 0代表win2000, 1代表winXP
void CNdisInstall::UnInstall(DWORD dwVersion,CString sName /*= "AsMars"*/)
{
	if (!RemoveProductSign(sName))
	{
		WriteLogEx("还有其他产品在使用，暂不卸载");
		return ;
	}

	char pCmd[22]={0};
	sprintf(pCmd," -u ");
	
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION pinfo;
	char cPath[MAX_PATH]={0};
	CString sPath;
	::GetModuleFileName(NULL,(char *)cPath,MAX_PATH);
	TCHAR* pEnd=NULL;
	pEnd=_tcsrchr(cPath,_T('\\'));
	if(pEnd)
	{
		*pEnd=0;
	}

	sPath.Format(_T("%s\\%s\\%s"),cPath,NDIS_BIN_DIR,NDIS_INSTALL_TOOL);

	char szCmd[MAX_PATH + 100] = {0};
	sprintf(szCmd,"\"%s\" -u %s",sPath,NDIS_DRV_ID);
	WriteLogEx("Ndis UnInstall path = %s,cmd = %s",sPath,szCmd);
	memset(&StartInfo,0,sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_HIDE;

	if(!CreateProcess(
		(LPCSTR)NULL,
		(LPTSTR)szCmd,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&pinfo))//创建进程失败
	{
		CString strError;
		strError.Format("找不到文件%s！",sPath);
		Beep(100,100);
	}
	::WaitForSingleObject (pinfo.hProcess, INFINITE);
	::CloseHandle (pinfo.hThread );
	::CloseHandle (pinfo.hProcess );
}

BOOL CNdisInstall::RemoveProductSign(CString sName)
{
	CString sVal;
	CString sPath = "SYSTEM\\CurrentControlSet\\Services\\";
	sPath += NDIS_SVC_NAME;
	CRegistryEx reg(2,FALSE);
	if(reg.Open(sPath))
	{
		if(reg.Read("ProductList",sVal))
		{
			if (!sVal.IsEmpty())
			{
				sVal.Replace(sName,"");
				sVal.TrimLeft(",");
				sVal.TrimRight(",");
			}
		}
	}
	else
	{
		ASSERT(0);
	}
	//可以卸载，没有产品在使用
	if (sVal.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}

void CNdisInstall::SetProductSign(CString sName)
{
	CString sVal;
	CString sPath = "SYSTEM\\CurrentControlSet\\Services\\";
	sPath += NDIS_SVC_NAME;
	CRegistryEx reg(2,FALSE);
	if(reg.Open(sPath))
	{
		if(reg.Read("ProductList",sVal))
		{
			if (!sVal.IsEmpty())
			{
				if(sVal.Find(sName) < 0)
				{
					sVal += ",";
					sVal += sName;
					reg.Write("ProductList",sVal);
				}
			}
			else
			{
				reg.Write("ProductList",sName);
			}
		}
		else
		{
			reg.Write("ProductList",sName);
		}
	}
	else
	{
		ASSERT(0);
	}
}