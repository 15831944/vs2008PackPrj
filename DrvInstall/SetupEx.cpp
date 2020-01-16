// Setup.cpp -- Helper classes for setup programs
// Copyright (C) 1999 by Walter Oney
// All rights reserved


#include "StdAfx.h"
#include "SetupEx.h"


FARPROC pfShowWindow; 
BYTE OldShowWindowCode[5],NewShowWindowCode[5]; 
#define THREADTYPE unsigned int (__stdcall *)(void *)


static BOOL ShowWindowHookOff();//将所属进程中JMP MyMessageBoxA的代码改为Jmp MessageBoxA 
static BOOL ShowWindowHookOn() ;
static BOOL WINAPI  HOOKShowWindow(HWND hWnd,int nCmdShow);

static HANDLE g_hProc=NULL;
bool g_dlgShow=true;

#define MIRROR_INST_TILE      "没有找到数字签名"

#define MIRROR_INST_TILE_2003 "硬件安装"

 

DWORD GetField(PINFCONTEXT ic, DWORD index, CString& value);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CInf::CInf()
{							// CInf::CInf
	m_hinf = INVALID_HANDLE_VALUE;
}							// CInf::CInf

CInf::~CInf()
{							// CInf::~CInf
	if (m_hinf)
		SetupCloseInfFile(m_hinf);
}							// CInf::~CInf

///////////////////////////////////////////////////////////////////////////////

DWORD CInf::EnumDevices(const CString& modelname, PENUMDEVCALLBACK callback, PVOID context)
{							// CInf::EnumDevices
	INFCONTEXT mod;
	DWORD code;
	
	if (!SetupFindFirstLine(m_hinf, modelname, NULL, &mod))
	{
		WriteLogEx("CInf::EnumDevices SetupFindFirstLine err = %d",::GetLastError());
		return GetLastError();
	}	
	do	
	{						// for each model
		CString devname, instname, id;
		CStringArray idlist;
		
		if ((code = GetField(&mod, 0, devname)) != 0
			|| (code = GetField(&mod, 1, instname)) != 0)
		{
			WriteLogEx("CInf::EnumDevices GetField err = %d",::GetLastError());
			return code;
		}

		int nfields = SetupGetFieldCount(&mod);
		
		for (int i = 2; i <= nfields; ++i)
		{
			if (GetField(&mod, i, id) == 0)
			{
				WriteLogEx("CInf::EnumDevices %s",id);
				idlist.Add(id);
			}
			else
				break;
		}
		if (idlist.GetSize() == 0)
			idlist.Add(_T(""));
		
		if (!(*callback)(this, devname, instname, idlist, context, code))
		{
			WriteLogEx("CInf::EnumDevices callback err = %d",::GetLastError());

			return code;
		}
	}						// for each model
	while (SetupFindNextLine(&mod, &mod));
	
	return ERROR_NO_MORE_ITEMS;
}							// CInf::EnumDevices

///////////////////////////////////////////////////////////////////////////////

DWORD CInf::EnumManufacturers(PENUMMFGCALLBACK callback, PVOID context)
{							// CInf::EnumManufacturers
	INFCONTEXT mfg;				// context for manufacturer section scan
	DWORD code = 0;
	
	if (!SetupFindFirstLine(m_hinf, _T("Manufacturer"), NULL, &mfg))
	{
		WriteLogEx("SetupFindFirstLine faile");
		return GetLastError();
	}	
	// Loop throught the manufacturer section, calling the specified callback
	// function for each one
	
	do	
	{						// for each manufacturer
		CString mfgname, 
				modelname,
				pltname;
		if ((code = GetField(&mfg, 0, mfgname)) != 0
			|| (code = GetField(&mfg, 1, modelname)) != 0)
		{
			WriteLogEx("EnumManufacturers GetField faile");
			return code;
		}	
		DWORD dwCount = SetupGetFieldCount(&mfg);
		WriteLogEx("mfgname = %s,moelname = %s,%d,%d,%d",mfgname,modelname,mfg.Section,mfg.Line,dwCount);

		if(dwCount > 1)
		{
			CString sModePltName;
			for(DWORD dwIdx = 2 ; dwIdx <= dwCount; dwIdx ++)
			{
				code = GetField(&mfg, dwIdx, pltname);
				WriteLogEx("plt = %s,%d",pltname,dwIdx);
				
				sModePltName.Format("%s.%s",modelname,pltname);
			
				// Invoke callback function. It returns TRUE if we should continue
				// the enumeration or FALSE if we should stop.
				
				if (!(*callback)(this, mfgname, sModePltName, context, code))
				{
					WriteLogEx("EnumManufacturers callback faile err = %d",::GetLastError());
					//return code;
					continue;
				}
			}
		}
		else
		{
			// Invoke callback function. It returns TRUE if we should continue
			// the enumeration or FALSE if we should stop.

			if (!(*callback)(this, mfgname, modelname, context, code))
			{
				WriteLogEx("EnumManufacturers callback faile err = %d",::GetLastError());
				return code;
			}
		}

	}						// for each manufacturer
	while (SetupFindNextLine(&mfg, &mfg));
	
	return ERROR_NO_MORE_ITEMS;
}							// CInf::EnumManufacturers

///////////////////////////////////////////////////////////////////////////////
CString ConverGuidToStr(GUID guid)
{
	LPOLESTR   pstrGuid;   
	int   i   =   StringFromCLSID(guid,   &pstrGuid);     
	CString   s(pstrGuid);    
	if   (NULL   !=   pstrGuid)           
		CoTaskMemFree   (pstrGuid);
	return s;
}

DWORD CInf::Open(LPCTSTR name, BOOL defsearch /* = TRUE */)
{							// CInf::Open
	
							/*	CString infname(name);
							if (infname[0] == '"')
							infname = infname.Mid(2, infname.GetLength() - 2); // remove quotes from name
							
							  // If we're not to use the default search path for INF files (namely, directories
							  // named in HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\DevicePath [i.e.,
							  // the INF directory], make sure we present a pathname rather than just a
							  // name to setup functions
							  
								if (!defsearch && _tcschr(infname, _T('\\')) == NULL)
								m_name = _T(".\\");
								m_name += infname;
	*/
	m_name = name;
	m_hinf = SetupOpenInfFile(name, NULL, INF_STYLE_WIN4, NULL);
	if (m_hinf == INVALID_HANDLE_VALUE)
	{
		WriteLogEx("SetupOpenInfFile  name = %s faile ",name);
		return GetLastError();
	}	
	// Determine the class name and GUID for this file
	
	TCHAR classname[64] = {0};
	if (!SetupDiGetINFClass(name, &m_guid, classname, arraysize(classname), NULL))
	{
		DWORD dw = GetLastError();
		WriteLogEx("SetupDiGetINFClass faile dw = %d",dw);
		return dw;
	}
	WriteLogEx("classname = %s,",classname);
	m_classname = classname;
	DWORD junk;
//	if (m_guid == GUID_NULL) //aaaaaaaa
	BOOL b = SetupDiClassGuidsFromName(classname, &m_guid, 1, &junk);

	WriteLogEx("SetupDiClassGuidsFromName = %d,guid = %s",b,ConverGuidToStr(m_guid));
	return 0;
}							// CInf::Open

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CDevInfoSet::CDevInfoSet()
{							// CDevInfoSet::CDevInfoSet
	m_hinfo = INVALID_HANDLE_VALUE;
	memset(&m_devinfo, 0, sizeof(m_devinfo));
	m_devinfo.cbSize = sizeof(m_devinfo);

	m_registered = FALSE;
	m_hModule=NULL;
	m_dwIdOld=NULL;
	m_dwIdNew=NULL; 

	m_hWinThread=NULL;
	m_ThreadID=0;
	m_bStop=false;
	m_EventHandle = NULL;

}							// CDevInfoSet::CDevInfoSet

CDevInfoSet::~CDevInfoSet()
{							// CDevInfoSet::~CDevInfoSet
	UnregisterDevice();
	if (m_hinfo != INVALID_HANDLE_VALUE)
		SetupDiDestroyDeviceInfoList(m_hinfo);
}							// CDevInfoSet::~CDevInfoSet

DWORD WINAPI windThread(PVOID pvoid)
{
	DWORD rt=0;
	CDevInfoSet * pDevInfo=(CDevInfoSet * )pvoid;
	CFrameWnd frmWnd;
	frmWnd.Create(NULL,NULL);	
	UINT Itimer=frmWnd.SetTimer(1,30,NULL);
	//AfxMessageBox("test");
	bool bover=false;
	while(!bover)
	{
		//rt = MsgWaitForMultipleObjects(1,&pDevInfo->m_EventHandle,FALSE,INFINITE,QS_ALLINPUT);
		//if(rt==WAIT_OBJECT_0+1)
		{
			bool binput=false;
			MSG msg;
			int inum=0;
			//if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			if(::GetMessage(&msg,NULL,0,0))
			{				
				if(WM_TIMER==msg.message)
				{
					frmWnd.KillTimer(Itimer);
					HWND hWnd = ::FindWindow("#32770","Windows 安全");
					if (NULL != hWnd)
					{			
						SetForegroundWindow(hWnd);

						BYTE dExInfo=0;						
						keybd_event('i', /*1376257*/0,1,dExInfo);//按 'c'键
						//::PostMessage(hWnd,WM_KEYDOWN,'i',0);
						//::PostMessage(hWnd,WM_KEYUP,'i',0);
						binput=true;
						inum++;
						Sleep(100);
						if(inum>=10)
						{
							bover=true;
							break;
						}

					}
					else
					{
						if(binput)
						{
							bover=true;
							break;
						}

					}
					frmWnd.SetTimer(1,30,NULL);
				}
				if(pDevInfo->m_bStop)
				{
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}	
			//	return false;
		}
		//if(rt==WAIT_OBJECT_0)
		{
			//	break;			 
		}
	}

	return 0;
}
void CDevInfoSet::CloseWindowThread()
{	
	if(m_EventHandle)
	{
		SetEvent (m_EventHandle);
		CloseHandle(m_EventHandle);
	}
	m_bStop=true;

	if(m_hWinThread)
	{
		WaitForSingleObject(m_hWinThread,3000);
		CloseHandle(m_hWinThread);
		m_hWinThread=NULL;
	}


}
void CDevInfoSet::CreateWindowThread()
{

	OSVERSIONINFO stOSVI ;
	ZeroMemory(&stOSVI,sizeof(OSVERSIONINFO)) ;
	stOSVI.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&stOSVI);


	if(stOSVI.dwMajorVersion>=6)
	{

		m_EventHandle=::CreateEvent(NULL,TRUE,FALSE,NULL);
		m_hWinThread = CreateThread ( 0, 0, windThread, this, CREATE_SUSPENDED,  &m_ThreadID);
		ResumeThread(m_hWinThread);	
	}
}
///////////////////////////////////////////////////////////////////////////////

DWORD CDevInfoSet::Create(CInf* inf, HWND hwnd)
{							// CDevInfoSet::Create
	m_inf = inf;
	m_hwnd = hwnd;
	
	m_hinfo = SetupDiCreateDeviceInfoList(&m_inf->m_guid, hwnd);
	if (m_hinfo == INVALID_HANDLE_VALUE)
	{
		DWORD dw = ::GetLastError();
		WriteLogEx("SetupDiCreateDeviceInfoList %d",dw);
		return dw;
	}	
	return 0;
}							// CDevInfoSet::Create

///////////////////////////////////////////////////////////////////////////////

DWORD CDevInfoSet::CreateDeviceInfo(const CString& devid)
{							// CDevInfoSet::CreateDeviceInfo
	if (!SetupDiCreateDeviceInfo(m_hinfo, m_inf->m_classname, &m_inf->m_guid,
		NULL, m_hwnd, DICD_GENERATE_ID, &m_devinfo))
	{
		DWORD dw = ::GetLastError();
		WriteLogEx("SetupDiCreateDeviceInfo %d",dw);
		return dw;
	}	
	WriteLogEx("CreateDeviceInfo devid = %s",devid);
	m_devid = devid;
	DWORD size = (devid.GetLength() + 2) * sizeof(TCHAR);
	PBYTE hwid = new BYTE[size];
	memset(hwid, 0, size);
	memcpy(hwid, devid, size - 2 * sizeof(TCHAR));
	
	BOOL okay = SetupDiSetDeviceRegistryProperty(m_hinfo, &m_devinfo, SPDRP_HARDWAREID,
		hwid, size);
	
	delete[] hwid;
	
	if (okay)
	{
		WriteLogEx("SetupDiSetDeviceRegistryProperty err = %d",GetLastError());
		return 0;
	}	
	return GetLastError();
}							// CDevInfoSet::CreateDeviceInfo

DWORD GetField(PINFCONTEXT ic, DWORD index, CString& value)
{							// GetField
	DWORD need;
	DWORD code;
	
	if (!SetupGetStringField(ic, index, NULL, 0, &need))
		return GetLastError();
	
	LPTSTR buffer = value.GetBuffer(need);
	if (SetupGetStringField(ic, index, buffer, need, NULL))
		code = 0;
	else
	{
		code = GetLastError();
		buffer[0] = 0;
	}
	
	value.ReleaseBuffer();
	return code;
}

///////////////////////////////////////////////////////////////////////////////

DWORD CDevInfoSet::RegisterDevice()
{							// CDevInfoSet::RegisterDevice
	if (!SetupDiCallClassInstaller(DIF_REGISTERDEVICE, m_hinfo, &m_devinfo))
	{
		DWORD dw = GetLastError();
		WriteLogEx("SetupDiCallClassInstaller err = %d",dw);
		return dw;
	}
	WriteLogEx("RegisterDevice success");
	m_registered = TRUE;
	return 0;
}							// CDevInfoSet::RegisterDevice

///////////////////////////////////////////////////////////////////////////////

DWORD CDevInfoSet::UnregisterDevice()
{							// CDevInfoSet::UnregisterDevice
	if (m_registered)
	{
		WriteLogEx("UnregisterDevice");
		if (!SetupDiCallClassInstaller(DIF_REMOVE, m_hinfo, &m_devinfo))
		{
			DWORD dw = GetLastError();
			WriteLogEx("SetupDiCallClassInstaller err = %d",dw);
			return dw;
		}
		WriteLogEx("UnregisterDevice success");
	}
	return 0;
}							// CDevInfoSet::UnregisterDevice

///////////////////////////////////////////////////////////////////////////////

DWORD CDevInfoSet::UpdateDriver(bool bShowDlg)
{							// CDevInfoSet::UpdateDriver
	
	
	InitFunction(bShowDlg);
	CreateWindowThread();

	BOOL reboot = FALSE;
	DWORD derror=0;
	DWORD dInstType=0;//INSTALLFLAG_FORCE;//
	WriteLogEx("UpdateDriver %s,%s",m_devid,m_inf->m_name);
	if (!UpdateDriverForPlugAndPlayDevices(m_hwnd, m_devid, m_inf->m_name,
		dInstType, &reboot))
	{
		ERROR_FILE_NOT_FOUND;
		derror=GetLastError();
		WriteLogEx("UpdateDriverForPlugAndPlayDevices = %d,guid = %s,name = %s",derror,m_devid,m_inf->m_name);

		
		goto RET;
	}
	WriteLogEx("UpdateDriver success bReboot = %d",reboot);
	m_registered = FALSE;		// prevent unregistration when done
RET:
	CloseWindowThread();
	ExitFunction();
	return derror;
	/*
	BOOL reboot = FALSE;
	if (!UpdateDriverForPlugAndPlayDevices(m_hwnd, m_devid, m_inf->m_name,
		INSTALLFLAG_FORCE, &reboot))
		return GetLastError();
	
	m_registered = FALSE;		// prevent unregistration when done
	return 0;
	*/
}							// CDevInfoSet::UpdateDriver
bool CDevInfoSet::InitFunction(bool bShow)
{

	return 0;
	g_dlgShow=bShow;

    m_hModule=LoadLibrary("user32.dll"); 
	if(!m_hModule)
		return 0;
	/*
    pfDialogBoxParamW=GetProcAddress(m_hModule,"DialogBoxParamW"); 
    if(pfDialogBoxParamW==NULL) 
      return false; 
    _asm 
    { 
        lea edi,OldDialogBoxParamWCode 
        mov esi,pfDialogBoxParamW
        cld 
        movsd 
        movsb 
    } 
    NewDialogBoxParamWCode[0]=0xe9;//jmp MyMessageBoxA的相对地址的指令 
    _asm 
    { 
        lea eax,HOOKDialogBoxParamW
        mov ebx,pfDialogBoxParamW
        sub eax,ebx 
        sub eax,5 
        mov dword ptr [NewDialogBoxParamWCode+1],eax 
    }  

    */
#if 0
	pfShowWindow=GetProcAddress(m_hModule,"ShowWindow"); 
    if(pfShowWindow==NULL) 
      return false; 
    _asm 
    { 
        lea edi,OldShowWindowCode 
        mov esi,pfShowWindow
        cld 
        movsd 
        movsb 
    } 
    NewShowWindowCode[0]=0xe9;//jmp MyMessageBoxA的相对地址的指令 
    _asm 
    { 
        lea eax,HOOKShowWindow
        mov ebx,pfShowWindow
        sub eax,ebx 
        sub eax,5 
        mov dword ptr [NewShowWindowCode+1],eax 
    }  
	
 

    m_dwIdNew=GetCurrentProcessId(); //得到所属进程的ID 
    m_dwIdOld=m_dwIdNew;   
	g_hProc=OpenProcess(PROCESS_ALL_ACCESS,0,m_dwIdOld);
	//DialogBoxParamWHookOn();//开始拦截 
	ShowWindowHookOn();//开始拦截 
#endif

}

void CDevInfoSet::ExitFunction()
{
	return ;
	ShowWindowHookOff(); 
   
   if(g_hProc)
   {
	   CloseHandle(g_hProc);
   }
}
BOOL ShowWindowHookOff()//将所属进程中JMP MyMessageBoxA的代码改为Jmp MessageBoxA 
{ 
	BOOL bsuc=true;
    HANDLE hProc=g_hProc; 
	DWORD doldInfo,dTemp;
    //dwIdOld=dwIdNew; 
    //hProc=OpenProcess(PROCESS_ALL_ACCESS,0,dwIdOld); 
	//if(hProc!=NULL)
	//{
	bsuc=VirtualProtectEx(hProc,pfShowWindow,5,PAGE_READWRITE,&doldInfo); 
	bsuc=bsuc&&WriteProcessMemory(hProc,pfShowWindow,OldShowWindowCode,5,0); 
	bsuc=bsuc&&VirtualProtectEx(hProc,pfShowWindow,5,doldInfo,&dTemp); 
	return bsuc;
	//CloseHandle(hProc);
	//}
    //bHook=false; 
} 

BOOL ShowWindowHookOn() 
{ 

	BOOL bsuc=true;
    HANDLE hProc=g_hProc; 
	DWORD doldInfo,dTemp;
    //dwIdOld=dwIdNew; 
    //hProc=OpenProcess(PROCESS_ALL_ACCESS,0,dwIdOld);//得到所属进程的句柄 
	//if(hProc!=NULL)
	//{	
	bsuc=VirtualProtectEx(hProc,pfShowWindow,5,PAGE_READWRITE,&doldInfo);//修改所属进程中MessageBoxA的前5个字节的属性为可写 
	bsuc=bsuc&&WriteProcessMemory(hProc,pfShowWindow,NewShowWindowCode,5,0);//将所属进程中MessageBoxA的前5个字节改为JMP 到MyMessageBoxA 
	bsuc=bsuc&&VirtualProtectEx(hProc,pfShowWindow,5,doldInfo,&dTemp);//修改所属进程中MessageBoxA的前5个字节的属性为原来的属性 
	return bsuc;
	//	CloseHandle(hProc);
	//}
    ///bHook=true; 
 
} 

BOOL WINAPI  HOOKShowWindow(HWND hWnd,int nCmdShow)
{
	if(!ShowWindowHookOff())
	{
		return false;
	}
	
	///*
	BOOL bShow=true,bneedClickYes=false;
	char lpString[256];
	GetWindowText(hWnd,lpString,256);

	//AfxMessageBox(lpString);

	TRACE("%s;\n",lpString);
#pragma message("!!!!!!!!!!!!!!!!!!!国际化时,这里要处理")
	bool bIs2KTtile=(strcmp(lpString,MIRROR_INST_TILE)==0);
	bool bIsWin2003Ttile=(strcmp(lpString,MIRROR_INST_TILE_2003)==0);
	if((bIsWin2003Ttile)||(bIs2KTtile))
	{
		bShow=false;		
		bneedClickYes=true;
		if(!g_dlgShow)
		{
			nCmdShow=false;
		}
	//	LogShow(lpString);
    }
	
	
	//LogShow(lpString);
	//Beep(100,100);
	//*/
	bShow=ShowWindow(hWnd,nCmdShow);
	
	if(bneedClickYes)
	{
		
		BYTE dExInfo=0;

		if(bIsWin2003Ttile)
		{
			keybd_event(67, /*1376257*/0,1,dExInfo);//按 'c'键
		}	

        if(bIs2KTtile)
		{
			::PostMessage(hWnd,WM_KEYDOWN,13,0);
			//keybd_event(89, /*1376257*/0,1,dExInfo);//按 'Y'键
		}		
		
	}
	ShowWindowHookOn();
	return bShow;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
