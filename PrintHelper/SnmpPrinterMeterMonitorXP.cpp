#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorXP.h"
#include "SharpMIB_Def.h"

//��������ֻ�ǲ���������[AR-M351U]����,�Ǻڰ׻���,��ɫ�����ǲ���ȷ��

CSnmpPrinterMeterMonitorXP::CSnmpPrinterMeterMonitorXP(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorXP::~CSnmpPrinterMeterMonitorXP(void)
{
}

void CSnmpPrinterMeterMonitorXP::InitOID()
{
	m_szSharpA4PrintTotalPageOID = DecryptOID(SharpA4PrintTotalPageOID);
	m_szSharpA4PrintColorPageOID = DecryptOID(SharpA4PrintColorPageOID);
	m_szSharpA4PrintHBPageOID = DecryptOID(SharpA4PrintHBPageOID);
	m_szSharpA4CopyHBPageOID = DecryptOID(SharpA4CopyHBPageOID);
	m_szSharpA4CopyHBPage2OID = DecryptOID(SharpA4CopyHBPage2OID);
	m_szSharpA4CopyHBPage3OID = DecryptOID(SharpA4CopyHBPage3OID);
	m_szSharpA4CopyFullColorPageOID = DecryptOID(SharpA4CopyFullColorPageOID);
	m_szSharpA4CopyDoubleColorPageOID = DecryptOID(SharpA4CopyDoubleColorPageOID);
	m_szSharpA4CopySingleColorPageOID = DecryptOID(SharpA4CopySingleColorPageOID);
	m_szSharpOtherOpHBPageOID = DecryptOID(SharpOtherOpHBPageOID);
	m_szSharpOtherOpColorPageOID = DecryptOID(SharpOtherOpColorPageOID);
	m_szSharpScanTotalPageOID = DecryptOID(SharpScanTotalPageOID);
	m_szSharpScanTotalPage2OID = DecryptOID(SharpScanTotalPage2OID);
	m_szSharpA4FaxHBPageOID = DecryptOID(SharpA4FaxHBPageOID);
}

//ֽ�Ų�����ȷָʾΪֽ��ʱ��Ĭ�ϼ���ΪA4
//��ӡ����ӡ�й��ڼ�����A3=2*A4
BOOL CSnmpPrinterMeterMonitorXP::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}


	if (!GetRequest(CStringToChar(m_szSharpA4PrintTotalPageOID), nValue))	//��ӡ-����=".1.3.6.1.4.1.2385.1.1.19.2.1.3.1.4.60"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,��ȡ��ӡ-�ڰ׵���ҳ��ʧ��");
	}
	else
	{
		int nValue2 = 0;
		if(!GetRequest(CStringToChar(m_szSharpA4PrintColorPageOID), nValue2)) //��ӡ��ɫ=".1.3.6.1.4.1.2385.1.1.19.2.1.3.1.4.63"
		{
			//m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = nValue2 > 0 ? nValue2 : m_oMeterInfo.nA4PrintColorPage;
		}
		nValue2 = 0;
		if(!GetRequest(CStringToChar(m_szSharpA4PrintHBPageOID), nValue2))//��ӡ�ڰ�=".1.3.6.1.4.1.2385.1.1.19.2.1.3.1.4.61"
		{
			m_oMeterInfo.nA4PrintHBPage = nValue > 0 ? nValue : m_oMeterInfo.nA4PrintHBPage ;
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = nValue2 > 0 ? nValue2 : m_oMeterInfo.nA4PrintHBPage;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,��ӡ-�ڰ׵���ҳ��=%d,��ӡ��ɫ��ҳ��=%d", m_oMeterInfo.nA4PrintHBPage, m_oMeterInfo.nA4PrintColorPage);
	}

	if (GetRequest(CStringToChar(m_szSharpA4CopyHBPageOID), nValue))	//��ӡ-�ڰ�=".1.3.6.1.4.1.2385.1.1.19.2.1.3.5.4.60"
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.5,��ȡ��ӡ-�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
	}

	if (GetRequest(CStringToChar(m_szSharpA4CopyHBPage2OID), nValue))	//��ӡ-����=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.60"
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.5,��ȡ��ӡ-�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
	}

	if(GetRequest(CStringToChar(m_szSharpA4CopyHBPage3OID), nValue))//��ӡ-�ڰ�=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.61"
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.6,��ȡ��ӡ-�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
	}

	if(!GetRequest(CStringToChar(m_szSharpA4CopyFullColorPageOID), nValue))//��ӡ-��ɫ=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.63"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.2,��ȡ��ӡ-��ɫ����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
	}

	if(!GetRequest(CStringToChar(m_szSharpA4CopyDoubleColorPageOID), nValue))//��ӡ-˫��=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.64"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.3,��ȡ��ӡ-˫�ʵ���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
	}

	if(!GetRequest(CStringToChar(m_szSharpA4CopySingleColorPageOID), nValue))//��ӡ-����=".1.3.6.1.4.1.2385.1.1.19.2.1.3.4.4.80"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,3.4,��ȡ��ӡ-���ʵ���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
	}

	if (!GetRequest(CStringToChar(m_szSharpOtherOpHBPageOID), nValue))	//�ڰ�����=".1.3.6.1.4.1.2385.1.1.19.2.1.3.5.5.61"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,4.1,��ȡ�ڰ�������ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpHBPage = (nValue>0) ? nValue : 0;
	}

	if (!GetRequest(CStringToChar(m_szSharpOtherOpColorPageOID), nValue))	//��ɫ����=".1.3.6.1.4.1.2385.1.1.19.2.1.3.5.5.63"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,4.2,��ȡ��ɫ������ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nOtherOpColorPage = (nValue>0) ? nValue : 0;
	}
	
	if (!GetRequest(CStringToChar(m_szSharpScanTotalPageOID), nValue))	//ɨ��=".1.3.6.1.4.1.2385.1.1.19.2.1.3.7.4.50"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,��ȡɨ�����ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3,��ȡɨ�����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSharpScanTotalPage2OID), nValue))	//ɨ�裨�ڰ�+��ɫ��=".1.3.6.1.4.1.2385.1.1.19.2.1.3.9.4.50"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,��ȡɨ�����ҳ��ʧ��");
	}
	else
	{
		if(m_oMeterInfo.nScanTotalPage <= 0 && nValue > 0)
		{
			m_oMeterInfo.nScanTotalPage = nValue;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3,��ȡɨ�����ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szSharpA4FaxHBPageOID), nValue))	//����-�ڰ�=".1.3.6.1.4.1.2385.1.1.19.2.1.3.8.4.50"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorXP::GetMeterInfo,2,��ȡ����-�ڰ׵���ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4FaxHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,3,��ȡ����-�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
	}

	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage += other;
	}

	//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorXP::GetMeterInfo,100,end");
	return TRUE;
}
