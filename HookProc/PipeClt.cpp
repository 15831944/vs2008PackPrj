#include "stdafx.h"
#include "pipeclt.h"

#define DEF_BUFSIZE 4096

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
	InitializeCriticalSection(&m_cs);
}

CPipeClt::~CPipeClt()
{
	Shutdown();
	if(m_pRequestBuf)
	{
		delete m_pRequestBuf;
		m_pRequestBuf = NULL;
	}
	DeleteCriticalSection(&m_cs);
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
		m_sPipename,             // pipe name 
		PIPE_ACCESS_DUPLEX |      // read/write access 
		FILE_FLAG_OVERLAPPED,     // overlapped mode 
		PIPE_TYPE_MESSAGE |       // message-type pipe 
		PIPE_READMODE_MESSAGE |   // message read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // unlimited instances 
		DEF_BUFSIZE,                  // output buffer size 
		DEF_BUFSIZE,                  // input buffer size 
		3000,             // client time-out 
		NULL);                    // no security attributes
	if (m_hPipe == INVALID_HANDLE_VALUE) 
	{
		WriteFileLog(_T("CreateNamedPipeÊ§°Ü¡£code:%d"), GetLastError());
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

		if (GetLastError() != ERROR_PIPE_BUSY) 
		{
			TRACE("Could not open pipe"); 
			return 0;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(m_sPipename, 20000)) 
		{ 
			printf("Could not open pipe"); 
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
		printf("SetNamedPipeHandleState failed"); 
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
	CLockCS ow(m_cs);
	ASSERT(m_hPipe);
	if(!m_hPipe)
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
		TRACE("WriteFile failed Err = %d",::GetLastError()); 
		return 0;
	}
	return fSuccess;
}

BOOL CPipeClt::Init()
{
	m_hShutDown = CreateEvent( 
		NULL,    // default security attribute 
		FALSE,    // manual-reset event 
		FALSE,    // initial state = signaled 
		NULL);

	DWORD dw = 0;
	m_hThread = CreateThread(0,0,ThreadFunc,this,0,&dw);

	return m_hThread && m_hShutDown;
}

void CPipeClt::OnReadCmplt(BYTE* pData,int nSize)
{
	
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
	CPipeClt* pThis = (CPipeClt*)lp;


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