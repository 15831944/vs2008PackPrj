#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorBrother.h"
#include "BrotherMIB_Def.h"

CSnmpPrinterMeterMonitorBrother::CSnmpPrinterMeterMonitorBrother(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorBrother::~CSnmpPrinterMeterMonitorBrother(void)
{
}

void CSnmpPrinterMeterMonitorBrother::InitOID()
{
	m_szBrotherCounterInfo1OID = DecryptOID(BrotherCounterInfo1OID);
	m_szBrotherCounterInfo2OID = DecryptOID(BrotherCounterInfo2OID);
	m_szBrotherCounterInfo3OID = DecryptOID(BrotherCounterInfo3OID);
	m_szBrotherCounterInfo4OID = DecryptOID(BrotherCounterInfo4OID);
	m_szBrotherCounterInfo21OID = DecryptOID(BrotherCounterInfo21OID);
	m_szBrotherCounterInfo22OID = DecryptOID(BrotherCounterInfo22OID);
	m_szBrotherCounterInfo31ID = DecryptOID(BrotherCounterInfo31OID);
	m_szBrotherCounterInfo41ID = DecryptOID(BrotherCounterInfo41OID);
	m_szBrotherModelOID = DecryptOID(BrotherModelOID);
	m_szBrotherA4HBpPagesOID_LenovoLJ3650DN = DecryptOID(BrotherA4HBpPagesOID_LenovoLJ3650DN);
	m_szBrotherTotalHBPagesOID_LenovoLJ3650DN = DecryptOID(BrotherTotalHBPagesOID_LenovoLJ3650DN);
}

//抄表暂时只区分色彩，不区分纸型，因为没有拿到mib文件和打印机不全，不能分析出所有的信息。
BOOL CSnmpPrinterMeterMonitorBrother::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();
	m_szModel = GetBrotherModel();
	if (m_szModel.Find("Lenovo LJ3650DN") >= 0)
	{
		return GetMeterInfo_LenovoLJ3650DN();
	}
	else
	{
		int nValue = 0;

		if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorBrother::GetMeterInfo,获取总页数失败");
			return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
		}

		//1.查找所有作业（打印、复印、扫描、传真等等）的计数索引
		BrotherCounterInfo2EntryMap oCounterInfo2;
		char cOidBegin[128] = {0};
		sprintf(cOidBegin, "%s.1", m_szBrotherCounterInfo2OID);	//BrotherCounterInfo2OID
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
				if (oCounterInfo2.find(nIndex) == oCounterInfo2.end())
				{
					PBrotherCounterInfo2Entry pEntry = new BrotherCounterInfo2Entry;
					memset(pEntry, 0x0, sizeof(BrotherCounterInfo2Entry));
					oCounterInfo2.insert(pair<int,PBrotherCounterInfo2Entry>(nIndex, pEntry));
					pEntry->Index = nIndex;
				}
			}
			else
			{
				break;
			}
		}

		int nCounterCount = oCounterInfo2.size();
		//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,nCounterCount=%d", nCounterCount);

		//2.根据作业计数索引，获取具体的作业计数信息，如色彩、计数。
		BrotherCounterInfo2EntryMap::iterator it;
		for (it=oCounterInfo2.begin(); it!=oCounterInfo2.end(); it++)
		{
			PBrotherCounterInfo2Entry pEntry = it->second;
			if (pEntry)
			{
				int nIndex = pEntry->Index;

				char cOidStr[128] = {0};
				sprintf(cOidStr, "%s.2.%d", m_szBrotherCounterInfo2OID, nIndex);	//Type=2 //BrotherCounterInfo2OID
				GetRequest(cOidStr, (int&)pEntry->Type);

				sprintf(cOidStr, "%s.3.%d", m_szBrotherCounterInfo2OID, nIndex);	//PageNumbers=3	//BrotherCounterInfo2OID
				GetRequest(cOidStr, pEntry->PageNumbers);
			}
		}

		BOOL bHaveColorCounter = FALSE;
		//3.统计打印实际出纸面数信息，
		for (it=oCounterInfo2.begin(); it!=oCounterInfo2.end(); it++)
		{
			PBrotherCounterInfo2Entry pEntry = it->second;
			if (pEntry)
			{
				if (pEntry->Type == ENUM_STRUCT_VALUE(BrotherCounterType)::TotalPages)
				{
					m_oMeterInfo.nOtherOpTotalPage = pEntry->PageNumbers;
				}
				else if (pEntry->Type == ENUM_STRUCT_VALUE(BrotherCounterType)::ColorTotalPages)
				{
					bHaveColorCounter = TRUE;
					m_oMeterInfo.nOtherOpColorPage = pEntry->PageNumbers;
				}
				else if (pEntry->Type == ENUM_STRUCT_VALUE(BrotherCounterType)::BlackWhiteTotalPages)
				{
					m_oMeterInfo.nOtherOpHBPage = pEntry->PageNumbers;
				}
				else if (pEntry->PageNumbers > 0)
				{
					theLog.Write("##CSnmpPrinterMeterMonitorBrother::GetMeterInfo,Index=%d,Type=%d,PageNumbers=%d,"
						, pEntry->Index, pEntry->Type, pEntry->PageNumbers);
					m_oMeterInfo.nOtherOpTotalPage += pEntry->PageNumbers;
				}

				delete pEntry;	//删除内存
			}
		}

		oCounterInfo2.clear();

		m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
		m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
		m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
		m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
		if (!bHaveColorCounter)
		{
			m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nOtherOpTotalPage;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,100,end");
		return (nCounterCount>0) ? TRUE : FALSE;
	}	
}
BOOL CSnmpPrinterMeterMonitorBrother::GetMeterInfo_LenovoLJ3650DN()
{
	//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,1,begin");
	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szBrotherTotalHBPagesOID_LenovoLJ3650DN), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorBrother::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}
	if (!GetRequest(CStringToChar(m_szBrotherA4HBpPagesOID_LenovoLJ3650DN), nValue))	//A4黑白打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorBrother::GetMeterInfo,获取a4黑白打印计数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4HBTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorBrother::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}
	if (m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA4HBTotalPage >= 0)
	{
		m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA4HBTotalPage;
		m_oMeterInfo.nOtherOpTotalPage = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA4HBTotalPage; 

	}
	return TRUE;
}
CString CSnmpPrinterMeterMonitorBrother::GetBrotherModel()
{
	CString szModel;
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szBrotherModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorBrother::GetBrotherModel,获取型号失败");
	}
	szModel.Format("%s",tmp);
	theLog.Write("CSnmpPrinterMeterMonitorBrother::GetBrotherModel,szModel=%s",szModel);
	return szModel;

}