#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorPRONNIX.h"
#include "PrintronixMIB_Def.h"

CSnmpPrinterMeterMonitorPRONNIX::CSnmpPrinterMeterMonitorPRONNIX(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorPRONNIX::~CSnmpPrinterMeterMonitorPRONNIX(void)
{
}
void CSnmpPrinterMeterMonitorPRONNIX::InitOID()
{
	m_szPRINTRONIX_TotalPageOID = DecryptOID(PRINTRONIX_TotalPageOID);
}
BOOL CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo()
{
	theLog.Write("CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;
	//theLog.Write("CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo,1,m_szPRINTRONIX_TotalPageOID=%s",m_szPRINTRONIX_TotalPageOID);
	if (!GetRequest(CStringToChar(m_szPRINTRONIX_TotalPageOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
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

	//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, oMeterInfoTmp.nA3HBTotalPage, oMeterInfoTmp.nA3ColorTotalPage, oMeterInfoTmp.nA4HBTotalPage, oMeterInfoTmp.nA4ColorTotalPage, oMeterInfoTmp.nOtherOpHBPage, oMeterInfoTmp.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,100,end");
	return TRUE;
}