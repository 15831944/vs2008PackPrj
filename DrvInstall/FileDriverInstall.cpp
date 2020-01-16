#include "stdafx.h"
#include "FileDriverInstall.h"
#include "NtDriverController.h"

#define SFTEST_WIN32_DRIVER_NAME "Soren"

BOOL CFileDriverInstall::Install()
{
	CNtDriverController ntDriver(SFTEST_WIN32_DRIVER_NAME);
	return ntDriver.InstallAndStart();
}

BOOL CFileDriverInstall::UnInstall()
{
	CNtDriverController ntDriver(SFTEST_WIN32_DRIVER_NAME);
	return ntDriver.StopAndRemove();
}

BOOL	__declspec(dllexport) InstallFileFilter()
{
	return CFileDriverInstall::Install();
}

BOOL	__declspec(dllexport) UnInstallFileFilter()
{
	return CFileDriverInstall::UnInstall();
}