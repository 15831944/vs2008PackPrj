#include "StdAfx.h"
#include "FileMap2.h"
#include <math.h>

CFileMap2::CFileMap2(void)
{
	m_pbFile = NULL;
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);
	m_dwAllocGranularity = sinf.dwAllocationGranularity;
	m_lExtendOnWriteLength = m_dwAllocGranularity;

	m_dwBytesInView = m_dwAllocGranularity;
	m_nCurPos = 0;
	m_nViewBegin = 0;
	m_pbFile = 0;
	m_hFileMapping = NULL;
	m_hFile = INVALID_HANDLE_VALUE;
	m_bFileExtended = false;
}

CFileMap2::~CFileMap2(void)
{
	Close();
}

BOOL CFileMap2::Create(CString strPath, UINT nLength, 
					  __in     DWORD dwShareMode,
					  __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
					  __in     DWORD dwCreationDisposition,
					  __in     DWORD dwFlagsAndAttributes)
{
	ASSERT(m_hFile == INVALID_HANDLE_VALUE);
	ASSERT(m_hFileMapping == NULL);
	if (m_hFile != INVALID_HANDLE_VALUE || m_hFileMapping != NULL)
		return FALSE;

	m_hFile = CreateFile(strPath, GENERIC_WRITE | GENERIC_READ,
		dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	m_nFileSize = nLength;

	m_dwflagsFileMapping = PAGE_READWRITE;

	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
	SECURITY_ATTRIBUTES sa = { sizeof(sa), &sd, FALSE };

	m_hFileMapping = CreateFileMapping(m_hFile, NULL,PAGE_READWRITE,0,m_nFileSize, 0);

	//在此分配空间
	//m_hFileMapping = CreateFileMapping(m_hFile, NULL, m_dwflagsFileMapping, 0, 	nLength, 0);

	if(NULL == m_hFileMapping)
	{
		//if(INVALID_HANDLE_VALUE != m_hFile)
		CloseHandle(m_hFile);
		//m_strErrMsg = MMF_ERR_CREATEFILEMAPPING;
		return FALSE;
	}

	if(m_nFileSize <= m_dwBytesInView)
		m_dwBytesInView = m_nFileSize;


	m_dwflagsView =  FILE_MAP_WRITE;

	m_pbFile = (PBYTE)MapViewOfFile(m_hFileMapping, m_dwflagsView, 0, 0, 
		m_dwBytesInView);

	if(NULL == m_pbFile)
	{
		CloseHandle(m_hFileMapping);
		CloseHandle(m_hFile);
		//m_strErrMsg = MMF_ERR_MAPVIEWOFFILE;
		return FALSE;
	}

	m_nCurPos = 0;
	m_nViewBegin = 0;
	m_bFileExtended = false;
	return true;
}

BOOL CFileMap2::Open(__in DWORD flProtect, __in CString strPath, __in DWORD dwDesiredAccess, __in DWORD dwShareMode,
					__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes, __in DWORD dwCreationDisposition,
					__in DWORD dwFlagsAndAttributes)
{
	ASSERT(m_hFile == INVALID_HANDLE_VALUE);
	ASSERT(m_hFileMapping == NULL);
	if (m_hFile != INVALID_HANDLE_VALUE || m_hFileMapping != NULL)
		return FALSE;

	m_hFile = CreateFile(strPath, dwDesiredAccess,
		dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD dwFileSizeHigh;
	m_nFileSize = GetFileSize(m_hFile, &dwFileSizeHigh);
	m_nFileSize += (((INT64) dwFileSizeHigh) << 32);


	if(m_nFileSize == 0)
	{
		CloseHandle(m_hFile);
		//m_strErrMsg = MMF_ERR_ZERO_BYTE_FILE;
		return FALSE;
	}

	// Create the file-mapping object.
	m_dwflagsFileMapping = flProtect;

	m_hFileMapping = CreateFileMapping(m_hFile, NULL, m_dwflagsFileMapping, 0, 
		0, 0);

	if(NULL == m_hFileMapping)
	{
		//if(INVALID_HANDLE_VALUE != m_hFile)
		CloseHandle(m_hFile);
		//m_strErrMsg = MMF_ERR_CREATEFILEMAPPING;
		return FALSE;
	}

	if(m_nFileSize <= m_dwBytesInView)
		m_dwBytesInView = m_nFileSize;

	//m_dwflagsView = (OPN_WRITE == oflags || OPN_READWRITE == oflags)?
	//FILE_MAP_WRITE: FILE_MAP_READ;
	/*
	FILE_MAP_WRITE Specifies read-write access. The hFileMappingObject parameter must have been created with PAGE_READWRITE protection. A read-write view of the file is mapped. 
	FILE_MAP_READ Specifies read-only access. The hFileMappingObject parameter must have been created with PAGE_READWRITE or PAGE_READONLY protection. A read-only view of the file is mapped. 
	FILE_MAP_ALL_ACCESS Same as FILE_MAP_WRITE. 
	*/
	m_dwflagsView = (flProtect == PAGE_READWRITE) ? FILE_MAP_WRITE : FILE_MAP_READ;

	m_pbFile = (PBYTE)MapViewOfFile(m_hFileMapping, m_dwflagsView, 0, 0, 
		m_dwBytesInView);

	if(NULL == m_pbFile)
	{
		CloseHandle(m_hFileMapping);
		CloseHandle(m_hFile);
		//m_strErrMsg = MMF_ERR_MAPVIEWOFFILE;
		return FALSE;
	}

	m_nCurPos = 0;
	m_nViewBegin = 0;
	m_bFileExtended = false;
	return true;
}

void CFileMap2::Close()
{
	if(m_pbFile)
	{
		FlushViewOfFile(m_pbFile, 0);
		UnmapViewOfFile(m_pbFile);
		m_pbFile = NULL;
	}

	if(m_hFileMapping)
	{
		//close mapping object handle
		CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
	}
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	m_dwBytesInView = m_dwAllocGranularity;
}

int CFileMap2::Read(void* pBuf, UINT nCountIn)
{
	if(nCountIn ==0)return 0;
	if (!CheckHandleValid())
		return 0;
	_checkFileExtended();

	if(m_nCurPos >= m_nFileSize)return 0;

	int nCount = nCountIn;//int is used to detect any bug

	m_dwBytesInView = m_dwAllocGranularity;
	//check if m_nViewBegin+m_dwBytesInView crosses filesize
	if(m_nViewBegin + m_dwBytesInView > m_nFileSize)
	{
		m_dwBytesInView = m_nFileSize - m_nViewBegin;
	}

	INT64 nDataEndPos = m_nCurPos + nCount;
	if(nDataEndPos >= m_nFileSize)
	{
		nDataEndPos = m_nFileSize;
		nCount = m_nFileSize - m_nCurPos;
	}

	ASSERT(nCount >= 0);//nCount is int, if -ve then error

	INT64 nViewEndPos = m_nViewBegin + m_dwBytesInView;

	if(nDataEndPos < nViewEndPos)
	{
		memcpy_s(pBuf, nCountIn, m_pbFile + (m_nCurPos-m_nViewBegin), nCount);
		m_nCurPos += nCount;
	}
	else if(nDataEndPos == nViewEndPos)
	{
		//Last exact bytes are read from the view
		memcpy_s(pBuf, nCountIn, m_pbFile + (m_nCurPos-m_nViewBegin), nCount);
		m_nCurPos += nCount;

		_seek(m_nCurPos, SP_BEGIN);
		nViewEndPos = m_nViewBegin + m_dwBytesInView;
	}
	else
	{
		LPBYTE pBufRead = (LPBYTE)pBuf;
		if(nDataEndPos > nViewEndPos)
		{
			//nDataEndPos can span multiple view blocks
			while(m_nCurPos < nDataEndPos)
			{
				int nReadBytes = nViewEndPos - m_nCurPos;

				if(nViewEndPos > nDataEndPos)
					nReadBytes = nDataEndPos - m_nCurPos;

				memcpy_s(pBufRead, nCountIn, m_pbFile + (m_nCurPos-m_nViewBegin), nReadBytes);
				pBufRead += nReadBytes;

				m_nCurPos += nReadBytes;
				//seeking does view remapping if m_nCurPos crosses view boundary
				_seek(m_nCurPos, SP_BEGIN);
				nViewEndPos = m_nViewBegin + m_dwBytesInView;
			}
		}
	}

	return nCount;
}

bool CFileMap2::SetLength(const INT64& nLength)
{
	//unmap view 
	UnmapViewOfFile(m_pbFile);
	//close mapping object handle
	CloseHandle(m_hFileMapping);

	LONG nLengthHigh = (nLength >> 32);
	DWORD dwPtrLow = SetFilePointer(m_hFile, (LONG) (nLength & 0xFFFFFFFF),
		&nLengthHigh, FILE_BEGIN);

	if(INVALID_SET_FILE_POINTER == dwPtrLow && GetLastError() != NO_ERROR)
	{
		return false;
	}
	//set the eof to the file pointer position
	if(SetEndOfFile(m_hFile) == 0)
	{
		return false;
	}

	m_nFileSize = nLength;

	//call CreateFileMapping 
	m_hFileMapping = CreateFileMapping(m_hFile, NULL, m_dwflagsFileMapping,
		0, 0, _T("SMP"));

	//remap here
	m_pbFile = (PBYTE)MapViewOfFile(m_hFileMapping, m_dwflagsView, 
		(DWORD) (m_nViewBegin >> 32), (DWORD) (m_nViewBegin & 0xFFFFFFFF), 
		m_dwBytesInView);

	return true;
}

int CFileMap2::Write(void* pBuf, UINT nCount)
{
	if(nCount == 0)return 0;
	if (!CheckHandleValid())
		return 0;
	INT64 nViewEndPos = m_nViewBegin + m_dwBytesInView;
	INT64 nDataEndPos = m_nCurPos + nCount;

	if(nDataEndPos > nViewEndPos)
	{
		if(nDataEndPos > m_nFileSize)
		{
			//Extend the end position by m_lExtendOnWriteLength bytes
			INT64 nNewFileSize = nDataEndPos + m_lExtendOnWriteLength;
			CCommonFun::WriteFileLog(_T("m_nViewBegin = %d, m_dwBytesInView = %d , m_nCurPos = %d, nCount = %d"),
				m_nViewBegin, m_dwBytesInView, m_nCurPos, nCount);
			//__asm int 3;
			if(SetLength(nNewFileSize))
			{
				m_bFileExtended = true;
			}
		}

		LPBYTE pBufWrite = (LPBYTE)pBuf;
		while(m_nCurPos < nDataEndPos)
		{
			int nWriteBytes = nViewEndPos - m_nCurPos;

			if(nViewEndPos > nDataEndPos)
				nWriteBytes = nDataEndPos - m_nCurPos;

			memcpy_s(&m_pbFile[m_nCurPos-m_nViewBegin], m_dwBytesInView, pBufWrite, nWriteBytes);
			pBufWrite += nWriteBytes;

			m_nCurPos += nWriteBytes;
			//seeking does view remapping if m_nCurPos crosses view boundary
			_seek(m_nCurPos, SP_BEGIN);
			nViewEndPos = m_nViewBegin + m_dwBytesInView;
		}
	}
	else
	{
		//nCount bytes lie within the current view
		memcpy_s(&m_pbFile [m_nCurPos-m_nViewBegin], nCount, pBuf, nCount);
		m_nCurPos += nCount;
	}

	return nCount;
}


bool CFileMap2::Seek(INT64 lOffset/*can be -ve */, SEEKPOS eseekpos)
{
	_checkFileExtended();
	bool bRet = _seek(lOffset, eseekpos);
	return bRet;
}

bool CFileMap2::_seek(INT64 lOffset/*can be -ve */, SEEKPOS eseekpos)
{
	if(SP_CUR == eseekpos)
	{
		lOffset = m_nCurPos + lOffset;
	}
	else if(SP_END == eseekpos)
	{
		if(lOffset >= 0)
		{
			return false;
		}

		//lOffset in RHS is -ve
		lOffset = m_nFileSize + lOffset;
	}
	//else means SP_BEGIN


	//lOffset must be less than the file size
	if(!(lOffset >= 0 && lOffset < m_nFileSize))
	{
		return false;
	}

	if(!(lOffset >= m_nViewBegin && lOffset < m_nViewBegin + m_dwBytesInView))
	{
		//__asm int 3;
		//lOffset lies outside the mapped view, remap the view
		INT64 _N = (INT64)floor((double)lOffset/((double)m_dwAllocGranularity));
		m_nViewBegin = _N*m_dwAllocGranularity;
		m_dwBytesInView = m_dwAllocGranularity;
		//check if m_nViewBegin+m_dwBytesInView crosses filesize
		if(m_nViewBegin + m_dwBytesInView > m_nFileSize)
		{
			m_dwBytesInView = m_nFileSize - m_nViewBegin;
		}
		if(m_dwBytesInView != 0 && m_pbFile)
		{
			//Unmap old view
			UnmapViewOfFile(m_pbFile);
			//Remap with new starting address
			m_pbFile = (PBYTE)MapViewOfFile(m_hFileMapping, m_dwflagsView, 
				(DWORD) (m_nViewBegin >> 32), (DWORD) (m_nViewBegin & 0xFFFFFFFF), 
				m_dwBytesInView);

			//DWORD err = GetLastError();
		}
	}

	m_nCurPos = lOffset;
	return true;
}

UINT64 CFileMap2::GetLength()
{
	_checkFileExtended();
	return m_nFileSize;
}

UINT64 CFileMap2::GetPosition()
{
	return m_nCurPos;
}

/*
If file is extended in Write function then this must be called to re-adjust
the file to its actual length before Seek or Read or any such fuction.
*/
bool CFileMap2::_checkFileExtended()
{
	bool bRet = true;
	if(m_bFileExtended)
	{
		//remove extra bytes
		bRet = SetLength(m_nCurPos);
	}
	m_bFileExtended = false;
	return bRet;
}

BOOL CFileMap2::CheckHandleValid()
{
	return m_hFile != INVALID_HANDLE_VALUE && m_hFileMapping != NULL && m_pbFile != NULL;
}

BOOL CFileMap2::OpenReadOnly(CString strPath, DWORD dwShareMode /*= FILE_SHARE_READ*/)
{
	return Open(PAGE_READONLY, strPath, GENERIC_READ, dwShareMode, NULL, OPEN_EXISTING, NULL);
}