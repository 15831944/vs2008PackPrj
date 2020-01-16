// ThreadMgr.cpp: implementation of the CThreadMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "PltSvr.h"
#include "ThreadMgr.h"
//#include "NetThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadMgr::CThreadMgr()
{
	m_nMaxSize = 20;
	m_dwExitCmd = WM_THREAD_EXIT;
//	m_oLog.SetLogName("CThreadMgr");
}

CThreadMgr::~CThreadMgr()
{
	Flush();
}

CWinThread* CThreadMgr::Alloc(BOOL bRun)
{
	CCriticalSection2::Owner ow(m_cs);

	CWinThread* pThread = NULL;
	if(m_oFreeList.GetCount() > 2)
	{
		pThread = m_oFreeList.RemoveHead();
	}
	else
	{
		pThread = Create();
		if (pThread)
			m_oFreeList.AddTail(pThread);
		pThread = m_oFreeList.RemoveHead();
	}
	if (pThread)
	{
#ifdef USE_MAP
		m_oActiveMap.SetAt(pThread->m_nThreadID,pThread);
#else
		m_oActiveList.AddTail(pThread);
#endif
	
		if (bRun)
		{
			pThread->ResumeThread();
		}
	}
	return pThread;
}
//激活前面几个线程
BOOL CThreadMgr::ActiveNextThread(int nCount /*= 1*/)
{
	CCriticalSection2::Owner ow(m_cs);
	POSITION pos = m_oFreeList.GetHeadPosition();
	while(pos && nCount > 0)
	{
		CWinThread* pTh = m_oFreeList.GetNext(pos);
		if(pTh)
		{
			pTh->ResumeThread();
			nCount --;
		}
		
	}
	return TRUE;
}
CWinThread* CThreadMgr::Create(CRuntimeClass* pRunTimeCls)
{
	if (!pRunTimeCls)
	{
		pRunTimeCls = m_pThTunTimeCls;
	}
	ASSERT(pRunTimeCls);
	if (!pRunTimeCls)
	{
		return NULL;
	}
	CWinThread* pTh = NULL;
	/*RUNTIME_CLASS(CNetThread)*/
	pTh = ::AfxBeginThread(pRunTimeCls,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	ASSERT(pTh);
	//pTh->SuspendThread();
	return pTh;
}

void CThreadMgr::Remove(CWinThread *pTh)
{
	ASSERT(pTh);
	CCriticalSection2::Owner ow(m_cs);

	//---------------------------
	//从活动线程中释放
#ifdef USE_MAP
	m_oActiveMap.RemoveKey(pTh->m_nThreadID);
#else
	POSITION pos = m_oActiveList.Find(pTh);
	if(pos)
	{
		m_oActiveList.RemoveAt(pos);
	}
#endif
}

void CThreadMgr::Release(CWinThread *pTh,BOOL bSuspend)
{
	ASSERT(pTh);
	CCriticalSection2::Owner ow(m_cs);

	//---------------------------
	//从活动线程中释放
#ifdef USE_MAP
	m_oActiveMap.RemoveKey(pTh->m_nThreadID);
#else
	POSITION pos = m_oActiveList.Find(pTh);
	if(pos)
	{
		m_oActiveList.RemoveAt(pos);
	}
#endif
	//
	if (m_oFreeList.GetCount() > m_nMaxSize)
	{
		DestroyThread(pTh);
	}
	else
	{
		if(bSuspend)
		{
			pTh->SuspendThread();
		}
		m_oFreeList.AddTail(pTh);
	}
}

void CThreadMgr::DestroyThread(CWinThread* pTh,DWORD dwWait /*= 3000*/,DWORD dwExitCmd /*= WM_THREAD_EXIT*/)
{
	ASSERT(pTh);

	//需要处理，暂时这样
	if (pTh && pTh->m_hThread)
	{
		EnumThreadWindows( pTh->m_nThreadID,EnumThreadWndProc,0);
		::Sleep(10);
		HANDLE h = pTh->m_hThread;
		int nIndex = 0; 
		BOOL b = pTh->PostThreadMessage(dwExitCmd,0,0);
		while(!b)
		{
			nIndex ++;
			b = pTh->PostThreadMessage(dwExitCmd,0,0);
			if(nIndex > 3)
			{
				break;
			}
		}

		DWORD dw = ::WaitForSingleObject(h,dwWait);
		if (dw != WAIT_OBJECT_0)
		{
			::TerminateThread(h, dw);
		}
		pTh = NULL;
	}
}

void CThreadMgr::Flush(DWORD dwTimeOut /* =30000*/,DWORD dwExit)
{
	CCriticalSection2::Owner ow(m_cs);
	//先激活未活动的线程
	ActiveNextThread(m_oFreeList.GetCount());
#ifdef USE_MAP
	POSITION pos = m_oActiveMap.GetStartPosition();
	while (pos)
	{
		DWORD dwKey = 0;
		CWinThread* pTh = NULL;
		m_oActiveMap.GetNextAssoc(pos,dwKey,pTh);
		if (pTh && pTh->m_hThread)
		{
			CCommonFun::WriteFileLog("CThreadMgr 结束线程 = %d ",pTh->m_nThreadID);
			DestroyThread(pTh,dwTimeOut,dwExit);
		}
	}
	m_oActiveMap.RemoveAll();
#else
	while (!m_oActiveList.IsEmpty())
	{	
		DestroyThread(m_oActiveList.RemoveHead());
	}
#endif
	while (!m_oFreeList.IsEmpty())
	{
		CWinThread* pTh = m_oFreeList.RemoveHead();
		if(pTh->ResumeThread() == 1)
		{
			::Sleep(10);
		}
		DestroyThread(pTh,dwTimeOut,dwExit);
	}	
}

void CThreadMgr::Init(int nCount,CRuntimeClass* pRunTimeCls,DWORD dwExitCmd /*= WM_THREAD_EXIT*/)
{
	ASSERT(0 <= nCount && nCount < 200);
	ASSERT(dwExitCmd);
	CCriticalSection2::Owner ow(m_cs);
	ASSERT(pRunTimeCls);
	m_dwExitCmd = dwExitCmd;
	if (!pRunTimeCls)
	{
		return ;
	}

	m_nMaxSize = nCount;
	m_pThTunTimeCls = pRunTimeCls;
	for(int nIndex = 0; nIndex < nCount; nIndex ++)
	{
		CWinThread* pThread = Create(m_pThTunTimeCls);
		ASSERT(pThread);
		m_oFreeList.AddTail(pThread);
	}
}

CWinThread* CThreadMgr::CheckThread(DWORD dwThId)
{
	CCriticalSection2::Owner ow(m_cs);
	CWinThread* pTh = NULL;
#ifdef USE_MAP
	if(m_oActiveMap.Lookup(dwThId,pTh))
	{
		return pTh;
	}
#else
	POSITION pos = m_oActiveList.GetHeadPosition();
	while(pos)
	{
		pTh = m_oActiveList.GetNext(pos);
		if (pTh)
		{
			if (pTh->m_nThreadID == dwThId)
			{
				return pTh;
			}
		}
	}
#endif
	return pTh;
}

void CThreadMgr::PostActiveThMsg(DWORD dwMsg, WPARAM wp, LPARAM lp/* = NULL*/)	//add by zfq,给所有活动的线程发消息
{
	POSITION pos = NULL;
	CWinThread *pTh = NULL;
#ifdef USE_MAP
	DWORD dwKey = 0;
	pos = m_oActiveMap.GetStartPosition();
	while(pos)
	{
		m_oActiveMap.GetNextAssoc(pos, dwKey, pTh);
		if(!pTh)
		{
			continue;
		}
		while(1)
		{
			if(!PostThreadMessage(pTh->m_nThreadID, dwMsg, wp, lp))
			{
				if(GetLastError() == ERROR_NOT_ENOUGH_QUOTA)
				{
					Sleep(10);
					continue;
				}
				else
				{//给该线程发消息失败
					ASSERT(0);
					break;
				}
			}
			break;
		}//while(1)
	}//while(pos)
#else
	POSITION pos = m_oActiveList.GetHeadPosition();
	while(pos)
	{
		pTh = m_oActiveList.GetNext(pos);
		if(!pTh)
		{
			ASSERT(0);
			continue;
		}
		while(1)
		{
			if(!PostThreadMessage(pTh->m_nThreadID, dwMsg, wp, lp))
			{
				if(GetLastError() == ERROR_NOT_ENOUGH_QUOTA)
				{
					Sleep(10);
					continue;
				}
				else
				{//给该线程发消息失败
					ASSERT(0);
					break;
				}
			}
			break;
		}//while(1)
	}
#endif
}

BOOL CThreadMgr::PostThreadMsg(DWORD dwThreadID,int nMsg,WPARAM wp,LPARAM lp)
{
	DWORD dwTick = ::GetTickCount();
	while(1)
	{
		if(!PostThreadMessage(dwThreadID,nMsg,wp,lp))
		{
			DWORD dwErr = GetLastError();
			DWORD dwSpan = ::GetTickCount() - dwTick;
			if (dwSpan >= 1000 * 3)
			{
				CCommonFun::WriteFileLog("CThreadMgr::PostThreadMsg Timeout (%d)",dwSpan);
				return FALSE;
			}
			if(dwErr == ERROR_NOT_ENOUGH_QUOTA)
				Sleep(10);
			else
			{
				ASSERT(0);
				TRACE1("CThreadMgr::PostThreadMsg(),error, dwErr=%d\n", dwErr);
				return FALSE;
			}
		}
		else
			return TRUE;
	}
	ASSERT(0);
	return FALSE;
}
BOOL CThreadMgr::PostWndMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwTick = ::GetTickCount();
	ASSERT(IsWindow(hWnd));
	if (!IsWindow(hWnd))
		return FALSE;
	while(1)
	{
		if(!PostMessage(hWnd, Msg, wParam, lParam))
		{
			DWORD dwErr = GetLastError();
			DWORD dwSpan = ::GetTickCount() - dwTick;
			if (dwSpan >= 1000 * 3)
			{
				CCommonFun::WriteFileLog("CThreadMgr::PostThreadMsg Timeout (%d)",dwSpan);
				return FALSE;
			}
	//		DWORD dwErr = GetLastError();
			if(dwErr == ERROR_NOT_ENOUGH_QUOTA)
				Sleep(10);
			else
			{
				ASSERT(0);
				TRACE1("CThreadMgr::PostWndMsg(),error, dwErr=%d\n", dwErr);
				return FALSE;
			}
		}
		else
			return TRUE;
	}
	ASSERT(0);
	return FALSE;
}

BOOL CThreadMgr::PostWndMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool *pbStop)	//add by zfq,外界可以控制停止
{
	DWORD dwTick = ::GetTickCount();
	ASSERT(IsWindow(hWnd));
	if (!IsWindow(hWnd))
		return FALSE;
	while(1)
	{
		if(pbStop && *pbStop)
		{
			TRACE("CThreadMgr::PostWndMsg()2, be stopped\n");
			return FALSE;
		}

		if(!PostMessage(hWnd, Msg, wParam, lParam))
		{
			DWORD dwErr = GetLastError();
			DWORD dwSpan = ::GetTickCount() - dwTick;
			if (dwSpan >= 1000 * 3)
			{
				CCommonFun::WriteFileLog("CThreadMgr::PostThreadMsg Timeout (%d)",dwSpan);
				return FALSE;
			}
//			DWORD dwErr = GetLastError();
			if(dwErr == ERROR_NOT_ENOUGH_QUOTA)
				Sleep(10);
			else
			{
				ASSERT(0);
				TRACE1("CThreadMgr::PostWndMsg()2,error, dwErr=%d\n", dwErr);
				return FALSE;
			}
		}
		else
			return TRUE;
	}
	ASSERT(0);
	return FALSE;
}

BOOL CALLBACK CThreadMgr::EnumThreadWndProc( HWND hwnd,LPARAM lParam)
{
	char sz[MAX_PATH] = {0};
	GetWindowText(hwnd,sz,MAX_PATH);
	char szCls[MAX_PATH] = {0};
	GetClassName(hwnd,szCls,MAX_PATH);
	//theLog.Write("Txt = %s,Cls = %s",sz,szCls);
	//::PostMessage(hwnd,WM_CANCELMODE,0,0);
	if (stricmp(szCls,"#32768") == 0 || stricmp(szCls,"#32770") == 0)
	{
		::PostMessage(hwnd,WM_CLOSE,0,0);
	}
	return TRUE;
}