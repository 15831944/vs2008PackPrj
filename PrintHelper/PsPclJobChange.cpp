#include "StdAfx.h"
#include "PsPclJobChange.h"

CPsPclJobChange::CPsPclJobChange(void)
{
}

CPsPclJobChange::~CPsPclJobChange(void)
{
}

BOOL CPsPclJobChange::ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType)
{
	return TRUE;
}

DWORD CPsPclJobChange::ReadToBuf(FILE* pFile, BYTE* pBuf, DWORD dwLen)
{
	if (!pFile || !pBuf || (dwLen<=0))
	{
		theLog.Write("!!CPsPclJobChange::ReadToBuf,pFile=0x%x,pBuf=0x%x,dwLen=%u", pFile, pBuf, dwLen);
		return 0;
	}

	DWORD dwHasRdLen = 0;
	DWORD dwNeedRdLen = 0;
	DWORD dwReadLen = 0;
	DWORD dwValidBufLen = dwLen;
	//theLog.Write("CPsPclJobChange::ReadToBuf,1,dwValidBufLen=%d",dwValidBufLen);
	while(dwHasRdLen < dwValidBufLen)
	{
		dwNeedRdLen = dwValidBufLen - dwHasRdLen;
		dwReadLen = fread(pBuf+dwHasRdLen, 1, dwNeedRdLen, pFile);
		dwHasRdLen += dwReadLen;
		if(0 >= dwReadLen)
		{
			break;
		}
		else if(dwReadLen < dwNeedRdLen)
		{
			//文件没有结尾，则要求继续读取文件。
			if (!feof(pFile))
			{
				theLog.Write("##CPsPclJobChange::ReadToBuf,2.1,dwReadLen=%d,dwNeedRdLen=%d", dwReadLen, dwNeedRdLen);
			}
			else
			{
				break;
			}
		}
		else if(dwReadLen == dwNeedRdLen)
		{
			break;
		}
	}//while
	return dwHasRdLen;
}
