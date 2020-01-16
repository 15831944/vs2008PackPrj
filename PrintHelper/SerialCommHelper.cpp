// RCSerial.cpp: implementation of the CSerialCommHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SerialCommHelper.h"
#include <Process.h>
#include <string>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define ATLTRACE6 theLog.Write

CSerialCommHelper::CSerialCommHelper()
{
	m_hThreadTerm = 0;
	m_hThread = 0;
	m_hThreadStarted = 0;
	m_hCommPort = 0;
	m_hComEvent = 0;
	m_lCmdPos = 0;
	m_eType = BT_SL;
	memset(m_szCmd,0,SERIAL_CMD_BUF_SIZE);
	InitLock();
}

CSerialCommHelper::~CSerialCommHelper()
{
	DelLock();
}

BOOL CSerialCommHelper:: Init2(int nCom,ICopyCmdInterface* pCall)
{
	ASSERT(pCall);
	m_pCallback = pCall;

	COMMTIMEOUTS timeouts;
	DCB dcb = {0};
	char szBuf[MAX_PATH] = {0};
	DCB dc = {0};
	DWORD v15 = 0;
	DWORD dwWait = 0;

	char szCom[20] = {0};
// 	if (nCom >= 1 && nCom <= 20)
// 	{
// 		sprintf(szCom,"COM%d",nCom);
// 	}
// 	else
	{
	    sprintf(szCom,"\\\\?\\COM%d",nCom);
		theLog.Write("##CSerialCommHelper::Init2,1,nCom=%d", nCom);
		//ASSERT(0);
	}
	try
	{
		
		//open the COM Port
		m_hCommPort = ::CreateFile(szCom,
			GENERIC_READ|GENERIC_WRITE,//access ( read and write)
			0,	//(share) 0:cannot share the COM port						
			0,	//security  (None)
			OPEN_EXISTING,// creation : open_existing
			FILE_FLAG_OVERLAPPED,// we want overlapped operation
			0// no templates file for COM port...
			);
		if ( m_hCommPort == INVALID_HANDLE_VALUE )
		{
			theLog.Write("!!CSerialCommHelper::Init2,2,CreateFile fail,err=%d,nCom=%d，szCom=%s", GetLastError(), nCom, szCom);
			ATLTRACE6 ( "CSerialCommHelper : Failed to open COM Port Reason: %d",GetLastError());
			goto ERR1;
		}
		
		ATLTRACE6 ( "CSerialCommHelper : COM port opened successfully" );
	
		//add by zhandb
		/*
		COMMPROP pro = {0};
		pro.wPacketLength = sizeof(pro);
		PST_FAX;
		GetCommProperties(m_hCommPort,&pro);
		WriteLogEx("type = %d",pro.dwProvSubType);
		pro.dwProvSubType = PST_RS232;
		//SetCommConfig(m_hCommPort,(LPCOMMCONFIG )&pro,sizeof(pro));
		*/
		
		
		timeouts.ReadIntervalTimeout					= 1000; 
		timeouts.ReadTotalTimeoutMultiplier		= 1000;
		timeouts.ReadTotalTimeoutConstant			= 1000;
		timeouts.WriteTotalTimeoutMultiplier	= 1000;
		timeouts.WriteTotalTimeoutConstant		= 1000;
		
		if (!SetCommTimeouts(m_hCommPort, &timeouts))
		{
			theLog.Write("!!CSerialCommHelper::Init2,3,SetCommTimeouts fail,err=%d,nCom=%d", GetLastError(), nCom);
			ATLTRACE6 ( "CSerialCommHelper :  Error setting time-outs. %d",GetLastError());
			goto ERR2;
		}
		
#if 1	//modify by zxl,2013.03.22,测试,先删除 [EV_EVENT2 | EV_EVENT1]两个事件,
		//now start to read but first we need to set the COM port settings and the timeouts
		if (! ::SetCommMask(m_hCommPort,EV_RXCHAR /*|  EV_EVENT2 | EV_EVENT1*/ /*| EV_RING | EV_PERR | EV_BREAK | EV_CTS*/) )
		{
			theLog.Write("!!CSerialCommHelper::Init2,4,SetCommMask fail,err=%d,nCom=%d", GetLastError(), nCom);
			ATLTRACE6 ( "CSerialCommHelper : Failed to Set Comm Mask Reason: %d",GetLastError());
			goto ERR2;
		}
		ATLTRACE6 ( "CSerialCommHelper : SetCommMask() success");

#else	//add by zxl,2013.03.22,
		if (! ::SetCommMask(m_hCommPort,EV_RXCHAR |  EV_EVENT2 | EV_EVENT1 /*| EV_RING | EV_PERR | EV_BREAK | EV_CTS*/) )
		{
			ATLTRACE6 ("CSerialCommHelper AA : Failed to Set Comm Mask Reason: %d",GetLastError());
			if (! ::SetCommMask(m_hCommPort,EV_RXCHAR))
			{
				ATLTRACE6 ( "CSerialCommHelper BB : Failed to Set Comm Mask Reason: %d",GetLastError());
				return FALSE;
			}
		}
		ATLTRACE6 ( "CSerialCommHelper : SetCommMask() success");

#endif
		
		//------------------
		//now we need to set baud rate etc,
		
		dcb.DCBlength = sizeof(DCB);
		if (!::GetCommState (m_hCommPort,&dcb))
		{
			theLog.Write("!!CSerialCommHelper::Init2,5,SetCommMask fail,err=%d,nCom=%d", GetLastError(), nCom);
			ATLTRACE6 ( "CSerialCommHelper : Failed to Get Comm State Reason: %d",GetLastError());
			goto ERR2;
		}
		sprintf(szBuf,"baud=%d parity=N data=%d stop=%d",9600,8,1);
		v15 = *(DWORD *)((BYTE*)&dcb + 8);
		
		//	*(DWORD *)((BYTE*)&dcb + 8) = LOBYTE(v15) & 0xDF | 0x10;
		
		if(!BuildCommDCB(szBuf,&dcb))
		{
			theLog.Write("!!CSerialCommHelper::Init2,6,BuildCommDCB fail,err=%d,nCom=%d", GetLastError(), nCom);
			ATLTRACE6 ("BuildCommDCB fail");
			goto ERR2;
		}
#if 1
		//	dcb.BaudRate	= dwBaudRate;
		//	dcb.ByteSize	= byByteSize;
		//	dcb.Parity		= byParity;
		//	if ( byStopBits == 1 )
		//		dcb.StopBits	= ONESTOPBIT;
		//	else if (byStopBits == 2 ) 
		//		dcb.StopBits	= TWOSTOPBITS;
		//	else 
		///		dcb.StopBits	= ONE5STOPBITS;
		
		dcb.fDsrSensitivity = 0;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fOutxDsrFlow = 0;
#endif
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		dcb.EvtChar = 0x71;
		dcb.fDsrSensitivity = 0;
		//	dcb.EofChar = 0xC0;
		//	dcb.fAbortOnError = TRUE;
		//	dcb.fParity = 1;
		//	dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		
		if (!::SetCommState (m_hCommPort,&dcb))
		{
			theLog.Write("!!CSerialCommHelper::Init2,7,SetCommState fail,err=%d,nCom=%d", GetLastError(), nCom);
			ATLTRACE6 ( "CSerialCommHelper : Failed to Set Comm State Reason: %d",GetLastError());
			goto ERR2;
		}
		theLog.Write("Current Settings, (Baud Rate %d; Parity %d; Byte Size %d; Stop Bits %d", dcb.BaudRate, 
			dcb.Parity,dcb.ByteSize,dcb.StopBits);
		
		if (!PurgeComm(m_hCommPort,0xf))
		{
			theLog.Write("!!CSerialCommHelper::Init2,8,PurgeComm fail,err=%d,nCom=%d", GetLastError(), nCom);
			ATLTRACE6 ( "PurgeComm fail");
			goto ERR2;
		}

		m_hComEvent = CreateEvent(0,TRUE,0,0);
#if 0
		if(!Logon())
		{
			theLog.Write("logon err init");
			return FALSE;
		}
#endif
		//create thread terminator event...
		m_hThreadTerm = CreateEvent(0,0,0,0);
		m_hThreadStarted = CreateEvent(0,0,0,0);
		
		m_hThread			= 	(HANDLE)_beginthreadex(0,0,CSerialCommHelper::ThreadFn,(void*)this,0,0 );
		
		dwWait = WaitForSingleObject ( m_hThreadStarted , INFINITE );
		
		ASSERT ( dwWait ==		WAIT_OBJECT_0 );
		
		
		CloseHandle(m_hThreadStarted);
		m_hThreadStarted = 0;
	}
	catch(...)
	{
		theLog.Write("!!CSerialCommHelper::Init2,30,catch sth,err=%d,nCom=%d", GetLastError(), nCom);
		ASSERT(0);
	}

	return TRUE;
ERR2:
	::CloseHandle(m_hCommPort);
	m_hCommPort = 0;
ERR1:
	return FALSE;
}

HRESULT CSerialCommHelper:: Init(std::string szPortName, DWORD dwBaudRate,BYTE byParity,BYTE byStopBits,BYTE byByteSize)
{
	
	HRESULT hr = S_OK;
	try
	{
		//open the COM Port
		m_hCommPort = ::CreateFile(szPortName.c_str (),
			GENERIC_READ|GENERIC_WRITE,//access ( read and write)
			0,	//(share) 0:cannot share the COM port						
			0,	//security  (None)				
			OPEN_EXISTING,// creation : open_existing
			FILE_FLAG_OVERLAPPED,// we want overlapped operation
			0// no templates file for COM port...
			);
		if ( m_hCommPort == INVALID_HANDLE_VALUE )
		{
			ATLTRACE6 ( "CSerialCommHelper : Failed to open COM Port Reason: %d",GetLastError());
			ASSERT ( 0 );
			return E_FAIL;
		}
		
		
		ATLTRACE6 ( "CSerialCommHelper : COM port opened successfully" );
		
	
		//add by zhandb
		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout					= 1000; 
		timeouts.ReadTotalTimeoutMultiplier		= 1000;
		timeouts.ReadTotalTimeoutConstant			= 1000;
		timeouts.WriteTotalTimeoutMultiplier	= 1000;
		timeouts.WriteTotalTimeoutConstant		= 1000;
		
		if (!SetCommTimeouts(m_hCommPort, &timeouts))
		{
			ASSERT(0);
			ATLTRACE6 ( "CSerialCommHelper :  Error setting time-outs. %d",GetLastError());
			return E_FAIL;
		}
		
		//now start to read but first we need to set the COM port settings and the timeouts
		if (! ::SetCommMask(m_hCommPort,EV_RXCHAR | EV_TXEMPTY) )
		{
			ASSERT(0);
			ATLTRACE6 ( "CSerialCommHelper : Failed to Set Comm Mask Reason: %d",GetLastError());
			return E_FAIL;
		}
		ATLTRACE6 ( "CSerialCommHelper : SetCommMask() success");
		
		//------------------
		
		
		//now we need to set baud rate etc,
		DCB dcb = {0};
		
		dcb.DCBlength = sizeof(DCB);
		
		if (!::GetCommState (m_hCommPort,&dcb))
		{
			ATLTRACE6 ( "CSerialCommHelper : Failed to Get Comm State Reason: %d",GetLastError());
			return E_FAIL;
		}
		char szBuf[MAX_PATH] = {0};
		DCB dc = {0};
		sprintf(szBuf,"baud=%d parity=N data=%d stop=%d",9600,8,1);
		//	if(BuildCommDCB(szBuf,&dcb))
		//	{
		//		AfxMessageBox("cc");
		//	}
#if 1
		dcb.BaudRate	= dwBaudRate;
		dcb.ByteSize	= byByteSize;
		dcb.Parity		= byParity;
		if ( byStopBits == 1 )
			dcb.StopBits	= ONESTOPBIT;
		else if (byStopBits == 2 ) 
			dcb.StopBits	= TWOSTOPBITS;
		else 
			dcb.StopBits	= ONE5STOPBITS;
		
		dcb.fDsrSensitivity = 0;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fOutxDsrFlow = 0;
#endif
		
		if (!::SetCommState (m_hCommPort,&dcb))
		{
			ASSERT(0);
			ATLTRACE6 ( "CSerialCommHelper : Failed to Set Comm State Reason: %d",GetLastError());
			return E_FAIL;
		}
		ATLTRACE6 ( "CSerialCommHelper : Current Settings, (Baud Rate %d; Parity %d; Byte Size %d; Stop Bits %d", dcb.BaudRate, 
			dcb.Parity,dcb.ByteSize,dcb.StopBits);
		
		
		
		PurgeComm(m_hCommPort,0xf);
		
		//create thread terminator event...
		m_hThreadTerm = CreateEvent(0,0,0,0);
		m_hThreadStarted = CreateEvent(0,0,0,0);
		
		m_hThread			= 	(HANDLE)_beginthreadex(0,0,CSerialCommHelper::ThreadFn,(void*)this,0,0 );
		
		DWORD dwWait = WaitForSingleObject ( m_hThreadStarted , INFINITE );
		
		ASSERT ( dwWait ==		WAIT_OBJECT_0 );
		
		CloseHandle(m_hThreadStarted);
		m_hThreadStarted = 0;
		
		
	}
	catch(...)
	{
		ASSERT(0);
		hr = E_FAIL;
	}

	return hr;
	
}	

void CSerialCommHelper:: UnInit()
{
	try
	{
		if(m_hThreadTerm)
		{
			SignalObjectAndWait(m_hThreadTerm,m_hThread,INFINITE,FALSE);
			CloseHandle(m_hThreadTerm);
//			CloseHandle(m_hThread);
//			CloseHandle(m_hCommPort);
//			CloseHandle(m_hComEvent);
//			m_hComEvent = 0;
//			m_hCommPort = 0;
//			m_hThread = 0;
			m_hThreadTerm = 0;
		}

		if(m_hThread)
		{
			::CloseHandle(m_hThread);
			m_hThread = 0;
		}

		if(m_hCommPort)
		{
			::CloseHandle(m_hCommPort);
			m_hCommPort = 0;
		}

		if(m_hComEvent)
		{
			::CloseHandle(m_hComEvent);
			m_hComEvent = 0;
		}
	}
	catch(...)
	{
		ASSERT(0);
	}
}
void CSerialCommHelper::InitBrand(BRAND_TYPE eType)
{
	m_eType = eType;
	
}
void CSerialCommHelper::OnCommand2(BYTE* szCmd,int nSize)
{
	ASSERT(szCmd);
	if (nSize < 4)
	{
		theLog.Write("data err");
		return ;
	}
	if (nSize > 4)
	{
		szCmd = &szCmd[nSize - 4];
	}

	long l = *(long*)szCmd;
	long lCmd = ntohl(l);
	BYTE fir = HIBYTE(HIWORD(lCmd));
	BYTE snd = LOBYTE(HIWORD(lCmd));
	BYTE thr = HIBYTE(LOWORD(lCmd));
	BYTE four = LOBYTE(LOWORD(lCmd));

	/*
	BusyBuffer[4] = { 0x02,0,0,0xC0};  //free

	BusyBuffer[4] = { 0x02,1,0,0xC0};  //busy



	*/
	//just for test
#if 0
	theLog.Write("lCmd = %X,cmd = %X,%X,%X",lCmd,snd,thr,four);
	if((fir == 0x02) && (snd == 0x0) && (thr == 0x0) && (four == 0xc0))
	{
		//free
		m_bBusy = FALSE;
	}
	if ((fir == 0x02) && (snd == 0x1) && (thr == 0x0) && (four == 0xc0))
	{
		//busy
		m_bBusy = TRUE;
	}
#endif
	if((fir == 0xA0) && (four == 0xc0))
	{
		switch(snd)
		{
		case 0xF0:
			if (m_pCallback)
			{
				m_pCallback->OnInitSuc();
			}
			break;
		case 0xF1:
			{
				if (m_pCallback)
				{
					m_pCallback->OnCheckStateSuc();
				}
			}
			break;
			//锁定或者解锁成功
		case 0xF2:
			{
				//解锁
				if (thr == 0x0)
				{
					if (m_pCallback)
					{
						m_pCallback->OnLockSuc(FALSE);
					}
				}
				//锁定
				else if (thr == 0x1)
				{
					if (m_pCallback)
					{
						m_pCallback->OnLockSuc(TRUE);
					}
				}
			}
			break;
			//电源开启或关闭成功
		case 0xF4:
			{
				//电源开启
				if (thr == 0x0)
				{
					if (m_pCallback)
					{
						m_pCallback->OnPowerSuc(TRUE);
					}
				}
				//电源关闭
				else if (thr == 0x1)
				{
					if (m_pCallback)
					{
						m_pCallback->OnPowerSuc(FALSE);
					}
				}
			}
			break;
		}	
	}
	if ((fir == 0x01) && (four == 0xc0))
	{
		PAPER_COLOR eColor = WB_PAPER;
		PAPER_TYPE eType = A4_PAPER;
		if(m_eType == BT_DZ || m_eType == BT_SL || m_eType == BT_SL2 || m_eType == BT_LG || BT_JC == m_eType || BT_XP == m_eType)
		{
			if((snd == 0x01) && (thr == 0x01))
			{
				theLog.Write("A4 黑白");
				eColor = WB_PAPER;
				eType = A4_PAPER;
			}
			else if((snd == 0x01) && (thr == 0x00))
			{
				theLog.Write("A3 黑白");
				eColor = WB_PAPER;
				eType = A3_PAPER;
			}
			else if((snd == 0x00) && (thr == 0x01))
			{
				theLog.Write("A4 彩色");
				eColor = COLOR_PAPER;
				eType = A4_PAPER;
			}
			else if((snd == 0x00) && (thr == 0x00))
			{
				theLog.Write("A3 彩色");
				eColor = COLOR_PAPER;
				eType = A3_PAPER;
			}
		}
		else if (m_eType == BT_KM || m_eType == BT_ZD)
		{
			if ((snd & 0x20 )== 0x20)
			{
				eColor = WB_PAPER;
				theLog.Write("黑白");
			}
			else if ((snd & 0x00 )== 0x0)
			{
				eColor = COLOR_PAPER;
				theLog.Write("彩色");
			}

			if (thr == 0x01)
			{
				eType = A4_PAPER;
				theLog.Write("A4");
			}
			else if (thr == 0x02)
			{
				eType = A3_PAPER;
				theLog.Write("A3");
			}
		}
		

		if (m_pCallback)
		{
			m_pCallback->OnOneCopyCmplt(eType,eColor);
		}
	}

}

void CSerialCommHelper::OnCommand(BYTE* szCmd,int nSize)
{
	ASSERT(szCmd);
	ASSERT(nSize == 4);
	long lLogonCmd		= 0x016263C0;
	long lResponseCmd	= 0x012627C0;
	
	if(szCmd && nSize == 4)
	{
		long l = *(long*)szCmd;
		long lCmd = ntohl(l);
		//TRACE("lCmd = %X \n",lCmd);
		switch (lCmd)
		{
		case 0x016263C0:
			{
				theLog.Write("logon suc ");
				if (m_pCallback)
				{
					m_pCallback->OnInitSuc();
				}
			}
			break;
		case 0x012627C0:
			{
				theLog.Write("state suc ");
				if (m_pCallback)
				{
					m_pCallback->OnCheckStateSuc();
				}
			}
			break;
		case 0X1A0A1C0:
			{
				theLog.Write("开始复印 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnBeginOneCopy();
				}
			}
			break;
		case 0x10A0BC0 :
			{
				theLog.Write("复印完成 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnOneCopyCmplt();
				}
			}
			break;
#if 0
		case 0x01A1A0C0:
			{
				theLog.Write("纸型A3 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperType(A3_PAPER);
				}
			}
			break;
		case 0x011A1BC0:
			{
				theLog.Write("纸型A4 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperType(A4_PAPER);
				}
			}
			
			break;
		case 0x012A2BC0:
			{
				theLog.Write("颜色黑白 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperColor(WB_PAPER);
				}
			}
			break;
		case 0x01A2A3C0:
			{
				theLog.Write("颜色彩色 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperColor(COLOR_PAPER);
				}
			}
			break;
		
#else
		case 0x01A1A0C0:
			{
				theLog.Write("颜色彩色 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperColor(COLOR_PAPER);
				}
			}
			break;
		case 0x011A1BC0:
			{
				theLog.Write("颜色黑白 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperColor(WB_PAPER);
				}
			}

			break;
		case 0x012A2BC0:
			{
				theLog.Write("纸型A4 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperType(A4_PAPER);
				}
				
			}
			break;
		case 0x01A2A3C0:
			{
				theLog.Write("纸型A3 %X \n",lCmd);
				if (m_pCallback)
				{
					m_pCallback->OnPaperType(A3_PAPER);
				}
				
			}
			break;
#endif
		default:
			theLog.Write(" unkown cmd %x \n",lCmd);
			break;
		}
		
		
		
	}
	else if (nSize > 4 && (nSize%4 == 0) && szCmd)
	{
		theLog.Write("size = %d",nSize);
		OnCommand(&szCmd[4],4);
	}
	else
	{
		
		theLog.Write("unkown err size = %d",nSize);
	}
}
void CSerialCommHelper::OnByte(BYTE c)
{
	BYTE szCmd[4] = {0};
	BOOL BEndFlag = FALSE;
	{
		Owner ow(m_csLock);
		m_szCmd[m_lCmdPos%SERIAL_CMD_BUF_SIZE] = c;


		//是结束符号
		if (c == 0xC0)
		{
			int nSize = m_lCmdPos%SERIAL_CMD_BUF_SIZE;
			szCmd[0] = m_szCmd[(m_lCmdPos - 3 + SERIAL_CMD_BUF_SIZE)%SERIAL_CMD_BUF_SIZE];
			szCmd[1] = m_szCmd[(m_lCmdPos - 2 + SERIAL_CMD_BUF_SIZE)%SERIAL_CMD_BUF_SIZE];
			szCmd[2] = m_szCmd[(m_lCmdPos - 1 + SERIAL_CMD_BUF_SIZE)%SERIAL_CMD_BUF_SIZE];
			szCmd[3] = m_szCmd[m_lCmdPos%SERIAL_CMD_BUF_SIZE];
			BEndFlag = TRUE;
	//		OnCommand2(szCmd,4);	//del by zfq,2013.11.21,移到后面，移出锁的作用域

			m_lCmdPos = 0;
		}
		else
		{
			m_lCmdPos ++;
		}
	}

	if(BEndFlag)
	{
		OnCommand2(szCmd,4);	//add by zfq,2013.11.21,移到这里，移出锁的作用域
	}
}

void CSerialCommHelper::OnRead()
{
	Owner ow(m_csLock);
	try 
	{

		DWORD dwErr = 0;
		COMSTAT Stat = {0};
		BOOL abRet = false;
		DWORD dwBytesRead = 0;
		BYTE szCmd[10] = {0};
		int nIndex = 0;
		ClearCommError(m_hCommPort, &dwErr, &Stat);

		while(Stat.cbInQue)
		{
			ResetEvent(m_hComEvent);
			OVERLAPPED read_ol = {0};
			read_ol.hEvent = m_hComEvent;

			BYTE OneChar[1];
			int iSize  = sizeof ( OneChar );
			memset(OneChar,0,sizeof OneChar);
			abRet = ::ReadFile(m_hCommPort,OneChar,1,&dwBytesRead,&read_ol);
			if (!abRet ) 
			{
				DWORD dwErr = GetLastError();
				if (dwErr != 997)
				{
					break;
				}
				if ( !GetOverlappedResult(m_hCommPort, &read_ol, &dwBytesRead, 1) )
				{
					break;
				}

			}
			else
			{

				SetEvent(read_ol.hEvent);
			}

			ClearCommError(m_hCommPort, &dwErr, &Stat);
			//
			szCmd[nIndex] = OneChar[0];
			nIndex ++;
			OnByte(OneChar[0]);
#if 0
			if(CHWConfig::GetInstance().IsHwVerTwo())
			{
				if (OneChar[0] == 0xc0)
				{
					OnCommand2(szCmd,nIndex);
					nIndex = 0;
				}
			}
			else
			{
				//one command over
				if (OneChar[0] == 0xc0 && nIndex%4 == 0)
				{
					OnCommand(szCmd,nIndex);
					nIndex = 0;
				}
			}
#endif
			//theLog.Write("nIndx = %d,%02X",nIndex,OneChar[0]);
			//	TRACE("read %X \n",OneChar[0]);
		}
#if 0
		//
		if (nIndex >= 4)
		{
			if(CHWConfig::GetInstance().IsHwVerTwo())
			{
				if((szCmd[nIndex - 1] == 0xC0))
					OnCommand2(szCmd,nIndex);
				else
					ASSERT(0);
			}
			else
			{
				if(szCmd[nIndex - 1] == 0xC0)
					OnCommand(szCmd,nIndex);
				else
				{
					ASSERT(0);
					CString sData;
					CString s;
					for (int n = 0; n < nIndex; n ++)
					{
						s.Format(_T("%X"),szCmd[n]);
						sData+=s;
					}

					theLog.Write("OnCommand err %s",sData);
				}
			}
			

		}
		else
		{
			//ASSERT(0);
			//theLog.Write("OnCommand err nIndex = %d",nIndex);
		}
#endif

	}
	catch(...)
	{
		ASSERT(0);
	}
}
unsigned __stdcall CSerialCommHelper::ThreadFn(void*pvParam)
{
	CSerialCommHelper* apThis = (CSerialCommHelper*) pvParam ;
	bool abContinue = true;
	DWORD dwEventMask=0;
	
	DWORD dwErr = 0;
	COMSTAT Stat = {0};
	
	HANDLE arHandles[2];
	arHandles[0] = apThis->m_hThreadTerm;
	
	DWORD dwWait;
	SetEvent(apThis->m_hThreadStarted);
	
	PurgeComm(apThis->m_hCommPort,0xf);
	
	OVERLAPPED ol = {0};
	ol.hEvent = CreateEvent(0,TRUE,0,0);
	while (  abContinue )
	{
		
		ResetEvent(ol.hEvent);		
		do 
		{
			if(!WaitCommEvent(apThis->m_hCommPort, &dwEventMask, &ol))
				break;
			
			ClearCommError(apThis->m_hCommPort, &dwErr, &Stat);
			
		} while (!Stat.cbInQue);
		
		
		arHandles[1] = ol.hEvent ;
		dwWait = WaitForMultipleObjects (2,arHandles,FALSE,INFINITE);
		switch ( dwWait )
		{
		case WAIT_OBJECT_0:
			{				
				if(NULL != ol.hEvent && INVALID_HANDLE_VALUE != ol.hEvent)
				{//add by zfq,2016-01-30
					::CloseHandle(ol.hEvent);
					ol.hEvent = NULL;
				}

				_endthreadex(1);
			}
			break;
		case WAIT_OBJECT_0 + 1:
			{
				DWORD dwMask;
				if (GetCommMask(apThis->m_hCommPort,&dwMask) )
				{
					if ( dwMask == EV_TXEMPTY )
					{
						//	AfxMessageBox("Data sent");
						//	ResetEvent ( ov.hEvent );
						continue;
					}
					
				}
				
				if(dwMask & EV_RXCHAR)
				{
#if 1
					apThis->OnRead();
#else
					try 
					{
						
						BOOL abRet = false;
						DWORD dwBytesRead = 0;
						BYTE szCmd[10] = {0};
						int nIndex = 0;
						ClearCommError(apThis->m_hCommPort, &dwErr, &Stat);
						
						while(Stat.cbInQue)
						{
							ResetEvent(apThis->m_hComEvent);
							OVERLAPPED read_ol = {0};
							read_ol.hEvent = apThis->m_hComEvent;
							
							BYTE OneChar[1];
							int iSize  = sizeof ( OneChar );
							memset(OneChar,0,sizeof OneChar);
							abRet = ::ReadFile(apThis->m_hCommPort,OneChar,1,&dwBytesRead,&read_ol);
							if (!abRet ) 
							{
								DWORD dwErr = GetLastError();
								if (dwErr != 997)
								{
									break;
								}
								if ( !GetOverlappedResult(apThis->m_hCommPort, &read_ol, &dwBytesRead, 1) )
								{
									break;
								}
								
							}
							else
							{
								
								SetEvent(read_ol.hEvent);
							}
							
							ClearCommError(apThis->m_hCommPort, &dwErr, &Stat);
							//
							szCmd[nIndex] = OneChar[0];
							nIndex ++;
							//one command over
							if (OneChar[0] == 0xc0)
							{
								apThis->OnCommand(szCmd,nIndex);
								nIndex = 0;
							}
							//	TRACE("nIndx = %d \n",nIndex);
							//	TRACE("read %X \n",OneChar[0]);
						}
						//
						if (nIndex >= 4)
						{
							if(szCmd[nIndex - 1] == 0xC0)
								apThis->OnCommand(szCmd,nIndex);
							else
								ASSERT(0);
							
						}
						else
						{
							
						}
						
						
					}
					catch(...)
					{
						ASSERT(0);
					}
				
#endif
				}
			}
			break;
		}//switch
	}

	//add by zfq,2016-01-30
	if(NULL != ol.hEvent && INVALID_HANDLE_VALUE != ol.hEvent)
	{
		::CloseHandle(ol.hEvent);
		ol.hEvent = NULL;
	}
	return 0;
}

BOOL CSerialCommHelper::Write(BYTE* pCmd,int nLen,DWORD dwTimeOut)
{
	Owner ow(m_csLock);
	COMSTAT st = {0};
	//	ClearCommError(m_hCommPort,&dw,&st);

#if 0
	if (!ResetEvent(m_hComEvent))
	{
		theLog.Write("!!CSerialCommHelper::Write,ResetEvent fail,error=%d,m_hComEvent=0x%x",GetLastError(),m_hComEvent);
	}
	OVERLAPPED ol = {0};
	ol.hEvent = m_hComEvent;
	DWORD dwBytesWritten = 0;
	int iRet = WriteFile (m_hCommPort,pCmd,nLen,&dwBytesWritten  ,&ol);
	if (!iRet)
	{
		int nErr = GetLastError();
		theLog.Write("!!CSerialCommHelper::Write,WriteFile fail,m_hCommPort=0x%x,error=%d",m_hCommPort, nErr);
	}
	DWORD dw = ::WaitForSingleObject(ol.hEvent,dwTimeOut);
	DWORD	NumberOfBytesTransferred = 0;
	GetOverlappedResult(m_hCommPort, &ol, &NumberOfBytesTransferred, FALSE);
	if (NumberOfBytesTransferred != nLen)
	{
		int nErr = ::GetLastError();
		theLog.Write("CSerialCommHelper Write err %d",nErr);
		if (996 == nErr)
		{
			theLog.Write("############CSerialCommHelper Write err %d",nErr);
			return TRUE;
		}
		return FALSE;
	}

	return TRUE;

#else
	int nMax = 2;
	int nCount = 0;
	while (nCount < nMax)
	{
		nCount++;
		if (!ResetEvent(m_hComEvent))
		{
			theLog.Write("!!CSerialCommHelper::Write,ResetEvent fail,error=%d,m_hComEvent=0x%x",GetLastError(),m_hComEvent);
		}
		OVERLAPPED ol = {0};
		ol.hEvent = m_hComEvent;
		DWORD dwBytesWritten = 0;
		//theLog.Write("##CSerialCommHelper::Write,Send=[%02X %02X %02X %02X]",pCmd[0], pCmd[1], pCmd[2], pCmd[3]);
		int iRet = WriteFile (m_hCommPort,pCmd,nLen,&dwBytesWritten  ,&ol);
		if (!iRet)
		{
			int nErr = GetLastError();
			if (nErr != 997)	//997错误日志太多了，而且不是真的错误。
			{
				theLog.Write("!!CSerialCommHelper::Write,WriteFile fail,m_hCommPort=0x%x,error=%d",m_hCommPort, nErr);
			}
		}
		DWORD dw = ::WaitForSingleObject(ol.hEvent,dwTimeOut);
		DWORD	NumberOfBytesTransferred = 0;
		GetOverlappedResult(m_hCommPort, &ol, &NumberOfBytesTransferred, FALSE);
		if (NumberOfBytesTransferred != nLen)
		{
			int nErr = ::GetLastError();
			theLog.Write("CSerialCommHelper Write err %d",nErr);
			if (996 == nErr)
			{//出现996错误,再往com串口写一次数据
				theLog.Write("############CSerialCommHelper Write err %d",nErr);
				Sleep(200);
				continue;
			}
			else
			{//不是996错误,直接break循环,返回FALSE
				break;
			}
		}
		else
		{//写com串口成功
			return TRUE;
		}
	}

	return FALSE;
#endif
}

//板子初始化
BOOL CSerialCommHelper::Logon(BYTE* pBuf ,int nLen)
{
	if(!pBuf)
	{
		BYTE szBuf[4] = {0x01, 0x62, 0x63, 0xC0};
		return Write(szBuf,4/*,INFINITE*/);
	}
	else
	{
		return Write(pBuf,nLen/*,INFINITE*/);
	}
}
//检测状态
BOOL CSerialCommHelper::CheckState(BYTE* pBuf ,int nLen)
{
	Owner ow(m_csLock);
	if(!pBuf)
	{
		BYTE szBuf[4] = {0x01, 0x26, 0x27, 0xC0 };
		return Write(szBuf,4);
	}
	else
	{
		return Write(pBuf,nLen);
	}
}
//解锁或者锁定
BOOL CSerialCommHelper::Lock(BYTE* pBuf /*= 0*/,int nLen/* = 0*/)
{
	if(!pBuf)
	{
		
		BYTE szBuf[4] = {0x01, 0x62, 0x63, 0xC0};
		return Write(szBuf,4/*,INFINITE*/);

	}
	else
	{
		return Write(pBuf,nLen/*,INFINITE*/);
	}
}

BOOL CSerialCommHelper::UnLock(BYTE* pBuf /*= 0*/,int nLen/* = 0*/)
{
	if(!pBuf)
	{
		
		BYTE szBuf[4] = {0x01, 0x62, 0x63, 0xC0};
		return Write(szBuf,4/*,INFINITE*/);
	}
	else
	{
		return Write(pBuf,nLen/*,INFINITE*/);
	}
}