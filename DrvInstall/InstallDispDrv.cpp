#include "stdafx.h"
#include "InstallDispDrv.h"
#include <setupapi.h>
#include <devguid.h>
#pragma comment(lib, "setupapi.lib")

#include "DriverOp.h"
#include "..\public\include\Comustruct.h"
#include "..\public\include\Fhcode.h"




//常规定义
#define DISP_IO_DRV_NAME	_T("XabDispIo")
#define DISP_IO_DRV_SYS		_T("XabDispIo.sys")
#define DISP_DRV_NAME		_T("XabDisp")
#define DISP_DRV_DLL		_T("XabDisp.dll")
#define DISP_MINI_DRV_NAME	_T("XabDisp")
#define DISP_MINI_DRV_SYS	_T("XabDisp.sys")
#define DISP_DEVICE_NAME	_T("XinAnBao Graphics Adapter")
#define DISP_IO_DEVICE_NAME	_T("\\\\.\\XabDispIo")
#define DISP_PATH			_T("SYSTEM\\CurrentControlSet\\Services\\XabDisp")

#define DISP_IMPORT_ADDRESS_FILE_SHARE                   "XabDisp.dat"
#define DISP_IMPORT_ADDRESS_FILE_SHARE2                  "XabDisp\\XabDisp.dat"

#define DISP_INF			_T("XabDisp.inf")
#define DISP_VISTA_INF		_T("XabDisp_Vista.inf")
#define DISP_KEY_PATH		_T("SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services\\XabDisp")

//#define DISP_HW_ID			_T("PCI\\VEN_XABX&DEV_0001&SUBSYS_00000000&REV_01")
#define DISP_HW_ID			_T("Xab_Mirror")
#ifdef _AMD64_
#define DISP_BIN_DIR		_T("x64\\disp")
#else
#define DISP_BIN_DIR		_T("x86\\disp")
#endif

CInstallHookDisp::CInstallHookDisp(void)
{
}

CInstallHookDisp::~CInstallHookDisp(void)
{
}

void CInstallHookDisp::UpdateDrvFile(CString sPath)
{
	char szSystemDir[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSystemDir,MAX_PATH + 1);
	CString sOpenGLPath,sBackup;
	sBackup.Format("%s\\ASSC.dll",szSystemDir);
	sOpenGLPath.Format("%s\\ASSC.dll",sPath);
	BOOL b = CopyFile(sOpenGLPath,sBackup,FALSE);
	WriteLogEx("CopyFile %d driver %s to %s",b,sOpenGLPath,sBackup);

	sBackup.Format("%s\\ASGL.dll",szSystemDir);
	sOpenGLPath.Format("%s\\ASGL.dll",sPath);
	b = CopyFile(sOpenGLPath,sBackup,FALSE);
	WriteLogEx("CopyFile %d driver %s to %s",b,sOpenGLPath,sBackup);
}

BOOL CInstallHookDisp::InstallDisplayDrv(CString sDrvName,BOOL bEnable)
{
	BOOL bOk = FALSE;
	//get the primary dispaly info
	DISPLAY_DEVICE dd;
	ZeroMemory(&dd,sizeof(dd));
	dd.cb = sizeof(dd);
	char szDrvDesc[MAX_PATH] = {0};
	for(int i = 0; EnumDisplayDevicesA(NULL,i,&dd,0); i++)
	{
		//获取主显示设备
		if( ((dd.StateFlags&DISPLAY_DEVICE_PRIMARY_DEVICE) == DISPLAY_DEVICE_PRIMARY_DEVICE) && 
			((dd.StateFlags&DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) == DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
		{
			strcpy(szDrvDesc,dd.DeviceString);

			WriteLogEx("InstallDisplayDrv DevNum:%d Name:%s String:%s ID:%s Key:%s",
				i,
				&dd.DeviceName[0],
				&dd.DeviceString[0],
				&dd.DeviceID[0],
				&dd.DeviceKey[0]);

			//WriteLogEx("DrvDesc = %s\n",szDrvDesc);
			break;
		}
	}

	//open the display class key
	GUID oDispGUID = GUID_DEVCLASS_DISPLAY;
	HKEY hKey = SetupDiOpenClassRegKey(&oDispGUID, 
		KEY_READ|KEY_WRITE);
	if (hKey != INVALID_HANDLE_VALUE)
	{
		FILETIME time;
		LONG lIndex = 0;
		LONG lRet = ERROR_SUCCESS;
		char szSubKey[MAX_PATH] = {0};
		DWORD dwSubKeyLen = MAX_PATH;
		HKEY hSubKey = NULL;
		do
		{
			dwSubKeyLen = MAX_PATH;
			memset(szSubKey,0,dwSubKeyLen);
			lRet = RegEnumKeyEx(hKey,lIndex,szSubKey,&dwSubKeyLen,0,0,0,&time);
			if(lRet == ERROR_SUCCESS)
			{
				if(RegOpenKeyEx(hKey,szSubKey,0,KEY_ALL_ACCESS,&hSubKey) == ERROR_SUCCESS)
				{
					DWORD dwType = 0;
					char szVal[MAX_PATH] = {0};
					DWORD dwValLen = MAX_PATH;
					if(RegQueryValueEx(hSubKey,_T("DriverDesc"),0,&dwType,(LPBYTE)szVal,&dwValLen) == ERROR_SUCCESS)
					{
						WriteLogEx("DriverDesc = %s",szVal);
						if (stricmp(szDrvDesc,szVal) == 0)
						{
							HKEY hDisplayKey = NULL;

							if(RegOpenKeyEx(hSubKey,_T("Settings"),0,KEY_ALL_ACCESS,&hDisplayKey) == ERROR_SUCCESS)
							{
								DWORD	dwType = 0;
								BYTE	szVal[MAX_PATH] = {0},
									szNew[MAX_PATH] = {0};
								DWORD dwValLen = MAX_PATH;
								if(bEnable)
								{
									if(RegQueryValueEx(hDisplayKey,_T("InstalledDisplayDrivers"),0,&dwType,(LPBYTE)szVal,&dwValLen) == ERROR_SUCCESS)
									{
										WriteLogEx("szVal = %s",szVal);
										char* pCur = (char*)szVal;	
										while(*pCur != '\0')
										{
											int n = strlen(pCur);
											WriteLogEx("szVal = %s",pCur);
											pCur += (n+1);
										}
										pCur = (char*)szVal;
										if(stricmp(pCur,"vga") == 0)
										{
											bOk = TRUE;
											WriteLogEx("InstalledDisplayDrivers vga is atcive %s",pCur);
										}
										else if (stricmp(pCur,sDrvName) == 0)
										{
											bOk = TRUE;
											WriteLogEx("InstalledDisplayDrivers aready install %s",sDrvName);
										}
										else
										{
											//backup the display driver
											char szSystemDir[MAX_PATH + 1] = {0};
											::GetSystemDirectory(szSystemDir,MAX_PATH + 1);
											CString sOpenGLPath,sBackup;
											sBackup.Format("%s\\SysOrgin.dll",szSystemDir);
											sOpenGLPath.Format("%s\\%s.dll",szSystemDir,pCur);
											WriteLogEx("display driver = %s",sOpenGLPath);
											BOOL b = CopyFile(sOpenGLPath,sBackup,FALSE);
											WriteLogEx("display driver = %s to %s %d",sOpenGLPath,sBackup,b);
											if(!b)
											{
												WriteLogEx("copy faile,");
												CString sDel;
												sDel.Format("%s\\SysOrgin-old.dll",szSystemDir);
												b = MoveFileEx(sBackup,sDel,MOVEFILE_REPLACE_EXISTING);
												WriteLogEx("MoveFileEx driver = %s to %s %d",sBackup,sDel,b);
												b = CopyFile(sOpenGLPath,sBackup,FALSE);
												WriteLogEx("display driver = %s to %s %d",sOpenGLPath,sBackup,b);
											}

											RegSetValueEx(hDisplayKey,_T("InstalledDisplayDrivers_backup"),0,dwType,(LPBYTE)szVal,dwValLen);

											memcpy(szNew,sDrvName,strlen(sDrvName));
											dwValLen = strlen(sDrvName) + 2;
											lRet = RegSetValueEx(hDisplayKey,_T("InstalledDisplayDrivers"),0,dwType,(LPBYTE)szNew,dwValLen);
											if(lRet == ERROR_SUCCESS)
											{
												bOk = TRUE;
											}
										}
									}
								}
								else
								{
									if(RegQueryValueEx(hDisplayKey,_T("InstalledDisplayDrivers_backup"),0,&dwType,(LPBYTE)szVal,&dwValLen) == ERROR_SUCCESS)
									{
										WriteLogEx("InstalledDisplayDrivers_backup = %s",szVal);
										lRet = RegSetValueEx(hDisplayKey,_T("InstalledDisplayDrivers"),0,dwType,(LPBYTE)szVal,dwValLen);
										if(lRet == ERROR_SUCCESS)
										{
											//RegSetValueEx(hDisplayKey,_T("InstalledDisplayDrivers_backup"),0,dwType,0,0);
											RegDeleteValue(hDisplayKey,_T("InstalledDisplayDrivers_backup"));
											bOk = TRUE;
										}
									}
									else
									{
										WriteLogEx("RegQueryValueEx InstalledDisplayDrivers_backup error = %d",::GetLastError());
									}
								}
								RegCloseKey(hDisplayKey);

							}
							//fixed by zhandongbao 2011-4-20
							//一块网卡上两个显卡的问题，存在多个显示接口
							//break;
						}
					}
					RegCloseKey(hSubKey);

				}
				lIndex ++;
			}
			else
			{
				break;
			}
		}while(1);

		RegCloseKey(hKey);
	}
	return bOk;
}

BOOL CInstallHookDisp::InstallOpenGLDrv(CString sDrvName, BOOL bEnable)
{
	typedef struct _OPENGL_INFO
	{
		DWORD Version;          
		DWORD DriverVersion;    
		WCHAR DriverName[MAX_PATH];
		DWORD dwResever;
	} OPENGL_INFO, *POPENGL_INFO;
#define OPENGL_CMD      4352        /* for OpenGL ExtEscape */
#define OPENGL_GETINFO  4353        /* for OpenGL ExtEscape */
#define WNDOBJ_SETUP    4354        /* for live video ExtEscape */
#define OPENGL_DRIVERS_SUBKEY _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\OpenGLDrivers")

	BOOL bOk = FALSE;
	HDC hdc = CreateDC("DISPLAY", // driver name
		"", // example 'mirror' device name
		NULL,
		NULL);
	if(!hdc)
		return FALSE;

	DWORD dwInput = 0;
	LONG ret;
	OPENGL_INFO info = {0};
	char szVal[MAX_PATH] = {0};
	CString sDriverName;
	/* get driver name */
	dwInput = OPENGL_GETINFO;
	ret = ExtEscape( hdc, QUERYESCSUPPORT, sizeof (dwInput), (LPCSTR)&dwInput, 0, NULL );
	if (ret > 0)
	{
		dwInput = 0;
		ret = ExtEscape( hdc, OPENGL_GETINFO, sizeof (dwInput),
			(LPCSTR)&dwInput, sizeof (OPENGL_INFO),
			(LPSTR)&info );
		sDriverName = info.DriverName;
		WriteLogEx("GlDrv = %s,ret = %d,%d,%d",sDriverName,ret,dwInput,::GetLastError());
		if(ret > 0 )
		{
			HKEY hKey;
			DWORD type, size;
			CString sKeyPath;
			sKeyPath.Format("%s\\%s",OPENGL_DRIVERS_SUBKEY,sDriverName);
			ret = RegOpenKeyEx( HKEY_LOCAL_MACHINE, sKeyPath, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey );


			if (ret == ERROR_SUCCESS)
			{
				WriteLogEx(szVal);
				if(bEnable)
				{
					/* query value */
					size = sizeof (szVal);
					ret = RegQueryValueEx( hKey, _T("dll"), 0, &type, (LPBYTE)szVal, &size );
					if(ret == ERROR_SUCCESS)
					{
						if(stricmp(szVal,sDrvName) == 0)
						{
							WriteLogEx("aready install opengl %s",szVal);
							bOk = TRUE;
						}
						else
						{
							RegSetValueEx( hKey, _T("dll_backup"), 0, type, (LPBYTE)szVal, size );
							//backup the system opengl driver
							char szSystemDir[MAX_PATH + 1] = {0};
							::GetSystemDirectory(szSystemDir,MAX_PATH + 1);
							CString sOpenGLPath,sBackup;
							sBackup.Format("%s\\SysGl.dll",szSystemDir);
							CString sTmp;
							sTmp.Format("%s",szVal);
							sTmp.MakeLower();
							if(sTmp.Find(".dll") > 0)
							{
								sOpenGLPath.Format("%s\\%s",szSystemDir,szVal);
							}
							else
							{
								sOpenGLPath.Format("%s\\%s.dll",szSystemDir,szVal);

							}
							WriteLogEx("glFile = %s",sOpenGLPath);
							BOOL b = CopyFile(sOpenGLPath,sBackup,FALSE);
							WriteLogEx("opengl display driver = %s to %s %d",sOpenGLPath,sBackup,b);
							if(!b)
							{
								WriteLogEx("copy faile,");
								CString sDel;
								sDel.Format("%s\\SysGL-old.dll",szSystemDir);
								b = MoveFileEx(sBackup,sDel,MOVEFILE_REPLACE_EXISTING);
								WriteLogEx("MoveFileEx driver = %s to %s %d",sBackup,sDel,b);
								b = CopyFile(sOpenGLPath,sBackup,FALSE);
								WriteLogEx("opengl display driver = %s to %s %d",sOpenGLPath,sBackup,b);
							}
							//set our opengl driver to current system
							strcpy(szVal,sDrvName);
							size = strlen(szVal) + 1;
							ret = RegSetValueEx( hKey, _T("dll"), 0, type, (LPBYTE)szVal, size );
							if(ret == ERROR_SUCCESS)
								bOk = TRUE;
						}
					}
					else
					{
						WriteLogEx("RegQueryValueEx dll err = %d",::GetLastError());
					}
				}
				else
				{
					/* query value */
					size = sizeof (szVal);
					ret = RegQueryValueEx( hKey, _T("dll_backup"), 0, &type, (LPBYTE)szVal, &size );
					if(ret == ERROR_SUCCESS)
					{
						//restore opengl driver to current system
						WriteLogEx("OpenGl dll = %s",szVal);
						ret = RegSetValueEx( hKey, _T("dll"), 0, type, (LPBYTE)szVal, size );
						if(ret == ERROR_SUCCESS)
						{
							//RegSetValueEx( hKey, _T("dll_backup"), 0, type, 0, 0);
							RegDeleteValue( hKey, _T("dll_backup"));

							bOk = TRUE;
						}
					}
					else
					{
						WriteLogEx("RegQueryValueEx dll_backup error = %d",::GetLastError());
					}
				}

				RegCloseKey( hKey );
			}
			else
			{
				WriteLogEx("regopenkey opengl err = %d",::GetLastError());
			}
		}
		else
		{
			WriteLogEx("get Opengl error ");
		}
	}

	::DeleteDC(hdc);
	return bOk;
}





//
//////////////////////////////////////////////////////////////////////


#define KEY_TITLE "XAB_"

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

BOOL IsWow64()
{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")),"IsWow64Process");

	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		//AfxMessageBox(_T("aa"));
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
			//AfxMessageBox(_T("errr"));
		}
	}
	return bIsWow64;
}

void StringSplit(const CString sSource, CStringArray *parArray , CString sSplitter,BOOL bTriNUll)
{
	int nSplitLen = sSplitter.GetLength();
	// [11/18/2011 Songqx]
	// 死循环
	if (0 == nSplitLen)
		return;

	CString sTemp(sSource);
	if(sTemp.IsEmpty())
		return ;
	while(sTemp)
	{
		int nSite = sTemp.Find(sSplitter);
		if(nSite == -1)
		{
			parArray->Add(sTemp);
			break;
		}

		CString sNew = sTemp.Left(nSite);
		if(bTriNUll)
		{
			sNew.TrimLeft(" ");
			sNew.TrimLeft("\t");
			sNew.TrimRight(" ");
			sNew.TrimRight("\t");
		}
		parArray->Add(sNew);

		sTemp = (LPCTSTR)sTemp + nSplitLen +nSite;
	}
}

CInstallMirrDisp::CInstallMirrDisp()
{
	m_sSysName = DISP_DEVICE_NAME;
}

CInstallMirrDisp::~CInstallMirrDisp()
{

}
bool CInstallMirrDisp::CheckDispInstalled()
{
	DISPLAY_DEVICE dispDevice;
	FillMemory(&dispDevice, sizeof(DISPLAY_DEVICE), 0);
	dispDevice.cb = sizeof(DISPLAY_DEVICE);
	LPSTR deviceName = NULL;
	HINSTANCE hUser32 = GetModuleHandle(TEXT("USER32"));
	if((!hUser32)||(hUser32==INVALID_HANDLE_VALUE))
	{
		WriteLogEx("bIsAskrnlInstalled hUser32 = NULL");
		return false;
	}
	BOOL     (WINAPI* g_pfnEnumDisplayDevices)(PVOID, DWORD, PDISPLAY_DEVICE,DWORD) = NULL;
	*(FARPROC*)&g_pfnEnumDisplayDevices =GetProcAddress(hUser32,"EnumDisplayDevicesA");


	INT devNum = 0;
	BOOL bFond = FALSE;
	//char* pOur = (char*)DRIVERNAME;
	while (g_pfnEnumDisplayDevices(NULL,
		devNum,
		&dispDevice,
		0))
	{
		//WriteLogEx("bIsAskrnlInstalled(desc = %s,name = %s)",(LPCTSTR)&dispDevice.DeviceString[0],(LPCTSTR)&dispDevice.DeviceName[0]);

		WriteLogEx("DevNum:%d Name:%s String:%s ID:%s Key:%s",
			devNum,
			&dispDevice.DeviceName[0],
			&dispDevice.DeviceString[0],
			&dispDevice.DeviceID[0],
			&dispDevice.DeviceKey[0]);

		if (stricmp((LPCTSTR)&dispDevice.DeviceString[0], m_sSysName) == 0)
		{
			WriteLogEx("find our (%s)",m_sSysName);
			bFond = TRUE;	
			break;
		}		
		devNum++;
	}
	FreeLibrary(hUser32);
	return bFond;
}

bool CInstallMirrDisp::InstallDisp()
{
	if(CheckDispInstalled())
	{
		StopDisp();
		UpdateAllDispFile();
	}
	else
	{
		UpdateAllDispFile();
		RealInstallDisp();
		Sleep(2000);
		int nIndex = 0;
		BOOL bSuc = CheckDispInstalled();
		while (!bSuc)
		{
			nIndex ++;
			::Sleep(1000);
			bSuc = CheckDispInstalled();
			if (nIndex > 2 || bSuc)
			{
				break;
			}
			WriteLogEx("CheckDispInstalled = %d,n = %d",bSuc,nIndex);
		}
		if(bSuc)
		{
			///AfxMessageBox("安装成功");
		}
		else
		{
			WriteLogEx("安装虚拟显卡失败");
			//AfxMessageBox("安装虚拟显卡失败");
		}
	}

	Sleep(400);
	char cSys[MAX_PATH];
	GetSystemDirectory(cSys,MAX_PATH);
	strcat(cSys,"\\MarsDrivers");
	CreateDirectory(cSys,NULL);

	//适用低权限用户访问
	CDriverOp op;
	op.RepairMirrorReg();
	op.AddAccessRights(cSys,GENERIC_ALL|STANDARD_RIGHTS_ALL);
	op.RepairWintMars();


	return true;
}

void CInstallMirrDisp::UpdateAllDispFile()
{
	TCHAR               SysPath[MAX_PATH];
	BOOL bRet = FALSE;
	CString sSource;
	CString sDest;
	CString sDispDrv;

	ZeroMemory(SysPath,MAX_PATH);	
	GetSystemDirectory (SysPath, MAX_PATH) ;
	if(IsWow64())
	{
		sDispDrv.Format(_T("%s\\%s"),m_strLocalPath,DISP_BIN_DIR);
	}
	else
	{
		sDispDrv.Format(_T("%s\\%s"),m_strLocalPath,DISP_BIN_DIR);
	}

	sSource.Format("%s\\%s",sDispDrv,DISP_DRV_DLL);
	sDest.Format(_T("%s\\%s"),SysPath,DISP_DRV_DLL);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

	sSource.Format("%s\\%s",sDispDrv,DISP_MINI_DRV_SYS);
	sDest.Format(_T("%s\\drivers\\%s"),SysPath,DISP_MINI_DRV_SYS);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

	sSource.Format("%s\\%s",sDispDrv,DISP_IO_DRV_SYS);
	sDest.Format(_T("%s\\drivers\\%s"),SysPath,DISP_IO_DRV_SYS);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

	//inf
	SelGetSystemWindowsDirectory(SysPath, MAX_PATH);

	sSource.Format("%s\\%s",sDispDrv,DISP_INF);
	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_INF);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

	sSource.Format("%s\\%s",sDispDrv,DISP_VISTA_INF);
	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_VISTA_INF);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

	sSource.Format("%s\\%s",sDispDrv,DISP_DRV_DLL);
	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_DRV_DLL);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

	sSource.Format("%s\\%s",sDispDrv,DISP_MINI_DRV_SYS);
	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_MINI_DRV_SYS);
	bRet = CopyFile(sSource,sDest,FALSE);
	WriteLogEx("%s to %s %d",sSource,sDest,bRet);

}

void CInstallMirrDisp::StopDisp()
{
	StartMirror(false);
}

bool CInstallMirrDisp::RealInstallDisp()
{
	OSVERSIONINFO stOSVI ;
	ZeroMemory(&stOSVI,sizeof(OSVERSIONINFO)) ;
	stOSVI.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&stOSVI);
	BOOL bIsSucceed = FALSE;
	TCHAR   SysPath[MAX_PATH] = {0};
	TCHAR	szInfPath[MAX_PATH] = {0};

	
	if(stOSVI.dwMajorVersion>=6)//是vista
	{
		GetSystemWindowsDirectory(SysPath, MAX_PATH) ;
		sprintf(szInfPath,_T("%s\\inf\\xabdisp.inf"),SysPath);
		WriteLogEx("安装vista = %s ",szInfPath);
	}
	else
	{
		sprintf(szInfPath,"%s\\%s\\xabdisp.inf",m_strLocalPath,DISP_BIN_DIR);
	}


	CInf inf;
	int code;
	CString sInfo;
	code = inf.Open(szInfPath, FALSE);
	InstallStruct is;
	is.done = FALSE;
	code = inf.EnumManufacturers(MfgCallback, (PVOID) &is);
	CDevInfoSet infoset;
	infoset.Create(&inf, NULL);
	infoset.CreateDeviceInfo(is.devid);
	infoset.RegisterDevice();
	infoset.UpdateDriver();
	return true;

}

void CInstallMirrDisp::DelAllDispFile()
{
	TCHAR               SysPath[MAX_PATH];
	ZeroMemory(SysPath,MAX_PATH);	
	GetSystemDirectory (SysPath, MAX_PATH) ;
	BOOL bRet = FALSE;

	CString sDest;
	sDest.Format(_T("%s\\%s"),SysPath,DISP_DRV_DLL);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);

	sDest.Format(_T("%s\\drivers\\%s"),SysPath,DISP_MINI_DRV_SYS);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);

	sDest.Format(_T("%s\\drivers\\%s"),SysPath,DISP_IO_DRV_SYS);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);


	//inf 
	SelGetSystemWindowsDirectory(SysPath, MAX_PATH);

	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_INF);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);

	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_VISTA_INF);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);

	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_MINI_DRV_SYS);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);

	sDest.Format(_T("%s\\inf\\%s"),SysPath,DISP_DRV_DLL);
	bRet = DeleteFile(sDest);
	WriteLogEx("DeleteFile %s %d",sDest,bRet);


}


BOOL MfgCallback(CInf* inf, const CString& mfgname, const CString& modelname, PVOID context, DWORD& code)
{
	InstallStruct* isp = (InstallStruct*) context;
	isp->mfgname = mfgname;
	code = inf->EnumDevices(modelname, DeviceCallback, context);
	return code == 0 && !isp->done;
}

BOOL DeviceCallback(CInf* inf, const CString& devname, const CString& instsecname, const CStringArray& devid, PVOID context, DWORD& code)
{
	InstallStruct* isp = (InstallStruct*) context;
	int n = devid.GetSize();
	int i = 0;
	WriteLogEx("DeviceCallback guid = %s",isp->devid);
	for (i = 0; i < n; ++i)
	{
		WriteLogEx("DeviceCallback name = %s,guid = %s",devname,devid[i]);
		if (isp->devid == devid[i] || isp->devid.GetLength() == 0)
			break;
	}		
	if (i >= n)
		return TRUE;			// keep enumerating -- this isn't it

	isp->devid = devid[i];
	isp->devname = devname;

	code = 0;					// successful conclusion
	isp->done = TRUE;
	return FALSE;				// stop enumeration
}



void CInstallMirrDisp::StartMirror(BOOL bIsStartDriver)
{
	Sleep(300);

	DEVMODE devmode;
	FillMemory(&devmode, sizeof(DEVMODE), 0);

	devmode.dmSize = sizeof(DEVMODE);
	devmode.dmDriverExtra = 0;

	BOOL change = EnumDisplaySettings(NULL,
		ENUM_CURRENT_SETTINGS,
		&devmode);

	devmode.dmFields = DM_BITSPERPEL |
		DM_PELSWIDTH | 
		DM_PELSHEIGHT;

	if (change) 
	{
		// query all display devices in the system until we hit
		// our favourate mirrored driver, then extract the device name string
		// of the format '\\.\DISPLAY#'

		DISPLAY_DEVICE dispDevice;
		FillMemory(&dispDevice, sizeof(DISPLAY_DEVICE), 0);
		dispDevice.cb = sizeof(DISPLAY_DEVICE);
		LPSTR deviceName = NULL;
		devmode.dmDeviceName[0] = '\0';
		HINSTANCE hUser32 = GetModuleHandle(TEXT("USER32"));
		if((!hUser32)||(hUser32==INVALID_HANDLE_VALUE))
		{
			return;
		}
		BOOL     (WINAPI* g_pfnEnumDisplayDevices)(PVOID, DWORD, PDISPLAY_DEVICE,DWORD) = NULL;
		*(FARPROC*)&g_pfnEnumDisplayDevices =GetProcAddress(hUser32,"EnumDisplayDevicesA");


		INT devNum = 0;
		BOOL bFond = FALSE;

		while (g_pfnEnumDisplayDevices(NULL,
			devNum,
			&dispDevice,
			0))
		{
			WriteLogEx("StartMirror bIsAskrnlInstalled(%s)",(LPCTSTR)&dispDevice.DeviceString[0]);
			WriteLogEx("key = %s",(LPCTSTR)&dispDevice.DeviceKey[0]);
			if (stricmp((LPCTSTR)&dispDevice.DeviceString[0], m_sSysName) == 0)
			{
				WriteLogEx("find our(%s)",m_sSysName);

				bFond = TRUE;
				break;
			}			
			devNum++;
		}
		FreeLibrary(hUser32);
		if (!bFond)
		{
			WriteLogEx("没有安装驱动 %s！",m_sSysName);
			return ;
		}

		while(IsWow64())
		{
			CString sDeviceKey;
			sDeviceKey.Format("%s",(LPCTSTR)&dispDevice.DeviceKey[0]);
			sDeviceKey.MakeLower();
			sDeviceKey.Replace("\\registry\\machine","system\\currentcontrolset\\hardware profiles\\current");

			HKEY hKey = NULL;
			if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,sDeviceKey,0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)
			{
				DWORD one = 0;
				if (RegSetValueEx(hKey,
					_T("Attach.ToDesktop"),
					0,
					REG_DWORD,
					(unsigned char *)&one,sizeof(DWORD)) != ERROR_SUCCESS)
				{
					//::MessageBox(NULL,"Attach.ToDesktop error","",0);
					return ;
				}

				strcpy((LPSTR)&devmode.dmDeviceName[0], DISP_IO_DRV_NAME);
				deviceName = (LPSTR)&dispDevice.DeviceName[0];

				INT code =
					ChangeDisplaySettingsEx(deviceName,
					&devmode, 
					NULL,
					CDS_UPDATEREGISTRY,
					NULL
					);

				code = ChangeDisplaySettingsEx(deviceName,
					&devmode, 
					NULL,
					0,
					NULL
					);

				if(bIsStartDriver)
				{
					//::MessageBox(NULL,"bIsStartDriver","",0);
					DWORD one = 1;

					if (RegSetValueEx(hKey,
						_T("Attach.ToDesktop"),
						0,
						REG_DWORD,
						(unsigned char *)&one,
						4) != ERROR_SUCCESS)
					{
						return ;
					}

					strcpy((LPSTR)&devmode.dmDeviceName[0], DISP_MINI_DRV_NAME);
					deviceName = (LPSTR)&dispDevice.DeviceName[0];
					INT code = ChangeDisplaySettingsEx(deviceName,
						&devmode, 
						NULL,
						CDS_UPDATEREGISTRY,
						NULL
						);
					DISP_CHANGE_SUCCESSFUL;

					code = ChangeDisplaySettingsEx(deviceName,
						&devmode, 
						NULL,
						0,
						NULL
						);
				}

				RegCloseKey(hKey);
				::Sleep(200);
				return ;
			}
			else
			{
				WriteLogEx("can not open the key(%s)",sDeviceKey);
				break;
			}

		}

		CHAR deviceNum[MAX_PATH] = {0};
		LPSTR deviceSub;

		// Simply extract 'DEVICE#' from registry key.  This will depend
		// on how many mirrored devices your driver has and which ones
		// you intend to use.

		_strupr(&dispDevice.DeviceKey[0]);

		deviceSub = strstr(&dispDevice.DeviceKey[0],"\\DEVICE");

		if (!deviceSub) 
			strcpy(&deviceNum[0], "DEVICE0");
		else
			strcpy(&deviceNum[0], ++deviceSub);

		//AfxMessageBox("2");
		HKEY hKeyProfileMirror = (HKEY)0;
		if (RegCreateKey(
			HKEY_LOCAL_MACHINE,
			DISP_KEY_PATH,
			&hKeyProfileMirror) != ERROR_SUCCESS)
		{
			return;
		}
		//AfxMessageBox("3");

		HKEY hKeyDevice = (HKEY)0;
		if (RegCreateKey(hKeyProfileMirror,
			_T(&deviceNum[0]),
			&hKeyDevice) != ERROR_SUCCESS)
		{
			return;
		}


		DWORD one = 0;

		if (RegSetValueEx(hKeyDevice,
			_T("Attach.ToDesktop"),
			0,
			REG_DWORD,
			(unsigned char *)&one,
			4) != ERROR_SUCCESS)
		{
			return;
		}

		strcpy((LPSTR)&devmode.dmDeviceName[0], DISP_IO_DRV_NAME);
		deviceName = (LPSTR)&dispDevice.DeviceName[0];

		INT code =
			ChangeDisplaySettingsEx(deviceName,
			&devmode, 
			NULL,
			CDS_UPDATEREGISTRY,
			NULL
			);

		WriteLogEx("AA ChangeDisplaySettingsEx = %d,err = %d,deviceName = %s",code,::GetLastError(),deviceName);

		code = ChangeDisplaySettingsEx(deviceName,
			&devmode, 
			NULL,
			0,
			NULL
			);

		WriteLogEx("ChangeDisplaySettingsEx = %d,err = %d,deviceName = %s",code,::GetLastError(),deviceName);
		if(bIsStartDriver)
		{
			one = 1;

			if (RegSetValueEx(hKeyDevice,
				_T("Attach.ToDesktop"),
				0,
				REG_DWORD,
				(unsigned char *)&one,
				4) != ERROR_SUCCESS)
			{
				return;
			}

			strcpy((LPSTR)&devmode.dmDeviceName[0], DISP_MINI_DRV_NAME);
			deviceName = (LPSTR)&dispDevice.DeviceName[0];

			INT code =
				ChangeDisplaySettingsEx(deviceName,
				&devmode, 
				NULL,
				CDS_UPDATEREGISTRY,
				NULL
				);


			code = ChangeDisplaySettingsEx(deviceName,
				&devmode, 
				NULL,
				0,
				NULL
				);

			/*
			one=0;
			if (RegSetValueEx(hKeyDevice,
			_T("Attach.ToDesktop"),
			0,
			REG_DWORD,
			(unsigned char *)&one,
			4) != ERROR_SUCCESS)
			{
			return;
			}//
			*/


		}

		RegCloseKey(hKeyProfileMirror);
		RegCloseKey(hKeyDevice);

		//	ASSERT("Performed bit blit.  Finished. \n");
	}
}

BOOL CInstallMirrDisp::StartIoDrv()
{
	SC_HANDLE scm = NULL;
	scm = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if(!scm)
	{
		WriteLogEx("OpenService = %d",::GetLastError());
		return FALSE;
	}
	SC_HANDLE  scv =NULL; 
	scv = OpenService( scm, DISP_IO_DRV_NAME, SERVICE_ALL_ACCESS);
	if (!scv)
	{  
		WriteLogEx("OpenService = %d",::GetLastError());
		CloseServiceHandle( scm );	   
		return FALSE;
	}
	BOOL bRet = StartService(scv,0,NULL);	 
	CloseServiceHandle(scv);	   
	CloseServiceHandle(scm); 
	return bRet;
}

void CInstallMirrDisp::StopIoDrv()
{
	SC_HANDLE	SchSCManager=NULL;
	SERVICE_STATUS  serviceStatus;
	SchSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if (SchSCManager)
	{
		SC_HANDLE  schService=NULL;
		schService = OpenService( SchSCManager, DISP_IO_DRV_NAME, SERVICE_ALL_ACCESS );
		if (schService)
		{	   
			BOOL ret = ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus );  
			CloseServiceHandle( schService );
			WriteLogEx("ControlService stop %d",ret);
		}	   	   
		CloseServiceHandle(SchSCManager); 
	}
}

BOOL  CInstallMirrDisp::CurSysIsWin2k()
{
	OSVERSIONINFO stOSVI ;
	ZeroMemory(&stOSVI,sizeof(OSVERSIONINFO)) ;
	stOSVI.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&stOSVI);

	//if((stOSVI.dwMajorVersion ==5)&&(stOSVI.dwMinorVersion == 1)) //XP or .net
	if((stOSVI.dwMajorVersion >=5)&&(stOSVI.dwMinorVersion >= 1)) //XP or .net
		return FALSE;
	if((stOSVI.dwMajorVersion >=6)) //XP or .net
		return FALSE;
	return TRUE;
}

void CInstallMirrDisp::UninstallDisp()
{
	UnInstallDriver("XabDisp");

	OSVERSIONINFO stOSVI ;
	ZeroMemory(&stOSVI,sizeof(OSVERSIONINFO)) ;
	stOSVI.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&stOSVI);
	if(stOSVI.dwMajorVersion>=6)//vista
	{		
		RemoveHardware(DISP_HW_ID);
	}
	else
	{
		RemoveHardware(DISP_HW_ID);	
	}
}


BOOL CInstallMirrDisp::UnInstallDriver(LPCTSTR lpszDirverSysName)
{
	BOOL status = FALSE;
	CString ss;
	SC_HANDLE serviceControlManager = NULL ;
	SC_HANDLE filespyService = NULL ;
	DWORD errorCode ;

	// Obtain a handle to the service control manager requesting all access

	serviceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!serviceControlManager) 
	{

		errorCode = GetLastError() ;
		ss.Format("The Service Control Manager could not be opened.\n errorCode=%d\n",errorCode) ;
		//AfxMessageBox(ss);
		goto Main_Exit;
	}

	//
	// Open the ASkernel service so that we can delete it
	//
	filespyService = OpenService(serviceControlManager,
		lpszDirverSysName,
		//SERVICE_ALL_ACCESS    // desired access
		SERVICE_STOP | DELETE
		);
	if (!filespyService) {
		errorCode = GetLastError();
		//ss.Format("The ASkernel Service could not be opened.\nerrorCode=%d\n",errorCode);
		//AfxMessageBox(ss);
		goto Main_Exit;
	}

	//Try to stop service first
	SERVICE_STATUS  serviceStatus;
	if(!ControlService( filespyService, SERVICE_CONTROL_STOP, &serviceStatus ))
	{

		errorCode = GetLastError();
		ss.Format("The ASkernel Service could not be Stopped.\nerrorCode=%d\n",errorCode);
		//  AfxMessageBox(ss);
		//    goto Main_Exit;
	};

	//
	// Try to delete the service
	//
	if (!DeleteService(filespyService)) {
		errorCode = GetLastError();
		ss.Format("The ASkernel Service could not be deleted.\nerrorCode=%d\n",errorCode);
		//   AfxMessageBox(ss);
		goto Main_Exit;
	}

	//
	// Display a message indicating that the driver has successfully been
	// uninstalled and the system should be shut down.
	//
	//  AfxMessageBox("Driver successfully uninstalled.\n");
	TRACE("Driver successfully uninstalled.\n\n") ;
	TRACE("For the driver removal to take effect, "
		"the system must now be restarted.\n") ;
	TRACE("Please restart the system after closing "
		"all applications and saving all data\n") ;

Main_Exit:
	if (filespyService) {
		CloseServiceHandle (filespyService) ;
	}
	if (serviceControlManager) {
		CloseServiceHandle (serviceControlManager) ;
	}
	return status;
}

void RegUnloadInf(LPCTSTR lpszRunOnceInfo,LPCTSTR lpszExePath)
{
	HKEY hSoftKey = NULL;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, KEY_ALL_ACCESS, &hSoftKey) == ERROR_SUCCESS)
	{
		RegSetValueEx (hSoftKey, lpszRunOnceInfo, 0L, REG_SZ,
			(CONST BYTE*)lpszExePath, strlen(lpszExePath)+1);
		RegCloseKey(hSoftKey);
	}	
}

void RegDriverStopState()
{
	HKEY hSoftKey = NULL;
	DWORD dwVal = 4;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\XabDisp", 0, KEY_ALL_ACCESS, &hSoftKey) == ERROR_SUCCESS)
	{
		RegSetValueEx (hSoftKey, "Start", 0L, REG_DWORD,
			(CONST BYTE*) &dwVal, sizeof(DWORD));
		RegCloseKey(hSoftKey);
	}
}

int CInstallMirrDisp::RemoveHardware(LPCTSTR lpszHardwareID)
{
	HDEVINFO DeviceInfoSet;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i,err;

	DeviceInfoSet = SetupDiGetClassDevs(NULL,
		0,
		0, 
		DIGCF_ALLCLASSES | DIGCF_PRESENT );
	if (DeviceInfoSet == INVALID_HANDLE_VALUE)
	{       
		WriteLogEx("SetupDiGetClassDevs AA %d",::GetLastError());
		return 1;
	}

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i=0; SetupDiEnumDeviceInfo(DeviceInfoSet,i,&DeviceInfoData); i++)
	{
		DWORD DataT;
		LPTSTR p,buffer = NULL;
		DWORD buffersize = 0;
		DWORD dwErr = 0;
		while (!SetupDiGetDeviceRegistryProperty(
			DeviceInfoSet,
			&DeviceInfoData,
			SPDRP_HARDWAREID,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			dwErr = GetLastError(); 
			if (dwErr == ERROR_INVALID_DATA)
			{
				break;
			}
			else if (dwErr == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) 
					LocalFree(buffer);
				buffer = (char*)LocalAlloc(LPTR,buffersize);
			}
			else
			{
				goto cleanup_DeviceInfo;
			}            
		}

		if (dwErr == ERROR_INVALID_DATA) 
			continue;

		for (p=buffer;*p&&(p<&buffer[buffersize]);p+=lstrlen(p)+sizeof(TCHAR))
		{
			_tprintf(TEXT("Compare device ID: [%s]\n"),p);

			WriteLogEx("HardID = %s,%s",lpszHardwareID,p);
			if (!_tcsicmp(lpszHardwareID,p))
			{
				_tprintf(TEXT("Found! [%s]\n"),p);

				if (!SetupDiCallClassInstaller(DIF_REMOVE,
					DeviceInfoSet,
					&DeviceInfoData))
				{
					WriteLogEx("SetupDiCallClassInstaller error = %d",::GetLastError());
				}
				else
				{
					WriteLogEx("SetupDiCallClassInstaller suc");

					
				}
				break;
			}
		}

		if (buffer) 
			LocalFree(buffer);
	}

	if ((GetLastError()!=NO_ERROR)&&(GetLastError()!=ERROR_NO_MORE_ITEMS))
	{
	}
	SP_DEVINSTALL_PARAMS sp = {0};
	sp.cbSize = sizeof(sp);
	sp.Flags = DI_NEEDREBOOT;
	if(SetupDiGetDeviceInstallParams(DeviceInfoSet,NULL,&sp))
	{
		WriteLogEx("SetupDiGetDeviceInstallParams need rebbot = %d",sp.Flags);
	}
cleanup_DeviceInfo:
	err = GetLastError();
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);

	return !(err == NO_ERROR);
}

void CInstallMirrDisp::InstallIoDrv()
{
	SC_HANDLE	schSCManager=NULL;
	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if (!schSCManager)
	{
		WriteLogEx("InstallWintMars faile(%d)",::GetLastError());
		return ;
	}
	SC_HANDLE  schService = NULL;
	CString sPath;
	char szSysPath[MAX_PATH + 1] = {0};
	::GetSystemDirectory(szSysPath,MAX_PATH + 1);
	sPath.Format("%s\\drivers\\%s",szSysPath,DISP_IO_DRV_SYS);
	schService = CreateService(schSCManager,
		DISP_IO_DRV_NAME,
		DISP_IO_DRV_NAME, 
		SERVICE_ALL_ACCESS, 
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START, 
		SERVICE_ERROR_NORMAL, 
		(LPCTSTR)sPath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (!schService)
	{
		DWORD err = GetLastError();
		WriteLogEx("安装(%s)失败(%d)",DISP_IO_DRV_NAME,err);   
		if (err != ERROR_SERVICE_EXISTS)
		{
			WriteLogEx("%s已经存在",DISP_IO_DRV_NAME);
		}	

	}
	else
	{
		CloseServiceHandle(schService); 
	}
	CloseServiceHandle(schSCManager); 
}

void CInstallMirrDisp::UninstallIoDrv()
{
	SC_HANDLE	schSCManager=NULL;
	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if (!schSCManager)
	{
		WriteLogEx("UninstallIoDrv faile(%d)",::GetLastError());
		return ;
	}
	SC_HANDLE  schService=NULL;
	schService = OpenService(schSCManager,
		DISP_IO_DRV_NAME,
		//SERVICE_ALL_ACCESS    // desired access
		SERVICE_STOP | DELETE
		);
	if (!schService) 
	{
		int errorCode = GetLastError();
		WriteLogEx("UninstallIoDrv IoDrv Service could not be opened.errorCode=%d",errorCode);
		CloseServiceHandle(schSCManager); 
		return;
	}
	else
	{
		//Try to stop service first
		SERVICE_STATUS  serviceStatus;
		if(!ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus ))
		{
			int errorCode = GetLastError();
			WriteLogEx("The UninstallIoDrv Service could not be Stopped.\nerrorCode=%d\n",errorCode);
		}
		if (!DeleteService(schService)) 
		{
			int errorCode = GetLastError();
			WriteLogEx("The UninstallIoDrv Service could not be deleted.\nerrorCode=%d\n",errorCode);
		}
	}

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager); 
}

UINT  CInstallMirrDisp::SelGetSystemWindowsDirectory(OUT LPSTR lpBuffer,IN UINT uSize)
{
	int iNum=GetSystemDirectory(lpBuffer,MAX_PATH);
	if((iNum<0)||(iNum>uSize))
	{
		return 0;
	}
	for(int j=iNum-1;j--;j>=0)
	{
		if(lpBuffer[j]=='\\')
		{
			lpBuffer[j]=0;
			break;
		}
	}
	return iNum;
}

void CInstallMirrDisp::WriteAskInstInfo(char *pTtile)
{
	char cInfo[MAX_PATH];
	sprintf(cInfo,"%s%s",KEY_TITLE,pTtile);
	HKEY hSoftKey=0;	
	DWORD one=1;
	WriteLogEx("WriteAskInstInfo(%s)",pTtile);
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\XabDisp", 0, KEY_ALL_ACCESS, &hSoftKey) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(hSoftKey,
			cInfo,
			0,
			REG_DWORD,
			(unsigned char *)&one,
			4) != ERROR_SUCCESS)
		{
			WriteLogEx("RegSetValueEx faile(%d)",::GetLastError());
		}
		RegCloseKey(hSoftKey);
	}
}

void CInstallMirrDisp::RemoveAskInstInfo(char *pTtile)
{
	HKEY hSoftKey=0;	
	DWORD one=1;
	WriteLogEx("RemoveAskInstInfo(%s)",pTtile);
	char cInfo[MAX_PATH] = {0};
	sprintf(cInfo,"%s%s",KEY_TITLE,pTtile);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\XabDisp", 0, KEY_ALL_ACCESS, &hSoftKey) == ERROR_SUCCESS)
	{
		::RegDeleteValue(hSoftKey, cInfo);
		RegCloseKey(hSoftKey);
	}
}


void CInstallMirrDisp::RemoveAskReg(char * InstInfo)
{
	//char pTtile[MAX_PATH];
	//GetInstName(pTtile);
	RemoveAskInstInfo(InstInfo);	
}

bool CInstallMirrDisp::bAskInstAsSoftAllRemove()
{
	bool bAskAllRemov=true;
	HKEY   m_key;
	char   m_SubKey[255]="SYSTEM\\CurrentControlSet\\Services\\XabDisp";

	DWORD   m_index=0,count=0,dwDataType=0,dwSize=200,dwDataSize=0;
	char   m_name[200],m_Tempname[200];	
	DWORD dvalue=0;
	DWORD   m_attr=REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;	


	int ierror=RegOpenKeyEx(HKEY_LOCAL_MACHINE,m_SubKey,0,KEY_ALL_ACCESS,&m_key);
	if(ierror==ERROR_SUCCESS)
	{
		while((dwDataType=RegEnumValue(m_key,m_index, m_name,&dwSize, NULL,NULL, NULL, NULL))==ERROR_SUCCESS)	 
		{
			m_index++;
			dwSize=200;
			strcpy(m_Tempname,m_name);
			if(strlen(m_Tempname)>3)
			{
				m_Tempname[3]=0;

			}
			if(strcmpi(m_Tempname,KEY_TITLE)==NULL)
			{
				dwDataSize=4;
				if (RegQueryValueEx(m_key, m_name, NULL, &dwDataType, (BYTE *)&dvalue, &dwDataSize)==ERROR_SUCCESS)
				{
					if(dvalue>=1)
					{
						bAskAllRemov=false;
						break;
					}
				}

			}

			memset(m_name,0,200);
		}
		RegCloseKey(m_key);
	}   
	return bAskAllRemov;
}

void CInstallMirrDisp::WriteAskReg(char * InstInfo)
{
	//char pTtile[MAX_PATH];
	//GetInstName(pTtile);
	WriteAskInstInfo(InstInfo);	
}

bool CInstallMirrDisp::GetSysImortFuncAddr(bool bwritefile)
{
	DWORD cbBytesReturned;
	COMMUNICATION_BLOCK cbwintmars;
	COMMUNICATION_IMGSYS_BLOCK cbimgsys;
	memset(&cbwintmars,0,sizeof(COMMUNICATION_BLOCK));
	//memset(&cbimgsys,0,sizeof(COMMUNICATION_IMGSYS_BLOCK));
	BOOL bRet = TRUE;
	bool brefrenceSuc=true;

	HANDLE m_hDrv = CreateFile ( DISP_IO_DEVICE_NAME,//DeviceName,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		0,
		OPEN_ALWAYS,//OPEN_EXISTING
		0,//FILE_ATTRIBUTE_NORMAL,
		NULL );


	if(m_hDrv)
	{
		DWORD cbBytesReturned;
		if(!DeviceIoControl(m_hDrv,IOCTL_FUNCHOOK_GETCOMUADDR,
			NULL,0,
			&cbwintmars,sizeof(COMMUNICATION_BLOCK),
			&cbBytesReturned, NULL))
		{
			return false;
		}
	}

	if(bwritefile)
	{
		//DelTempFile();
		char psyspath[256],fwintmarsdat[256];
		GetSystemDirectory(psyspath,256);
		sprintf(fwintmarsdat,"%s\\%s",psyspath,DISP_IMPORT_ADDRESS_FILE_SHARE);					

		DWORD dcount;
		HANDLE hFile = CreateFile(fwintmarsdat,
			GENERIC_READ|GENERIC_WRITE, 0, NULL,CREATE_ALWAYS,0,0);
		if(INVALID_HANDLE_VALUE==hFile||NULL==hFile)
		{
			sprintf(fwintmarsdat,"%s\\%s",psyspath,DISP_IMPORT_ADDRESS_FILE_SHARE2);								
			hFile = CreateFile(fwintmarsdat,
				GENERIC_READ|GENERIC_WRITE, 0, NULL,CREATE_ALWAYS,0,0);
		}

		WriteFile(hFile,&cbwintmars,sizeof(COMMUNICATION_BLOCK),&dcount,NULL);
		CloseHandle(hFile);

	}
	CloseHandle(m_hDrv);

	return true;
}

void CInstallMirrDisp::DelTempFile()
{
	char psyspath[256];
	GetSystemDirectory(psyspath,256);
	strcat(psyspath,"\\");
	strcat(psyspath,(char *)DISP_IMPORT_ADDRESS_FILE_SHARE);
	DeleteFile(psyspath);

	GetSystemDirectory(psyspath,256);
	strcat(psyspath,"\\");
	strcat(psyspath,(char *)DISP_IMPORT_ADDRESS_FILE_SHARE2);
	DeleteFile(psyspath);
}
