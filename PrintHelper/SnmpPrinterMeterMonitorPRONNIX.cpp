#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorPRONNIX.h"
#include "PrintronixMIB_Def.h"

CSnmpPrinterMeterMonitorPRONNIX::CSnmpPrinterMeterMonitorPRONNIX(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorPRONNIX::~CSnmpPrinterMeterMonitorPRONNIX(void)
{
}
void CSnmpPrinterMeterMonitorPRONNIX::InitOID()
{
	m_szPRINTRONIX_TotalPageOID = DecryptOID(PRINTRONIX_TotalPageOID);
}
BOOL CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo()
{
	theLog.Write("CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;
	//theLog.Write("CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo,1,m_szPRINTRONIX_TotalPageOID=%s",m_szPRINTRONIX_TotalPageOID);
	if (!GetRequest(CStringToChar(m_szPRINTRONIX_TotalPageOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorPRONNIX::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
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

	//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, oMeterInfoTmp.nA3HBTotalPage, oMeterInfoTmp.nA3ColorTotalPage, oMeterInfoTmp.nA4HBTotalPage, oMeterInfoTmp.nA4ColorTotalPage, oMeterInfoTmp.nOtherOpHBPage, oMeterInfoTmp.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,100,end");
	return TRUE;
}