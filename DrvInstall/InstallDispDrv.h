#pragma once

class CInstallHookDisp
{
public:
	CInstallHookDisp(void);
	~CInstallHookDisp(void);

	BOOL InstallOpenGLDrv(CString sDrvName, BOOL bEnable = TRUE);
	BOOL InstallDisplayDrv(CString sDrvName,BOOL bEnable = TRUE);
	void UpdateDrvFile(CString sPath);
};

class CInstallMirrDisp  
{
public:
	CInstallMirrDisp();
	virtual ~CInstallMirrDisp();

public:
	void WriteAskReg(char * InstInfo);
	bool bAskInstAsSoftAllRemove();
	void RemoveAskReg(char * InstInfo);
	void RemoveAskInstInfo(char *pTtile);
	void WriteAskInstInfo(char * pTtile);


	//io driver
	void InstallIoDrv();
	void UninstallIoDrv();
	void StopIoDrv();
	BOOL StartIoDrv();

	void UpdateAllDispFile();
	void DelAllDispFile();

	void StartMirror(BOOL bIsStartDriver);


	void StopDisp();


	bool CheckDispInstalled();
	bool InstallDisp();
	bool RealInstallDisp();
	void UninstallDisp();


	BOOL  CurSysIsWin2k();


	bool GetSysImortFuncAddr(bool bwritefile);
	void DelTempFile();

private:
	BOOL UnInstallDriver(LPCTSTR lpszDirverSysName);
	int RemoveHardware(LPCTSTR lpszHardwareID);
	UINT SelGetSystemWindowsDirectory(OUT LPSTR lpBuffer,IN UINT uSize);

	CString m_sSysName;
public:
	CString m_strLocalPath;
};
