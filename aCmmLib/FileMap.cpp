#include "stdafx.h"
#include "FileMap.h"

CFileMap::CFileMap(void)
{
	m_hMapHandle = NULL;
	m_pMapMem = NULL;
}

CFileMap::~CFileMap(void)
{
	Close();
}

BOOL CFileMap::Create(CString sName,int nMapSize)
{
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
	SECURITY_ATTRIBUTES sa = { sizeof(sa), &sd, FALSE };

	
	m_hMapHandle = CreateFileMapping( (HANDLE)0xFFFFFFFF, &sa,PAGE_READWRITE,0,nMapSize,(LPCTSTR)sName);
	if(!m_hMapHandle)
		return FALSE;
	m_pMapMem = MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS,0,0,0);
	if (!m_pMapMem) 
	{
		CloseHandle(m_hMapHandle);
		return  FALSE;
	}
	return TRUE;
}

BOOL CFileMap::Open(CString sName,int nMapSize)
{
	m_hMapHandle = ::OpenFileMapping(FILE_MAP_WRITE,FALSE,(LPCTSTR)sName);

	if(!m_hMapHandle)
	{
		int ierror=::GetLastError();
		return  FALSE;
	}

	m_pMapMem = MapViewOfFile(m_hMapHandle, FILE_MAP_WRITE,0,0,0);
	if (!m_pMapMem) 
	{
		CloseHandle(m_hMapHandle);
		return  FALSE;
	}
	return TRUE;
}

void CFileMap::Close()
{
	if (m_pMapMem)
	{
		::UnmapViewOfFile(m_pMapMem);
		m_pMapMem = NULL;
	}

	if (m_hMapHandle)
	{
		::CloseHandle(m_hMapHandle);
		m_hMapHandle = NULL;
	}
}