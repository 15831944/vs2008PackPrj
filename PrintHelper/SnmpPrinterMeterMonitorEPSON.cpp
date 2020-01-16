#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorEPSON.h"
#include "EpsonMIB_Def.h"

CSnmpPrinterMeterMonitorEPSON::CSnmpPrinterMeterMonitorEPSON(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorEPSON::~CSnmpPrinterMeterMonitorEPSON(void)
{
}

void CSnmpPrinterMeterMonitorEPSON::InitOID()
{
	m_szEpsonA4PrintHBPageOID = DecryptOID(EpsonA4PrintHBPageOID);
	m_szEpsonA4PrintTotalPageOID = DecryptOID(EpsonA4PrintTotalPageOID);
	m_szEpsonA4PrintColorPageOID = DecryptOID(EpsonA4PrintColorPageOID);
	m_szEpsonA4CopyTotalPageOID = DecryptOID(EpsonA4CopyTotalPageOID);
	m_szEpsonA4CopyColorPageOID = DecryptOID(EpsonA4CopyColorPageOID);
	m_szEpsonOtherOpTotalPageOID = DecryptOID(EpsonOtherOpTotalPageOID);
	m_szEpsonOtherOpColorPageOID = DecryptOID(EpsonOtherOpColorPageOID);
}

BOOL CSnmpPrinterMeterMonitorEPSON::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4PrintHBPageOID), nValue))	//�ڰ�-����=".1.3.6.1.4.1.1248.1.2.2.27.1.1.3.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,2,��ȡ�ڰ׵���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,3,��ȡ�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4PrintTotalPageOID), nValue))	//��ӡ�������ڰ�+��ɫ=".1.3.6.1.4.1.1248.1.2.2.27.6.1.4.1.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,4,��ȡ��ӡ�������ڰ�+��ɫ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,5,��ȡ��ӡ�������ڰ�+��ɫ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4PrintColorPageOID), nValue))	//��ӡ��ɫ=".1.3.6.1.4.1.1248.1.2.2.27.6.1.5.1.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,6,��ȡ��ӡ��ɫʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		if(m_oMeterInfo.nA4PrintTotalPage - m_oMeterInfo.nA4PrintColorPage > 0)
		{//��ӡ����-��ɫ��ӡ
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintTotalPage - m_oMeterInfo.nA4PrintColorPage;
		}
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,7,��ȡ��ӡ�ڰ׳ɹ���nValue=%d", m_oMeterInfo.nA4PrintHBPage);
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,8,��ȡ��ӡ��ɫ�ɹ���nValue=%d", m_oMeterInfo.nA4PrintColorPage);
	}


	if (!GetRequest(CStringToChar(m_szEpsonA4CopyTotalPageOID), nValue))	//��ӡ����=".1.3.6.1.4.1.1248.1.2.2.27.6.1.4.1.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,9.2,��ȡ��ӡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,9.3,��ȡ��ӡ�����ɹ�,nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonA4CopyColorPageOID), nValue))	//��ӡ��ɫ=".1.3.6.1.4.1.1248.1.2.2.27.6.1.5.1.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,11,��ȡ��ӡ��ɫʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
		if(m_oMeterInfo.nA4CopyTotalPage - m_oMeterInfo.nA4CopyColorPage > 0)
		{
			m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage - m_oMeterInfo.nA4CopyColorPage;
		}
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,12,��ȡ��ӡ�ڰ׳ɹ���nValue=%d", m_oMeterInfo.nA4CopyHBPage);
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,13,��ȡ��ӡ��ɫ�ɹ���nValue=%d", m_oMeterInfo.nA4CopyColorPage);
	}

	if (!GetRequest(CStringToChar(m_szEpsonOtherOpTotalPageOID), nValue))	//����������ֽ����=".1.3.6.1.4.1.1248.1.2.2.27.6.1.4.1.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,14,��ȡ����������ֽʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,15,��ȡ����������ֽ�ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szEpsonOtherOpColorPageOID), nValue))	//����������ɫ��ֽ=".1.3.6.1.4.1.1248.1.2.2.27.6.1.5.1.1.4"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,16,��ȡ����������ɫ��ֽʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage = (nValue>0) ? nValue : 0;
		if(m_oMeterInfo.nOtherOpTotalPage - m_oMeterInfo.nOtherOpColorPage > 0)
		{
			m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nOtherOpTotalPage - m_oMeterInfo.nOtherOpColorPage;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,17,��ȡ���������ڰ���ֽ�ɹ���nValue=%d", m_oMeterInfo.nOtherOpHBPage);
		//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo,18,��ȡ����������ɫ��ֽ�ɹ���nValue=%d", m_oMeterInfo.nOtherOpColorPage);
	}
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorEPSON::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	return TRUE;
}