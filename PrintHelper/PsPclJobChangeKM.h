/*����Ŀǰ˫���ӡֻ������˫�泤��*/
#pragma once
#include "pspcljobchange.h"

class CPsPclJobChangeKM : public CPsPclJobChange
{
public:
	CPsPclJobChangeKM(void);
	~CPsPclJobChangeKM(void);

	virtual BOOL ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType);

	BOOL ChangeJob_PS(CString szSplPath, DWORD dwColor, DWORD dwDuplex);

	//���нӿڵķ���ֵ����ʾ�����Ƿ����޸�
	BOOL ChangeJobStrToHB_PS(CString& szSource);
	BOOL ChangeJobStrToColor_PS(CString& szSource);
	BOOL ChangeJobStrToDuplexV_PS(CString& szSource);
	BOOL ChangeJobStrToDuplexH_PS(CString& szSource);
};
