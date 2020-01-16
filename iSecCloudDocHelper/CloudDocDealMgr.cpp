#include "StdAfx.h"
#include "CloudDocDealMgr.h"

CCloudDocDealMgr::CCloudDocDealMgr(void)
{
}

CCloudDocDealMgr::~CCloudDocDealMgr(void)
{
}

void CCloudDocDealMgr::InitWork()
{
	Release();
	InitTh();
}

void CCloudDocDealMgr::Release()
{
	ReleaseJob();
	ReleaseTh();
}

void CCloudDocDealMgr::CheckNeedDealCloudJob()
{
	//theLog.Write("CCloudDocDealMgr::CheckNeedDealCloudJob,begin");
	
	DWORD dwCloudJobCount = GetCloudJobCount();
	if (dwCloudJobCount > Max_Wait_Cloud_Doc_Deal_Count)
	{
		theLog.Write("##CCloudDocDealMgr::CheckNeedDealCloudJob,dwCloudJobCount=%d,Max_Wait_Cloud_Doc_Deal_Count=%d"
			, dwCloudJobCount, Max_Wait_Cloud_Doc_Deal_Count);
		return;
	}

	CDbOperateHelper helper;
	CListCloudJobInfo list;
	if (helper.GetNeedDealCloudJob(list))
	{
		theLog.Write("CCloudDocDealMgr::CheckNeedDealCloudJob,2,GetNeedDealCloudJob count=%d", list.GetCount());
		POSITION pos = list.GetHeadPosition();
		for (int i = 0; i < list.GetCount(); i++)
		{
			PCloudJobInfo pJob = (PCloudJobInfo)list.GetNext(pos);
			AddOneCloudJob(pJob);
		}  

	}
	//theLog.Write("CCloudDocDealMgr::CheckNeedDealCloudJob,end");
}

void CCloudDocDealMgr::AddOneCloudJob(PCloudJobInfo job)
{
	CCriticalSection2::Owner ow(m_cs4ListJob);

	if (job)
	{
		if (m_oListJob.GetCount() < Max_Wait_Cloud_Doc_Deal_Count)
		{
			m_oListJob.AddTail(job);

			//设置云打印作业状态,
			CDbOperateHelper helper;
			helper.UpdateCloudJobState(job, JOB_CLOUD_DEAL_PENDING);
		}
	}
	else
	{
		theLog.Write("!!CCloudDocDealMgr::AddOneCloudJob,job=%p", job);
	}
}

PCloudJobInfo CCloudDocDealMgr::GetOneCloudJob()
{
	CCriticalSection2::Owner ow(m_cs4ListJob);

	PCloudJobInfo job = NULL;
	if (m_oListJob.GetCount()>0)
	{
		job = m_oListJob.RemoveHead();	//返回头结点，并且删除头结点
	}
	if (job)
	{
		CDbOperateHelper helper;
		helper.UpdateCloudJobState(job, JOB_CLOUD_DEALING);
	}
	return job;
}

DWORD CCloudDocDealMgr::GetCloudJobCount()
{
	CCriticalSection2::Owner ow(m_cs4ListJob);
	DWORD dwCloudJobCount = m_oListJob.GetCount();
	return dwCloudJobCount;
}

void CCloudDocDealMgr::InitTh()
{
	CCriticalSection2::Owner ow(m_cs4ListTh);

	SYSTEM_INFO si;
	memset(&si, 0x0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&si);
#ifdef _DEBUG
	DWORD dwNumberOfProcessors = 2;
#else
	DWORD dwNumberOfProcessors = si.dwNumberOfProcessors;
	if (dwNumberOfProcessors < Min_Cloud_Doc_Deal_Thread_Count)
	{
		theLog.Write("##CCloudDocDealMgr::InitTh,1.B,dwNumberOfProcessors=%d,Min_Cloud_Doc_Deal_Thread_Count"
			, dwNumberOfProcessors, Min_Cloud_Doc_Deal_Thread_Count);
		dwNumberOfProcessors = Min_Cloud_Doc_Deal_Thread_Count;
	}
#endif
	DWORD dwProcessThreads = dwNumberOfProcessors * 2;	//转换线程数为处理器核心数的2倍。
	theLog.Write("CCloudDocDealMgr::InitTh,1,dwNumberOfProcessors=%d,dwProcessThreads=%d", dwNumberOfProcessors, dwProcessThreads);
	for (DWORD i=0; i<dwProcessThreads; i++)
	{
		CCloudDocDealThread* pTh = (CCloudDocDealThread*)::AfxBeginThread(RUNTIME_CLASS(CCloudDocDealThread),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if (pTh)
		{
			pTh->SetOwner(this);
			pTh->SetActive(TRUE);
			m_oListTh.AddTail(pTh);
			DWORD dwRet = pTh->ResumeThread();
			if (dwRet == 0xFFFFFFFF)
			{
				theLog.Write("!!CCloudDocDealMgr::InitTh,2,i=%d,pTh=%p,dwRet=0x%X,err=%u", i, pTh, dwRet, GetLastError());
			}
		}
		else
		{
			theLog.Write("!!CCloudDocDealMgr::InitTh,3,i=%d,pTh=%p,err=%u", i, pTh, GetLastError());
		}
	}
}

void CCloudDocDealMgr::ReleaseJob()
{
	CCriticalSection2::Owner ow(m_cs4ListJob);

	theLog.Write("CCloudDocDealMgr::ReleaseJob,1,count=%d", m_oListJob.GetCount());

	POSITION pos = m_oListJob.GetHeadPosition();
	for (int i = 0; i < m_oListJob.GetCount(); i++)
	{
		PCloudJobInfo pJob = (PCloudJobInfo)m_oListJob.GetNext(pos);
		if (pJob)
		{
			//设置云打印作业状态,
			CDbOperateHelper helper;
			helper.UpdateCloudJobState(pJob, JOB_CLOUD_DEAL_WAIT);

			delete pJob;
			pJob = NULL;
		}
	}  
	m_oListJob.RemoveAll();
}

void CCloudDocDealMgr::ReleaseTh()
{
	CCriticalSection2::Owner ow(m_cs4ListTh);

	theLog.Write("CCloudDocDealMgr::ReleaseTh,1,count=%d", m_oListTh.GetCount());

	POSITION pos = m_oListTh.GetHeadPosition();
	for (int i = 0; i < m_oListTh.GetCount(); i++)
	{
		CCloudDocDealThread* pTh = (CCloudDocDealThread*)m_oListTh.GetNext(pos);
		if (pTh)
		{
			CThreadMgr::DestroyThread(pTh, INFINITE);
			pTh = NULL;
		}
	}  
	m_oListTh.RemoveAll();
}
