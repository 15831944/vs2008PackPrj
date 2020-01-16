#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorLG.h"
#include "RicohMIB_Def.h"

CSnmpPrinterMeterMonitorLG::CSnmpPrinterMeterMonitorLG(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorLG::~CSnmpPrinterMeterMonitorLG(void)
{
}

void CSnmpPrinterMeterMonitorLG::InitOID()
{
	m_szRicohEngCounterEntryOID = DecryptOID(RicohEngCounterEntryOID);
	m_szRicohTonerColorOID = DecryptOID(RicohTonerColorOID);
	m_szRicohTonerDescOID = DecryptOID(RicohTonerDescOID);
	m_szRicohTonerColorTypeOID = DecryptOID(RicohTonerColorTypeOID);
	m_szRicohTonerLevelPercentOID = DecryptOID(RicohTonerLevelPercentOID);
}

//纸张不能明确指示为纸型时，默认计算为A4
//打印、复印中关于计数，A3=2*A4
BOOL CSnmpPrinterMeterMonitorLG::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorLG::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue;
	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLG::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorLG::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	//1.查找所有作业（打印、复印、扫描、传真等等）的计数索引
	RicohEngCounterEntryMap oRicohEngCounterEntryMap;
	char *cOidBegin = CStringToChar(m_szRicohEngCounterEntryOID);	//
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (oRicohEngCounterEntryMap.find(nIndex) == oRicohEngCounterEntryMap.end())
			{
				PricohEngCounterEntry pEntry = new ricohEngCounterEntry;
				memset(pEntry, 0x0, sizeof(PricohEngCounterEntry));
				oRicohEngCounterEntryMap.insert(pair<int,PricohEngCounterEntry>(nIndex, pEntry));
				pEntry->ricohEngCounterIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}
	int nMapSize = oRicohEngCounterEntryMap.size();
	//theLog.Write("CSnmpPrinterMeterMonitorLG::CSnmpPrinterMeterMonitorLG,nMapSize=%d", nMapSize);

	//2.根据作业计数索引，获取具体的作业计数信息，如作业类型、纸型、计数单元、单双面、色彩、计数。
	RicohEngCounterEntryMap::iterator it;
	for (it=oRicohEngCounterEntryMap.begin(); it!=oRicohEngCounterEntryMap.end(); it++)
	{
		PricohEngCounterEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->ricohEngCounterIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.2.%d", m_szRicohEngCounterEntryOID, nIndex);	//RicohEngCounterEntryOID
			GetRequest(cOidStr, (int&)pEntry->ricohEngCounterType);

			//sprintf(cOidStr, "%s.4.%d", m_szRicohEngCounterEntryOID, nIndex);
			//GetRequest(cOidStr, (int&)pEntry->ricohEngCounterUnit);

			sprintf(cOidStr, "%s.9.%d", m_szRicohEngCounterEntryOID, nIndex);
			GetRequest(cOidStr, (int&)pEntry->ricohEngCounterValue);
		}
	}


	//3.统计打印实际出纸面数信息，
	for (it=oRicohEngCounterEntryMap.begin(); it!=oRicohEngCounterEntryMap.end(); it++)
	{
		PricohEngCounterEntry pEntry = it->second;
		if (pEntry)
		{
			if (pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Print_Black_White)
			{
				m_oMeterInfo.nA4PrintHBPage += pEntry->ricohEngCounterValue;
			}
			else if (pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Print_Full_Color
				 || pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Print_SingleOrTwo_Color)
			{
				m_oMeterInfo.nA4PrintColorPage += pEntry->ricohEngCounterValue;
			}
			else if (pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Copy_Black_White)
			{
				m_oMeterInfo.nA4CopyHBPage += pEntry->ricohEngCounterValue;

			}
			else if (pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Copy_Full_Color
				|| pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Copy_SingleOrTwo_Color)
			{
				m_oMeterInfo.nA4CopyColorPage += pEntry->ricohEngCounterValue;

			}
			else if(pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Fax_Black_White)
			{
				m_oMeterInfo.nA4FaxHBPage += pEntry->ricohEngCounterValue;
			}
			else if(pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Fax_SingleOrTwo_Color
				|| pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Fax_Full_Color)
			{
				m_oMeterInfo.nA4FaxColorPage += pEntry->ricohEngCounterValue;
			}
			else if(pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Transmission_Black_White_Scan
				|| pEntry->ricohEngCounterType == ENUM_STRUCT_VALUE(ricohEngCounterType)::Transmission_Color_Scan)
			{
				m_oMeterInfo.nScanTotalPage += pEntry->ricohEngCounterValue;
			}
			delete pEntry;	//删除内存
		}
	}
	//theLog.Write("CSnmpPrinterMeterMonitorLG::CSnmpPrinterMeterMonitorLG,1,nScanTotalPage=%d", m_oMeterInfo.nScanTotalPage);

	oRicohEngCounterEntryMap.clear();

	m_oMeterInfo.nA4FaxTotalPage = m_oMeterInfo.nA4FaxHBPage + m_oMeterInfo.nA4FaxColorPage;
	m_oMeterInfo.nA3FaxTotalPage = m_oMeterInfo.nA3FaxHBPage + m_oMeterInfo.nA3FaxColorPage;
	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage + m_oMeterInfo.nA3FaxHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage + m_oMeterInfo.nA3FaxColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage + m_oMeterInfo.nA4FaxHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage + m_oMeterInfo.nA4FaxColorPage;
// 	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage;
// 	if(other > 0)
// 	{
// 		m_oMeterInfo.nOtherOpHBPage = other;
// 	}

	//theLog.Write("CSnmpPrinterMeterMonitorLG::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	/*
	int nMaxTonerCount = 4;//最多4个硒鼓
	int nTonerType = ENUM_STRUCT_VALUE(ricohEngTonerType)::Black_toner1;
	for(int i = 0; i < nMaxTonerCount; i++)
	{

		char cOidStr[128] = {0};
		sprintf(cOidStr, ".1.3.6.1.4.1.367.3.2.1.2.24.1.1.4.%d", i+1); //TonerType
		if (!GetRequest(cOidStr, nValue))
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorLG::GetMeterInfo,获取硒鼓类型失败");
		}
		else
		{
			nTonerType = nValue;
			theLog.Write("CSnmpPrinterMeterMonitorLG::GetMeterInfo,获取硒鼓类型成功，nValue=%d", nValue);
		}

		sprintf(cOidStr, ".1.3.6.1.4.1.367.3.2.1.2.24.1.1.5.%d", i+1); ////TonerLevel
		if (!GetRequest(cOidStr, nValue))
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorLG::GetMeterInfo,获取硒鼓剩余量失败");
		}
		else
		{
			if(nValue == ENUM_STRUCT_VALUE(ricohEngTonerType)::Black_toner1 || nValue == ENUM_STRUCT_VALUE(ricohEngTonerType)::Black_toner2)
			{
				strcpy(m_oMeterInfo.cTonerBlackLevelPercent, GetTonerPercent(nValue).GetBuffer());
			}
			else if(nValue == ENUM_STRUCT_VALUE(ricohEngTonerType)::Cyan_toner)
			{
				strcpy(m_oMeterInfo.cTonerCyanLevelPercent, GetTonerPercent(nValue).GetBuffer());
			}
			else if(nValue == ENUM_STRUCT_VALUE(ricohEngTonerType)::Magenta_toner)
			{
				strcpy(m_oMeterInfo.cTonerMagentaLevelPercent, GetTonerPercent(nValue).GetBuffer());
			}
			else if(nValue == ENUM_STRUCT_VALUE(ricohEngTonerType)::Yellow_toner)
			{
				strcpy(m_oMeterInfo.cTonerYellowLevelPercent, GetTonerPercent(nValue).GetBuffer());
			}

			theLog.Write("CSnmpPrinterMeterMonitorLG::GetMeterInfo,获取硒鼓剩余量成功，nValue=%d", nValue);
		}
	}*/


	//theLog.Write("CSnmpPrinterMeterMonitorLG::GetMeterInfo,100,end");

	return TRUE;
}

CString CSnmpPrinterMeterMonitorLG::GetTonerPercent( int value )
{
	CString percentret = "";
	switch(value)
	{
		case -3://100% - some remaining
			percentret = "100%";
			break;
		case 0://
			percentret = "0%";
			break;
		case -100:
			percentret = "0%";
			break;
		case 10:
			percentret = "10%";
			break;
		case 20:
			percentret = "20%";
			break;
		case 30:
			percentret = "30%";
			break;
		case 40:
			percentret = "40%";
			break;
		case 50:
			percentret = "50%";
			break;
		case 60:
			percentret = "60%";
			break;
		case 70:
			percentret = "70%";
			break;
		case 80:
			percentret = "80%";
			break;
		case 90:
			percentret = "90%";
			break;
		case 100:
			percentret = "100%";
			break;
	}
	return percentret;
}