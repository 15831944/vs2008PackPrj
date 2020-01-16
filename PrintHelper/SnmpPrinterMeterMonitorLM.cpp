#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorLM.h"
#include "LexmarkMIB_Def.h"

#if Use_Html_Get_Meter
#include <afxinet.h>
#define DEF_Str_Lexmark_X654de			"Lexmark X654de"
#define DEF_Str_Lexmark_XM1145			"Lexmark XM1145"
#define DEF_Str_Lexmark_X950			"Lexmark X950"
#define DEF_Str_Lexmark_MX811			"Lexmark MX811" 
#define DEF_Str_Lexmark_X860de			"Lexmark X860de"
#define DEF_Str_Lexmark_MX610de			"Lexmark MX610de"
#define DEF_Str_OID_Printer_SerialNo	".1.3.6.1.4.1.641.2.1.2.1.6.1"
#define DEF_Str_OID_Printer_Model		".1.3.6.1.4.1.641.6.2.3.1.4.1"
#define DEF_Str_Url						"/cgi-bin/dynamic/printer/config/reports/devicestatistics.html"
#define DEF_Str_Http					"http://"
#endif


CSnmpPrinterMeterMonitorLM::CSnmpPrinterMeterMonitorLM(void)
{
#if Use_Html_Get_Meter
	m_pBuf = NULL;
#endif
	InitOID();
}

CSnmpPrinterMeterMonitorLM::~CSnmpPrinterMeterMonitorLM(void)
{
#if Use_Html_Get_Meter
	delete[] m_pBuf;
#endif
}

void CSnmpPrinterMeterMonitorLM::InitOID()
{
	m_szGenCountEntryOID = DecryptOID(GenCountEntryOID);
	m_szPaperGeneralCountEntryOID = DecryptOID(PaperGeneralCountEntryOID);
	m_szPaperSidesCountEntryOID = DecryptOID(PaperSidesCountEntryOID);
	m_szPaperSheetsCountEntryOID = DecryptOID(PaperSheetsCountEntryOID);
	m_szPaperNupCountEntryOID = DecryptOID(PaperNupCountEntryOID);
	m_szPaperJobSizeEntryOID = DecryptOID(PaperJobSizeEntryOID);
	m_szScanCountEntryOID = DecryptOID(ScanCountEntryOID);
	m_szCurrentSuppliesEntryOID = DecryptOID(CurrentSuppliesEntryOID);
	m_szSupplyHistoryEntryOID = DecryptOID(SupplyHistoryEntryOID);
	m_szSupplyHistogramEntryOID = DecryptOID(SupplyHistogramEntryOID);
}

BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo,1,begin");

	BOOL bRet = FALSE;

#if Use_Html_Get_Meter
	if(!m_pBuf)
	{
		m_pBuf = new char[1024];
	}
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf,1024))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo,4,fail");
		m_szValue = "";
	}
	m_szValue.Format("%s",m_pBuf);
	theLog.Write("##CSnmpPrinterMeterMonitorLM::GetMeterInfo,4.1,m_szValue=[%s]", m_szValue);
	if(0 == m_szValue.CompareNoCase(DEF_Str_Lexmark_X654de))
	{
		bRet = GetMeterInfo_X654de();
	}
	else if (0 == m_szValue.CompareNoCase(DEF_Str_Lexmark_XM1145))
	{
		bRet = GetMeterInfo_XM1145();
	}
	else if (0 == m_szValue.CompareNoCase(DEF_Str_Lexmark_X950))
	{
		bRet = GetMeterInfo_X950();
	}
	else if (0 == m_szValue.CompareNoCase(DEF_Str_Lexmark_MX811))
	{
		bRet = GetMeterInfo_MX811();
	}
	else if (0 == m_szValue.CompareNoCase(DEF_Str_Lexmark_X860de))
	{
		bRet = GetMeterInfo_X860de();
	}
	else if (0 == m_szValue.CompareNoCase(DEF_Str_Lexmark_MX610de))
	{
		bRet = GetMeterInfo_MX610de();	
	}
	else
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo,5,fail");
	}
#else
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	//��ȡ��ӡ������ͳ����Ϣ
	BOOL bGetPrint = GetPrintSidesCountInfo();

	//��ȡɨ�������ͳ����Ϣ
	BOOL bGetScan = GetScanSidesCountInfo();

	if (bGetPrint || bGetScan)
	{
		bRet = TRUE;
	}
#endif

	//theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo,100,end");

	return bRet;
}

#if Use_Html_Get_Meter	//del by zxl,20160901
/************************************************************************/
/* ��ȡX654��ӡ��������Ϣ                                               */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de()
{
	/*
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.2.0", m_nValue))//��ȡ����X654de�Ѵ�ӡ�������ܼ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,1,��ȡ�Ѵ�ӡ�������ܽ�ʧ��");
	}
	else
	{
		 m_oMeterInfo.nA4PrintHBPage = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,2,X654de�Ѵ�ӡ�������ܼ�=%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf,1024))//��ȡ����X654de��ӡ���ͺ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,3,��ȡ��ӡ���ͺ�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,4,����X654de��ӡ���ͺ�=%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo,m_pBuf,1024))//��ȡ����X654de��ӡ�����к�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,5,��ȡ��ӡ�����к�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,6,��ӡ�����к���:%s",m_szValue);
	}
	//GetOtherMeterInfoByHtml_X654de();//��ȡX654����������Ϣ
	*/
	return TRUE;
}
/************************************************************************/
/* ��ȡXM1145������Ϣ                                                   */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.2.1.6.1.1",m_nValue))//��ȡXM1145�������Ѵ�ӡ���ܼ���
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,1,fail");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,2,XM1145����������½������Ѵ�ӡ���ܼ�:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.1.1.4.1.11",m_nValue))//��ȡXM1145�����渴ӡ�ܼ���
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,1,fail");
	}
	else
	{
		m_oMeterInfo.nA4CopyTotalPage = m_nValue;
		m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nA4PrintTotalPage - m_oMeterInfo.nA4CopyTotalPage;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,2,XM1145����������¸�ӡ�ܼ���:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.3.3.1.2.1.15",m_nValue))//��ȡXM1145ɨ���÷�����
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,3,ɨ���÷����������ʧ��");
	}
	else
	{
		m_nPrintedNet = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,4,XM1145ɨ���÷����������:%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))	//��ȡXM1145��ӡ�����к�
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,5,��ȡ��ӡ�����к�ʧ��,m_pBuf=%s",m_pBuf);
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,6,��ӡ�����кţ�%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf))//��ȡXM1145��ӡ���ͺ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,7,��ȡ��ӡ���ͺ�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,8,��ӡ���ͺ���:%s",m_szValue);
	}
	return GetOtherMeterInfoByHtml_XM1145(pInfo);//��ȡXM1145����������Ϣ
#else
	return GetOtherMeterInfoByHtml_XM1145_2();//��ȡXM1145����������Ϣ
#endif
}

/************************************************************************/
/* ��ȡX950������Ϣ                                                     */
/************************************************************************/
BOOL  CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950()
{
	/*
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.2.0",m_nValue))//��ȡXM950�Ѵ�ӡ��ɫ�����
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,1,fail");
	}
	else
	{
		m_nHBTotal = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,2,XM950�Ѵ�ӡ��ɫ������:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.3.0",m_nValue))//��ȡXM950�Ѵ�ӡ��ɫ�����
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,3,fail");
	}
	else
	{
		m_nPrintedColorTotal = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,4,XM950�Ѵ�ӡ��ɫ�������:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.2.1.6.1.1",m_nValue))//��ȡXM950��ӡ�ĵ�ɫA4-��ֽͨ��
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,3,fail");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = m_nValue;
		theLog.Write("XM950��ӡ�ĵ�ɫA4-��ֽͨ����:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.2.1.7.1.1",m_nValue))//��ȡXM950��ӡ�Ĳ�ɫA4-��ֽͨ���ܼ���
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,4,fail");
	}
	else
	{	
		m_oMeterInfo.nA4PrintColorPage = m_nValue;
		theLog.Write("XM950��ӡ�Ĳ�ɫA4-��ֽͨ���ܼ�����:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.4.1.1.17.1.1",m_nValue))//��ȡXM950��ӡ��ɫ�����ܼ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,5,fail");
	}
	else
	{
		m_nA4PrintedColorTotal = m_nValue;
		theLog.Write("XM950��ӡ��ɫ���ܼ���:%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo,m_pBuf,1024))//��ȡXM950��ӡ�����к�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,6,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("XM950��ӡ�����к���:%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(".1.3.6.1.4.1.641.641.2.1.2.1.2.1",m_pBuf,1024))//��ȡXM950��ӡ���ͺ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,7,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("XM950��ӡ���ͺ���:%s",m_szValue);
	}
	//GetOtherMeterInfoByHtml_X950();//��ȡXM950����������Ϣ
	*/
	return TRUE;
}
/************************************************************************/
/*��ȡ����MX811������Ϣ                                               */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.1.1.4.1.3",m_nValue))//��ȡMX811de������������Ѵ�ӡ�ܼ�
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,1,fail");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,2,MX811de������������Ѵ�ӡ�ܼ�=%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf))//��ȡ����MX811de��ӡ���ͺ�
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,3,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,4,��ӡ���ͺ�:%s",m_szValue);
	}
	//m_szValue.ReleaseBuffer();
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))//��ȡ����MX811de��ӡ�����к�
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,5,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,6,��ӡ�����к�:%s",m_szValue);
	}
	//m_szValue.ReleaseBuffer();
	GetOtherMeterInfoByHtml_MX811();//��ȡMX811����������Ϣ
	return TRUE;
#else
	return GetOtherMeterInfoByHtml_MX811_2();
#endif

}
/************************************************************************/
/*��ȡ����X860de������Ϣ                                                */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.2.0",m_nValue))//��ȡX860de������������Ѵ�ӡ�ܼ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de,1,��ȡ����������Ѵ�ӡ�ܼ�ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("X860de������������Ѵ�ӡ�ܼ�:%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model, m_pBuf))//��ȡ����X860de��ӡ���ͺ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de,2,��ȡ��ӡ���ͺ�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("X860de��ӡ���ͺ�:%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))//��ȡ����X860de��ӡ�����к�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de,3,��ȡ��ӡ�����к�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("��ӡ�����к�:%s",m_szValue);
	}
	GetOtherMeterInfoByHtml_X860de();//��ȡX860de����������Ϣ
	return TRUE;
#else
	return GetOtherMeterInfoByHtml_X860de_2();
#endif
}
/************************************************************************/
/*��ȡ����MX610de������Ϣ                                                */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.1.1.4.1.3",m_nValue))//��ȡMX610dede������������Ѵ�ӡ�ܼ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,1,��ȡ����������Ѵ�ӡ�ܼ�ʧ��");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("MX610de���ʼ������Ѵ�ӡ�ܼ�:%d",m_nValue);

	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model, m_pBuf))//��ȡ����MX610de��ӡ���ͺ�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,3,��ȡ��ӡ���ͺ�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,4,MX610de��ӡ���ͺ���:%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))//��ȡ����MX610de��ӡ�����к�
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,5,��ȡ��ӡ�����к�ʧ��");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,6��MX610de��ӡ�����к���:%s",m_szValue);
	}
	
	GetOtherMeterInfoByHtml_MX610de();//��ȡX610de����������Ϣ
	return TRUE;
#else
	return GetOtherMeterInfoByHtml_MX610de_2();
#endif

}
/************************************************************************/
/*         ͨ����ȡ����X654��̬ҳ��ķ�ʽ��ȡ����������Ϣ               */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de()
{
	
	list <char *> *pListMsg;
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	int nLength = strlen("</TR>");
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	pListMsg = HtmlToList(szUrl);
	list<char *>::iterator it = pListMsg->begin();
	for (; it != pListMsg->end(); it++)
	{
		char *pTableTitle = *it;
		char *pScanUseage = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>ɨ���÷�</b></p></td><td><p>");
		if (pScanUseage)
		{
			/*��ȡɨ���÷��������������*/
			char *pNet = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">����</p></td><td><p>");
			if (pNet)
			{
				char *pEnd = strstr(pNet,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pNet,(pEnd - pNet)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">����</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_nPrintedNet = atoi(szNeedInfo);
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,1,ɨ���÷������������%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,2,��ȡɨ���÷����������ʧ��");
			}
			/*��ȡɨ���÷���USB�˿�*/
			char *pUsbPort = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB �˿�</p></td><td><p>");
			if (pUsbPort)
			{
				char *pEnd = strstr(pUsbPort,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pUsbPort,(pEnd - pUsbPort)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB �˿�</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_szUsbPort = szNeedInfo;
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,3,ɨ���÷���USB�˿���:%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,4,��ȡɨ���÷���USB�˿�ʧ��");
			}
		}
		char *pFax = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>�����÷�</b></p></td><td><p>");
		if (pFax)
		{
			/*��ȡ�����÷�����ҵ���ѽ���*/
			char *pOperation = strstr(pFax,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ҵ</b></p></td><td><p>");
			if (pOperation)
			{
				char *pAccepted = strstr(pOperation,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">�ѽ���</p></td><td><p>");
				if (pAccepted)
				{
					char *pEnd = strstr(pAccepted,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pAccepted,(pEnd - pAccepted)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">�ѽ���</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szAccepted = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,5,��ȡ�����÷�����ҵ�ѽ��ռ�����:%s",szNeedInfo);
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,6,��ȡ�����÷�����ҵ�ѽ�����ҵ����ʧ��");
			}
		}
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>����Ʒ��Ϣ</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*��ȡ����Ʒ��Ϣ�º�ɫ̼���µ�̼��ˮƽ*/
			char *pBlcakToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ɫ̼��</b></p></td><td><p>");
			if (pBlcakToner)
			{
				char *pTonerLevel = strstr(pBlcakToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,7,��ȡ����Ʒ��Ϣ�º�ɫ̼�۵�̼��ˮƽ��%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,8,��ȡ����Ʒ��Ϣ�º�ɫ̼�۵�̼��ˮƽʧ��");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de��9����ȡ��ɫ̼��ʧ��");
			}
			/*��ȡ����Ʒ��Ϣ��ά�����߰��µ�����Ʒˮƽ*/
			char *pMaintainTool = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>ά�����߰�</b></p></td><td><p>");
			if (pMaintainTool)
			{
				char *pConsumeLevel = strstr(pMaintainTool,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szConsumeLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,,10,��ȡ����Ʒ��Ϣ��ά�����߰�������Ʒˮƽ��:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,11,��ȡ����Ʒ��Ϣ��ά�����߰�������Ʒˮƽʧ��");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,12,��ȡ����Ʒ��Ϣ��ά�����߰�ʧ��");
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/*         ͨ����ȡ����XM1145��̬ҳ��ķ�ʽ��ȡ����������Ϣ             */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	//theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,1,szUrl=%s", szUrl);
	list <char *> *pListMsg;
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	int nLength = strlen("</TR>");
	pListMsg = HtmlToList(szUrl);
	list<char *>::iterator it = pListMsg->begin();
	for (; it != pListMsg->end(); it++)
	{
		char *pTableTitle = *it;
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>����Ʒ��Ϣ</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*��ȡ����Ʒ��Ϣ�º�ɫ̼�ۺе�̼��ˮƽ*/
			char *pBlackToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ɫ̼�ۺ�</b></p></td><td><p>");
			if (pBlackToner)
			{
				char *pTonerLevel = strstr(pBlackToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,1,����Ʒ��Ϣ�º�ɫ̼�ۺе�̼��ˮƽ��:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,2,��ȡ��ɫ̼�ۺе�̼��ˮƽʧ��");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,3,��ȡ��ɫ̼�ۺ�ʧ��");
			}
			/*��ȡ���񲿼��µ�����Ʒˮƽ*/
			char *pImageBlock = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>���񲿼�</b></p></td><td><p>");
			if (pImageBlock)
			{
				char *pConsumeLevel = strstr(pImageBlock,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szConsumeLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,4,����Ʒ��Ϣ�³��񲿼�������Ʒˮƽ��:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,5,��ȡ���񲿼�������Ʒˮƽʧ��");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,6,��ȡ���񲿼�ʧ��");
			}
		}
		char *pMediumSufaceCount = strstr(pTableTitle,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>�����Ѵ�ӡ�����</b></p></td><td><p>");
		if (pMediumSufaceCount)
		{
			char *pMediumSufacePrintedCount = strstr(pMediumSufaceCount,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>�����Ѵ�ӡ�����</b></p></td><td><p> ");
			if (pMediumSufacePrintedCount)
			{
				char *pPrintedCount = strstr(pMediumSufacePrintedCount,"<TR><td><p align=\"left\" style=\"margin-left: 40;\"><b>��ӡ</b></p></td><td><p>");
				if (pPrintedCount)
				{
					char *pPrintedTotalCount = strstr(pPrintedCount,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">�ܼ�</p></td><td><p>");
					if (pPrintedTotalCount)
					{
						char *pEnd = strstr(pPrintedTotalCount,"</TR>");
						memset(pBuf, 0, dwBufLen);
						strncpy(pBuf,pPrintedTotalCount,(pEnd - pPrintedTotalCount)+nLength);
						CString szNeedInfo;
						szNeedInfo.Format("%s",pBuf);
						szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">�ܼ�</p></td><td><p>","");
						szNeedInfo.Replace("</p></td></TR>","");
						szNeedInfo.Replace(" ","");
						m_szConsumeLevel = szNeedInfo;
						m_oMeterInfo.nA4PrintTotalPage = atoi(szNeedInfo);
						theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,A4��ӡ�ܼ�%s",szNeedInfo);
					}
				}
			}
		}

	}
	return TRUE;
}

/************************************************************************/
/*         ͨ����ȡ����XM1145��̬ҳ��ķ�ʽ��ȡ����������Ϣ (2)         */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145_2()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145_2,2,szUrl=%s", szUrl);
	CStringArray ary;
	if (!HtmlToList(szUrl, ary))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145_2,3,HtmlToList fail");
		return FALSE;		
	}

	for (int i=0; i<ary.GetCount(); i++)
	{
		CString szCurrentLine = ary.GetAt(i);
		if (szCurrentLine.Find("�����Ѵ�ӡ�����") >= 0)
		{
			CString szNextLine2 = ary.GetAt(i+2);
			if (szNextLine2.Find("��ӡ") >= 0)
			{
				CString szNextLine4 = ary.GetAt(i+4);
				if (szNextLine4.Find("��ɫ") >= 0)
				{
					szNextLine4.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">��ɫ</p></td><td><p>", "");
					szNextLine4.Replace("</p></td>", "");
					szNextLine4.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine4);
				}
			}
			CString szNextLine8 = ary.GetAt(i+8);
			if (szNextLine8.Find("��ӡ") >= 0)
			{
				CString szNextLine10 = ary.GetAt(i+10);
				if (szNextLine10.Find("��ɫ") >= 0)
				{
					szNextLine10.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">��ɫ</p></td><td><p>", "");
					szNextLine10.Replace("</p></td>", "");
					szNextLine10.Trim();
					m_oMeterInfo.nOtherCopyTotalPage = atoi(szNextLine10);
				}
			}
		}
	}

	return TRUE;
}

/************************************************************************/
/*         ͨ����ȡ����X950��̬ҳ��ķ�ʽ��ȡ����������Ϣ               */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X950()
{
	list <char *> *pListMsg;
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	int nLength = strlen("</TR>");
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	pListMsg = HtmlToList(szUrl);
	list<char *>::iterator it = pListMsg->begin();
	for (; it != pListMsg->end(); it++)
	{
		char *pTableTitle = *it;
		char *pMediumSufaceCount = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>���������</b></p></td><td><p>");
		/*��ȡ����������´�ӡ��ɫ�����µ��ܼ�*/
		if (pMediumSufaceCount)
		{
			char *pPrintedHBSufaceCount = strstr(pMediumSufaceCount,"<TR><td><p align=\"left\" style=\"margin-left: 40;\"><b>��ӡ�ĵ�ɫ����</b></p></td><td><p>");
			if (pPrintedHBSufaceCount)
			{
				char *pHBTotal = strstr(pPrintedHBSufaceCount,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">�ܼ�</p></td><td><p>");
				if (pHBTotal)
				{
					char *pEnd = strstr(pHBTotal,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pHBTotal,(pEnd - pHBTotal)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">�ܼ�</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
				}
			}
		}
		char *pScanUseage = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>ɨ���÷�</b></p></td><td><p>");
		if (pScanUseage)
		{	/*��ȡɨ���÷�����ɨ��ҳ��������ɨ�����*/
			char *pScanedPage = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ɨ��ҳ��</b></p></td><td><p>");
			if (pScanedPage)
			{
				char *pScanForNet = strstr(pScanedPage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">����</p></td><td><p>");
				if (pScanForNet)
				{
					char *pEnd = strstr(pScanForNet,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pScanForNet,(pEnd - pScanForNet)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">����</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_nPrintedNet = atoi(szNeedInfo); 
				}
			}
		}
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>����Ʒ��Ϣ</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*��ȡ����Ʒ��Ϣ�º�ɫ����������Ʒˮƽ*/
			char *pBlackLightElectricStick = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ɫ����</b></p></td><td><p>");
			if (pBlackLightElectricStick)
			{
				char *pConsumeLevel = strstr(pBlackLightElectricStick,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szConsumeLevel = szNeedInfo; 
				}
			}
		}	
	}
	return TRUE;
}
/************************************************************************/
/*          ͨ����ȡ����MX811��̬ҳ��ķ�ʽ��ȡ����������Ϣ              */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811()
{
	list <char *> *pListMsg;
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	int nLength = strlen("</TR>");
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	pListMsg = HtmlToList(szUrl);
	list<char *>::iterator it = pListMsg->begin();
	for (; it != pListMsg->end(); it++)
	{
		char *pTableTitle = *it;
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>����Ʒ��Ϣ</b></p></td><td><p>");
		if (pConsumeInfo)
		{	/*��ȡ����Ʒ��Ϣ�º�ɫ̼�ۺ�̼��ˮƽ*/
			char *pBlackToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ɫ̼�ۺ�</b></p></td><td><p>");
			if (pBlackToner)
			{
				char *pTonerLevel = strstr(pBlackToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811,1,��ɫ̼�ۺ�̼��ˮƽ:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811,2,��ȡ����Ʒ��Ϣ�º�ɫ̼�ۺ�̼��ˮƽʧ��");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811,3,��ȡ��ɫ̼�ۺ�ˮƽʧ��");
			}
		}
	}
	
	return TRUE;
}

/************************************************************************/
/*          ͨ����ȡ����MX811��̬ҳ��ķ�ʽ��ȡ����������Ϣ ��2��       */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811_2()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811_2,2,szUrl=%s", szUrl);
	CStringArray ary;
	if (!HtmlToList(szUrl, ary))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811_2,3,HtmlToList fail");
		return FALSE;		
	}

	for (int i=0; i<ary.GetCount(); i++)
	{
		CString szCurrentLine = ary.GetAt(i);
		if (szCurrentLine.Find("�����Ѵ�ӡ�����") >= 0)
		{
			CString szNextLine2 = ary.GetAt(i+2);
			if (szNextLine2.Find("��ӡ") >= 0)
			{
				CString szNextLine4 = ary.GetAt(i+4);
				if (szNextLine4.Find("��ɫ") >= 0)
				{
					szNextLine4.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">��ɫ</p></td><td><p>", "");
					szNextLine4.Replace("</p></td>", "");
					szNextLine4.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine4);
				}
			}
			CString szNextLine8 = ary.GetAt(i+8);
			if (szNextLine8.Find("��ӡ") >= 0)
			{
				CString szNextLine10 = ary.GetAt(i+10);
				if (szNextLine10.Find("��ɫ") >= 0)
				{
					szNextLine10.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">��ɫ</p></td><td><p>", "");
					szNextLine10.Replace("</p></td>", "");
					szNextLine10.Trim();
					m_oMeterInfo.nOtherCopyTotalPage = atoi(szNextLine10);
				}
			}
		}
	}

	return TRUE;
}

/************************************************************************/
/*          ͨ����ȡ����X860de��̬ҳ��ķ�ʽ��ȡ����������Ϣ            */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de()
{
	list <char *> *pListMsg;
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	int nLength = strlen("</TR>");
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	pListMsg = HtmlToList(szUrl);
	list<char *>::iterator it = pListMsg->begin();
	for (; it != pListMsg->end(); it++)
	{
		char *pTableTitle = *it;
		char *pScanUseage = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>ɨ���÷�</b></p></td><td><p>");
		if (pScanUseage)
		{
			//ɨ���÷����������
			char *pPrintedNet = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">����</p></td><td><p>");
			if (pPrintedNet)
			{
				char *pEnd = strstr(pPrintedNet,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pPrintedNet,(pEnd - pPrintedNet)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">����</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_nPrintedNet = atoi(szNeedInfo);
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,1,��ȡɨ���÷�����·������:%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,2,��ȡɨ���÷����������ʧ��");
			}
			//��ȡɨ���÷���USB�˿�
			char *pUsbPort = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB �˿�</p></td><td><p>");
			if (pUsbPort)
			{
				char *pEnd = strstr(pUsbPort,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pUsbPort,(pEnd - pUsbPort)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB �˿�</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_szUsbPort = szNeedInfo;  
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,3,��ȡɨ���÷���USB�˿���:%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,4,��ȡɨ���÷���USB�˿�ʧ��");
			}
		}
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>����Ʒ��Ϣ</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			//��ȡ����Ʒ��Ϣ�º�ɫ̼�۵�̼��ˮƽ
			char *pBlcakToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>��ɫ̼��</b></p></td><td><p>");
			if (pBlcakToner)
			{
				char *pTonerLevel = strstr(pBlcakToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">̼��ˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,5,��ȡ����Ʒ��Ϣ���º�ɫ̼�۵�̼��ˮƽ��:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,6,��ȡ����Ʒ��Ϣ���º�ɫ̼�۵�̼��ˮƽʧ��");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,7,��ȡ����Ʒ��Ϣ���º�ɫ̼��ʧ��");
			}
		}
	}
	return TRUE;
}

/************************************************************************/
/*          ͨ����ȡ����X860de��̬ҳ��ķ�ʽ��ȡ����������Ϣ  (2)       */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de_2()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de_2,2,szUrl=%s", szUrl);
	CStringArray ary;
	if (!HtmlToList(szUrl, ary))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de_2,3,HtmlToList fail");
		return FALSE;		
	}

	for (int i=0; i<ary.GetCount(); i++)
	{
		CString szCurrentLine = ary.GetAt(i);
		if (szCurrentLine.Find("���������") >= 0)
		{
			CString szNextLine4 = ary.GetAt(i+4);
			if (szNextLine4.Find("��ӡ") >= 0)
			{
				CString szNextLine6 = ary.GetAt(i+6);
				if (szNextLine6.Find("��ɫ") >= 0)
				{
					szNextLine6.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">��ɫ</p></td><td><p>", "");
					szNextLine6.Replace("</p></td>", "");
					szNextLine6.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine6);
				}
			}
			CString szNextLine10 = ary.GetAt(i+10);
			if (szNextLine10.Find("��ӡ") >= 0)
			{
				CString szNextLine12 = ary.GetAt(i+12);
				if (szNextLine12.Find("��ɫ") >= 0)
				{
					szNextLine12.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">��ɫ</p></td><td><p>", "");
					szNextLine12.Replace("</p></td>", "");
					szNextLine12.Trim();
					m_oMeterInfo.nOtherCopyTotalPage = atoi(szNextLine12);
				}
			}
		}
	}

	return TRUE;
}

/************************************************************************/
/*          ͨ����ȡ����MX610de��̬ҳ��ķ�ʽ��ȡ����������Ϣ            */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX610de()
{
	list <char *> *pListMsg;
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	int nLength = strlen("</TR>");
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	pListMsg = HtmlToList(szUrl);
	list<char *>::iterator it = pListMsg->begin();
	for (; it != pListMsg->end(); it++)
	{
		char *pTableTitle = *it;
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>����Ʒ��Ϣ</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*��ȡ���񲿼��µ�����Ʒˮƽ*/
			char *pImageBlock = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>���񲿼�</b></p></td><td><p>");
			if (pImageBlock)
			{
				char *pConsumeLevel = strstr(pImageBlock,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">����Ʒˮƽ</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szConsumeLevel = szNeedInfo;
				}
			}
		}
	}
	return TRUE;
}

/************************************************************************/
/*          ͨ����ȡ����MX610de��̬ҳ��ķ�ʽ��ȡ����������Ϣ   (2)     */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX610de_2()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_Str_Http, m_szIP, DEF_Str_Url);
	theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX610de_2,2,szUrl=%s", szUrl);
	CStringArray ary;
	if (!HtmlToList(szUrl, ary))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX610de_2,3,HtmlToList fail");
		return FALSE;		
	}

	for (int i=0; i<ary.GetCount(); i++)
	{
		CString szCurrentLine = ary.GetAt(i);
		if (szCurrentLine.Find("�����Ѵ�ӡ�����") >= 0)
		{
			CString szNextLine2 = ary.GetAt(i+2);
			if (szNextLine2.Find("��ӡ") >= 0)
			{
				CString szNextLine4 = ary.GetAt(i+4);
				if (szNextLine4.Find("��ɫ") >= 0)
				{
					szNextLine4.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">��ɫ</p></td><td><p>", "");
					szNextLine4.Replace("</p></td>", "");
					szNextLine4.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine4);
				}
			}
			CString szNextLine8 = ary.GetAt(i+8);
			if (szNextLine8.Find("��ӡ") >= 0)
			{
				CString szNextLine10 = ary.GetAt(i+10);
				if (szNextLine10.Find("��ɫ") >= 0)
				{
					szNextLine10.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">��ɫ</p></td><td><p>", "");
					szNextLine10.Replace("</p></td>", "");
					szNextLine10.Trim();
					m_oMeterInfo.nOtherCopyTotalPage = atoi(szNextLine10);
				}
			}
		}
	}

	return TRUE;
}

/************************************************************************/
/*         ����web��html�ָ���ַ�������				                */
/************************************************************************/
list<char *>* CSnmpPrinterMeterMonitorLM::HtmlToList(CString szUrl)
{
	CInternetSession *pInetSession = new CInternetSession; 
	CHttpFile *pHttpFile = NULL ;
	pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //��һ��URL
	if (!pHttpFile)
	{
		theLog.Write("CSnmpPrinterMeterMonitorLM::HtmlToList,1,fail");
	}
	CString str ;
	CString szinfo="";
	while(pHttpFile->ReadString(str))   
	{
		szinfo += CCommonFun::UTF8ToMultiByte((const unsigned char*)str.GetString()) ;
	}
	list<char *> *pListMsg = new list<char *>;
	DWORD dwInfoLen = szinfo.GetLength();
	char *pBeginPos = new char[dwInfoLen + 1];
	memset(pBeginPos, 0x0, dwInfoLen + 1);
	memcpy(pBeginPos, LPCTSTR(szinfo), dwInfoLen);
	char *pEndPos = NULL;
	int nLength = strlen("</TABLE>");
	DWORD dwBufLen = 4096;
	char *pBuf = new char[dwBufLen];
	CString szBenginPos = szinfo;
	if(!pBuf)
	{
	}
	DWORD dwMsgLen = 0;
	while(1)
	{
		pBeginPos = strstr(pBeginPos, "<TABLE>");
		if(!pBeginPos)
		{
			theLog.Write("!!CmonitorDlg::OnRead,10");
			break;
		}
		pEndPos = strstr(pBeginPos, "</TABLE>");
		if(!pEndPos)
		{
			break;
		}
		memset(pBuf, 0, dwBufLen);
		dwMsgLen = (pEndPos - pBeginPos) + nLength;
		if(dwMsgLen > dwBufLen)
		{
			delete []pBuf;
			dwBufLen = dwMsgLen*2;
			pBuf = new char[dwBufLen];
			memset(pBuf, 0, dwBufLen);
		}
		strncpy(pBuf, pBeginPos, dwMsgLen);
		char *pTmp = new char[dwMsgLen];
		memcpy(pTmp, pBuf, dwMsgLen);
		pListMsg->push_back(pTmp);
		pBeginPos = pBeginPos + (pEndPos - pBeginPos + nLength);
		char *pRet = strstr(pBeginPos, "<TABLE>");
		if(!pRet)
		{
			break;
		}
	}
	delete []pBuf;
	pBuf = NULL;
	pBeginPos = NULL;

	delete pHttpFile;
	pHttpFile = NULL;

	delete pInetSession;
	pInetSession = NULL;

	return pListMsg;
}

/************************************************************************/
/*         ����web��html�ָ���ַ�������				                */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::HtmlToList(CString szUrl, CStringArray& szAry)
{
	BOOL bRet = FALSE;
	try
	{
		CInternetSession *pInetSession = new CInternetSession; 
		CHttpFile *pHttpFile = NULL ;
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //��һ��URL
		if (!pHttpFile)
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorLM::HtmlToList,1,fail,err=%d,szUrl=%s"
				, GetLastError(), szUrl);
			delete pInetSession;
			return bRet;
		}

		CString szOneLine ;
		szAry.RemoveAll();
		while(pHttpFile->ReadString(szOneLine))   
		{
			szAry.Add(CCommonFun::UTF8ToMultiByte((const unsigned char*)szOneLine.GetString()));
		}

		delete pHttpFile;
		pHttpFile = NULL;

		delete pInetSession;
		pInetSession = NULL;

		bRet = TRUE;
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::HtmlToList,fail,catch CException [%s]", tcErrMsg);
	}

	return bRet;
}
#endif

//��ȡ��ӡ������ͳ����Ϣ
BOOL CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo()
{
	int nValue;
	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	//�ر�˵�������˴�ӡ�������С�����ҳ���͡������桿������һ�������1ҳ=2�档
	//��ӡ�����֡�ʰȡ�����͡�ʵ�ʳ�ֽ������
	//������ֻͳ�ơ������桿�ġ�ʵ�ʳ�ֽ������
	//������ֻͳ�Ƽ���Ϊ��ӡ�������ָ�ӡ�ʹ��棬��Ϊ��ӡ�ʹ��������Ǵ�ӡ������ͳ�Ƶġ�

	//1.�������д�ӡ���ֽ�ͼ�������
	PaperSidesCountEntryMap oPaperSidesCount;
	char cOidBegin[128] = {0};
	sprintf(cOidBegin, "%s.2", m_szPaperSidesCountEntryOID);	//PaperSidesCountEntryOID
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (oPaperSidesCount.find(nIndex) == oPaperSidesCount.end())
			{
				PPaperSidesCountEntry pEntry = new PaperSidesCountEntry;
				memset(pEntry, 0x0, sizeof(PaperSidesCountEntry));
				oPaperSidesCount.insert(pair<int,PPaperSidesCountEntry>(nIndex, pEntry));
				pEntry->paperSidesCountIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	//2.���ݴ�ӡ���ֽ�ͼ�����������ȡ����Ĵ�ӡ������Ϣ����ֽ�͡�ɫ�ʡ�ʰȡ������ʵ�ʳ�ֽ������
	PaperSidesCountEntryMap::iterator it;
	for (it=oPaperSidesCount.begin(); it!=oPaperSidesCount.end(); it++)
	{
		PPaperSidesCountEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->paperSidesCountIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.2.1.%d", m_szPaperSidesCountEntryOID, nIndex);	//paperSidesPaperSize=2	//PaperSidesCountEntryOID
			GetRequest(cOidStr, (int&)pEntry->paperSidesPaperSize);

			sprintf(cOidStr, "%s.3.1.%d", m_szPaperSidesCountEntryOID, nIndex);	//paperSidesPaperType=3
			GetRequest(cOidStr, (int&)pEntry->paperSidesPaperType);

			sprintf(cOidStr, "%s.4.1.%d", m_szPaperSidesCountEntryOID, nIndex);	//paperSidesMonoPicked=4
			GetRequest(cOidStr, pEntry->paperSidesMonoPicked);

			sprintf(cOidStr, "%s.5.1.%d", m_szPaperSidesCountEntryOID, nIndex);	//paperSidesColorPicked=5
			GetRequest(cOidStr, pEntry->paperSidesColorPicked);

			sprintf(cOidStr, "%s.6.1.%d", m_szPaperSidesCountEntryOID, nIndex);	//paperSidesMonoSafe=6
			GetRequest(cOidStr, pEntry->paperSidesMonoSafe);

			sprintf(cOidStr, "%s.7.1.%d", m_szPaperSidesCountEntryOID, nIndex);	//paperSidesColorSafe=7
			GetRequest(cOidStr, pEntry->paperSidesColorSafe);
		}
	}

	//3.ͳ�ƴ�ӡʵ�ʳ�ֽ������Ϣ��
	for (it=oPaperSidesCount.begin(); it!=oPaperSidesCount.end(); it++)
	{
		PPaperSidesCountEntry pEntry = it->second;
		if (pEntry)
		{
			if (pEntry->paperSidesPaperSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA4)
			{
				m_oMeterInfo.nA4PrintHBPage += pEntry->paperSidesMonoSafe;
				m_oMeterInfo.nA4PrintColorPage += pEntry->paperSidesColorSafe;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,��ӡA4�ڰ׽�������=%d��A4��ɫ��������=%d"
				//	, pEntry->paperSidesMonoSafe, pEntry->paperSidesColorSafe);
			}
			else if (pEntry->paperSidesPaperSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA3)
			{
				m_oMeterInfo.nA3PrintHBPage += pEntry->paperSidesMonoSafe;
				m_oMeterInfo.nA3PrintColorPage += pEntry->paperSidesColorSafe;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,��ӡA3�ڰ׽�������=%d��A3��ɫ��������=%d"
				//	, pEntry->paperSidesMonoSafe, pEntry->paperSidesColorSafe);
			}
			else
			{
				m_oMeterInfo.nOtherPrintHBPage += pEntry->paperSidesMonoSafe;
				m_oMeterInfo.nOtherPrintColorPage += pEntry->paperSidesColorSafe;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,��ӡ����ֽ��(%d)���ڰ׽�������=%d����ɫ��������=%d"
				//	, pEntry->paperSidesPaperSize, pEntry->paperSidesMonoSafe, pEntry->paperSidesColorSafe);
			}

			delete pEntry;
		}
	}

	int nGetPaperSizeCount = oPaperSidesCount.size();
	oPaperSidesCount.clear();

	//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,nGetPaperSizeCount=%d", nGetPaperSizeCount);

	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage;
	if(other > 0)
	{
		m_oMeterInfo.nOtherOpHBPage = other;
	}

	return (nGetPaperSizeCount>0) ? TRUE : FALSE;
}

//��ȡɨ��ͳ����Ϣ
BOOL CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo()
{
	//�ر�˵�������˴�ӡ�������С�����ҳ���͡������桿������һ�������1ҳ=2�档

	//1.��������ɨ�����ֽ�ͼ�������
	ScanCountEntryMap oScanSidesCount;
	char cOidBegin[128] = {0};
	sprintf(cOidBegin, "%s.2", m_szScanCountEntryOID);	//ScanCountEntryOID
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (oScanSidesCount.find(nIndex) == oScanSidesCount.end())
			{
				PScanCountEntry pEntry = new ScanCountEntry;
				memset(pEntry, 0x0, sizeof(ScanCountEntry));
				oScanSidesCount.insert(pair<int,PScanCountEntry>(nIndex, pEntry));
				pEntry->scanCountIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	//2.����ɨ�����ֽ�ͼ�����������ȡ�����ɨ�������Ϣ�������͡�ֽ�͡�������ҳ����
	ScanCountEntryMap::iterator it;
	for (it=oScanSidesCount.begin(); it!=oScanSidesCount.end(); it++)
	{
		PScanCountEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->scanCountIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.2.1.%d", m_szScanCountEntryOID, nIndex);	//scanCountType=2	//ScanCountEntryOID
			GetRequest(cOidStr, (int&)pEntry->scanCountType);

			if (pEntry->scanCountType != ENUM_STRUCT_VALUE(ScanCountType)::scanToEmailAdf
				&& pEntry->scanCountType != ENUM_STRUCT_VALUE(ScanCountType)::scanToEmailFlatbed)
			{
				continue;
			}

			sprintf(cOidStr, "%s.3.1.%d", m_szScanCountEntryOID, nIndex);	//scanCountSize=3
			GetRequest(cOidStr, (int&)pEntry->scanCountSize);

			sprintf(cOidStr, "%s.4.1.%d", m_szScanCountEntryOID, nIndex);	//scanCountSides=4
			GetRequest(cOidStr, pEntry->scanCountSides);

			sprintf(cOidStr, "%s.5.1.%d", m_szScanCountEntryOID, nIndex);	//scanCountSheets=5
			GetRequest(cOidStr, pEntry->scanCountSheets);
		}
	}

	//3.ͳ��ɨ��������Ϣ��
	for (it=oScanSidesCount.begin(); it!=oScanSidesCount.end(); it++)
	{
		PScanCountEntry pEntry = it->second;
		if (pEntry)
		{
			if (pEntry->scanCountType != ENUM_STRUCT_VALUE(ScanCountType)::scanToEmailAdf
				&& pEntry->scanCountType != ENUM_STRUCT_VALUE(ScanCountType)::scanToEmailFlatbed)
			{
				continue;
			}

			if (pEntry->scanCountSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA4)
			{
				m_oMeterInfo.nScanSmallColorPage += pEntry->scanCountSides;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,ɨ������(%d),ɨ��A4��������=%d"
				//	, pEntry->scanCountType, pEntry->scanCountSides);
			}
			else if (pEntry->scanCountSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA3)
			{
				m_oMeterInfo.nScanBigColorPage += pEntry->scanCountSides;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,ɨ������(%d),ɨ��A3��������=%d"
				//	, pEntry->scanCountType, pEntry->scanCountSides);
			}
			else
			{
				m_oMeterInfo.nScanSmallColorPage += pEntry->scanCountSides;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,ɨ������(%d),ɨ��ֽ��(%d),ɨ��������������=%d"
				//	, pEntry->scanCountType, pEntry->scanCountSize, pEntry->scanCountSides);
			}

			delete pEntry;
		}
	}

	int nGetScanSizeCount = oScanSidesCount.size();
	oScanSidesCount.clear();

	//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,nGetScanSizeCount=%d", nGetScanSizeCount);

	return (nGetScanSizeCount>0) ? TRUE : FALSE;
}
