/***********20160627 �޸�****************
��(.1.3.6.1.2.1.43.10.2.1.4.1.1)����ڵ�ȡ��ҳ������
����ҳ���ɸ����������
*************************************************/

#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorJC.h"
#include "KyoceraMIB_Def.h"

CSnmpPrinterMeterMonitorJC::CSnmpPrinterMeterMonitorJC(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorJC::~CSnmpPrinterMeterMonitorJC(void)
{
}

void CSnmpPrinterMeterMonitorJC::InitOID()
{
	m_szKyoceraA4PrintTotalPageOID = DecryptOID(KyoceraA4PrintTotalPageOID);
	m_szKyoceraA4PrintHBPageOID = DecryptOID(KyoceraA4PrintHBPageOID);
	m_szKyoceraA4PrintSingleColorPageOID = DecryptOID(KyoceraA4PrintSingleColorPageOID);
	m_szKyoceraA4PrintFullColorPageOID = DecryptOID(KyoceraA4PrintFullColorPageOID);
	m_szKyoceraA4CopyTotalPageOID = DecryptOID(KyoceraA4CopyTotalPageOID);
	m_szKyoceraA4CopyHBPageOID = DecryptOID(KyoceraA4CopyHBPageOID);
	m_szKyoceraA4CopySingleColorPageOID = DecryptOID(KyoceraA4CopySingleColorPageOID);
	m_szKyoceraA4CopyFullColorPageOID = DecryptOID(KyoceraA4CopyFullColorPageOID);
	m_szKyoceraScanTotalPageOID = DecryptOID(KyoceraScanTotalPageOID);
	m_szKyoceraScanTotalPage2OID = DecryptOID(KyoceraScanTotalPage2OID);
	m_szKyoceraTotalPageInfo6OID = DecryptOID(KyoceraTotalPageInfo6OID);
	m_szKyoceraTotalPageInfo7OID = DecryptOID(KyoceraTotalPageInfo7OID);
	m_szKyoceraTotalPageInfo8OID = DecryptOID(KyoceraTotalPageInfo8OID);
	m_szKyoceraTotalPageInfo9OID = DecryptOID(KyoceraTotalPageInfo9OID);

	m_szKyoceraModelOID = DecryptOID(KyoceraModelOID);						
	m_szECOSYS_P5021cdn_A4PrintHBOID = DecryptOID(ECOSYS_P5021cdn_A4PrintHBOID);				
	m_szECOSYS_P5021cdn_A4PrintColorOID = DecryptOID(ECOSYS_P5021cdn_A4PrintColorOID);				
	m_szECOSYS_P5021cdn_A4PrintTotalOID = DecryptOID(ECOSYS_P5021cdn_A4PrintTotalOID);
	m_szFS_1060DN_A4PrintHBOID = DecryptOID(FS_1060DN_A4PrintHBOID);
	m_szTASKalfa_6002iA4HBOID = DecryptOID(TASKalfa6002iA4PrintHBOID);
	m_szTASKalfa_6002iTotalOID = DecryptOID(TASKalfa6002iA4TotalOID);
}

BOOL CSnmpPrinterMeterMonitorJC::GetMeterInfo_ECOSYS_P5021cdn()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szECOSYS_P5021cdn_A4PrintTotalOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P5021cdn::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szECOSYS_P5021cdn_A4PrintHBOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P5021cdn::GetMeterInfo,A4��ӡ�ڰ�");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,A4��ӡ�ڰף�nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szECOSYS_P5021cdn_A4PrintColorOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P5021cdn::GetMeterInfo,A4��ӡ��ɫ");
	}
	else
	{
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,,A4��ӡ��ɫ��nValue=%d", nValue);
	}

	m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nAllTotalPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage;
	m_oMeterInfo.nA4ColorTotalPage =m_oMeterInfo.nA4PrintColorPage;

	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorJC::GetMeterInfo_ECOSYS_P2135dn()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();
	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szECOSYS_P5021cdn_A4PrintTotalOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P2135dn::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}
	m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nAllTotalPage;
	m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nAllTotalPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nAllTotalPage;
	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorJC::GetMeterInfo_FS_1060DN()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();
	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szFS_1060DN_A4PrintHBOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P2135dn::GetMeterInfo_FS_1060DN,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}
	m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nAllTotalPage;
	m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nAllTotalPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nAllTotalPage;
	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorJC::GetMeterInfo_TASKalfa6002i()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();
	int nValue = 0;
	if (!GetRequest(CStringToChar(m_szTASKalfa_6002iTotalOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P2135dn::GetMeterInfo_TASKalfa6002i,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}
	if (!GetRequest(CStringToChar(m_szTASKalfa_6002iA4HBOID), nValue))
	{
		theLog.Write("!!GetMeterInfo_ECOSYS_P2135dn::GetMeterInfo_TASKalfa6002i,��ȡa4�ڰ�ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nA4PrintHBPage;
		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}
	return TRUE;
}

//ֽ�Ų�����ȷָʾΪֽ��ʱ��Ĭ�ϼ���ΪA4
//��ӡ����ӡ�й��ڼ�����A3=2*A4
BOOL CSnmpPrinterMeterMonitorJC::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,1,begin");
	CString szModel = GetModel();
	if (szModel.Find("P5021cdn") >= 0)
	{
		return GetMeterInfo_ECOSYS_P5021cdn();
	}
	else if (szModel.Find("P2135dn") >= 0 || szModel.Find("P2235dn") >= 0)
	{
		return GetMeterInfo_ECOSYS_P2135dn();
	}
	else if (szModel.Find("FS-1060DN") >= 0)
	{
		return GetMeterInfo_FS_1060DN();
	}
	else if (szModel.Find("TASKalfa 6002i") >= 0)
	{
		return GetMeterInfo_TASKalfa6002i();
	}
	else
	{
		CCriticalSection2::Owner lock(m_cs4MeterInfo);

		//���ó�ʼ�������������ֹ���ݳ����ۼ������
		ResetPaperMeter();

		int nValue = 0;

		if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ��ʧ��");
			return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
		}
		//OID=.1.3.6.1.4.1.1347.42.3.1.1.1.1.1, Type=Integer, Value=31277
		//OID=.1.3.6.1.4.1.1347.42.3.1.1.1.1.2, Type=Integer, Value=17687

		if (!GetRequest(CStringToChar(m_szKyoceraA4PrintTotalPageOID), nValue))	//��ӡ�������ڰ�+��ɫ��=".1.3.6.1.4.1.1347.42.3.1.1.1.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,2,��ȡ��ӡ�������ڰ�+��ɫ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
			/*if ((m_oMeterInfo.nA4PrintHBPage <= 0) && (nValue > 0))
			{
			m_oMeterInfo.nA4PrintHBPage = nValue;
			theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,3,��ȡ��ӡ�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
			}*/
		}


		if (!GetRequest(CStringToChar(m_szKyoceraA4PrintHBPageOID), nValue))	//��ӡ�ڰ�=".1.3.6.1.4.1.1347.42.3.1.2.1.1.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,2,��ȡ��ӡ�ڰ׵���ҳ��ʧ��");
			m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nA4PrintTotalPage > 0 ? m_oMeterInfo.nA4PrintTotalPage : 0;
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,3,��ȡ��ӡ�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraA4PrintSingleColorPageOID), nValue))	//��ӡ��ɫ(����)=".1.3.6.1.4.1.1347.42.3.1.2.1.1.1.2"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,4,��ȡ��ӡ��ɫ(����)����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage += (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,5,��ȡ��ӡ��ɫ(����)����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraA4PrintFullColorPageOID), nValue))	//��ӡ��ɫ(ȫ��)=".1.3.6.1.4.1.1347.42.3.1.2.1.1.1.3"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,6,��ȡ��ӡ��ɫ(ȫ��)����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage += (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,7,��ȡ��ӡ��ɫ(ȫ��)����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraA4CopyTotalPageOID), nValue))	//��ӡ����(�ڰ�+��ɫ)=".1.3.6.1.4.1.1347.42.3.1.1.1.1.2"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,8,��ȡ��ӡ����(�ڰ�+��ɫ)����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
			/*if ((m_oMeterInfo.nA4CopyHBPage <= 0) && (nValue > 0))
			{

			m_oMeterInfo.nA4CopyHBPage = nValue;
			theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,9,��ȡ��ӡ����(�ڰ�+��ɫ)����ҳ���ɹ���nValue=%d", nValue);
			}*/
		}

		if (!GetRequest(CStringToChar(m_szKyoceraA4CopyHBPageOID), nValue))	//��ӡ�ڰ�=".1.3.6.1.4.1.1347.42.3.1.2.1.1.2.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,9,��ȡ��ӡ�ڰ׵���ҳ��ʧ��");
			m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage > 0 ? m_oMeterInfo.nA4CopyTotalPage : 0;
		}
		else
		{
			m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,10,��ȡ��ӡ�ڰ׵���ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraA4CopySingleColorPageOID), nValue))	//��ӡ����=".1.3.6.1.4.1.1347.42.3.1.2.1.1.2.2"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,11,��ȡ��ӡ���ʵ���ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,12,��ȡ��ӡ���ʵ���ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraA4CopyFullColorPageOID), nValue))	//��ӡȫ��=".1.3.6.1.4.1.1347.42.3.1.2.1.1.2.3"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,13,��ȡ��ӡȫ�ʵ���ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage += (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,14,��ȡ��ӡȫ�ʵ���ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraScanTotalPageOID), nValue))	//ɨ�裨������=".1.3.6.1.4.1.1347.42.3.1.4.1.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,15,��ȡɨ�裨����������ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,16,��ȡɨ�裨����������ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szKyoceraScanTotalPage2OID), nValue))	//ɨ�裨������=".1.3.6.1.4.1.1347.46.10.1.1.5.3"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetMeterInfo,17,��ȡɨ�裨����������ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = nValue > 0 ? nValue : m_oMeterInfo.nScanTotalPage;
			//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,18,��ȡɨ�裨����������ҳ���ɹ���nValue=%d", nValue);
		}

#if 0	//del by zxl,20160831,��ʱɾ������Ļ�ȡ����,��Ϊ̫���ˣ����濴����û�и��õķ�������ȡ����
		//--------------------------------------------------------------------------
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.1", nValue))	//A3�ڰ���
		{
			m_oMeterInfo.nA3HBTotalPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,18,��ȡA3�ڰ������ɹ���nValue=%d", nValue);
		}

		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.2", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.3", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.4", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.5", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.6", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.7", nValue))//A3�ڰ���
		{
			m_oMeterInfo.nA3HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.8", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.9", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.10", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.11", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.6.1.12", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}

		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.2", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.3", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.4", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.5", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.6", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.7", nValue))//A3�ڰ���
		{
			m_oMeterInfo.nA3HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.8", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.9", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.10", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.11", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.7.1.12", nValue))
		{
			m_oMeterInfo.nA4HBTotalPage += (nValue>0) ? nValue : 0;
		}

		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.1", nValue))	//A3ȫ����
		{
			m_oMeterInfo.nA3ColorTotalPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,18,��ȡA3ȫ�����ɹ���nValue=%d", nValue);
		}

		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.2", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.3", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.4", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.5", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.6", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.7", nValue))//A3ȫ����
		{
			m_oMeterInfo.nA3ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.8", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.9", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.10", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.11", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.8.1.12", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}

		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.1", nValue))	//A3������
		{
			m_oMeterInfo.nA3ColorTotalPage += (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,18,��ȡA3�ڰ������ɹ���nValue=%d", nValue);
		}

		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.2", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.3", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.4", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.5", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.6", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.7", nValue))//A3������
		{
			m_oMeterInfo.nA3ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.8", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.9", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.10", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.11", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
		if (GetRequest(".1.3.6.1.4.1.1347.42.2.1.1.1.9.1.12", nValue))
		{
			m_oMeterInfo.nA4ColorTotalPage += (nValue>0) ? nValue : 0;
		}
#endif

		m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3HBTotalPage > 0 ? m_oMeterInfo.nA3HBTotalPage : m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
		m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3ColorTotalPage > 0 ? m_oMeterInfo.nA3ColorTotalPage : m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
		m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4HBTotalPage > 0 ? m_oMeterInfo.nA4HBTotalPage : m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
		m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4ColorTotalPage > 0 ? m_oMeterInfo.nA4ColorTotalPage : m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
		int temp = m_oMeterInfo.nA3HBTotalPage + m_oMeterInfo.nA3ColorTotalPage + m_oMeterInfo.nA4HBTotalPage + m_oMeterInfo.nA4ColorTotalPage + m_oMeterInfo.nOtherOpHBPage + m_oMeterInfo.nOtherOpColorPage;
		if(temp > 0)
		{
			m_oMeterInfo.nAllTotalPage = temp;
		}	
		/*
		int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
		if(other > 0)
		{
		m_oMeterInfo.nOtherOpHBPage += other;
		}*/

		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorJC::GetMeterInfo,100,end");
		return TRUE;
	}	
}
CString CSnmpPrinterMeterMonitorJC::GetModel()
{
	CString szModel;
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szKyoceraModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetHPModel,��ȡ�ͺ�ʧ��");
	}
	szModel.Format("%s",tmp);
	theLog.Write("CSnmpPrinterMeterMonitorJC::GetModel,szModel=%s",szModel);
	szModel.Trim();
	return szModel;
}
