#include "stdafx.h"
#include "CopyMgr.h"
#include <Process.h>

CCopyStat::CCopyStat()
{
	InitLock();
	SetIdleDelay(90);
	NowTickCount();
	m_eActiveColor = WB_PAPER;
	m_eActiveType = A4_PAPER;
	m_lLogonTimeout = 1000;
	InterlockedExchange(&m_lUserLogoned,0);
	InterlockedExchange(&m_lInitSuc,0);
	InterlockedExchange(&m_lActive,0);
	InterlockedExchange(&m_lBeginOnePaper,0);
	InterlockedExchange(&m_lCmdResponse,0);

	InterlockedExchange(&m_lCheckTickCount,0);
	
	InterlockedExchange(&m_lLocked,1);
	InterlockedExchange(&m_lPowered,1);

	m_nCopySpan = 5000;
	InterlockedExchange(&m_lLastNotifyTickCount,0);

	m_pCallback = 0;
	m_hThreadTerm = 0;
	m_hThread = 0;
	m_hThreadStarted = 0;
	
	m_szCheckCode[0] = 0xA0;
	m_szCheckCode[1] = 0xF1;
	m_szCheckCode[2] = 0x00;
	m_szCheckCode[3] = 0xC0;

	m_szCheckHw[0] = 0xA0;
	m_szCheckHw[1] = 0xA1;
	m_szCheckHw[2] = 0xA2;
	m_szCheckHw[3] = 0xC0;

	m_szLockCode[0] = 0xA0;
	m_szLockCode[1] = 0xF2;
	m_szLockCode[2] = 0x01;
	m_szLockCode[3] = 0xC0;

	m_szUnLockCode[0] = 0xA0;
	m_szUnLockCode[1] = 0xF2;
	m_szUnLockCode[2] = 0x00;
	m_szUnLockCode[3] = 0xC0;

	m_szPowerOnCode[0] = 0xA0;
	m_szPowerOnCode[1] = 0xF4;
	m_szPowerOnCode[2] = 0x00;
	m_szPowerOnCode[3] = 0xC0;

	m_szPowerDownCode[0] = 0xA0;
	m_szPowerDownCode[1] = 0xF4;
	m_szPowerDownCode[2] = 0x01;
	m_szPowerDownCode[3] = 0xC0;

	//品牌，施乐，东芝
#if 0
	//施乐
	m_szBrandCode[0] = 0xA0;
	m_szBrandCode[1] = 0xF0;
	m_szBrandCode[2] = 0x00;
	m_szBrandCode[3] = 0xC0;
#else
	//东芝
	m_szBrandCode[0] = 0xA0;
	m_szBrandCode[1] = 0xF0;
	m_szBrandCode[2] = 0x01;
	m_szBrandCode[3] = 0xC0;
#endif
	m_xor = 0;
	m_add = 0;
	m_nComIndex = 0;
}

CCopyStat::~CCopyStat()
{
	ReleaseList();
	Shutdown();
	DelLock();
}

void CCopyStat::Reset()
{
// 	Owner ow(m_csLock);
	SetIdleDelay(90);
	NowTickCount();
	m_eActiveColor = WB_PAPER;
	m_eActiveType = A4_PAPER;
}

void CCopyStat::InitBrand(BRAND_TYPE eType)
{
	theLog.Write("CCopyStat::InitBrand,eType=%d",eType);
	m_eType = eType;
	//CopyMemory(m_szBrandCode,pBuf,nLen);
	m_szBrandCode[0] = 0xA0;
	m_szBrandCode[1] = 0xF0;
	m_szBrandCode[2] = 0x00;
	m_szBrandCode[3] = 0xC0;
	switch (eType)
	{
	case BT_SL:
	case BT_SL2:
		m_szBrandCode[2] = 0x00;
		break;
	case BT_DZ:
		m_szBrandCode[2] = 0x01;
		break;
	case BT_KM:
		m_szBrandCode[2] = 0x02;
		break;
	case BT_LG:
		m_szBrandCode[2] = 0x03;
		break;
	case BT_JC:
		m_szBrandCode[2] = 0x04;
		break;
	case BT_ZD:
		m_szBrandCode[2] = 0x05;
		break;
	case BT_XP:
		m_szBrandCode[2] = 0x06;
		break;
	case BT_SX:
		m_szBrandCode[2] = 0x07;
		break;
	case BT_JT:
		m_szBrandCode[2] = 0x08;
		break;
	case BT_HP:
		m_szBrandCode[2] = 0x09;
		break;
	default:
		m_szBrandCode[2] = 0x00;
		break;
	}

	m_oHelper.InitBrand(eType);
}

BOOL CCopyStat::Init(long lIdle,int nComIndex)
{
	theLog.Write("CCopyStat::Init,m_eType=%d,lIdle=%d,nComIndex=%d", m_eType, lIdle, nComIndex);
	if (m_eType == BT_LM)
	{
		return TRUE;
	}

	m_nComIndex = nComIndex;

	if(nComIndex <= 0)
	{
		theLog.Write("CCopyStat::Init,nComIndex == %d,没有复印控制器。", m_nComIndex);
		return TRUE;
	}

	Reset();
	SetIdleDelay(lIdle);
	if (!m_hThread)
	{
		//to do
		//zhandb
		if(!m_oHelper.Init2(nComIndex,this))
		{
			theLog.Write("!!CCopyStat::Init,1, m_oHelper.Init2, fail, nComIndex=%d",nComIndex);
			return FALSE;
		}
		//初始化
		InitHw();

		BOOL bSuc = FALSE;
		int nIndex = 3;
		while (nIndex > 0)
		{
			if(IsInited())
			{
				CreateTh();
				bSuc = TRUE;
				break;
			}
			//
			::Sleep(1000);
			InitHw();
			
			nIndex --;
		}
		if (!bSuc)
		{
			theLog.Write("!!CCopyStat::Init,2, fail, goto m_oHelper.UnInit");
			m_oHelper.UnInit();
		}
	}
	if (m_hThread)
	{
		return TRUE;
	}
	else
	{
		theLog.Write("!!CCopyStat::Init,3, m_hThread=0x%x", m_hThread);
		return FALSE;
	}
}

void CCopyStat::CreateTh()
{
	InterlockedExchange(&m_lActive,1);
	m_hThreadTerm = CreateEvent(0,0,0,0);
	m_hThreadStarted = CreateEvent(0,0,0,0);
	m_hThread			= 	(HANDLE)_beginthreadex(0,0,CCopyStat::ThreadFn,(void*)this,0,0 );
	
	DWORD dwWait = WaitForSingleObject ( m_hThreadStarted , INFINITE );
	
	ASSERT ( dwWait == WAIT_OBJECT_0 );
	
	CloseHandle(m_hThreadStarted);
	m_hThreadStarted = 0;
}
void CCopyStat::Shutdown()
{
	m_pCallback = NULL;
	m_oHelper.UnInit();
	InterlockedExchange(&m_lActive,0);
	if (m_hThread && m_hThreadTerm)
	{
		SetEvent(m_hThreadTerm);
		WaitForSingleObject(m_hThread,INFINITE);
		CloseHandle(m_hThread);
		CloseHandle(m_hThreadTerm);
		m_hThreadTerm = 0;
		m_hThread = 0;
	}
}

void CCopyStat::OnInitSuc()
{
// 	Owner ow(m_csLock);
	InterlockedExchange(&m_lInitSuc,1);
	InterlockedExchange(&m_lCmdResponse,0);
	//第一次收到时间戳
	long lVal = 0;
	InterlockedExchange(&lVal,m_lUserLogonedSucTickCount);
	if(lVal == 0)
	{
		InterlockedExchange(&m_lUserLogonedSucTickCount,GetTickCount());
		theLog.Write("first receive tc %d",m_lUserLogonedSucTickCount);
	}
}

void CCopyStat::OnCheckStateSuc()
{
// 	Owner ow(m_csLock);
	InterlockedExchange(&m_lInitSuc,1);
	//long lVal = InterlockedDecrement(&m_lCmdResponse);
	long lVal = InterlockedExchange(&m_lCmdResponse,0);
	theLog.Write("OnCheckStateSuc %d",lVal);
	//第一次收到时间戳
	lVal = 0;
	InterlockedExchange(&lVal,m_lUserLogonedSucTickCount);
	if(lVal == 0)
	{
		InterlockedExchange(&m_lUserLogonedSucTickCount,GetTickCount());
		theLog.Write("first receive tc %d",m_lUserLogonedSucTickCount);
	}
}

void CCopyStat::OnLockSuc(BOOL bLocked)
{
	InterlockedExchange(&m_lLocked, bLocked);
	theLog.Write("CCopyStat::OnLockSuc,bLocked=%d", bLocked);
}

void CCopyStat::OnPowerSuc(BOOL bPowered)
{
	InterlockedExchange(&m_lPowered, bPowered);
	theLog.Write("CCopyStat::OnPowerSuc,bPowered=%d", bPowered);
}

void CCopyStat::OnPaperType(PAPER_TYPE e)
{
// 	Owner ow(m_csLock);
	
	long lVal = 0;
	InterlockedExchange(&lVal,m_lBeginOnePaper);
	if (lVal == 1)
	{
		theLog.Write("开始复印，纸型变化丢弃");
		InterlockedIncrement(&m_lBeginOnePaper);
		return;
	}
	m_eActiveType = e;
	NowTickCount();	
}

void CCopyStat::OnPaperColor(PAPER_COLOR e)
{
// 	Owner ow(m_csLock);
	long lVal = 0;
	InterlockedExchange(&lVal,m_lBeginOnePaper);
	if (lVal == 1)
	{
		theLog.Write("开始复印，颜色变化丢弃");
		InterlockedIncrement(&m_lBeginOnePaper);
		return;
	}
	m_eActiveColor = e;
	NowTickCount();
	
}

double GetNowDateTime()
{
	COleDateTime dt = COleDateTime::GetCurrentTime();
	return dt.m_dt;
}

int CCopyStat::GetPrice(PAPER_TYPE eType,PAPER_COLOR eColor)
{
	return 0;
}

void CCopyStat::OnBeginOneCopy()
{
// 	Owner ow(m_csLock);
	NowTickCount();
	if(!IsInited())
	{
		theLog.Write("OnBeginOneCopy no logon");
		return ;
	}
	InterlockedExchange(&m_lBeginOnePaper,1);
}
void CCopyStat::OnOneCopyCmplt(PAPER_TYPE eType,PAPER_COLOR eColor)
{
	if(!IsInited())
	{
		theLog.Write("OnOneCopyCmplt no logon");
		return ;
	}
	
	if (!IsUserLogoned())
	{
		theLog.Write("OnOneCopyCmplt no user logon");
		return ;
	}

	if(!IsUserLogonedSuc())
	{
		theLog.Write("OnOneCopyCmplt user logon not ok");
		return ;
	}

	Owner ow(m_csLock);
	CCopyInfo* pVal = 0;
	UINT64 key = MAKE64(eColor,eType);
	if(m_oCopyMap.Lookup(key,pVal) && pVal)
	{
		pVal->m_lCount ++;
	}
	else
	{
		CCopyInfo* pInfo = new CCopyInfo();
		pInfo->m_eColor = eColor;
		pInfo->m_ePaper = eType;
		pInfo->m_lCount = 1;
		pInfo->m_dt = GetNowDateTime();
		pInfo->m_nPrice = GetPrice(eType,eColor);
		m_oCopyMap.SetAt(key,pInfo);
	}
	
/*
	if (m_pCallback)
	{
		m_pCallback->OnOneCopyCmplt(pInfo);
	}
	*/
//	delete pInfo;
	NowTickCount();
}

//重置空闲时间,这个是在当扫描也需要解锁的打印机上有用
void CCopyStat::ReSetTimeCount()
{
	NowTickCount();
}

void CCopyStat::OnOneCopyCmplt()
{
	if(!IsInited())
	{
		theLog.Write("OnOneCopyCmplt no logon");
		return ;
	}
	long lVal = 0;
	InterlockedExchange(&lVal,m_lBeginOnePaper);
	if (lVal == 0)
	{
		theLog.Write("paper not begin ");
		return ;
	}
	else if (lVal > 1)
	{
		theLog.Write("paper chaged");
		return ;
	}
	if (!IsUserLogoned())
	{
		theLog.Write("OnOneCopyCmplt no user logon");
		return ;
	}

	Owner ow(m_csLock);
	CCopyInfo* pVal = 0;
	UINT64 key = MAKE64(m_eActiveColor,m_eActiveType);
	if(m_oCopyMap.Lookup(key,pVal) && pVal)
	{
		pVal->m_lCount ++;
	}
	else
	{
		CCopyInfo* pInfo = new CCopyInfo();
		pInfo->m_eColor = m_eActiveColor;
		pInfo->m_ePaper = m_eActiveType;
		pInfo->m_lCount = 1;
		pInfo->m_dt = GetNowDateTime();
		pInfo->m_nPrice = GetPrice(m_eActiveType,m_eActiveColor);
		m_oCopyMap.SetAt(key,pInfo);
	}

	
	NowTickCount();

	InterlockedExchange(&m_lBeginOnePaper,0);
}

void CCopyStat::OnErr()
{
	theLog.Write("OnErr AA %x",(DWORD)m_pCallback);
	Owner ow(m_csLock);
	if (m_pCallback)
	{
		theLog.Write("OnErr BB %x",(DWORD)m_pCallback);
		if (IsInited())
		{
			m_pCallback->OnErr(0);
		}
	}	
}

void CCopyStat::SetIdleDelay(long l)
{
	InterlockedExchange(&m_lDelayTime,l);
}

void CCopyStat::NowTickCount()
{
// 	Owner ow(m_csLock);
	m_lTickCount = ::GetTickCount();
}

BOOL CCopyStat::IsCmdResponseTimeOut()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lCmdResponse);
	if (lVal > 10)
	{
		theLog.Write("IsCmdResponseTimeOut %d",lVal);
		InterlockedExchange(&m_lCmdResponse,0);
		return TRUE;
	}
	return FALSE;
}

BOOL CCopyStat::IsIdleTimeOut()
{
// 	Owner ow(m_csLock);

#if 1
	return FALSE;	//不要在这里设置超时,要在LmPrinterMgr或者printstation里面超时退出,
#endif

	DWORD dwNow = GetTickCount();
	if (m_pCallback)
	{
		m_pCallback->OnIdle((dwNow - m_lTickCount) / 1000);
	}
#if 0
	if (dwNow - m_lTickCount > m_lDelayTime * 1000)
#else
	DWORD dwTmp1 = m_lTickCount;
	DWORD dwLeft = dwNow - dwTmp1;
	DWORD dwRight = m_lDelayTime;
	dwRight *= 1000;
	if (dwLeft > dwRight)
#endif 
	{
		theLog.Write("=============CCopyStat::IsIdleTimeOut,dwNow=%d,m_lTickCount=%d,m_lDelayTime=%d",dwNow,m_lTickCount,m_lDelayTime);
		return TRUE;
	}
	return FALSE;
}

void CCopyStat::ReleaseList()
{
	Owner ow(m_csLock);
	UINT64 key = 0;
	CCopyInfo* pVal = 0;
	POSITION pos = m_oCopyMap.GetStartPosition();
	while (pos)
	{
		 m_oCopyMap.GetNextAssoc(pos,key,pVal);
		if (pVal)
		{
			delete pVal;
		}
	}
	m_oCopyMap.RemoveAll();
}

BOOL CCopyStat::IsCanNotify()
{
// 	Owner ow(m_csLock);
	long lVal = 0;
	long lNow = GetTickCount();
	InterlockedExchange(&lVal,m_lLastNotifyTickCount);
	if (((lNow - lVal) >= m_nCopySpan) && m_oCopyMap.GetCount() > 0)
	{
		theLog.Write("IsCanNotify: %d,%d,%d,%d",lNow,lVal,m_nCopySpan,m_oCopyMap.GetCount());
		return TRUE;
	}
	return FALSE;
}

void CCopyStat::NotifyCopyData()
{
	Owner ow(m_csLock);
	UINT64 key = 0;
	CCopyInfo* pVal = 0;
	POSITION pos = m_oCopyMap.GetStartPosition();
	while (pos)
	{
		m_oCopyMap.GetNextAssoc(pos,key,pVal);
		if (pVal)
		{
			if (m_pCallback)
			{
				m_pCallback->OnOneCopyCmplt(pVal);
				
			}
			delete pVal;
		}
	}
	m_oCopyMap.RemoveAll();

	DWORD dw = GetTickCount();
	InterlockedExchange(&m_lLastNotifyTickCount,dw);
}

void CCopyStat::CalcAmount(float& fAmount)
{
	Owner ow(m_csLock);
	fAmount = 0;
}

BOOL CCopyStat::IsCopying()
{
	return FALSE;
}
//初始化板子成功
BOOL CCopyStat::IsInited()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lInitSuc);
	return lVal;
}
BOOL CCopyStat::IsLocked()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lLocked);
	return lVal;
}
BOOL CCopyStat::IsPowered()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lPowered);
	return lVal;
}
BOOL CCopyStat::IsUserLogoned()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lUserLogoned);
	return lVal;
}
BOOL CCopyStat::IsUserLogonedSuc()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lUserLogonedSucTickCount);
	//theLog.Write("IsUserLogonedSuc %d,%d",lVal,GetTickCount() );
	if(lVal > 0)
	{
		if((GetTickCount() - lVal) < 2000)
		{
			theLog.Write("延迟没到2000ms，计数无效");
			return FALSE;
		}
	}
	return TRUE;
}

//一卡通刷卡接口，每次都调用此接口；
//
/*
void CCopyStat::CheckUser(void *pInfo,int nSize)
{
	Owner ow(m_csLock);
	if (IsUserLogoned())
	{
		if (m_pCallback)
		{
			m_pCallback->OnAllCopyCmplt(0);
		}
		LogoffUser();
		
		if(!IsSameUser(pInfo,nSize))
		{
			LogonUser(pInfo,nSize);
		}
	}
	else
	{
		LogonUser(pInfo,nSize);
	}
}
*/
void CCopyStat::LogonUser()
{
	if (!IsInited())
	{
		return;
	}

	theLog.Write("LogonUser");
	InterlockedExchange(&m_lUserLogoned,1);
	InterlockedExchange(&m_lUserLogonedSucTickCount,0);
	InitHw();
	LockHw(FALSE);
	ReleaseList();
	NowTickCount();
}

void CCopyStat::LogoffUser()
{
	if (!IsInited() || !IsUserLogoned())
	{
		return;
	}

	theLog.Write("LogoffUser");
#if 0	//del by zxl,20151104-处理用户登出时，复印多出的一些纸张要求记数
	m_pCallback = NULL;
	InterlockedExchange(&m_lUserLogoned,0);	
	InterlockedExchange(&m_lUserLogonedSucTickCount,0);
	NotifyCopyData();
#endif
	LockHw(TRUE);
}


BOOL CCopyStat::IsRunning()
{
	long lVal = 0;
	InterlockedExchange(&lVal,m_lActive);
	return lVal;
}

void CCopyStat::InitHw()
{
// 	Owner ow(m_csLock);
	NowTickCount();

	DelayWrite();

	BOOL bRet = FALSE;
	if(CHWConfig::GetInstance().IsHwVerTwo())
	{
		bRet = m_oHelper.Logon(m_szBrandCode,4);
	}
	else
	{
		bRet = m_oHelper.Logon();
	}
	if(!bRet)
	{
		theLog.Write("!!CCopyStat::InitHw,fail,IsHwVerTwo=%d", CHWConfig::GetInstance().IsHwVerTwo());
		//OnErr();
	}
	else
	{
		//InterlockedIncrement(&m_lCmdResponse);
	}
}

void CCopyStat::LockHw(BOOL bLock)
{
	theLog.Write("CCopyStat::LockHw, lock=%d,m_eType=%d", bLock, m_eType);
// 	Owner ow(m_csLock);
	NowTickCount();
	DelayWrite();
	if(m_eType == BT_KM || m_eType == BT_SL2 || m_eType == BT_ZD || m_eType == BT_HP)
	{
		BOOL bRet = FALSE;
		if(bLock)
		{
			bRet = m_oHelper.Lock(m_szLockCode,4);
		}
		else
		{
			bRet = m_oHelper.UnLock(m_szUnLockCode,4);
		}
		if (!bRet)
		{
			theLog.Write("!!CCopyStat::LockHw,fail,m_eType=%d,bLock=%d", m_eType, bLock);
			//OnErr();
		}
		else
		{
			//InterlockedIncrement(&m_lCmdResponse);
		}
	}

	theLog.Write("CCopyStat::LockHw,3,m_eType=%d", m_eType);
	OnLockSuc(bLock);
}

void CCopyStat::PowerHw(BOOL bPower)
{
	theLog.Write("CCopyStat::PowerHw,bPower=%d,m_eType=%d", bPower, m_eType);
// 	Owner ow(m_csLock);
	NowTickCount();
	DelayWrite();
	BOOL bRet = FALSE;
	if(bPower)
	{
		bRet = m_oHelper.Write(m_szPowerOnCode,4);
	}
	else
	{
		bRet = m_oHelper.Write(m_szPowerDownCode,4);
	}
	if (!bRet)
	{
		theLog.Write("!!CCopyStat::PowerHw,fail");
		//OnErr();
	}
	else
	{
		//InterlockedIncrement(&m_lCmdResponse);
	}
}

void CCopyStat::OnIdleTimeout()
{
	Owner ow(m_csLock);
	LogoffUser();
	if (m_pCallback)
	{
		//to do
		//zhandb
		m_pCallback->OnTimeout();
		
	}
}

void CCopyStat::SetCallback(ICopyChargeInterface* pCall)
{
	Owner ow(m_csLock);
	m_pCallback = pCall;
}

unsigned __stdcall CCopyStat::ThreadFn(void*pvParam)
{
	CCopyStat* pThis = (CCopyStat*)pvParam;
	
	SetEvent(pThis->m_hThreadStarted);
	BOOL bSend = TRUE;
	
	while (  pThis->IsRunning() )
	{
		
		DWORD dwWait = WaitForSingleObject(pThis->m_hThreadTerm,1000);
		switch ( dwWait )
		{
		case WAIT_OBJECT_0:
			{
				_endthreadex(1);
			}
			break;
		case WAIT_TIMEOUT:
			{
				::InterlockedIncrement(&(pThis->m_lCheckTickCount));
				if (pThis->IsCmdResponseTimeOut())
				{
					theLog.Write("设备故障,命令超时");
					pThis->OnErr();
					break;
				}
				if(pThis->IsUserLogoned())
				{
					if (pThis->IsInited())
					{
// 						if (pThis->IsLocked())
// 						{
// 							pThis->LockHw(FALSE);
// 						}
// 						else
// 						{
							//超时
							if (pThis->IsIdleTimeOut())
							{
								pThis->OnIdleTimeout();
							}
							else
							{
								pThis->CheckState(bSend);
							}

							if (pThis->IsCanNotify())
							{
								pThis->NotifyCopyData();
							}
							else
							{
								//theLog.Write("###NotifyCopyData no");
							}
// 						}
						
					}
					else
					{
						theLog.Write("设备重新登陆");
						pThis->InitHw();
					}
				}
				
			}
			break;
		default:
			{
				ASSERT(0);
			}
			break;
		}//switch
	}
	return 0;
}

void CCopyStat::CheckState(BOOL& bSend)
{
#if 1	//add by zxl,20151104-处理用户登出时，复印多出的一些纸张要求记数
	if (IsLocked())
	{
		//theLog.Write("!!CCopyStat::CheckState,IsLocked,no check");
		return;
	}
#endif

	long lVal = 0;//::InterlockedIncrement(&m_lCheckTickCount);
	InterlockedExchange(&lVal,m_lCheckTickCount);
	//Owner ow(m_csLock);

	if(CHWConfig::GetInstance().IsHwVerTwo())
	{
		if(!m_oHelper.CheckState(m_szCheckCode,4))
		{
			theLog.Write("CheckState err");
			bSend = FALSE;
			OnErr();
		}
		else
		{
			//long lVal = InterlockedIncrement(&m_lCmdResponse);
			//theLog.Write("CheckState %d",lVal);
			bSend = TRUE;
		}
	}
	else
	{
		if(!m_oHelper.CheckState())
		{
			//OnErr();
		}
		else
		{
			InterlockedIncrement(&m_lCmdResponse);
		}
	}
	//bSend = TRUE;
}

void CCopyStat::SendKey(BOOL& bSend)
{
	long lVal = 0;//::InterlockedIncrement(&m_lCheckTickCount);
	InterlockedExchange(&lVal,m_lCheckTickCount);
	
	if (lVal % 11 != 0 || bSend)
	{
		return ;
	}
	static BYTE seed = 0x88;
	static BYTE seed2 = 0x17;
	BYTE xor = (seed ++)%0xff;
	BYTE add = (seed2 ++)%0xff;

	BYTE szBuf[4] = {0xb0,0x00,0x00,0xc0};
	szBuf[1] = xor;
	szBuf[2] = add;
	m_oHelper.Write(szBuf,4);

	m_xor = xor;
	m_add = add;
	bSend = TRUE;
}

void CCopyStat::SendData(BOOL& bSend)
{
	long lVal = 0;//::InterlockedIncrement(&m_lCheckTickCount);
	InterlockedExchange(&lVal,m_lCheckTickCount);
	
	if (lVal % 13 != 0 || bSend)
	{
		return ;
	}
	BYTE szGuid[16] = {0x40,0x2d,0xdf,0x10,0x19,0xdc,0x49,0xf2,0xbc,0x3d,0xc3,0xe4,0xbb,0x22,0xf2,0xcc};
	BYTE szCode[18] = {0};
	CSimpleCry::InitKey(m_xor,m_add);
	CSimpleCry::Encrypt((char*)szGuid,(char*)szGuid,16);
	memcpy(&szCode[1],szGuid,16);
	szCode[0] = 0xD0;
	szCode[17] = 0xc0;
  	//szCode[16] = (szCode[16] + 1)%255;
  	//return ;
	m_oHelper.Write(szCode,18);
}

BOOL CCopyStat::CheckHw()	//add by zxl,20140923,检测复印控制器是否正常
{
	//Owner ow(m_csLock);
	if(m_nComIndex <= 0)
	{
		//theLog.Write("CCopyStat::CheckHw,m_nComIndex == %d,没有复印控制器。", m_nComIndex);
		return TRUE;
	}

	return m_oHelper.Write(m_szCheckHw, 4);
}

//延迟写入数据到复印控制器
void CCopyStat::DelayWrite(DWORD dwMilliseconds /*= 500*/)
{
	if ((m_eType == BT_SL) || (m_eType == BT_SL2))
	{
		//发送com数据后，不能太快的发送下一个数据,否则复印控制器程序可能会丢失上一个数据。
		Sleep(dwMilliseconds);
	}
}
