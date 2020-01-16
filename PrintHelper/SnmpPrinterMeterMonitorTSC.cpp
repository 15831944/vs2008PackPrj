#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorTSC.h"
#include "TSCMIB_Def.h"

CSnmpPrinterMeterMonitorTSC::CSnmpPrinterMeterMonitorTSC(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorTSC::~CSnmpPrinterMeterMonitorTSC(void)
{
}

void CSnmpPrinterMeterMonitorTSC::InitOID()
{
	m_szModelOID = DecryptOID(TSC_Model_OID);
}

BOOL CSnmpPrinterMeterMonitorTSC::GetMeterInfo()
{
	CString szModel = GetModel();
	//theLog.Write("==============szModel=%s",szModel);
	if (szModel.Find("T-4502E") >= 0)
	{
		return GetMeterInfo_T_4502E();
	}
	return TRUE;
}

BOOL CSnmpPrinterMeterMonitorTSC::GetMeterInfo_T_4502E()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();
	int nValue = 0;

	CString szUrl;
	szUrl.Format("http://%s/title.asp", GetPrtIP());
	CString szMeter = GetMeterByUrl(szUrl);

	//theLog.Write("=========szMeter=%s",szMeter);
	CString szPrintLen = GetPrintLen(szMeter);
	//theLog.Write("CSnmpPrinterMeterMonitorTSC::GetMeterInfo_T_4502E,szPrintLen=%s",szPrintLen);
	double fLen = atof(szPrintLen);
	int nLen = fLen * 1000.0;
	//theLog.Write("CSnmpPrinterMeterMonitorTSC::GetMeterInfo_T_4502E,nLen=%d",nLen);
	m_oMeterInfo.nPrintDistance = nLen;
	m_oMeterInfo.nAllTotalDistance = nLen;
	m_oMeterInfo.nAllTotalPage = nLen;
	return TRUE;
}

CString CSnmpPrinterMeterMonitorTSC::GetModel()
{
	CString szModel;
	unsigned char tmp[200];
	//theLog.Write("================CSnmpPrinterMeterMonitorTSC::GetModel,m_szModelOID=%s",m_szModelOID);
	if (!GetRequest(CStringToChar(m_szModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorJC::GetModel,获取型号失败");
	}
	szModel.Format("%s",tmp);
	theLog.Write("CSnmpPrinterMeterMonitorTSC::GetModel,szModel=%s",szModel);
	szModel.Trim();
	return szModel;
}

CString CSnmpPrinterMeterMonitorTSC::GetPrintLen(CString szHtml)
{
	CString szPrintLen;
	BOOL bRet = FALSE;
	do 
	{
		CString szTmp = "Milage :</TD><TD>";
		int nIndex = szHtml.Find(szTmp);
		//theLog.Write("CSnmpPrinterMeterMonitorTSC::GetPrintLen,nIndex=%d",nIndex);
		if (nIndex >= 0)
		{
			szPrintLen = szHtml.Right(szHtml.GetLength()-szTmp.GetLength()-nIndex);
			szTmp = "Km</TD>";
			int nIndex2 = szPrintLen.Find(szTmp);
			//theLog.Write("CSnmpPrinterMeterMonitorTSC::GetPrintLen,nIndex2=%d",nIndex2);
			if (nIndex2 >= 0)
			{
				szPrintLen = szPrintLen.Left(nIndex2);
				bRet = TRUE;
			}
		}
	} while (FALSE);
	if (!bRet)
	{
		szPrintLen = "";
	}

	return szPrintLen;
}

CString CSnmpPrinterMeterMonitorTSC::GetMeterByUrl(CString szUrl)
{
	CString szinfo="";
	CInternetSession *pInetSession = NULL;
	try
	{
		pInetSession = new CInternetSession; 
		CHttpFile *pHttpFile = NULL ;
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //打开一个URL
		if (!pHttpFile)
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorTSC::HtmlToList,1,fail");
			return szinfo;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorHP::HtmlToList,2,pHttpFile=0x%x", pHttpFile);
		//pHttpFile 确实是否要删除
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


