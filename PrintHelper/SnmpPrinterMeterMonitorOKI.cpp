#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorOKI.h"
#include "OkiMIB_Def.h"

CSnmpPrinterMeterMonitorOKI::CSnmpPrinterMeterMonitorOKI(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorOKI::~CSnmpPrinterMeterMonitorOKI(void)
{
}

void CSnmpPrinterMeterMonitorOKI::InitOID()
{
	m_szOkiA4PrintTotalPageOID = DecryptOID(OkiA4PrintTotalPageOID);
	m_szOkiA4PrintColorPageOID = DecryptOID(OkiA4PrintColorPageOID);
	m_szOkiA4PrintHBPageOID = DecryptOID(OkiA4PrintHBPageOID);
}

BOOL CSnmpPrinterMeterMonitorOKI::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szOkiA4PrintTotalPageOID), nValue))	//总页数=".1.3.6.1.4.1.2001.1.1.1.1.100.1.1.1.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,2,获取总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,3,获取总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szOkiA4PrintColorPageOID), nValue))	//彩色打印数=".1.3.6.1.4.1.2001.1.1.1.1.11.1.10.170.1.8.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,4,获取彩色打印数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,5,获取彩色打印数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szOkiA4PrintHBPageOID), nValue))	//黑白打印数=".1.3.6.1.4.1.2001.1.1.1.1.11.1.10.170.1.7.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,6,获取黑白打印数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,7,获取黑白打印数成功，nValue=%d", nValue);
	}

	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage = other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,100,end");
	return TRUE;
}