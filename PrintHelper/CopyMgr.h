#pragma once

#include "SerialCommHelper.h"
#include <afxtempl.h>
#include "SimpleCry.h"

class CCopyInfo
{
public:
	CCopyInfo()
	{
		m_dt = 0;//ʱ��
		m_lCount = 0;//
		m_nPrice = 0;
		m_nShouldMoney = 0;
		m_nPayMoney = 0;
		m_eColor = WB_PAPER;
		m_ePaper = A4_PAPER;
		SYSTEMTIME st = {0};
		::GetLocalTime(&st);
		COleDateTime dt(st);
		m_dt = dt.m_dt;
		m_eFrom = CIF_HardWare;
		m_nJobid = 0;
	}
	double m_dt;
	long m_lCount;
	int m_nPrice;
	int m_nShouldMoney;
	int m_nPayMoney;
	PAPER_COLOR m_eColor;
	PAPER_TYPE m_ePaper;
	COPY_INFO_FROM m_eFrom;
	int m_nJobid;
};

enum USER_STATE
{
	USER_ERR,
	USER_SUC
	
};


class _AA_DLL_EXPORT_ CCopyStat : public ICopyCmdInterface
{
public:
	CCopyStat();
	~CCopyStat();
private:
	void Reset();
	CSerialCommHelper m_oHelper;
	//CList<CCopyInfo*,CCopyInfo*> m_oCopyList;
	CMap<UINT64,UINT64,CCopyInfo*,CCopyInfo*>	m_oCopyMap;
	long m_lUserLogonedSucTickCount;
	long m_lUserLogoned;
	long m_lInitSuc;
	long m_lActive;
	long m_lLogonTimeout;
	long m_lDelayTime;
	int m_nComIndex;
	long m_lTickCount;
	PAPER_TYPE m_eActiveType;
	PAPER_COLOR m_eActiveColor;
	long m_lBeginOnePaper;
	long m_lCmdResponse;
	long m_lCheckTickCount;
	long m_lLocked;
	long m_lLastNotifyTickCount;
	long m_nCopySpan;
	long m_lPowered;	//����״̬

	ICopyChargeInterface* m_pCallback;
	CRITICAL_SECTION m_csLock;

	inline void		InitLock()			{InitializeCriticalSection (&m_csLock );}
	inline void		DelLock()				{DeleteCriticalSection (&m_csLock );}
	
	HANDLE	m_hThreadTerm ;
	HANDLE	m_hThread;
	HANDLE	m_hThreadStarted;

	BYTE	m_szBrandCode[4];
	BYTE	m_szCheckCode[4];
	BYTE	m_szLockCode[4];
	BYTE	m_szUnLockCode[4];
	BYTE	m_szCheckHw[4];
	BYTE	m_szPowerOnCode[4];		//����
	BYTE	m_szPowerDownCode[4];	//ͣ��
	BRAND_TYPE m_eType;
protected:
	void OnInitSuc();
	void OnCheckStateSuc();
	void OnLockSuc(BOOL bLocked);
	void OnPowerSuc(BOOL bPowered);

	void OnPaperType(PAPER_TYPE e);
	void OnPaperColor(PAPER_COLOR e);
	void OnBeginOneCopy();
	void OnOneCopyCmplt();
	virtual void OnOneCopyCmplt(PAPER_TYPE,PAPER_COLOR);
	void OnErr();
	

	void CreateTh();
	
	void NowTickCount();
	BOOL IsIdleTimeOut();
	BOOL IsCmdResponseTimeOut();
	
	BOOL IsSameUser(void *pInfo,int nSize);
	BOOL IsCopying();
	
	void CheckState(BOOL& bSend);
	void InitHw();
	void LockHw(BOOL bLock);
	void OnIdleTimeout();
	int GetPrice(PAPER_TYPE eType,PAPER_COLOR eColor);
	static unsigned __stdcall ThreadFn(void*pvParam);

	
	void SendKey(BOOL& bSend);
	void SendData(BOOL& bSend);
	BYTE m_xor;
	BYTE m_add;
	//�ͷŸ�ӡ��¼
	void ReleaseList();
	void NotifyCopyData();
	BOOL IsCanNotify();
	//�ӳ�д�����ݵ���ӡ������
	void DelayWrite(DWORD dwMilliseconds = 500);
public:
	BOOL IsRunning();
	BOOL IsInited();
	BOOL IsLocked();
	BOOL CheckHw();	//add by zxl,20140923,��⸴ӡ�������Ƿ�����
	//һ��ͨˢ���ӿڣ�ÿ�ζ����ô˽ӿڣ�
	//
	//void CheckUser(void *pInfo,int nSize);
	//��ʼ��ӡ
	void LogonUser();
	//��ӡ����
	void LogoffUser();
	//�û�����ʹ��
	BOOL IsUserLogoned();
	//�û���¼�ɹ�
	BOOL IsUserLogonedSuc();
	//�Ƿ񹩵�
	BOOL IsPowered();
	//�������
	void PowerHw(BOOL bPower);
	//����ϼƻ���
	void CalcAmount(float& fAmount);
	//
	void SetCallback(ICopyChargeInterface* pCall);
	void SetIdleDelay(long l);
	BOOL Init(long lIdle,int nComIndex);
	void Shutdown();

	//����Ʒ��
	void InitBrand(BRAND_TYPE eType);

	//���ÿ���ʱ��,������ڵ�ɨ��Ҳ��Ҫ�����Ĵ�ӡ��������
	void ReSetTimeCount();
};