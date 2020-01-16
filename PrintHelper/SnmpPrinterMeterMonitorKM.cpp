#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorKM.h"
#include "KonicaMinoltaMIB_Def.h"

CSnmpPrinterMeterMonitorKM::CSnmpPrinterMeterMonitorKM(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorKM::~CSnmpPrinterMeterMonitorKM(void)
{
}

void CSnmpPrinterMeterMonitorKM::InitOID()
{
	m_szKmA4CopyHBPageOID = DecryptOID(KmA4CopyHBPageOID);
	m_szKmA4CopyFullColorPageOID = DecryptOID(KmA4CopyFullColorPageOID);
	m_szKmA4CopySingleColorPageOID = DecryptOID(KmA4CopySingleColorPageOID);
	m_szKmA4CopyDoubleColorPage = DecryptOID(KmA4CopyDoubleColorPage);
	m_szKmA3CopyHBPageOID = DecryptOID(KmA3CopyHBPageOID);
	m_szKmA3CopyFullColorPageOID = DecryptOID(KmA3CopyFullColorPageOID);
	m_szKmA3CopySingleColorPageOID = DecryptOID(KmA3CopySingleColorPageOID);
	m_szKmA3CopyDoubleColorPageOID = DecryptOID(KmA3CopyDoubleColorPageOID);
	m_szKmA4PrintHBPageOID = DecryptOID(KmA4PrintHBPageOID);
	m_szKmA4PrintFullColorPageOID = DecryptOID(KmA4PrintFullColorPageOID);
	m_szKmA4PrintSingleColorPageOID = DecryptOID(KmA4PrintSingleColorPageOID);
	m_szKmA4PrintDoubleColorPageOID = DecryptOID(KmA4PrintDoubleColorPageOID);
	m_szKmA3PrintHBPageOID = DecryptOID(KmA3PrintHBPageOID);
	m_szKmA3PrintFullColorPageOID = DecryptOID(KmA3PrintFullColorPageOID);
	m_szKmA3PrintSingleColorPageOID = DecryptOID(KmA3PrintSingleColorPageOID);
	m_szKmA3PrintDoubleColorPageOID = DecryptOID(KmA3PrintDoubleColorPageOID);
	m_szKmScanTotalPageOID = DecryptOID(KmScanTotalPageOID);
	m_szKmScanBigColorPageOID = DecryptOID(KmScanBigColorPageOID);

	m_szSysDescrOID.Format("%s.1.%d", DecryptOID(SystemGroupOID), 0);//=SystemGroupOID
}

//检测打印机抄表计数大纸计数是否是双倍,如：1张A3,计数为2，实际的张数要将计数除以2。
BOOL CSnmpPrinterMeterMonitorKM::IsBigPaperDoubleCounter()
{
	CString szSysDescr = GetStrByOid(m_szSysDescrOID);
	if (szSysDescr.CompareNoCase("KONICA MINOLTA 306") == 0)
	{
		//KONICA MINOLTA 306，抄表计数策略：
		//1.打印1张大纸打印计数为2张大纸;
		//2.打印1张小纸打印计数为1张小纸;
		//3.复印1张大纸复印计数为2张大纸、并且小纸复印计数也为1张;
		//4.复印1张小纸计数为1张小纸。
		theLog.Write("CSnmpPrinterMeterMonitorKM::CheckCounterPolicy,szSysDescr=%s", szSysDescr);
		return TRUE;
	}
	return FALSE;
}

//纸张不能明确指示为纸型时，默认计算为A4
//打印、复印中关于计数，A3=2*A4
//现在修改抄表计数，A3和A4分开，不采用A3=2*A4的计数方式。
BOOL CSnmpPrinterMeterMonitorKM::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	BOOL bBigPaperDoubleCounter = IsBigPaperDoubleCounter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4CopyHBPageOID), nValue))	//复印黑色页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,2,获取复印黑色页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,3,获取复印黑色页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4CopyFullColorPageOID), nValue))	//复印全彩页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.2.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,4,获取复印全彩页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,5,获取复印全彩页数成功，nValue=%d", nValue);
	}

	//（如果不存在[2彩色]节点[.4.1]时，此节点变成[2彩色]）
	if (!GetRequest(CStringToChar(m_szKmA4CopySingleColorPageOID), nValue))	//复印单彩色页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,6,获取复印单彩色页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,7,获取复印单彩色页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4CopyDoubleColorPage), nValue))	//复印2彩色页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.4.1"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,8,获取复印2彩色页数失败");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,9,获取复印2彩色页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3CopyHBPageOID), nValue))	//复印黑色（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,2,获取复印黑色（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,3,获取复印黑色（最大尺寸）页数成功，nValue=%d", nValue);
	}

	if (bBigPaperDoubleCounter)
	{
		m_oMeterInfo.nA3CopyHBPage = m_oMeterInfo.nA3CopyHBPage / 2;
	}
	if ((m_oMeterInfo.nA3CopyHBPage>0) && (m_oMeterInfo.nA4CopyHBPage>0))
	{
		if (bBigPaperDoubleCounter)
		{
			m_oMeterInfo.nA4CopyHBPage -= m_oMeterInfo.nA3CopyHBPage;
		}
		else
		{
			m_oMeterInfo.nA4CopyHBPage -= (m_oMeterInfo.nA3CopyHBPage*2);
		}
	}

	if (!GetRequest(CStringToChar(m_szKmA3CopyFullColorPageOID), nValue))	//复印全彩（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.2.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,4,获取复印全彩（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,5,获取复印全彩（最大尺寸）页数成功，nValue=%d", nValue);
	}

	//（如果不存在[2彩色]节点[.4.1]时，此节点变成[2彩色]）
	if (!GetRequest(CStringToChar(m_szKmA3CopySingleColorPageOID), nValue))	//复印单彩色（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,6,获取复印单彩色（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,7,获取复印单彩色（最大尺寸）页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3CopyDoubleColorPageOID), nValue))	//复印2彩色（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.4.1"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,8,获取复印2彩色（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,9,获取复印2彩色（最大尺寸）页数成功，nValue=%d", nValue);
	}
	if (bBigPaperDoubleCounter)
	{
		m_oMeterInfo.nA3CopyColorPage = m_oMeterInfo.nA3CopyColorPage / 2;
	}
	if ((m_oMeterInfo.nA3CopyColorPage > 0) && (m_oMeterInfo.nA4CopyColorPage>0))
	{
		if (bBigPaperDoubleCounter)
		{
			m_oMeterInfo.nA4CopyColorPage -= m_oMeterInfo.nA3CopyColorPage;
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage -= (m_oMeterInfo.nA3CopyColorPage*2);
		}
	}

	if (!GetRequest(CStringToChar(m_szKmA4PrintHBPageOID), nValue))	//打印黑色页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,10,获取打印黑色页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,11,获取打印黑色页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4PrintFullColorPageOID), nValue))	//打印全彩页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.2.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,12,获取打印全彩页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,13,获取打印全彩页数成功，nValue=%d", nValue);
	}

	//打印单彩页数（如果不存在[2彩色]节点[.4.1]时，此节点变成[2彩色]，如果存在[2彩色]节点[.4.1]时，此节点无效=[-1]）
	if (!GetRequest(CStringToChar(m_szKmA4PrintSingleColorPageOID), nValue))	//打印单彩色页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.3.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,14,获取打印单彩色页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,15,获取打印单彩色页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4PrintDoubleColorPageOID), nValue))	//打印2彩色页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.4.2"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,16,获取打印2彩色页数失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,17,获取打印2彩色页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3PrintHBPageOID), nValue))	//打印黑色（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,10,获取打印黑色（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,11,获取打印黑色（最大尺寸）页数成功，nValue=%d", nValue);
	}
	if (bBigPaperDoubleCounter)
	{
		m_oMeterInfo.nA3PrintHBPage = m_oMeterInfo.nA3PrintHBPage / 2;
	}
	if ((m_oMeterInfo.nA3PrintHBPage>0) && (m_oMeterInfo.nA4PrintHBPage>0))
	{
		if (bBigPaperDoubleCounter)
		{
			//m_oMeterInfo.nA4PrintHBPage -= m_oMeterInfo.nA3PrintHBPage;
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage -= (m_oMeterInfo.nA3PrintHBPage*2);
		}
	}

	if (!GetRequest(CStringToChar(m_szKmA3PrintFullColorPageOID), nValue))	//打印全彩（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.2.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,12,获取打印全彩（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,13,获取打印全彩（最大尺寸）页数成功，nValue=%d", nValue);
	}

	//打印单彩（最大尺寸）页数（如果不存在[2彩色（最大尺寸）]节点[.4.2]时，此节点变成[2彩色]，如果存在[2彩色]节点[.4.1]时，此节点无效=[-1]）
	if (!GetRequest(CStringToChar(m_szKmA3PrintSingleColorPageOID), nValue))	//打印单彩色（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.3.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,14,获取打印单彩色（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,15,获取打印单彩色（最大尺寸）页数成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3PrintDoubleColorPageOID), nValue))	//打印2彩色（最大尺寸）页数=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.4.2"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,16,获取打印2彩色（最大尺寸）页数失败");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,17,获取打印2彩色（最大尺寸）页数成功，nValue=%d", nValue);
	}
	if (bBigPaperDoubleCounter)
	{
		m_oMeterInfo.nA3PrintColorPage = m_oMeterInfo.nA3PrintColorPage / 2;
	}
	if ((m_oMeterInfo.nA3PrintColorPage>0) && (m_oMeterInfo.nA4PrintColorPage>0))
	{
		if (bBigPaperDoubleCounter)
		{
			//m_oMeterInfo.nA4PrintColorPage -= m_oMeterInfo.nA3PrintColorPage;
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage -= (m_oMeterInfo.nA3PrintColorPage*2);
		}
	}

	if (!GetRequest(CStringToChar(m_szKmScanTotalPageOID), nValue))	//扫描总计=".1.3.6.1.4.1.18334.1.1.1.5.7.2.3.1.5.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,15,获取扫描总计失败");
	}
	else
	{
		m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,18,获取扫描总计成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmScanBigColorPageOID), nValue))	//扫描最大尺寸总计=".1.3.6.1.4.1.18334.1.1.1.5.7.2.3.1.6.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,19,获取扫描最大尺寸总计失败");
	}
	else
	{
		m_oMeterInfo.nScanBigColorPage = (nValue>0) ? nValue : 0;
		if (m_oMeterInfo.nScanTotalPage >= m_oMeterInfo.nScanBigColorPage)
		{
			m_oMeterInfo.nScanSmallColorPage = m_oMeterInfo.nScanTotalPage - m_oMeterInfo.nScanBigColorPage;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,20,获取扫描最大尺寸总计成功，nValue=%d", nValue);
	}


	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - (m_oMeterInfo.nA3HBTotalPage*2) - (m_oMeterInfo.nA3ColorTotalPage*2)- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage = other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,100,end");

	return TRUE;
}
