#include "StdAfx.h"
#include "httpclient.h"  
#include "curl/curl.h"
#include <io.h>

char g_szLogDir[MAX_PATH] = {0};

using namespace CurlHttp;

//获取日志目录
char* GetLogDir()
{
	if (strlen(g_szLogDir) <= 0)
	{
		OutputDebugString(_T("##GetLogDir"));
		TCHAR path[MAX_PATH];
		if(GetModuleFileName(0,path,MAX_PATH))
		{
			TCHAR *pdest;
			pdest=strrchr(path, '\\');
			if(pdest) 
				*pdest=0;
			strcpy_s(g_szLogDir, path);
			strcat_s(g_szLogDir, "\\xlog");
		}

		if (strlen(g_szLogDir) <= 0)
		{
			strcpy_s(g_szLogDir, ".\\xlog");
		}

		OutputDebugString(g_szLogDir);
	}

	return g_szLogDir;
}


void WriteFileLog(char* szFormat, ...)
{
	char LogText[1024 + 1];
	memset(LogText, 0, sizeof(LogText));
	va_list argList;
	va_start(argList, szFormat);
	int offset =_vsnprintf_s(LogText,sizeof(LogText)-1,szFormat,argList);
	LogText[offset]=0;
	va_end(argList);

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	char cfile[260] = {0};
	sprintf(cfile, "%s\\CHttpClient_%02d%02d%02d.Log", GetLogDir(), timeinfo->tm_year-100, timeinfo->tm_mon+1, timeinfo->tm_mday);
	FILE *pFile = fopen(cfile, "a+");
	if (pFile)
	{
		fprintf(pFile, "%04d-%02d-%02d %02d:%02d:%02d,\t%s\n"
			, (timeinfo->tm_year+1900), timeinfo->tm_mon+1, timeinfo->tm_mday
			, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, LogText);
		fclose (pFile);
	}
}
  
CHttpClient::CHttpClient(void)
{  
	m_bDebug = false;
	m_nCnntTimeOutSeconds = 15;	//连接超时时间 add by zfq,2019-09-03
	m_nTimeOutSeconds = 30;		//操作超时时间 add by zfq,2019-09-03
}  
  
CHttpClient::~CHttpClient(void)  
{  
} 

void CHttpClient::SetTimeOut(int nCnntTimeOutSeconds, int nTimeOutSeconds)
{
	m_nCnntTimeOutSeconds = nCnntTimeOutSeconds;	//连接超时时间 add by zfq,2019-09-03
	m_nTimeOutSeconds = nTimeOutSeconds;		//操作超时时间 add by zfq,2019-09-03
}

void CHttpClient::GetTimeOut(int &nCnntTimeOutSeconds, int &nTimeOutSeconds)
{
	nCnntTimeOutSeconds = m_nCnntTimeOutSeconds;
	nTimeOutSeconds = m_nTimeOutSeconds;
	return;
}
  
static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)  
{  
    if(itype == CURLINFO_TEXT)  
    {  
        printf("[TEXT]%s\n", pData);
		WriteFileLog("[TEXT]%s",pData);
    }  
    else if(itype == CURLINFO_HEADER_IN)  
    {  
        printf("[HEADER_IN]%s\n", pData);
		WriteFileLog("[HEADER_IN]%s",pData);
    }  
    else if(itype == CURLINFO_HEADER_OUT)  
    {  
        printf("[HEADER_OUT]%s\n", pData); 
		WriteFileLog("[HEADER_OUT]%s",pData);
    }  
    else if(itype == CURLINFO_DATA_IN)  
    {  
        printf("[DATA_IN]%s\n", pData); 
		WriteFileLog("[DATA_IN]%s",pData);
    }  
    else if(itype == CURLINFO_DATA_OUT)  
    {  
        printf("[DATA_OUT]%s\n", pData); 
		WriteFileLog("[DATA_OUT]%s",pData);
    }  
    return 0;  
}  
  
static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);  
    if( NULL == str || NULL == buffer )  
    {  
        return -1;  
    }  
  
    char* pData = (char*)buffer;  
    str->append(pData, size * nmemb);
	WriteFileLog("=======pData=%s",pData);
    return nmemb;  
}

static size_t OnFileWriteFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
} 

static int OnProgressFunc(char *progress_data,
					 double t, /* dltotal */
					 double d, /* dlnow */
					 double ultotal,
					 double ulnow)
{
	//printf("%s %g / %g (%g %%)\n", progress_data, d, t, d*100.0/t);
	return 0;
}

  
int CHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)  
{ 
    CURLcode res; 
	long http_code = 0;
	CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_nCnntTimeOutSeconds);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_nTimeOutSeconds);  
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);
    res = curl_easy_perform(curl);
	if (res == 0)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
		if (http_code != 200)
		{
			printf("!!CHttpClient::Post,http_code=%d\n",http_code);
		}
	}
    curl_easy_cleanup(curl);  
    return res;  
}

int CHttpClient::Post(const std::string & strUrl, std::list<std::string>& strHeaders, const std::string & strPost, std::string & strResponse)
{
	CURLcode res;
	long http_code = 0;
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	struct curl_slist* headers = NULL;
	if (strHeaders.size() == 0)
	{
		headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	}
	else
	{
		std::list<std::string>::iterator it = strHeaders.begin();
		for (;it != strHeaders.end();it++)
		{
			std::string strHeader = *it;
			headers = curl_slist_append(headers, strHeader.c_str());
		}

	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	if (m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_nCnntTimeOutSeconds);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_nTimeOutSeconds);  
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);
	res = curl_easy_perform(curl);
	if (res == 0)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
		if (http_code != 200)
		{
			printf("!!CHttpClient::Post,http_code=%d\n",http_code);
			WriteFileLog("!!CHttpClient::Post,http_code=%d",http_code);
		}
	}
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	return res;
}
  
int CHttpClient::Get(const std::string & strUrl, std::string & strResponse)  
{  
    CURLcode res; 
	long http_code = 0;
	CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
	//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt"); 
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse); 
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);
    /** 
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。 
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。 
    */  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_nCnntTimeOutSeconds);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_nTimeOutSeconds); 
	//curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt"); 
    res = curl_easy_perform(curl);
	if (res == 0)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
		if (http_code != 200)
		{
			printf("!!CHttpClient::Get,http_code=%d\n",http_code);
			WriteFileLog("!!CHttpClient::Get,http_code=%d",http_code);
		}
	}
	curl_easy_cleanup(curl);  
    return res;  
}  
  
int CHttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)  
{
    CURLcode res;
	long http_code = 0;
	CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);
    if(NULL == pCaPath)  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
    }  
    else  
    {  
        //缺省情况就是PEM，所以无需设置，另外支持DER  
        //curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
    }  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_nCnntTimeOutSeconds);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_nTimeOutSeconds); 
    res = curl_easy_perform(curl);
	if (res == 0)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
		if (http_code != 200)
		{
			printf("!!CHttpClient::Posts,http_code=%d\n",http_code);
			WriteFileLog("!!CHttpClient::Posts,http_code=%d",http_code);
		}
	}
	curl_easy_cleanup(curl);  
    return res;  
}

int CHttpClient::Posts(const std::string & strUrl, std::list<std::string>& strHeaders, const std::string & strPost, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	long http_code = 0;
	CURL* curl = curl_easy_init();  
	if(NULL == curl)  
	{  
		return CURLE_FAILED_INIT;  
	}

	struct curl_slist* headers = NULL;
	if (strHeaders.size() == 0)
	{
		headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	}
	else
	{
		std::list<std::string>::iterator it = strHeaders.begin();
		for (;it != strHeaders.end();it++)
		{
			std::string strHeader = *it;
			headers = curl_slist_append(headers, strHeader.c_str());
		}

	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	if(m_bDebug)  
	{  
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
	}  
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
	curl_easy_setopt(curl, CURLOPT_POST, 1);  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);
	if(NULL == pCaPath)  
	{  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
	}  
	else  
	{  
		//缺省情况就是PEM，所以无需设置，另外支持DER  
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
	}  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_nCnntTimeOutSeconds);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_nTimeOutSeconds);  
	res = curl_easy_perform(curl);
	if (res == 0)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
		if (http_code != 200)
		{
			printf("!!CHttpClient::Posts,http_code=%d\n",http_code);
			WriteFileLog("!!CHttpClient::Posts,http_code=%d",http_code);
		}
	}
	curl_easy_cleanup(curl);  
	return res;  
}
  
int CHttpClient::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)  
{ 
    CURLcode res; 
	long http_code = 0;
	CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);

    if(NULL == pCaPath)  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
    }  
    else  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
    } 
	
	//curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, "SSLv1");
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_nCnntTimeOutSeconds);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_nTimeOutSeconds);   
    res = curl_easy_perform(curl);
	if (res == 0)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
		if (http_code != 200)
		{
			printf("!!CHttpClient::Gets,http_code=%d\n",http_code);
			WriteFileLog("!!CHttpClient::Gets,http_code=%d",http_code);
		}
	}
	//WriteFileLog("======strResponse=%s",strResponse.c_str());
	curl_easy_cleanup(curl); 
    return res;  
}

int CHttpClient::HttpDownLoadeFile(const std::string& sUrl,const std::string sFilePath)
{
	if (!_access(sFilePath.c_str(),0))
	{
		printf("##CHttpClient::HttpDownLoadeFile,file exit,sFilePath=%s\n",sFilePath.c_str());
		WriteFileLog("##CHttpClient::HttpDownLoadeFile,file exit,sFilePath=%s",sFilePath.c_str());
		return 0;
	}
	CURL *curl;
	CURLcode res;
	FILE *outfile = NULL;
	
	char *progress_data = "progress ";

	curl = curl_easy_init();
	if(curl)
	{
		outfile = fopen(sFilePath.c_str(), "wb");

		if(m_bDebug)  
		{  
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
		}  
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		if (outfile)
		{
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
		}
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,OnFileWriteFunc);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgressFunc);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);

		res = curl_easy_perform(curl);

		fclose(outfile);
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return res;
}
  
///////////////////////////////////////////////////////////////////////////////////////////////  
  
void CHttpClient::SetDebug(bool bDebug)  
{  
    m_bDebug = bDebug;  
}
