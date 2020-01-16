#include "StdAfx.h"
#include "ParseSpl.h"

void WriteFileLog(LPCTSTR lpszFormat, ...)
{
	//ASSERT(AfxIsValidString(lpszFormat));
	va_list argList;
	va_start(argList, lpszFormat);
	CString sLog;
	sLog.FormatV(lpszFormat, argList);
	va_end(argList);
	OutputDebugString(sLog);
}

CParseSpl::CParseSpl(void)
{
	m_szSplFilePath.Empty();
	m_szXtfFilePath.Empty();

	m_dwCopyCount = 0;
	m_dwPageCountPerCopy = 0;
	m_dwTotalPageCount = 0;

	m_bHaveDevmode = FALSE;
	m_pDevmode = NULL;
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	m_pFileData = NULL;
	m_hMapFile = NULL;
#endif
	m_dwFileSize = 0;
	m_bEmbedFont = FALSE;

	m_dwFilePostion = 0;
}

CParseSpl::~CParseSpl(void)
{
	Release();
}

BOOL CParseSpl::InitParse(CString szFilePath, BOOL bEmbedFont/* = FALSE*/)
{
	Release();

	if (IsSplFile(szFilePath))
	{
		m_szSplFilePath = szFilePath;
		m_szSplFilePath.MakeLower();
		m_szXtfFilePath.Format("%s.xtf", m_szSplFilePath.Left(m_szSplFilePath.GetLength()-4));
		m_bEmbedFont = bEmbedFont;
		return ParseSpl();
	}
	return FALSE;
}

void CParseSpl::Release()
{
	m_szSplFilePath.Empty();
	m_szXtfFilePath.Empty();

	m_dwCopyCount = 0;
	m_dwPageCountPerCopy = 0;
	m_dwTotalPageCount = 0;

	m_bHaveDevmode = FALSE;
	if (m_pDevmode)
	{
		delete[] m_pDevmode;
		m_pDevmode = NULL;
	}
	ClearEmfData();
	ClearXabFontData();
	CloseFile();
}

DWORD CParseSpl::GetCopyCount()
{
	return m_dwCopyCount;
}

DWORD CParseSpl::GetPageCountPerCopy()
{
	return m_dwPageCountPerCopy;
}

DWORD CParseSpl::GetTotalPageCount()
{
	return m_dwTotalPageCount;
}

DEVMODEW* CParseSpl::GetDevMode()
{
	if (m_bHaveDevmode && m_pDevmode)
	{
		return (DEVMODEW*)m_pDevmode;
	}
	else
	{
		WriteFileLog("!!CParseSpl::GetDevMode,1,m_bHaveDevmode=%d, m_pDevmode=%p", m_bHaveDevmode, m_bHaveDevmode);
		return NULL;
	}
}

BOOL CParseSpl::OpenFile()
{
	WriteFileLog("CParseSpl::OpenFile,1");

#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	DWORD dwExternDataSize = 0;
	CFileStatus XtfFileStatus;
	if (CFile::GetStatus(m_szXtfFilePath, XtfFileStatus))
	{
		dwExternDataSize = XtfFileStatus.m_size;
		WriteFileLog("CParseSpl::OpenFile,2,dwExternDataSize=%u", dwExternDataSize);
	}
	else
	{
		WriteFileLog("##CParseSpl::OpenFile,3,CFile::GetStatus fail,[%s], err=%u", m_szXtfFilePath, GetLastError());
	}

	HANDLE hFile;
	DWORD dwCreateFile = (dwExternDataSize > 0) ? GENERIC_READ|GENERIC_WRITE : GENERIC_READ;
	hFile = CreateFile(m_szSplFilePath, dwCreateFile, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::OpenFile,4,CreateFile fail,err=%d,m_szSplFilePath=[%s]",GetLastError(), m_szSplFilePath);
		return FALSE;
	}

	m_dwFileSize = GetFileSize(hFile, NULL);

	DWORD dwMapViewSize = m_dwFileSize + dwExternDataSize;
	DWORD dwCreateFileMapping = (dwExternDataSize > 0) ? PAGE_READWRITE : PAGE_READONLY;
	m_hMapFile = CreateFileMapping(hFile, NULL, dwCreateFileMapping, 0, dwMapViewSize, NULL);
	if (m_hMapFile == NULL) 
	{
		WriteFileLog("!!CParseSpl::OpenFile,5,CreateFileMapping fail,err=%d",GetLastError());
		goto ERR1;
	}

	DWORD dwMapViewOfFile = (dwExternDataSize > 0) ? (FILE_MAP_WRITE|FILE_MAP_READ) : FILE_MAP_READ;
	m_pFileData = (BYTE *)MapViewOfFile(m_hMapFile, dwMapViewOfFile, 0, 0, dwMapViewSize);
	if (m_pFileData == NULL) 
	{
		WriteFileLog("!!CParseSpl::OpenFile,6,MapViewOfFile fail,err=%d",GetLastError());
		goto ERR2;
	}

	//将字体文件[xtf]添加到spl文件中.
	AppendXabFontDataToFile();

	CloseHandle(hFile);
	hFile = NULL;
	return TRUE;

ERR1:
	UnmapViewOfFile(m_pFileData);
	m_pFileData = NULL;
ERR2:
	CloseHandle(m_hMapFile);
	m_hMapFile = NULL;
	CloseHandle(hFile);
	hFile = NULL;
	return FALSE;
#else
	if (m_oFile.Open(m_szSplFilePath, CFile::modeReadWrite))
	{
		m_dwFileSize = (DWORD)m_oFile.GetLength();
		AppendXabFontDataToFile();
	}
	else
	{
		WriteFileLog("!!CParseSpl::OpenFile,6,Open fail,m_szSplFilePath=%s,err=%u", m_szSplFilePath, GetLastError());
		return FALSE;
	}
	return TRUE;
#endif
}

void CParseSpl::CloseFile()
{
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	if (m_pFileData)
	{
		UnmapViewOfFile(m_pFileData);
		m_pFileData = NULL;
	}

	if (m_hMapFile)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
	}
#else
	if (m_oFile.m_hFile != CFile::hFileNull)
	{
		m_oFile.Close();
	}
#endif

	m_dwFilePostion = 0;
	m_dwFileSize = 0;
}

BOOL CParseSpl::HaveMoreData()
{
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	if (!m_pFileData)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::HaveMoreData,1,m_pFileData=%p", m_pFileData);
		return FALSE;
	}
#else
	if (m_oFile.m_hFile == CFile::hFileNull)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::HaveMoreData,1,m_oFile.m_hFile=%d", m_oFile.m_hFile);
		return FALSE;
	}
#endif
	else if (m_dwFilePostion == m_dwFileSize)
	{
		WriteFileLog("##CParseSpl::HaveMoreData,2,m_dwFilePostion=%u,m_dwFileSize=%u"
			, m_dwFilePostion, m_dwFileSize);
		return FALSE;
	}
	else if (m_dwFilePostion > m_dwFileSize)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::HaveMoreData,3,m_dwFilePostion=%u,bigger m_dwFileSize=%u"
			, m_dwFilePostion, m_dwFileSize);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CParseSpl::GetSplHead(SPL_HEADER& header)
{
	m_dwFilePostion = 0;
	if (!ReadData(&header, sizeof(SPL_HEADER), m_dwFilePostion))
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::GetSplHead,ReadData fail");
		return FALSE;
	}

	m_dwFilePostion = sizeof(SPL_HEADER);
	return TRUE;
}

BOOL CParseSpl::SkipSplHead(SPL_HEADER& header)
{	
	if (header.nSize < m_dwFileSize)
	{
		m_dwFilePostion = header.nSize;
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::SkipSplHead,header.nSize=%d,m_dwFileSize=%d"
			,header.nSize,m_dwFileSize);
		return FALSE;
	}
}

BOOL CParseSpl::GetSplNextSmr(SMR& smr)
{
	if (!ReadData(&smr, sizeof(SMR), m_dwFilePostion))
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::GetSplNextSmr,1,ReadData fail");
		return FALSE;
	}

	m_dwFilePostion += sizeof(SMR);
	return TRUE;
}

BOOL CParseSpl::SkipSplSmr(SMR& smr)
{
	if (smr.nSize <= 0)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::SkipSplSmr,1,smr.iType=%u,smr.nSize=%u", smr.iType, smr.nSize);
		return FALSE;
	}

	if ((m_dwFilePostion+smr.nSize)<=m_dwFileSize)
	{
		m_dwFilePostion += smr.nSize;
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::SkipSplSmr,2,(m_dwFilePostion+smr.nSize)<=m_dwFileSize,"
			"(m_dwFilePostion+smr.nSize)=%d,m_dwFileSize=%d", (m_dwFilePostion+smr.nSize), m_dwFileSize);
		return FALSE;
	}
}

BOOL CParseSpl::GetDevmode(SMR& smr)
{
	if (smr.iType != SRT_DEVMODE)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::GetDevmode,1,smr.iType != SRT_DEVMODE, smr.iType=%d", smr.iType);
		return FALSE;
	}

	if (m_pDevmode)
	{
		delete[] m_pDevmode;
		m_pDevmode = NULL;
	}

	if ((m_dwFilePostion+smr.nSize) > m_dwFileSize)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::GetDevmode,2,(m_dwFilePostion+smr.nSize) > m_dwFileSize,"
			"(m_dwFilePostion+smr.nSize)=%d, m_dwFileSize=%d", (m_dwFilePostion+smr.nSize), m_dwFileSize);
		return FALSE;
	}


	m_pDevmode = new BYTE[smr.nSize];
	memset(m_pDevmode, 0x0, smr.nSize);
	if (!ReadData(m_pDevmode, smr.nSize, m_dwFilePostion))
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::GetDevmode,3,ReadData fail");
		return FALSE;
	}

	DEVMODEW *pDevMode = (DEVMODEW*)m_pDevmode;
	m_dwCopyCount = pDevMode->dmCopies;	
	m_bHaveDevmode = TRUE;
	WriteFileLog("##CParseSpl::GetDevmode,5,m_dwCopyCount=%d,m_bHaveDevmode=%d", m_dwCopyCount, m_bHaveDevmode);
	return TRUE;
}

BOOL CParseSpl::IsSplFile(CString szFilePath)
{
	if (szFilePath.GetLength()>4)
	{
		CString szExt = szFilePath.Right(4);
		if (szExt.CompareNoCase(SPL_EXT) == 0)
		{
			return TRUE;
		}
	}
	WriteFileLog("!!CParseSpl::IsSplFile,1,szFilePath=%s",szFilePath);
	ASSERT(FALSE);
	return FALSE;
}

BOOL CParseSpl::ParseSpl()
{
	if (!OpenFile())
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::ParseSpl,1,OpenFile fail");
		return FALSE;
	}

	SPL_HEADER header;
	if (GetSplHead(header))
	{
		if (!SkipSplHead(header))
		{
			WriteFileLog("!!CParseSpl::ParseSpl,2,SkipSplHead fail");
			return FALSE;
		}

		while(HaveMoreData())
		{
			SMR smr = {0};
			if (!GetSplNextSmr(smr))
			{
				WriteFileLog("!!CParseSpl::ParseSpl,3,SkipSplSmr fail");
				return FALSE;
			}

			switch (smr.iType)
			{
			case SRT_METAFILE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_METAFILE");
				break;
			case SRT_ENGINE_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_ENGINE_FONT");
				break;
			case SRT_DEVMODE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_DEVMODE");
				GetDevmode(smr);
				break;
			case SRT_TYPE1_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_TYPE1_FONT");
				break;
			case SRT_PRESTARTPAGE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_PRESTARTPAGE");
				break;
			case SRT_DESIGNVECTOR:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_DESIGNVECTOR");
				break;
			case SRT_SUBSET_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_SUBSET_FONT");
				break;
			case SRT_DELTA_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_DELTA_FONT");
				break;
			case SRT_FORM_METAFILE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_FORM_METAFILE");
				break;
			case SRT_BW_METAFILE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_BW_METAFILE");
				break;
			case SRT_BW_FORM_METAFILE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_BW_FORM_METAFILE");
				break;
			case SRT_METAFILE_DATA:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_METAFILE_DATA");
				AddOneEmfData(smr);
				break;
			case SRT_EXT_METAFILE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_METAFILE");
				break;
			case SRT_EXT_BW_METAFILE:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_BW_METAFILE");
				break;
			case SRT_EXT_ENGINE_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_ENGINE_FONT");
				//AddOneFontData(smr); //del by zxl,20160530，驱动中嵌入字体有完善，此处可以不需要二次嵌入
				break;
			case SRT_EXT_TYPE1_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_TYPE1_FONT");
				break;
			case SRT_EXT_DESIGNVECTOR:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_DESIGNVECTOR");
				break;
			case SRT_EXT_SUBSET_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_SUBSET_FONT");
				//AddOneFontData(smr); //好像与SRT_XAB_EMBED_FONT_DATA在相同字体的情况下有冲突
				break;
			case SRT_EXT_DELTA_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_DELTA_FONT");
				break;
			case SRT_EXT_PS_JOB_DATA:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_PS_JOB_DATA");
				break;
			case SRT_EXT_EMBED_FONT:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_EXT_EMBED_FONT");
				break;
			case SRT_XAB_EMBED_FONT_DATA:
				WriteFileLog("##CParseSpl::ParseSpl,SRT_XAB_EMBED_FONT_DATA");
				AddOneXabFontData(smr);
				break;
			default:
				WriteFileLog("##CParseSpl::ParseSpl,default Unknow SRT_Type=%d", smr.iType);
				break;
			}

			if (!SkipSplSmr(smr))
			{
				WriteFileLog("!!CParseSpl::ParseSpl,3,SkipSplSmr fail");
				return FALSE;
			}
		}
		if (m_dwCopyCount <= 0)
		{
			WriteFileLog("##CParseSpl::ParseSpl,4.A,,m_dwCopyCount=%d, ForceSet 1 copy", m_dwCopyCount);
			m_dwTotalPageCount = m_dwPageCountPerCopy * 1;
		}
		else
		{
			m_dwTotalPageCount = m_dwPageCountPerCopy * m_dwCopyCount;
		}
		if (m_dwPageCountPerCopy <= 0)
		{
			WriteFileLog("!!CParseSpl::ParseSpl,4.B,ParseSpl fail,m_dwPageCountPerCopy=%d", m_dwPageCountPerCopy);
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::ParseSpl,5,GetSplHead fail");
		return FALSE;
	}	
}

void CParseSpl::ClearEmfData()
{
	POSITION pos = m_oPageMap.GetStartPosition();
	int iKey;
	PEMF_DATA pValue;
	while (pos != NULL)
	{
		m_oPageMap.GetNextAssoc(pos, iKey, pValue);
		if (pValue)
		{
			pValue->pData = NULL;
			pValue->dwOff = 0;
			pValue->dwSize = 0;
			delete pValue;
		}
	}
	m_oPageMap.RemoveAll();
}

#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
PEMF_DATA CParseSpl::GetEmfDataByPageIndex(DWORD dwPageIndex)
{
	PEMF_DATA emf = NULL;
	if ((dwPageIndex>=0) && (dwPageIndex<m_oPageMap.GetCount()))
	{
		if (m_oPageMap.Lookup(dwPageIndex, emf))
		{
			if (emf && emf->pData)
			{
				//emf->pData = GetEmfData(emf->dwOff, emf->dwSize);				
			}
			else
			{
				WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,1,Lookup succ,emf=%p,emf->pData=%p", emf, emf->pData);
				ASSERT(FALSE);
				emf = NULL;
			}
		}
		else
		{
			WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,2,Lookup fail,dwPageIndex=%d", dwPageIndex);
		}
	}
	else
	{
		WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,3,dwPageIndex=%d, m_oPageMap.GetCount=%d"
			, dwPageIndex, m_oPageMap.GetCount());
	}
	return emf;
}
#else
HENHMETAFILE CParseSpl::GetEmfDataByPageIndex(DWORD dwPageIndex)
{
	HENHMETAFILE hEmf = NULL;
	if ((dwPageIndex>=0) && (dwPageIndex<m_oPageMap.GetCount()))
	{
		PEMF_DATA emf = NULL;
		if (m_oPageMap.Lookup(dwPageIndex, emf))
		{
			if (emf)
			{
				emf->pData = new BYTE[emf->dwSize];
				if (ReadData(emf->pData, emf->dwSize, emf->dwOff))
				{
					hEmf = SetEnhMetaFileBits(emf->dwSize, emf->pData);
					if (!hEmf)
					{
						WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,1,SetEnhMetaFileBits fail, err=%u", GetLastError());
					}
				}
				else
				{
					WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,2,ReadData fail");
				}
				delete[] emf->pData;
				emf->pData = NULL;
			}
			else
			{
				WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,3,Lookup succ,emf=%p", emf);
				ASSERT(FALSE);
			}
		}
		else
		{
			WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,4,Lookup fail,dwPageIndex=%d", dwPageIndex);
		}
	}
	else
	{
		WriteFileLog("!!CParseSpl::GetEmfDataByPageIndex,5,dwPageIndex=%d, m_oPageMap.GetCount=%d"
			, dwPageIndex, m_oPageMap.GetCount());
	}
	return hEmf;
}
#endif

BOOL CParseSpl::AddOneEmfData(SMR& smr)
{
	if (smr.iType != SRT_METAFILE_DATA)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::AddOneEmfData,1,smr.iType != SRT_METAFILE_DATA, smr.iType=%d", smr.iType);
		return FALSE;
	}

	if ((m_dwFilePostion+smr.nSize) > m_dwFileSize)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::AddOneEmfData,2,(m_dwFilePostion+smr.nSize) > m_dwFileSize,"
			"(m_dwFilePostion+smr.nSize)=%d, m_dwFileSize=%d", (m_dwFilePostion+smr.nSize), m_dwFileSize);
		return FALSE;
	}

	EMF_DATA *pEmf = new EMF_DATA;
	memset(pEmf, 0x0, sizeof(EMF_DATA));
	pEmf->dwOff = m_dwFilePostion;
	pEmf->dwSize = smr.nSize;
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	pEmf->pData = m_pFileData + m_dwFilePostion;
#else
	pEmf->pData = NULL;
#endif
	m_oPageMap.SetAt(m_dwPageCountPerCopy, pEmf);
	m_dwPageCountPerCopy++;
	WriteFileLog("CParseSpl::AddOneEmfData,3,PageIndex=%d", m_dwPageCountPerCopy);
	return TRUE;
}

BOOL CParseSpl::AddOneFontData(SMR& smr)
{
	if (!m_bEmbedFont)
	{
		WriteFileLog("##CParseSpl::AddOneFontData,1,m_bEmbedFont=%d, No Embed Font", m_bEmbedFont);
		return TRUE;
	}

	switch (smr.iType)
	{
	case SRT_EXT_ENGINE_FONT:
		{
			SMR_EXTFONT ExtFont;
			if (ReadData(&ExtFont, sizeof(SMR_EXTFONT), m_dwFilePostion-sizeof(SMR)))
			{
				XAB_FONT_DATA *pFontData = new XAB_FONT_DATA;
				memset(pFontData, 0x0, sizeof(XAB_FONT_DATA));
				//wcscpy_s(pFontData->lfFaceName, LF_FACESIZE, pFont->lfFaceName);
				pFontData->dwOff = m_dwFilePostion;
				pFontData->dwSize = ExtFont.smrext.DistanceLow;
				DWORD dwPostion = m_dwFilePostion + sizeof(SMR_EXTFONT) - ExtFont.smrext.DistanceLow;
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
				pFontData->pData = m_pFileData + dwPostion;
				pFontData->hFont = InstallFont(pFontData->pData, pFontData->dwSize);
				m_oFontList.AddTail(pFontData);
#else
				pFontData->pData = new BYTE[pFontData->dwSize];
				if (!ReadData(pFontData->pData, pFontData->dwSize, dwPostion))
				{
					WriteFileLog("!!CParseSpl::AddOneXabFontData,2,ReadData fail");
					delete pFontData;
				}
				else
				{
					pFontData->hFont = InstallFont(pFontData->pData, pFontData->dwSize);
					m_oFontList.AddTail(pFontData);
				}
				delete[] pFontData->pData;
				pFontData->pData = NULL;
#endif
				CStringW szFaceNameW(pFontData->lfFaceName);
				CString szFaceName(szFaceNameW);
				WriteFileLog("CParseSpl::AddOneXabFontData,3,lfFaceName=[%s],dwSize=%d", szFaceName.GetString(), pFontData->dwSize);
			}
			else
			{
				WriteFileLog("!!CParseSpl::AddOneXabFontData,4,ReadData fail");
			}
		}
		break;
	case SRT_EXT_SUBSET_FONT:
		{
			SMR_EXTFONT ExtFont;
			if (ReadData(&ExtFont, sizeof(SMR_EXTFONT), m_dwFilePostion-sizeof(SMR)))
			{
				XAB_FONT_DATA *pFontData = new XAB_FONT_DATA;
				memset(pFontData, 0x0, sizeof(XAB_FONT_DATA));
				//wcscpy_s(pFontData->lfFaceName, LF_FACESIZE, pFont->lfFaceName);
				pFontData->dwOff = m_dwFilePostion-ExtFont.smrext.DistanceLow;
				pFontData->dwSize = ExtFont.smrext.DistanceLow;
				DWORD dwPostion = m_dwFilePostion + sizeof(SMR) - ExtFont.smrext.DistanceLow;
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
				pFontData->pData = m_pFileData + dwPostion;
				pFontData->hFont = InstallFont(pFontData->pData, pFontData->dwSize);
				m_oFontList.AddTail(pFontData);
#else
				pFontData->pData = new BYTE[pFontData->dwSize];
				if (!ReadData(pFontData->pData, pFontData->dwSize, dwPostion))
				{
					WriteFileLog("!!CParseSpl::AddOneXabFontData,5,ReadData fail");
					delete pFontData;
				}
				else
				{
					pFontData->hFont = InstallFont(pFontData->pData, pFontData->dwSize);
					m_oFontList.AddTail(pFontData);
				}
				delete[] pFontData->pData;
				pFontData->pData = NULL;
#endif
				CStringW szFaceNameW(pFontData->lfFaceName);
				CString szFaceName(szFaceNameW);
				WriteFileLog("CParseSpl::AddOneXabFontData,6,lfFaceName=[%s],dwSize=%d", szFaceName.GetString(), pFontData->dwSize);
			}
			else
			{
				WriteFileLog("!!CParseSpl::AddOneXabFontData,7,ReadData fail");
			}
		}
		break;
	}

	return TRUE;
}

//字体数据操作
void CParseSpl::ClearXabFontData()
{
	int nCount = m_oFontList.GetCount();
	for (int nIndex=0; nIndex<nCount; nIndex++)
	{
		PXAB_FONT_DATA pFont = m_oFontList.GetTail();
		if (pFont)
		{
			if (pFont->hFont)
			{
				if (!RemoveFontMemResourceEx(pFont->hFont))
				{
					WriteFileLog("!!CParseSpl::ClearXabFontData,RemoveFontMemResourceEx fail,err=%d", GetLastError());
					ASSERT(FALSE);
				}
				pFont->hFont = NULL;
			}
			delete pFont;
		}
		m_oFontList.RemoveTail();
	}
	m_oFontList.RemoveAll();
}

BOOL CParseSpl::AddOneXabFontData(SMR& smr)
{
	if (!m_bEmbedFont)
	{
		WriteFileLog("##CParseSpl::AddOneXabFontData,1,m_bEmbedFont=%d,Not Embed Font", m_bEmbedFont);
		return TRUE;
	}

	if (smr.iType != SRT_XAB_EMBED_FONT_DATA)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::AddOneXabFontData,2,smr.iType != SRT_DEVMODE, smr.iType=%d", smr.iType);
		return FALSE;
	}

	SMR_XAB_FONT XabFont;
	if (ReadData(&XabFont, sizeof(SMR_XAB_FONT), m_dwFilePostion-sizeof(SMR)))
	{
		XAB_FONT_DATA *pFontData = new XAB_FONT_DATA;
		memset(pFontData, 0x0, sizeof(XAB_FONT_DATA));
		wcscpy_s(pFontData->lfFaceName, LF_FACESIZE, XabFont.lfFaceName);
		pFontData->dwOff = m_dwFilePostion;
		pFontData->dwSize = smr.nSize - sizeof(pFontData->lfFaceName);
		DWORD dwPostion = m_dwFilePostion + sizeof(pFontData->lfFaceName);
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
		pFontData->pData = m_pFileData + dwPostion;
		pFontData->hFont = InstallFont(pFontData->pData, pFontData->dwSize);
		m_oFontList.AddTail(pFontData);
#else
		pFontData->pData = new BYTE[pFontData->dwSize];
		if (!ReadData(pFontData->pData, pFontData->dwSize, dwPostion))
		{
			WriteFileLog("!!CParseSpl::AddOneXabFontData,3,ReadData fail");
			delete pFontData;
		}
		else
		{
			pFontData->hFont = InstallFont(pFontData->pData, pFontData->dwSize);
			m_oFontList.AddTail(pFontData);
		}
		delete[] pFontData->pData;
		pFontData->pData = NULL;
#endif
		CStringW szFaceNameW(pFontData->lfFaceName);
		CString szFaceName(szFaceNameW);
		WriteFileLog("CParseSpl::AddOneXabFontData,4,lfFaceName=[%s],dwSize=%d", szFaceName.GetString(), pFontData->dwSize);
	}

	return TRUE;
}

HANDLE CParseSpl::InstallFont(BYTE* pFontData, int nFontSize)
{
	HANDLE hFont = NULL;
	if (m_bEmbedFont)
	{
		DWORD nFonts;
		hFont = AddFontMemResourceEx(pFontData, nFontSize, NULL, &nFonts);
		if(!hFont)
		{
			WriteFileLog("!!CParseSpl::InstallFont,hFont=%x,err=%u", hFont, GetLastError());
			ASSERT(FALSE);
		}
	}
	return hFont;
}

BOOL CParseSpl::AppendXabFontDataToFile()
{
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	if (!m_pFileData)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::AppendXabFontDataToFile,1,m_pFileData=%p", m_pFileData);
		return FALSE;
	}
#else
	if (m_oFile.m_hFile == CFile::hFileNull)
	{
		ASSERT(FALSE);
		WriteFileLog("!!CParseSpl::AppendXabFontDataToFile,1,m_oFile.m_hFile=%d", m_oFile.m_hFile);
		return FALSE;
	}
#endif

	if (!PathFileExists(m_szXtfFilePath))
	{
		WriteFileLog("##CParseSpl::AppendXabFontDataToFile,2,PathFileExists not exist m_szXtfFilePath=%s", m_szXtfFilePath);
		return FALSE;
	}

	CFile file;
	if (file.Open(m_szXtfFilePath, CFile::modeRead))
	{
		DWORD dwSizeUnit = 1024*1024*10;
		BYTE *pData = new BYTE[dwSizeUnit];
		DWORD dwRead = 0;
		do 
		{
			memset(pData, 0x0, dwSizeUnit);
			dwRead = file.Read(pData,dwSizeUnit);
			if (dwRead>0)
			{
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
				memcpy_s(m_pFileData+m_dwFileSize, dwRead, pData, dwRead);
				FlushViewOfFile(m_pFileData+m_dwFileSize, dwRead);
#else
				m_oFile.SeekToEnd();
				m_oFile.Write(pData, dwRead);
				m_oFile.Flush();
#endif
				m_dwFileSize += dwRead;
			}
		} while (dwRead>0);

		file.Close();
		delete[] pData;
		//删除文件
		DeleteFile(m_szXtfFilePath);
		return TRUE;
	}
	else
	{
		WriteFileLog("!!CParseSpl::AppendXabFontDataToFile,3,file.Open fail,err=%u,m_szXtfFilePath=[%s]"
			, GetLastError(), m_szXtfFilePath);
		return FALSE;
	}
}

//从spl文件中读取，从指定位置(dwPostion)读取指定长度的数据(dwCount)
DWORD CParseSpl::ReadData(void* pBuf, DWORD dwCount, DWORD dwPostion)
{
	BOOL bRet = FALSE;

#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	if (!pBuf || !m_pFileData || dwCount<0)
	{
		WriteFileLog("!!CParseSpl::ReadData,1,fail,pBuf=%p,m_pFileData=%p,dwCount=%u"
			, pBuf, m_pFileData, dwCount);
		return bRet;
	}
#else
	if (!pBuf || m_oFile.m_hFile==CFile::hFileNull || dwCount<=0)
	{
		WriteFileLog("!!CParseSpl::ReadData,1,fail,pBuf=%p,m_oFile.m_hFile=%d,dwCount=%u"
			, pBuf, m_oFile.m_hFile, dwCount);
		return bRet;
	}
#endif

	if (dwPostion + dwCount > m_dwFileSize)
	{
		WriteFileLog("!!CParseSpl::ReadData,2,fail,dwPostion=%d,dwCount=%d, m_dwFileSize=%d"
			, dwPostion, dwCount, m_dwFileSize);
		return bRet;
	}

#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	if (memcpy_s(pBuf, dwCount, m_pFileData+dwPostion, dwCount) != 0)
	{
		WriteFileLog("!!CParseSpl::ReadData,3,memcpy_s fail,errno=%d", GetLastError());
	}
	else
	{
		bRet = TRUE;
	}
#else
	m_oFile.Seek(dwPostion, CFile::begin);
	DWORD dwRead = m_oFile.Read(pBuf, dwCount);
	if (dwRead != dwCount)
	{
		WriteFileLog("!!CParseSpl::ReadData,3,m_oFile.Read fail,dwRead=%u,dwCount=%d,errno=%d"
			, dwRead, dwCount, GetLastError());
	}
	else
	{
		bRet = TRUE;
	}
#endif

	return bRet;
}
