/*科美目前双面打印只看到过双面长边*/
#pragma once
#include "pspcljobchange.h"

class CPsPclJobChangeKM : public CPsPclJobChange
{
public:
	CPsPclJobChangeKM(void);
	~CPsPclJobChangeKM(void);

	virtual BOOL ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType);

	BOOL ChangeJob_PS(CString szSplPath, DWORD dwColor, DWORD dwDuplex);

	//下列接口的返回值，表示内容是否有修改
	BOOL ChangeJobStrToHB_PS(CString& szSource);
	BOOL ChangeJobStrToColor_PS(CString& szSource);
	BOOL ChangeJobStrToDuplexV_PS(CString& szSource);
	BOOL ChangeJobStrToDuplexH_PS(CString& szSource);
};
