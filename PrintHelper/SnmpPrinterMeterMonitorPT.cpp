#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorPT.h"
#include "PantumMIB_Def.h"

CSnmpPrinterMeterMonitorPT::CSnmpPrinterMeterMonitorPT(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorPT::~CSnmpPrinterMeterMonitorPT(void)
{
}

void CSnmpPrinterMeterMonitorPT::InitOID()
{
 	m_szPantumTotalPrintOID = DecryptOID(PantumTotalPageOID);
 	m_szPantumA4PrintHBPageOID = DecryptOID(PantumA4HBTotalPageOID);
	m_szPantumA4PrintTotalPageOID = DecryptOID(PantumA4PrintTotalPageOID);
}

BOOL CSnmpPrinterMeterMonitorPT::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,1,begin");
	// �ȶ������е���Ϣ
    CSnmpPrinterMeterMonitor::GetMeterInfo();

    CCriticalSection2::Owner lock(m_cs4MeterInfo);
	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPantumTotalPrintOID), nValue))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorPT::GetMeterInfo,��ȡ��ҳ��ʧ��");

        return StaticInfo();
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szPantumA4PrintTotalPageOID), nValue))	//��ҳ��=".1.3.6.1.4.1.2001.1.1.1.1.100.1.1.1.3.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorPT::GetMeterInfo,2,��ȡA4��ҳ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,3,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	if (!GetRequest(CStringToChar(m_szPantumA4PrintHBPageOID), nValue))	//�ڰ״�ӡ��=".1.3.6.1.4.1.2001.1.1.1.1.11.1.10.170.1.7.3"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorPT::GetMeterInfo,6,��ȡ�ڰ״�ӡ��ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorPT::GetMeterInfo,7,��ȡ�ڰ״�ӡ���ɹ���nValue=%d", nValue);
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
	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorPT::StaticInfo()
{
    std::string oHexInfo;
    if (!GetRequest(PantumStaticRequstOID, oHexInfo))
    {
        theLog.Write("CSnmpMonitorHelperPT::StaticInfo, ��̬��ѯʧ��, ��Ϊ��ѯ�����ڵ�");
        return FALSE;        
    }

    //���ó�ʼ�������������ֹ���ݳ����ۼ������
    ResetPaperMeter();

    const int nLen = oHexInfo.size();
    if (nLen < 172)
    {
        theLog.Write(_T("!!!!CSnmpMonitorHelperPT::DynamicInfo ���ȴ���: %d"), nLen);
        return FALSE;
    }

    unsigned char* pszInfo = (unsigned char*)oHexInfo.c_str(); 
    int nValue = 0;

    // �����ӡA4��LETTERֽ����ҳ��
    nValue = 0;
    nValue += pszInfo[116] << 8 * 0;
    nValue += pszInfo[117] << 8 * 1;
    nValue += pszInfo[118] << 8 * 2;
    nValue += pszInfo[119] << 8 * 3;
    m_oMeterInfo.nA4PrintHBPage = nValue;
    m_oMeterInfo.nA4PrintTotalPage = nValue;
    m_oMeterInfo.nAllTotalPage += nValue;

    // �����ӡLegal��filioֽ����ҳ��
    nValue = 0;
    nValue += pszInfo[120] << 8 * 0;
    nValue += pszInfo[121] << 8 * 1;
    nValue += pszInfo[122] << 8 * 2;
    nValue += pszInfo[123] << 8 * 3;
    m_oMeterInfo.nAllTotalPage += nValue;

    // �����ӡB5��EXECUTEֽ����ҳ��
    nValue = 0;
    nValue += pszInfo[124] << 8 * 0;
    nValue += pszInfo[125] << 8 * 1;
    nValue += pszInfo[126] << 8 * 2;
    nValue += pszInfo[127] << 8 * 3;
    m_oMeterInfo.nAllTotalPage += nValue;

    // �����ӡB6��B6 ENVֽ����ҳ��
    nValue = 0;
    nValue += pszInfo[128] << 8 * 0;
    nValue += pszInfo[129] << 8 * 1;
    nValue += pszInfo[130] << 8 * 2;
    nValue += pszInfo[131] << 8 * 3;
    m_oMeterInfo.nAllTotalPage += nValue;

    // �����ӡ����ֽ�͵���ҳ��
    nValue = 0;
    nValue += pszInfo[132] << 8 * 0;
    nValue += pszInfo[133] << 8 * 1;
    nValue += pszInfo[134] << 8 * 2;
    nValue += pszInfo[135] << 8 * 3;
    m_oMeterInfo.nAllTotalPage += nValue;

    // �����ӡA5ֽ����ҳ��
    nValue = 0;
    nValue += pszInfo[136] << 8 * 0;
    nValue += pszInfo[137] << 8 * 1;
    nValue += pszInfo[138] << 8 * 2;
    nValue += pszInfo[139] << 8 * 3;
    m_oMeterInfo.nAllTotalPage += nValue;

    // ��ӡ����ֽ����ҳ��
    nValue = 0;
    nValue += pszInfo[140] << 8 * 0;
    nValue += pszInfo[141] << 8 * 1;
    nValue += pszInfo[142] << 8 * 2;
    nValue += pszInfo[143] << 8 * 3;
    m_oMeterInfo.nAllTotalPage += nValue;

    // ��������
    nValue = 0;
    nValue += pszInfo[144] << 8 * 0;
    nValue += pszInfo[145] << 8 * 1;
    nValue += pszInfo[146] << 8 * 2;
    nValue += pszInfo[147] << 8 * 3;
    m_oMeterInfo.nOPCBlackMaxCapacity = nValue;

    // ���Ĵ�ӡҳ��
    nValue = 0;
    nValue += pszInfo[148] << 8 * 0;
    nValue += pszInfo[149] << 8 * 1;
    nValue += pszInfo[150] << 8 * 2;
    nValue += pszInfo[151] << 8 * 3;
    m_oMeterInfo.nOPCBlackLevel = nValue;

    return TRUE;
}