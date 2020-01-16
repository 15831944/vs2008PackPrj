#include "StdAfx.h"
#include "GetContenByUrl.h"

CGetContenByUrl::CGetContenByUrl(void)
{
}

CGetContenByUrl::~CGetContenByUrl(void)
{
}
BOOL CGetContenByUrl::getHttpContentByUrl(CString szUrl,CString& szMsgBack)
{
	BOOL bRet = TRUE;
	CString szinfo="";
	CInternetSession *pInetSession = NULL;
	CHttpFile *pHttpFile = NULL ;
	do 
	{
		try
		{
			pInetSession = new CInternetSession; 
			
			pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //打开一个URL
			if (!pHttpFile)
			{
				bRet = FALSE;
				break;
				theLog.Write("!!CGetContenByUrl::getHttpContentByUrl,1,fail");
			}

			//theLog.Write("CSnmpPrinterMeterMonitorHP::HtmlToList,2,pHttpFile=0x%x", pHttpFile);
			//pHttpFile 确实是否要删除
			CString str ;
			while(pHttpFile->ReadString(str))   
			{
				szinfo += CCommonFun::UTF8ToMultiByte((const unsigned char*)str.GetString());
			}
		}
		catch(...)
		{
			bRet = FALSE;
			break;
			theLog.Write("!!CGetContenByUrl::getHttpContentByUrl,2,catch exception.ERROR=%d", GetLastError());
		}
	} while (FALSE);
	delete pHttpFile;
	pHttpFile = NULL;
	delete pInetSession;
	pInetSession  = NULL;
	szMsgBack = szinfo;
	return bRet;
}
BOOL CGetContenByUrl::getHttpsContentByUrl(CString szUrl,CString& szMsgBack)
{
	DWORD dwFlags;  
	DWORD dwStatus = 0;  
	DWORD dwStatusLen = sizeof(dwStatus);  
	CString strLine;  
	DWORD m_dwServiceType;
	CString m_strServer;
	CString m_strObject_URI;
	INTERNET_PORT  m_nServerPort = 0;
	CString m_strHttpVersion;
	CString m_strHtml;

	AfxParseURL(szUrl, m_dwServiceType, m_strServer, m_strObject_URI, m_nServerPort);  
	CInternetSession *m_pSess = new CInternetSession; 
	CHttpFile *m_pHttpFile = NULL ;
	CHttpConnection *m_pHttpConn= NULL;
	BOOL bRet = TRUE;
	do 
	{
		try {  
			m_pHttpConn = m_pSess->GetHttpConnection(m_strServer, INTERNET_FLAG_SECURE, m_nServerPort,  
				NULL, NULL);  
			if(m_pHttpConn)
			{  
				m_pHttpFile = (CHttpFile*)m_pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, m_strObject_URI, NULL, 1,  //HTTP_VERB_POST
					NULL, m_strHttpVersion,  
					INTERNET_FLAG_SECURE |  
					INTERNET_FLAG_EXISTING_CONNECT |  
					INTERNET_FLAG_RELOAD |  
					INTERNET_FLAG_NO_CACHE_WRITE |  
					INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  
					INTERNET_FLAG_IGNORE_CERT_CN_INVALID  
					);  
				//get web server option  
				BOOL BRet = m_pHttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
				if(!BRet)
				{
					bRet = FALSE;
					break;
					theLog.Write("!!CGetContenByUrl::getHttpsContentByUrl,1,QueryOption fail,err=%d", GetLastError());
				}
				//dwFlags |= SECURITY_FLAG_IGNORE_WRONG_USAGE;//SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
				//			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;//SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
				dwFlags |= SECURITY_IGNORE_ERROR_MASK;	//SECURITY_FLAG_IGNORE_UNKNOWN_CA;
				//set web server option  
				BRet = m_pHttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
				if(!BRet)
				{
					bRet = FALSE;
					break;
					theLog.Write("!!CGetContenByUrl::getHttpsContentByUrl,2,QueryOption fail,err=%d", GetLastError());
				}
				m_pHttpFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
				m_pHttpFile->AddRequestHeaders(_T("Accept: */*"));
				if(m_pHttpFile->SendRequest()) 
				{
					//get response status if success, return 200  
					int nRet = m_pHttpFile->QueryInfo(HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE,  
						&dwStatus, &dwStatusLen, 0); 
					//theLog.Write("CSnmpPrinterMeterMonitorHP::SetOption,13,QueryOption nRet=%d",nRet);
					while(m_pHttpFile->ReadString(strLine))
					{  
						m_strHtml += strLine + char(13) + char(10);  
					}
				} 
				else 
				{  
					theLog.Write("!!CGetContenByUrl::getHttpsContentByUrl,3,send request failed");

				}  
			} 
			else 
			{  
				theLog.Write("!!CGetContenByUrl::getHttpsContentByUrl,4,send request failed");
			}  
		} 
		catch(CInternetException *e) 
		{  
			bRet = FALSE;
			e->ReportError();
			theLog.Write("!!CGetContenByUrl::getHttpsContentByUrl,5,failed");
			break;
			  
		}  
	} while (FALSE);
	szMsgBack = CCommonFun::UTF8ToMultiByte((const unsigned char*)m_strHtml.GetString());
	m_pSess->Close(); 
	delete m_pSess;
	m_pSess = NULL;
	m_pHttpFile->Close();
	delete m_pHttpFile;
	m_pHttpFile = NULL;
	return bRet;
}
