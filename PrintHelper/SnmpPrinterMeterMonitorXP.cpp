#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorXP.h"
#include "SharpMIB_Def.h"

//本操作类只是测试了夏普[AR-M351U]机器,是黑白机器,彩色抄表是不明确的

CSnmpPrinterMeterMonitorXP::CSnmpPrinterMeterMonitorXP(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorXP::~CSnmpPrinterMeterMonitorXP(void)
{
}

void CSnmpPrinterMeterMonitorXP::InitOID()
{
	m_szSharpA4PrintTotalPageOID = DecryptOID(SharpA4PrintTotalPageOID);
	m_szSharpA4PrintColorPageOID = DecryptOID(SharpA4PrintColorPageOID);
	m_szSharpA4PrintHBPageOID = DecryptOID(SharpA4PrintHBPageOID);
	m_szSharpA4CopyHBPageOID = DecryptOID(SharpA4CopyHBPageOID);
	m_szSharpA4CopyHBPage2OID = DecryptOID(SharpA4CopyHBPage2OID);
	m_szSharpA4CopyHBPage3OID = DecryptOID(SharpA4CopyHBPage3OID);
	m_szSharpA4CopyFullColorPageOID = DecryptOID(SharpA4CopyFullColorPageOID);
	m_szSharpA4CopyDoubleColorPageOID = DecryptOID(SharpA4CopyDoubleColorPageOID);
	m_szSharpA4CopySingleColorPageOID = DecryptOID(SharpA4CopySingleColorPageOID);
	m_szSharpOtherOpHBPageOID = DecryptOID(SharpOtherOpHBPageOID);
	m_szSharpOtherOpColorPageOID = DecryptOID(SharpOtherOpColorPageOID);
	m_szSharpScanTotalPageOID = DecryptOID(SharpScanTotalPageOID);
	m_szSharpScanTotalPage2OID = DecryptOID(SharpScanTotalPage2OID);
	m_szSharpA4FaxHBPageOID = DecryptOID(SharpA4FaxHBPageOID);
}

//纸张不能明确指示为纸型时，默认计算为A4
//打印、复印中关于计数，A3=2*A4
BOOL CSnmpPrinterMeterMonitorXP::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}


	if (!GetRequest(CStringToChar(m_szSharpA4PrintTotalPageOID), nValue))	//打印-总数=".1.3.6.1.4.1.2385.1.1.19.2.1.3.1.4.60"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,获取打印-黑白的总页数失败");
	}
	else
	{
		int nValue2 = 0;
		if(!GetRequest(CStringToChar(m_szSharpA4PrintColorPageOID), nValue2)) //打印彩色=".1.3.6.1.4.1.2385.1.1.19.2.1.3.1.4.63"
		{
			//m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = nValue2 > 0 ? nValue2 : m_oMeterInfo.nA4PrintColorPage;
		}
		nValue2 = 0;
		if(!GetRequest(CStringToChar(m_szSharpA4PrintHBPageOID), nValue2))//打印黑白=".1.3.6.1.4.1.2385.1.1.19.2.1.3.1.4.61"
		{
			m_oMeterInfo.nA4PrintHBPage = nValue > 0 ? nValue : m_oMeterInfo.nA4PrintHBPage ;
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = nValue2 > 0 ? nValue2 : m_oMeterInfo.nA4PrintHBPage;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,打印-黑白的总页数=%d,打印彩色总页数=%d", m_oMeterInfo.nA4PrintHBPage, m_oMeterInfo.nA4PrintColorPage);
	}

	if (GetRequest(CStringToChar(m_szSharpA4CopyHBPageOID), nValue))	//复印-黑白=".1.3.6.1.4.1.2385.1.1.19.2.1.3.5.4.60"
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.5,获取复印-黑白的总页数成功，nValue=%d", nValue);
	}

	if (GetRequest(CStringToChar(m_szSharpA4CopyHBPage2OID), nValue))	//复印-总数=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.60"
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.5,获取复印-黑白的总页数成功，nValue=%d", nValue);
	}

	if(GetRequest(CStringToChar(m_szSharpA4CopyHBPage3OID), nValue))//复印-黑白=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.61"
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.6,获取复印-黑白的总页数成功，nValue=%d", nValue);
	}

	if(!GetRequest(CStringToChar(m_szSharpA4CopyFullColorPageOID), nValue))//复印-彩色=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.63"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.2,获取复印-彩色的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
	}

	if(!GetRequest(CStringToChar(m_szSharpA4CopyDoubleColorPageOID), nValue))//复印-双彩=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.64"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.3,获取复印-双彩的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
	}

	if(!GetRequest(CStringToChar(m_szSharpA4CopySingleColorPageOID), nValue))//复印-单彩=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.80"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.4,获取复印-单彩的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
	}

	if (!GetRequest(CStringToChar(m_szSharpOtherOpHBPageOID), nValue))	//黑白其它=".1.3.6.1.4.1.2385.1.1.19.2.1.3.5.5.61"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,4.1,获取黑白其它总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpHBPage = (nValue>0) ? nValue : 0;
	}

	if (!GetRequest(CStringToChar(m_szSharpOtherOpColorPageOID), nValue))	//彩色其它=".1.3.6.1.4.1.2385.1.1.19.2.1.3.5.5.63"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,4.2,获取彩色其它总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage = (nValue>0) ? nValue : 0;
	}
	
	if (!GetRequest(CStringToChar(m_szSharpScanTotalPageOID), nValue))	//扫描=".1.3.6.1.4.1.2385.1.1.19.2.1.3.7.4.50"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,获取扫描的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3,获取扫描的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSharpScanTotalPage2OID), nValue))	//扫描（黑白+彩色）=".1.3.6.1.4.1.2385.1.1.19.2.1.3.9.4.50"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,获取扫描的总页数失败");
	}
	else
	{
		if(m_oMeterInfo.nScanTotalPage <= 0 && nValue > 0)
		{
			m_oMeterInfo.nScanTotalPage = nValue;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3,获取扫描的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSharpA4FaxHBPageOID), nValue))	//传真-黑白=".1.3.6.1.4.1.2385.1.1.19.2.1.3.8.4.50"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,获取传真-黑白的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4FaxHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3,获取传真-黑白的总页数成功，nValue=%d", nValue);
	}

	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,100,end");
	return TRUE;
}
