#pragma once
#include <aclapi.h>
#include "PipeClt.h"

class CAutoMutex
{
public:
	CAutoMutex(HANDLE& h,DWORD dwTime,DWORD& dwRet)
	{
		m_hMutex = h;
		m_dwRet = WaitForSingleObject(m_hMutex,dwTime);
		dwRet = m_dwRet;
	}
	~CAutoMutex()
	{
		if (m_dwRet == WAIT_OBJECT_0)
		{
			ReleaseMutex(m_hMutex);
		}	
	}
	HANDLE m_hMutex;
	DWORD m_dwRet;
};

class CDocumentArray : public CArray<CDocumentLogNode,CDocumentLogNode>
{
public:
	void Attach(void* pData,int nSize)
	{
		m_pData = (CDocumentLogNode*)pData;
		m_nSize = nSize;
	}
};

class CFileMap
{
public:
	CFileMap(void);
	virtual ~CFileMap(void);
	
	void Close();
	BOOL Create(char* szDoc = NULL);
	BOOL Open();
	void OnOpenFile(HANDLE h,CString sPath,CString sProcess); 
	void OnCloseFile(HANDLE h,CString sProcess);
	void OnPrinted(CString sPrintedDocName,CString sDocName,CString sProcess);
	void OnProcessClosed(DWORD dwPid);
protected:
	CString GetDocType();
	void AddNode(CString sProcess,CString sPath,HANDLE hFile,DWORD dwPid);
	BOOL CheckDocType(CString s,CString s2);
	BOOL Create(CString sName,int nSize);
	BOOL Open(CString sName,int nSize);
	BOOL InitMutex();
	BOOL NotifyPrinted(CDocumentLogNode* pNode);
	void FillNode(CDocumentLogNode* pNode,CString sProcess,CString sPath,HANDLE h,DWORD dwPid);
	void UpdateNode(CDocumentLogNode* pNode,CString sDocName);
protected:
	CString m_sMapName;
	DWORD m_dwTimeout;
	HANDLE m_hMapHandle;
	void* m_pMapMem;
	HANDLE m_hMutex;
	CRITICAL_SECTION m_cs;
	CPipeClt* m_pPipe;
	CMap<HANDLE, HANDLE, CString, CString> m_oHandleMap;	//本进程文件句柄
};




