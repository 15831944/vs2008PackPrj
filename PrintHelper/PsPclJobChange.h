#pragma once
#include "..\Public\SplHelperDef.h"


class CPsPclJobChange
{
public:
	CPsPclJobChange(void);
	~CPsPclJobChange(void);
	virtual BOOL ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType);
protected:
	DWORD ReadToBuf(FILE* pFile, BYTE* pBuf, DWORD dwLen);
};
