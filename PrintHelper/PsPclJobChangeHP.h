#pragma once
#include "pspcljobchange.h"

class CPsPclJobChangeHP : public CPsPclJobChange
{
public:
	CPsPclJobChangeHP(void);
	~CPsPclJobChangeHP(void);

	virtual BOOL ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType);

	virtual BOOL ChangeJob_PS(CString szSplPath, DWORD dwColor, DWORD dwDuplex);
	virtual BOOL ChangeJob_PCL(CString szSplPath, DWORD dwColor, DWORD dwDuplex);

	//下列接口的返回值，表示内容是否有修改
	BOOL ChangeJobStrToHB_PS(CString& szSource);
	BOOL ChangeJobStrToColor_PS(CString& szSource);
	BOOL ChangeJobStrToDuplexV_PS(CString& szSource);
	BOOL ChangeJobStrToDuplexH_PS(CString& szSource);

	BOOL ChangeJobStrToHB_PCL(CString& szSource);
	BOOL ChangeJobStrToColor_PCL(CString& szSource);
	BOOL ChangeJobStrToDuplexV_PCL(CString& szSource);
	BOOL ChangeJobStrToDuplexH_PCL(CString& szSource);
};
