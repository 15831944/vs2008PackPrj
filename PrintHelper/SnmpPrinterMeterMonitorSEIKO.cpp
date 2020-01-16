#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorSEIKO.h"
#include "SeikoMIB_Def.h"

CSnmpPrinterMeterMonitorSEIKO::CSnmpPrinterMeterMonitorSEIKO(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorSEIKO::~CSnmpPrinterMeterMonitorSEIKO(void)
{
}

void CSnmpPrinterMeterMonitorSEIKO::InitOID()
{
	m_szSeikoAllTotalDistanceOID = DecryptOID(SeikoAllTotalDistanceOID);
	m_szSeikoAllTotalPageOID = DecryptOID(SeikoAllTotalPageOID);
	m_szSeikoPrintDistanceOID = DecryptOID(SeikoPrintDistanceOID);
	m_szSeikoPrintAreaOID = DecryptOID(SeikoPrintAreaOID);
	m_szSeikoCopyDistanceOID = DecryptOID(SeikoCopyDistanceOID);
	m_szSeikoCopyAreaOID = DecryptOID(SeikoCopyAreaOID);
	m_szSeikoPaper1LevelPercentOID = DecryptOID(SeikoPaper1LevelPercentOID);
	m_szSeikoPaper2LevelPercentOID = DecryptOID(SeikoPaper2LevelPercentOID);
	m_szSeikoPaper3LevelPercentOID = DecryptOID(SeikoPaper3LevelPercentOID);
	m_szSeikoPaper4LevelPercentOID = DecryptOID(SeikoPaper4LevelPercentOID);
	m_szSeikoTonerBlackLevelPercentOID = DecryptOID(SeikoTonerBlackLevelPercentOID);
}

BOOL CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szSeikoAllTotalDistanceOID), nValue))	//总打印长度=".1.3.6.1.4.1.263.2.2.2.1.3.4.1.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,2,获取总打印长度");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,3,获取总打印长度，nValue=%d米", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoAllTotalPageOID), nValue))	//总打印计数器=".1.3.6.1.4.1.263.2.2.2.1.3.4.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,4,获取总打印计数器失败");
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,5,获取总打印计数器成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPrintDistanceOID), nValue))	//打印长度=".1.3.6.1.4.1.263.2.2.2.6.1.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,6,获取打印长度失败");
	}
	else
	{
		m_oMeterInfo.nPrintDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,7,获取打印长度成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPrintAreaOID), nValue))	//打印面积=".1.3.6.1.4.1.263.2.2.2.6.2.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,8,获取打印长度失败");
	}
	else
	{
		m_oMeterInfo.nPrintArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,9,获取打印长度成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoCopyDistanceOID), nValue))	//复印长度=".1.3.6.1.4.1.263.2.2.2.6.1.4.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,10,获取复印长度失败");
	}
	else
	{
		m_oMeterInfo.nCopyDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,11,获取复印长度成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoCopyAreaOID), nValue))	//复印面积=".1.3.6.1.4.1.263.2.2.2.6.2.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,12,获取复印面积失败");
	}
	else
	{
		m_oMeterInfo.nCopyArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,13,获取复印面积成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper1LevelPercentOID), nValue))	//纸盒1剩余量=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,14,获取纸盒1剩余量失败");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,15,获取纸盒1剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper2LevelPercentOID), nValue))	//纸盒2剩余量=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,16,获取纸盒2剩余量失败");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,17,获取纸盒2剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper3LevelPercentOID), nValue))	//纸盒3剩余量=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,18,获取纸盒3剩余量失败");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,19,获取纸盒3剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper4LevelPercentOID), nValue))	//纸盒4剩余量=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.4"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,18,获取纸盒4剩余量失败");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,19,获取纸盒4剩余量成功，nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoTonerBlackLevelPercentOID), nValue))	//黑色碳粉剩余量=".1.3.6.1.4.1.263.2.2.2.2.4.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,18,获取黑色碳粉剩余量失败");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,19,获取黑色碳粉剩余量成功，nValue=%d", nValue);
	}

	return TRUE;
}