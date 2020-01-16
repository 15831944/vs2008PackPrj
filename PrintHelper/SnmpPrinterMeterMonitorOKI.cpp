#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorOKI.h"
#include "OkiMIB_Def.h"

CSnmpPrinterMeterMonitorOKI::CSnmpPrinterMeterMonitorOKI(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorOKI::~CSnmpPrinterMeterMonitorOKI(void)
{
}

void CSnmpPrinterMeterMonitorOKI::InitOID()
{
	m_szOkiA4PrintTotalPageOID = DecryptOID(OkiA4PrintTotalPageOID);
	m_szOkiA4PrintColorPageOID = DecryptOID(OkiA4PrintColorPageOID);
	m_szOkiA4PrintHBPageOID = DecryptOID(OkiA4PrintHBPageOID);
}

BOOL CSnmpPrinterMeterMonitorOKI::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szOkiA4PrintTotalPageOID), nValue))	//��ҳ��=".1.3.6.1.4.1.2001.1.1.1.1.100.1.1.1.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,2,��ȡ��ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,3,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szOkiA4PrintColorPageOID), nValue))	//��ɫ��ӡ��=".1.3.6.1.4.1.2001.1.1.1.1.11.1.10.170.1.8.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,4,��ȡ��ɫ��ӡ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,5,��ȡ��ɫ��ӡ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szOkiA4PrintHBPageOID), nValue))	//�ڰ״�ӡ��=".1.3.6.1.4.1.2001.1.1.1.1.11.1.10.170.1.7.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorOKI::GetMeterInfo,6,��ȡ�ڰ״�ӡ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,7,��ȡ�ڰ״�ӡ���ɹ���nValue=%d", nValue);
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

	//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorOKI::GetMeterInfo,100,end");
	return TRUE;
}