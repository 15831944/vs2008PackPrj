// Hardware.cpp: implementation of the CHardware class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
 
#include "Hardware.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define MOUSE_PATH "{4D36E96F-E325-11CE-BFC1-08002BE10318}"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
typedef CONFIGRET (WINAPI * SELFCM_GET_DEVNODE_STATUS)(PULONG pulStatus, PULONG pulProblemNumber, DEVINST dnDevInst, ULONG ulFlags);

CHardware::CHardware()
{
	TIndex = -1;
	m_hCsLibrary=NULL;
	DrvID = NULL;

}

CHardware::~CHardware()
{

}
 
#define UnknownDevice TEXT("<Unknown Device>")

/////////////////////////////////////////////////////////////////////////////
// CHardware

/*************************************************

    parameter:	DriverID[in]--unique ID of the device in registry
		pVal[out,retval]--£¬return 0 if succeed£¬return 1 if fail
    	2002£­6£­21

**********************************************************/
DWORD CHardware::Disable(BSTR DriverID, long *pVal)
{
	BOOL ShowHidden = 0;
	HDEVINFO hDevInfo = 0;
	long len;
//init the value
	TIndex = -1;
	len = wcstombs(NULL,DriverID,wcslen(DriverID));
	len = len + 1;
	DrvID = (char *)malloc(len);
	memset(DrvID,0,len+1);
	wcstombs(DrvID,DriverID,wcslen(DriverID));


	if (INVALID_HANDLE_VALUE == (hDevInfo = 
                SetupDiGetClassDevs(NULL,NULL,NULL,
                DIGCF_PRESENT|DIGCF_ALLCLASSES)))
				
        {

            *pVal = -1;
			return S_OK;
        }
//get the index of drv in the set
	EnumAddDevices(ShowHidden,hDevInfo);

//disable the drv	
       
//	if ((IsDisableable(TIndex,hDevInfo))&&(!(TIndex==-1)))
	if (!IsDisabled(TIndex,hDevInfo)) 
		if (IsDisableable(TIndex,hDevInfo))
			if (StateChange(DICS_DISABLE,TIndex,hDevInfo))
				*pVal = 0;
			else
				*pVal = -1;			
		else
			*pVal = 1;			
	else 
		*pVal = 0;

	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return S_OK;
}

BOOL CHardware::EnumAddDevices(BOOL ShowHidden, HDEVINFO hDevInfo)
{
	DWORD i, Status, Problem;

    LPTSTR IOName=NULL;
	DWORD len=0;
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
	
  

    //
    // Enumerate though all the devices.
    //
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
        //
        // Should we display this device, or move onto the next one.
        //
        if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                    DeviceInfoData.DevInst,0))
        {
            
            continue;
        }

        if (!(ShowHidden || !((Status & DN_NO_SHOW_IN_DM) || 
            IsClassHidden(&DeviceInfoData.ClassGuid))))
            continue;



        //
        // Get a friendly name for the device.
        //
			
        ConstructDeviceName(hDevInfo,&DeviceInfoData,
							&IOName,
							(PULONG)&len);
		TRACE("%s   %s %d\n",IOName,DrvID,DeviceInfoData.DevInst);
		char cinfo[MAX_PATH],cpath[MAX_PATH];		
		if (strcmp(IOName,DrvID) == 0)
		{
			sprintf(cpath,"SYSTEM\\CurrentControlSet\\Control\\Class\\%s",DrvID);
		    //GetClassDevId(cpath,cinfo);
			TIndex = i;
			
			return TRUE;
		}
	}
	return TRUE;
   
}


 
BOOL CHardware::IsClassHidden(GUID *ClassGuid)
{
	BOOL bHidden = FALSE;
    HKEY hKeyClass;

    //
    // If the devices class has the NoDisplayClass value then
    // don't display this device.
    //
    if (hKeyClass = SetupDiOpenClassRegKey(ClassGuid,KEY_READ))
    {
        bHidden = (RegQueryValueEx(hKeyClass, //close
              REGSTR_VAL_NODISPLAYCLASS, 
              NULL, NULL, NULL, NULL) == ERROR_SUCCESS);
        RegCloseKey(hKeyClass);
    }                                 

    return bHidden;
}

BOOL CHardware::ConstructDeviceName(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, PVOID Buffer, PULONG Length)
{
	if (!GetRegistryProperty(DeviceInfoSet,
        DeviceInfoData,
        SPDRP_DRIVER ,
        Buffer,
        Length))
    {
        if (!GetRegistryProperty(DeviceInfoSet,
            DeviceInfoData,
            SPDRP_DEVICEDESC ,
            Buffer,
            Length))
        {
            if (!GetRegistryProperty(DeviceInfoSet,
                DeviceInfoData,
                SPDRP_CLASS ,
                Buffer,
                Length))
            {
                if (!GetRegistryProperty(DeviceInfoSet,
                    DeviceInfoData,
                    SPDRP_CLASSGUID ,
                    Buffer,
                    Length))
                {
                     *Length = (_tcslen(UnknownDevice)+1)*sizeof(TCHAR);
                     Buffer =(char *)malloc(*Length);
                     _tcscpy(*(LPTSTR *)Buffer,UnknownDevice);
                }
            }
        }

    }

    
	return TRUE;
}

BOOL CHardware::GetRegistryProperty(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, ULONG Property, PVOID Buffer, PULONG Length)
{
	while (!SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
        DeviceInfoData,
        Property,
        NULL,
        (BYTE *)*(TCHAR **)Buffer,
        *Length,
        Length
        ))
    {

        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            //
            // We need to change the buffer size.
            //
            if (*(LPTSTR *)Buffer) 
                LocalFree(*(LPTSTR *)Buffer);
            *(LPTSTR *)Buffer = (PCHAR)LocalAlloc(LPTR,*Length);
        }
        else
        {
            //
            // Unknown Failure.
            //
        
            return FALSE;
        }            
    }

    return (*(LPTSTR *)Buffer)[0];

}

BOOL CHardware::StateChange(DWORD NewState, DWORD SelectedItem, HDEVINFO hDevInfo)
{
	SP_PROPCHANGE_PARAMS PropChangeParams = {sizeof(SP_CLASSINSTALL_HEADER)};
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
    
    //
    // Get a handle to the Selected Item.
    //
    if (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))
    {

        return FALSE;
    }

    //
    // Set the PropChangeParams structure.
    //
    PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
    PropChangeParams.Scope = DICS_FLAG_GLOBAL;
    PropChangeParams.StateChange = NewState; 

    if (!SetupDiSetClassInstallParams(hDevInfo,
        &DeviceInfoData,
        (SP_CLASSINSTALL_HEADER *)&PropChangeParams,
        sizeof(PropChangeParams)))
    {

    
        return FALSE;
    }

    //
    // Call the ClassInstaller and perform the change.
    //
    if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,
        hDevInfo,
        &DeviceInfoData))
    {

        
        return TRUE;
    }

    
    return TRUE;
}

BOOL CHardware::IsDisabled(DWORD SelectedItem, HDEVINFO hDevInfo)
{
	SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
    DWORD Status, Problem;

    //
    // Get a handle to the Selected Item.
    //
    if (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))
    {

        return FALSE;
    }

    if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                DeviceInfoData.DevInst,0))
    {

        return FALSE;
    }

    return ((Status & DN_HAS_PROBLEM) && (CM_PROB_DISABLED == Problem)) ;
}

BOOL CHardware::IsDisableable(DWORD SelectedItem, HDEVINFO hDevInfo)
{
	SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
    DWORD Status, Problem;


    //
    // Get a handle to the Selected Item.
    //
    if (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))
    {

        return FALSE;
    }

    if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                DeviceInfoData.DevInst,0))
    {

        return FALSE;
    }

    return ((Status & DN_DISABLEABLE) && 
        (CM_PROB_HARDWARE_DISABLED != Problem));
}

DWORD CHardware::Enable(BSTR DriverID, long *pVal)
{

	BOOL ShowHidden = 0;
	HDEVINFO hDevInfo = 0;
	long len;
//init the value
	TIndex = -1;
	len = wcstombs(NULL,DriverID,wcslen(DriverID));
	len = len + 1;
	DrvID = (char *)malloc(len);
	memset(DrvID,0,len+1);
	wcstombs(DrvID,DriverID,wcslen(DriverID));


	if (INVALID_HANDLE_VALUE == (hDevInfo = 
                SetupDiGetClassDevs(NULL,NULL,NULL,
                DIGCF_PRESENT|DIGCF_ALLCLASSES)))
				
        {

            *pVal = -1;
			return S_OK;
        }
//get the index of drv in the set
	EnumAddDevices(ShowHidden,hDevInfo);

//disable the drv	
       
	if (IsDisabled(TIndex,hDevInfo))
		if (StateChange(DICS_ENABLE,TIndex,hDevInfo))
			*pVal = 0;
		else
			*pVal = -1;	
	else
		*pVal = 0;	
  
	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return S_OK;
}



void CHardware::DisableUsbMouse()
{
 
	SYSTEMTIME st1,st2;
	::GetSystemTime(&st1);
	

	if(!IsNt())
	{
		return ;
	}
	HDEVINFO hDevInfo = 0;
	long len;
 
	int ShowHidden=0;
	
	if (INVALID_HANDLE_VALUE == (hDevInfo = 
		SetupDiGetClassDevs(NULL,NULL,NULL,
		DIGCF_PRESENT|DIGCF_ALLCLASSES)))
		
	{
		return ;
	}
	::GetSystemTime(&st2);
	TRACE("%d---%d---\n",st2.wSecond-st1.wSecond,st2.wMilliseconds-st1.wMilliseconds);

	DWORD i, Status, Problem;

    LPTSTR IOName=NULL;
	len=0;
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
	
  

    //
    // Enumerate though all the devices.
    //
	
	char cHeader[MAX_PATH];
	int ilength=strlen(MOUSE_PATH);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
        //
        // Should we display this device, or move onto the next one.
        //
		Sleep(1);
        if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                    DeviceInfoData.DevInst,0))
        {
            
            continue;
        }

        if (!(ShowHidden || !((Status & DN_NO_SHOW_IN_DM) || 
            IsClassHidden(&DeviceInfoData.ClassGuid))))
            continue;



        //
        // Get a friendly name for the device.
        //
			
        ConstructDeviceName(hDevInfo,&DeviceInfoData,
							&IOName,
							(PULONG)&len);
	
		strcpy(cHeader,IOName);
		if(strlen(cHeader)>=ilength)
		{
            cHeader[ilength]=0;
			
		}
		TRACE("%s   %s\n",cHeader,MOUSE_PATH);

		char cinfo[MAX_PATH],cpath[MAX_PATH];		
		if (stricmp(cHeader,MOUSE_PATH) == 0)
		{
			TIndex = i;		
		
			sprintf(cpath,"SYSTEM\\CurrentControlSet\\Control\\Class\\%s",IOName);
		    GetClassDevId(cpath,cinfo);
            if((stricmp(cinfo,"hid_device_system_mouse")==0)||
				(stricmp(cinfo,"hid\\vid_062a&pid_0000")==0))
			{
				if (!IsDisabled(TIndex,hDevInfo)) 
				{
					if (IsDisableable(TIndex,hDevInfo))
					{
						if (StateChange(DICS_DISABLE,TIndex,hDevInfo))
						{
							
						}
					}
				}
			}
			//return TRUE;
		}
	}

	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return  ;
}

void CHardware::EnableUsbMouse()
{
		if(!IsNt())
	{
		return ;
	}
	HDEVINFO hDevInfo = 0;
	long len;
 	int ShowHidden=0;
	
	
	if (INVALID_HANDLE_VALUE == (hDevInfo = 
		SetupDiGetClassDevs(NULL,NULL,NULL,
		DIGCF_PRESENT|DIGCF_ALLCLASSES)))
		
	{
		return ;
	}

	DWORD i, Status, Problem;

    LPTSTR IOName=NULL;
	  len=0;
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
	
  

    //
    // Enumerate though all the devices.
    //

	char cHeader[MAX_PATH];
	int ilength=strlen(MOUSE_PATH);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
        //
        // Should we display this device, or move onto the next one.
        //
        if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                    DeviceInfoData.DevInst,0))
        {
            
            continue;
        }

        if (!(ShowHidden || !((Status & DN_NO_SHOW_IN_DM) || 
            IsClassHidden(&DeviceInfoData.ClassGuid))))
            continue;



        //
        // Get a friendly name for the device.
        //
			
        ConstructDeviceName(hDevInfo,&DeviceInfoData,
							&IOName,
							(PULONG)&len);
		TRACE("%s   %s\n",IOName,DrvID);
		strcpy(cHeader,IOName);
		if(strlen(cHeader)>=ilength)
		{
            cHeader[ilength]=0;
			
		}
		if (stricmp(cHeader,MOUSE_PATH) == 0)
		{
			TIndex = i;
			if (IsDisabled(TIndex,hDevInfo))
			{
				if (StateChange(DICS_ENABLE,TIndex,hDevInfo))			
				{
					//return TRUE;
				}
			}
		}
	}

	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return  ;
}

void CHardware::GetClassDevId(char * pPath,char *pinfo)
{
	BOOL bSuc = FALSE;
    HKEY hKeyClass;

    //
    // If the devices class has the NoDisplayClass value then
    // don't display this device.
    //
	if(ERROR_SUCCESS==RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,         // handle to open key
		pPath,  // subkey name
		0,   // reserved
		KEY_QUERY_VALUE, // security access mask
		&hKeyClass    // handle to open key
		))
	{
    
		DWORD dtype=REG_SZ;
		DWORD dLen= MAX_PATH;
        bSuc = (RegQueryValueEx(hKeyClass, //close
              REGSTR_VAL_MATCHINGDEVID, 
              NULL, &dtype, (BYTE *)pinfo,&dLen) == ERROR_SUCCESS);
        RegCloseKey(hKeyClass);
    }                                 

    return  ;
}

bool CHardware::bExistUsbMouse()
{
	HDEVINFO hDevInfo = 0;
	long len;
 	int ShowHidden=0;

	bool bret=false;
	if (INVALID_HANDLE_VALUE == (hDevInfo = 
		SetupDiGetClassDevs(NULL,NULL,NULL,
		DIGCF_PRESENT|DIGCF_ALLCLASSES)))
		
	{
		return false;
	}

	DWORD i, Status, Problem;

    LPTSTR IOName=NULL;
	  len=0;
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
	
  

    //
    // Enumerate though all the devices.
    //

	char cHeader[MAX_PATH];
	int ilength=strlen(MOUSE_PATH);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
        //
        // Should we display this device, or move onto the next one.
        //
        if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                    DeviceInfoData.DevInst,0))
        {
            
            continue;
        }

        if (!(ShowHidden || !((Status & DN_NO_SHOW_IN_DM) || 
            IsClassHidden(&DeviceInfoData.ClassGuid))))
            continue;



        //
        // Get a friendly name for the device.
        //
			
        ConstructDeviceName(hDevInfo,&DeviceInfoData,
							&IOName,
							(PULONG)&len);
		TRACE("%s   %s\n",IOName,DrvID);
		strcpy(cHeader,IOName);
		if(strlen(cHeader)>=ilength)
		{
            cHeader[ilength]=0;
			
		}
		 
		if (stricmp(cHeader,MOUSE_PATH) == 0)
		{
			TIndex = i;
			/*
			
			*/
			if (IsDisabled(TIndex,hDevInfo))
			{		
	       
				bret= true;
			}
			else
			{
		 
			}
		}
	}
	
	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return  bret;
}

void CHardware::KeepCursorStart(bool bStart)
{
	if(!IsNt())
	{
		return ;
	}

	typedef int (*CS_StartCallLine)(int isKeyboard);
	typedef int (*CS_StopCallLine)(int isKeyboard);
	CS_StartCallLine csstart=0;
	CS_StopCallLine  csstop=0;
 
	
	if(bStart)
	{
		if(bExistUsbMouse())
		{
			//AfxMessageBox("usbmouse");
			
			if(m_hCsLibrary==NULL)
			{
				TCHAR pszModuleName[MAX_PATH]="";
				if( GetModuleFileName(AfxGetApp()->m_hInstance,pszModuleName,MAX_PATH))
				{
					_TCHAR* pEndChar = _tcsrchr(pszModuleName,'\\');
					if(pEndChar)
					{
						*(pEndChar+1) = 0;
					}
				}
				strcat(pszModuleName,"CursorShow.dll");
				//AfxMessageBox(pszModuleName);
				m_hCsLibrary = (HINSTANCE)LoadLibrary(pszModuleName);			
			}
			if(m_hCsLibrary)
			{
				csstart = (CS_StartCallLine)GetProcAddress (m_hCsLibrary, "MH_StartCallLine");
				if(csstart)
				{
					csstart(0);
				}
				
			}
		}
	}
	else
	{
		//AfxMessageBox("111");
		if(m_hCsLibrary)
		{
			//AfxMessageBox("222");
            csstop = (CS_StopCallLine)GetProcAddress (m_hCsLibrary, "MH_StopCallLine");
			if(csstop)
			{
				//AfxMessageBox("333");
				 csstop(0);
			}
			::FreeLibrary(m_hCsLibrary);			
			m_hCsLibrary=NULL;
		}
		
	}
}


bool CHardware::IsNt()
{
	OSVERSIONINFO ov;  
	ov.dwOSVersionInfoSize = sizeof(ov); 
	GetVersionEx(&ov); 
	if((ov.dwMajorVersion == 4) &&(ov.dwMinorVersion == 10))
	{
 
	  return false;
	}
	else
	{
 
   	    return true;
	}
}

CONFIGRET CHardware::ASCM_Get_DevNode_Status(PULONG pulStatus, PULONG pulProblemNumber, DEVINST dnDevInst, ULONG ulFlags)
{
	CONFIGRET status=CR_DEFAULT;
    HINSTANCE hInst = LoadLibraryA ( "SETUPAPI.DLL" ) ;
    if (! hInst ) return CR_DEFAULT ;
    SELFCM_GET_DEVNODE_STATUS sefFunc;
    sefFunc =(SELFCM_GET_DEVNODE_STATUS)GetProcAddress ( hInst ,
                                                "CM_Get_DevNode_Status" ) ;
	if(sefFunc)
	{
		status=sefFunc(pulStatus, pulProblemNumber, dnDevInst, ulFlags);
	}
     
    FreeLibrary(hInst);
    return status;
}

void CHardware::RefReshUsbMouse()
{
	SYSTEMTIME st1,st2;
	::GetSystemTime(&st1);
	

	if(!IsNt())
	{
		return ;
	}
	HDEVINFO hDevInfo = 0;
	long len;
 
	int ShowHidden=0;
	
	if (INVALID_HANDLE_VALUE == (hDevInfo = 
		SetupDiGetClassDevs(NULL,NULL,NULL,
		DIGCF_PRESENT|DIGCF_ALLCLASSES)))
		
	{
		return ;
	}
	::GetSystemTime(&st2);
	TRACE("%d---%d---\n",st2.wSecond-st1.wSecond,st2.wMilliseconds-st1.wMilliseconds);

	DWORD i, Status, Problem;

    LPTSTR IOName=NULL;
	len=0;
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
 
	//DeviceInfoData.ClassGuid={4D36E96F-E325-11CE-BFC1-08002BE10318};
	GUID GU;
	
	GU.Data1=0x4D36E96F;
	GU.Data2=0xE325;
	GU.Data3=0x11CE;
	GU.Data4[0]=0xBF;
	GU.Data4[1]=0xC1;
	GU.Data4[2]=0x08;
	GU.Data4[3]=0x00;
	GU.Data4[4]=0x2B;
	GU.Data4[5]=0xE1;
	GU.Data4[6]=0x03;
	GU.Data4[7]=0x18;
	
	
  

    //
    // Enumerate though all the devices.
    //
	
	char cHeader[MAX_PATH];
	int ilength=strlen(MOUSE_PATH);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
        //
        // Should we display this device, or move onto the next one.
        //
		if(GU!=DeviceInfoData.ClassGuid)
		{
              continue;
		}
		//Sleep(1);
        if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                    DeviceInfoData.DevInst,0))
        {
            
            continue;
        }

        if (!(ShowHidden || !((Status & DN_NO_SHOW_IN_DM) || 
            IsClassHidden(&DeviceInfoData.ClassGuid))))
            continue;



        //
        // Get a friendly name for the device.
        //
			
        ConstructDeviceName(hDevInfo,&DeviceInfoData,
							&IOName,
							(PULONG)&len);
	
		strcpy(cHeader,IOName);
		if(strlen(cHeader)>=ilength)
		{
            cHeader[ilength]=0;
			
		}
		TRACE("%s   %s\n",cHeader,MOUSE_PATH);

		char cinfo[MAX_PATH],cpath[MAX_PATH];		
		if (stricmp(cHeader,MOUSE_PATH) == 0)
		{
			TIndex = i;		
		
			sprintf(cpath,"SYSTEM\\CurrentControlSet\\Control\\Class\\%s",IOName);
		    GetClassDevId(cpath,cinfo);
            if((stricmp(cinfo,"hid_device_system_mouse")==0)||
				(stricmp(cinfo,"hid\\vid_062a&pid_0000")==0))
			{
				    if(!IsHasProblem(TIndex, hDevInfo))
					{
						if (!IsDisabled(TIndex,hDevInfo)) 
						{
							if (IsDisableable(TIndex,hDevInfo))
							{
								if (StateChange(DICS_DISABLE,TIndex,hDevInfo))
								{
									StateChange(DICS_ENABLE,TIndex,hDevInfo);							
								}
								//break;
							}
						}
					}
					else
					{
						RemoveDev(TIndex,hDevInfo);

					}
			}
			//return TRUE;
		}
	}

	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return  ;
}

BOOL CHardware::IsHasProblem(DWORD SelectedItem, HDEVINFO hDevInfo)
{
	SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
    DWORD Status, Problem;

    //
    // Get a handle to the Selected Item.
    //
    if (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))
    {

        return FALSE;
    }

    if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                DeviceInfoData.DevInst,0))
    {

        return FALSE;
    }
	

    return (Status & DN_HAS_PROBLEM);
}

bool CHardware::RemoveDev(DWORD SelectedItem, HDEVINFO hDevInfo)
{
	SP_REMOVEDEVICE_PARAMS  RemoveDeviceParams = {sizeof(SP_CLASSINSTALL_HEADER)};
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
    
    //
    // Get a handle to the Selected Item.
    //
    if (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))
    {

        return FALSE;
    }

    //
    // Set the PropChangeParams structure.
    //
    RemoveDeviceParams.ClassInstallHeader.InstallFunction = DIF_REMOVE;
	RemoveDeviceParams.Scope=DI_UNREMOVEDEVICE_CONFIGSPECIFIC ;
	RemoveDeviceParams.HwProfile=0;
 


    if (!SetupDiSetClassInstallParams(hDevInfo,
        &DeviceInfoData,
        (SP_CLASSINSTALL_HEADER *)&RemoveDeviceParams,
        sizeof(RemoveDeviceParams)))
    {
        int ierror=::GetLastError();
    
        return FALSE;
    }

    //
    // Call the ClassInstaller and perform the change.
    //
    if (!SetupDiCallClassInstaller(DIF_REMOVE,
        hDevInfo,
        &DeviceInfoData))
    {

        
        return FALSE;
    }

    
    return TRUE;
}

void CHardware::RemoveProblemMouse()
{
	SYSTEMTIME st1,st2;
	::GetSystemTime(&st1);
	

	if(!IsNt())
	{
		return ;
	}
	HDEVINFO hDevInfo = 0;
	long len;
 
	int ShowHidden=0;
	
	if (INVALID_HANDLE_VALUE == (hDevInfo = 
		SetupDiGetClassDevs(NULL,NULL,NULL,
		DIGCF_PRESENT|DIGCF_ALLCLASSES)))
		
	{
		return ;
	}
	::GetSystemTime(&st2);
	TRACE("%d---%d---\n",st2.wSecond-st1.wSecond,st2.wMilliseconds-st1.wMilliseconds);

	DWORD i, Status, Problem;

    LPTSTR IOName=NULL;
	len=0;
    SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
 
	//DeviceInfoData.ClassGuid={4D36E96F-E325-11CE-BFC1-08002BE10318};
	GUID GU;
	
	GU.Data1=0x4D36E96F;
	GU.Data2=0xE325;
	GU.Data3=0x11CE;
	GU.Data4[0]=0xBF;
	GU.Data4[1]=0xC1;
	GU.Data4[2]=0x08;
	GU.Data4[3]=0x00;
	GU.Data4[4]=0x2B;
	GU.Data4[5]=0xE1;
	GU.Data4[6]=0x03;
	GU.Data4[7]=0x18;
	
	
  

    //
    // Enumerate though all the devices.
    //
	
	char cHeader[MAX_PATH];
	int ilength=strlen(MOUSE_PATH);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {
        //
        // Should we display this device, or move onto the next one.
        //
		if(GU!=DeviceInfoData.ClassGuid)
		{
              continue;
		}
		//Sleep(1);
        if (CR_SUCCESS != ASCM_Get_DevNode_Status(&Status, &Problem,
                    DeviceInfoData.DevInst,0))
        {
            
            continue;
        }

        if (!(ShowHidden || !((Status & DN_NO_SHOW_IN_DM) || 
            IsClassHidden(&DeviceInfoData.ClassGuid))))
            continue;



        //
        // Get a friendly name for the device.
        //
			
        ConstructDeviceName(hDevInfo,&DeviceInfoData,
							&IOName,
							(PULONG)&len);
	
		strcpy(cHeader,IOName);
		if(strlen(cHeader)>=ilength)
		{
            cHeader[ilength]=0;
			
		}
		TRACE("%s   %s\n",cHeader,MOUSE_PATH);

		char cinfo[MAX_PATH],cpath[MAX_PATH];		
		if (stricmp(cHeader,MOUSE_PATH) == 0)
		{
			TIndex = i;		
		
			sprintf(cpath,"SYSTEM\\CurrentControlSet\\Control\\Class\\%s",IOName);
		    GetClassDevId(cpath,cinfo);
            if((stricmp(cinfo,"hid_device_system_mouse")==0)||
				(stricmp(cinfo,"hid\\vid_062a&pid_0000")==0))
			{
				    if(IsHasProblem(TIndex, hDevInfo))
					{
						RemoveDev(TIndex,hDevInfo);						 
					}
			}
			//return TRUE;
		}
	}

	if(hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);
	return  ;
}
