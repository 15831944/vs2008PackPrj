#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorDZ.h"
#include "ToshibaMIB_Def.h"

CSnmpPrinterMeterMonitorDZ::CSnmpPrinterMeterMonitorDZ(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorDZ::~CSnmpPrinterMeterMonitorDZ(void)
{
}

void CSnmpPrinterMeterMonitorDZ::InitOID()
{
	m_szToshibaA4PrintFullColorPageOID = DecryptOID(ToshibaA4PrintFullColorPageOID);
	m_szToshibaA4PrintSingleColorPageOID = DecryptOID(ToshibaA4PrintSingleColorPageOID);
	m_szToshibaA4PrintHBPageOID = DecryptOID(ToshibaA4PrintHBPageOID);
	m_szToshibaA3PrintFullColorPageOID = DecryptOID(ToshibaA3PrintFullColorPageOID);
	m_szToshibaA3PrintSingleColorPageOID = DecryptOID(ToshibaA3PrintSingleColorPageOID);
	m_szToshibaA3PrintHBPageOID = DecryptOID(ToshibaA3PrintHBPageOID);
	m_szToshibaA4CopyFullColorPageOID = DecryptOID(ToshibaA4CopyFullColorPageOID);
	m_szToshibaA4CopySingleColorPageOID = DecryptOID(ToshibaA4CopySingleColorPageOID);
	m_szToshibaA4CopyHBPageOID = DecryptOID(ToshibaA4CopyHBPageOID);
	m_szToshibaA3CopyFullColorPageOID = DecryptOID(ToshibaA3CopyFullColorPageOID);
	m_szToshibaA3CopySingleColorPageOID = DecryptOID(ToshibaA3CopySingleColorPageOID);
	m_szToshibaA3CopyHBPageOID = DecryptOID(ToshibaA3CopyHBPageOID);
	m_szToshibaScanSmallFullColorPageOID = DecryptOID(ToshibaScanSmallFullColorPageOID);
	m_szToshibaScanSmallSingleColorPageOID = DecryptOID(ToshibaScanSmallSingleColorPageOID);
	m_szToshibaScanSmallHBPageOID = DecryptOID(ToshibaScanSmallHBPageOID);
	m_szToshibaScanBigFullColorPageOID = DecryptOID(ToshibaScanBigFullColorPageOID);
	m_szToshibaScanBigSingleColorPageOID = DecryptOID(ToshibaScanBigSingleColorPageOID);
	m_szToshibaScanBigHBPageOID = DecryptOID(ToshibaScanBigHBPageOID);
	m_szToshibaOtherOpFullColorA4PageOID = DecryptOID(ToshibaOtherOpFullColorA4PageOID);
	m_szToshibaOtherOpSinglelColorA4PageOID = DecryptOID(ToshibaOtherOpSingleColorA4PageOID);
	m_szToshibaOtherOpHBA4PageOID = DecryptOID(ToshibaOtherOpHBA4PageOID);
	m_szToshibaOtherOpFullColorA3PageOID = DecryptOID(ToshibaOtherOpFullColorA3PageOID);
	m_szToshibaOtherOpSingleColorA3PageOID = DecryptOID(ToshibaOtherOpSingleColorA3PageOID);
	m_szToshibaOtherOpHBA3PageOID = DecryptOID(ToshibaOtherOpHBA3PageOID);
}

BOOL CSnmpPrinterMeterMonitorDZ::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	//A4打印
	if (!GetRequest(CStringToChar(m_szToshibaA4PrintFullColorPageOID), nValue))	//打印-彩色-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.210.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,2,获取打印-彩色-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = nValue > 0 ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,3,获取打印-彩色-小纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4PrintSingleColorPageOID), nValue))	//打印-双色/单色-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.210.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,4,获取打印-双色/单色-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,5,获取打印-双色/单色-小纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4PrintHBPageOID), nValue))	//打印-黑白-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.210.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,6,获取打印-黑白-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,7,获取打印-黑白-小纸的总页数成功，nValue=%d", nValue);
	}

	//A3打印
	if (!GetRequest(CStringToChar(m_szToshibaA3PrintFullColorPageOID), nValue))	//打印-彩色-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.209.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,8,获取打印-彩色-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,9,获取打印-彩色-大纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3PrintSingleColorPageOID), nValue))	//打印-双色/单色-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.209.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,10,获取打印-双色/单色-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,11,获取打印-双色/单色-大纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3PrintHBPageOID), nValue))	//打印-黑白-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.209.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,12,获取打印-黑白-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,13,获取打印-黑白-大纸的总页数成功，nValue=%d", nValue);
	}

	//A4复印
	if (!GetRequest(CStringToChar(m_szToshibaA4CopyFullColorPageOID), nValue))	//复印-彩色-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.212.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,14,获取复印-彩色-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,15,获取复印-彩色-小纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4CopySingleColorPageOID), nValue))	//复印-双色/单色-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.212.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,16,获取复印-双色/单色-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,17,获取复印-双色/单色-小纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4CopyHBPageOID), nValue))	//复印-黑白-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.212.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,18,获取复印-黑白-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,19,获取复印-黑白-小纸的总页数成功，nValue=%d", nValue);
	}

	//A3复印
	if (!GetRequest(CStringToChar(m_szToshibaA3CopyFullColorPageOID), nValue))	//复印-彩色-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.211.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,20,获取复印-彩色-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,21,获取复印-彩色-大纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3CopySingleColorPageOID), nValue))	//复印-双色/单色-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.211.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,22,获取复印-双色/单色-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,23,获取复印-双色/单色-大纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3CopyHBPageOID), nValue))	//复印-黑白-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.211.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,24,获取复印-黑白-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,25,获取复印-黑白-大纸的总页数成功，nValue=%d", nValue);
	}

	//扫描小纸
	if (!GetRequest(CStringToChar(m_szToshibaScanSmallFullColorPageOID), nValue))	//扫描-网络-彩色-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.220.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,26,获取扫描-网络-彩色-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanSmallColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,27,获取扫描-网络-彩色-小纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanSmallSingleColorPageOID), nValue))	//扫描-网络-双色/单色-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.220.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,28,获取扫描-网络-双色/单色-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanSmallColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,29,获取扫描-网络-双色/单色-小纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanSmallHBPageOID), nValue))	//扫描-网络-黑白-小纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.220.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,30,获取扫描-网络-黑白-小纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanSmallHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,31,获取扫描-网络-黑白-小纸的总页数成功，nValue=%d", nValue);
	}

	//扫描大纸
	if (!GetRequest(CStringToChar(m_szToshibaScanBigFullColorPageOID), nValue))	//扫描-网络-彩色-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.219.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,32,获取扫描-网络-彩色-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanBigColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,33,获取扫描-网络-彩色-大纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanBigSingleColorPageOID), nValue))	//扫描-网络-双色/单色-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.219.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,34,获取扫描-网络-双色/单色-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanBigColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,35,获取扫描-网络-双色/单色-大纸的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanBigHBPageOID), nValue))	//扫描-网络-黑白-大纸=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.219.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,36,获取扫描-网络-黑白-大纸的总页数失败");
	}
	else
	{
		m_oMeterInfo.nScanBigHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,37,获取扫描-网络-黑白-大纸的总页数成功，nValue=%d", nValue);
	}

	//---------------------------------------------

	//A3彩色
	//A3黑白
	//A4彩色
	//A4黑白
	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;

	//其它操作大纸，其它操作一般指打印机web中的计数器的【列表】类。
	if (!GetRequest(CStringToChar(m_szToshibaOtherOpFullColorA3PageOID), nValue))	//其它操作大纸 全彩=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.225.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,38,获取打其它操作大纸全彩的总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA3ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,39,获取其它操作大纸全彩的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpSingleColorA3PageOID), nValue))	//其它操作大纸 单彩=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.225.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,40,获取其它操作大纸单彩的总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA3ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,41,获取其它操作大纸单彩的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpHBA3PageOID), nValue))	//其它操作大纸 黑白=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.225.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,42,获取其它操作大纸黑白的总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpHBPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA3HBTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,43,获取打印-其它-大纸的总页数成功，nValue=%d", nValue);
	}

	//其它操作小纸，其它操作一般指打印机web中的计数器的【列表】类。
	if (!GetRequest(CStringToChar(m_szToshibaOtherOpFullColorA4PageOID), nValue))	//其它操作小纸 全彩=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.226.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,44,获取打其它操作小纸全彩的总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA4ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,45,获取其它操作小纸全彩的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpSinglelColorA4PageOID), nValue))	//其它操作小纸 单彩=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.226.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,46,获取其它操作小纸单彩的总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA4ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,47,获取其它操作大纸单彩的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpHBA4PageOID), nValue))	//其它操作小纸 黑白=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.226.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,48,获取其它操作小纸黑白的总页数失败");
	}
	else
	{
		m_oMeterInfo.nOtherOpHBPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA4HBTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,49,获取打印-其它-小纸的总页数成功，nValue=%d", nValue);
	}

	//其它彩色
	//其它黑白
	if(m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage - m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage > 0)
		m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage - m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage;

	//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,100,end");
	return TRUE;
}
