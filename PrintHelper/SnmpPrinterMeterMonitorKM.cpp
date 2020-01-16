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

//����ӡ�����������ֽ�����Ƿ���˫��,�磺1��A3,����Ϊ2��ʵ�ʵ�����Ҫ����������2��
BOOL CSnmpPrinterMeterMonitorKM::IsBigPaperDoubleCounter()
{
	CString szSysDescr = GetStrByOid(m_szSysDescrOID);
	if (szSysDescr.CompareNoCase("KONICA MINOLTA 306") == 0)
	{
		//KONICA MINOLTA 306������������ԣ�
		//1.��ӡ1�Ŵ�ֽ��ӡ����Ϊ2�Ŵ�ֽ;
		//2.��ӡ1��Сֽ��ӡ����Ϊ1��Сֽ;
		//3.��ӡ1�Ŵ�ֽ��ӡ����Ϊ2�Ŵ�ֽ������Сֽ��ӡ����ҲΪ1��;
		//4.��ӡ1��Сֽ����Ϊ1��Сֽ��
		theLog.Write("CSnmpPrinterMeterMonitorKM::CheckCounterPolicy,szSysDescr=%s", szSysDescr);
		return TRUE;
	}
	return FALSE;
}

//ֽ�Ų�����ȷָʾΪֽ��ʱ��Ĭ�ϼ���ΪA4
//��ӡ����ӡ�й��ڼ�����A3=2*A4
//�����޸ĳ��������A3��A4�ֿ���������A3=2*A4�ļ�����ʽ��
BOOL CSnmpPrinterMeterMonitorKM::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	BOOL bBigPaperDoubleCounter = IsBigPaperDoubleCounter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4CopyHBPageOID), nValue))	//��ӡ��ɫҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,2,��ȡ��ӡ��ɫҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,3,��ȡ��ӡ��ɫҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4CopyFullColorPageOID), nValue))	//��ӡȫ��ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.2.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,4,��ȡ��ӡȫ��ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,5,��ȡ��ӡȫ��ҳ���ɹ���nValue=%d", nValue);
	}

	//�����������[2��ɫ]�ڵ�[.4.1]ʱ���˽ڵ���[2��ɫ]��
	if (!GetRequest(CStringToChar(m_szKmA4CopySingleColorPageOID), nValue))	//��ӡ����ɫҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,6,��ȡ��ӡ����ɫҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,7,��ȡ��ӡ����ɫҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4CopyDoubleColorPage), nValue))	//��ӡ2��ɫҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.4.1"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,8,��ȡ��ӡ2��ɫҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,9,��ȡ��ӡ2��ɫҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3CopyHBPageOID), nValue))	//��ӡ��ɫ�����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,2,��ȡ��ӡ��ɫ�����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,3,��ȡ��ӡ��ɫ�����ߴ磩ҳ���ɹ���nValue=%d", nValue);
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

	if (!GetRequest(CStringToChar(m_szKmA3CopyFullColorPageOID), nValue))	//��ӡȫ�ʣ����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.2.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,4,��ȡ��ӡȫ�ʣ����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,5,��ȡ��ӡȫ�ʣ����ߴ磩ҳ���ɹ���nValue=%d", nValue);
	}

	//�����������[2��ɫ]�ڵ�[.4.1]ʱ���˽ڵ���[2��ɫ]��
	if (!GetRequest(CStringToChar(m_szKmA3CopySingleColorPageOID), nValue))	//��ӡ����ɫ�����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,6,��ȡ��ӡ����ɫ�����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,7,��ȡ��ӡ����ɫ�����ߴ磩ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3CopyDoubleColorPageOID), nValue))	//��ӡ2��ɫ�����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.4.1"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,8,��ȡ��ӡ2��ɫ�����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3CopyColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,9,��ȡ��ӡ2��ɫ�����ߴ磩ҳ���ɹ���nValue=%d", nValue);
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

	if (!GetRequest(CStringToChar(m_szKmA4PrintHBPageOID), nValue))	//��ӡ��ɫҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,10,��ȡ��ӡ��ɫҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,11,��ȡ��ӡ��ɫҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4PrintFullColorPageOID), nValue))	//��ӡȫ��ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.2.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,12,��ȡ��ӡȫ��ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,13,��ȡ��ӡȫ��ҳ���ɹ���nValue=%d", nValue);
	}

	//��ӡ����ҳ�������������[2��ɫ]�ڵ�[.4.1]ʱ���˽ڵ���[2��ɫ]���������[2��ɫ]�ڵ�[.4.1]ʱ���˽ڵ���Ч=[-1]��
	if (!GetRequest(CStringToChar(m_szKmA4PrintSingleColorPageOID), nValue))	//��ӡ����ɫҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.3.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,14,��ȡ��ӡ����ɫҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,15,��ȡ��ӡ����ɫҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA4PrintDoubleColorPageOID), nValue))	//��ӡ2��ɫҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.4.2"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,16,��ȡ��ӡ2��ɫҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,17,��ȡ��ӡ2��ɫҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3PrintHBPageOID), nValue))	//��ӡ��ɫ�����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.1.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,10,��ȡ��ӡ��ɫ�����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,11,��ȡ��ӡ��ɫ�����ߴ磩ҳ���ɹ���nValue=%d", nValue);
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

	if (!GetRequest(CStringToChar(m_szKmA3PrintFullColorPageOID), nValue))	//��ӡȫ�ʣ����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.2.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,12,��ȡ��ӡȫ�ʣ����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,13,��ȡ��ӡȫ�ʣ����ߴ磩ҳ���ɹ���nValue=%d", nValue);
	}

	//��ӡ���ʣ����ߴ磩ҳ�������������[2��ɫ�����ߴ磩]�ڵ�[.4.2]ʱ���˽ڵ���[2��ɫ]���������[2��ɫ]�ڵ�[.4.1]ʱ���˽ڵ���Ч=[-1]��
	if (!GetRequest(CStringToChar(m_szKmA3PrintSingleColorPageOID), nValue))	//��ӡ����ɫ�����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.3.2"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,14,��ȡ��ӡ����ɫ�����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,15,��ȡ��ӡ����ɫ�����ߴ磩ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmA3PrintDoubleColorPageOID), nValue))	//��ӡ2��ɫ�����ߴ磩ҳ��=".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.4.2"
	{
		//theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,16,��ȡ��ӡ2��ɫ�����ߴ磩ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA3PrintColorPage += (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,17,��ȡ��ӡ2��ɫ�����ߴ磩ҳ���ɹ���nValue=%d", nValue);
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

	if (!GetRequest(CStringToChar(m_szKmScanTotalPageOID), nValue))	//ɨ���ܼ�=".1.3.6.1.4.1.18334.1.1.1.5.7.2.3.1.5.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,15,��ȡɨ���ܼ�ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,18,��ȡɨ���ܼƳɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szKmScanBigColorPageOID), nValue))	//ɨ�����ߴ��ܼ�=".1.3.6.1.4.1.18334.1.1.1.5.7.2.3.1.6.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorKM::GetMeterInfo,19,��ȡɨ�����ߴ��ܼ�ʧ��");
	}
	else
	{
		m_oMeterInfo.nScanBigColorPage = (nValue>0) ? nValue : 0;
		if (m_oMeterInfo.nScanTotalPage >= m_oMeterInfo.nScanBigColorPage)
		{
			m_oMeterInfo.nScanSmallColorPage = m_oMeterInfo.nScanTotalPage - m_oMeterInfo.nScanBigColorPage;
		}
		//theLog.Write("CSnmpPrinterMeterMonitorKM::GetMeterInfo,20,��ȡɨ�����ߴ��ܼƳɹ���nValue=%d", nValue);
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
