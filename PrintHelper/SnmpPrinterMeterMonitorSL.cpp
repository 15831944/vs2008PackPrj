/*************2016-06-27 修改********************
	增加A3黑白总使用量的抄表 
注意： 总页数 不等于 A3黑白 + A3彩色 + A4黑白 + A4彩色
	因为 A4黑白和A4彩色里面已经包括了A3的换算
*************************************************/


#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorSL.h"
#include "XeroxMIB_Def.h"
#define DEF_HTTP	"http://"
#define DEF_COUNT_ApeosPort_V_C3373		"/prcnt.htm"

CSnmpPrinterMeterMonitorSL::CSnmpPrinterMeterMonitorSL(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorSL::~CSnmpPrinterMeterMonitorSL(void)
{
}

void CSnmpPrinterMeterMonitorSL::InitOID()
{
	m_szXeroxA4PrintHBPageOID = DecryptOID(XeroxA4PrintHBPageOID);
	m_szXeroxA4PrintColorPageOID = DecryptOID(XeroxA4PrintColorPageOID);
	m_szXeroxA4CopyHBPageOID = DecryptOID(XeroxA4CopyHBPageOID);
	m_szXeroxA4CopyColorPageOID = DecryptOID(XeroxA4CopyColorPageOID);
	m_szXeroxScannedImagesStoredOID = DecryptOID(XeroxScannedImagesStoredOID);
	m_szXeroxNetworkScanningImagesSentOID = DecryptOID(XeroxNetworkScanningImagesSentOID);
	m_szXeroxA4FaxTotalPageOID = DecryptOID(XeroxA4FaxTotalPageOID);
	m_szXeroxA4ColorTotalPageOID = DecryptOID(XeroxA4ColorTotalPageOID);
	m_szXeroxA4HBTotalPageOID = DecryptOID(XeroxA4HBTotalPageOID);
	m_szXeroxA3ColorTotalPageOID = DecryptOID(XeroxA3ColorTotalPageOID);
	m_szXeroxA3HBTotalPageOID = DecryptOID(XeroxA3HBTotalPageOID);
	m_szSLPrinterModelOID = DecryptOID(XeroxPrintModel);
	m_szXeroxA3HBTotalPageOID_DocuCentre_S2110 = DecryptOID(XeroxA3HBTotalPageOIDDocuCentre_S2110);
	m_szSLPrintModel = "";
}
CString CSnmpPrinterMeterMonitorSL::GetSLModel()
{
	CString szModel;
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szSLPrinterModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetSLModel,获取型号失败");
	}
	szModel.Format("%s",tmp);
	theLog.Write("CSnmpPrinterMeterMonitorSL::GetSLModel,szModel=%s",szModel);
	szModel.Trim();
	return szModel;

}

BOOL CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTP,m_szIP,DEF_COUNT_ApeosPort_V_C3373);
	theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetMeterByUrl(szUrl);
	szWebContent = CCommonFun::MultiByteToUTF8(szWebContent.GetString());
	//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,1,szWebContent=%s",szWebContent);
	CString szA4PrintTotal = GetDetailMeterInfoByFlag("Total Printed Impressions",szWebContent);
	CString szA4PrintHB = GetDetailMeterInfoByFlag("Black Printed Impressions",szWebContent);
	CString szA4PrintColor = GetDetailMeterInfoByFlag("Color Printed Impressions",szWebContent);
	//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,1,szA4PrintHB=%s",szA4PrintHB);
	CString szA4CopyTotal = GetDetailMeterInfoByFlag("Total Copied Impressions",szWebContent);
	CString szA4CopyColor = GetDetailMeterInfoByFlag("Color Copied Impressions",szWebContent);
	CString szA4CopyHB = GetDetailMeterInfoByFlag("Black Copied Impressions",szWebContent);
	CString szA4ScanTotal = GetDetailMeterInfoByFlag("Total Scanned Images",szWebContent);
	CString szA4ScanColor = GetDetailMeterInfoByFlag("Color Scanned Images",szWebContent);
	CString szA4ScanHB = GetDetailMeterInfoByFlag("Black Scanned Images",szWebContent);
	theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373"
		",szA4PrintTotal=%s,szA4PrintHB=%s,szA4PrintColor=%s,szA4CopyTotal=%s"
		",szA4CopyColor=%s,szA4CopyHB=%s,szA4ScanTotal=%s"
		,szA4PrintTotal,szA4PrintHB,szA4PrintColor,szA4CopyTotal,szA4CopyColor,szA4CopyHB,szA4ScanTotal);
	int nValue = 0;
	//打印
	if (szA4PrintTotal.GetLength() <= 0)//a4 打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4打印总页数失败");
	}
	else
	{
		nValue = atoi(szA4PrintTotal.GetString());
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4PrintHB.GetLength() <= 0)//a4 黑白打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4打印黑白总页数失败");
	}
	else
	{
		nValue = atoi(szA4PrintHB.GetString());
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4PrintColor.GetLength() <= 0)//a4 彩色打印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4打印彩色总页数失败");
	}
	else
	{
		nValue = atoi(szA4PrintColor.GetString());
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
	}
	//复印
	if (szA4CopyTotal.GetLength() <= 0)//a4 复印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4复印总页数失败");
	}
	else
	{
		nValue = atoi(szA4PrintTotal.GetString());
		m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4CopyHB.GetLength() <= 0)//a4 黑白复印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4复印黑白总页数失败");
	}
	else
	{
		nValue = atoi(szA4CopyHB.GetString());
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4CopyColor.GetLength() <= 0)//a4 彩色复印
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4打印彩色总页数失败");
	}
	else
	{
		nValue = atoi(szA4CopyColor.GetString());
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
	}
// 扫描
	nValue = 0;
	if (szA4ScanTotal.GetLength() <= 0)//a4 扫描
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,获取A4扫描总页数失败");
	}
	else
	{
		nValue = atoi(szA4ScanTotal.GetString());
		m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
	}
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage + m_oMeterInfo.nScanTotalPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	m_oMeterInfo.nAllTotalPage = m_oMeterInfo.nA4HBTotalPage + m_oMeterInfo.nA4ColorTotalPage;



	return TRUE;
}
CString CSnmpPrinterMeterMonitorSL::GetDetailMeterInfoByFlag(CString szFlag,CString szWebContent)
{
	CString szTmp = "";
	CString szFlagEx = szFlag + "',";
	int indx1 = szWebContent.Find(szFlagEx);
	if (indx1 >= 0)
	{
		szTmp = szWebContent.Right(szWebContent.GetLength()-indx1-szFlagEx.GetLength());
		szTmp.Trim();
		int nIdex2 = szTmp.Find(",");
		if (nIdex2 >= 0)
		{
			szTmp = szTmp.Left(nIdex2);
		}
	}
	return szTmp;
}
//纸张不能明确指示为纸型时，默认计算为A4
//打印、复印中关于计数，A3=2*A4
BOOL CSnmpPrinterMeterMonitorSL::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	m_szSLPrintModel = GetSLModel();
	if(m_szSLPrintModel.Find("ApeosPort-V C3373") >= 0)
	{
		return GetMeterInfo_ApeosPort_V_C3373();		
	}
	else
	{
		int nValue = 0;

		if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,获取总页数失败");
			return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4PrintHBPageOID), nValue))	//Black Printed Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.7"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,2,获取黑白打印的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,3,获取黑白打印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4PrintColorPageOID), nValue))	//Color Printed Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.29"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,获取彩色打印的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,获取彩色打印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4CopyHBPageOID), nValue))	//Black Copied Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.103.20.3"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,6,获取黑白复印的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,7,获取黑白复印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4CopyColorPageOID), nValue))	//Color Copied Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.103.20.25"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,8,获取彩色复印的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,9,获取彩色复印的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxScannedImagesStoredOID), nValue))	//Scanned Images Stored=".1.3.6.1.4.1.253.8.53.13.2.1.6.102.20.21"
		{
			if (!GetRequest(CStringToChar(m_szXeroxNetworkScanningImagesSentOID), nValue))	//Network Scanning Images Sent=".1.3.6.1.4.1.253.8.53.13.2.1.6.102.20.11"
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,10,获取扫描的总页数失败");
			}
			else
			{
				m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
				//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,11,获取扫描的总页数成功，nValue=%d", nValue);
			}
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,12,获取扫描的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4FaxTotalPageOID), nValue))	//Fax Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.71"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,13,获取传真的总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4FaxTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,14,获取传真的总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4ColorTotalPageOID), nValue))	//彩色总数=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.33"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,获取黑白A4总数失败");
		}
		else
		{
			m_oMeterInfo.nA4ColorTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,获取黑白A4总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4HBTotalPageOID), nValue))	//黑白总数=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.34"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,获取黑白总数失败");
		}
		else
		{
			m_oMeterInfo.nA4HBTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,获取黑白总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA3ColorTotalPageOID), nValue))	//彩色A3总数=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.43"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,获取彩色A3总数失败");
		}
		else
		{
			m_oMeterInfo.nA3ColorTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,获取彩色A3总数成功，nValue=%d", nValue);
		}
		if (m_szSLPrintModel.Find("DocuCentre S2110") >=0 )
		{
			if (!GetRequest(CStringToChar(m_szXeroxA3HBTotalPageOID_DocuCentre_S2110), nValue))	//黑白A3总数=".1.3.6.1.4.1.253.8.53.13.2.1.6.101.20.5"
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,DocuCentre_S2110,获取黑白A3总数失败");
			}
			else
			{
				m_oMeterInfo.nA3HBTotalPage = (nValue>0) ? nValue : 0;
				//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,7,获取黑白A3总数成功，nValue=%d", nValue);
			}
		}
		else
		{
			if (!GetRequest(CStringToChar(m_szXeroxA3HBTotalPageOID), nValue))	//黑白A3总数=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.44"
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,6,获取黑白A3总数失败");
			}
			else
			{
				m_oMeterInfo.nA3HBTotalPage = (nValue>0) ? nValue : 0;
				//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,7,获取黑白A3总数成功，nValue=%d", nValue);
			}
		}
		

		int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage;
		if(other > 0)
		{
			m_oMeterInfo.nOtherOpHBPage += other;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,100,end");
		return TRUE;
	}
}
CString CSnmpPrinterMeterMonitorSL::GetMeterByUrl(CString szUrl)
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