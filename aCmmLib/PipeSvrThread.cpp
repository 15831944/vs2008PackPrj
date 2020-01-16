// PipeSvrThread.cpp : 实现文件
//

#include "stdafx.h"
#include "CommonFun.h"
#include "PipeSvrThread.h"
/*#include "PipeCltThread.h"*/
#include "ThreadMgr.h"

#define DEF_BUFSIZE 4096
class   CShareRestrictedSD      
{  
public:  
	CShareRestrictedSD();  
	virtual   ~CShareRestrictedSD();  
	SECURITY_ATTRIBUTES* GetSA();  
protected:  
	PVOID     ptr;  
	SECURITY_ATTRIBUTES sa;  
	SECURITY_DESCRIPTOR   sd;  
};  
//////////////////////////////////////////////////////////////////////  
//   Construction/Destruction  
//////////////////////////////////////////////////////////////////////  
PVOID   BuildRestrictedSD(PSECURITY_DESCRIPTOR   pSD) 
{  
	DWORD     dwAclLength;  
	PSID       psidEveryone   =   NULL;  
	PACL       pDACL       =   NULL;  
	BOOL       bResult   =   FALSE;  
	PACCESS_ALLOWED_ACE   pACE   =   NULL;  
	SID_IDENTIFIER_AUTHORITY   siaWorld   =   SECURITY_WORLD_SID_AUTHORITY;  
	SECURITY_INFORMATION   si   =   DACL_SECURITY_INFORMATION;  
	__try 
	{  
		//   initialize   the   security   descriptor  
		if   (!InitializeSecurityDescriptor(pSD,    
			SECURITY_DESCRIPTOR_REVISION)) 
		{  
			printf("InitializeSecurityDescriptor()   failed   with   error   %d\n",  
				GetLastError());  
			__leave;  
		}  

		//   obtain   a   sid   for   the   Authenticated   Users   Group  
		if   (!AllocateAndInitializeSid(&siaWorld,   1,    
			SECURITY_WORLD_RID,   0,   0,   0,   0,   0,   0,   0,    
			&psidEveryone))  
		{  
			printf("AllocateAndInitializeSid()   failed   with   error   %d\n",  
				GetLastError());  
			__leave;  
		}  

		//   NOTE:  
		//    
		//   The   Authenticated   Users   group   includes   all   user   accounts   that  
		//   have   been   successfully   authenticated   by   the   system.   If   access  
		//   must   be   restricted   to   a   specific   user   or   group   other   than    
		//   Authenticated   Users,   the   SID   can   be   constructed   using   the  
		//   LookupAccountSid()   API   based   on   a   user   or   group   name.  

		//   calculate   the   DACL   length  
		dwAclLength   =   sizeof(ACL)  
			//   add   space   for   Authenticated   Users   group   ACE  
			+   sizeof(ACCESS_ALLOWED_ACE)   -   sizeof(DWORD)  
			+   GetLengthSid(psidEveryone);  

		//   allocate   memory   for   the   DACL  
		pDACL   =   (PACL)HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,    
			dwAclLength);  
		if   (!pDACL) 
		{  
			printf("HeapAlloc()   failed   with   error   %d\n",   GetLastError());  
			__leave;  
		}  

		//   initialize   the   DACL  
		if   (!InitializeAcl(pDACL,   dwAclLength,   ACL_REVISION)) 
		{  
			printf("InitializeAcl()   failed   with   error   %d\n",    
				GetLastError());  
			__leave;  
		}  

		//   add   the   Authenticated   Users   group   ACE   to   the   DACL   with  
		//   GENERIC_READ,   GENERIC_WRITE,   and   GENERIC_EXECUTE   access  
		if   (!AddAccessAllowedAce(pDACL,   ACL_REVISION,  
			GENERIC_ALL,  
			psidEveryone)) 
		{  
			printf("AddAccessAllowedAce()   failed   with   error   %d\n",  
				GetLastError());  
			__leave;  
		}  

		//   set   the   DACL   in   the   security   descriptor  
		if   (!SetSecurityDescriptorDacl(pSD,   TRUE,   pDACL,   FALSE)) 
		{  
			printf("SetSecurityDescriptorDacl()   failed   with   error   %d\n",  
				GetLastError());  
			__leave;  
		}  

		bResult   =   TRUE;  

	} 
	__finally
	{  

		if   (psidEveryone)   FreeSid(psidEveryone);  
	}  

	if   (bResult   ==   FALSE)  
	{  
		if   (pDACL)   HeapFree(GetProcessHeap(),   0,   pDACL);  
		pDACL   =   NULL;  
	}  

	return   (PVOID)   pDACL;  
}  

//   The   following   function   frees   memory   allocated   in   the  
//   BuildRestrictedSD()   function  
VOID   FreeRestrictedSD(PVOID   ptr)   {  

	if (ptr)
		HeapFree(GetProcessHeap(), 0, ptr);  
	return;  
}  


CShareRestrictedSD::CShareRestrictedSD()  
{  
	ptr = NULL;  
	sa.nLength = sizeof(sa);  
	sa.lpSecurityDescriptor = &sd;  
	sa.bInheritHandle = FALSE;  
	//   build   a   restricted   security   descriptor  
	ptr   =   BuildRestrictedSD(&sd);  
	if (!ptr) 
	{  
		TRACE("BuildRestrictedSD()   failed\n");  
	}  
}  

CShareRestrictedSD::~CShareRestrictedSD()  
{  
	if(ptr)
	{  
		FreeRestrictedSD(ptr);  
	}  
}  
SECURITY_ATTRIBUTES* CShareRestrictedSD::GetSA()  
{  
	if(ptr)
	{  
		return   &sa;  
	}  
	else  
		return   NULL;  
}

// CPipeSvrThread

static VOID WINAPI CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED); 
static VOID WINAPI CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED); 

IMPLEMENT_DYNCREATE(CPipeSvrThread, CWinThread)

CPipeSvrThread::CPipeSvrThread()
{
	m_hPipe = INVALID_HANDLE_VALUE;
}

CPipeSvrThread::~CPipeSvrThread()
{
#if 0	//在m_oPipeList的析构中释放
	if(INVALID_HANDLE_VALUE != m_hPipe && NULL != m_hPipe)	//add by zfq,2013.06.07
	{
		::CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}
#endif
}

CPipeSvrThread* CPipeSvrThread::Create(CString sName,IPipeCallback* pCall)
{
	CPipeSvrThread* m_pThread = (CPipeSvrThread*)AfxBeginThread(RUNTIME_CLASS(CPipeSvrThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pThread->m_sPipeName = sName;
	m_pThread->m_pCallback = pCall;
	m_pThread->ResumeThread();
	return m_pThread;
}

BOOL CPipeSvrThread::End(CPipeSvrThread*& pThread)
{
	ASSERT(pThread);
	CThreadMgr::DestroyThread(pThread);
	pThread = 0;
	return TRUE;
}

void CPipeSvrThread::OnNewPipe(void* pVoid)
{
	ASSERT(m_pCallback);
	if (m_pCallback)
	{
		m_pCallback->OnNewPipe(pVoid);
	}
	
}

BOOL CPipeSvrThread::InitInstance()
{
	if(m_sPipeName.IsEmpty())
	{
		theLog.Writeli(_T("CPipeSvrThread线程管道名称为空。"));
		return FALSE;
	}

	ASSERT(m_pCallback);
	m_oPipeList.m_pCallback = m_pCallback;

	//theLog.Writeli(_T("CPipeSvrThread线程【%s】启动。"),m_sPipeName);
	OVERLAPPED oConnect; 
	CAsyncPipeClt* pInst = NULL; 
	DWORD dwWait, cbBytes; 
	BOOL fSuccess, fPendingIO;
	HANDLE hEvtConn;

	// Create event objects
	hEvtConn = CreateEvent(NULL, TRUE, TRUE, NULL);

	//创建消息泵
	MSG tmsg;
	PeekMessage(&tmsg, NULL, 0, 0, PM_NOREMOVE);

	if (hEvtConn==NULL) 
	{
		theLog.Writele(_T("内存不足。EvtConn:0x%x"), hEvtConn);		
		return FALSE;
	}

	oConnect.hEvent = hEvtConn; 

	// Call a subroutine to create one instance, and wait for 
	// the client to connect. 
	fPendingIO = CreateAndConnectInstance(&oConnect); 

	HANDLE hEvt[1];
	hEvt[0]=hEvtConn;

	while (1) 
	{
		dwWait = MsgWaitForMultipleObjectsEx(1, hEvt, INFINITE, QS_ALLPOSTMESSAGE, MWMO_ALERTABLE);
		BOOL bSuc = FALSE;

		switch (dwWait) 
		{ 
			// The wait conditions are satisfied by a completed connect 
			// operation. 
		case 0: 
			// If an operation is pending, get the result of the 
			// connect operation. 
			if (fPendingIO) 
			{ 
				fSuccess = GetOverlappedResult( 
					m_hPipe,     // pipe handle 
					&oConnect, // OVERLAPPED structure 
					&cbBytes,  // bytes transferred 
					FALSE);    // does not wait 
				if (!fSuccess) 
				{
					theLog.Writele(_T("ConnectNamedPipe错误。err:%d"), GetLastError());
					::CloseHandle(hEvtConn);	//add by zfq,2013.06.07
					return FALSE;
				}
			} 

			// Allocate storage for this instance. 
			pInst = m_oPipeList.Alloc();
			if (pInst == NULL)
			{
				::CloseHandle(hEvtConn);	//add by zfq,2013.06.07
				return FALSE;
			}

			//theLog.Writeld(_T("CreateAndConnectInstance PIPE:0x%x"), m_hPipe);
			pInst->hPipeInst = m_hPipe; 
			pInst->cbToWrite = 0; 

			OnNewPipe(pInst);
			

			bSuc = pInst->PostRead();
			if(!bSuc)
				pInst->Release();

			// Create new pipe instance for the next client. 
			fPendingIO = CreateAndConnectInstance( 
				&oConnect); 

			break; 

		case 1:
			MSG msg;
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				//theLog.Writeld(_T("CPipeSvrThread线程处理消息。msg:%s"), CFMN(msg.message));
				switch(msg.message)
				{
				case WM_THREAD_EXIT:
					{
						theLog.Writeld(_T("CPipeSvrThread线程收到结束事件。"));
						m_oPipeList.ReleaseAll();
						PostQuitMessage(0);
						::CloseHandle(hEvtConn);	//add by zfq,2013.06.07
						return TRUE;
					}
					break;
				}
			}
			break;

			// The wait is satisfied by a completed read or write 
			// operation. This allows the system to execute the 
			// completion routine. 
		case WAIT_IO_COMPLETION:
			//theLog.Writeld(_T("WAIT_IO_COMPLETION事件等待完成。"));
			break; 

			// An error occurred in the wait function. 
		default: 
			{
				theLog.Writele(_T("WaitForSingleObjectEx错误。err:%d"), GetLastError());
				::CloseHandle(hEvtConn);	//add by zfq,2013.06.07
				return FALSE;
			}
		} 
	} 

	::CloseHandle(hEvtConn);	//add by zfq,2013.06.07
	return TRUE;
}

int CPipeSvrThread::ExitInstance()
{
	theLog.Writeli(_T("CPipeSvrThread线程结束。"));
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPipeSvrThread, CWinThread)
END_MESSAGE_MAP()

// ConnectToNewClient(HANDLE, LPOVERLAPPED) 
// This function is called to start an overlapped connect operation. 
// It returns TRUE if an operation is pending or FALSE if the 
// connection has been completed. 
BOOL CPipeSvrThread::ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo) 
{ 
	BOOL fConnected, fPendingIO = FALSE; 

	// Start an overlapped connection for this pipe instance. 
	fConnected = ConnectNamedPipe(hPipe, lpo); 

	// Overlapped ConnectNamedPipe should return zero. 
	if (fConnected) 
	{
		theLog.Writele(_T("ConnectNamedPipe失败。 err:%d"), GetLastError());
		return FALSE;
	}

	switch (GetLastError()) 
	{ 
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING: 
		fPendingIO = TRUE; 
		break; 

		// Client is already connected, so signal an event. 
	case ERROR_PIPE_CONNECTED: 
		if (SetEvent(lpo->hEvent)) 
			break; 

		// If an error occurs during the connect operation... 
	default: 
		theLog.Writele(_T("ConnectNamedPipe失败。 err:%d"), GetLastError());
		return FALSE;
	} 

	return fPendingIO; 
}

// CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as a completion routine after writing to 
// the pipe, or when a new client has connected to a pipe instance. It 
// starts another read operation. 
VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten, 
								  LPOVERLAPPED lpOverLap) 
{ 
	CAsyncPipeClt* lpPipeInst; 
	BOOL fRead = TRUE; 

	// lpOverlap points to storage for this instance. 
	lpPipeInst = (CAsyncPipeClt*) lpOverLap; 
	if ((dwErr == 0) && (cbWritten == lpPipeInst->cbToWrite)) 
	{
		theLog.Writeld(_T("应答发送成功。written:%d"), cbWritten);
		lpPipeInst->OnWriteCmpt();
		
	}
	else
	{
		theLog.Writelw(_T("应答发送失败。dwErr:%d written:%d"), dwErr, cbWritten);
		
	}
#if 0
 	if ((dwErr == 0) && (cbWritten == lpPipeInst->cbToWrite)) 
 		fRead = ReadFileEx( 
 			lpPipeInst->hPipeInst, 
 			lpPipeInst->pBuf, 
 			lpPipeInst->cbBufSize, 
 			(LPOVERLAPPED) lpPipeInst, 
			(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine); 
#endif
	// Disconnect if an error occurred. 
	if (!fRead)
	{
		if(GetLastError() != ERROR_BROKEN_PIPE)
			theLog.Writelw(_T("CompletedWriteRoutine中ReadFileEx失败。err:%d"), GetLastError());
		lpPipeInst->Release();
	}
} 

// CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as an I/O completion routine after reading a 
// request from the client. It gets data and writes it to the pipe. 
VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbBytesRead, 
								 LPOVERLAPPED lpOverLap) 
{ 
	CAsyncPipeClt* lpPipeInst = NULL; 
	BOOL fSuc = FALSE; 

	// lpOverlap points to storage for this instance. 
	lpPipeInst = (CAsyncPipeClt*) lpOverLap; 

	// The read operation has finished, so write a response (if no 
	// error occurred). 
	if (dwErr == 0 && cbBytesRead != 0) 
	{
		lpPipeInst->cbRead += cbBytesRead;
		DWORD dwBs;
		BOOL bRet = GetOverlappedResult(lpPipeInst->hPipeInst, &lpPipeInst->oOverlap, &dwBs, FALSE);
		
		// test report
// 		char pMsg[1024];
// 		sprintf_s(pMsg,"CompletedReadRoutine cbBytesRead = %d bRet = %d lpPipeInst->cbRequestBufSize %d \n",cbBytesRead,bRet,lpPipeInst->cbRequestBufSize);
// 		OutputDebugStringA(pMsg);

		do 
		{
			// if error and GetLastError = ERROR_MORE_DATA,new a big memory for it
			if (!bRet)
			{
				DWORD dwLErr = GetLastError();		
				if (dwLErr == ERROR_MORE_DATA)
				{
					// build a big memory
					if(!lpPipeInst->GrowRequestBuf())
					{
						ASSERT(0);
						lpPipeInst->Release();
						return;
					}

					fSuc = ReadFileEx(
						lpPipeInst->hPipeInst, 
						lpPipeInst->pRequestBuf+lpPipeInst->cbRead, 
						lpPipeInst->cbRequestBufSize - lpPipeInst->cbRead, 
						(LPOVERLAPPED)lpPipeInst, 
						(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine);
					break;
				}
				else
					return;
			}

			// 提交
			UINT nSize = 0;
			if(lpPipeInst->cbRead >= sizeof(nSize))
			{
				IST_PIPE_STRUCT* pAk = (IST_PIPE_STRUCT*)lpPipeInst->pRequestBuf;
				if((pAk->nPackBodySize + sizeof(IST_PIPE_STRUCT)) != lpPipeInst->cbRead)
				{
					theLog.Writelw(_T("读取消息出错。msg:%s, size:%d, read:%d"), CFMN(pAk->message), pAk->nPackBodySize, lpPipeInst->cbRead);
				}
				else
				{
					//theLog.Writeld(_T("接收消息成功。msg:%s, size:%d, read:%d PIPE:0x%x"), 
					//	CFMN(pAk->message), pAk->nPackBodySize, lpPipeInst->cbRead, lpPipeInst->hPipeInst);
					//读取完成
					if(lpPipeInst->OnReadCmpt(pAk))
					{
						//继续读取
						fSuc = lpPipeInst->PostRead();
					}
				}
			}
			else
				theLog.Writelw(_T("接收数据不正确。size:%d"), lpPipeInst->cbRead);
		} while (0);
	} 

	// Disconnect if an error occurred. 
	if(!fSuc) 
	{
		if(dwErr != ERROR_BROKEN_PIPE)
			theLog.Writelw(_T("CompletedReadRoutine失败，链接中断。dwErr:%d"), dwErr);
		lpPipeInst->Release();
	}
} 

// CreateAndConnectInstance(LPOVERLAPPED) 
// This function creates a pipe instance and connects to the client. 
// It returns TRUE if the connect operation is pending, and FALSE if 
// the connection has been completed. 
BOOL CPipeSvrThread::CreateAndConnectInstance(LPOVERLAPPED lpoOverlap) 
{ 
	CShareRestrictedSD ShareRestrictedSD;
	m_hPipe = CreateNamedPipe( 
		m_sPipeName,             // pipe name 
		PIPE_ACCESS_DUPLEX |      // read/write access 
		FILE_FLAG_OVERLAPPED,     // overlapped mode 
		PIPE_TYPE_MESSAGE |       // message-type pipe 
		PIPE_READMODE_MESSAGE |   // message read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // unlimited instances 
		DEF_BUFSIZE,                  // output buffer size 
		DEF_BUFSIZE,                  // input buffer size 
		3000,             // client time-out 
		ShareRestrictedSD.GetSA());                    // no security attributes
	if (m_hPipe == INVALID_HANDLE_VALUE) 
	{
		theLog.Writele(_T("CreateNamedPipe失败。code:%d"), GetLastError());
		return FALSE;
	}

	// Call a subroutine to connect to the new client. 
	return ConnectToNewClient(m_hPipe, lpoOverlap); 
} 
BOOL CPipeSvrThread::CheckPipeInstance(CAsyncPipeClt* p)
{
	return m_oPipeList.Check(p);
}
//----------------------------------------
CAsyncPipeClt::CAsyncPipeClt()
{
	ZeroMemory(this,sizeof(PIPEINST));
}
CAsyncPipeClt::CAsyncPipeClt(CAsyncPipeCltList* p)
{
	ZeroMemory(this,sizeof(PIPEINST));
	m_pOwner = p;
}

CAsyncPipeClt::~CAsyncPipeClt()
{
	Close();
	if(pReplyBuf)
	{
		delete pReplyBuf;
	}
	if(pRequestBuf)
	{
		delete pRequestBuf;
	}
}
void CAsyncPipeClt::Release()
{
	if(m_pOwner)
	{
		m_pOwner->Release(this);
	}
	else
	{
		ASSERT(0);
	}
}
void CAsyncPipeClt::Close()
{
	if(hPipeInst)
	{
		if (! DisconnectNamedPipe(hPipeInst) ) 
		{
			theLog.Writele(_T("DisconnectNamedPipe失败。 err:%d"), GetLastError());
			return;
		}
		CloseHandle(hPipeInst); 
		hPipeInst = NULL;
	}
}
void CAsyncPipeClt::Init(int nBufDefSize,IPipeCallback* pCall)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(nBufDefSize);
	ASSERT(pCall);
	pCallback = pCall;
	pRequestBuf = new BYTE[nBufDefSize];
	pReplyBuf = new BYTE[nBufDefSize];
	ASSERT(pReplyBuf && pRequestBuf);
	cbRequestBufSize = nBufDefSize;
	cbReplyBufSize = nBufDefSize;
	ZeroMemory(pReplyBuf,cbReplyBufSize);
	ZeroMemory(pRequestBuf,cbRequestBufSize);
}

BOOL CAsyncPipeClt::Write(int nCmd,int nSub,BYTE* pData /*= 0*/,int nSize /*= 0*/)
{
	int nAllLen = sizeof(IST_PIPE_STRUCT) + nSize;
	IST_PIPE_STRUCT* pInfo = (IST_PIPE_STRUCT*)new BYTE[nAllLen];
	pInfo->nPackBodySize = nSize;
	pInfo->message = nCmd;
	pInfo->para1 = nSub;
	if(pData && nSize > 0 )
		CopyMemory((BYTE*)(pInfo + 1),pData,nSize);
	BOOL b = Write((BYTE*)pInfo,nAllLen);
	delete pInfo;
	return b;
}

BOOL CAsyncPipeClt::Write(BYTE* pData,int nSize)
{
	CCriticalSection2::Owner ow(m_cs);
	if(cbToWrite > 0)
	{
		ASSERT(0);
		theLog.Write(_T("cbToWrite %d"),cbToWrite);
		return 0;
	}
	if(cbReplyBufSize < nSize)
	{
		if(pReplyBuf)
		{
			delete pReplyBuf;
		}
		pReplyBuf = new BYTE[nSize];
		cbReplyBufSize = nSize;
	}
	CopyMemory(pReplyBuf,pData,nSize);

	BOOL fSuc = WriteFileEx( 
		hPipeInst, 
		pReplyBuf, 
		nSize,
		(LPOVERLAPPED)this, 
		(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedWriteRoutine);
	return fSuc;
}

int CAsyncPipeClt::OnWriteCmpt()
{
	CCriticalSection2::Owner ow(m_cs);
	return 0;
}
int CAsyncPipeClt::OnReadCmpt(BYTE* pData,int nSize)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(pCallback);
	if (pCallback)
	{
		pCallback->OnPipeRcv(this,pData,nSize);	
	}
	cbRead = 0;
	return 1;
}
int CAsyncPipeClt::OnReadCmpt(IST_PIPE_STRUCT* pInfo)
{
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(pCallback);
	if (pCallback)
	{
		pCallback->OnPipeRcv(this,(BYTE*)pInfo,pInfo->nPackBodySize + sizeof(IST_PIPE_STRUCT));	
	}
	cbRead = 0;
	return 1;
}

BOOL CAsyncPipeClt::PostRead()
{
	CCriticalSection2::Owner ow(m_cs);
	if(cbRead > 0)
	{
		ASSERT(0);
		return 0;
	}
	BOOL b =  ReadFileEx( 
		hPipeInst, 
		pRequestBuf, 
		cbRequestBufSize, 
		(LPOVERLAPPED)this, 
		(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine); 

	return b;
}
BOOL CAsyncPipeClt::GrowRequestBuf()
{
	CCriticalSection2::Owner ow(m_cs);
	BYTE* pNew = new BYTE[cbRequestBufSize*2];
	if(!pNew)
	{
		theLog.Writele(_T("内存不足。size:%d, err:%d"), cbRequestBufSize*2, GetLastError());
		return FALSE;
	}
	ZeroMemory(pNew,cbRequestBufSize*2);
	memcpy(pNew, pRequestBuf, cbRead);
	cbRequestBufSize = cbRequestBufSize*2;
	delete pRequestBuf;
	pRequestBuf = pNew;
	return TRUE;
}
//------------------------------
CAsyncPipeCltList::CAsyncPipeCltList()
{

}
CAsyncPipeCltList::~CAsyncPipeCltList()
{

}
CAsyncPipeClt* CAsyncPipeCltList::Alloc()
{
	CCriticalSection2::Owner ow(m_cs);
	CAsyncPipeClt* lpPipeInst = new CAsyncPipeClt(this);
	if(!lpPipeInst)
	{
		theLog.Writele(_T("内存不足。AllocPI失败。 err:%d"), GetLastError());
		return NULL;
	}
	lpPipeInst->Init(DEF_BUFSIZE,m_pCallback);
	AddTail(lpPipeInst);
	return lpPipeInst;
}
BOOL CAsyncPipeCltList::Release(CAsyncPipeClt* p)
{
	CCriticalSection2::Owner ow(m_cs);
	POSITION pos = this->Find(p);
	if(pos)
	{
		m_pCallback->OnPipeClose(p);
		this->RemoveAt(pos);
		delete p;
	}
	return TRUE;
}
BOOL CAsyncPipeCltList::ReleaseAll()
{
	POSITION old = 0;
	POSITION pos = GetHeadPosition();
	while(pos)
	{
		old = pos;
		CAsyncPipeClt* p = GetNext(pos);
		if(p)
		{
			delete p;
		}
		RemoveAt(old);
	}
	return TRUE;
}
BOOL CAsyncPipeCltList::Check(CAsyncPipeClt* p)
{
	CCriticalSection2::Owner ow(m_cs);
	POSITION pos = this->Find(p);
	if(pos)
	{
		return TRUE;
	}
	return FALSE;
}
//------------------------------
CPipeClt::CPipeClt()
{
	m_hPipe = 0;
	m_hEvent = 0;
	m_hShutDown = 0;
	m_hThread = 0;
	m_pRequestBuf = 0;
	m_nRequestBufSize = 0;
	m_nRead = 0;
	m_pRequestBuf = new BYTE[DEF_BUFSIZE];
	m_nRequestBufSize = DEF_BUFSIZE;
}
CPipeClt::CPipeClt(IPipeCallback* pCall)
{
	m_hPipe = 0;
	m_hEvent = 0;
	m_hShutDown = 0;
	m_hThread = 0;
	m_pRequestBuf = 0;
	m_nRequestBufSize = 0;
	m_nRead = 0;
	m_pCallBack = pCall;
	m_pRequestBuf = new BYTE[DEF_BUFSIZE];
	m_nRequestBufSize = DEF_BUFSIZE;
}
CPipeClt::~CPipeClt()
{
	Shutdown();
	if(m_pRequestBuf)
	{
		delete m_pRequestBuf;
		m_pRequestBuf = NULL;
	}
}

void CPipeClt::Shutdown()
{
	if(m_hShutDown)
	{
		SetEvent(m_hShutDown);
	}
	if(m_hPipe)
	{
		CancelIo(m_hPipe);
		CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}
	if(m_hThread)
	{
		DWORD dw = WaitForSingleObject(m_hThread,INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
		ASSERT(0);
	}
}
BOOL CPipeClt::Create(CString sName)
{
	m_sPipename = sName;
	m_hPipe = CreateNamedPipe( 
		m_sPipename,              // pipe name 
		PIPE_ACCESS_DUPLEX |      // read/write access 
		FILE_FLAG_OVERLAPPED,     // overlapped mode 
		PIPE_TYPE_MESSAGE |       // message-type pipe 
		PIPE_READMODE_MESSAGE |   // message read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // unlimited instances 
		DEF_BUFSIZE,              // output buffer size 
		DEF_BUFSIZE,              // input buffer size
		3000,                     // client time-out
		NULL);                    // no security attributes
	if (m_hPipe == INVALID_HANDLE_VALUE) 
	{
		theLog.Writele(_T("CreateNamedPipe失败。code:%d"), GetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CPipeClt::Open(CString sName)
{
	m_sPipename = sName;

	while(1)
	{
		m_hPipe = CreateFile( 
			m_sPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE, 
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0/*FILE_FLAG_OVERLAPPED*/,              // default attributes /**/
			NULL);          // no template file 

		// Break if the pipe handle is valid. 
		if (m_hPipe != INVALID_HANDLE_VALUE) 
			break; 

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 
		DWORD dwErr = GetLastError();
		if (dwErr != ERROR_PIPE_BUSY) 
		{
			theLog.Write("Could not open pipe Err %d",dwErr); 
			::CloseHandle(m_hPipe);	//add by zfq,2013.06.07
			m_hPipe = INVALID_HANDLE_VALUE;	//add by zfq,2013.06.07
			return 0;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(m_sPipename, 20000)) 
		{ 
			theLog.Write("Could not open pipe"); 
			::CloseHandle(m_hPipe);	//add by zfq,2013.06.07
			m_hPipe = INVALID_HANDLE_VALUE;
			return 0;
		} 
	}
	
	DWORD dwMode = PIPE_READMODE_MESSAGE; 
	BOOL fSuccess = SetNamedPipeHandleState( 
		m_hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess) 
	{
		theLog.Write("SetNamedPipeHandleState failed"); 
		::CloseHandle(m_hPipe);	//add by zfq,2013.06.07
		m_hPipe = INVALID_HANDLE_VALUE;	//add by zfq,2013.06.07
		return 0;
	}
	return TRUE;
}
BOOL CPipeClt::Write(IST_PIPE_STRUCT* pInfo)
{
	if (pInfo)
	{
		return Write((BYTE*)pInfo,pInfo->nPackBodySize + sizeof(IST_PIPE_STRUCT));
	}
	return FALSE;
}
BOOL CPipeClt::Write(int nCmd,int nSub,BYTE* pData /*= 0*/,int nSize /*= 0*/)
{
	int nAllLen = sizeof(IST_PIPE_STRUCT) + nSize;
	IST_PIPE_STRUCT* pInfo = (IST_PIPE_STRUCT*)new BYTE[nAllLen];
	pInfo->nPackBodySize = nSize;
	pInfo->message = nCmd;
	pInfo->para1 = nSub;
	if(pData && nSize > 0 )
		CopyMemory((BYTE*)(pInfo + 1),pData,nSize);
	BOOL b = Write((BYTE*)pInfo,nAllLen);
	delete pInfo;
	return b;
}
BOOL CPipeClt::Write(BYTE* pData,int nSize)
{
	CCriticalSection2::Owner ow(m_cs);
	//ASSERT(m_hPipe);
	if(!m_hPipe || m_hPipe == INVALID_HANDLE_VALUE)
		return 0;
	DWORD cbWritten = 0; 
	BOOL fSuccess = WriteFile( 
		m_hPipe,                  // pipe handle 
		pData,             // message 
		nSize, // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 
	if (!fSuccess) 
	{
		theLog.Write("WriteFile failed Err = %d",::GetLastError()); 
		return 0;
	}
	return fSuccess;
}

//add by zfq,2013.05.13,begin,为了兼容已经发出去的货，不加在头文件中
typedef struct tag_ST_MsgQueOK_OfPipeSvrThread
{
	void	*pThis;			//指向拥有者的类
	HANDLE	*phEvMsgQueOK;	//指向等待的事件
} ST_MsgQueOK_OfPipeSvrThread, *PST_MsgQueOK_OfPipeSvrThread;
#define DEF_UseMsgQueOkForCreateThread 1	//创建线程后是否使用消息等待的宏
//add by zfq,2013.05.13,end

#ifdef DEF_UseMsgQueOkForCreateThread	//add by zfq,2013.05.13,确保线程创建并得到执行后再返回
BOOL CPipeClt::Init()
{
	HANDLE hEvMsgQueOK = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if(!hEvMsgQueOK || INVALID_HANDLE_VALUE == hEvMsgQueOK)
	{
		theLog.Write("!!CPipeClt::Init,1,CreateEvent fail,err=%d,hEvMsgQueOK=0x%x", GetLastError(), hEvMsgQueOK);
		return FALSE;
	}
	ST_MsgQueOK_OfPipeSvrThread stMsgQueOK;
	memset(&stMsgQueOK, 0x0, sizeof(stMsgQueOK));
	stMsgQueOK.pThis = this;
	stMsgQueOK.phEvMsgQueOK = &hEvMsgQueOK;


	DWORD dw = 0;
	m_hShutDown = CreateEvent( 
		NULL,    // default security attribute 
		FALSE,    // manual-reset event 
		FALSE,    // initial state = signaled 
		NULL);
	if(!m_hShutDown || INVALID_HANDLE_VALUE == m_hShutDown)
	{
		theLog.Write("!!CPipeClt::Init,2,CreateEvent fail,err=%d,m_hShutDown=0x%x", GetLastError(), m_hShutDown);
		goto ERR1;
	}

#if 0
	m_hEvent = CreateEvent( 
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 


#endif

//	m_hThread = CreateThread(0,0,ThreadFunc,this,0,&dw);
	m_hThread = CreateThread(0,0,ThreadFunc, &stMsgQueOK, 0, &dw);
	if(!m_hThread)
	{
		theLog.Write("!!CPipeClt::Init,3,CreateThread fail,err=%d,m_hThread=0x%x", GetLastError(), m_hThread);
		goto ERR2;
	}

	::WaitForSingleObject(hEvMsgQueOK,INFINITE);	//等待线程创建并执行
	::CloseHandle(hEvMsgQueOK);

	return m_hThread && m_hShutDown;
ERR2:
	::CloseHandle(m_hShutDown);
	m_hShutDown = NULL;
ERR1:
	::CloseHandle(hEvMsgQueOK);
	return FALSE;
}
#else //del by zfq,2013.05.13
BOOL CPipeClt::Init()
{
	m_hShutDown = CreateEvent( 
		NULL,    // default security attribute 
		FALSE,    // manual-reset event 
		FALSE,    // initial state = signaled 
		NULL);
#if 0
	m_hEvent = CreateEvent( 
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 

	
#endif
	
	DWORD dw = 0;
	m_hThread = CreateThread(0,0,ThreadFunc,this,0,&dw);

	return m_hThread && m_hShutDown;
}
#endif

void CPipeClt::OnReadCmplt(BYTE* pData,int nSize)
{
	ASSERT(m_pCallBack);
	if(m_pCallBack)
	{
		m_pCallBack->OnPipeRcv(this,pData,nSize);
	}
	m_nRead = 0;
}

BOOL CPipeClt::OnProcess()
{
	BOOL fSuccess = FALSE;
	char chBuf[1024];

	do 
	{ 
		// Read from the pipe. 
		DWORD cbRead = 0;
		fSuccess = ReadFile( 
			m_hPipe,    // pipe handle 
			chBuf,    // buffer to receive reply 
			1024,  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 

		if (! fSuccess && GetLastError() != ERROR_MORE_DATA) 
			break; 
		
		CopyMemory(m_pRequestBuf + m_nRead,chBuf,cbRead);
		m_nRead += cbRead;

		TRACE( TEXT("%s\n"), chBuf ); 
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

	if(fSuccess)
	{
		OnReadCmplt(m_pRequestBuf,m_nRead);
	}

	return fSuccess;
}
BOOL CPipeClt::HasData()
{
	DWORD dw = 0;
	DWORD dw2 = 0;
	if(PeekNamedPipe(m_hPipe,0,0,0,&dw,&dw2))
	{
		if(dw > 0 || dw2 > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}
DWORD WINAPI CPipeClt::ThreadFunc(void* lp)
{
#ifdef DEF_UseMsgQueOkForCreateThread	//add by zfq,2013.05.13,确保线程起来
	ST_MsgQueOK_OfPipeSvrThread *pMsgQueOK = (ST_MsgQueOK_OfPipeSvrThread *)lp;
	HANDLE *phEvMsgQueOK = pMsgQueOK->phEvMsgQueOK;
	CPipeClt*	pThis= (CPipeClt*)(pMsgQueOK->pThis);
	if(!pThis || !phEvMsgQueOK)
	{
		theLog.Write("!!CPipeClt::ThreadFunc,1,pThis=0x%x,phEvMsgQueOK=0x%x", pThis, phEvMsgQueOK);
		return 0;
	}
	::SetEvent(*phEvMsgQueOK);
#else	//del by zfq,2013.05.13
	CPipeClt* pThis = (CPipeClt*)lp;
#endif
	
#if 0
	HANDLE ary[2] = {pThis->m_hEvent,pThis->m_hShutDown};
	while (1)
	{
		DWORD dwWait = ::WaitForMultipleObjects(2,ary,FALSE,INFINITE);
		if (dwWait == WAIT_OBJECT_0)
		{
			if(!pThis->OnProcess())
			{
				ASSERT(0);
				break;
			}
		}
		else
		{
			ASSERT(0);
			break;
		}
		
	}
#else
	HANDLE ary[1] = {pThis->m_hShutDown};
	while (1)
	{
		DWORD dwWait = ::WaitForSingleObject(pThis->m_hShutDown,10);
		if (dwWait == WAIT_OBJECT_0)
		{
			break;
		}
		else
		{
			if(pThis->HasData())
			{
				if(!pThis->OnProcess())
				{
					ASSERT(0);
					break;
				}
			}
		}
	}
#endif
	return 0;
}