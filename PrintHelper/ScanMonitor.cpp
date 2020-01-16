// ScanMonitor.cpp: implementation of the CScanMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanMonitor.h"
#include "../Public/PdfHelperDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


BOOL CScanInfo::Attach(CString& sPath)
{
	m_sPath = sPath;
	COleDateTime dt = COleDateTime::GetCurrentTime();
	m_dt = dt.m_dt;
	theLog.Write("CScanInfo::Attach,1,sPath=%s", sPath);
	
	CFileStatus st;
	if(!CFile::GetStatus(sPath,st))
	{
		theLog.Write("文件不存在 = %s",sPath);
		return FALSE;
	}
	m_nSize = st.m_size;
	do 
	{
		if (sPath.GetLength() > 4 && sPath.Right(4).CompareNoCase(".pdf") == 0)
		{
			m_nColor = 2;
			m_nPageCount = 1;
			CPdfHelper pdf;
			if (pdf.OpenFile(m_sPath.GetString()))
			{
				m_nPageCount = pdf.GetPageCount();
			}
			theLog.Write("CScanInfo::Attach,2,PDF,m_nPageCount=%d", m_nPageCount);
			break;
		}
		if (sPath.GetLength() > 4 && sPath.Right(4).CompareNoCase(".xps") == 0)
		{
			m_nColor = 2;
			m_nPageCount = 1;
			theLog.Write("CScanInfo::Attach,3,XPS,m_nPageCount=%d", m_nPageCount);
			break;
		}
		CStringW wsPath(sPath);
		Image image(wsPath);
		Status st = image.GetLastStatus();
		if (st != Gdiplus::Ok)
		{
			theLog.Write("GetLastStatus = %s,%d",sPath,st);
			return FALSE;
		}
		UINT count = 0;
		count = image.GetFrameDimensionsCount();
		PixelFormat pf = image.GetPixelFormat();
		if (pf == PixelFormat1bppIndexed || pf == PixelFormat4bppIndexed ||  pf == PixelFormat8bppIndexed)
		{
			m_nColor = 1;
		}
		else
		{
			m_nColor = 2;
		}
		theLog.Write("color = %d,%d",m_nColor,pf);
		TRACE("The number of dimensions is %d.\n", count);
		GUID* pDimensionIDs = (GUID*)malloc(sizeof(GUID)*count);
		UINT frameCount = 0;
		// Get the list of frame dimensions from the Image object.
		image.GetFrameDimensionsList(pDimensionIDs, count);
		for (int n = 0 ; n < count; n ++)
		{
			// Display the GUID of the first (and only) frame dimension.
			WCHAR strGuid[39];
			StringFromGUID2(pDimensionIDs[n], strGuid, 39);
			TRACE("The first (and only) dimension ID is %s.\n", strGuid);

			// Get the number of frames in the first dimension.
			frameCount += image.GetFrameCount(&pDimensionIDs[n]);
		}
		free(pDimensionIDs);
		m_nPageCount = frameCount;
		theLog.Write("CScanInfo::Attach,4,image,m_nPageCount=%d", m_nPageCount);
	} while (0);
		
	if (CompressTif())
	{
		m_sFileName = PathFindFileName(m_sZipFilePath);
		return TRUE;
	}
	return FALSE;
}
#define SCAN_JOB_DIR _T("scan_job")
BOOL CScanInfo::CompressTif()
{
	theLog.Write("CScanInfo::CompressTif,1");
	CString sDestPath;
	m_sDir.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SCAN_JOB_DIR);
	sDestPath.Format(_T("%s%s\\%s%s"),CCommonFun::GetDefaultPath(),SCAN_JOB_DIR,CCommonFun::NewGuid(),PathFindExtension(m_sPath));
	CCommonFun::ValidatePath(sDestPath);
	theLog.Write("tiff = %s,dest = %s",m_sPath,sDestPath);
	if(!PathFileExists(m_sPath))
	{
		theLog.Write("GetTiffFile %s not exist",m_sPath);
	}
	if(MoveFile(m_sPath,sDestPath) && PathFileExists(sDestPath))
	{
#if Enable_Router_Not_Compress_Scan_Doc
		m_sZipFilePath = sDestPath;
#else
		//压缩
		char* pPath = sDestPath.GetBuffer();
		file_compress(pPath,-1);
		sDestPath.ReleaseBuffer();
		m_sZipFilePath.Format("%s.gz",sDestPath);
#endif
		return TRUE;
	}
	theLog.Write("!!CScanInfo::CompressTif,2,fail,err=%u", GetLastError());
	return FALSE;
}
CScanMonitor::CScanMonitor()
{
	InterlockedExchange(&m_lActive,0);
	m_pCallback = 0;
	m_hShutdown = 0;
	m_hThread = 0;
	m_hThreadStarted = 0;
	m_aryData.Add(".jpg");
	m_aryData.Add(".tif");
	m_aryData.Add(".tiff");
	m_aryData.Add(".pdf");
}

CScanMonitor::~CScanMonitor()
{
	Stop();
	Shutdown();
}

void CScanMonitor::SetScanData(CString& s)
{
	m_aryData.RemoveAll();
	CCommonFun::StringSplit(s,&m_aryData,",");
}

BOOL CScanMonitor::IsTiff(CString& sPath)
{
	CFileStatus st;
	if(!CFile::GetStatus(sPath,st))
		return FALSE;
	//
	for (int n = 0; n < m_aryData.GetSize(); n ++)
	{
		CString s = m_aryData.GetAt(n);
		if (sPath.GetLength() > s.GetLength())
		{
			if(sPath.Right(s.GetLength()).CompareNoCase(s) == 0)
			{
				return TRUE;
			}
		}
	}
	/*
	if (sPath.GetLength() > 4)
	{
		if(sPath.Right(4).CompareNoCase(_T(".tif")) == 0)
		{
			return TRUE;
		}
	}
	if (sPath.GetLength() > 5)
	{
		if(sPath.Right(5).CompareNoCase(_T(".tiff")) == 0)
		{
			return TRUE;
		}
	}*/

	return FALSE;
}

BOOL CScanMonitor::IsActive()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lActive);
	return lVal;
}

void CScanMonitor::ReadChanges2(CString sDir)
{
	theLog.Write(_T("CScanMonitor::ReadChanges2,读取改变，目录：(%s)。"), sDir);
	if(sDir.IsEmpty())
		return;

	OVERLAPPED ol = {0};
	ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	

	HANDLE hDir = CreateFile(sDir, 
		FILE_LIST_DIRECTORY, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
		NULL
		);

	//theLog.Writeld(_T("\t打开目录句柄:0x%x\n"), hDir);
	if(!hDir)
		return;

	DWORD cbBuf=1024, dwRet;
	BYTE* pBuf = (BYTE*)malloc(cbBuf);
	BOOL bLoop = TRUE;
	while(bLoop)
	{
		memset(pBuf, 0xFF, 1024);
		BOOL bRet = ReadDirectoryChangesW(hDir, 
			pBuf, 
			cbBuf, 
			TRUE, 
			FILE_NOTIFY_CHANGE_FILE_NAME|	//modify by zxl,20151228,在一些打印机扫描的操作中，是先扫描文件（a.bcd），然后重命名为(a.pdf).
			/*FILE_NOTIFY_CHANGE_DIR_NAME|*/
			FILE_NOTIFY_CHANGE_LAST_WRITE//|
			/*FILE_NOTIFY_CHANGE_LAST_ACCESS|*/
			/*FILE_NOTIFY_CHANGE_SIZE*/, 
			&dwRet, 
			&ol, 
			NULL);
		if(!bRet)
		{
		//	theLog.Writelw(_T("读取目录改变错误, b:%d, err:%d\n"), bRet, GetLastError());
			break;
		}

		HANDLE h[2];
		h[0] = ol.hEvent;
		h[1] = m_hShutdown;
		DWORD dw = WaitForMultipleObjects(2, h, FALSE, INFINITE);
		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				BOOL bSuc = GetOverlappedResult(hDir, &ol, &dwRet, FALSE);
				if(bSuc)
				{
				//	theLog.Writeld(_T("读取目录改变成功。ret:%d, bytes:%d"), bSuc, dwRet);

					if(dwRet == 0)	//缓冲区不够大
					{
						if(cbBuf < 1024*1024)
						{
							cbBuf*=2;
							pBuf = (BYTE*)realloc(pBuf, cbBuf);
							if(!pBuf)
								break;
						}
				//		theLog.Writelw(_T("读取大小为0，现在缓冲区大小为:%d。\n"), cbBuf);
						continue;
					}

					FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*)pBuf;
					//DumpFNIBuffer(pInfo);
					WriteToMemFile(pInfo);
				}
				else
				{
					ASSERT(FALSE);
				//	theLog.Writelw(_T("读取目录改变失败。ret:%d, bytes:%d, err:%d"), bSuc, dwRet, GetLastError());
					bLoop = FALSE;
					break;
				}
			}
			break;
		case WAIT_OBJECT_0+1:
		//	theLog.Writeld(_T("接收到关闭事件。"));
			bLoop = FALSE;
			break;
		default:
			ASSERT(FALSE);
			//theLog.Writelw(_T("等待失败。dw:%d err:%d"), dw, GetLastError());
			break;
		}
		long lVal = 0;
		InterlockedExchange(&lVal,m_lActive);
		bLoop = lVal;
	}

	if(pBuf)
		free(pBuf);

	CloseHandle(ol.hEvent);
	CloseHandle(hDir);
	CloseHandle(m_hShutdown);
	m_hShutdown = NULL;
}

void CScanMonitor::DumpFNIBuffer(FILE_NOTIFY_INFORMATION* pInfo)
{
	ASSERT(pInfo);
	if(!pInfo)
		return;

	static PTSTR szAction[] = {
		_T(""), 
		_T("FILE_ACTION_ADDED"), 
		_T("FILE_ACTION_REMOVED"), 
		_T("FILE_ACTION_MODIFIED"), 
		_T("FILE_ACTION_RENAMED_OLD_NAME"), 
		_T("FILE_ACTION_RENAMED_NEW_NAME")};

	do 
	{
//		theLog.Writeld(_T("\tNext:%d"), pInfo->NextEntryOffset);
//		theLog.Writeld(_T("\tAction:%s"), szAction[pInfo->Action]);
//		theLog.Writeld(_T("\tNameLen:%d"), pInfo->FileNameLength);
//		CString s(pInfo->FileName, pInfo->FileNameLength/2);
//		theLog.Writeld(_T("\tName:%s"), s);

		if(pInfo->NextEntryOffset > 0)
			pInfo = (FILE_NOTIFY_INFORMATION*)(((BYTE*)pInfo)+pInfo->NextEntryOffset);
		else
			pInfo = NULL;
	} while(pInfo != NULL);
}

void CScanMonitor::WriteToMemFile(FILE_NOTIFY_INFORMATION* pInfo)
{
	ASSERT(pInfo);
	if(!pInfo)
		return;

//	CCriticalSection2::Owner lock(theCrit);
	BOOL bLoop = TRUE;
	do
	{
		if(pInfo->NextEntryOffset == 0)	//最后一条记录
		{
			bLoop = FALSE;
			pInfo->NextEntryOffset = sizeof(FILE_NOTIFY_INFORMATION)-4+pInfo->FileNameLength;
		}

		if(pInfo->Action == FILE_ACTION_MODIFIED || pInfo->Action == FILE_ACTION_RENAMED_NEW_NAME)	
		{
			CString sPath (pInfo->FileName, pInfo->FileNameLength/2);
			CString sFullPath;
			sFullPath.Format("%s\\%s",m_sDir,sPath);

			theLog.Write("Add File %s \n",CString(pInfo->FileName, pInfo->FileNameLength/2));
			if (m_pCallback && IsTiff(sFullPath))
			{
				if(IsActive())
				{
					Sleep(10);
					CScanInfo scan;
					if(scan.Attach(sFullPath))
					{
						m_pCallback->OnOneScanCmplt(&scan);
					}
					else
					{
						theLog.Write("get scan file err %s",sFullPath);
					}
				}
				else
				{
					theLog.Write("OnOneScanCmplt is not active");
				}
			}
		}
		if (pInfo->Action == FILE_ACTION_ADDED)
		{
			
		}
		else 
		{
			
//			theFile.SeekToEnd();
	//		theFile.Write(pInfo, pInfo->NextEntryOffset);
		}

		CString sPath (pInfo->FileName, pInfo->FileNameLength/2);
		CString sFullPath;
		sFullPath.Format("%s\\%s",m_sDir,sPath);
		theLog.Write("ad %d,%s ",pInfo->Action,sFullPath);
		if(bLoop)
			pInfo = (FILE_NOTIFY_INFORMATION*)(((BYTE*)pInfo)+pInfo->NextEntryOffset);
	}while(bLoop);
}

DWORD CScanMonitor::ThreadFn(void* pVoid)
{
	CScanMonitor* pThis = (CScanMonitor*)pVoid;
	if (pThis->m_hThreadStarted)
	{
		SetEvent(pThis->m_hThreadStarted);
	}
	

	pThis->ReadChanges2(pThis->m_sDir);

	return 0;
}

BOOL CScanMonitor::Start(CString sDir)
{
	InterlockedExchange(&m_lActive,1);
	if(!m_hShutdown)
	{
		m_sDir = sDir;
		m_hShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hThreadStarted = CreateEvent(0,0,0,0);

		DWORD dw = 0;
		m_hThread = ::CreateThread(0,0,ThreadFn,this,0,&dw);

		DWORD dwWait = WaitForSingleObject ( m_hThreadStarted , INFINITE );
	
		ASSERT ( dwWait ==	WAIT_OBJECT_0 );
		CloseHandle(m_hThreadStarted);
	}
	return TRUE;
}
void CScanMonitor::Shutdown()
{
	InterlockedExchange(&m_lActive,0);
	if (m_hShutdown && m_hThread)
	{
		SetEvent(m_hShutdown);
		DWORD dwWait = WaitForSingleObject ( m_hThread , INFINITE );
		CloseHandle(m_hThread);
		CloseHandle(m_hShutdown);
		m_hThread = 0;
		m_hShutdown = 0;
	}
}
void CScanMonitor::Stop()
{
	InterlockedExchange(&m_lActive,0);
	//清除这个文件夹下所有文件
	//theLog.Write("del dir %s ",m_sDir);
	//CCommonFun::DeleteDir(m_sDir,TRUE);
}

void CScanMonitor::SetCallback(IScanChargeInterface* pCall)
{
	ASSERT(pCall);
	m_pCallback = pCall;
}
