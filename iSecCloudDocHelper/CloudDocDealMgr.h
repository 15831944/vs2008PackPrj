#pragma once

class CCloudDocDealMgr
{
public:
	CCloudDocDealMgr(void);
	~CCloudDocDealMgr(void);

	void InitWork();
	void Release();
	void CheckNeedDealCloudJob();
	void AddOneCloudJob(PCloudJobInfo job);
	PCloudJobInfo GetOneCloudJob();
	DWORD GetCloudJobCount();

protected:
	void InitTh();
	void ReleaseJob();
	void ReleaseTh();

protected:
	CList<CCloudDocDealThread*> m_oListTh;
	CCriticalSection2 m_cs4ListTh;
	CListCloudJobInfo m_oListJob;
	CCriticalSection2 m_cs4ListJob;
};
