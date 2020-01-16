#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorJT.h"
#include "JingTuMIB_Def.h"

CSnmpPrinterMeterMonitorJT::CSnmpPrinterMeterMonitorJT(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorJT::~CSnmpPrinterMeterMonitorJT(void)
{
}

void CSnmpPrinterMeterMonitorJT::InitOID()
{
	m_szJingTuAllTotalDistanceOID = DecryptOID(JingTuAllTotalDistanceOID);
	m_szJingTuAllTotalPageOID = DecryptOID(JingTuAllTotalPageOID);
	m_szJingTuPrintDistanceOID = DecryptOID(JingTuPrintDistanceOID);
	m_szJingTuPrintAreaOID = DecryptOID(JingTuPrintAreaOID);
	m_szJingTuCopyDistanceOID = DecryptOID(JingTuCopyDistanceOID);
	m_szJingTuCopyAreaOID = DecryptOID(JingTuCopyAreaOID);
	m_szJingTuPaper1LevelPercentOID = DecryptOID(JingTuPaper1LevelPercentOID);
	m_szJingTuPaper2LevelPercentOID = DecryptOID(JingTuPaper2LevelPercentOID);
	m_szJingTuPaper3LevelPercentOID = DecryptOID(JingTuPaper3LevelPercentOID);
	m_szJingTuPaper4LevelPercentOID = DecryptOID(JingTuPaper4LevelPercentOID);
	m_szJingTuTonerBlackLevelPercentOID = DecryptOID(JingTuTonerBlackLevelPercentOID);
}

BOOL CSnmpPrinterMeterMonitorJT::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szJingTuAllTotalDistanceOID), nValue))	//总打印长度=".1.3.6.1.4.1.44966.2.2.2.1.3.4.1.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,2,获取总打印长度");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,3,获取总打印长度，nValue=%d米", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuAllTotalPageOID), nValue))	//总打印计数器=".1.3.6.1.4.1.44966.2.2.2.1.3.4.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,4,获取总打印计数器失败");
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,5,获取总打印计数器成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPrintDistanceOID), nValue))	//打印长度=".1.3.6.1.4.1.44966.2.2.2.6.1.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,6,获取打印长度失败");
	}
	else
	{
		m_oMeterInfo.nPrintDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,7,获取打印长度成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPrintAreaOID), nValue))	//打印面积=".1.3.6.1.4.1.44966.2.2.2.6.2.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,8,获取打印长度失败");
	}
	else
	{
		m_oMeterInfo.nPrintArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,9,获取打印长度成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuCopyDistanceOID), nValue))	//复印长度=".1.3.6.1.4.1.44966.2.2.2.6.1.4.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,10,获取复印长度失败");
	}
	else
	{
		m_oMeterInfo.nCopyDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,11,获取复印长度成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuCopyAreaOID), nValue))	//复印面积=".1.3.6.1.4.1.44966.2.2.2.6.2.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,12,获取复印面积失败");
	}
	else
	{
		m_oMeterInfo.nCopyArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,13,获取复印面积成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper1LevelPercentOID), nValue))	//纸盒1剩余量=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,14,获取纸盒1剩余量失败");
	}
	else
	{
		if(nValue == 3)
		{
			strcpy(m_oMeterInfo.cPaper1LevelPercent, "100%");
		}
		else if(nValue == 4)
		{
			strcpy(m_oMeterInfo.cPaper1LevelPercent, "25%");
		}
		else if(nValue == 5)
		{
			strcpy(m_oMeterInfo.cPaper1LevelPercent, "0%");
		}
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,15,获取纸盒1剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper2LevelPercentOID), nValue))	//纸盒2剩余量=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,16,获取纸盒2剩余量失败");
	}
	else
	{
		if(nValue == 3)
		{
			strcpy(m_oMeterInfo.cPaper2LevelPercent, "100%");
		}
		else if(nValue == 4)
		{
			strcpy(m_oMeterInfo.cPaper2LevelPercent, "25%");
		}
		else if(nValue == 5)
		{
			strcpy(m_oMeterInfo.cPaper2LevelPercent, "0%");
		}
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,17,获取纸盒2剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper3LevelPercentOID), nValue))	//纸盒3剩余量=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,18,获取纸盒3剩余量失败");
	}
	else
	{
		if(nValue == 3)
		{
			strcpy(m_oMeterInfo.cPaper3LevelPercent, "100%");
		}
		else if(nValue == 4)
		{
			strcpy(m_oMeterInfo.cPaper3LevelPercent, "25%");
		}
		else if(nValue == 5)
		{
			strcpy(m_oMeterInfo.cPaper3LevelPercent, "0%");
		}
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,19,获取纸盒3剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper4LevelPercentOID), nValue))	//纸盒4剩余量=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.4"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,20,获取纸盒4剩余量失败");
	}
	else
	{
		if(nValue == 3)
		{
			strcpy(m_oMeterInfo.cPaper4LevelPercent, "100%");
		}
		else if(nValue == 4)
		{
			strcpy(m_oMeterInfo.cPaper4LevelPercent, "25%");
		}
		else if(nValue == 5)
		{
			strcpy(m_oMeterInfo.cPaper4LevelPercent, "0%");
		}
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,21,获取纸盒4剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuTonerBlackLevelPercentOID), nValue))	//黑色碳粉剩余量=".1.3.6.1.4.1.44966.2.2.2.2.4.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,22,获取黑色碳粉剩余量失败");
	}
	else
	{
		if(nValue == 3)
		{
			strcpy(m_oMeterInfo.cTonerBlackLevelPercent, "100%");
		}
		else if(nValue == 4)
		{
			strcpy(m_oMeterInfo.cTonerBlackLevelPercent, "25%");
		}
		else if(nValue == 5)
		{
			strcpy(m_oMeterInfo.cTonerBlackLevelPercent, "0%");
		}
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,23获取黑色碳粉剩余量成功，nValue=%d", nValue);
	}

	return TRUE;
}