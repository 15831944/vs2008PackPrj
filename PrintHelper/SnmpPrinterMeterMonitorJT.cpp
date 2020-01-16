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

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szJingTuAllTotalDistanceOID), nValue))	//�ܴ�ӡ����=".1.3.6.1.4.1.44966.2.2.2.1.3.4.1.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,2,��ȡ�ܴ�ӡ����");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,3,��ȡ�ܴ�ӡ���ȣ�nValue=%d��", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuAllTotalPageOID), nValue))	//�ܴ�ӡ������=".1.3.6.1.4.1.44966.2.2.2.1.3.4.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,4,��ȡ�ܴ�ӡ������ʧ��");
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,5,��ȡ�ܴ�ӡ�������ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPrintDistanceOID), nValue))	//��ӡ����=".1.3.6.1.4.1.44966.2.2.2.6.1.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,6,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nPrintDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,7,��ȡ��ӡ���ȳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPrintAreaOID), nValue))	//��ӡ���=".1.3.6.1.4.1.44966.2.2.2.6.2.2.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,8,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nPrintArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,9,��ȡ��ӡ���ȳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuCopyDistanceOID), nValue))	//��ӡ����=".1.3.6.1.4.1.44966.2.2.2.6.1.4.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,10,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nCopyDistance = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,11,��ȡ��ӡ���ȳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuCopyAreaOID), nValue))	//��ӡ���=".1.3.6.1.4.1.44966.2.2.2.6.2.3.0"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,12,��ȡ��ӡ���ʧ��");
	}
	else
	{
		m_oMeterInfo.nCopyArea = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,13,��ȡ��ӡ����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper1LevelPercentOID), nValue))	//ֽ��1ʣ����=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,14,��ȡֽ��1ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,15,��ȡֽ��1ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper2LevelPercentOID), nValue))	//ֽ��2ʣ����=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,16,��ȡֽ��2ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,17,��ȡֽ��2ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper3LevelPercentOID), nValue))	//ֽ��3ʣ����=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,18,��ȡֽ��3ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,19,��ȡֽ��3ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuPaper4LevelPercentOID), nValue))	//ֽ��4ʣ����=".1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4.4"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,20,��ȡֽ��4ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,21,��ȡֽ��4ʣ�����ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szJingTuTonerBlackLevelPercentOID), nValue))	//��ɫ̼��ʣ����=".1.3.6.1.4.1.44966.2.2.2.2.4.2.1.4.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJT::GetMeterInfo,22,��ȡ��ɫ̼��ʣ����ʧ��");
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
		//theLog.Write("CSnmpPrinterMeterMonitorJT::GetMeterInfo,23��ȡ��ɫ̼��ʣ�����ɹ���nValue=%d", nValue);
	}

	return TRUE;
}