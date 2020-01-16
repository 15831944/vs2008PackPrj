// WorkThread.cpp : 实现文件
//

#include "stdafx.h"
#include "IsecSyncDingTalk.h"
#include "WorkThread.h"


// CWorkThread

IMPLEMENT_DYNCREATE(CWorkThread, CWinThread)

CWorkThread::CWorkThread()
{
}

CWorkThread::~CWorkThread()
{
}

BOOL CWorkThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	// TODO: 在此执行任意逐线程初始化
	theLog.Write("CWorkThread::InitInstance,1");
	CThreadMgr::PostThreadMsg(m_nThreadID, WM_MSG_QUE_OK, 0, NULL);
	return TRUE;
	return TRUE;
}

int CWorkThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

void CWorkThread::SetHandleOfMsgQueOK( HANDLE hThMsgQueOK )
{
	m_hThMsgQueOK = hThMsgQueOK;
}

void CWorkThread::OnEndTh(WPARAM wp, LPARAM lp)
{
	theLog.Write("CWorkThread::OnEndTh");
	ExitInstance();
	AfxEndThread(0);
}

void CWorkThread::OnMsgQueOK( WPARAM wp, LPARAM lp )
{
	if(m_hThMsgQueOK)
	{
		::SetEvent(m_hThMsgQueOK);
		m_hThMsgQueOK = NULL;
	}
}

void CWorkThread::KillThis()
{
	theLog.Write("CWorkThread::KillThis,1,begin");
	BOOL bRet = CThreadMgr::PostThreadMsg(m_nThreadID, WM_THREAD_EXIT, 0, 0);
	theLog.Write("CWorkThread::KillThis,2,begin,bRet=%d", bRet);
}
void CWorkThread::OnStartSyncDingTalk(WPARAM wp,LPARAM lp)
{
	CString szCorpId = (char*)wp;
	CString szCorpSecret = (char*)lp;
	theLog.Write("CWorkThread::OnStartSyncDingTalk,szCorpId=%s,szCorpSecret=%s",szCorpId,szCorpSecret);
	CDingDingTalking dingtaking;
	dingtaking.InitDingTalking(szCorpId,szCorpSecret);
	Json::Value jsonEntList;
	Json::Value jsonDeptList;
	Json::Value jsonUserList;
	Json::Value jsonAll;
	if (!dingtaking.GetDingTalkingAllInfoList(jsonEntList,jsonDeptList,jsonUserList))
	{
		theLog.Write("!!CWorkThread::OnStartSyncDingTalk,1");
		if (g_pIsecSyncDingTalk)
		{
			g_pIsecSyncDingTalk->OntTipMessageBox("！！获取同步信息失败");
			return;
		}
	}
	else
	{
		if (g_pIsecSyncDingTalk)
		{
			g_pIsecSyncDingTalk->OnUiLog("获取同步信息成功");
		}
		g_pIsecSyncDingTalk->OnUiLog("正在同步");
		if (g_webservice.synOrgArchiteDingd(jsonEntList,jsonDeptList,jsonUserList))
		{
			g_pIsecSyncDingTalk->OnUiLog("同步流程结束");
		}
		else
		{
			g_pIsecSyncDingTalk->OnUiLog("同步失败");
			return;
		}
	}
	
	//theLog.Write("============jsonAll=%s",jsonAll.toStyledString().c_str());
}


BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
END_MESSAGE_MAP()


// CWorkThread 消息处理程序

BOOL CWorkThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(pMsg->message)
	{
	case WM_SYNC_DING_TALK:
		{
			if (g_pIsecSyncDingTalk)
			{
				g_pIsecSyncDingTalk->pushButton_start->setEnabled(FALSE);
			}
			OnStartSyncDingTalk(pMsg->wParam,pMsg->lParam);
			if (g_pIsecSyncDingTalk)
			{
				g_pIsecSyncDingTalk->pushButton_start->setEnabled(TRUE);
			}
		}
		break;
	case WM_MSG_QUE_OK:
		{
			OnMsgQueOK(pMsg->wParam, pMsg->lParam);
		}
		break;
	case WM_THREAD_EXIT:
		{
			OnEndTh(pMsg->wParam,pMsg->lParam);
			return TRUE;
		}
		break;
	default:
		break;
	}


	return CWinThread::PreTranslateMessage(pMsg);
}
