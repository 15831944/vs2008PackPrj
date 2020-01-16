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

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szSeikoAllTotalDistanceOID), nValue))	//�ܴ�ӡ����=".1.3.6.1.4.1.263.2.2.2.1.3.4.1.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,2,��ȡ�ܴ�ӡ����");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,3,��ȡ�ܴ�ӡ���ȣ�nValue=%d��", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoAllTotalPageOID), nValue))	//�ܴ�ӡ������=".1.3.6.1.4.1.263.2.2.2.1.3.4.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,4,��ȡ�ܴ�ӡ������ʧ��");
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,5,��ȡ�ܴ�ӡ�������ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPrintDistanceOID), nValue))	//��ӡ����=".1.3.6.1.4.1.263.2.2.2.6.1.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,6,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nPrintDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,7,��ȡ��ӡ���ȳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPrintAreaOID), nValue))	//��ӡ���=".1.3.6.1.4.1.263.2.2.2.6.2.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,8,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nPrintArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,9,��ȡ��ӡ���ȳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoCopyDistanceOID), nValue))	//��ӡ����=".1.3.6.1.4.1.263.2.2.2.6.1.4.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,10,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nCopyDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,11,��ȡ��ӡ���ȳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoCopyAreaOID), nValue))	//��ӡ���=".1.3.6.1.4.1.263.2.2.2.6.2.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,12,��ȡ��ӡ���ʧ��");
	}
	else
	{
		m_oMeterInfo.nCopyArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,13,��ȡ��ӡ����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper1LevelPercentOID), nValue))	//ֽ��1ʣ����=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,14,��ȡֽ��1ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,15,��ȡֽ��1ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper2LevelPercentOID), nValue))	//ֽ��2ʣ����=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,16,��ȡֽ��2ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,17,��ȡֽ��2ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper3LevelPercentOID), nValue))	//ֽ��3ʣ����=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,18,��ȡֽ��3ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,19,��ȡֽ��3ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoPaper4LevelPercentOID), nValue))	//ֽ��4ʣ����=".1.3.6.1.4.1.263.2.2.2.2.2.2.1.4.4"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,18,��ȡֽ��4ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,19,��ȡֽ��4ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSeikoTonerBlackLevelPercentOID), nValue))	//��ɫ̼��ʣ����=".1.3.6.1.4.1.263.2.2.2.2.4.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,18,��ȡ��ɫ̼��ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorSEIKO::GetMeterInfo,19,��ȡ��ɫ̼��ʣ�����ɹ���nValue=%d", nValue);
	}

	return TRUE;
}