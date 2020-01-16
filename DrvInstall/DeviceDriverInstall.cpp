#include "StdAfx.h"
#include "DeviceDriverInstall.h"
#include "NtDriverController.h"
#include	<devguid.h>

#define DEVDRIVER_WIN32_DRIVER_NAME "ISecDev"

const GUID classguid[] = {GUID_DEVCLASS_DISKDRIVE, GUID_DEVCLASS_CDROM};

CDeviceDriverInstall::CDeviceDriverInstall(void)
{
}

CDeviceDriverInstall::~CDeviceDriverInstall(void)
{
}

BOOL CDeviceDriverInstall::installDeviceFilter()
{
	if (!InstallDriver())
		return FALSE;

	BOOL bRet = TRUE;
	for (int i = 0; i < sizeof(classguid) / sizeof(REFGUID); i++)
	{
		bRet &= AddLowerFilter(classguid[i]);
	}

	return bRet;
}

BOOL CDeviceDriverInstall::uninstallDeviceFilter()
{
	if (!UninstallDriver())
		return FALSE;

	BOOL bRet = TRUE;
	for (int i = 0; i < sizeof(classguid) / sizeof(REFGUID); i++)
	{
		bRet &= RemoveLowerFilter(classguid[i]);
	}

	return bRet;
}
BOOL CDeviceDriverInstall::InstallDriver()
{
	CNtDriverController ntDriver(DEVDRIVER_WIN32_DRIVER_NAME);
	DWORD status = ERROR_SUCCESS;
	CString str;
	TCHAR windowsDirectory[MAX_PATH];
	TCHAR ImageFilePath[MAX_PATH];
	LPCTSTR lpFileName;
	ZeroMemory(windowsDirectory, sizeof(windowsDirectory));
	ZeroMemory(ImageFilePath,MAX_PATH);

	//lpSrv=;
	lpFileName= DEVDRIVER_WIN32_DRIVER_NAME".sys";

	_tcscpy(ImageFilePath,_T("System32\\Drivers\\"));
	_tcscat(ImageFilePath, lpFileName);
	if (!ntDriver.Init(DEVDRIVER_WIN32_DRIVER_NAME))
	{
		return FALSE;
	}

	status = ntDriver.AddDevice(ImageFilePath, SERVICE_BOOT_START);

	if ((status != ERROR_SUCCESS) && (status != ERROR_SERVICE_EXISTS))
	{
		str.Format("status:%d", status);
		return FALSE;
	}
	return TRUE;
}

LPTSTR SearchMultiSz(LPTSTR Value, ULONG ValueLength, LPCTSTR SearchString)
{
	if(ValueLength < _tcslen(SearchString)*sizeof(TCHAR))
		return NULL;

	SIZE_T length = ValueLength - _tcslen(SearchString)*sizeof(TCHAR);
	do
	{
		if (!_tcsicmp(&Value[length/sizeof(TCHAR)], SearchString))
			return &Value[length];

	}

#if 0
	while(--length);
#else
	while(length--);
#endif

	return NULL;
}

BOOL CDeviceDriverInstall::AddLowerFilter(REFGUID classguid)
{
	DWORD guidListSize = 0;
	DWORD status = 0;
	LPCTSTR lpSrv = DEVDRIVER_WIN32_DRIVER_NAME;

	if (!SetupDiBuildClassInfoList(0, NULL, 0, &guidListSize))
	{
		status = GetLastError();

		if (status != ERROR_INSUFFICIENT_BUFFER)
		{			
			return FALSE;
		}
	}
	else
	{		
		return FALSE;
	}

	LPGUID guidList = new GUID[guidListSize];
	if (guidList == NULL)
	{		
		return FALSE;
	}

	if (SetupDiBuildClassInfoList(0, guidList, guidListSize, &guidListSize))
	{
		for (DWORD index = 0; index < guidListSize; ++index)
		{
			CString ss;
			ss.Format(_T("index=%d"),index);
			if(IsEqualGUID(classguid, guidList[index]))
			{
				HKEY hKey = SetupDiOpenClassRegKey(&guidList[index], 
					KEY_READ|KEY_WRITE);
				if (hKey != INVALID_HANDLE_VALUE)
				{
					TCHAR filters[MAX_PATH] = {0x0};
					DWORD filtersLength = sizeof(filters);
					DWORD valueType;

					status = RegQueryValueEx(
						hKey,
						_T("LowerFilters"),
						0,
						&valueType,
						(PBYTE)filters,
						&filtersLength
						);
					if (status == ERROR_SUCCESS)
					{
						LPTSTR p = SearchMultiSz(filters, filtersLength, 
							lpSrv);

						if (p == NULL)
						{
							_tcscpy(&filters[filtersLength/sizeof(TCHAR) - 1], lpSrv);
							filtersLength += _tcslen(lpSrv)*sizeof(TCHAR);
							filters[filtersLength/sizeof(TCHAR)] = 0;
							filters[filtersLength/sizeof(TCHAR) + 1] = 0;

							RegSetValueEx(
								hKey,
								_T("LowerFilters"),
								0,
								REG_MULTI_SZ,
								(PBYTE)filters,
								filtersLength + sizeof(TCHAR)
								);

						}
					}
					else if (status == ERROR_FILE_NOT_FOUND)
					{
						_tcscpy(filters, lpSrv);
						filtersLength = _tcslen(lpSrv)*sizeof(TCHAR);
						filters[filtersLength/sizeof(TCHAR)] = 0;
						filters[filtersLength/sizeof(TCHAR) + 1] = 0;

						status = RegSetValueEx(
							hKey,
							_T("LowerFilters"),
							0,
							REG_MULTI_SZ,
							(PBYTE)filters,
							filtersLength + 2*sizeof(TCHAR)
							);
					}
					RegCloseKey(hKey);
				}
				break;
			}
		}
	}
	delete[] guidList;
	return TRUE;
}

BOOL CDeviceDriverInstall::UninstallDriver()
{
	CNtDriverController ntDriver(DEVDRIVER_WIN32_DRIVER_NAME);
	return ntDriver.RemoveService();
}

BOOL CDeviceDriverInstall::RemoveLowerFilter(REFGUID classguid)
{
	DWORD status = ERROR_SUCCESS;
	DWORD guidListSize = 0;

	if (!SetupDiBuildClassInfoList(0, NULL, 0, &guidListSize))
	{
		status = GetLastError();

		if (status != ERROR_INSUFFICIENT_BUFFER)
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	LPGUID guidList = new GUID[guidListSize];
	if (guidList == NULL)
	{
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	TCHAR classDesc[MAX_PATH] = {0x0};
	TCHAR filters[MAX_PATH] = {0x0};
	DWORD filtersLength = sizeof(filters);
	DWORD valueType;
	HKEY hKey;
	if (SetupDiBuildClassInfoList(0, guidList, guidListSize, &guidListSize))
	{
		for (DWORD index = 0; index < guidListSize; ++index)
		{
			if (SetupDiGetClassDescription(&guidList[index], 
				classDesc, 
				sizeof(classDesc), 
				NULL))
			{
				if(IsEqualGUID(classguid, guidList[index]))
				{
					hKey= SetupDiOpenClassRegKey(&guidList[index], 
						KEY_READ|KEY_WRITE);
					if (hKey != INVALID_HANDLE_VALUE)
					{

						status = RegQueryValueEx(hKey,
							_T("LowerFilters"), 
							0, 
							&valueType, 
							(PBYTE)filters, 
							&filtersLength);
						if (status == ERROR_SUCCESS)
						{
							RegDeleteValue(hKey,_T("LowerFilters"));    
						}
						else if (status == ERROR_FILE_NOT_FOUND)
						{
							status = ERROR_SUCCESS;
						}

						RegCloseKey(hKey);
					}
					break;
				}
			}
		}
	}
	delete[] guidList;
	return (ERROR_SUCCESS == status);
}

BOOL	__declspec(dllexport) InstallDeviceFilter()
{
	return CDeviceDriverInstall::installDeviceFilter();
}

BOOL	__declspec(dllexport) UnInstallDeviceFilter()
{
	return CDeviceDriverInstall::uninstallDeviceFilter();
}