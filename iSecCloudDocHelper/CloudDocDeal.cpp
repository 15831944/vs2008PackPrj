#include "StdAfx.h"
#include "CloudDocDeal.h"

CCloudDocDeal::CCloudDocDeal(void)
{
	m_pJob = NULL;
	m_szOrgJobPath.Empty();
	m_szJobPath.Empty();
	m_szDealSaveJobPath.Empty();
	m_szNewJobName.Empty();
	m_szNewJobPath.Empty();
	m_bNeedDelOrgFile = TRUE;
}

CCloudDocDeal::CCloudDocDeal(PCloudJobInfo job)
{
	SetJobInfo(job);
}

CCloudDocDeal::~CCloudDocDeal(void)
{
	Release();
}

void CCloudDocDeal::SetJobInfo(PCloudJobInfo job)
{
	m_pJob = job;
	if (m_pJob)
	{
		theLog.Write("CCloudDocDeal::SetJobInfo,cDocName=%s, cJobDir=%s, cOrgJobName=%s"
			, m_pJob->cDocName, m_pJob->cJobDir, m_pJob->cOrgJobName);
		InitFilePath();
	}
	else
	{
		theLog.Write("!!CCloudDocDeal::SetJobInfo, m_pJob=%p", m_pJob);
	}
}

PCloudJobInfo CCloudDocDeal::GetJobInfo()
{
	return m_pJob;
}

void CCloudDocDeal::Release()
{
	if (m_pJob)
	{
		delete m_pJob;
		m_pJob = NULL;
	}
}

BOOL CCloudDocDeal::IsGzFile()
{
	BOOL bRet = FALSE;
	if (m_pJob)
	{
		CString szOrgJobName(m_pJob->cOrgJobName);
		if (szOrgJobName.Right(strlen(Cloud_Doc_Format_Suffix_GZ)).CompareNoCase(Cloud_Doc_Format_Suffix_GZ) == 0)
		{
			bRet = TRUE;
		}
	}
	return bRet;
}

BOOL CCloudDocDeal::IsImageFile()
{
	if ((m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_JPG)).CompareNoCase(Cloud_Doc_Format_Suffix_JPG) == 0)
		|| (m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_JPEG)).CompareNoCase(Cloud_Doc_Format_Suffix_JPEG) == 0)
		|| (m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_TIF)).CompareNoCase(Cloud_Doc_Format_Suffix_TIF) == 0)
		|| (m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_TIFF)).CompareNoCase(Cloud_Doc_Format_Suffix_TIFF) == 0)
		|| (m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_PNG)).CompareNoCase(Cloud_Doc_Format_Suffix_PNG) == 0)
		|| (m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_BMP)).CompareNoCase(Cloud_Doc_Format_Suffix_BMP) == 0)
		|| (m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_GIF)).CompareNoCase(Cloud_Doc_Format_Suffix_GIF) == 0)
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCloudDocDeal::IsPdfFile()
{
	if ((m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_PDF)).CompareNoCase(Cloud_Doc_Format_Suffix_PDF) == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCloudDocDeal::IsWordFile()
{
	if ((m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_Word)).CompareNoCase(Cloud_Doc_Format_Suffix_Word) == 0)
		||(m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_WordX)).CompareNoCase(Cloud_Doc_Format_Suffix_WordX) == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCloudDocDeal::IsPptFile()
{
	if ((m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_PowerPoint)).CompareNoCase(Cloud_Doc_Format_Suffix_PowerPoint) == 0)
		||(m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_PowerPointX)).CompareNoCase(Cloud_Doc_Format_Suffix_PowerPointX) == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCloudDocDeal::IsExcelFile()
{
	if ((m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_Excel)).CompareNoCase(Cloud_Doc_Format_Suffix_Excel) == 0)
		||(m_szJobPath.Right(strlen(Cloud_Doc_Format_Suffix_ExcelX)).CompareNoCase(Cloud_Doc_Format_Suffix_ExcelX) == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCloudDocDeal::ConvertFile()
{
	BOOL bRet = FALSE;
	do 
	{
		if (!m_pJob)
		{
			theLog.Write("!!CCloudDocDeal::ConvertFile,1,m_pJob=%p", m_pJob);
			break;

		}
		if (!PathFileExists(m_szJobPath))
		{
			theLog.Write("!!CCloudDocDeal::ConvertFile,2,PathFileExists fail,m_szJobPath=%s", m_szJobPath);
			break;
		}

		DWORD dwCopies = m_pJob->nCopies;
		DWORD dwPaperSize = m_pJob->nPaperSize;
		DWORD dwOrientation = m_pJob->nOrientation;
		BOOL bColor = (m_pJob->nColor==DMCOLOR_COLOR) ? TRUE : FALSE;

		if (IsImageFile())
		{
			bRet = CPrintableDocumentDeal::PictureToPrintTifFile(m_szJobPath, m_szDealSaveJobPath, dwPaperSize, dwOrientation, bColor
				, CPrintGlobalConfig::GetInstance().m_nImageDealFormatDpi, m_bNeedDelOrgFile);
			if (!bRet)
			{
				theLog.Write("!!CCloudDocDeal::ConvertFile,3,PictureToPrintTifFile fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
				break;
			}
		}
		else if (IsWordFile())
		{
			if (CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatType == Cloud_Doc_Format_PDF)
			{
				bRet = CPrintableDocumentDeal::WordToPdf(m_szJobPath, m_szDealSaveJobPath, m_bNeedDelOrgFile);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,4,WordToPdf fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
			else
			{
				bRet = CPrintableDocumentDeal::WordToTiff(m_szJobPath, m_szDealSaveJobPath, bColor, 
					CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatDpi, m_bNeedDelOrgFile);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,4,WordToTiff fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
		}
		else if (IsExcelFile())
		{
			if (CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatType == Cloud_Doc_Format_PDF)
			{
				bRet = CPrintableDocumentDeal::ExcelToPdf(m_szJobPath, m_szDealSaveJobPath, m_bNeedDelOrgFile, dwPaperSize, dwOrientation);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,5,ExcelToPdf fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
			else
			{
				bRet = CPrintableDocumentDeal::ExcelToTiff(m_szJobPath, m_szDealSaveJobPath, bColor, 
					CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatDpi, m_bNeedDelOrgFile, dwPaperSize, dwOrientation);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,5,ExcelToTiff fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
		}
		else if (IsPptFile())
		{
			if (CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatType == Cloud_Doc_Format_PDF)
			{
				bRet = CPrintableDocumentDeal::PPTToPdf(m_szJobPath, m_szDealSaveJobPath, m_bNeedDelOrgFile);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,6,PPTToPdf fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
			else
			{
				bRet = CPrintableDocumentDeal::PPTToTiff(m_szJobPath, m_szDealSaveJobPath, bColor, 
					CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatDpi, m_bNeedDelOrgFile);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,6,PPTToTiff fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
		}
		else if (IsPdfFile())
		{
			if (CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatType == Cloud_Doc_Format_PDF)
			{
				CFile::Rename(m_szJobPath, m_szDealSaveJobPath);	//不转换pdf,将原来的pdf文件名重命名为转换后的文件名
				bRet = TRUE;
			}
			else
			{
				bRet = CPrintableDocumentDeal::PdfToTiff(m_szJobPath, m_szDealSaveJobPath, bColor, 
					CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatDpi, m_bNeedDelOrgFile);
				if (!bRet)
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,7,PPTToTiff fail,m_szJobPath=%s,m_szDealSaveJobPath=%s", m_szJobPath, m_szDealSaveJobPath);
					break;
				}
			}
		}	
		else
		{
			theLog.Write("!!CCloudDocDeal::ConvertFile,8,fail,m_szJobPath=%s,m_bNeedDelOrgFile=%d", m_szJobPath, m_bNeedDelOrgFile);
			if (m_bNeedDelOrgFile)
			{
				if (!DeleteFile(m_szJobPath))
				{
					theLog.Write("!!CCloudDocDeal::ConvertFile,9,DeleteFile fail,m_szJobPath=%s,err=%u", m_szJobPath, GetLastError());
				}
			}
			break;
		}

		DWORD dwDocPageCount = 0;
		if (IsImageFile())
		{
			CPrintableDocumentDeal::GetTiffPageCount(m_szDealSaveJobPath, dwDocPageCount);
			theLog.Write("CCloudDocDeal::ConvertFile,10.A,m_szDealSaveJobPath=%s,dwDocPageCount=%u,dwCopies=%u", m_szDealSaveJobPath, dwDocPageCount, dwCopies);
		}
		else
		{
			if (CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatType == Cloud_Doc_Format_PDF)
			{
				CPrintableDocumentDeal::GetPdfPageCount(m_szDealSaveJobPath, dwDocPageCount);
				theLog.Write("CCloudDocDeal::ConvertFile,10.B,m_szDealSaveJobPath=%s,dwDocPageCount=%u,dwCopies=%u", m_szDealSaveJobPath, dwDocPageCount, dwCopies);
			}
			else
			{
				CPrintableDocumentDeal::GetTiffPageCount(m_szDealSaveJobPath, dwDocPageCount);
				theLog.Write("CCloudDocDeal::ConvertFile,10.C,m_szDealSaveJobPath=%s,dwDocPageCount=%u,dwCopies=%u", m_szDealSaveJobPath, dwDocPageCount, dwCopies);
			}
		}
		m_pJob->nTotalPage = dwCopies * dwDocPageCount;

		if (CPrintGlobalConfig::GetInstance().m_bCloudDocDealAfterIsGz)
		{
			if (!CompressGzFile(m_szDealSaveJobPath, m_szNewJobPath, TRUE))
			{
				theLog.Write("!!CCloudDocDeal::ConvertFile,11,fail,m_szJobPath=%s, m_szNewJobPath=%s"
					, m_szJobPath, m_szNewJobPath);
				break;
			}
		}

		//更新压缩包名称
		strcpy(m_pJob->cNewJobName, m_szNewJobName);
		m_pJob->nNewJobSize = CCommonFun::GetFileSizeByte(m_szNewJobPath);
		m_pJob->nNewJobIsGz = CPrintGlobalConfig::GetInstance().m_bCloudDocDealAfterIsGz;

		theLog.Write("CCloudDocDeal::ConvertFile,12,succ,cOrgJobName=%s,nOrgJobSize=%d,cNewJobName=%s,nNewJobSize=%d,nNewJobIsGz=%d"
			, m_pJob->cOrgJobName, m_pJob->nOrgJobSize, m_pJob->cNewJobName, m_pJob->nNewJobSize, m_pJob->nNewJobIsGz);
		bRet = TRUE;

	} while (FALSE);

	return bRet;
}

BOOL CCloudDocDeal::InitFilePath()
{
	BOOL bRet = FALSE;
	if (m_pJob)
	{
		CString szJobDir(m_pJob->cJobDir);
		if (CPrintGlobalConfig::GetInstance().m_bIsUseFtpStogeCloudDoc && strlen(CPrintGlobalConfig::GetInstance().m_oFtpInfo.szFtpDiskDir)>0)
		{
			szJobDir = CPrintGlobalConfig::GetInstance().m_oFtpInfo.szFtpDiskDir;
		}
		szJobDir.Trim();
		szJobDir.Replace(Cloud_Doc_Path_Slash_Current_Reverse, Cloud_Doc_Path_Slash_Current);
		szJobDir.TrimRight(Cloud_Doc_Path_Slash_Current);
		CString szOrgJobName(m_pJob->cOrgJobName);
		szOrgJobName.Trim();
		szOrgJobName.Trim(Cloud_Doc_Path_Slash_Current_Reverse);
		szOrgJobName.Replace(Cloud_Doc_Path_Slash_Current_Reverse, Cloud_Doc_Path_Slash_Current);
		if (IsGzFile())
		{
			m_bNeedDelOrgFile = TRUE;	//转换完成后，要删除解压后的原文档
			m_szOrgJobPath.Format("%s%s%s", szJobDir, Cloud_Doc_Path_Slash_Current, szOrgJobName);
			m_szJobPath = m_szOrgJobPath.Left(m_szOrgJobPath.GetLength()-strlen(Cloud_Doc_Format_Suffix_GZ));
			if (!UnCompressGzFile(m_szOrgJobPath, m_szJobPath, FALSE))
			{
				theLog.Write("!!CCloudDocDeal::InitFilePath,1,UnCompressGzFile fail,m_szOrgJobPath=%s, m_szJobPath=%s", m_szOrgJobPath, m_szJobPath);
				m_szJobPath.Empty();
			}
		}
		else
		{
			m_bNeedDelOrgFile = FALSE;	//转换完成后，不要删除未压缩的原文档
			m_szJobPath.Format("%s%s%s", szJobDir, Cloud_Doc_Path_Slash_Current, szOrgJobName);
			m_szOrgJobPath = m_szJobPath;
		}

		szJobDir = GetDirByFilePath(m_szOrgJobPath);

		if (m_szJobPath.GetLength()>0)
		{
			CString szOfficeDealFormatType = Cloud_Doc_Format_Suffix_TIF;	//默认是tif
			if (IsImageFile())
			{
				if (CPrintGlobalConfig::GetInstance().m_nImageDealFormatType == Cloud_Doc_Format_TIF)
				{
					//szOfficeDealFormatType = Cloud_Doc_Format_Suffix_TIF;
				}
				else if (CPrintGlobalConfig::GetInstance().m_nImageDealFormatType == Cloud_Doc_Format_JPG)
				{
					szOfficeDealFormatType = Cloud_Doc_Format_Suffix_JPG;
				}
				else if (CPrintGlobalConfig::GetInstance().m_nImageDealFormatType == Cloud_Doc_Format_PNG)
				{
					szOfficeDealFormatType = Cloud_Doc_Format_Suffix_PNG;
				}
				else if (CPrintGlobalConfig::GetInstance().m_nImageDealFormatType == Cloud_Doc_Format_BMP)
				{
					szOfficeDealFormatType = Cloud_Doc_Format_Suffix_BMP;
				}
				else if (CPrintGlobalConfig::GetInstance().m_nImageDealFormatType == Cloud_Doc_Format_GIF)
				{
					szOfficeDealFormatType = Cloud_Doc_Format_Suffix_GIF;
				}
			}
			else
			{
				if (CPrintGlobalConfig::GetInstance().m_nOfficeDealFormatType == Cloud_Doc_Format_PDF)
				{
					szOfficeDealFormatType =  Cloud_Doc_Format_Suffix_PDF;
				}
			}
			m_szNewJobName.Format("%s%s%s%s", m_pJob->cJobDir, GetFileNameByPathName(m_szJobPath), szOfficeDealFormatType
				, CPrintGlobalConfig::GetInstance().m_bCloudDocDealAfterIsGz ? Cloud_Doc_Format_Suffix_GZ : "");
			m_szDealSaveJobPath = m_szJobPath + szOfficeDealFormatType;
			m_szNewJobPath = m_szDealSaveJobPath + (CPrintGlobalConfig::GetInstance().m_bCloudDocDealAfterIsGz ? Cloud_Doc_Format_Suffix_GZ : "");
			bRet = TRUE;
		}
		else
		{
			theLog.Write("!!CCloudDocDeal::InitFilePath,2,m_szJobPath=%s", m_szJobPath);
			m_szNewJobName.Empty();
			m_szNewJobPath.Empty();
			m_szDealSaveJobPath.Empty();
			bRet = FALSE;
		}

		theLog.Write("CCloudDocDeal::InitFilePath,3,m_szOrgJobPath=%s,m_szJobPath=%s,m_szDealSaveJobPath=%s,"
			"m_szNewJobName=%s,m_szNewJobPath=%s,m_bNeedDelOrgFile=%d"
			, m_szOrgJobPath, m_szJobPath, m_szDealSaveJobPath, m_szNewJobName, m_szNewJobPath, m_bNeedDelOrgFile);
	}
	else
	{
		theLog.Write("!!CCloudDocDeal::InitFilePath,4,m_pJob=%p", m_pJob);
	}

	return bRet;
}

BOOL CCloudDocDeal::UnCompressGzFile(IN CString szFileGzPath, IN CString szFileOrgPath, BOOL bDelGzFile)
{
	BOOL bRet = FALSE;
	do 
	{
		if (!PathFileExists(szFileGzPath))
		{
			theLog.Write("!!CCloudDocDeal::UnCompressGzFile,1,PathFileExists fail,szFileGzPath=[%s]", szFileGzPath);
			break;
		}

		DeleteFile(szFileOrgPath);

		CFile fileOrg;
		if (!fileOrg.Open(szFileOrgPath, CFile::modeReadWrite|CFile::modeCreate))
		{
			theLog.Write("!!CCloudDocDeal::UnCompressGzFile,2,fileOrg.Open fail,err=%u,szFileOrgPath=[%s]", GetLastError(), szFileOrgPath);
			break;
		}

		gzFile GzFile = gzopen(szFileGzPath, "rb");
		if(GzFile == NULL)
		{
			theLog.Write("!!CCloudDocDeal::UnCompressGzFile,3,gzopen fail,GzFile=%p szFileGzPath=[%s]", GzFile, szFileGzPath);
			fileOrg.Close();
			break;
		}

		int nRead = 0;
		int nBufSize = 1024 * 1024 * 10;
		BYTE *pBuf = new BYTE[nBufSize];
		BOOL bSucc = TRUE;
		while(!gzeof(GzFile))
		{
			memset(pBuf, 0x0, nBufSize);
			nRead = gzread(GzFile, pBuf, nBufSize);
			if (nRead>0)
			{
				fileOrg.Write(pBuf, nRead);
			}
			else if (nRead<0)
			{
				theLog.Write("!!CCloudDocDeal::CompressGzFile,4,gzread fail,nRead=%d", nRead);
				bSucc = FALSE;
				break;
			}
			else
			{
				break;
			}
		}
		fileOrg.Close();
		gzclose(GzFile);
		delete[] pBuf;

		bRet = bSucc;
	} while (FALSE);

	if (bDelGzFile)
	{
		if (!DeleteFile(szFileGzPath))
		{
			theLog.Write("!!CCloudDocDeal::UnCompressGzFile,5,DeleteFile fail,szFileGzPath=[%s]", szFileGzPath);
		}
	}

	return bRet;
}

BOOL CCloudDocDeal::CompressGzFile(IN CString szFileOrgPath, IN CString szFileGzPath, BOOL bDelOrgFile)
{
	BOOL bRet = FALSE;
	do 
	{
		if (!PathFileExists(szFileOrgPath))
		{
			theLog.Write("!!CCloudDocDeal::CompressGzFile,1,PathFileExists fail,szFileOrg=[%s]", szFileOrgPath);
			break;
		}

		DeleteFile(szFileGzPath);

		CFile fileOrg;
		if (!fileOrg.Open(szFileOrgPath, CFile::modeRead))
		{
			theLog.Write("!!CCloudDocDeal::CompressGzFile,2,szFileGzPath.Open fail,err=%u,szFileOrgPath=[%s]", GetLastError(), szFileOrgPath);
			break;
		}

		gzFile GzFile = gzopen(szFileGzPath, "wb");
		if(GzFile == NULL)
		{
			theLog.Write("!!CCloudDocDeal::CompressGzFile,3,gzopen fail,GzFile=%p szFileGzPath=[%s]", GzFile, szFileGzPath);
			fileOrg.Close();
			break;
		}

		int nRead = 0;
		int nWrite = 0;
		int nBufSize = 1024 * 1024 * 10;
		BYTE *pBuf = new BYTE[nBufSize];
		BOOL bSucc = TRUE;
		do 
		{
			memset(pBuf, 0x0, nBufSize);
			nRead = fileOrg.Read(pBuf, nBufSize);
			if (nRead>0)
			{
				nWrite = gzwrite(GzFile, pBuf, nRead);
				if(nWrite != nRead)
				{
					theLog.Write("!!CCloudDocDeal::CompressGzFile,4,gzwrite fail,nWrite=%d,nRead=%d", nWrite, nRead);
					bSucc = FALSE;
					break;
				}
			}
			else
			{
				break;
			}		
		} while (nRead>0);
		fileOrg.Close();
		gzclose(GzFile);
		delete[] pBuf;

		bRet = bSucc;
	} while (FALSE);

	if (bDelOrgFile)
	{
		if (!DeleteFile(szFileOrgPath))
		{
			theLog.Write("!!CCloudDocDeal::CompressGzFile,5,DeleteFile fail,szFileGzPath=[%s]", szFileGzPath);
		}
	}
	return bRet;
}

CString CCloudDocDeal::GetFileNameByPathName(CString szPathName)
{
	if( szPathName.IsEmpty()) 
		return _T("");

	CStringArray sAry;
	CCommonFun::StringSplit(szPathName,&sAry,Cloud_Doc_Path_Slash_Current);
	if( sAry.GetSize() > 0 )
		return sAry.GetAt(sAry.GetSize()-1);

	return _T("");
}

CString CCloudDocDeal::GetDirByFilePath(CString szFilePath)
{
	CString sTmp;
	CString szCloud_Doc_Path_Slash_Current(Cloud_Doc_Path_Slash_Current);
	int nPos = szFilePath.ReverseFind(szCloud_Doc_Path_Slash_Current.GetAt(0));
	sTmp = szFilePath.Left(nPos);
	return sTmp;
}
