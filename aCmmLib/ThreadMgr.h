// ThreadMgr.h: interface for the CThreadMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADMGR_H__89314F1C_2094_484B_B4CA_81DE3C900832__INCLUDED_)
#define AFX_THREADMGR_H__89314F1C_2094_484B_B4CA_81DE3C900832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
//class CNetThread;
typedef CList<CWinThread*,CWinThread*> CWinThreadList;
typedef CMap<DWORD,DWORD,CWinThread*,CWinThread*> CWinThreadMap;

#define USE_MAP


#define WM_THREAD_EXIT	WM_USER + 101 
#define WM_RELEASE_TH	WM_USER + 102
 

class _AA_DLL_EXPORT_ CThreadMgr  
{
public:	
	BOOL ActiveNextThread(int nCount = 1);
	CWinThread* CheckThread(DWORD dwThId);
	void Init(int nCount,CRuntimeClass* pRunTimeCls,DWORD dwExitCmd = WM_THREAD_EXIT);
	void Flush(DWORD dwTimeOut = 30000,DWORD dwExitCmd = WM_THREAD_EXIT);
	void Release(CWinThread* pTh,BOOL bSuspend = TRUE);
	void Remove(CWinThread *pTh);	//add by zfq,移除一个线程
	CWinThread* Alloc(BOOL bRun = TRUE);

	CThreadMgr();
	virtual ~CThreadMgr();

	static BOOL CALLBACK EnumThreadWndProc( HWND hwnd,LPARAM lParam);
	void PostActiveThMsg(DWORD dwMsg, WPARAM wp, LPARAM lp = NULL);	//add by zfq,给所有活动的线程发消息
	static BOOL PostThreadMsg(DWORD dwThreadID, int nMsg,WPARAM wp,LPARAM lp);
	static BOOL PostWndMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static BOOL PostWndMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool *pbStop);	//add by zfq,外界可以控制停止

	static void DestroyThread(CWinThread* pTh,DWORD dwWait = 3000,DWORD dwExitCmd = WM_THREAD_EXIT);
protected:
	CWinThread* Create(CRuntimeClass* pRunTimeCls = NULL);
	
//private:
	CWinThreadList	m_oFreeList;
#ifdef USE_MAP 
	CWinThreadMap	m_oActiveMap;
#else
	CWinThreadList	m_oActiveList;
#endif
	int m_nMaxSize;
	CCriticalSection2 m_cs;
	CRuntimeClass* m_pThTunTimeCls;
	DWORD			m_dwExitCmd;
//	CFileLog m_oLog;
};

#endif // !defined(AFX_THREADMGR_H__89314F1C_2094_484B_B4CA_81DE3C900832__INCLUDED_)
