#pragma once

class CDeviceDriverInstall
{
public:
	CDeviceDriverInstall(void);
	~CDeviceDriverInstall(void);
public:
	static BOOL	installDeviceFilter();
	static BOOL	uninstallDeviceFilter();

private:
	static BOOL	InstallDriver();
	static BOOL	UninstallDriver();

	static BOOL	AddLowerFilter(REFGUID classguid);
	static BOOL	RemoveLowerFilter(REFGUID classguid);
};
