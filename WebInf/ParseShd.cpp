#include "StdAfx.h"
#include "ParseShd.h"
#include <winspool.h>

CParseShd::CParseShd(void)
{
	m_szShdFilePath.Empty();

	m_dwSignature = 0;
	m_pShdHeader = NULL;

	m_bIsWindowServer2008 = IsWindowServer2008();

	m_JobInfo.pDevMode = NULL;
	m_JobInfo.pSecurityDescriptor = NULL;
	ClearJobInfo();
}

CParseShd::~CParseShd(void)
{
	Release();
}

BOOL CParseShd::InitParse(CString szFilePath)
{
	Release();

	if (IsShdFile(szFilePath))
	{
		m_szShdFilePath = szFilePath;
		return ParseShd();
	}
	return FALSE;
}

PJOB_INFO_2_Ex CParseShd::GetJobInfo()
{
	return &m_JobInfo;
}

void CParseShd::Release()
{
	m_dwSignature = 0;
	m_szShdFilePath.Empty();

	if (m_ShdFile.m_hFile != CFile::hFileNull)
	{
		m_ShdFile.Close();
	}
	if (m_pShdHeader)
	{
		delete[] m_pShdHeader;
		m_pShdHeader = NULL;
	}

	ClearJobInfo();
}

BOOL CParseShd::ParseShd()
{
	if (m_ShdFile.Open(m_szShdFilePath, CFile::modeRead))
	{
		if (!GetShdSignature())
		{
			ASSERT(FALSE);
			WriteFileLog("!!CParseShd::ParseShd,1,GetShdSignature fail");
			return FALSE;
		}

		if (!GetShdDetial())
		{
			ASSERT(FALSE);
			WriteFileLog("!!CParseShd::ParseShd,2,GetShdDetial fail");
			return FALSE;
		}

		if (!GetDevMode())
		{
			ASSERT(FALSE);
			WriteFileLog("!!CParseShd::ParseShd,3,GetDevMode fail");
			return FALSE;
		}

		return TRUE;	
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseShd::ParseShd,4,m_ShdFile.Open fail,m_szShdFilePath=[%s]", m_szShdFilePath);
		return FALSE;
	}
}

BOOL CParseShd::GetShdSignature()
{
	if (m_ShdFile.m_hFile != CFile::hFileNull)
	{
		m_ShdFile.SeekToBegin();
		DWORD dwHead[2] = {0};
		DWORD dwCount = m_ShdFile.Read(dwHead, sizeof(DWORD)*2);
		if (dwCount == sizeof(DWORD)*2)
		{
			m_dwSignature = dwHead[0];
			if (dwHead[1] == SHD_HEADSIZE_WIN2008
				|| dwHead[1] == SHD_HEADSIZE_WIN8_1)
			{
				m_bIsWindowServer2008 = TRUE;
			}
			else
			{
				m_bIsWindowServer2008 = FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CParseShd::GetShdDetial()
{
	if (!GetShdHead())
	{
		WriteFileLog("!!CParseShd::GetShdDetial,1,GetShdHead fail");
		return FALSE;
	}

	if (!GetOffInfo())
	{
		WriteFileLog("!!CParseShd::GetShdDetial,2,GetOffInfo fail");
		return FALSE;
	}
	return TRUE;
}


BOOL CParseShd::GetShdHead()
{
	BOOL bRet = FALSE;
	switch (m_dwSignature)
	{
	case SHD_SIGNATURE_WIN98:
		bRet = GetShdHead98();
		break;
	case SHD_SIGNATURE_WINNT:
		bRet = GetShdHeadNT();
		break;
	case SHD_SIGNATURE_WIN2000:
	case SHD_SIGNATURE_WIN2003:
	case SHD_SIGNATURE_WIN8_1:
		if (m_bIsWindowServer2008)
		{
			bRet = GetShdHead2008();
		}
		else
		{
			bRet = GetShdHead2000();
		}
		break;
	default:
		WriteFileLog("!!CParseShd::GetShdHead,UnKnow Signature = [%x]", m_dwSignature);
		break;
	}
	return bRet;
}

BOOL CParseShd::GetShdHeadData(DWORD dwSize)
{
	if (m_ShdFile.m_hFile != CFile::hFileNull)
	{
		if (dwSize <= 0)
		{
			ASSERT(dwSize>0);
			WriteFileLog("!!CParseShd::GetShdHeadData,1,dwSize = %d", dwSize);
			return FALSE;
		}

		if (m_pShdHeader)
		{
			delete[] m_pShdHeader;
			m_pShdHeader = NULL;
		}

		m_pShdHeader = new BYTE[dwSize];
		memset(m_pShdHeader, 0x0, dwSize);
		ASSERT(m_pShdHeader);
		m_ShdFile.SeekToBegin();
		DWORD dwCount = m_ShdFile.Read(m_pShdHeader, dwSize);
		if (dwCount != dwSize)
		{
			ASSERT(FALSE);
			WriteFileLog("!!CParseShd::GetShdHeadData,2,dwCount=%d, dwSize=%d", dwCount, dwCount);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseShd::GetShdHeadData,3,m_ShdFile.m_hFile=%X", m_ShdFile.m_hFile);
		return FALSE;
	}
}

BOOL CParseShd::GetShdHead98()
{
	if (GetShdHeadData(sizeof(SHADOW_FILE_HEADER_WIN98)))
	{
		PSHADOW_FILE_HEADER_WIN98 pHeader98 = (PSHADOW_FILE_HEADER_WIN98)m_pShdHeader;
		m_JobInfo.JobId			= pHeader98->dwJobID;
		m_JobInfo.PagesPrinted	= pHeader98->dwPageCount;
		m_JobInfo.Position		= 0;
		m_JobInfo.Priority		= pHeader98->dwPriority;
		m_JobInfo.pSecurityDescriptor = NULL;
		m_JobInfo.pDevMode		= NULL;
		m_JobInfo.Size			= 0;
		m_JobInfo.SPLSize		= pHeader98->dwSPLSize;
		m_JobInfo.StartTime		= pHeader98->dwStartTime;
		m_JobInfo.Status		= pHeader98->wStatus;
		m_JobInfo.Submitted		= pHeader98->stSubmitTime;
		m_JobInfo.Time			= pHeader98->dwUntilTime;
		m_JobInfo.TotalPages	= pHeader98->dwPageCount;
		m_JobInfo.UntilTime		= pHeader98->dwUntilTime;

		return TRUE;
	}
	else
	{
		WriteFileLog("!!CParseShd::GetShdHead98,1,GetShdHeadData fail");
		return FALSE;
	}

}

BOOL CParseShd::GetShdHeadNT()
{
	if (GetShdHeadData(sizeof(SHADOW_FILE_HEADER_WINNT)))
	{
		PSHADOW_FILE_HEADER_WINNT pHeaderNT = (PSHADOW_FILE_HEADER_WINNT)m_pShdHeader;
		m_JobInfo.JobId			= pHeaderNT->dwJobID;
		m_JobInfo.PagesPrinted	= pHeaderNT->dwPageCount;
		m_JobInfo.Position		= 0;
		m_JobInfo.Priority		= pHeaderNT->dwPriority;
		m_JobInfo.pSecurityDescriptor = NULL;
		m_JobInfo.pDevMode		= NULL;
		m_JobInfo.Size			= 0;
		m_JobInfo.SPLSize		= pHeaderNT->dwSPLSize;
		m_JobInfo.StartTime		= pHeaderNT->dwStartTime;
		m_JobInfo.Status		= pHeaderNT->wStatus;
		m_JobInfo.Submitted		= pHeaderNT->stSubmitTime;
		m_JobInfo.Time			= pHeaderNT->dwUntilTime;
		m_JobInfo.TotalPages	= pHeaderNT->dwPageCount;
		m_JobInfo.UntilTime		= pHeaderNT->dwUntilTime;

		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseShd::GetShdHeadNT,1,GetShdHeadData fail");
		return FALSE;
	}
}

BOOL CParseShd::GetShdHead2000()
{
	if (GetShdHeadData(sizeof(SHADOW_FILE_HEADER_WIN2000)))
	{
		PSHADOW_FILE_HEADER_WIN2000 pHeader2000 = (PSHADOW_FILE_HEADER_WIN2000)m_pShdHeader;
		m_JobInfo.JobId			= pHeader2000->dwJobID;
		m_JobInfo.PagesPrinted	= pHeader2000->dwPageCount;
		m_JobInfo.Position		= 0;
		m_JobInfo.Priority		= pHeader2000->dwPriority;
		m_JobInfo.pSecurityDescriptor = NULL;
		m_JobInfo.pDevMode		= NULL;
		m_JobInfo.Size			= 0;
		m_JobInfo.SPLSize		= pHeader2000->dwSPLSize;
		m_JobInfo.StartTime		= pHeader2000->dwStartTime;
		m_JobInfo.Status		= pHeader2000->wStatus;
		m_JobInfo.Submitted		= pHeader2000->stSubmitTime;
		m_JobInfo.Time			= pHeader2000->dwUntilTime;
		m_JobInfo.TotalPages	= pHeader2000->dwPageCount;
		m_JobInfo.UntilTime		= pHeader2000->dwUntilTime;
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseShd::GetShdHead2000,1,GetShdHeadData fail");
		return FALSE;
	}
}

BOOL CParseShd::GetShdHead2008()
{
	if (GetShdHeadData(sizeof(SHADOW_FILE_HEADER_WIN2008)))
	{
		PSHADOW_FILE_HEADER_WIN2008 pHeader2008 = (PSHADOW_FILE_HEADER_WIN2008)m_pShdHeader;
		m_JobInfo.JobId			= pHeader2008->dwJobID;
		m_JobInfo.PagesPrinted	= pHeader2008->dwPageCount;
		m_JobInfo.Position		= 0;
		m_JobInfo.Priority		= /*pHeader2008->dwPriority*/0;
		m_JobInfo.pSecurityDescriptor = NULL;
		m_JobInfo.pDevMode		= NULL;
		m_JobInfo.Size			= 0;
		m_JobInfo.SPLSize		= pHeader2008->dwSPLSize;
		m_JobInfo.StartTime		= /*pHeader2008->dwStartTime*/0;
		m_JobInfo.Status		= pHeader2008->wStatus;
		m_JobInfo.Submitted		= pHeader2008->stSubmitTime;
		m_JobInfo.Time			= /*pHeader2008->dwUntilTime*/0;
		m_JobInfo.TotalPages	= pHeader2008->dwPageCount;
		m_JobInfo.UntilTime		= /*pHeader2008->dwUntilTime*/0;
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseShd::GetShdHead2008,1,GetShdHeadData fail");
		return FALSE;
	}
}
BOOL CParseShd::GetOffInfo()
{
	BOOL bRet = FALSE;
	switch (m_dwSignature)
	{
	case SHD_SIGNATURE_WIN98:
		bRet = GetOffInfo98();
		break;
	case SHD_SIGNATURE_WINNT:
		bRet = GetOffInfoNT();
		break;
	case SHD_SIGNATURE_WIN2000:
	case SHD_SIGNATURE_WIN2003:
	case SHD_SIGNATURE_WIN8_1:
		if (m_bIsWindowServer2008)
		{
			bRet = GetOffInfo2008();
		}
		else
		{
			bRet = GetOffInfo2000();
		}
		break;
	default:
		WriteFileLog("!!CParseShd::GetOffInfo,UnKnow Signature = [%x]", m_dwSignature);
		break;
	}
	return bRet;
}

BOOL CParseShd::GetOffInfo98()
{
	if (!m_pShdHeader)
	{
		WriteFileLog("!!CParseShd::GetOffInfo98,1,m_pShdHeader=%p", m_pShdHeader);
		return FALSE;
	}

	PSHADOW_FILE_HEADER_WIN98 pHeader98 = (PSHADOW_FILE_HEADER_WIN98)m_pShdHeader;
	GetString(pHeader98->offDataFormat,	m_JobInfo.szDatatype);
	GetString(pHeader98->offDocumentName,	m_JobInfo.szDocument);
	GetString(pHeader98->offDriverName,	m_JobInfo.szDriverName);
	m_JobInfo.szMachineName = TEXT("");
	GetString(pHeader98->offNotifyName,	m_JobInfo.szNotifyName);
	m_JobInfo.szParameters	= TEXT("");
	GetString(pHeader98->offPrinterName,	m_JobInfo.szPrinterName);
	m_JobInfo.szPortName	= TEXT("");
	GetString(pHeader98->offPrintProcessor, m_JobInfo.szPrintProcessor);
	m_JobInfo.szStatus		= GetStatus(pHeader98->wStatus);
	GetString(pHeader98->offUserName, m_JobInfo.szUserName);
	return TRUE;
}

BOOL CParseShd::GetOffInfoNT()
{
	if (!m_pShdHeader)
	{
		WriteFileLog("!!CParseShd::GetOffInfoNT,1,m_pShdHeader=%p", m_pShdHeader);
		return FALSE;
	}

	PSHADOW_FILE_HEADER_WINNT pHeaderNT = (PSHADOW_FILE_HEADER_WINNT)m_pShdHeader;
	GetString(pHeaderNT->offDataFormat,	m_JobInfo.szDatatype);
	GetString(pHeaderNT->offDocumentName,	m_JobInfo.szDocument);
	GetString(pHeaderNT->offDriverName,	m_JobInfo.szDriverName);
	m_JobInfo.szMachineName = TEXT("");
	GetString(pHeaderNT->offNotifyName,	m_JobInfo.szNotifyName);
	m_JobInfo.szParameters	= TEXT("");
	GetString(pHeaderNT->offPrinterName,	m_JobInfo.szPrinterName);
	GetString(pHeaderNT->offPort, m_JobInfo.szPortName);
	GetString(pHeaderNT->offPrintProcessor, m_JobInfo.szPrintProcessor);
	m_JobInfo.szStatus		= GetStatus(pHeaderNT->wStatus);
	GetString(pHeaderNT->offUserName, m_JobInfo.szUserName);
	return TRUE;
}

BOOL CParseShd::GetOffInfo2000()
{
	if (!m_pShdHeader)
	{
		WriteFileLog("!!CParseShd::GetOffInfo2000,1,m_pShdHeader=%p", m_pShdHeader);
		return FALSE;
	}

	PSHADOW_FILE_HEADER_WIN2000 pHeader2000 = (PSHADOW_FILE_HEADER_WIN2000)m_pShdHeader;
	GetString(pHeader2000->offDataFormat,	m_JobInfo.szDatatype);
	GetString(pHeader2000->offDocumentName,	m_JobInfo.szDocument);
	GetString(pHeader2000->offDriverName,	m_JobInfo.szDriverName);
	GetString(pHeader2000->offComputername,	m_JobInfo.szMachineName);
	GetString(pHeader2000->offNotifyName,	m_JobInfo.szNotifyName);
	m_JobInfo.szParameters	= TEXT("");
	GetString(pHeader2000->offPrinterName,	m_JobInfo.szPrinterName);
	GetString(pHeader2000->offPort, m_JobInfo.szPortName);
	GetString(pHeader2000->offPrintProcessor, m_JobInfo.szPrintProcessor);
	m_JobInfo.szStatus		= GetStatus(pHeader2000->wStatus);
	GetString(pHeader2000->offUserName, m_JobInfo.szUserName);
	return TRUE;
}

BOOL CParseShd::GetOffInfo2008()
{
	if (!m_pShdHeader)
	{
		WriteFileLog("!!CParseShd::GetOffInfo2008,1,m_pShdHeader=%p", m_pShdHeader);
		return FALSE;
	}

	PSHADOW_FILE_HEADER_WIN2008 pHeader2008 = (PSHADOW_FILE_HEADER_WIN2008)m_pShdHeader;
	GetString(pHeader2008->offDataFormat,	m_JobInfo.szDatatype);
	GetString(pHeader2008->offDocumentName,	m_JobInfo.szDocument);
	GetString(pHeader2008->offDriverName,	m_JobInfo.szDriverName);
	GetString(pHeader2008->offComputername,	m_JobInfo.szMachineName);
	GetString(pHeader2008->offNotifyName,	m_JobInfo.szNotifyName);
	m_JobInfo.szParameters	= TEXT("");
	GetString(pHeader2008->offPrinterName,	m_JobInfo.szPrinterName);
	GetString(pHeader2008->offPort, m_JobInfo.szPortName);
	GetString(pHeader2008->offPrintProcessor, m_JobInfo.szPrintProcessor);
	m_JobInfo.szStatus		= GetStatus(pHeader2008->wStatus);
	GetString(pHeader2008->offUserName, m_JobInfo.szUserName);
	return TRUE;
}

BOOL CParseShd::GetDevMode()
{
	if (m_ShdFile.m_hFile != CFile::hFileNull)
	{
		DWORD dwOffDevMode = 0;
		switch (m_dwSignature)
		{
		case SHD_SIGNATURE_WIN98:
			{
				SHADOW_FILE_HEADER_WIN98 *pShdHeader = (SHADOW_FILE_HEADER_WIN98*)m_pShdHeader;
				dwOffDevMode = pShdHeader->offDevMode;
			}
			break;
		case SHD_SIGNATURE_WINNT:
			{
				SHADOW_FILE_HEADER_WINNT *pShdHeader = (SHADOW_FILE_HEADER_WINNT*)m_pShdHeader;
				dwOffDevMode = pShdHeader->offDevMode;
			}
			break;
		case SHD_SIGNATURE_WIN2000:
		case SHD_SIGNATURE_WIN2003:
		case SHD_SIGNATURE_WIN8_1:
			{
				if (m_bIsWindowServer2008)
				{
					SHADOW_FILE_HEADER_WIN2008 *pShdHeader = (SHADOW_FILE_HEADER_WIN2008*)m_pShdHeader;
					dwOffDevMode = pShdHeader->offDevMode;
				}
				else
				{
					SHADOW_FILE_HEADER_WIN2000 *pShdHeader = (SHADOW_FILE_HEADER_WIN2000*)m_pShdHeader;
					dwOffDevMode = pShdHeader->offDevMode;
				}
			}
			break;
		default:
			break;
		}
		if (dwOffDevMode == 0)
		{
			ASSERT(FALSE);
			WriteFileLog("!!CParseShd::GetDevMode,1,dwOffDevMode == 0");
			return FALSE;
		}

		m_ShdFile.Seek(dwOffDevMode, CFile::begin);
		int nSize = sizeof(DEVMODEW);
		BYTE *pData = new BYTE[nSize];
		memset(pData, 0x0, nSize);
		DWORD dwCount = m_ShdFile.Read(pData, nSize);
		if (dwCount != nSize)
		{
			ASSERT(FALSE);
			WriteFileLog("!!CParseShd::GetDevMode,2,dwCount=%d, nSize=%d", dwCount, nSize);
			delete []pData;
			return FALSE;
		}
		m_JobInfo.pDevMode = (DEVMODEW*)pData;
// 		if (!IsValidDevmodeW(m_JobInfo.pDevMode, nSize))
// 		{
// 			ASSERT(FALSE);
// 			return FALSE;
// 		}
		return TRUE;
	}
	else
	{
		WriteFileLog("!!CParseShd::GetDevMode,3,m_ShdFile.m_hFile=%x", m_ShdFile.m_hFile);
		return FALSE;
	}
}

BOOL CParseShd::GetString(DWORD dwOffString, CString& szValue)
{
	//ASSERT(dwOffString > 0);
 	szValue.Empty();
	if (m_ShdFile.m_hFile != CFile::hFileNull)
	{
		if (dwOffString <= 0)
		{
			WriteFileLog("!!CParseShd::GetString,1,dwOffString=%d", dwOffString);
			return FALSE;
		}
		m_ShdFile.Seek(dwOffString, CFile::begin);
		WCHAR *pData = new WCHAR[MAX_PATH];
		memset(pData, 0x0, sizeof(WCHAR)*MAX_PATH);
		memset(pData, 0, MAX_PATH*sizeof(WCHAR));
		DWORD dwCount = m_ShdFile.Read(pData, MAX_PATH*sizeof(WCHAR));
		if (dwCount > 0 && dwCount <= MAX_PATH*2)
		{
			szValue = pData;
			delete[] pData;
			return TRUE;
		}
		WriteFileLog("!!CParseShd::GetString,2,dwCount=%d", dwCount);
		delete[] pData;
		return FALSE;
	}
	else
	{
		WriteFileLog("!!CParseShd::GetString,3,m_ShdFile.m_hFile=%x", m_ShdFile.m_hFile);
		return FALSE;
	}
}

BOOL CParseShd::IsShdFile(CString szFilePath)
{
	if (szFilePath.GetLength()>4)
	{
		CString szExt = szFilePath.Right(4);
		if (szExt.CompareNoCase(SHD_EXT) == 0)
		{
			return TRUE;
		}
	}
	WriteFileLog("!!CParseShd::GetString,1,IsShdFile, szFilePath=[%s]", szFilePath);
	return FALSE;
}

/*
#define JOB_STATUS_PAUSED               0x00000001
#define JOB_STATUS_ERROR                0x00000002
#define JOB_STATUS_DELETING             0x00000004
#define JOB_STATUS_SPOOLING             0x00000008
#define JOB_STATUS_PRINTING             0x00000010
#define JOB_STATUS_OFFLINE              0x00000020
#define JOB_STATUS_PAPEROUT             0x00000040
#define JOB_STATUS_PRINTED              0x00000080
#define JOB_STATUS_DELETED              0x00000100
#define JOB_STATUS_BLOCKED_DEVQ         0x00000200
#define JOB_STATUS_USER_INTERVENTION    0x00000400
#define JOB_STATUS_RESTART              0x00000800

#if (NTDDI_VERSION >= NTDDI_WINXP)
#define JOB_STATUS_COMPLETE         0x00001000
#endif // (NTDDI_VERSION >= NTDDI_WINXP)

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
#define JOB_STATUS_RETAINED             0x00002000
#define JOB_STATUS_RENDERING_LOCALLY    0x00004000
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)
*/
CString CParseShd::GetStatus(DWORD dwStatus)
{
	CString szStatus;
	switch (dwStatus)
	{
	case JOB_STATUS_PAUSED:
		szStatus = "JOB_STATUS_PAUSED";
		break;
	case JOB_STATUS_ERROR:
		szStatus = "JOB_STATUS_ERROR";
		break;
	case JOB_STATUS_DELETING:
		szStatus = "JOB_STATUS_DELETING";
		break;
	case JOB_STATUS_SPOOLING:
		szStatus = "JOB_STATUS_SPOOLING";
		break;
	case JOB_STATUS_PRINTING:
		szStatus = "JOB_STATUS_PRINTING";
		break;
	case JOB_STATUS_OFFLINE:
		szStatus = "JOB_STATUS_OFFLINE";
		break;
	case JOB_STATUS_PAPEROUT:
		szStatus = "JOB_STATUS_PAPEROUT";
		break;
	case JOB_STATUS_PRINTED:
		szStatus = "JOB_STATUS_PRINTED";
		break;
	case JOB_STATUS_DELETED:
		szStatus = "JOB_STATUS_DELETED";
		break;
	case JOB_STATUS_BLOCKED_DEVQ:
		szStatus = "JOB_STATUS_BLOCKED_DEVQ";
		break;
	case JOB_STATUS_USER_INTERVENTION:
		szStatus = "JOB_STATUS_USER_INTERVENTION";
		break;
	case JOB_STATUS_RESTART:
		szStatus = "JOB_STATUS_RESTART";
		break;

#if (NTDDI_VERSION >= NTDDI_WINXP)
	case JOB_STATUS_COMPLETE:
		szStatus = "JOB_STATUS_COMPLETE";
		break;
#endif // (NTDDI_VERSION >= NTDDI_WINXP)

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
	case JOB_STATUS_RETAINED:
		szStatus = "JOB_STATUS_RETAINED";
		break;
	case JOB_STATUS_RENDERING_LOCALLY:
		szStatus = "JOB_STATUS_RENDERING_LOCALLY";
		break;
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

	default:
		szStatus = "UnKnown";
		break;
	}
	return szStatus;
}

void CParseShd::ClearJobInfo()
{
	m_JobInfo.JobId = 0;
	m_JobInfo.PagesPrinted = 0;
	m_JobInfo.Position = 0;
	m_JobInfo.Priority = 0;
	m_JobInfo.Size = 0;
	m_JobInfo.SPLSize = 0;
	m_JobInfo.StartTime = 0;
	m_JobInfo.Status = 0;
	memset(&m_JobInfo.Submitted, 0, sizeof(SYSTEMTIME));
	m_JobInfo.szDatatype.Empty();
	m_JobInfo.szDocument.Empty();
	m_JobInfo.szDriverName.Empty();
	m_JobInfo.szMachineName.Empty();
	m_JobInfo.szNotifyName.Empty();
	m_JobInfo.szParameters.Empty();
	m_JobInfo.szPortName.Empty();
	m_JobInfo.szPrinterName.Empty();
	m_JobInfo.szPrintProcessor.Empty();
	m_JobInfo.szStatus.Empty();
	m_JobInfo.szUserName.Empty();
	m_JobInfo.Time = 0;
	m_JobInfo.TotalPages = 0;
	m_JobInfo.UntilTime = 0;

	if (m_JobInfo.pDevMode)
	{
		delete m_JobInfo.pDevMode;
		m_JobInfo.pDevMode = NULL;
	}
	if (m_JobInfo.pSecurityDescriptor)
	{
		delete m_JobInfo.pSecurityDescriptor;
		m_JobInfo.pSecurityDescriptor = NULL;
	}
}

/************************************************************************/
/* 
Operating system		Version number	dwMajorVersion	dwMinorVersion	Other
Windows	8.1				6.3*			6				3				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2012 R2	6.3*			6				3				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows 8				6.2				6				2				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2012		6.2				6				2				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows 7				6.1				6				1				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2008 R2	6.1				6				1				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows Server 2008		6.0				6				0				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows Vista			6.0				6				0				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2003 R2	5.2				5				2				GetSystemMetrics(SM_SERVERR2) != 0
Windows Home Server		5.2				5				2				OSVERSIONINFOEX.wSuiteMask & VER_SUITE_WH_SERVER
Windows Server 2003		5.2				5				2				GetSystemMetrics(SM_SERVERR2) == 0
Windows XP Professional x64 Edition	5.2	5				2				(OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION) && (SYSTEM_INFO.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
Windows XP				5.1				5				1				Not applicable
Windows 2000			5.0				5				0				Not applicable*/
/************************************************************************/
BOOL CParseShd::IsWindowServer2008()
{
#if 0
	return FALSE;
#else
	OSVERSIONINFOEX  osvex;
	osvex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL bSuc = GetVersionEx((OSVERSIONINFO *)&osvex);
	if(osvex.dwMajorVersion == 6 && osvex.wProductType != VER_NT_WORKSTATION)
	{
		return TRUE;
	}
	return FALSE;
#endif

}
