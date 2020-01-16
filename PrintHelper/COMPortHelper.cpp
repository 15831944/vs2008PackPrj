#include "stdafx.h"
#include "COMPortHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCOMPortHelper::CCOMPortHelper()
{
	m_dwLastError = NO_ERROR;
	m_hThreadTerm = NULL;
	m_hThread = NULL;
	m_dwThreadID = 0;
	m_hThreadStarted = NULL;
	m_hComPort = NULL;
	m_hComEvent = NULL;
	m_pCallback = NULL;
}

CCOMPortHelper::~CCOMPortHelper()
{
	Close();
}

BOOL CCOMPortHelper::Open(int nComIndex)
{
  {//限制锁的作用域

	CCriticalSection2::Owner ow(m_cs);

	if (m_hComPort && (m_hComPort != INVALID_HANDLE_VALUE))
	{
		theLog.Write("##CCOMPortHelper::Open,1,m_hComPort=%d,nComIndex=%d,has opened.", m_hComPort, nComIndex);
		return TRUE;
	}

	COMMTIMEOUTS timeouts;
	DCB dcb = {0};
	char szBuf[MAX_PATH] = {0};
	DCB dc = {0};
	DWORD dwWait = 0;

	if (nComIndex <= 0)
	{
		theLog.Write("!!CCOMPortHelper::Open,2,nComIndex=%d", nComIndex);
		return FALSE;
	}

	char szCom[20] = {0};
	sprintf(szCom, "\\\\?\\COM%d", nComIndex);
	theLog.Write("CCOMPortHelper::Open,3,szCom=%s,nComIndex=%d", szCom, nComIndex);

	try
	{
		//open the COM Port
		m_hComPort = ::CreateFile(szCom,
			GENERIC_READ|GENERIC_WRITE,//access ( read and write)
			0,	//(share) 0:cannot share the COM port						
			0,	//security  (None)
			OPEN_EXISTING,// creation : open_existing
			FILE_FLAG_OVERLAPPED,// we want overlapped operation
			0// no templates file for COM port...
			);
		if (m_hComPort == INVALID_HANDLE_VALUE)
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Open,4,CreateFile fail,m_dwLastError=%d,", m_dwLastError);
			goto ERR;
		}

		/*
		COMMPROP pro = {0};
		pro.wPacketLength = sizeof(pro);
		PST_FAX;
		GetCommProperties(m_hComPort,&pro);
		WriteLogEx("type = %d",pro.dwProvSubType);
		pro.dwProvSubType = PST_RS232;
		//SetCommConfig(m_hComPort,(LPCOMMCONFIG )&pro,sizeof(pro));
		*/

		timeouts.ReadIntervalTimeout			= 1000; 
		timeouts.ReadTotalTimeoutMultiplier		= 1000;
		timeouts.ReadTotalTimeoutConstant		= 1000;
		timeouts.WriteTotalTimeoutMultiplier	= 1000;
		timeouts.WriteTotalTimeoutConstant		= 1000;

		if (!SetCommTimeouts(m_hComPort, &timeouts))
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Open,5,SetCommTimeouts fail,m_dwLastError=%d", m_dwLastError);
			goto ERR;
		}

		//modify by zxl,2013.03.22,测试,先删除 [EV_EVENT2 | EV_EVENT1]两个事件,
		//now start to read but first we need to set the COM port settings and the timeouts
		if (!SetCommMask(m_hComPort,EV_RXCHAR /*|  EV_EVENT2 | EV_EVENT1*/ /*| EV_RING | EV_PERR | EV_BREAK | EV_CTS*/) )
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Open,6,SetCommMask fail,m_dwLastError=%d", m_dwLastError);
			goto ERR;
		}

		//------------------
		//now we need to set baud rate etc,
		dcb.DCBlength = sizeof(DCB);
		if (!::GetCommState (m_hComPort,&dcb))
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CSerialCommHelper::Open,5,SetCommMask fail,m_dwLastError=%d,nComIndex=%d", m_dwLastError, nComIndex);
			goto ERR;
		}
		sprintf(szBuf,"baud=%d parity=N data=%d stop=%d",9600,8,1);
		//	*(DWORD *)((BYTE*)&dcb + 8) = LOBYTE(v15) & 0xDF | 0x10;

		if(!BuildCommDCB(szBuf,&dcb))
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CSerialCommHelper::Open,6,BuildCommDCB fail,m_dwLastError=%d,nComIndex=%d", m_dwLastError, nComIndex);
			goto ERR;
		}

		dcb.BaudRate	= 9600;
		dcb.ByteSize	= 8;
		dcb.Parity		= 0;
		dcb.StopBits	= ONESTOPBIT;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fOutxDsrFlow = 0;
		dcb.EvtChar = 0x71;
		dcb.fDsrSensitivity = 0;

		if (!::SetCommState (m_hComPort,&dcb))
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Open,7,SetCommState fail,m_dwLastError=%d,nComIndex=%d", m_dwLastError, nComIndex);
			goto ERR;
		}
		theLog.Write("CCOMPortHelper::Open,Current Settings, (Baud Rate %d; Parity %d; Byte Size %d; Stop Bits %d", dcb.BaudRate, 
			dcb.Parity,dcb.ByteSize,dcb.StopBits);

		if (!PurgeComm(m_hComPort,0xf))
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::open,8,PurgeComm fail,m_dwLastError=%d,nComIndex=%d", m_dwLastError, nComIndex);
			goto ERR;
		}

		m_hComEvent = CreateEvent(0,TRUE,0,0);

		//create thread terminator event...
		m_hThreadTerm = CreateEvent(0,0,0,0);
		m_hThreadStarted = CreateEvent(0,0,0,0);
		if (!m_hComEvent || !m_hThreadTerm || !m_hThreadStarted)
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Open,12,CreateEvent fail,m_dwLastError=%d,m_hComEvent=%d,m_hThreadTerm=%d,m_hThreadStarted=%d"
				, m_dwLastError, m_hComEvent, m_hThreadTerm, m_hThreadStarted);
			goto ERR;
		}

		m_hThread = CreateThread(NULL, 0, CCOMPortHelper::ThreadFn, (LPVOID)this, 0, &m_dwThreadID);
		if (!m_hThread)
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Open,13,CreateThread fail,m_dwLastError=%d", m_dwLastError);
			goto ERR;
		}

		theLog.Write("CCOMPortHelper::Open,14,CreateThread succ,m_hThread=%d", m_hThread);

		dwWait = WaitForSingleObject(m_hThreadStarted, INFINITE);
		if (dwWait != WAIT_OBJECT_0)
		{
			theLog.Write("!!CCOMPortHelper::Open,15,WaitForSingleObject fail,dwWait=%d, m_hThreadStarted=%d", dwWait, m_hThreadStarted);
		}
		CloseHandle(m_hThreadStarted);
		m_hThreadStarted = NULL;
	}
	catch(...)
	{
		m_dwLastError = GetLastError();
		theLog.Write("!!CCOMPortHelper::Open,100,catch sth,m_dwLastError=%d", m_dwLastError);
		goto ERR;
	}

  }//限制锁的作用域

	return TRUE;

ERR:
	if (m_hComPort)
	{
		if (m_hComPort != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hComPort);
		}
		m_hComPort = NULL;
	}
	if (m_hComEvent)
	{
		CloseHandle(m_hComEvent);
		m_hComEvent = NULL;
	}
	if (m_hThreadTerm)
	{
		CloseHandle(m_hThreadTerm);
		m_hThreadTerm = NULL;
	}
	if (m_hThreadStarted)
	{
		CloseHandle(m_hThreadStarted);
		m_hThreadStarted = NULL;
	}

	OnErr(m_dwLastError);

	return FALSE;
}

void CCOMPortHelper::Close()
{
	BOOL BErr = FALSE;

	{//限制锁的作用域
		CCriticalSection2::Owner ow(m_cs);

		try
		{
			if(m_hThreadTerm)
			{
				SignalObjectAndWait(m_hThreadTerm, m_hThread, INFINITE, FALSE);
				CloseHandle(m_hThreadTerm);
				m_hThreadTerm = NULL;
			}
			if(m_hThread)
			{
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
			if(m_hComPort)
			{
				if (m_hComPort!=INVALID_HANDLE_VALUE)
				{
					CloseHandle(m_hComPort);
				}
				m_hComPort = NULL;
			}
			if (m_hThreadStarted)
			{
				CloseHandle(m_hThreadStarted);
				m_hThreadStarted = NULL;
			}
			if(m_hComEvent)
			{
				CloseHandle(m_hComEvent);
				m_hComEvent = NULL;
			}
		}
		catch(...)
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::Close,100,catch sth,m_dwLastError=%d", m_dwLastError);
//			OnErr(m_dwLastError);	//del by zfq,2017-04-26,移动到锁的作用域外
			BErr = TRUE;
		}

	}//限制锁的作用域

	if(BErr)	//add by zfq,2017-04-26,移动到锁的作用域外
	{
		OnErr(m_dwLastError);
	}

	m_pCallback = NULL;
}

void CCOMPortHelper::SetCallBack(ICOMPortInterface* pCall)
{
	CCriticalSection2::Owner ow(m_cs);
	m_pCallback = pCall;
}

void CCOMPortHelper::OnByte(BYTE byte)
{
	if (m_pCallback)
	{
		m_pCallback->OnByte(byte);
	}
}

void CCOMPortHelper::OnErr(DWORD dwError)
{
	if (m_pCallback)
	{
		m_pCallback->OnErr(dwError);
	}
}

void CCOMPortHelper::OnRead()
{
	//add by zfq,2017-04-26,begin
	vector<BYTE> byvReadContent;	//存放读取到数据的容器，发送错误时，也加一个到里面
	BOOL BErr = FALSE;
	int nErrIdx = 0;	//发送错误的序号,从0开始
	//add by zfq,2017-04-26,end

	{//限制锁的作用域
		CCriticalSection2::Owner ow(m_cs);

		try 
		{
			DWORD dwErr = 0;
			COMSTAT Stat = {0};
			BOOL abRet = false;
			DWORD dwBytesRead = 0;
			ClearCommError(m_hComPort, &dwErr, &Stat);

			while(Stat.cbInQue)
			{
				ResetEvent(m_hComEvent);
				OVERLAPPED read_ol = {0};
				read_ol.hEvent = m_hComEvent;

				BYTE byte = 0;
				abRet = ReadFile(m_hComPort, &byte, sizeof(BYTE), &dwBytesRead, &read_ol);
				if (!abRet) 
				{
					m_dwLastError = GetLastError();
					if (m_dwLastError != 997)
					{
						theLog.Write("!!CCOMPortHelper::OnRead,1,m_dwLastError=%d", m_dwLastError);
//						OnErr(m_dwLastError);	//移动到锁的作用域外
						BErr = TRUE;		//add by zfq,2017-04-26,begin
						nErrIdx = byvReadContent.size();
						byte = 0;
						byvReadContent.push_back(byte);	//add by zfq,2017-04-26,end
						break;
					}
					if (!GetOverlappedResult(m_hComPort, &read_ol, &dwBytesRead, 1))
					{
						m_dwLastError = GetLastError();
						theLog.Write("!!CCOMPortHelper::OnRead,2,m_dwLastError=%d", m_dwLastError);
//						OnErr(m_dwLastError);	//移动到锁的作用域外
						BErr = TRUE;	//add by zfq,2017-04-26,begin
						nErrIdx = byvReadContent.size();
						byte = 0;
						byvReadContent.push_back(byte);	//add by zfq,2017-04-26,end
						break;
					}
				}
				else
				{
					SetEvent(read_ol.hEvent);
				}

				ClearCommError(m_hComPort, &dwErr, &Stat);
//				OnByte(byte);	//移动到锁的作用域外
				byvReadContent.push_back(byte);	//add by zfq,2017-04-26,
			}
		}
		catch(...)
		{
			m_dwLastError = GetLastError();
			theLog.Write("!!CCOMPortHelper::OnRead,100,catch sth,m_dwLastError=%d", m_dwLastError);
			BErr = TRUE;	//add by zfq,2017-04-26,begin
			nErrIdx = byvReadContent.size();
			BYTE byte = 0;
			byvReadContent.push_back(byte);//add by zfq,2017-04-26,end
//			OnErr(m_dwLastError);	//移动到锁的作用域外
		}

	}//限制锁的作用域

	//add by zfq,2017-04-26,begin
	int nCount = byvReadContent.size();
	int i = 0;
	BYTE byTmp = 0;
	for (; i < nCount; i++)
	{
		byTmp = byvReadContent[i];
		if(i == nErrIdx && BErr)
		{//在当前序号下发送错误
			OnErr(m_dwLastError);
			break;
		}
		else
		{
			OnByte(byTmp);
		}
	}
	//add by zfq,2017-04-26,end
}
DWORD WINAPI CCOMPortHelper::ThreadFn(LPVOID lpParameter)
{
	theLog.Write("CCOMPortHelper::ThreadFn,1,begin");

	CCOMPortHelper* apThis = (CCOMPortHelper*)lpParameter;
	if (!apThis)
	{
		theLog.Write("!!CCOMPortHelper::ThreadFn,2,apThis=%p", apThis);
		return 0;
	}

	SetEvent(apThis->m_hThreadStarted);

	DWORD dwEventMask=0;
	DWORD dwErr = 0;
	DWORD dwWait;
	COMSTAT Stat = {0};
	OVERLAPPED ol = {0};
	ol.hEvent = CreateEvent(0,TRUE,0,0);
	HANDLE arHandles[2] = {NULL};
	arHandles[0] = apThis->m_hThreadTerm;

	while (true)
	{
		ResetEvent(ol.hEvent);		
		do 
		{
			if(!WaitCommEvent(apThis->m_hComPort, &dwEventMask, &ol))
				break;

			ClearCommError(apThis->m_hComPort, &dwErr, &Stat);

		} while (!Stat.cbInQue);

		arHandles[1] = ol.hEvent ;
		dwWait = WaitForMultipleObjects(2, arHandles, FALSE, INFINITE);
		switch (dwWait)
		{
		case WAIT_OBJECT_0:
			{				
				goto EXIT;
			}
			break;
		case WAIT_OBJECT_0 + 1:
			{
				DWORD dwMask;
				if (GetCommMask(apThis->m_hComPort, &dwMask))
				{
					if (dwMask == EV_TXEMPTY)
					{
						continue;
					}
				}

				if(dwMask & EV_RXCHAR)
				{
					apThis->OnRead();
				}
			}
			break;
		}//switch
	}

EXIT:
	if(NULL != ol.hEvent && INVALID_HANDLE_VALUE != ol.hEvent)
	{
		::CloseHandle(ol.hEvent);
		ol.hEvent = NULL;
	}
	theLog.Write("CCOMPortHelper::ThreadFn,3,end");
	return 0;
}

BOOL CCOMPortHelper::Write(BYTE* pData, int nDataSize, DWORD dwTimeOut /*= 2000*/)
{
	BOOL BErr = FALSE;

	{//限制锁的作用域
		CCriticalSection2::Owner ow(m_cs);

		if (!pData)
		{
			theLog.Write("!!CCOMPortHelper::Write,pData=%p", pData);
			return FALSE;
		}

		for (int i=0; i<2; i++)
		{
			if (!ResetEvent(m_hComEvent))
			{
				m_dwLastError = GetLastError();
				theLog.Write("!!CCOMPortHelper::Write,ResetEvent fail,m_dwLastError=%d,m_hComEvent=0x%x", m_dwLastError, m_hComEvent);
			}
			OVERLAPPED ol = {0};
			ol.hEvent = m_hComEvent;
			DWORD dwBytesWritten = 0;
			int iRet = WriteFile(m_hComPort, pData, nDataSize, &dwBytesWritten, &ol);
			if (!iRet)
			{
				m_dwLastError = GetLastError();
				if (m_dwLastError != 997)	//997错误日志太多了，而且不是真的错误。
				{
					theLog.Write("!!CCOMPortHelper::Write,WriteFile fail,m_hComPort=0x%x,m_dwLastError=%d",m_hComPort, m_dwLastError);
				}
			}
			DWORD dw = WaitForSingleObject(ol.hEvent,dwTimeOut);
			DWORD NumberOfBytesTransferred = 0;
			GetOverlappedResult(m_hComPort, &ol, &NumberOfBytesTransferred, FALSE);
			if (NumberOfBytesTransferred != nDataSize)
			{
				m_dwLastError = GetLastError();
				if (996 == m_dwLastError)
				{//出现996错误,再往com串口写一次数据
					theLog.Write("##CCOMPortHelper Write err %d",m_dwLastError);
					Sleep(200);
					continue;
				}
				else
				{//不是996错误,直接break循环,返回FALSE
					theLog.Write("!!CCOMPortHelper::Write,fail,m_dwLastError=%d", m_dwLastError);
					BErr = TRUE;
//					OnErr(m_dwLastError);	//移动到锁的作用域外
					break;
				}
			}
			else
			{//写com串口成功
				return TRUE;
			}	
		}//for (int i=0; i<2; i++)

	}//限制锁的作用域

	if(BErr)
	{
		OnErr(m_dwLastError);	//移动到锁的作用域外
	}

	return FALSE;
}
