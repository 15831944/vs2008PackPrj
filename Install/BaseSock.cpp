// BaseSock.cpp : implementation file
//

#include "stdafx.h"
#include "BaseSock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseSock

CBaseSock::CBaseSock()
{
	m_bSetTime = false;
	m_bTimeOut = false;
}

CBaseSock::~CBaseSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CBaseSock, CSocket)
	//{{AFX_MSG_MAP(CBaseSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CBaseSock member functions

void CBaseSock::OnReceive(int nErrorCode) 
{	
	CSocket::OnReceive(nErrorCode);
}

BOOL CBaseSock::OnMessagePending() 
{
	if(m_bSetTime)
	{
		MSG msg;
		while(::PeekMessage(&msg,NULL,WM_TIMER,WM_TIMER,PM_REMOVE))
		{
			if(msg.wParam == (UINT)m_nTimerID)
			{
				m_bTimeOut = true;
				CancelBlockingCall();
				KillTimeOut();
				TRACE("\nconnect time out\n");
				break;
			}
		}
	}	
	return CSocket::OnMessagePending();
}

BOOL CBaseSock::SetTimeOut(UINT uTimeOut)
{
	m_bSetTime = true;
	m_nTimerID = SetTimer(NULL,0,uTimeOut,NULL);
	TRACE("m_nTimerID is %d\n\n",m_nTimerID);
	return m_nTimerID;
}

BOOL CBaseSock::KillTimeOut()
{
	m_bSetTime = false;
	TRACE("Close Sock timer");
	return KillTimer(NULL,m_nTimerID);
}

int CBaseSock::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	return CSocket::Send(lpBuf, nBufLen, nFlags);
}
