// WorkThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	theLog.Write("CWorkThread::InitInstance,1");
	CThreadMgr::PostThreadMsg(m_nThreadID, WM_MSG_QUE_OK, 0, NULL);
	return TRUE;
	return TRUE;
}

int CWorkThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
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
			g_pIsecSyncDingTalk->OntTipMessageBox("������ȡͬ����Ϣʧ��");
			return;
		}
	}
	else
	{
		if (g_pIsecSyncDingTalk)
		{
			g_pIsecSyncDingTalk->OnUiLog("��ȡͬ����Ϣ�ɹ�");
		}
		g_pIsecSyncDingTalk->OnUiLog("����ͬ��");
		if (g_webservice.synOrgArchiteDingd(jsonEntList,jsonDeptList,jsonUserList))
		{
			g_pIsecSyncDingTalk->OnUiLog("ͬ�����̽���");
		}
		else
		{
			g_pIsecSyncDingTalk->OnUiLog("ͬ��ʧ��");
			return;
		}
	}
	
	//theLog.Write("============jsonAll=%s",jsonAll.toStyledString().c_str());
}


BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
END_MESSAGE_MAP()


// CWorkThread ��Ϣ�������

BOOL CWorkThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
