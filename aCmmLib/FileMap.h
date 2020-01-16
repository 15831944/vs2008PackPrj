#pragma once

class _AA_DLL_EXPORT_ CFileMap
{
public:
	CFileMap(void);
public:
	virtual ~CFileMap(void);
	BOOL Create(CString sName,int nSize);
	BOOL Open(CString sName,int nSize);
	void Close();
protected:
	HANDLE m_hMapHandle;
	void* m_pMapMem;
};
