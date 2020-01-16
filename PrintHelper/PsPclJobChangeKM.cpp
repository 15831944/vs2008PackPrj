#include "StdAfx.h"
#include "PsPclJobChangeKM.h"

CPsPclJobChangeKM::CPsPclJobChangeKM(void)
{
}

CPsPclJobChangeKM::~CPsPclJobChangeKM(void)
{
}

BOOL CPsPclJobChangeKM::ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType)
{
	theLog.Write("CPsPclJobChangeKM::ChangeJob,1,beign,nSplType=%d,szSplPath=%s",nSplType,szSplPath);
	if (szSplPath.IsEmpty() || !PathFileExists(szSplPath))
	{
		theLog.Write("!!CPsPclJobChangeKM::ChangeJob,1,szSplPath=%s",szSplPath);
		return FALSE;
	}

	if (ST_PCL_6 == nSplType)
	{
	}
	else if (ST_PS == nSplType)
	{
		if (!ChangeJob_PS(szSplPath,dwColor,dwDuplex))
		{
			theLog.Write("!!CPsPclJobChangeKM::ChangeJob,2");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPsPclJobChangeKM::ChangeJob_PS(CString szSplPath, DWORD dwColor, DWORD dwDuplex)
{
	BOOL bSucc = FALSE;
	FILE *pFile = NULL;
	BYTE *pBuf = NULL;
	BYTE *pTmpBuf = NULL;

	do 
	{
		BOOL bNeedChange = FALSE;
		DWORD dwNeedChangeEndPos = 0;
		CString szNeedChangeStr = "";
		CStdioFile oFile;
		if (oFile.Open(szSplPath,CFile::modeRead))
		{
			theLog.Write("CPsPclJobChangeKM::ChangeJob_PS,1,oFile FileLen=%d", oFile.GetLength());
			DWORD dwReadLineCount = 0;	//最大读取行数不要超过10000
			CString szOneLine;
			while (oFile.ReadString(szOneLine) && dwReadLineCount<10000)
			{
				dwReadLineCount++;

				if (szOneLine.CompareNoCase("%%EndSetup") != 0)
				{
					szNeedChangeStr += szOneLine + "\n";
				}
				else
				{	
					szNeedChangeStr += szOneLine;
					theLog.Write("CPsPclJobChangeKM::ChangeJob_PS,2");
					bNeedChange = TRUE;
					break;
				}
			}

			dwNeedChangeEndPos = oFile.GetPosition();
			oFile.Close();
		}
		else
		{
			theLog.Write("CPsPclJobChangeKM::ChangeJob_PS,3,oFile.Open fail,error=%d", GetLastError());
			break;
		}
		
		if (!bNeedChange || dwNeedChangeEndPos<=0)
		{
			theLog.Write("##CPsPclJobChangeKM::ChangeJob_PS,4,bNeedChange=%d,dwNeedChangeEndPos=%d"
				, bNeedChange, dwNeedChangeEndPos);
			break;
		}

		pFile = ::fopen(szSplPath.GetString(), "rb");
		if (!pFile)
		{
			theLog.Write("##CPsPclJobChangeKM::ChangeJob_PS,5,pFile=%p", pFile);
			break;
		}

		DWORD dwTmpBufLen = dwNeedChangeEndPos + 1;
		pTmpBuf = new BYTE[dwTmpBufLen];
		memset(pTmpBuf, 0x0, dwTmpBufLen);
		if (!ReadToBuf(pFile, pTmpBuf, dwNeedChangeEndPos))
		{
			theLog.Write("##CPsPclJobChangeKM::ChangeJob_PS,6,ReadToBuf fail");
			break;
		}

		char *pHasRead = (char*)pTmpBuf;
		char* cEndSetUp = "%%EndSetup";
		char* pIndex = strstr(pHasRead, cEndSetUp);
		DWORD dwNeedChangeEndPos2 = pIndex - pHasRead + strlen(cEndSetUp);

		theLog.Write("CPsPclJobChangeKM::ChangJob_PS,7,dwNeedChangeEndPos=%d,dwNeedChangeEndPos2=%d"
			, dwNeedChangeEndPos, dwNeedChangeEndPos2);

		BOOL bChangeToHB = FALSE;
		if (DMCOLOR_MONOCHROME == dwColor)
		{
			bChangeToHB = ChangeJobStrToHB_PS(szNeedChangeStr);
		}

		BOOL bChangeToDuplexV = FALSE;
		BOOL bChangeToDuplexH = FALSE;
		if (DMDUP_VERTICAL == dwDuplex)
		{
			bChangeToDuplexV = ChangeJobStrToDuplexV_PS(szNeedChangeStr);
		}
		else if (DMDUP_HORIZONTAL == dwDuplex)
		{
			bChangeToDuplexH = ChangeJobStrToDuplexH_PS(szNeedChangeStr);
		}

		//如果文件内容没有任何修改，则不要创建新文件
		if (!bChangeToHB && !bChangeToDuplexV && !bChangeToDuplexH)
		{
			theLog.Write("##CPsPclJobChangeKM::ChangJob_PS,8,Not Need Change Job");
			break;
		}

		//创建新文件
		CString szNewSplPath;
		int nDotPos = szSplPath.ReverseFind('.');
		szNewSplPath.Format("%s_NewSpl.spl", szSplPath.Left(nDotPos));
		CFile oNewFile;
		if (!oNewFile.Open(szNewSplPath, CFile::modeCreate | CFile::modeWrite))
		{
			theLog.Write("##CPsPclJobChangeKM::ChangeJob_PS,9,oNewFile.Open fail,szNewSplPath=%s,error=%u"
				, szNewSplPath, GetLastError());
			break;
		}

		//将修改后的前半部分写入到新文件
		oNewFile.Write(szNeedChangeStr.GetString(),szNeedChangeStr.GetLength());

		//将后半部分文件写入到新文件
		fseek(pFile, dwNeedChangeEndPos2, SEEK_SET);

		DWORD dwBufLen = 1024*1024*10;//后半部分，按照10M大小分段写入新文件
		pBuf = new BYTE[dwBufLen];
		while (TRUE)
		{
			DWORD dwRead = ReadToBuf(pFile,pBuf,dwBufLen);
			if (dwRead > 0)
			{
				oNewFile.Write(pBuf,dwRead);
			}
			else
			{
				break;
			}
		}

		oNewFile.Close();

		if (pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}

		//将新文件覆盖原来的文件
		if (!MoveFileEx(szNewSplPath,szSplPath,MOVEFILE_REPLACE_EXISTING))
		{
			theLog.Write("!!CPsPclJobChangeKM::ChangJobToHB_PS,10,MoveFileEx fail,error=%u", GetLastError());
		}

		theLog.Write("CPsPclJobChangeKM::ChangJobToHB_PS,11,succ");

		bSucc = TRUE;

	} while (FALSE);

	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}

	if (pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}

	if (pTmpBuf)
	{
		delete[] pTmpBuf;
		pTmpBuf = NULL;
	}

	return bSucc;
}

BOOL CPsPclJobChangeKM::ChangeJobStrToHB_PS(CString& szSource)
{
	int nRet = szSource.Replace("<</ProcessColorModel /DeviceCMYK>>setpagedevice", "<</ProcessColorModel /DeviceGray>>setpagedevice");
	if (nRet != 0)
	{
		theLog.Write("CPsPclJobChangeKM::ChangJobToHB_PS,2,找到彩色，替换成黑白");
		return TRUE;
	}
	else
	{
		//theLog.Write("CPsPclJobChangeKM::ChangJobToHB_PS,3,没找到彩色");
		return FALSE;
	}
}

BOOL CPsPclJobChangeKM::ChangeJobStrToColor_PS(CString& szSource)
{
	return FALSE;
}

BOOL CPsPclJobChangeKM::ChangeJobStrToDuplexV_PS(CString& szSource)
{
	int nRet = szSource.Replace("<</Duplex false>>setpagedevice","<</Duplex true>>setpagedevice");
	int nRet2 = szSource.Replace("%%BeginFeature: *Duplex None","%%BeginFeature: *Duplex DuplexNoTumble");
	if (nRet != 0 || nRet2 != 0)
	{
		theLog.Write("CPsPclJobChangeKM::ChangeJobToDuplexV_PS,2,找到单面，替换成双面长边");
		return TRUE;
	}
	else
	{
		//theLog.Write("CPsPclJobChangeKM::ChangeJobToDuplexV_PS,3,没找到单面");
		return FALSE;
	}
}

BOOL CPsPclJobChangeKM::ChangeJobStrToDuplexH_PS(CString& szSource)
{
	int nRet = szSource.Replace("<</Duplex false>>setpagedevice","<</Duplex true>>setpagedevice");
	int nRet2 = szSource.Replace("%%BeginFeature: *Duplex None","%%BeginFeature: *Duplex DuplexTumble");
	if (nRet != 0 || nRet2 !=0)
	{
		theLog.Write("CPsPclJobChangeKM::ChangeJobStrToDuplexH_PS,2,找到单面，替换成双面短边");
		return TRUE;
	}
	else
	{
		//theLog.Write("CPsPclJobChangeKM::ChangeJobStrToDuplexH_PS,3,没找到单面");
		return FALSE;
	}
}
