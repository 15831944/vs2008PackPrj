#pragma once

#include <string>  
#include <list>

namespace CurlHttp
{
	class CHttpClient  
	{  
	public:  
		CHttpClient(void);  
		~CHttpClient(void);  

	public:  
		/** 
		* @brief HTTP POST请求 
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&… 
		* @param strResponse 输出参数,返回的内容 
		* @return 返回是否Post成功 
		*/  
		int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse); 


		/** 
		* @brief HTTP POST请求 
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
		* @param strHeaders post方法的头
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&… 
		* @param strResponse 输出参数,返回的内容 
		* @return 返回是否Post成功 
		*/  
		int Post(const std::string & strUrl,std::list<std::string>& strHeaders, const std::string & strPost, std::string & strResponse);

		/** 
		* @brief HTTP GET请求 
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
		* @param strResponse 输出参数,返回的内容 
		* @return 返回是否Post成功 
		*/  
		int Get(const std::string & strUrl, std::string & strResponse);  

		/** 
		* @brief HTTPS POST请求,无证书版本 
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com 
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&… 
		* @param strResponse 输出参数,返回的内容 
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性. 
		* @return 返回是否Post成功 
		*/  
		int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);  



		/** 
		* @brief HTTPS POST请求,无证书版本 
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com 
		* @param strHeaders post方法的头
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&… 
		* @param strResponse 输出参数,返回的内容 
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性. 
		* @return 返回是否Post成功 
		*/  
		int Posts(const std::string & strUrl, std::list<std::string>& strHeaders, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);  

		/** 
		* @brief HTTPS GET请求,无证书版本 
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com 
		* @param strResponse 输出参数,返回的内容 
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性. 
		* @return 返回是否Post成功 
		*/  
		int Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL); 	

		int HttpDownLoadeFile(const std::string& sUrl,const std::string sFilePath);


		void SetTimeOut(int nCnntTimeOutSeconds = 8, int nTimeOutSeconds = 15);
		void GetTimeOut(int &nCnntTimeOutSeconds, int &nTimeOutSeconds);




	public:  
		void SetDebug(bool bDebug);
	protected:
		int m_nCnntTimeOutSeconds;	//连接超时时间
		int m_nTimeOutSeconds;		//操作超时时间

	private:  
		bool m_bDebug;
	};
}
  
