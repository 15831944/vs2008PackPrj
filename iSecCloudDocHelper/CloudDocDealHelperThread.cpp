// CloudDocDealHelperThread.cpp : 实现文件
//

#include "stdafx.h"
#include "CloudDocDealHelperThread.h"

IMPLEMENT_DYNCREATE(CCloudDocDealHelperThread, CWinThread)

CCloudDocDealHelperThread::CCloudDocDealHelperThread()
{
	m_nCheckBlockedCloudDocTimer = NULL;
	m_pOwner = NULL;
	SetActive(FALSE);
}

CCloudDocDealHelperThread::~CCloudDocDealHelperThread()
{
}

BOOL CCloudDocDealHelperThread::InitInstance()
{
	theLog.Write("CCloudDocDealHelperThread::InitInstance, m_nThreadID=%u", m_nThreadID);
	EnableCheckBlockedCloudDocTimer(TRUE);
	return TRUE;
}

int CCloudDocDealHelperThread::ExitInstance()
{
	theLog.Write("CCloudDocDealHelperThread::ExitInstance, m_nThreadID=%u", m_nThreadID);
	SetActive(FALSE);

	//停止检测阻塞的文档
	EnableCheckBlockedCloudDocTimer(FALSE);

	//释放Office进程信息
	ReleaseCloudJobDealProcessInfo();

	//释放窗口进程信息
	ReleaseWindowProcessInfo();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCloudDocDealHelperThread, CWinThread)
END_MESSAGE_MAP()


//结束线程
void CCloudDocDealHelperThread::OnEndTh(WPARAM wp, LPARAM lp)
{
	theLog.Write("CCloudDocDealHelperThread::OnEndTh,m_nThreadID=%u", m_nThreadID);
	ExitInstance();
	AfxEndThread(0);
}

BOOL CCloudDocDealHelperThread::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_TIMER:
		{
			OnTimer(pMsg->wParam,pMsg->lParam);		
		}
		break;
	case WM_THREAD_EXIT:
		{
			OnEndTh(pMsg->wParam,pMsg->lParam);
			return TRUE;
		}
		break;
	}

	return CWinThread::PreTranslateMessage(pMsg);
}

void CCloudDocDealHelperThread::OnTimer(WPARAM wp, LPARAM lp)
{
	if (wp == m_nCheckBlockedCloudDocTimer)
	{
		if (IsActive())
		{
			OnCheckBlockedCloudDoc();
		}
	}
}

void CCloudDocDealHelperThread::SetOwner(CCloudDocDealMgr* pOwner)
{
	m_pOwner = pOwner;
	if (!m_pOwner)
	{
		theLog.Write("!!CCloudDocDealHelperThread::SetOwner,m_pOwner=%p", m_pOwner);
	}
}

void CCloudDocDealHelperThread::SetActive(BOOL bActive)
{
	if (bActive)
	{
		InterlockedExchange(&m_lActiveDealDoc,TRUE);
	}
	else
	{
		InterlockedExchange(&m_lActiveDealDoc,FALSE);
	}
}

BOOL CCloudDocDealHelperThread::IsActive()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lActiveDealDoc);
	return lVal;
}

CString CCloudDocDealHelperThread::GetTimeStr(double fTime)
{
	COleDateTime time;
	time.m_dt = fTime;
	return time.Format("%Y-%m-%d %H:%M:%S");
}

void CCloudDocDealHelperThread::EnableCheckBlockedCloudDocTimer(BOOL bEnable)
{
	//theLog.Write("CCloudDocDealHelperThread::EnableCheckBlockedCloudDocTimer,bEnable=%d,m_nCheckBlockedCloudDocTimer=%u", bEnable, m_nCheckBlockedCloudDocTimer);
	if (bEnable)
	{
		if (!m_nCheckBlockedCloudDocTimer)
		{
			m_nCheckBlockedCloudDocTimer = ::SetTimer(NULL,0,Check_Blocked_Cloud_Doc_TimeOut_MS,NULL);
		}
	}
	else
	{
		if (m_nCheckBlockedCloudDocTimer)
		{
			KillTimer(NULL, m_nCheckBlockedCloudDocTimer);
			m_nCheckBlockedCloudDocTimer = NULL;
		}
	}
}

void CCloudDocDealHelperThread::OnCheckBlockedCloudDoc()
{
	//theLog.Write("CCloudDocDealHelperThread::OnCheckBlockedCloudDoc");

	//获取所有界面窗口进程
	GetAllUiProcess();

	//将m_oCloudJobDealProcessInfoMap中的进程信息重置状态为PROCESS_STATE_NOT_EXIST
	POSITION pos = m_oCloudJobDealProcessInfoMap.GetStartPosition();
	DWORD  dwProcessID = 0;	//进程ID
	CloudJobDealProcessInfo* pInfo = NULL;	//进程信息
	while (pos != NULL)
	{
		m_oCloudJobDealProcessInfoMap.GetNextAssoc(pos, dwProcessID, pInfo);
		if (pInfo)
		{
			pInfo->oProcessState = PROCESS_STATE_NOT_EXIST;
		}
	}

	//当前运行的office进程
	CString szMSOfficeProcessExe(MS_Office_ProcessExe);
	CString szMSOfficeErrorExe(MS_Office_ErrorExe);

	//获取所有office进程
	PROCESSENTRY32 pe32;//用来存放快照进程信息的一个结构体。
	pe32.dwSize = sizeof(pe32);   //这个操作必须要不然获取不到pe32  

	//给系统内的所有进程拍一个快照  
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if (hProcessSnap == INVALID_HANDLE_VALUE)  
	{  
		theLog.Write("!!CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,1,hProcessSnap=%p,err=%u", hProcessSnap, GetLastError());
		return;  
	}

	BOOL bMore = Process32First(hProcessSnap , &pe32);
	if (!bMore)
	{
		theLog.Write("!!CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,2,err=%u", GetLastError());
	}

	//遍历进程快照，轮流显示每个进程的信息  
	while (bMore)  
	{
		//theLog.Write("CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,3,szExeFile=[%s],PID=[%u],Threads=[%u]"
		//	, pe32.szExeFile, pe32.th32ProcessID, pe32.cntThreads);
		CString szExeFile(pe32.szExeFile);
		szExeFile.MakeUpper();
		if (szMSOfficeProcessExe.Find(szExeFile) >= 0)
		{
			BOOL bNewProcess = TRUE;
			if (m_oCloudJobDealProcessInfoMap.Lookup(pe32.th32ProcessID, pInfo) && pInfo)
			{
				//如果进程ID存在并且进程名相同，则将状态设置为PROCESS_STATE_RUNING
				if (szExeFile.CompareNoCase(pInfo->oProcessEntry32.szExeFile) == 0)
				{
					bNewProcess = FALSE;
					pInfo->oProcessState = PROCESS_STATE_RUNING;
				}
				else
				{//如果进程ID存在并且进程名不同，则表示旧的进程已经结束，要删除信息
					delete pInfo;
					m_oCloudJobDealProcessInfoMap.RemoveKey(pe32.th32ProcessID);
				}
			}
			if (bNewProcess)
			{
				pInfo = new CloudJobDealProcessInfo;
				memset(pInfo, 0x0, sizeof(CloudJobDealProcessInfo));
				memcpy(&pInfo->oProcessEntry32, &pe32, sizeof(PROCESSENTRY32));
				pInfo->oProcessState = PROCESS_STATE_RUNING;
				pInfo->fProcessBeginTime = COleDateTime::GetCurrentTime().m_dt;
				m_oCloudJobDealProcessInfoMap.SetAt(pe32.th32ProcessID, pInfo);
			}
		}
		else if (szMSOfficeErrorExe.Find(szExeFile) >= 0)
		{
			//结束Office错误进程
			BOOL bRet = CCommonFun::TerminaPID(pe32.th32ProcessID);
			theLog.Write("##CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,4,TerminaPID,bRet=%d,th32ProcessID=%d,szExeFile=%s"
				, bRet, pe32.th32ProcessID, pe32.szExeFile);
		}
		bMore = Process32Next(hProcessSnap, &pe32);
	}  

	//关闭内核对象  
	CloseHandle(hProcessSnap);

	//将m_oCloudJobDealProcessInfoMap中的进程信息为PROCESS_STATE_NOT_EXIST的删除，并且将超时进程强制结束。
	pos = m_oCloudJobDealProcessInfoMap.GetStartPosition();
	dwProcessID = 0;	//进程ID
	pInfo = NULL;	//进程信息
	while (pos != NULL)
	{
		m_oCloudJobDealProcessInfoMap.GetNextAssoc(pos, dwProcessID, pInfo);
		if (pInfo)
		{
			//theLog.Write("CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,4.1,oProcessState=%d,fProcessBeginTime=%s,"
			//	"dwProcessID=%d,th32ProcessID=%d,szExeFile=%s"
			//	, pInfo->oProcessState, GetTimeStr(pInfo->fProcessBeginTime), dwProcessID
			//	, pInfo->oProcessEntry32.th32ProcessID, pInfo->oProcessEntry32.szExeFile);

			//判断进程是否存在窗口，如果存在，则向窗口发送模拟Enter按键，
			//以模拟点击类似于当office文档存在错误时，会弹出提示，从而阻塞线程，
			//一般情况下点击错误提示框，文档可能会转换成功。
			//但是如果一些提示框在点击确认之后，文档转换线程依然阻塞，
			//则只能在达到阻塞超时时间后，强制结束该Office进程，文档转换失败。
			PWindowProcessInfo pWindows = GetWindowProcessInfo(dwProcessID);
			if (pWindows)
			{
				theLog.Write("CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,4.2,hWnd=%u,dwProcessID=%d,cWindowText=%s"
					, pWindows->hWnd, pWindows->dwProcessID, pWindows->cWindowText);
				if (stricmp(pInfo->oProcessEntry32.szExeFile, MS_Office_Word) == 0)
				{
					//有时候弹框要求两次Enter,或者一次Esc
					SimulationKeyboard(pWindows->hWnd, VK_ESCAPE);
				}
				else if (stricmp(pInfo->oProcessEntry32.szExeFile, MS_Office_PowerPoint) == 0)
				{
					SimulationKeyboard(pWindows->hWnd, VK_RETURN);
					CString szWindowText(pWindows->cWindowText);
					if (szWindowText.Find("Microsoft PowerPoint 安全声明") >= 0)
					{
						//在中文版本office2016下，此窗口只能向子窗口发送消息，才能结束弹框，
						//其它office没有测试，结果未知
						HWND hWnd = GetWindow(pWindows->hWnd, GW_CHILD);
						if (hWnd)
						{
							SimulationKeyboard(hWnd, VK_ESCAPE);
						}
					}
				}
				else
				{
					SimulationKeyboard(pWindows->hWnd, VK_RETURN);
				}
			}
			else
			{
				//theLog.Write("CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,4.3,dwProcessID=%d", dwProcessID);
			}
			
			if (pInfo->oProcessState == PROCESS_STATE_NOT_EXIST)
			{
				delete pInfo;
				m_oCloudJobDealProcessInfoMap.RemoveKey(dwProcessID);
			}
			else if (pInfo->oProcessState == PROCESS_STATE_RUNING)
			{
				COleDateTime oProcessBeginTime;
				oProcessBeginTime.m_dt = pInfo->fProcessBeginTime;
				COleDateTimeSpan span = COleDateTime::GetCurrentTime() - oProcessBeginTime;
				if (span.GetTotalSeconds() > Max_Wait_Blocked_Cloud_Doc_Time_S)
				{
					BOOL bRet = CCommonFun::TerminaPID(dwProcessID);
					theLog.Write("##CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,5,文档处理超时(%d秒),ProcessBeginTime=%s,TerminaPID,bRet=%d,dwProcessID=%d,szExeFile=%s"
						, Max_Wait_Blocked_Cloud_Doc_Time_S, oProcessBeginTime.Format("%Y-%m-%d %H:%M:%S"), bRet, dwProcessID, pInfo->oProcessEntry32.szExeFile);
					delete pInfo;
					m_oCloudJobDealProcessInfoMap.RemoveKey(dwProcessID);
				}
			}
		}
	}

	//theLog.Write("CCloudDocDealHelperThread::OnCheckBlockedCloudDoc,end,count=%d", m_oCloudJobDealProcessInfoMap.GetCount());
}

void CCloudDocDealHelperThread::ReleaseCloudJobDealProcessInfo()
{
	theLog.Write("CCloudDocDealHelperThread::ReleaseCloudJobDealProcessInfo,count=%d", m_oCloudJobDealProcessInfoMap.GetCount());

	POSITION pos = m_oCloudJobDealProcessInfoMap.GetStartPosition();
	DWORD  dwProcessID = 0;	//进程ID
	CloudJobDealProcessInfo* pInfo = NULL;	//进程信息
	while (pos != NULL)
	{
		m_oCloudJobDealProcessInfoMap.GetNextAssoc(pos, dwProcessID, pInfo);
		if (pInfo)
		{
			delete pInfo;
		}
	}
	m_oCloudJobDealProcessInfoMap.RemoveAll();
}

#if 0
BOOL CALLBACK EnumWindowsFunc(HWND hWnd, LPARAM lParam)
{
	if (IsWindowVisible(hWnd))
	{
		CCloudDocDealHelperThread* pThis = (CCloudDocDealHelperThread*)lParam;
		if (!pThis)
		{
			theLog.Write("!!EnumWindowsFunc fail,pThis=%p", pThis);
			return FALSE;
		}

		pThis->AddWindowProcessInfo(hWnd);
	}

	return TRUE;
}
#endif

void CCloudDocDealHelperThread::GetAllUiProcess()
{
	//清理窗口进程信息
	ReleaseWindowProcessInfo();

	//重新获取窗口进程信息
#if 0
	if (!EnumWindows(EnumWindowsFunc, this))
	{
		theLog.Write("!!CCloudDocDealHelperThread::GetAllUiProcess,EnumWindows fail,err=%u", GetLastError());
	}
#else
	//1.先获得桌面窗口
	CWnd* pDesktopWnd = CWnd::GetDesktopWindow();
	if (!pDesktopWnd)
	{
		theLog.Write("!!CCloudDocDealHelperThread::GetAllUiProcess,GetDesktopWindow fail,err=%u", GetLastError());
		return;
	}
	//2.获得一个子窗口
	CWnd* pWnd = pDesktopWnd->GetWindow(GW_CHILD);
	//3.循环取得桌面下的所有子窗口
	while(pWnd != NULL)
	{
		if (pWnd->IsWindowVisible())
		{
			AddWindowProcessInfo(pWnd->m_hWnd);
		}

		//继续下一个子窗口
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
#endif
}

BOOL CCloudDocDealHelperThread::IsUiProcess(DWORD dwProcessID)
{
	WindowProcessInfo* pInfo = NULL;	//窗口进程信息
	if (m_oWindowProcessInfo.Lookup(dwProcessID, pInfo))
	{
		return TRUE;
		//同一个进程可能有多个UI窗口，但是线程ID可能不相同，暂时不处理，只保存一个信息
	}
	else
	{
		return FALSE;
	}
}

void CCloudDocDealHelperThread::ReleaseWindowProcessInfo()
{
	//theLog.Write("CCloudDocDealHelperThread::ReleaseWindowProcessInfo,count=%d", m_oWindowProcessInfo.GetCount());

	POSITION pos = m_oWindowProcessInfo.GetStartPosition();
	DWORD  dwProcessID = 0;	//进程ID
	WindowProcessInfo* pInfo = NULL;	//窗口进程信息
	while (pos != NULL)
	{
		m_oWindowProcessInfo.GetNextAssoc(pos, dwProcessID, pInfo);
		if (pInfo)
		{
			delete pInfo;
		}
	}
	m_oWindowProcessInfo.RemoveAll();
}

void CCloudDocDealHelperThread::AddWindowProcessInfo(HWND hWnd)
{
	WindowProcessInfo* pInfo = new WindowProcessInfo;
	memset(pInfo, 0x0, sizeof(WindowProcessInfo));

	//设置窗口句柄
	pInfo->hWnd = hWnd;

	//获取窗口的线程ID和进程ID
	pInfo->dwThreadID = GetWindowThreadProcessId(hWnd, &pInfo->dwProcessID);

	//获取窗口类名
	GetClassName(hWnd, pInfo->cClassName, sizeof(pInfo->cClassName));

	//获取窗口标题
	GetWindowText(hWnd, pInfo->cWindowText, sizeof(pInfo->cWindowText));

	{
		WindowProcessInfo* pWindows = NULL;	//窗口进程信息
		if (m_oWindowProcessInfo.Lookup(pInfo->dwProcessID, pWindows))
		{
			//同一个进程可能有多个UI窗口，但是线程ID可能不相同，暂时不处理，只保存一个信息
		}
		else
		{
			m_oWindowProcessInfo.SetAt(pInfo->dwProcessID, pInfo);
		}
	}
}

PWindowProcessInfo CCloudDocDealHelperThread::GetWindowProcessInfo(DWORD dwProcessID)
{
	WindowProcessInfo* pInfo = NULL;	//窗口进程信息
	m_oWindowProcessInfo.Lookup(dwProcessID, pInfo);
	return pInfo;
}

void CCloudDocDealHelperThread::SimulationKeyboard(HWND hWnd, int nVK)
{
	//模拟Esc按键
	//VK_RETURN,VK_ESCAPE
	::PostMessage(hWnd, WM_KEYDOWN, nVK, 0);
	::PostMessage(hWnd, WM_KEYUP, nVK, 0);	
}
