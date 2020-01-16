#include "StdAfx.h"
#include "PsPclJobChangeHP.h"

CPsPclJobChangeHP::CPsPclJobChangeHP(void)
{
}

CPsPclJobChangeHP::~CPsPclJobChangeHP(void)
{
}

BOOL CPsPclJobChangeHP::ChangeJob(CString szSplPath, DWORD dwColor, DWORD dwDuplex, SplType nSplType)
{
	theLog.Write("CPsPclJobChangeHP::ChangeJob,1,beign,nSplType=%d,szSplPath=%s",nSplType,szSplPath);
	if (szSplPath.IsEmpty() || !PathFileExists(szSplPath))
	{
		theLog.Write("!!CPsPclJobChangeHP::ChangeJob,1,szSplPath=%s",szSplPath);
		return FALSE;
	}

	if (ST_PCL_6 == nSplType)
	{
		if (!ChangeJob_PCL(szSplPath,dwColor,dwDuplex))
		{
			theLog.Write("!!CPsPclJobChangeHP::ChangeJob,ChangeJob_PCL fail");
			return FALSE;
		}
	}
	else if (ST_PS == nSplType)
	{
		if (!ChangeJob_PS(szSplPath,dwColor,dwDuplex))
		{
			theLog.Write("!!CPsPclJobChangeHP::ChangeJob,ChangeJob_PS fail");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPsPclJobChangeHP::ChangeJob_PS(CString szSplPath, DWORD dwColor, DWORD dwDuplex)
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
			theLog.Write("CPsPclJobChangeHP::ChangeJob_PS,1,oFile FileLen=%d", oFile.GetLength());
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
					theLog.Write("CPsPclJobChangeHP::ChangeJob_PS,2");
					bNeedChange = TRUE;
					break;
				}
			}

			dwNeedChangeEndPos = oFile.GetPosition();
			oFile.Close();
		}
		else
		{
			theLog.Write("CPsPclJobChangeHP::ChangeJob_PS,3,oFile.Open fail,error=%d", GetLastError());
			break;
		}

		if (!bNeedChange || dwNeedChangeEndPos<=0)
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PS,4,bNeedChange=%d,dwNeedChangeEndPos=%d"
				, bNeedChange, dwNeedChangeEndPos);
			break;
		}

		pFile = ::fopen(szSplPath.GetString(), "rb");
		if (!pFile)
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PS,5,pFile=%p", pFile);
			break;
		}

		DWORD dwTmpBufLen = dwNeedChangeEndPos + 1;
		pTmpBuf = new BYTE[dwTmpBufLen];
		memset(pTmpBuf, 0x0, dwTmpBufLen);
		if (!ReadToBuf(pFile, pTmpBuf, dwNeedChangeEndPos))
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PS,6,ReadToBuf fail");
			break;
		}

		char *pHasRead = (char*)pTmpBuf;
		char* cEndSetUp = "%%EndSetup";
		char* pIndex = strstr(pHasRead, cEndSetUp);
		DWORD dwNeedChangeEndPos2 = pIndex - pHasRead + strlen(cEndSetUp);

		theLog.Write("CPsPclJobChangeHP::ChangJob_PS,7,dwNeedChangeEndPos=%d,dwNeedChangeEndPos2=%d"
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
			theLog.Write("##CPsPclJobChangeHP::ChangJob_PS,8,Not Need Change Job");
			break;
		}

		//创建新文件
		CString szNewSplPath;
		int nDotPos = szSplPath.ReverseFind('.');
		szNewSplPath.Format("%s_NewSpl.spl", szSplPath.Left(nDotPos));
		CFile oNewFile;
		if (!oNewFile.Open(szNewSplPath, CFile::modeCreate | CFile::modeWrite))
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PS,9,oNewFile.Open fail,szNewSplPath=%s,error=%u"
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
			theLog.Write("!!CPsPclJobChangeHP::ChangJobToHB_PS,10,MoveFileEx fail,error=%u", GetLastError());
		}

		theLog.Write("CPsPclJobChangeHP::ChangJobToHB_PS,11,succ");

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

BOOL CPsPclJobChangeHP::ChangeJob_PCL(CString szSplPath, DWORD dwColor, DWORD dwDuplex)
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
			theLog.Write("CPsPclJobChangeHP::ChangeJob_PCL,1,oFile FileLen=%d", oFile.GetLength());
			DWORD dwReadLineCount = 0;	//最大读取行数不要超过10000
			CString szOneLine;
			while (oFile.ReadString(szOneLine) && dwReadLineCount<10000)
			{
				dwReadLineCount++;

				if (szOneLine.CompareNoCase("@PJL ENTER LANGUAGE=PCLXL") != 0)
				{
					szNeedChangeStr += szOneLine + "\n";
				}
				else
				{	
					szNeedChangeStr += szOneLine;
					theLog.Write("CPsPclJobChangeHP::ChangeJob_PCL,2");
					bNeedChange = TRUE;
					break;
				}
			}

			dwNeedChangeEndPos = oFile.GetPosition();
			oFile.Close();
		}
		else
		{
			theLog.Write("CPsPclJobChangeHP::ChangeJob_PCL,3,oFile.Open fail,error=%d", GetLastError());
			break;
		}

		if (!bNeedChange || dwNeedChangeEndPos<=0)
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PCL,4,bNeedChange=%d,dwNeedChangeEndPos=%d"
				, bNeedChange, dwNeedChangeEndPos);
			break;
		}

		pFile = ::fopen(szSplPath.GetString(), "rb");
		if (!pFile)
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PCL,5,pFile=%p", pFile);
			break;
		}

		DWORD dwTmpBufLen = dwNeedChangeEndPos + 1;
		pTmpBuf = new BYTE[dwTmpBufLen];
		memset(pTmpBuf, 0x0, dwTmpBufLen);
		if (!ReadToBuf(pFile, pTmpBuf, dwNeedChangeEndPos))
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PCL,6,ReadToBuf fail");
			break;
		}

		char *pHasRead = (char*)pTmpBuf;
		char* cEndSetUp = "@PJL ENTER LANGUAGE=PCLXL";
		char* pIndex = strstr(pHasRead, cEndSetUp);
		DWORD dwNeedChangeEndPos2 = pIndex - pHasRead + strlen(cEndSetUp);

		theLog.Write("CPsPclJobChangeHP::ChangeJob_PCL,7,dwNeedChangeEndPos=%d,dwNeedChangeEndPos2=%d"
			, dwNeedChangeEndPos, dwNeedChangeEndPos2);

		BOOL bChangeToHB = FALSE;
		if (DMCOLOR_MONOCHROME == dwColor)
		{
			bChangeToHB = ChangeJobStrToHB_PCL(szNeedChangeStr);
		}

		BOOL bChangeToDuplexV = FALSE;
		BOOL bChangeToDuplexH = FALSE;
		if (DMDUP_VERTICAL == dwDuplex)
		{
			bChangeToDuplexV = ChangeJobStrToDuplexV_PCL(szNeedChangeStr);
		}
		else if (DMDUP_HORIZONTAL == dwDuplex)
		{
			bChangeToDuplexH = ChangeJobStrToDuplexH_PCL(szNeedChangeStr);
		}

		//如果文件内容没有任何修改，则不要创建新文件
		if (!bChangeToHB && !bChangeToDuplexV && !bChangeToDuplexH)
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PCL,8,Not Need Change Job");
			break;
		}

		//创建新文件
		CString szNewSplPath;
		int nDotPos = szSplPath.ReverseFind('.');
		szNewSplPath.Format("%s_NewSpl.spl", szSplPath.Left(nDotPos));
		CFile oNewFile;
		if (!oNewFile.Open(szNewSplPath, CFile::modeCreate | CFile::modeWrite))
		{
			theLog.Write("##CPsPclJobChangeHP::ChangeJob_PCL,9,oNewFile.Open fail,szNewSplPath=%s,error=%u"
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
			theLog.Write("!!CPsPclJobChangeHP::ChangeJob_PCL,10,MoveFileEx fail,error=%u", GetLastError());
		}

		theLog.Write("CPsPclJobChangeHP::ChangeJob_PCL,11,succ");

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

BOOL CPsPclJobChangeHP::ChangeJobStrToHB_PS(CString& szSource)
{
	if (szSource.Find("BLACKONLY") >= 0 || szSource.Find("COMPOSITE") >= 0)
	{
		return FALSE;
	}

	int nRet = szSource.Replace("@PJL SET GRAYSCALE=OFF", "@PJL SET GRAYSCALE=BLACKONLY");
	CString szTag = "@PJL SET PLANESINUSE=";
	int nIndex = szSource.Find(szTag);
	if (nIndex >= 0)
	{
		szTag = szSource.Mid(nIndex,szTag.GetLength() + 1);
	}
	theLog.Write("CPsPclJobChangeHP::ChangeJobStrToHB_PS,szTag=%s",szTag);
	szSource.Replace(szTag, "@PJL SET PLANESINUSE=1");//不管黑白还是彩色一律替换
	if (nRet != 0)
	{
		theLog.Write("ChangJobToHB_PS,2,找到彩色，替换成黑白");
	}
	else
	{
		theLog.Write("ChangJobToHB_PS,3,没找到彩色或本身无色彩参数，追加黑白");
		int nPos = szSource.Find("@PJL ENTER LANGUAGE=POSTSCRIPT");
		if (nPos >= 0)
		{
			szSource.Insert(nPos,"@PJL SET PLANESINUSE=1\n");
			szSource.Insert(nPos,"@PJL SET GRAYSCALE=BLACKONLY\n");//顺序不能变
		}
	}
	return TRUE;
}

BOOL CPsPclJobChangeHP::ChangeJobStrToColor_PS(CString& szSource)
{
	return FALSE;
}

BOOL CPsPclJobChangeHP::ChangeJobStrToDuplexV_PS(CString& szSource)
{
	int nRet = szSource.Replace("%%BeginFeature: *Duplex None","%%BeginFeature: *Duplex DuplexNoTumble");
	int nRet2 = szSource.Replace("<</Duplex false /Tumble false>> setpagedevice","<</Duplex true /Tumble false>> setpagedevice");
	if (nRet != 0 || nRet2 != 0)
	{
		theLog.Write("ChangeJobToDuplexV_PS,找到单面，替换成双面长边");
		return TRUE;
	}
	else
	{
		//theLog.Write("ChangeJobToDuplexV_PS,没找到单面");
		return FALSE;
	}
}

BOOL CPsPclJobChangeHP::ChangeJobStrToDuplexH_PS(CString& szSource)
{
	int nRet = szSource.Replace("%%BeginFeature: *Duplex None","%%BeginFeature: *Duplex DuplexTumble");
	int nRet2 = szSource.Replace("<</Duplex false /Tumble false>> setpagedevice","<</Duplex true /Tumble true>> setpagedevice");
	if (nRet != 0 || nRet2 !=0)
	{
		theLog.Write("ChangeJobToDuplexH_PS,2,找到单面，替换成双面短边");
		return TRUE;
	}
	else
	{
		//theLog.Write("ChangeJobToDuplexH_PS,3,没找到单面");
		return FALSE;
	}
}

BOOL CPsPclJobChangeHP::ChangeJobStrToHB_PCL(CString& szSource)
{
	if (szSource.Find("BLACKONLY") >= 0 || szSource.Find("COMPOSITE") >= 0)
	{
		return FALSE;
	}

	int nRet = szSource.Replace("@PJL SET GRAYSCALE=OFF", "@PJL SET GRAYSCALE=BLACKONLY");
	CString szTag = "@PJL SET PLANESINUSE=";
	int nIndex = szSource.Find(szTag);
	if (nIndex >= 0)
	{
		szTag = szSource.Mid(nIndex,szTag.GetLength() + 1);
	}
	theLog.Write("CPsPclJobChangeHP::ChangeJobStrToHB_PCL,szTag=%s",szTag);
	szSource.Replace(szTag, "@PJL SET PLANESINUSE=1");//不管黑白还是彩色一律替换
	if (nRet != 0)
	{
		theLog.Write("ChangeJobStrToHB_PCL,2,找到彩色，替换成黑白");
	}
	else
	{
		theLog.Write("ChangeJobStrToHB_PCL,3,没找到彩色或本身无色彩参数，追加黑白");
		int nPos = szSource.Find("@PJL ENTER LANGUAGE=PCLXL");
		if (nPos >= 0)
		{
			szSource.Insert(nPos,"@PJL SET PLANESINUSE=1\n");
			szSource.Insert(nPos,"@PJL SET GRAYSCALE=BLACKONLY\n");//顺序不能变
		}
	}
	return TRUE;
}

BOOL CPsPclJobChangeHP::ChangeJobStrToColor_PCL(CString& szSource)
{
	return FALSE;
}

BOOL CPsPclJobChangeHP::ChangeJobStrToDuplexV_PCL(CString& szSource)
{
	return FALSE;
}

BOOL CPsPclJobChangeHP::ChangeJobStrToDuplexH_PCL(CString& szSource)
{
	return FALSE;
}
