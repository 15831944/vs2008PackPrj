#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorHP.h"
#include "HewlettPackardMIB_Def.h"

#include <math.h>

#define DEF_HTTP	"http://"
#define DEF_HTTPS	"https://"
#define DEF_USEAGEPAGE_COMMON	"/hp/device/InternalPages/Index?id=UsagePage"
#define DEF_USEAGEPAGE_725		"/hp/device/InternalPages/Index?id=UsagePage"
#define DEF_USEAGEPAGE_CP3505	"/hp/device/this.LCDispatcher?nav=hp.Usage"
#define DEF_METERPAGE_M525		"/hp/device/InternalPages/Index?id=UsagePage"
#define DEF_USEAGEPAGE_E77822	"/hp/device/InternalPages/Index?id=UsagePage"
#define DEF_USEAGEPAGE_E72530 	"/hp/device/InternalPages/Index?id=UsagePage"
#define DEF_USEAGEPAGE_CM3530_MFP "/hp/device/this.LCDispatcher?nav=hp.Usage"


CSnmpPrinterMeterMonitorHP::CSnmpPrinterMeterMonitorHP(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorHP::~CSnmpPrinterMeterMonitorHP(void)
{
}

void CSnmpPrinterMeterMonitorHP::InitOID()
{
	m_szHpPrinterModelOID = DecryptOID(HpPrinterModelOID);
	m_szHpA4PrintHBPageOID = DecryptOID(HpA4PrintHBPageOID);
	m_szHpA4PrintColorPageOID = DecryptOID(HpA4PrintColorPageOID);
	m_szHpA4CopyTotalPageOID = DecryptOID(HpA4CopyTotalPageOID);
	m_szHpA4CopyHBPageOID = DecryptOID(HpA4CopyHBPageOID);
	m_szHpA4CopyColorPageOID = DecryptOID(HpA4CopyColorPageOID);

	m_szHpPrintTotalPageOID_M1216 = DecryptOID(HpPrintTotalPageOID_M1216);
	m_szszHpPrintTotalPageOID_M4345 = DecryptOID(HpPrintTotalPageOID_M4345);
	m_szHpA4PrintHBPageOID_4345 = DecryptOID(HpA4PrintHBPageOID_M4345);

	//Equivalent OID
	m_szHpEquivalentPrintHBOID = DecryptOID(HpEquivalentPrintHBOID);
	m_szHpEquivalentPrintColorOID = DecryptOID(HpEquivalentPrintColorOID);
	m_szHpEquivalentCopyHBOID = DecryptOID(HpEquivalentCopyHBOID);
	m_szHpEquivalentCopyColorOID = DecryptOID(HpEquivalentCopyColorOID);
	m_szHpEquivalentFaxHBOID = DecryptOID(HpEquivalentFaxHBOID);
	m_szHpA4TotalPrintOID_M401 = DecryptOID(HpA4TotalPrintOID_M401);
	m_szHpA4HBPrintOID_M401 = DecryptOID(HpA4HBPrintOID_M401);
	m_szHpA4ColorPrintOID_M401 = DecryptOID(HpA4ColorPrintOID_M401);
	m_szHpA4TotalPrintOID_M401n = DecryptOID(HpA4HBPrintTotal_M401n);
	m_szHpScanHBOID = DecryptOID(HpScanHBOID);

	m_szHpEquivalentPrintHBOID_3015dn = DecryptOID(HpEquivalentPrintHBOID_3015dn);

}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4345()
{
	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szszHpPrintTotalPageOID_M4345), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4345,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szHpA4PrintHBPageOID_4345), nValue))	//黑白打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.6.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4345,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
	return TRUE;
}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M1216()
{
	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szHpPrintTotalPageOID_M1216), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M1216,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
	return TRUE;

}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650()
{
	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szHpA4PrintHBPageOID), nValue))	//黑白打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.6.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szHpA4PrintColorPageOID), nValue))	//彩色打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.7.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,4,获取彩色打印的总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,5,获取彩色打印的总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szHpA4CopyTotalPageOID), nValue))	//复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.63.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,6,获取复印总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取复印总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szHpA4CopyHBPageOID), nValue))	//黑白复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.122.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,8,获取复印总页数失败");
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage;
		if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
		{
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
		}
	}
	else
	{
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyHBPage = nValue;
		if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
		{
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,9,获取黑白复印总页数成功，nValue=%d,黑白打印总页数=%d", nValue, m_oMeterInfo.nA4PrintHBPage);
	}

	if (!GetRequest(CStringToChar(m_szHpA4CopyColorPageOID), nValue))	//彩色复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.123.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,10,获取彩色总页数失败");
	}
	else
	{
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyColorPage= nValue;
		if(m_oMeterInfo.nA4PrintColorPage > nValue)
		{
			m_oMeterInfo.nA4PrintColorPage = m_oMeterInfo.nA4PrintColorPage - nValue;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,11,获取彩色复印总页数成功，nValue=%d,彩色打印总页数=%d", nValue, m_oMeterInfo.nA4PrintColorPage);
	}

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
	return TRUE;

}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_METERPAGE_M525);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,1,szWebContent=%s",szWebContent);
	CString szEquivalentPrint = GetEquivalentPrint_M525(szWebContent);
	CString szEquivalentCopy = GetEquivalentCopy_M525(szWebContent);
	CString szEquivalentFax = GetEquivalentFax_M525(szWebContent);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,1,szEquivalentPrint=%s,szEquivalentCopy=%s,szEquivalentFax=%s"
	//	,szEquivalentPrint,szEquivalentCopy,szEquivalentFax);

	if (szEquivalentPrint.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
	}
	else
	{
		
		int nValue = DoubleToInt(atof(szEquivalentPrint.GetString()));
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;// A4打印总计数
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;//A4打印黑白总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentCopy.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,6,获取复印总页数失败");
	}
	else
	{
		int nValue = DoubleToInt(atof(szEquivalentCopy.GetString()));
		m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;//A4复印总计数
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;//A4复印黑白总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取复印总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentFax.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,6,获取复印总页数失败");
	}
	else
	{
		int nValue = DoubleToInt(atof(szEquivalentFax.GetString()));
		m_oMeterInfo.nA4FaxTotalPage = (nValue>0) ? nValue : 0;//A4传真总计数
		m_oMeterInfo.nA4FaxHBPage = (nValue>0) ? nValue : 0;//A4传真黑白总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取复印总页数成功，nValue=%d", nValue);
	}
	m_oMeterInfo.nAllTotalPage = m_oMeterInfo.nA4PrintHBPage 
		+ m_oMeterInfo.nA4CopyTotalPage + m_oMeterInfo.nA4FaxTotalPage;
#if 0
	if (szEquivalentPrint.GetLength() <= 0)
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}




	if (!GetRequest(CStringToChar(m_szHpA4CopyHBPageOID), nValue))	//黑白复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.122.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,8,获取复印总页数失败");
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage;
		if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
		{
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
		}
	}
	else
	{
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyHBPage = nValue;
		if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
		{
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,9,获取黑白复印总页数成功，nValue=%d,黑白打印总页数=%d", nValue, m_oMeterInfo.nA4PrintHBPage);
	}


	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
#endif
	return TRUE;

}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTP,szIp,DEF_USEAGEPAGE_725);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725,1,szWebContent=%s",szWebContent);
	CString szEquivalentPrint = GetEquivalentPrint_M725(szWebContent);
	CString szEquivalentCopy = GetEquivalentCopy_M725(szWebContent);
	CString szEquivalentFax = GetEquivalentFax_M725(szWebContent);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725,1,szEquivalentPrint=%s,szEquivalentCopy=%s,szEquivalentFax=%s"
	//	,szEquivalentPrint,szEquivalentCopy,szEquivalentFax);

	if (szEquivalentPrint.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = atoi(szEquivalentPrint.GetString());// A4打印总计数
		m_oMeterInfo.nA4PrintTotalPage = atoi(szEquivalentPrint.GetString());//A4打印黑白总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentCopy.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,6,获取复印总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyTotalPage = atoi(szEquivalentCopy);//A4复印总计数
		m_oMeterInfo.nA4CopyHBPage = atoi(szEquivalentCopy);//A4复印黑白总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取复印总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentFax.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M4650,6,获取复印总页数失败");
	}
	else
	{
		m_oMeterInfo.nA4FaxTotalPage = atoi(szEquivalentFax);//A4传真总计数
		m_oMeterInfo.nA4FaxHBPage =  atoi(szEquivalentFax);//A4传真黑白总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取复印总页数成功，nValue=%d", nValue);
	}
	m_oMeterInfo.nAllTotalPage = m_oMeterInfo.nA4PrintHBPage 
								+ m_oMeterInfo.nA4CopyTotalPage + m_oMeterInfo.nA4FaxTotalPage;
#if 0
	if (szEquivalentPrint.GetLength() <= 0)
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M725,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}


	

	if (!GetRequest(CStringToChar(m_szHpA4CopyHBPageOID), nValue))	//黑白复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.122.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,8,获取复印总页数失败");
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage;
		if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
		{
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
		}
	}
	else
	{
		nValue = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4CopyHBPage = nValue;
		if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
		{
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,9,获取黑白复印总页数成功，nValue=%d,黑白打印总页数=%d", nValue, m_oMeterInfo.nA4PrintHBPage);
	}


	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
#endif
	return TRUE;

}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTP,szIp,DEF_USEAGEPAGE_CP3505);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,1,szWebContent=%s",szWebContent);
	CString szEquivalentToalPrint = GetEquivalentTotalPrint_CP3505(szWebContent);
	CString szEquivalentA4ToalPrint = GetEquivalentA4TotalPrint_CP3505(szWebContent);
	CString szEquivalentA4HBToalPrint = GetEquivalentA4HBTotalPrint_CP3505(szWebContent);
	CString szEquivalentA4ColorToalPrint = GetEquivalentA4ColorTotalPrint_CP3505(szWebContent);
	CString szEquivalentHBToalPrint = GetEquivalentHBTotalPrint_CP3505(szWebContent);
	CString szEquivalentColorToalPrint = GetEquivalentColorTotalPrint_CP3505(szWebContent);
// 	theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,1,szEquivalentToalPrint=%s,szEquivalentA4HBToalPrint=%s"
// 		,szEquivalentToalPrint,szEquivalentA4HBToalPrint);
	
	if (szEquivalentToalPrint.GetLength() <= 0)	//总页数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentToalPrint.GetString()));
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}
	
	if (szEquivalentA4ToalPrint.GetLength() <= 0)//a4 打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,获取A4打印总页数失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentA4ToalPrint.GetString()));
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
	}
	if (szEquivalentA4HBToalPrint.GetLength() <=0 )	//A4黑白打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentA4HBToalPrint.GetString()));
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentA4ColorToalPrint.GetLength() <= 0)	//A4彩色打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP3505,4,获取彩色打印的总页数失败");
	}
	else
	{
		nValue =  DoubleToInt(atof(szEquivalentA4ColorToalPrint.GetString()));
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,5,获取彩色打印的总页数成功，nValue=%d", nValue);
	}

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;

	
	m_oMeterInfo.nOtherOpHBPage = DoubleToInt(atof(szEquivalentHBToalPrint.GetString())) - DoubleToInt(atof(szEquivalentA4HBToalPrint.GetString()));
	
	m_oMeterInfo.nOtherOpColorPage = DoubleToInt(atof(szEquivalentColorToalPrint.GetString())) - DoubleToInt(atof(szEquivalentA4ColorToalPrint.GetString()));

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
	return TRUE;

}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4TotalPrint_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "A4";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 6;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "TOTAL PRINTER USAGE";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 3;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP3505,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}
		
	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "A4";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 4;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "A4";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 5;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "TOTAL PRINTER USAGE";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 2;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "TOTAL PRINTER USAGE";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 1;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP3505,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}

BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP5520()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_USEAGEPAGE_COMMON);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP5520,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,1,szWebContent=%s",szWebContent);
	CString szEquivalentTotalPrint = GetEquivalentTotalPrint_CP5520(szWebContent);
	CString szEquivalentA4HBTotalPrint = GetEquivalentA4HBTotalPrint_CP5520(szWebContent);
	CString szEquivalentA4ColorTotalPrint = GetEquivalentA4ColorTotalPrint_CP5520(szWebContent);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP5520,1,szEquivalentTotalPrint=%s,szEquivalentA4HBTotalPrint=%s,szEquivalentA4ColorTotalPrint=%s"
	//	, szEquivalentTotalPrint, szEquivalentA4HBTotalPrint, szEquivalentA4ColorTotalPrint);

	if (szEquivalentTotalPrint.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP5520,2,获取等效打印的总页数失败");
	}
	else
	{
		int nValue = DoubleToInt(atof(szEquivalentTotalPrint.GetString()));
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;// A4打印总计数
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentA4HBTotalPrint.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CP5520,6,获取等效黑白打印总页数失败");
	}
	else
	{
		int nValue = DoubleToInt(atof(szEquivalentA4HBTotalPrint.GetString()));
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取等效黑白打印总页数失败，nValue=%d", nValue);
	}

	if (szEquivalentA4ColorTotalPrint.GetLength() <= 0)	
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M525,6,获取等效彩色总页数失败");
	}
	else
	{
		int nValue = DoubleToInt(atof(szEquivalentA4ColorTotalPrint.GetString()));
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取等效彩色总页数失败，nValue=%d", nValue);
	}
	m_oMeterInfo.nAllTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4PrintColorPage;

	return TRUE;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CP5520(CString& szContent)
{
	if (szContent.GetLength() <= 0)
	{
		return "";
	}
	CString szTmp;
	CString	szFalg = "UsagePage.EquivalentImpressionsTable.Print.Total";
	int nIndex = szContent.Find(szFalg);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - szFalg.GetLength() - nIndex);
		szFalg = ">";
		int nIndex1 = szTmp.Find(szFalg);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - szFalg.GetLength()-nIndex1);
			szFalg = "</td>";
			int nIndex2 = szTmp.Find(szFalg);
			if (nIndex2 >= 0)
			{
				szTmp = szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace(" ","");
	szTmp.Replace(",","");
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CP5520(CString& szContent)
{
	if (szContent.GetLength() <= 0)
	{
		return "";
	}
	CString szTmp;
	CString	szFalg = "UsagePage.EquivalentImpressionsTable.Print.Monochrome";
	int nIndex = szContent.Find(szFalg);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - szFalg.GetLength() - nIndex);
		szFalg = ">";
		int nIndex1 = szTmp.Find(szFalg);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - szFalg.GetLength()-nIndex1);
			szFalg = "</td>";
			int nIndex2 = szTmp.Find(szFalg);
			if (nIndex2 >= 0)
			{
				szTmp = szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace(" ","");
	szTmp.Replace(",","");
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CP5520(CString& szContent)
{
	if (szContent.GetLength() <= 0)
	{
		return "";
	}
	CString szTmp;
	CString	szFalg = "UsagePage.EquivalentImpressionsTable.Print.Color";
	int nIndex = szContent.Find(szFalg);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - szFalg.GetLength() - nIndex);
		szFalg = ">";
		int nIndex1 = szTmp.Find(szFalg);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - szFalg.GetLength()-nIndex1);
			szFalg = "</td>";
			int nIndex2 = szTmp.Find(szFalg);
			if (nIndex2 >= 0)
			{
				szTmp = szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace(" ","");
	szTmp.Replace(",","");
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetHPModel()
{
	CString szModel;
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szHpPrinterModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetHPModel,获取型号失败");
	}
	szModel.Format("%s",tmp);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetHPModel,szModel=%s",szModel);
	szModel.Trim();
	return szModel;

}
//纸张不能明确指示为纸型时，默认计算为A4
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	CString szHpPrintModel = GetHPModel();
	if(szHpPrintModel.Find("4345") >= 0)
	{
		return GetMeterInfo_M4345();		
	}
	else if(szHpPrintModel.Find("1216") >= 0)
	{
		return GetMeterInfo_M1216();
	}
	else if (szHpPrintModel.Find("4650") >= 0)
	{
		return GetMeterInfo_M4650();
	}
	else if (szHpPrintModel.Find("M725") >= 0)
	{
		return GetMeterInfo_M725();
	}
	else if (szHpPrintModel.Find("CP3505") >= 0)
	{
		return GetMeterInfo_CP3505();
	}
	else if (szHpPrintModel.Find("M525") >= 0)
	{
		return GetMeterInfo_M525();
	}
	else if (szHpPrintModel.Find("CP5520") >= 0)
	{
		return GetMeterInfo_CP5520();
	}
	else if ((szHpPrintModel.Find("LaserJet 400") >=0 && szHpPrintModel.Find("M401dn")>=0 || szHpPrintModel.Find("M401dn")>=0) || szHpPrintModel.Find("M252n") >= 0
		|| szHpPrintModel.Find("CP1525N") >= 0 || szHpPrintModel.Find("CP2025n") >= 0 || szHpPrintModel.Find("M403n") >= 0 || szHpPrintModel.Find("HP Color LaserJet CM1312nfi MFP") >= 0)
		//HP Color LaserJet Pro M252n  HP LaserJet CP1525N  M401包括M401dn,M401dne,HP Color LaserJet CP2025n
	{
		return GetMeterInfo_M401();
	}
	else if (szHpPrintModel.Find("HP LaserJet 400 M401n") >= 0 ||szHpPrintModel.Find("HP LaserJet M1536dnf MFP") >= 0 ||szHpPrintModel.Find("P2055dn") >= 0)
	{
		return GetMeterInfo_M401n();//HP LaserJet 400 M401n
	}
	else if (szHpPrintModel.Find("P3010") >= 0)
	{
		return GetMeterInfo_3015dn();
	}

// 	else if (m_szHpPrintModel.Find("E77822") >= 0)
// 	{
// 		return GetMeterInfo_E77822();
// 	}
// 	else if (m_szHpPrintModel.Find("E72530") >= 0)
// 	{
// 		return GetMeterInfo_E72530();
// 	}
// 	else if (m_szHpPrintModel.Find("E72535") >= 0)//E72535跟E72530耗材页一样
// 	{
// 		return GetMeterInfo_E72530();
// 	}
	else if (szHpPrintModel.Find("CM3530 MFP") >= 0)
	{
		return GetMeterInfo_CM3530_MFP();
	}
	else if (szHpPrintModel.Find("M452") >= 0)
	{
		int nValue = 0;
		if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数失败");
			return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szHpA4PrintHBPageOID), nValue))	//黑白打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.6.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szHpA4PrintColorPageOID), nValue))	//彩色打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.7.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,4,获取彩色打印的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,5,获取彩色打印的总页数成功，nValue=%d", nValue);
		}

		m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
		m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
		int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
		if(other > 0)
		{
			m_oMeterInfo.nOtherOpHBPage += other;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
		return TRUE;
	}
	else if (szHpPrintModel.Find("Officejet") >= 0 && szHpPrintModel.Find("X585") < 0)
	{
		int nValue = 0;
		if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数失败");
			return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szHpA4PrintHBPageOID), nValue))	//黑白打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.6.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szHpA4PrintColorPageOID), nValue))	//彩色打印的总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.4.1.2.7.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,4,获取彩色打印的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,5,获取彩色打印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szHpA4CopyTotalPageOID), nValue))	//复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.63.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,6,获取复印总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,获取复印总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szHpA4CopyHBPageOID), nValue))	//黑白复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.122.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,8,获取复印总页数失败");
			nValue = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage;
			if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
			{
				m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
			}
		}
		else
		{
			nValue = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nA4CopyHBPage = nValue;
			if(m_oMeterInfo.nA4PrintHBPage - nValue > 0)
			{
				m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintHBPage - nValue;
			}
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,9,获取黑白复印总页数成功，nValue=%d,黑白打印总页数=%d", nValue, m_oMeterInfo.nA4PrintHBPage);
		}

		if (!GetRequest(CStringToChar(m_szHpA4CopyColorPageOID), nValue))	//彩色复印总页数=".1.3.6.1.4.1.11.2.3.9.4.2.1.2.2.1.123.0"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,10,获取彩色总页数失败");
		}
		else
		{
			nValue = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nA4CopyColorPage= nValue;
			if(m_oMeterInfo.nA4PrintColorPage > nValue)
			{
				m_oMeterInfo.nA4PrintColorPage = m_oMeterInfo.nA4PrintColorPage - nValue;
			}
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,11,获取彩色复印总页数成功，nValue=%d,彩色打印总页数=%d", nValue, m_oMeterInfo.nA4PrintColorPage);
		}

		m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
		m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
		int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
		if(other > 0)
		{
			m_oMeterInfo.nOtherOpHBPage += other;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
		return TRUE;
	}
	else
	{
		/*CString m_szHpEquivalentPrintHBOID;
		CString m_szHpEquivalentPrintColorOID;
		CString m_szHpEquivalentCopyHBOID;
		CString m_szHpEquivalentCopyColorOID;
		CString m_szHpEquivalentFaxHBOID;*/
		int nValue = 0;
		char cRes[1024] = {0};
		char ch[32] = {0};
		int nPrintHBCount = 0;
		int nPrintColorCount = 0;
		int nCopyHBCount = 0;
		int nCopyColorCount = 0;
		int nFaxHBCount = 0;
		if (!GetHexRequest(CStringToChar(m_szHpEquivalentPrintHBOID), (unsigned char*)cRes,sizeof(cRes)))
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取等效打印黑白计数失败");
		}
		else
		{
			memset(ch,0,32);
			ltoa(strtol(cRes,'\0',16),ch,2);
			double fValue = ConvertIEEE754(ch);
			nValue = DoubleToInt(fValue);
			nPrintHBCount = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nEquivalentHBPrintPage = nPrintHBCount;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,1,nValue=%d", nValue);
		}
		memset(cRes,0,1024);
		if (!GetHexRequest(CStringToChar(m_szHpEquivalentPrintColorOID), (unsigned char*)cRes,sizeof(cRes)))
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,2,获取等效打印彩色计数失败");
		}
		else
		{
			memset(ch,0,32);
			ltoa(strtol(cRes,'\0',16),ch,2);
			double fValue = ConvertIEEE754(ch);
			nValue = DoubleToInt(fValue);
			nPrintColorCount = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nEquivalentColorPrintPage = nPrintColorCount;

			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,nValue=%d", nValue);
		}
		memset(cRes,0,1024);
		if (!GetHexRequest(CStringToChar(m_szHpEquivalentCopyHBOID), (unsigned char*)cRes,sizeof(cRes)))
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,4,获取等效复印黑白计数失败");
		}
		else
		{
			memset(ch,0,32);
			ltoa(strtol(cRes,'\0',16),ch,2);
			double fValue = ConvertIEEE754(ch);
			nValue = DoubleToInt(fValue);
			nCopyHBCount = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nEquivalentHBCopyPage = nCopyHBCount;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,5,nValue=%d", nValue);
		}
		memset(cRes,0,1024);
		if (!GetHexRequest(CStringToChar(m_szHpEquivalentCopyColorOID), (unsigned char*)cRes,sizeof(cRes)))	
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,6,获取等效复印彩色计数失败");
		}
		else
		{
			memset(ch,0,32);
			ltoa(strtol(cRes,'\0',16),ch,2);
			double fValue = ConvertIEEE754(ch);
			nValue = DoubleToInt(fValue);
			nCopyColorCount = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nEquivalentColorCopyPage = nCopyColorCount;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,7,nValue=%d", m_oMeterInfo.nEquivalentColorCopyPage);
		}
		memset(cRes,0,1024);
		if (!GetHexRequest(CStringToChar(m_szHpEquivalentFaxHBOID), (unsigned char*)cRes,sizeof(cRes)))	
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,8,获取传真黑白计数失败");
		}
		else
		{
			memset(ch,0,32);
			ltoa(strtol(cRes,'\0',16),ch,2);
			double fValue = ConvertIEEE754(ch);
			nValue = DoubleToInt(fValue);
			nFaxHBCount = (nValue>0) ? nValue : 0;
			m_oMeterInfo.nEquivalentHBFaxPage = nFaxHBCount;
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,9,nValue=%d", nValue);
		}
		//获取扫描计数
		nValue = 0;
		if (!GetRequest(CStringToChar(m_szHpScanHBOID),nValue))
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo,8,获取扫描计数失败");
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = nValue>0?nValue:0;
		}

		//传真彩色计数节点没有，略
		m_oMeterInfo.nAllTotalPage = nPrintHBCount + nPrintColorCount + nCopyHBCount + nCopyColorCount + nFaxHBCount;

		m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
		m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
		int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
		
		m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nEquivalentHBPrintPage + m_oMeterInfo.nEquivalentHBCopyPage + m_oMeterInfo.nEquivalentHBFaxPage;
		m_oMeterInfo.nOtherOpColorPage = m_oMeterInfo.nEquivalentColorPrintPage + m_oMeterInfo.nEquivalentColorCopyPage + m_oMeterInfo.nEquivalentColorFaxPage;
		

		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
		return TRUE;
	}
}
CString CSnmpPrinterMeterMonitorHP::GetMeterByUrl(CString szUrl)
{
	CString szinfo="";
	CInternetSession *pInetSession = NULL;
	try
	{
		pInetSession = new CInternetSession; 
		CHttpFile *pHttpFile = NULL ;
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //打开一个URL
		if (!pHttpFile)
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::HtmlToList,1,fail");
			return szinfo;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorHP::HtmlToList,2,pHttpFile=0x%x", pHttpFile);
		//pHttpFile 确实是否要删除
		CString str ;

		while(pHttpFile->ReadString(str))   
		{
			szinfo += CCommonFun::UTF8ToMultiByte((const unsigned char*)str.GetString());
		}
		delete pHttpFile;
		pHttpFile = NULL;
		delete pInetSession;
		pInetSession  = NULL;
	}
	catch(...)
	{
		theLog.Write("!!GetMeterByUrl catch exception.ERROR=%d", GetLastError());
		delete pInetSession;
		pInetSession  = NULL;
	}
	return szinfo;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentPrint_M725(CString& szContent)
{
	CString szValue;
	int nFlag = szContent.Find("UsagePage.EquivalentImpressionsTable.Print.Total");
	if (nFlag >=0)
	{
		CString szTmp = szContent.Right(szContent.GetLength()-nFlag-1);
		int nFlag1 = szTmp.Find(">");
		if (nFlag1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength()-nFlag1-1);
		}
		int nFlag2 = szContent.Find("</td>");
		if (nFlag2 >=0 )
		{
			szValue = szTmp.Left(nFlag2);
		}
	}
	return szValue;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentCopy_M725(CString& szContent)
{
	CString szValue;
	int nFlag = szContent.Find("UsagePage.EquivalentImpressionsTable.Copy.Text");
	if (nFlag >=0)
	{
		CString szTmp = szContent.Right(szContent.GetLength()-nFlag-1);
		int nFlag1 = szTmp.Find(">");
		if (nFlag1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength()-nFlag1-1);
		}
		int nFlag2 = szContent.Find("</td>");
		if (nFlag2 >=0 )
		{
			szValue = szTmp.Left(nFlag2);
		}
	}
	return szValue;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentFax_M725(CString& szContent)
{
	CString szValue;
	int nFlag = szContent.Find("UsagePage.EquivalentImpressionsTable.Fax.Total");
	if (nFlag >=0)
	{
		CString szTmp = szContent.Right(szContent.GetLength()-nFlag-1);
		int nFlag1 = szTmp.Find(">");
		if (nFlag1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength()-nFlag1-1);
		}
		int nFlag2 = szContent.Find("</td>");
		if (nFlag2 >=0 )
		{
			szValue = szTmp.Left(nFlag2);
		}
	}
	return szValue;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentPrint_M525(CString& szContent)
{
	if (szContent.GetLength() <= 0)
	{
		return "";
	}
	CString szTmp;
	CString	szFalg = "UsagePage.EquivalentImpressionsTable.Print.Total";
	int nIndex = szContent.Find(szFalg);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - szFalg.GetLength() - nIndex);
		szFalg = ">";
		int nIndex1 = szTmp.Find(szFalg);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - szFalg.GetLength()-nIndex1);
			szFalg = "</td>";
			int nIndex2 = szTmp.Find(szFalg);
			if (nIndex2 >= 0)
			{
				szTmp = szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace(" ","");
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentCopy_M525(CString& szContent)
{
	if (szContent.GetLength() <= 0)
	{
		return "";
	}
	CString szTmp;
	CString	szFalg = "UsagePage.EquivalentImpressionsTable.Copy.Total";
	int nIndex = szContent.Find(szFalg);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - szFalg.GetLength() - nIndex);
		szFalg = ">";
		int nIndex1 = szTmp.Find(szFalg);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - szFalg.GetLength()-nIndex1);
			szFalg = "</td>";
			int nIndex2 = szTmp.Find(szFalg);
			if (nIndex2 >= 0)
			{
				szTmp = szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace(" ","");
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentFax_M525(CString& szContent)
{
	if (szContent.GetLength() <= 0)
	{
		return "";
	}
	CString szTmp;
	CString	szFalg = "UsagePage.EquivalentImpressionsTable.Fax.Total";
	int nIndex = szContent.Find(szFalg);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - szFalg.GetLength() - nIndex);
		szFalg = ">";
		int nIndex1 = szTmp.Find(szFalg);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - szFalg.GetLength()-nIndex1);
			szFalg = "</td>";
			int nIndex2 = szTmp.Find(szFalg);
			if (nIndex2 >= 0)
			{
				szTmp = szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace(" ","");
	szTmp.Replace(",","");
	return szTmp;
}
int CSnmpPrinterMeterMonitorHP::DoubleToInt(double fValue)
{
	return ((int)(fValue+0.5));
}

CString CSnmpPrinterMeterMonitorHP::GetHttpsMeterByUrl(CString szUrl)
{
	DWORD dwFlags;  
	DWORD dwStatus = 0;  
	DWORD dwStatusLen = sizeof(dwStatus);  
	CString strLine;  
	DWORD m_dwServiceType;
	CString m_strServer;
	CString m_strObject_URI;
	INTERNET_PORT  m_nServerPort = 0;
	CString m_strHttpVersion;
	CString m_strHtml;

	AfxParseURL(szUrl, m_dwServiceType, m_strServer, m_strObject_URI, m_nServerPort);  
	CInternetSession *m_pSess = new CInternetSession; 
	CHttpFile *m_pHttpFile = NULL ;
	CHttpConnection *m_pHttpConn= NULL;

	try {  
		m_pHttpConn = m_pSess->GetHttpConnection(m_strServer, INTERNET_FLAG_SECURE, m_nServerPort,  
			NULL, NULL);  
		if(m_pHttpConn)
		{  
			m_pHttpFile = (CHttpFile*)m_pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, m_strObject_URI, NULL, 1,  //HTTP_VERB_POST
				NULL, m_strHttpVersion,  
				INTERNET_FLAG_SECURE |  
				INTERNET_FLAG_EXISTING_CONNECT |  
				INTERNET_FLAG_RELOAD |  
				INTERNET_FLAG_NO_CACHE_WRITE |  
				INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  
				INTERNET_FLAG_IGNORE_CERT_CN_INVALID  
				);  
			//get web server option  
			BOOL BRet = m_pHttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
			if(!BRet)
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetHttpsMeterByUrl,11,QueryOption fail,err=%d", GetLastError());
			}
			//dwFlags |= SECURITY_FLAG_IGNORE_WRONG_USAGE;//SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
//			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;//SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
			dwFlags |= SECURITY_IGNORE_ERROR_MASK;	//SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			//set web server option  
			BRet = m_pHttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
			if(!BRet)
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorHP::SetOption,11,QueryOption fail,err=%d", GetLastError());
			}
			m_pHttpFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
			m_pHttpFile->AddRequestHeaders(_T("Accept: */*"));
			if(m_pHttpFile->SendRequest()) 
			{
				//get response status if success, return 200  
				int nRet = m_pHttpFile->QueryInfo(HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE,  
					&dwStatus, &dwStatusLen, 0); 
				//theLog.Write("CSnmpPrinterMeterMonitorHP::SetOption,13,QueryOption nRet=%d",nRet);
				while(m_pHttpFile->ReadString(strLine))
				{  
					m_strHtml += strLine + char(13) + char(10);  
				}
			} 
			else 
			{  
				theLog.Write("!!CSnmpPrinterMeterMonitorHP::SetOption,13,send request failed");
	
			}  
		} 
		else 
		{  
			theLog.Write("CSnmpPrinterMeterMonitorHP::SetOption,13,send request failed");
		}  
	} 
	catch(CInternetException *e) 
	{  
		e->ReportError();  
	}  
	m_pSess->Close(); 
	delete m_pSess;
	m_pSess = NULL;
	m_pHttpFile->Close();
	delete m_pHttpFile;
	m_pHttpFile = NULL;
	return m_strHtml;
}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_USEAGEPAGE_E77822);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,1,szWebContent=%s",szWebContent);
	CString szA3HBPrint_E77822 = GetA3HBPrint_E77822(szWebContent);
	CString szA3ColorPrint_E77822 = GetA3ColorPrint_E77822(szWebContent);
	CString szA4HBPrint_E77822 = GetA4HBPrint_E77822(szWebContent);
	CString szA4ColorPrint_E77822 = GetA4ColorPrint_E77822(szWebContent);

	CString szA4HBCopy_E77822 = GetA4HBCopy_E77822(szWebContent);
	CString szA4ColorCopy_E77822 = GetA4ColorCopy_E77822(szWebContent);
	CString szEquivalentTotalPrint_E77822 = GetEquivalentTotalPrint_E77822(szWebContent);
	CString szEquivalentTotalCopy_E77822 = GetEquivalentTotalCopy_E77822(szWebContent);
	CString szEquivalentTotalFx_E77822 = GetEquivalentTotalFx_E77822(szWebContent);
	CString szA4FaxHB_E77822 = GetA4HBFax_E77822(szWebContent);
	CString szA4FaxColor_E77822 = GetA4ColorFax_E77822(szWebContent);

	theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,3,"
		"szA3HBPrint_E77822=%s,szA3ColorPrint_E77822=%s,szA4HBPrint_E77822=%s,szA4ColorPrint_E77822=%s,szA4HBCopy_E77822=%s,"
		"szA4ColorCopy_E77822=%s,szEquivalentTotalPrint_E77822=%s,szEquivalentTotalCopy_E77822=%s,"
		"szEquivalentTotalFx_E77822=%s"
		,szA3HBPrint_E77822,szA3ColorPrint_E77822,szA4HBPrint_E77822,szA4ColorPrint_E77822,szA4HBCopy_E77822
		,szA4ColorCopy_E77822,szEquivalentTotalPrint_E77822,szEquivalentTotalCopy_E77822
		,szEquivalentTotalFx_E77822);
	if (szEquivalentTotalPrint_E77822.GetLength() < 0 || szEquivalentTotalCopy_E77822.GetLength() < 0 || szEquivalentTotalFx_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,4,获取总数失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentTotalPrint_E77822.GetString()) + atof(szEquivalentTotalCopy_E77822.GetString()) + atof(szEquivalentTotalFx_E77822.GetString()));
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
	}

	if (szA4HBPrint_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,5,获取a4黑白打印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4HBPrint_E77822.GetString()));
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
	}
	if (szA4ColorPrint_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,6,获取a4彩色打印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4ColorPrint_E77822.GetString()));
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
	}
	if (szA4HBCopy_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,7,获取a4黑白复印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4HBCopy_E77822.GetString()));
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
	}
	if (szA4ColorCopy_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,7,获取a4彩色复印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4ColorCopy_E77822.GetString()));
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
	}
	if (szA3HBPrint_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,7,获取a3黑白打印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA3HBPrint_E77822.GetString()));
		m_oMeterInfo.nA3PrintHBPage = (nValue>0) ? nValue : 0;
	}
	if (szA3ColorPrint_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,7,获取a3彩色打印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA3ColorPrint_E77822.GetString()));
		m_oMeterInfo.nA3PrintColorPage = (nValue>0) ? nValue : 0;
	}

	if (szA4FaxHB_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,8,获取a4黑白传真失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4FaxHB_E77822.GetString()));
		m_oMeterInfo.nA4FaxHBPage = (nValue>0) ? nValue : 0;
	}

	if (szA4FaxColor_E77822.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,8,获取a4彩色传真失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4FaxColor_E77822.GetString()));
		m_oMeterInfo.nA4FaxColorPage = (nValue>0) ? nValue : 0;
	}
	m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4PrintColorPage;
	m_oMeterInfo.nA4CopyTotalPage = m_oMeterInfo.nA4CopyHBPage + m_oMeterInfo.nA4CopyColorPage;
	m_oMeterInfo.nA4FaxTotalPage = m_oMeterInfo.nA4FaxHBPage + m_oMeterInfo.nA4FaxColorPage;

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintTotalPage + m_oMeterInfo.nA4CopyTotalPage + m_oMeterInfo.nA4FaxTotalPage;



	m_oMeterInfo.nA3PrintTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3PrintColorPage;
	m_oMeterInfo.nA3CopyTotalPage = m_oMeterInfo.nA3CopyHBPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA3FaxTotalPage = m_oMeterInfo.nA3FaxHBPage + m_oMeterInfo.nA3FaxColorPage;

	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintTotalPage + m_oMeterInfo.nA3CopyTotalPage + m_oMeterInfo.nA3FaxTotalPage;
	
	return TRUE;

}
CString CSnmpPrinterMeterMonitorHP::GetA3HBPrint_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.打印.A3.Monochrome\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		 nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Print.A3.Monochrome\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA3HBPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength()-1);
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA3HBPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA3HBPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetA3ColorPrint_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.打印.A3.Color\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Print.A3.Color\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA3ColorPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength()-1);
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA3ColorPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA3ColorPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.打印.A4.Monochrome\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Print.A4.Monochrome\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength()-1);
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetA4ColorPrint_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.打印.A4.Color\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Print.A4.Color\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4ColorPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength()-1);
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4ColorPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4ColorPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}


CString CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.复印.A4.Monochrome\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Copy.A4.Monochrome\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetA4ColorCopy_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.复印.A4.Color\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Copy.A4.Color\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4ColorCopy_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4ColorCopy_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4ColorCopy_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Fax.Monochrome\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
//	theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,4,szTmp=%s",szTmp);
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetA4ColorFax_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Fax.Color\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
//	theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,4,szTmp=%s",szTmp);
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Print.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Copy.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalFx_E77822(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Fax.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalFx_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalFx_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalFx_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_E72530()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_USEAGEPAGE_E72530);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,1,szWebContent=%s",szWebContent);
	
	CString szA4HBPrint_E72530 = GetA4HBPrint_E72530(szWebContent);
	CString szA4HBCopy_E72530 = GetA4HBCopy_E72530(szWebContent);
	CString szEquivalentTotalPrint_E72530 = GetEquivalentTotalPrint_E72530(szWebContent);
	CString szEquivalentTotalCopy_E72530 = GetEquivalentTotalCopy_E72530(szWebContent);
	CString szA4FaxHB_E72530 = GetA4HBFax_E72530(szWebContent);

	theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E72530,1,szA4HBPrint_E72530=%s,szA4HBCopy_E72530=%s,szEquivalentTotalPrint_E72530=%s,szEquivalentTotalCopy_E72530=%s,szA4FaxHB_E72530=%s"
		,szA4HBPrint_E72530,szA4HBCopy_E72530,szEquivalentTotalPrint_E72530,szEquivalentTotalCopy_E72530,szA4FaxHB_E72530);
	if (szEquivalentTotalPrint_E72530.GetLength() < 0 || szEquivalentTotalCopy_E72530.GetLength() < 0 || szA4FaxHB_E72530.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,4,获取总数失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentTotalPrint_E72530.GetString()) + atof(szEquivalentTotalCopy_E72530.GetString()) + atof(szA4FaxHB_E72530.GetString()));
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
	}

	if (szA4HBPrint_E72530.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,5,获取a4黑白打印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4HBPrint_E72530.GetString()));
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
	}
	
	if (szA4HBCopy_E72530.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,7,获取a4黑白复印失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4HBCopy_E72530.GetString()));
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
	}
	
	
	if (szA4FaxHB_E72530.GetLength() < 0)
	{
		theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_E77822,8,获取a4黑白传真失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szA4FaxHB_E72530.GetString()));
		m_oMeterInfo.nA4FaxHBPage = (nValue>0) ? nValue : 0;
	}

	
	m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4PrintColorPage;
	m_oMeterInfo.nA4CopyTotalPage = m_oMeterInfo.nA4CopyHBPage + m_oMeterInfo.nA4CopyColorPage;
	m_oMeterInfo.nA4FaxTotalPage = m_oMeterInfo.nA4FaxHBPage + m_oMeterInfo.nA4FaxColorPage;

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintTotalPage + m_oMeterInfo.nA4CopyTotalPage + m_oMeterInfo.nA4FaxTotalPage;



	m_oMeterInfo.nA3PrintTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3PrintColorPage;
	m_oMeterInfo.nA3CopyTotalPage = m_oMeterInfo.nA3CopyHBPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA3FaxTotalPage = m_oMeterInfo.nA3FaxHBPage + m_oMeterInfo.nA3FaxColorPage;

	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintTotalPage + m_oMeterInfo.nA3CopyTotalPage + m_oMeterInfo.nA3FaxTotalPage;
	
	return TRUE;
}


BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401()
{
	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szHpA4TotalPrintOID_M401),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401,1,获取打印总数失败");
		return FALSE;
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue:0; 
	}
	nValue = 0;
	if (!GetRequest(CStringToChar(m_szHpA4HBPrintOID_M401),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401,2,获取黑白打印总数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue > 0) ? nValue:0; 
		m_oMeterInfo.nA4HBTotalPage = (nValue > 0) ? nValue:0; 
	}
	if (!GetRequest(CStringToChar(m_szHpA4ColorPrintOID_M401),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401,3,获取彩色打印总数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue > 0) ? nValue:0; 
		m_oMeterInfo.nA4ColorTotalPage = (nValue > 0) ? nValue:0; 
	}
	
	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_3015dn()
{
	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szHpEquivalentPrintHBOID_3015dn),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_3015dn,1,获取等效打印总数失败");
		return FALSE;
	}
	else
	{
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401n,1,nValue=%d",nValue);
		m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue:0; 
	}
	nValue = 0;
	if (!GetRequest(CStringToChar(m_szHpEquivalentPrintHBOID_3015dn),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_3015dn,2,获取等效黑白打印总数失败");
	}
	else
	{
		m_oMeterInfo.nEquivalentHBPrintPage = (nValue > 0) ? nValue:0; 
	}

	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401n()
{
	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szHpA4TotalPrintOID_M401n),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401n,1,获取打印总数失败");
		return FALSE;
	}
	else
	{
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401n,1,nValue=%d",nValue);
		m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue:0; 
	}
	nValue = 0;
	if (!GetRequest(CStringToChar(m_szHpA4TotalPrintOID_M401n),nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401n,2,获取黑白打印总数失败");
	}
	else
	{
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_M401n,1,nValue=%d",nValue);
		m_oMeterInfo.nA4PrintHBPage = (nValue > 0) ? nValue:0; 
		m_oMeterInfo.nA4HBTotalPage = (nValue > 0) ? nValue:0; 
	}

	return TRUE;
}


//============
CString CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E72530(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.打印.A4.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Print.A4.Total\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength()-1);
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E72530(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.ImpressionsByMediaSizeTable.复印.A4.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	if (nIndex < 0)
	{
		nIndex = szContent.Find("<td id=\"UsagePage.ImpressionsByMediaSizeTable.Copy.A4.Total\" class=\"align-right\">");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBCopy_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E72530(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Print.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E72530(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Copy.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalCopy_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetA4HBFax_E72530(CString& szContent)
{
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "<td id=\"UsagePage.EquivalentImpressionsTable.Fax.Total\" class=\"align-right\">";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 0;i++)
		{
			szFlag = "class=\"align-right\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</td>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	//	theLog.Write("CSnmpPrinterMeterMonitorHP::GetA4HBFax_E77822,4,szTmp=%s",szTmp);
	return szTmp;
}
double CSnmpPrinterMeterMonitorHP::ConvertIEEE754(const char* pBin)
{
	CString szBin = pBin;
	CString szBinTmp;
	CString szTmp;
	if (szBin.GetLength() <= 32)
	{
		for (int i = 0;i < 32 - szBin.GetLength();i++)
		{
			szTmp += "0";
		}
	}
	szBinTmp.Format("%s%s",szTmp,szBin);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,1,szBinTmp=%s",szBinTmp);
	CString decimals = szBinTmp.Right(23);
	int nSign = atoi(szBinTmp.Left(1));
	CString szEBin = szBinTmp.Mid(1,8);
	char cEBin[9]={0}; 
	strncpy(cEBin,szEBin.GetString(),szEBin.GetLength());
	//theLog.Write("ConvertIEEE754,2,cEBin=%s",cEBin);
	int nE = 0;
	for (int i = 0;i < 8; i++)
	{
		nE += (cEBin[8-i-1] - '0') * pow(2.0,i);
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,3,nE=%d",nE);
	CString szDes;
	szDes.Format("1.%s",decimals);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,4,szDes=%s",szDes);
	double fdecimals = 0.0;
	char cDecimal[24] = {0};
	strcpy(cDecimal,decimals.GetString());
	//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,5,cDecimal=%s",cDecimal);
	for (int i = 0;i < 23;i ++)
	{
		//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,6,cDecimal[i]-'0'=%d",cDecimal[i]-'0');
		fdecimals += (cDecimal[i]-'0')*pow(2.0,-i-1);
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,7,fdecimals=%f",fdecimals);
	double fDest = 1.0 + fdecimals;
	fDest = fDest * pow(2.0,nE-127);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::ConvertIEEE754,8,szDes=%f",fDest);
	//	st(szDes,'\0',2);
	return fDest;
}
BOOL CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP()
{
	int nValue = 0;
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_USEAGEPAGE_CM3530_MFP);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,1,szWebContent=%s",szWebContent);
	CString szEquivalentToalPrint = GetEquivalentTotalPrint_CM3530_MFP(szWebContent);
	CString szEquivalentA4ToalPrint = GetEquivalentA4TotalPrint_CM3530_MFP(szWebContent);
	CString szEquivalentA4HBToalPrint = GetEquivalentA4HBTotalPrint_CM3530_MFP(szWebContent);
	CString szEquivalentA4ColorToalPrint = GetEquivalentA4ColorTotalPrint_CM3530_MFP(szWebContent);
	CString szEquivalentHBToalPrint = GetEquivalentHBTotalPrint_CM3530_MFP(szWebContent);
	CString szEquivalentColorToalPrint = GetEquivalentColorTotalPrint_CM3530_MFP(szWebContent);
 	theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,1,szEquivalentToalPrint=%s,szEquivalentA4HBToalPrint=%s"
 		,szEquivalentToalPrint,szEquivalentA4HBToalPrint);

	if (szEquivalentToalPrint.GetLength() <= 0)	//总页数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentToalPrint.GetString()));
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentA4ToalPrint.GetLength() <= 0)//a4 打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,获取A4打印总页数失败");
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentA4ToalPrint.GetString()));
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
	}
	if (szEquivalentA4HBToalPrint.GetLength() <=0 )	//A4黑白打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,2,获取黑白打印的总页数失败");//如果支持复印，则也包含了复印黑白出纸数
	}
	else
	{
		nValue = DoubleToInt(atof(szEquivalentA4HBToalPrint.GetString()));
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
	}

	if (szEquivalentA4ColorToalPrint.GetLength() <= 0)	//A4彩色打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetMeterInfo_CM3530_MFP,4,获取彩色打印的总页数失败");
	}
	else
	{
		nValue =  DoubleToInt(atof(szEquivalentA4ColorToalPrint.GetString()));
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,5,获取彩色打印的总页数成功，nValue=%d", nValue);
	}

	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;


	m_oMeterInfo.nOtherOpHBPage = DoubleToInt(atof(szEquivalentHBToalPrint.GetString())) - DoubleToInt(atof(szEquivalentA4HBToalPrint.GetString()));

	m_oMeterInfo.nOtherOpColorPage = DoubleToInt(atof(szEquivalentColorToalPrint.GetString())) - DoubleToInt(atof(szEquivalentA4ColorToalPrint.GetString()));

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMeterInfo,100,end");
	return TRUE;

}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4TotalPrint_CM3530_MFP(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4TotalPrint_CM3530_MFP,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "A4";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4TotalPrint_CM3530_MFP,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 6;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4TotalPrint_CM3530_MFP,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4TotalPrint_CM3530_MFP,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}

CString CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CM3530_MFP(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CM3530_MFP,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "Total Printer Usage";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CM3530_MFP,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 3;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CM3530_MFP,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentTotalPrint_CM3530_MFP,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CM3530_MFP(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CM3530_MFP,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "A4";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CM3530_MFP,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 4;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CM3530_MFP,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4HBTotalPrint_CM3530_MFP,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CM3530_MFP(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CM3530_MFP,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "A4";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CM3530_MFP,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 5;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CM3530_MFP,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentA4ColorTotalPrint_CM3530_MFP,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CM3530_MFP(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CM3530_MFP,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "TOTAL PRINTER USAGE";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CM3530_MFP,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 2;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CM3530_MFP,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentColorTotalPrint_CM3530_MFP,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
CString CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CM3530_MFP(CString& szContent)
{
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CM3530_MFP,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "TOTAL PRINTER USAGE";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CM3530_MFP,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		for (int i = 0;i < 1;i++)
		{
			szFlag = "<div class=\"hpPageText\">";
			int nIndex1 = szTmp.Find(szFlag);
			//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CM3530_MFP,2,nIndex1=%d",nIndex1);
			if (nIndex1 >= 0)
			{
				szTmp = szTmp.Right(szTmp.GetLength()-szFlag.GetLength()-nIndex1);
			}
		}
		szFlag = "</div>";
		int nIndex2 = szTmp.Find(szFlag);
		//theLog.Write("CSnmpPrinterMeterMonitorHP::GetEquivalentHBTotalPrint_CM3530_MFP,3,nIndex2=%d",nIndex2);
		if (nIndex2 >= 0)
		{
			szTmp = szTmp.Left(nIndex2);
		}

	}
	szTmp.Replace(",","");
	return szTmp;
}
