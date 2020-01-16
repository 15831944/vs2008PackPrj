#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorEPSON.h"
#include "EpsonMIB_Def.h"

CSnmpPrinterMeterMonitorEPSON::CSnmpPrinterMeterMonitorEPSON(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorEPSON::~CSnmpPrinterMeterMonitorEPSON(void)
{
}

void CSnmpPrinterMeterMonitorEPSON::InitOID()
{
	m_szEpsonA4PrintHBPageOID = DecryptOID(EpsonA4PrintHBPageOID);
	m_szEpsonA4PrintTotalPageOID = DecryptOID(EpsonA4PrintTotalPageOID);
	m_szEpsonA4PrintColorPageOID = DecryptOID(EpsonA4PrintColorPageOID);
	m_szEpsonA4CopyTotalPageOID = DecryptOID(EpsonA4CopyTotalPageOID);
	m_szEpsonA4CopyColorPageOID = DecryptOID(EpsonA4CopyColorPageOID);
	m_szEpsonOtherOpTotalPageOID = DecryptOID(EpsonOtherOpTotalPageOID);
	m_szEpsonOtherOpColorPageOID = DecryptOID(EpsonOtherOpColorPageOID);
}

BOOL CSnmpPrinterMeterMonitorEPSON::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4PrintHBPageOID), nValue))	//黑白-总数=".1.3.6.1.4.1.1248.1.2.2.27.1.1.3.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,2,获取黑白的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,3,获取黑白的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4PrintTotalPageOID), nValue))	//打印总数（黑白+彩色=".1.3.6.1.4.1.1248.1.2.2.27.6.1.4.1.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,4,获取打印总数（黑白+彩色）失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,5,获取打印总数（黑白+彩色）成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4PrintColorPageOID), nValue))	//打印彩色=".1.3.6.1.4.1.1248.1.2.2.27.6.1.5.1.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,6,获取打印彩色失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		if(m_oMeterInfo.nA4PrintTotalPage - m_oMeterInfo.nA4PrintColorPage > 0)
		{//打印总数-彩色打印
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintTotalPage - m_oMeterInfo.nA4PrintColorPage;
		}
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,7,获取打印黑白成功，nValue=%d", m_oMeterInfo.nA4PrintHBPage);
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,8,获取打印彩色成功，nValue=%d", m_oMeterInfo.nA4PrintColorPage);
	}


	if (!GetRequest(CStringToChar(m_szEpsonA4CopyTotalPageOID), nValue))	//复印总数=".1.3.6.1.4.1.1248.1.2.2.27.6.1.4.1.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,9.2,获取复印总数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,9.3,获取复印总数成功,nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4CopyColorPageOID), nValue))	//复印彩色=".1.3.6.1.4.1.1248.1.2.2.27.6.1.5.1.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,11,获取复印彩色失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
		if(m_oMeterInfo.nA4CopyTotalPage - m_oMeterInfo.nA4CopyColorPage > 0)
		{
			m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage - m_oMeterInfo.nA4CopyColorPage;
		}
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,12,获取复印黑白成功，nValue=%d", m_oMeterInfo.nA4CopyHBPage);
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,13,获取复印彩色成功，nValue=%d", m_oMeterInfo.nA4CopyColorPage);
	}

	if (!GetRequest(CStringToChar(m_szEpsonOtherOpTotalPageOID), nValue))	//其它操作用纸总数=".1.3.6.1.4.1.1248.1.2.2.27.6.1.4.1.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,14,获取其它操作用纸失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,15,获取其它操作用纸成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonOtherOpColorPageOID), nValue))	//其它操作彩色用纸=".1.3.6.1.4.1.1248.1.2.2.27.6.1.5.1.1.4"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,16,获取其它操作彩色用纸失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage = (nValue>0) ? nValue : 0;
		if(m_oMeterInfo.nOtherOpTotalPage - m_oMeterInfo.nOtherOpColorPage > 0)
		{
			m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nOtherOpTotalPage - m_oMeterInfo.nOtherOpColorPage;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,17,获取其它操作黑白用纸成功，nValue=%d", m_oMeterInfo.nOtherOpHBPage);
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,18,获取其它操作彩色用纸成功，nValue=%d", m_oMeterInfo.nOtherOpColorPage);
	}
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	return TRUE;
}