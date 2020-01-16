#include "StdAfx.h"
#include "WinFireWallOp.h"
#include "FileLog.h"

CWinFireWallOp::CWinFireWallOp(void)
{
	
}

CWinFireWallOp::~CWinFireWallOp(void)
{
}

void CWinFireWallOp::Install(CString sPath,CString sParam,DWORD dwWait /* = TRUE*/,DWORD dwShowCmd)
{
	CFileLog theLog;
//	theLog.EnableLog(TRUE);
	theLog.SetLogName("Install");

	int nLen = sPath.GetLength() + sParam.GetLength() + 10;
	char* pszPath = new char[nLen];
	ASSERT(pszPath);
	memset(pszPath,0,nLen);

	sprintf(pszPath,"\"%s\" %s",sPath,sParam);
	STARTUPINFO si = { sizeof si, 0, "" };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = dwShowCmd;

	theLog.Write("Install pathandParam = %s",pszPath);
	PROCESS_INFORMATION pi;
	if(CreateProcess(0, 
		pszPath,// command line
		0, 0,  // security attributes
		FALSE, // handle inheritance
		0,     // creation flags
		0,     // environment block
		0,     // current directory
		&si, &pi ))
	{
		theLog.Write("createprocess pid = %d",pi.dwProcessId);
		//	if(bWait)
		{
			::WaitForSingleObject(pi.hProcess,dwWait);
		}
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
	else
	{
		CString s;
		s.Format("Install Ê§°Ü err = %d \n",::GetLastError());
		theLog.Write(s);
	}
	if (pszPath)
	{
		delete pszPath;
		pszPath = NULL;
	}
}

void CWinFireWallOp::CloseFireWall()
{
	char szSysDir[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSysDir,MAX_PATH + 1);

	CString sPath;
	sPath.Format("%s\\%s",szSysDir,"netsh.exe");

	CString sParam = "firewall set opmode disable";

	Install(sPath,sParam);
}
void CWinFireWallOp::EnableExceptMode()
{
	char szSysDir[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSysDir,MAX_PATH + 1);

	CString sPath;
	sPath.Format("%s\\%s",szSysDir,"netsh.exe");

	CString sParam = "firewall set opmode mode=ENABLE exception=enable ";

	Install(sPath,sParam);
}
void CWinFireWallOp::ExceptProgram(CString sProgram)
{
	char szSysDir[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSysDir,MAX_PATH + 1);

	CString sPath;
	sPath.Format("%s\\%s",szSysDir,"netsh.exe");

	CString sParam;
	if (CCommonFun::IsWinVistaOrLater())
	{
		sParam.Format("advfirewall firewall add rule name=\"iSecStar\" dir=in protocol=any program=\"%s\" action=allow",sProgram);
	}
	else
	{
		sParam.Format("firewall set allowedprogram \"%s\" iSecStar enable",sProgram);;
	}
	Install(sPath,sParam);
}
void CWinFireWallOp::ExceptUDPPort(UINT nPort)
{
	char szSysDir[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSysDir,MAX_PATH + 1);

	CString sPath;
	sPath.Format("%s\\%s",szSysDir,"netsh.exe");

	CString sParam;
	sParam.Format("netsh firewall set portopening udp %d enable ",nPort);

	Install(sPath,sParam);
}
void CWinFireWallOp::ExceptTCPPort(UINT nPort)
{
	char szSysDir[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSysDir,MAX_PATH + 1);

	CString sPath;
	sPath.Format("%s\\%s",szSysDir,"netsh.exe");

	CString sParam;
	sParam.Format("netsh firewall set portopening tcp %d enable ",nPort);
	Install(sPath,sParam);
}

BOOL CWinFireWallOp::IsWinFireWallEnabled()
{
	CString sPath("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile"); 
	CRegistryEx reg(2,TRUE);
	if (reg.Open(sPath))
	{
		DWORD dwVal = 0;
		if(reg.Read("EnableFirewall",dwVal) && (dwVal == 1))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWinFireWallOp::IsExceptModeEnabled()
{
	CString sPath("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile"); 
	CRegistryEx reg(2,TRUE);
	if (reg.Open(sPath))
	{
		DWORD dwVal = 0;
		if(reg.Read("DoNotAllowExceptions",dwVal) && (dwVal == 1))
		{
			return FALSE;
		}
	}
	return TRUE;
}