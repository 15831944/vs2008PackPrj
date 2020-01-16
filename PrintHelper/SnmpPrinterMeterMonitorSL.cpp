/*************2016-06-27 �޸�********************
	����A3�ڰ���ʹ�����ĳ��� 
ע�⣺ ��ҳ�� ������ A3�ڰ� + A3��ɫ + A4�ڰ� + A4��ɫ
	��Ϊ A4�ڰ׺�A4��ɫ�����Ѿ�������A3�Ļ���
*************************************************/


#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorSL.h"
#include "XeroxMIB_Def.h"
#define DEF_HTTP	"http://"
#define DEF_COUNT_ApeosPort_V_C3373		"/prcnt.htm"

CSnmpPrinterMeterMonitorSL::CSnmpPrinterMeterMonitorSL(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorSL::~CSnmpPrinterMeterMonitorSL(void)
{
}

void CSnmpPrinterMeterMonitorSL::InitOID()
{
	m_szXeroxA4PrintHBPageOID = DecryptOID(XeroxA4PrintHBPageOID);
	m_szXeroxA4PrintColorPageOID = DecryptOID(XeroxA4PrintColorPageOID);
	m_szXeroxA4CopyHBPageOID = DecryptOID(XeroxA4CopyHBPageOID);
	m_szXeroxA4CopyColorPageOID = DecryptOID(XeroxA4CopyColorPageOID);
	m_szXeroxScannedImagesStoredOID = DecryptOID(XeroxScannedImagesStoredOID);
	m_szXeroxNetworkScanningImagesSentOID = DecryptOID(XeroxNetworkScanningImagesSentOID);
	m_szXeroxA4FaxTotalPageOID = DecryptOID(XeroxA4FaxTotalPageOID);
	m_szXeroxA4ColorTotalPageOID = DecryptOID(XeroxA4ColorTotalPageOID);
	m_szXeroxA4HBTotalPageOID = DecryptOID(XeroxA4HBTotalPageOID);
	m_szXeroxA3ColorTotalPageOID = DecryptOID(XeroxA3ColorTotalPageOID);
	m_szXeroxA3HBTotalPageOID = DecryptOID(XeroxA3HBTotalPageOID);
	m_szSLPrinterModelOID = DecryptOID(XeroxPrintModel);
	m_szXeroxA3HBTotalPageOID_DocuCentre_S2110 = DecryptOID(XeroxA3HBTotalPageOIDDocuCentre_S2110);
	m_szSLPrintModel = "";
}
CString CSnmpPrinterMeterMonitorSL::GetSLModel()
{
	CString szModel;
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szSLPrinterModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetSLModel,��ȡ�ͺ�ʧ��");
	}
	szModel.Format("%s",tmp);
	theLog.Write("CSnmpPrinterMeterMonitorSL::GetSLModel,szModel=%s",szModel);
	szModel.Trim();
	return szModel;

}

BOOL CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373()
{
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTP,m_szIP,DEF_COUNT_ApeosPort_V_C3373);
	theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,0,begin,szUrl=%s",szUrl);
	CString szWebContent = GetMeterByUrl(szUrl);
	szWebContent = CCommonFun::MultiByteToUTF8(szWebContent.GetString());
	//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,1,szWebContent=%s",szWebContent);
	CString szA4PrintTotal = GetDetailMeterInfoByFlag("Total Printed Impressions",szWebContent);
	CString szA4PrintHB = GetDetailMeterInfoByFlag("Black Printed Impressions",szWebContent);
	CString szA4PrintColor = GetDetailMeterInfoByFlag("Color Printed Impressions",szWebContent);
	//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,1,szA4PrintHB=%s",szA4PrintHB);
	CString szA4CopyTotal = GetDetailMeterInfoByFlag("Total Copied Impressions",szWebContent);
	CString szA4CopyColor = GetDetailMeterInfoByFlag("Color Copied Impressions",szWebContent);
	CString szA4CopyHB = GetDetailMeterInfoByFlag("Black Copied Impressions",szWebContent);
	CString szA4ScanTotal = GetDetailMeterInfoByFlag("Total Scanned Images",szWebContent);
	CString szA4ScanColor = GetDetailMeterInfoByFlag("Color Scanned Images",szWebContent);
	CString szA4ScanHB = GetDetailMeterInfoByFlag("Black Scanned Images",szWebContent);
	theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373"
		",szA4PrintTotal=%s,szA4PrintHB=%s,szA4PrintColor=%s,szA4CopyTotal=%s"
		",szA4CopyColor=%s,szA4CopyHB=%s,szA4ScanTotal=%s"
		,szA4PrintTotal,szA4PrintHB,szA4PrintColor,szA4CopyTotal,szA4CopyColor,szA4CopyHB,szA4ScanTotal);
	int nValue = 0;
	//��ӡ
	if (szA4PrintTotal.GetLength() <= 0)//a4 ��ӡ
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4��ӡ��ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4PrintTotal.GetString());
		m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4PrintHB.GetLength() <= 0)//a4 �ڰ״�ӡ
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4��ӡ�ڰ���ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4PrintHB.GetString());
		m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4PrintColor.GetLength() <= 0)//a4 ��ɫ��ӡ
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4��ӡ��ɫ��ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4PrintColor.GetString());
		m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
	}
	//��ӡ
	if (szA4CopyTotal.GetLength() <= 0)//a4 ��ӡ
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4��ӡ��ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4PrintTotal.GetString());
		m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4CopyHB.GetLength() <= 0)//a4 �ڰ׸�ӡ
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4��ӡ�ڰ���ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4CopyHB.GetString());
		m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
	}
	nValue = 0;
	if (szA4CopyColor.GetLength() <= 0)//a4 ��ɫ��ӡ
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4��ӡ��ɫ��ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4CopyColor.GetString());
		m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
	}
// ɨ��
	nValue = 0;
	if (szA4ScanTotal.GetLength() <= 0)//a4 ɨ��
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo_ApeosPort_V_C3373,��ȡA4ɨ����ҳ��ʧ��");
	}
	else
	{
		nValue = atoi(szA4ScanTotal.GetString());
		m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
	}
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage + m_oMeterInfo.nScanTotalPage;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
	m_oMeterInfo.nAllTotalPage = m_oMeterInfo.nA4HBTotalPage + m_oMeterInfo.nA4ColorTotalPage;



	return TRUE;
}
CString CSnmpPrinterMeterMonitorSL::GetDetailMeterInfoByFlag(CString szFlag,CString szWebContent)
{
	CString szTmp = "";
	CString szFlagEx = szFlag + "',";
	int indx1 = szWebContent.Find(szFlagEx);
	if (indx1 >= 0)
	{
		szTmp = szWebContent.Right(szWebContent.GetLength()-indx1-szFlagEx.GetLength());
		szTmp.Trim();
		int nIdex2 = szTmp.Find(",");
		if (nIdex2 >= 0)
		{
			szTmp = szTmp.Left(nIdex2);
		}
	}
	return szTmp;
}
//ֽ�Ų�����ȷָʾΪֽ��ʱ��Ĭ�ϼ���ΪA4
//��ӡ����ӡ�й��ڼ�����A3=2*A4
BOOL CSnmpPrinterMeterMonitorSL::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	m_szSLPrintModel = GetSLModel();
	if(m_szSLPrintModel.Find("ApeosPort-V C3373") >= 0)
	{
		return GetMeterInfo_ApeosPort_V_C3373();		
	}
	else
	{
		int nValue = 0;

		if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,��ȡ��ҳ��ʧ��");
			return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4PrintHBPageOID), nValue))	//Black Printed Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.7"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,2,��ȡ�ڰ״�ӡ����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,3,��ȡ�ڰ״�ӡ����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4PrintColorPageOID), nValue))	//Color Printed Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.29"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,��ȡ��ɫ��ӡ����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,��ȡ��ɫ��ӡ����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4CopyHBPageOID), nValue))	//Black Copied Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.103.20.3"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,6,��ȡ�ڰ׸�ӡ����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,7,��ȡ�ڰ׸�ӡ����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4CopyColorPageOID), nValue))	//Color Copied Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.103.20.25"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,8,��ȡ��ɫ��ӡ����ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,9,��ȡ��ɫ��ӡ����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxScannedImagesStoredOID), nValue))	//Scanned Images Stored=".1.3.6.1.4.1.253.8.53.13.2.1.6.102.20.21"
		{
			if (!GetRequest(CStringToChar(m_szXeroxNetworkScanningImagesSentOID), nValue))	//Network Scanning Images Sent=".1.3.6.1.4.1.253.8.53.13.2.1.6.102.20.11"
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,10,��ȡɨ�����ҳ��ʧ��");
			}
			else
			{
				m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
				//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,11,��ȡɨ�����ҳ���ɹ���nValue=%d", nValue);
			}
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,12,��ȡɨ�����ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4FaxTotalPageOID), nValue))	//Fax Impressions=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.71"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,13,��ȡ�������ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4FaxTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,14,��ȡ�������ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4ColorTotalPageOID), nValue))	//��ɫ����=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.33"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,��ȡ�ڰ�A4����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4ColorTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,��ȡ�ڰ�A4�����ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA4HBTotalPageOID), nValue))	//�ڰ�����=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.34"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,��ȡ�ڰ�����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4HBTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,��ȡ�ڰ������ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szXeroxA3ColorTotalPageOID), nValue))	//��ɫA3����=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.43"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,4,��ȡ��ɫA3����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA3ColorTotalPage = (nValue>0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,5,��ȡ��ɫA3�����ɹ���nValue=%d", nValue);
		}
		if (m_szSLPrintModel.Find("DocuCentre S2110") >=0 )
		{
			if (!GetRequest(CStringToChar(m_szXeroxA3HBTotalPageOID_DocuCentre_S2110), nValue))	//�ڰ�A3����=".1.3.6.1.4.1.253.8.53.13.2.1.6.101.20.5"
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,DocuCentre_S2110,��ȡ�ڰ�A3����ʧ��");
			}
			else
			{
				m_oMeterInfo.nA3HBTotalPage = (nValue>0) ? nValue : 0;
				//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,7,��ȡ�ڰ�A3�����ɹ���nValue=%d", nValue);
			}
		}
		else
		{
			if (!GetRequest(CStringToChar(m_szXeroxA3HBTotalPageOID), nValue))	//�ڰ�A3����=".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.44"
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorSL::GetMeterInfo,6,��ȡ�ڰ�A3����ʧ��");
			}
			else
			{
				m_oMeterInfo.nA3HBTotalPage = (nValue>0) ? nValue : 0;
				//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,7,��ȡ�ڰ�A3�����ɹ���nValue=%d", nValue);
			}
		}
		

		int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage;
		if(other > 0)
		{
			m_oMeterInfo.nOtherOpHBPage += other;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		//theLog.Write("CSnmpPrinterMeterMonitorSL::GetMeterInfo,100,end");
		return TRUE;
	}
}
CString CSnmpPrinterMeterMonitorSL::GetMeterByUrl(CString szUrl)
{
	CString szinfo="";
	CInternetSession *pInetSession = NULL;
	try
	{
		pInetSession = new CInternetSession; 
		CHttpFile *pHttpFile = NULL ;
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //��һ��URL
		if (!pHttpFile)
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorHP::HtmlToList,1,fail");
			return szinfo;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorHP::HtmlToList,2,pHttpFile=0x%x", pHttpFile);
		//pHttpFile ȷʵ�Ƿ�Ҫɾ��
		CString str ;

		while(pHttpFile->ReadString(str))   
		{
			szinfo += CCommonFun::UTF8ToMultiByte((const unsigned char*)str.GetString());
		}
		delete pHttpFile;
		pHttpFile = NULL;
		delete pInetSession;
		pInetSession  = NULL;
	}
	catch(...)
	{
		theLog.Write("!!GetMeterByUrl catch exception.ERROR=%d", GetLastError());
		delete pInetSession;
		pInetSession  = NULL;
	}
	return szinfo;
}