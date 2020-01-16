#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorSX.h"
#include "SamsungMIB_Def.h"

CSnmpPrinterMeterMonitorSX::CSnmpPrinterMeterMonitorSX(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorSX::~CSnmpPrinterMeterMonitorSX(void)
{
}

void CSnmpPrinterMeterMonitorSX::InitOID()
{
	m_szScmHrDevCountEntryOID = DecryptOID(ScmHrDevCountEntryOID);
}

//纸张不能明确指示为纸型时，默认计算为A4
//打印、复印中关于计数，A3=2*A4
BOOL CSnmpPrinterMeterMonitorSX::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSX::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	//1.查找所有作业（打印、复印、扫描、传真等等）的计数索引
	ScmHrDevCountEntryMap oScmHrDevCount;
	char *cOidBegin = CStringToChar(m_szScmHrDevCountEntryOID);	//ScmHrDevCountEntryOID
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
			if (oScmHrDevCount.find(nIndex) == oScmHrDevCount.end())
			{
				PScmHrDevCountEntry pEntry = new ScmHrDevCountEntry;
				memset(pEntry, 0x0, sizeof(ScmHrDevCountEntry));
				oScmHrDevCount.insert(pair<int,PScmHrDevCountEntry>(nIndex, pEntry));
				pEntry->scmHrDevCountIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	int nHrDevCount = oScmHrDevCount.size();
	theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,nHrDevCount=%d", nHrDevCount);

	//2.根据作业计数索引，获取具体的作业计数信息，如作业类型、纸型、计数单元、单双面、色彩、计数。
	ScmHrDevCountEntryMap::iterator it;
	for (it=oScmHrDevCount.begin(); it!=oScmHrDevCount.end(); it++)
	{
		PScmHrDevCountEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->scmHrDevCountIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.2.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountJobType=2
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountJobType);

			sprintf(cOidStr, "%s.3.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountMediaSize=3
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountMediaSize);

			sprintf(cOidStr, "%s.4.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountUnit=4
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountUnit);

			sprintf(cOidStr, "%s.5.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountDuplex=5
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountDuplex);

			sprintf(cOidStr, "%s.6.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountColor=6
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountColor);

			sprintf(cOidStr, "%s.7.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountValue=7
			GetRequest(cOidStr, pEntry->scmHrDevCountValue);
		}
	}
	int nA4HBReport = 0;
	int nA4ColorReport = 0;
	int nA3HBReport = 0;
	int nA3ColorReport = 0;
	//3.统计打印实际出纸面数信息，
	for (it=oScmHrDevCount.begin(); it!=oScmHrDevCount.end(); it++)
	{
		PScmHrDevCountEntry pEntry = it->second;
		if (pEntry)
		{
			if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::print)
			{
				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4PrintHBPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3PrintHBPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4PrintHBPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherPrintHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4PrintColorPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3PrintColorPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4PrintColorPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherPrintColorPage += pEntry->scmHrDevCountValue;
					}
				}
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::copy)
			{
				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4CopyHBPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3CopyHBPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4CopyHBPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherCopyHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4CopyColorPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3CopyColorPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4CopyColorPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherCopyColorPage += pEntry->scmHrDevCountValue;
					}
				}			
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::faxIn
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::faxOut)
			{
				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4FaxHBPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3FaxHBPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4FaxHBPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherFaxHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4FaxColorPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3FaxColorPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4FaxColorPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherFaxColorPage += pEntry->scmHrDevCountValue;
					}
				}	
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalSend/*scan*/)
			{
				m_oMeterInfo.nScanTotalPage += pEntry->scmHrDevCountValue;
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::report)	//报告打印计数
			{

				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						nA4HBReport += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						nA3HBReport += pEntry->scmHrDevCountValue;
						nA4HBReport += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherPrintHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						nA4ColorReport += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						nA3ColorReport += pEntry->scmHrDevCountValue;
						nA4ColorReport += pEntry->scmHrDevCountValue * 2; 
					}
					else
					{
						m_oMeterInfo.nOtherPrintColorPage += pEntry->scmHrDevCountValue;
					}
				}
			}
			else /*if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalSend
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalSend
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalRecieve
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::localStorage)*/
			{
				/*
				theLog.Write("##CSnmpPrinterMeterMonitorSX::GetMeterInfo,"
					"scmHrDevCountIndex=%d,scmHrDevCountJobType=%d,scmHrDevCountColor=%d,"
					"scmHrDevCountMediaSize=%d,scmHrDevCountDuplex=%d,scmHrDevCountValue=%d,scmHrDevCountUnit=%d"
					, pEntry->scmHrDevCountIndex, pEntry->scmHrDevCountJobType, pEntry->scmHrDevCountColor
					, pEntry->scmHrDevCountMediaSize, pEntry->scmHrDevCountDuplex, pEntry->scmHrDevCountValue
					, pEntry->scmHrDevCountUnit);
				*/
			}

			delete pEntry;	//删除内存
		}
	}

	oScmHrDevCount.clear();
	
	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage + nA3HBReport;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage + nA3ColorReport;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage + nA4HBReport;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage + nA4ColorReport;
	m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nOtherPrintHBPage + m_oMeterInfo.nOtherCopyHBPage;
	m_oMeterInfo.nOtherOpColorPage = m_oMeterInfo.nOtherPrintColorPage + m_oMeterInfo.nOtherCopyColorPage;
	//不是所有品牌的打印机都要添加其它，因为有些打印机本身计数已经很详细了，没有必要二次计算其它值的计数。
// 	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
// 	if(other > 0)
// 	{
// 		m_oMeterInfo.nOtherOpHBPage += other;
// 	}

	//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,100,end");
	return (nHrDevCount>0) ? TRUE : FALSE;
}
