// Hardware.h: interface for the CHardware class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDWARE_H__0D6A3B7B_189F_4D11_ADEF_382DCE7282F7__INCLUDED_)
#define AFX_HARDWARE_H__0D6A3B7B_189F_4D11_ADEF_382DCE7282F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RegStr.h"
#include "cfgmgr32.h"
#include "Setupapi.h"
#pragma comment(lib,"Setupapi.lib")
//typedef void * HDEVINFO;
class CHardware  
{
public:
	void RemoveProblemMouse();
	bool RemoveDev( DWORD SelectedItem, HDEVINFO hDevInfo);
	BOOL IsHasProblem(DWORD SelectedItem, HDEVINFO hDevInfo);
	void RefReshUsbMouse();
	CONFIGRET ASCM_Get_DevNode_Status(PULONG   pulStatus,PULONG   pulProblemNumber,DEVINST  dnDevInst,ULONG    ulFlags);
	bool IsNt();
	HINSTANCE m_hCsLibrary;
	void KeepCursorStart(bool bStart);
	bool bExistUsbMouse();
	void GetClassDevId(char * pPath,char * pinfo);
	void EnableUsbMouse();
	void DisableUsbMouse();
	char * DrvID;
	int TIndex;
	CHardware();
	virtual ~CHardware();

	DWORD Disable(BSTR DriverID, long *pVal);

	BOOL EnumAddDevices(BOOL ShowHidden, HDEVINFO hDevInfo);
    BOOL IsClassHidden(GUID *ClassGuid);
    BOOL ConstructDeviceName(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, PVOID Buffer, PULONG Length);
    BOOL GetRegistryProperty(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, ULONG Property, PVOID Buffer, PULONG Length);
    BOOL StateChange(DWORD NewState, DWORD SelectedItem, HDEVINFO hDevInfo);
    BOOL IsDisabled(DWORD SelectedItem, HDEVINFO hDevInfo);

    BOOL IsDisableable(DWORD SelectedItem, HDEVINFO hDevInfo);
	DWORD Enable(BSTR DriverID, long *pVal);


};

#endif // !defined(AFX_HARDWARE_H__0D6A3B7B_189F_4D11_ADEF_382DCE7282F7__INCLUDED_)
