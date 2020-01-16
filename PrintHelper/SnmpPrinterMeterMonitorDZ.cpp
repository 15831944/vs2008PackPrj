#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorDZ.h"
#include "ToshibaMIB_Def.h"

CSnmpPrinterMeterMonitorDZ::CSnmpPrinterMeterMonitorDZ(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorDZ::~CSnmpPrinterMeterMonitorDZ(void)
{
}

void CSnmpPrinterMeterMonitorDZ::InitOID()
{
	m_szToshibaA4PrintFullColorPageOID = DecryptOID(ToshibaA4PrintFullColorPageOID);
	m_szToshibaA4PrintSingleColorPageOID = DecryptOID(ToshibaA4PrintSingleColorPageOID);
	m_szToshibaA4PrintHBPageOID = DecryptOID(ToshibaA4PrintHBPageOID);
	m_szToshibaA3PrintFullColorPageOID = DecryptOID(ToshibaA3PrintFullColorPageOID);
	m_szToshibaA3PrintSingleColorPageOID = DecryptOID(ToshibaA3PrintSingleColorPageOID);
	m_szToshibaA3PrintHBPageOID = DecryptOID(ToshibaA3PrintHBPageOID);
	m_szToshibaA4CopyFullColorPageOID = DecryptOID(ToshibaA4CopyFullColorPageOID);
	m_szToshibaA4CopySingleColorPageOID = DecryptOID(ToshibaA4CopySingleColorPageOID);
	m_szToshibaA4CopyHBPageOID = DecryptOID(ToshibaA4CopyHBPageOID);
	m_szToshibaA3CopyFullColorPageOID = DecryptOID(ToshibaA3CopyFullColorPageOID);
	m_szToshibaA3CopySingleColorPageOID = DecryptOID(ToshibaA3CopySingleColorPageOID);
	m_szToshibaA3CopyHBPageOID = DecryptOID(ToshibaA3CopyHBPageOID);
	m_szToshibaScanSmallFullColorPageOID = DecryptOID(ToshibaScanSmallFullColorPageOID);
	m_szToshibaScanSmallSingleColorPageOID = DecryptOID(ToshibaScanSmallSingleColorPageOID);
	m_szToshibaScanSmallHBPageOID = DecryptOID(ToshibaScanSmallHBPageOID);
	m_szToshibaScanBigFullColorPageOID = DecryptOID(ToshibaScanBigFullColorPageOID);
	m_szToshibaScanBigSingleColorPageOID = DecryptOID(ToshibaScanBigSingleColorPageOID);
	m_szToshibaScanBigHBPageOID = DecryptOID(ToshibaScanBigHBPageOID);
	m_szToshibaOtherOpFullColorA4PageOID = DecryptOID(ToshibaOtherOpFullColorA4PageOID);
	m_szToshibaOtherOpSinglelColorA4PageOID = DecryptOID(ToshibaOtherOpSingleColorA4PageOID);
	m_szToshibaOtherOpHBA4PageOID = DecryptOID(ToshibaOtherOpHBA4PageOID);
	m_szToshibaOtherOpFullColorA3PageOID = DecryptOID(ToshibaOtherOpFullColorA3PageOID);
	m_szToshibaOtherOpSingleColorA3PageOID = DecryptOID(ToshibaOtherOpSingleColorA3PageOID);
	m_szToshibaOtherOpHBA3PageOID = DecryptOID(ToshibaOtherOpHBA3PageOID);
}

BOOL CSnmpPrinterMeterMonitorDZ::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	//A4��ӡ
	if (!GetRequest(CStringToChar(m_szToshibaA4PrintFullColorPageOID), nValue))	//��ӡ-��ɫ-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.210.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,2,��ȡ��ӡ-��ɫ-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = nValue > 0 ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,3,��ȡ��ӡ-��ɫ-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4PrintSingleColorPageOID), nValue))	//��ӡ-˫ɫ/��ɫ-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.210.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,4,��ȡ��ӡ-˫ɫ/��ɫ-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,5,��ȡ��ӡ-˫ɫ/��ɫ-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4PrintHBPageOID), nValue))	//��ӡ-�ڰ�-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.210.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,6,��ȡ��ӡ-�ڰ�-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,7,��ȡ��ӡ-�ڰ�-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//A3��ӡ
	if (!GetRequest(CStringToChar(m_szToshibaA3PrintFullColorPageOID), nValue))	//��ӡ-��ɫ-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.209.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,8,��ȡ��ӡ-��ɫ-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,9,��ȡ��ӡ-��ɫ-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3PrintSingleColorPageOID), nValue))	//��ӡ-˫ɫ/��ɫ-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.209.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,10,��ȡ��ӡ-˫ɫ/��ɫ-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,11,��ȡ��ӡ-˫ɫ/��ɫ-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3PrintHBPageOID), nValue))	//��ӡ-�ڰ�-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.209.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,12,��ȡ��ӡ-�ڰ�-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,13,��ȡ��ӡ-�ڰ�-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//A4��ӡ
	if (!GetRequest(CStringToChar(m_szToshibaA4CopyFullColorPageOID), nValue))	//��ӡ-��ɫ-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.212.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,14,��ȡ��ӡ-��ɫ-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,15,��ȡ��ӡ-��ɫ-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4CopySingleColorPageOID), nValue))	//��ӡ-˫ɫ/��ɫ-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.212.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,16,��ȡ��ӡ-˫ɫ/��ɫ-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,17,��ȡ��ӡ-˫ɫ/��ɫ-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA4CopyHBPageOID), nValue))	//��ӡ-�ڰ�-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.212.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,18,��ȡ��ӡ-�ڰ�-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,19,��ȡ��ӡ-�ڰ�-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//A3��ӡ
	if (!GetRequest(CStringToChar(m_szToshibaA3CopyFullColorPageOID), nValue))	//��ӡ-��ɫ-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.211.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,20,��ȡ��ӡ-��ɫ-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,21,��ȡ��ӡ-��ɫ-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3CopySingleColorPageOID), nValue))	//��ӡ-˫ɫ/��ɫ-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.211.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,22,��ȡ��ӡ-˫ɫ/��ɫ-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,23,��ȡ��ӡ-˫ɫ/��ɫ-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaA3CopyHBPageOID), nValue))	//��ӡ-�ڰ�-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.211.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,24,��ȡ��ӡ-�ڰ�-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,25,��ȡ��ӡ-�ڰ�-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//ɨ��Сֽ
	if (!GetRequest(CStringToChar(m_szToshibaScanSmallFullColorPageOID), nValue))	//ɨ��-����-��ɫ-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.220.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,26,��ȡɨ��-����-��ɫ-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanSmallColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,27,��ȡɨ��-����-��ɫ-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanSmallSingleColorPageOID), nValue))	//ɨ��-����-˫ɫ/��ɫ-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.220.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,28,��ȡɨ��-����-˫ɫ/��ɫ-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanSmallColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,29,��ȡɨ��-����-˫ɫ/��ɫ-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanSmallHBPageOID), nValue))	//ɨ��-����-�ڰ�-Сֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.220.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,30,��ȡɨ��-����-�ڰ�-Сֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanSmallHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,31,��ȡɨ��-����-�ڰ�-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//ɨ���ֽ
	if (!GetRequest(CStringToChar(m_szToshibaScanBigFullColorPageOID), nValue))	//ɨ��-����-��ɫ-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.219.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,32,��ȡɨ��-����-��ɫ-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanBigColorPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,33,��ȡɨ��-����-��ɫ-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanBigSingleColorPageOID), nValue))	//ɨ��-����-˫ɫ/��ɫ-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.219.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,34,��ȡɨ��-����-˫ɫ/��ɫ-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanBigColorPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,35,��ȡɨ��-����-˫ɫ/��ɫ-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaScanBigHBPageOID), nValue))	//ɨ��-����-�ڰ�-��ֽ=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.219.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,36,��ȡɨ��-����-�ڰ�-��ֽ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanBigHBPage = (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,37,��ȡɨ��-����-�ڰ�-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//---------------------------------------------

	//A3��ɫ
	//A3�ڰ�
	//A4��ɫ
	//A4�ڰ�
	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;

	//����������ֽ����������һ��ָ��ӡ��web�еļ������ġ��б��ࡣ
	if (!GetRequest(CStringToChar(m_szToshibaOtherOpFullColorA3PageOID), nValue))	//����������ֽ ȫ��=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.225.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,38,��ȡ������������ֽȫ�ʵ���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA3ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,39,��ȡ����������ֽȫ�ʵ���ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpSingleColorA3PageOID), nValue))	//����������ֽ ����=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.225.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,40,��ȡ����������ֽ���ʵ���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA3ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,41,��ȡ����������ֽ���ʵ���ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpHBA3PageOID), nValue))	//����������ֽ �ڰ�=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.225.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,42,��ȡ����������ֽ�ڰ׵���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpHBPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA3HBTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,43,��ȡ��ӡ-����-��ֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//��������Сֽ����������һ��ָ��ӡ��web�еļ������ġ��б��ࡣ
	if (!GetRequest(CStringToChar(m_szToshibaOtherOpFullColorA4PageOID), nValue))	//��������Сֽ ȫ��=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.226.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,44,��ȡ����������Сֽȫ�ʵ���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA4ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,45,��ȡ��������Сֽȫ�ʵ���ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpSinglelColorA4PageOID), nValue))	//��������Сֽ ����=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.226.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,46,��ȡ��������Сֽ���ʵ���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA4ColorTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,47,��ȡ����������ֽ���ʵ���ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szToshibaOtherOpHBA4PageOID), nValue))	//��������Сֽ �ڰ�=".1.3.6.1.4.1.1129.2.3.50.1.3.21.6.1.226.1.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorDZ::GetMeterInfo,48,��ȡ��������Сֽ�ڰ׵���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpHBPage += (nValue > 0 ? nValue : 0);
		m_oMeterInfo.nA4HBTotalPage += (nValue > 0 ? nValue : 0);
		//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,49,��ȡ��ӡ-����-Сֽ����ҳ���ɹ���nValue=%d", nValue);
	}

	//������ɫ
	//�����ڰ�
	if(m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage - m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage > 0)
		m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage - m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage;

	//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorDZ::GetMeterInfo,100,end");
	return TRUE;
}
