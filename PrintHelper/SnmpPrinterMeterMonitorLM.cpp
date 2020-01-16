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

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	//获取打印介质面统计信息
	BOOL bGetPrint = GetPrintSidesCountInfo();

	//获取扫描介质面统计信息
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
/* 获取X654打印机抄表信息                                               */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de()
{
	/*
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.2.0", m_nValue))//获取利盟X654de已打印介质面总计
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,1,获取已打印介质面总结失败");
	}
	else
	{
		 m_oMeterInfo.nA4PrintHBPage = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,2,X654de已打印介质面总计=%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf,1024))//获取利盟X654de打印机型号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,3,获取打印机型号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,4,利盟X654de打印机型号=%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo,m_pBuf,1024))//获取利盟X654de打印机序列号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,5,获取打印机序列号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X654de,6,打印机序列号是:%s",m_szValue);
	}
	//GetOtherMeterInfoByHtml_X654de();//获取X654抄表其它信息
	*/
	return TRUE;
}
/************************************************************************/
/* 获取XM1145抄表信息                                                   */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.2.1.6.1.1",m_nValue))//获取XM1145介质面已打印面总计数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,1,fail");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,2,XM1145介质面计数下介质面已打印面总计:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.1.1.4.1.11",m_nValue))//获取XM1145介质面复印总计数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,1,fail");
	}
	else
	{
		m_oMeterInfo.nA4CopyTotalPage = m_nValue;
		m_oMeterInfo.nA4PrintTotalPage = m_oMeterInfo.nA4PrintTotalPage - m_oMeterInfo.nA4CopyTotalPage;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,2,XM1145介质面计数下复印总计数:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.3.3.1.2.1.15",m_nValue))//获取XM1145扫描用法网络
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,3,扫描用法下网络计数失败");
	}
	else
	{
		m_nPrintedNet = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,4,XM1145扫描用法下网络计数:%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))	//获取XM1145打印机序列号
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,5,获取打印机序列号失败,m_pBuf=%s",m_pBuf);
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,6,打印机序列号：%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf))//获取XM1145打印机型号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,7,获取打印机型号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_XM1145,8,打印机型号是:%s",m_szValue);
	}
	return GetOtherMeterInfoByHtml_XM1145(pInfo);//获取XM1145其他抄表信息
#else
	return GetOtherMeterInfoByHtml_XM1145_2();//获取XM1145其他抄表信息
#endif
}

/************************************************************************/
/* 获取X950抄表信息                                                     */
/************************************************************************/
BOOL  CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950()
{
	/*
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.2.0",m_nValue))//获取XM950已打印单色面计数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,1,fail");
	}
	else
	{
		m_nHBTotal = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,2,XM950已打印单色面数是:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.3.0",m_nValue))//获取XM950已打印彩色面计数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,3,fail");
	}
	else
	{
		m_nPrintedColorTotal = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,4,XM950已打印彩色面计数是:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.2.1.6.1.1",m_nValue))//获取XM950打印的单色A4-普通纸张
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,3,fail");
	}
	else
	{
		m_oMeterInfo.nA4PrintHBPage = m_nValue;
		theLog.Write("XM950打印的单色A4-普通纸张是:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.2.1.7.1.1",m_nValue))//获取XM950打印的彩色A4-普通纸张总计数
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,4,fail");
	}
	else
	{	
		m_oMeterInfo.nA4PrintColorPage = m_nValue;
		theLog.Write("XM950打印的彩色A4-普通纸张总计数是:%d",m_nValue);
	}
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.4.1.1.17.1.1",m_nValue))//获取XM950打印彩色面数总计
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,5,fail");
	}
	else
	{
		m_nA4PrintedColorTotal = m_nValue;
		theLog.Write("XM950打印彩色面总计是:%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo,m_pBuf,1024))//获取XM950打印机序列号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,6,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("XM950打印机序列号是:%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(".1.3.6.1.4.1.641.641.2.1.2.1.2.1",m_pBuf,1024))//获取XM950打印机型号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X950,7,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("XM950打印机型号是:%s",m_szValue);
	}
	//GetOtherMeterInfoByHtml_X950();//获取XM950其他抄表信息
	*/
	return TRUE;
}
/************************************************************************/
/*获取利盟MX811抄表信息                                               */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.1.1.4.1.3",m_nValue))//获取MX811de介质面计数的已打印总计
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,1,fail");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,2,MX811de介质面计数的已打印总计=%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model,m_pBuf))//获取利盟MX811de打印机型号
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,3,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,4,打印机型号:%s",m_szValue);
	}
	//m_szValue.ReleaseBuffer();
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))//获取利盟MX811de打印机序列号
	{
		theLog.Write("!! CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,5,fail");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX811,6,打印机序列号:%s",m_szValue);
	}
	//m_szValue.ReleaseBuffer();
	GetOtherMeterInfoByHtml_MX811();//获取MX811其他抄表信息
	return TRUE;
#else
	return GetOtherMeterInfoByHtml_MX811_2();
#endif

}
/************************************************************************/
/*获取利盟X860de抄表信息                                                */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.2.1.5.2.0",m_nValue))//获取X860de介质面计数的已打印总计
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de,1,获取介质面计数已打印总计失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("X860de介质面计数的已打印总计:%d",m_nValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model, m_pBuf))//获取利盟X860de打印机型号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de,2,获取打印机型号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("X860de打印机型号:%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))//获取利盟X860de打印机序列号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_X860de,3,获取打印机序列号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("打印机序列号:%s",m_szValue);
	}
	GetOtherMeterInfoByHtml_X860de();//获取X860de其他抄表信息
	return TRUE;
#else
	return GetOtherMeterInfoByHtml_X860de_2();
#endif
}
/************************************************************************/
/*获取利盟MX610de抄表信息                                                */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de()
{
#if 0
	if (!GetRequest(".1.3.6.1.4.1.641.6.4.2.1.1.4.1.3",m_nValue))//获取MX610dede介质面计数的已打印总计
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,1,获取介质面计数已打印总计失败");
	}
	else
	{
		m_oMeterInfo.nA4PrintTotalPage = m_nValue;
		theLog.Write("MX610de介质计数的已打印总计:%d",m_nValue);

	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_Model, m_pBuf))//获取利盟MX610de打印机型号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,3,获取打印机型号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterModel = m_szValue;
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,4,MX610de打印机型号是:%s",m_szValue);
	}
	memset(m_pBuf,0x0,1024);
	if (!GetRequestStr(DEF_Str_OID_Printer_SerialNo, m_pBuf))//获取利盟MX610de打印机序列号
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,5,获取打印机序列号失败");
	}
	else
	{
		m_szValue.Format("%s",m_pBuf);
		m_szPrinterSerialNo = m_szValue;
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetMeterInfo_MX610de,6，MX610de打印机序列号是:%s",m_szValue);
	}
	
	GetOtherMeterInfoByHtml_MX610de();//获取X610de其他抄表信息
	return TRUE;
#else
	return GetOtherMeterInfoByHtml_MX610de_2();
#endif

}
/************************************************************************/
/*         通过读取利盟X654静态页面的方式获取抄表其它信息               */
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
		char *pScanUseage = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>扫描用法</b></p></td><td><p>");
		if (pScanUseage)
		{
			/*获取扫描用法下用于网络计数*/
			char *pNet = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">网络</p></td><td><p>");
			if (pNet)
			{
				char *pEnd = strstr(pNet,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pNet,(pEnd - pNet)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">网络</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_nPrintedNet = atoi(szNeedInfo);
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,1,扫描用法下网络计数是%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,2,获取扫描用法下网络计数失败");
			}
			/*获取扫描用法下USB端口*/
			char *pUsbPort = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB 端口</p></td><td><p>");
			if (pUsbPort)
			{
				char *pEnd = strstr(pUsbPort,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pUsbPort,(pEnd - pUsbPort)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB 端口</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_szUsbPort = szNeedInfo;
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,3,扫描用法下USB端口是:%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,4,获取扫描用法下USB端口失败");
			}
		}
		char *pFax = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>传真用法</b></p></td><td><p>");
		if (pFax)
		{
			/*获取传真用法下作业的已接收*/
			char *pOperation = strstr(pFax,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>作业</b></p></td><td><p>");
			if (pOperation)
			{
				char *pAccepted = strstr(pOperation,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">已接收</p></td><td><p>");
				if (pAccepted)
				{
					char *pEnd = strstr(pAccepted,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pAccepted,(pEnd - pAccepted)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">已接收</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szAccepted = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,5,获取传真用法下作业已接收计数是:%s",szNeedInfo);
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,6,获取传真用法下作业已接收作业计数失败");
			}
		}
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>消耗品信息</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*获取消耗品信息下黑色碳粉下的碳粉水平*/
			char *pBlcakToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>黑色碳粉</b></p></td><td><p>");
			if (pBlcakToner)
			{
				char *pTonerLevel = strstr(pBlcakToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,7,获取消耗品信息下黑色碳粉的碳粉水平是%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,8,获取消耗品信息下黑色碳粉的碳粉水平失败");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de，9，获取黑色碳粉失败");
			}
			/*获取消耗品信息下维护工具包下的消耗品水平*/
			char *pMaintainTool = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>维护工具包</b></p></td><td><p>");
			if (pMaintainTool)
			{
				char *pConsumeLevel = strstr(pMaintainTool,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szConsumeLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,,10,获取消耗品信息下维护工具包的消耗品水平是:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,11,获取消耗品信息下维护工具包的消耗品水平失败");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X654de,12,获取消耗品信息下维护工具包失败");
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/*         通过读取利盟XM1145静态页面的方式获取抄表其它信息             */
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
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>消耗品信息</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*获取消耗品信息下黑色碳粉盒的碳粉水平*/
			char *pBlackToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>黑色碳粉盒</b></p></td><td><p>");
			if (pBlackToner)
			{
				char *pTonerLevel = strstr(pBlackToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,1,消耗品信息下黑色碳粉盒的碳粉水平是:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,2,获取黑色碳粉盒的碳粉水平失败");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,3,获取黑色碳粉盒失败");
			}
			/*获取成像部件下的消耗品水平*/
			char *pImageBlock = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>成像部件</b></p></td><td><p>");
			if (pImageBlock)
			{
				char *pConsumeLevel = strstr(pImageBlock,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szConsumeLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,4,消耗品信息下成像部件的消耗品水平是:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,5,获取成像部件下消耗品水平失败");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,6,获取成像部件失败");
			}
		}
		char *pMediumSufaceCount = strstr(pTableTitle,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>介质已打印面计数</b></p></td><td><p>");
		if (pMediumSufaceCount)
		{
			char *pMediumSufacePrintedCount = strstr(pMediumSufaceCount,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>介质已打印面计数</b></p></td><td><p> ");
			if (pMediumSufacePrintedCount)
			{
				char *pPrintedCount = strstr(pMediumSufacePrintedCount,"<TR><td><p align=\"left\" style=\"margin-left: 40;\"><b>打印</b></p></td><td><p>");
				if (pPrintedCount)
				{
					char *pPrintedTotalCount = strstr(pPrintedCount,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">总计</p></td><td><p>");
					if (pPrintedTotalCount)
					{
						char *pEnd = strstr(pPrintedTotalCount,"</TR>");
						memset(pBuf, 0, dwBufLen);
						strncpy(pBuf,pPrintedTotalCount,(pEnd - pPrintedTotalCount)+nLength);
						CString szNeedInfo;
						szNeedInfo.Format("%s",pBuf);
						szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">总计</p></td><td><p>","");
						szNeedInfo.Replace("</p></td></TR>","");
						szNeedInfo.Replace(" ","");
						m_szConsumeLevel = szNeedInfo;
						m_oMeterInfo.nA4PrintTotalPage = atoi(szNeedInfo);
						theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_XM1145,A4打印总计%s",szNeedInfo);
					}
				}
			}
		}

	}
	return TRUE;
}

/************************************************************************/
/*         通过读取利盟XM1145静态页面的方式获取抄表其它信息 (2)         */
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
		if (szCurrentLine.Find("介质已打印面计数") >= 0)
		{
			CString szNextLine2 = ary.GetAt(i+2);
			if (szNextLine2.Find("打印") >= 0)
			{
				CString szNextLine4 = ary.GetAt(i+4);
				if (szNextLine4.Find("单色") >= 0)
				{
					szNextLine4.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">单色</p></td><td><p>", "");
					szNextLine4.Replace("</p></td>", "");
					szNextLine4.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine4);
				}
			}
			CString szNextLine8 = ary.GetAt(i+8);
			if (szNextLine8.Find("复印") >= 0)
			{
				CString szNextLine10 = ary.GetAt(i+10);
				if (szNextLine10.Find("单色") >= 0)
				{
					szNextLine10.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">单色</p></td><td><p>", "");
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
/*         通过读取利盟X950静态页面的方式获取抄表其它信息               */
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
		char *pMediumSufaceCount = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>介质面计数</b></p></td><td><p>");
		/*获取介质面计数下打印单色面数下的总计*/
		if (pMediumSufaceCount)
		{
			char *pPrintedHBSufaceCount = strstr(pMediumSufaceCount,"<TR><td><p align=\"left\" style=\"margin-left: 40;\"><b>打印的单色面数</b></p></td><td><p>");
			if (pPrintedHBSufaceCount)
			{
				char *pHBTotal = strstr(pPrintedHBSufaceCount,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">总计</p></td><td><p>");
				if (pHBTotal)
				{
					char *pEnd = strstr(pHBTotal,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pHBTotal,(pEnd - pHBTotal)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">总计</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
				}
			}
		}
		char *pScanUseage = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>扫描用法</b></p></td><td><p>");
		if (pScanUseage)
		{	/*获取扫描用法下已扫描页数的网络扫描计数*/
			char *pScanedPage = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>已扫描页数</b></p></td><td><p>");
			if (pScanedPage)
			{
				char *pScanForNet = strstr(pScanedPage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">网络</p></td><td><p>");
				if (pScanForNet)
				{
					char *pEnd = strstr(pScanForNet,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pScanForNet,(pEnd - pScanForNet)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">网络</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_nPrintedNet = atoi(szNeedInfo); 
				}
			}
		}
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>消耗品信息</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*获取消耗品信息下黑色光电棍的消耗品水平*/
			char *pBlackLightElectricStick = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>黑色光电辊</b></p></td><td><p>");
			if (pBlackLightElectricStick)
			{
				char *pConsumeLevel = strstr(pBlackLightElectricStick,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>","");
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
/*          通过读取利盟MX811静态页面的方式获取抄表其它信息              */
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
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>消耗品信息</b></p></td><td><p>");
		if (pConsumeInfo)
		{	/*获取消耗品信息下黑色碳粉盒碳粉水平*/
			char *pBlackToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>黑色碳粉盒</b></p></td><td><p>");
			if (pBlackToner)
			{
				char *pTonerLevel = strstr(pBlackToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811,1,黑色碳粉盒碳粉水平:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811,2,获取消耗品信息下黑色碳粉盒碳粉水平失败");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_MX811,3,获取黑色碳粉盒水平失败");
			}
		}
	}
	
	return TRUE;
}

/************************************************************************/
/*          通过读取利盟MX811静态页面的方式获取抄表其它信息 （2）       */
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
		if (szCurrentLine.Find("介质已打印面计数") >= 0)
		{
			CString szNextLine2 = ary.GetAt(i+2);
			if (szNextLine2.Find("打印") >= 0)
			{
				CString szNextLine4 = ary.GetAt(i+4);
				if (szNextLine4.Find("单色") >= 0)
				{
					szNextLine4.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">单色</p></td><td><p>", "");
					szNextLine4.Replace("</p></td>", "");
					szNextLine4.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine4);
				}
			}
			CString szNextLine8 = ary.GetAt(i+8);
			if (szNextLine8.Find("复印") >= 0)
			{
				CString szNextLine10 = ary.GetAt(i+10);
				if (szNextLine10.Find("单色") >= 0)
				{
					szNextLine10.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">单色</p></td><td><p>", "");
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
/*          通过读取利盟X860de静态页面的方式获取抄表其它信息            */
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
		char *pScanUseage = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>扫描用法</b></p></td><td><p>");
		if (pScanUseage)
		{
			//扫描用法下网络计数
			char *pPrintedNet = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">网络</p></td><td><p>");
			if (pPrintedNet)
			{
				char *pEnd = strstr(pPrintedNet,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pPrintedNet,(pEnd - pPrintedNet)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">网络</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_nPrintedNet = atoi(szNeedInfo);
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,1,获取扫描用法下网路计数是:%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,2,获取扫描用法下网络计数失败");
			}
			//获取扫描用法下USB端口
			char *pUsbPort = strstr(pScanUseage,"<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB 端口</p></td><td><p>");
			if (pUsbPort)
			{
				char *pEnd = strstr(pUsbPort,"</TR>");
				memset(pBuf, 0, dwBufLen);
				strncpy(pBuf,pUsbPort,(pEnd - pUsbPort)+nLength);
				CString szNeedInfo;
				szNeedInfo.Format("%s",pBuf);
				szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 30;\">USB 端口</p></td><td><p>","");
				szNeedInfo.Replace("</p></td></TR>","");
				szNeedInfo.Replace(" ","");
				m_szUsbPort = szNeedInfo;  
				theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,3,获取扫描用法下USB端口是:%s",szNeedInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,4,获取扫描用法下USB端口失败");
			}
		}
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>消耗品信息</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			//获取消耗品信息下黑色碳粉的碳粉水平
			char *pBlcakToner = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>黑色碳粉</b></p></td><td><p>");
			if (pBlcakToner)
			{
				char *pTonerLevel = strstr(pBlcakToner,"<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>");
				if (pTonerLevel)
				{
					char *pEnd = strstr(pTonerLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pTonerLevel,(pEnd - pTonerLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">碳粉水平</p></td><td><p>","");
					szNeedInfo.Replace("</p></td></TR>","");
					szNeedInfo.Replace(" ","");
					m_szTonerLevel = szNeedInfo;
					theLog.Write("CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,5,获取消耗品信息向下黑色碳粉的碳粉水平是:%s",szNeedInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,6,获取消耗品信息向下黑色碳粉的碳粉水平失败");
				}
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetOtherMeterInfoByHtml_X860de,7,获取消耗品信息向下黑色碳粉失败");
			}
		}
	}
	return TRUE;
}

/************************************************************************/
/*          通过读取利盟X860de静态页面的方式获取抄表其它信息  (2)       */
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
		if (szCurrentLine.Find("介质面计数") >= 0)
		{
			CString szNextLine4 = ary.GetAt(i+4);
			if (szNextLine4.Find("打印") >= 0)
			{
				CString szNextLine6 = ary.GetAt(i+6);
				if (szNextLine6.Find("单色") >= 0)
				{
					szNextLine6.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">单色</p></td><td><p>", "");
					szNextLine6.Replace("</p></td>", "");
					szNextLine6.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine6);
				}
			}
			CString szNextLine10 = ary.GetAt(i+10);
			if (szNextLine10.Find("复印") >= 0)
			{
				CString szNextLine12 = ary.GetAt(i+12);
				if (szNextLine12.Find("单色") >= 0)
				{
					szNextLine12.Replace("<TR><td><p align=\"left\" style=\"margin-left: 40;\">单色</p></td><td><p>", "");
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
/*          通过读取利盟MX610de静态页面的方式获取抄表其它信息            */
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
		char *pConsumeInfo = strstr(pTableTitle,"<TABLE><TR><td><p align=\"left\" style=\"margin-left: 20;\"><b>消耗品信息</b></p></td><td><p>");
		if (pConsumeInfo)
		{
			/*获取成像部件下的消耗品水平*/
			char *pImageBlock = strstr(pConsumeInfo,"<TR><td><p align=\"left\" style=\"margin-left: 30;\"><b>成像部件</b></p></td><td><p>");
			if (pImageBlock)
			{
				char *pConsumeLevel = strstr(pImageBlock,"<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>");
				if (pConsumeLevel)
				{
					char *pEnd = strstr(pConsumeLevel,"</TR>");
					memset(pBuf, 0, dwBufLen);
					strncpy(pBuf,pConsumeLevel,(pEnd - pConsumeLevel)+nLength);
					CString szNeedInfo;
					szNeedInfo.Format("%s",pBuf);
					szNeedInfo.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">消耗品水平</p></td><td><p>","");
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
/*          通过读取利盟MX610de静态页面的方式获取抄表其它信息   (2)     */
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
		if (szCurrentLine.Find("介质已打印面计数") >= 0)
		{
			CString szNextLine2 = ary.GetAt(i+2);
			if (szNextLine2.Find("打印") >= 0)
			{
				CString szNextLine4 = ary.GetAt(i+4);
				if (szNextLine4.Find("单色") >= 0)
				{
					szNextLine4.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">单色</p></td><td><p>", "");
					szNextLine4.Replace("</p></td>", "");
					szNextLine4.Trim();
					m_oMeterInfo.nOtherPrintTotalPage = atoi(szNextLine4);
				}
			}
			CString szNextLine8 = ary.GetAt(i+8);
			if (szNextLine8.Find("复印") >= 0)
			{
				CString szNextLine10 = ary.GetAt(i+10);
				if (szNextLine10.Find("单色") >= 0)
				{
					szNextLine10.Replace("<TR><td><p align=\"left\" style=\"margin-left: 50;\">单色</p></td><td><p>", "");
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
/*         利盟web的html分割成字符串链表				                */
/************************************************************************/
list<char *>* CSnmpPrinterMeterMonitorLM::HtmlToList(CString szUrl)
{
	CInternetSession *pInetSession = new CInternetSession; 
	CHttpFile *pHttpFile = NULL ;
	pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //打开一个URL
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
/*         利盟web的html分割成字符串数组				                */
/************************************************************************/
BOOL CSnmpPrinterMeterMonitorLM::HtmlToList(CString szUrl, CStringArray& szAry)
{
	BOOL bRet = FALSE;
	try
	{
		CInternetSession *pInetSession = new CInternetSession; 
		CHttpFile *pHttpFile = NULL ;
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //打开一个URL
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

//获取打印介质面统计信息
BOOL CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo()
{
	int nValue;
	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,获取总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorLM::GetMeterInfo,获取总页数成功，nValue=%d", nValue);
	}

	//特别说明：利盟打印机抄表有【介质页】和【介质面】的区别，一般情况下1页=2面。
	//打印又区分【拾取数】和【实际出纸数】。
	//本函数只统计【介质面】的【实际出纸数】。
	//本函数只统计计数为打印，不区分复印和传真，因为复印和传真最终是打印介质来统计的。

	//1.查找所有打印面的纸型计数索引
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

	//2.根据打印面的纸型计数索引，获取具体的打印计数信息，如纸型、色彩、拾取面数和实际出纸面数。
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

	//3.统计打印实际出纸面数信息，
	for (it=oPaperSidesCount.begin(); it!=oPaperSidesCount.end(); it++)
	{
		PPaperSidesCountEntry pEntry = it->second;
		if (pEntry)
		{
			if (pEntry->paperSidesPaperSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA4)
			{
				m_oMeterInfo.nA4PrintHBPage += pEntry->paperSidesMonoSafe;
				m_oMeterInfo.nA4PrintColorPage += pEntry->paperSidesColorSafe;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,打印A4黑白介质面数=%d，A4彩色介质面数=%d"
				//	, pEntry->paperSidesMonoSafe, pEntry->paperSidesColorSafe);
			}
			else if (pEntry->paperSidesPaperSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA3)
			{
				m_oMeterInfo.nA3PrintHBPage += pEntry->paperSidesMonoSafe;
				m_oMeterInfo.nA3PrintColorPage += pEntry->paperSidesColorSafe;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,打印A3黑白介质面数=%d，A3彩色介质面数=%d"
				//	, pEntry->paperSidesMonoSafe, pEntry->paperSidesColorSafe);
			}
			else
			{
				m_oMeterInfo.nOtherPrintHBPage += pEntry->paperSidesMonoSafe;
				m_oMeterInfo.nOtherPrintColorPage += pEntry->paperSidesColorSafe;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetPrintSidesCountInfo,打印其它纸型(%d)，黑白介质面数=%d，彩色介质面数=%d"
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

//获取扫描统计信息
BOOL CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo()
{
	//特别说明：利盟打印机抄表有【介质页】和【介质面】的区别，一般情况下1页=2面。

	//1.查找所有扫描面的纸型计数索引
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

	//2.根据扫描面的纸型计数索引，获取具体的扫描计数信息，如类型、纸型、面数和页数。
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

	//3.统计扫描面数信息，
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
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,扫描类型(%d),扫描A4介质面数=%d"
				//	, pEntry->scanCountType, pEntry->scanCountSides);
			}
			else if (pEntry->scanCountSize == ENUM_STRUCT_VALUE(PaperSizeTC)::isoA3)
			{
				m_oMeterInfo.nScanBigColorPage += pEntry->scanCountSides;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,扫描类型(%d),扫描A3介质面数=%d"
				//	, pEntry->scanCountType, pEntry->scanCountSides);
			}
			else
			{
				m_oMeterInfo.nScanSmallColorPage += pEntry->scanCountSides;
				//theLog.Write("CSnmpPrinterMeterMonitorLM::GetScanSidesCountInfo,扫描类型(%d),扫描纸型(%d),扫描其它介质面数=%d"
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
