// CloudDocDealThread.cpp : 实现文件
//

#include "stdafx.h"
#include "CloudDocDealThread.h"

IMPLEMENT_DYNCREATE(CCloudDocDealThread, CWinThread)

CCloudDocDealThread::CCloudDocDealThread()
{
	m_nCloudDocDealTimer = NULL;
	m_nCheckDbConnTimer = NULL;
	m_pOwner = NULL;
	SetActive(FALSE);
}

CCloudDocDealThread::~CCloudDocDealThread()
{
}

BOOL CCloudDocDealThread::InitInstance()
{
	theLog.Write("CCloudDocDealThread::InitInstance, m_nThreadID=%u", m_nThreadID);
	EnableCheckCloudDocDeal(TRUE);
	EnableCheckDbConn(TRUE);
	return TRUE;
}

int CCloudDocDealThread::ExitInstance()
{
	theLog.Write("CCloudDocDealThread::ExitInstance, m_nThreadID=%u", m_nThreadID);
	SetActive(FALSE);
	EnableCheckCloudDocDeal(FALSE);
	EnableCheckDbConn(FALSE);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCloudDocDealThread, CWinThread)
END_MESSAGE_MAP()


//结束线程
void CCloudDocDealThread::OnEndTh(WPARAM wp, LPARAM lp)
{
	theLog.Write("CCloudDocDealThread::OnEndTh,m_nThreadID=%u", m_nThreadID);
	ExitInstance();
	AfxEndThread(0);
}

BOOL CCloudDocDealThread::PreTranslateMessage(MSG* pMsg) 
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

void CCloudDocDealThread::OnTimer(WPARAM wp, LPARAM lp)
{
	if (wp == m_nCloudDocDealTimer)
	{
		EnableCheckCloudDocDeal(FALSE);
		if (IsActive())
		{
			CheckCloudDocDeal();
		}
		else
		{
			theLog.Write("##CCloudDocDealThread::OnTimer,IsActive false");
		}
		EnableCheckCloudDocDeal(TRUE);
	}
	else if (wp == m_nCheckDbConnTimer)
	{
		EnableCheckDbConn(FALSE);
		CDbOperateHelper helper;
		if (!helper.CheckDbConn())
		{
			theLog.Write("!!CCloudDocDealThread::OnTimer,1,CheckDbConn fail");
#if 0	//可以不需要重连，ado有自动重连的机制。
			if (!helper.ReConnDb())
			{
				theLog.Write("!!CCloudDocDealThread::OnTimer,2,ReConnDb fail");
			}
#endif
		}
		EnableCheckDbConn(TRUE);
	}
}

void CCloudDocDealThread::EnableCheckCloudDocDeal(BOOL bEnable)
{
	//theLog.Write("CCloudDocDealThread::EnableCheckCloudDocDeal,bEnable=%d,m_nCloudDocDealTimer=%u", bEnable, m_nCloudDocDealTimer);
	if (bEnable)
	{
		if (!m_nCloudDocDealTimer)
		{
			m_nCloudDocDealTimer = ::SetTimer(NULL,0,Check_Cloud_Doc_Deal_TimeOut_MS,NULL);
		}
	}
	else
	{
		if (m_nCloudDocDealTimer)
		{
			KillTimer(NULL, m_nCloudDocDealTimer);
			m_nCloudDocDealTimer = NULL;
		}
	}
}

void CCloudDocDealThread::EnableCheckDbConn(BOOL bEnable)
{
	//theLog.Write("CCloudDocDealThread::EnableCheckDbConn,bEnable=%d,m_nCheckDbConnTimer=%u", bEnable, m_nCheckDbConnTimer);
	if (bEnable)
	{
		if (!m_nCheckDbConnTimer)
		{
			m_nCheckDbConnTimer = ::SetTimer(NULL,0,Check_Cloud_Db_Conn_TimeOut_MS,NULL);
		}
	}
	else
	{
		if (m_nCheckDbConnTimer)
		{
			KillTimer(NULL, m_nCheckDbConnTimer);
			m_nCheckDbConnTimer = NULL;
		}
	}
}

void CCloudDocDealThread::CheckCloudDocDeal()
{
// 	theLog.Write("CCloudDocDealThread::CheckCloudDocDeal, begin, m_nThreadID=%d", m_nThreadID);
	if (m_pOwner)
	{
		PCloudJobInfo pJob = m_pOwner->GetOneCloudJob();
		if (pJob)
		{
			theLog.Write("CCloudDocDealThread::CheckCloudDocDeal,2,m_nThreadID=%d,"
				"nJobId=%d, nUserId=%d, nEntId=%d, nFileState=%d, cDocName=%s, cJobDir=%s, cOrgJobName=%s, nJobState=%d, cPageType=%s, nPaperSize=%d, "
				"nColor=%d, nCopies=%d, nDoublePrint=%d, nDocSize=%d, nOrgJobSize=%d, nOrientation=%d, nPaperSource=%d, nTotalPage=%d, fSubmitTime=%s"
				, m_nThreadID
				, pJob->nJobId, pJob->nUserId, pJob->nEntId, pJob->nFileState, pJob->cDocName, pJob->cJobDir, pJob->cOrgJobName, pJob->nJobState
				, pJob->cPageType, pJob->nPaperSize, pJob->nColor, pJob->nCopies, pJob->nDoublePrint, pJob->nDocSize, pJob->nOrgJobSize
				, pJob->nOrientation, pJob->nPaperSource, pJob->nTotalPage, GetTimeStr(pJob->fSubmitTime));

			CCloudDocDeal deal(pJob);
			if (!deal.ConvertFile())
			{
				pJob->nJobState = JOB_CLOUD_DEAL_FAIL;
				theLog.Write("!!CCloudDocDealThread::CheckCloudDocDeal,3,ConvertFile fail, m_nThreadID=%d", m_nThreadID);
			}
			else
			{
				pJob->nJobState = JOB_CLOUD_DEAL_SUCC;
			}

			CDbOperateHelper helper;
			if (!helper.UpdateCloudJob(pJob))
			{
				theLog.Write("!!CCloudDocDealThread::CheckCloudDocDeal,4,UpdateCloudJob fail, m_nThreadID=%d", m_nThreadID);
			}

			//释放内存
			deal.Release();
		}
	}
// 	theLog.Write("CCloudDocDealThread::CheckCloudDocDeal, end, m_nThreadID=%d", m_nThreadID);
}

void CCloudDocDealThread::SetOwner(CCloudDocDealMgr* pOwner)
{
	m_pOwner = pOwner;
	if (!m_pOwner)
	{
		theLog.Write("!!CCloudDocDealThread::SetOwner,m_pOwner=%p", m_pOwner);
	}
}

void CCloudDocDealThread::SetActive(BOOL bActive)
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

BOOL CCloudDocDealThread::IsActive()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lActiveDealDoc);
	return lVal;
}

CString CCloudDocDealThread::GetTimeStr(double fTime)
{
	COleDateTime time;
	time.m_dt = fTime;
	return time.Format("%Y-%m-%d %H:%M:%S");
}
