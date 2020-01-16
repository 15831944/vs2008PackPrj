#include "StdAfx.h"
#include "MailSender.h"
#include <sstream>
#include <algorithm>

/************************************************************************/
/*	特别说明
	1.一般只能发送不能大于50M的邮件内容，包括主题，正文和附件。
	2.因为base64编码在多平台下兼容性更好好，所以实际最大不能超过37.5M。
*/
/************************************************************************/

using namespace std;

#include <curl/curl.h>
#include <mimetic/mimetic.h>
using namespace mimetic;

CMailSender::CMailSender(void)
{
	m_strCharset = "GB2312";
	m_strServer.clear();
	m_nPort = 0;
	m_bUseSSL = false;
	m_strUser.clear();
	m_strPassword.clear();

	m_strSubject.clear();
	m_strContent.clear();
	m_strSenderName.clear();
	m_strSenderMail.clear();
	m_oListRecipient.clear();
	m_oListCarbonCopy.clear();
	m_oListAttachment.clear();
	m_strSendData.clear();

	m_nSentDataSize = 0;
	m_nTotalDataSize = 0;
	m_bDebug = false;
}

CMailSender::~CMailSender(void)
{
}

void CMailSender::SetCharset(const char* pCharset)
{
	m_strCharset = pCharset;
}

void CMailSender::SetServer(const char* pcServer, int nPort, const char* pcUser, const char* pcPassword, bool bUseSSL)
{
	m_strServer = pcServer;
	m_nPort = nPort;
	m_strUser = pcUser;
	m_strPassword = pcPassword;
	m_bUseSSL = bUseSSL;
}

void CMailSender::SetSender(const char* pcFrom, const char* pSenderName /*= NULL*/)
{
	m_strSenderMail = pcFrom;
	m_strSenderName = "";
	if (pSenderName)
	{
		m_strSenderName = "=?";  
		m_strSenderName += m_strCharset;  
		m_strSenderName += "?B?";  
		m_strSenderName += Base64Encode((unsigned char *)pSenderName, strlen(pSenderName));
		m_strSenderName += "?=";  
	}
}

void CMailSender::AddRecipient(const char* pcTo)
{
	string strRecipient(pcTo);
	m_oListRecipient.push_back(strRecipient);
}

void CMailSender::ClearRecipient()
{
	m_oListRecipient.clear();
}

void CMailSender::AddCarbonCopy(const char* pcCC)
{
	string strCarbonCopy(pcCC);
	m_oListCarbonCopy.push_back(strCarbonCopy);
}

void CMailSender::ClearCarbonCopy()
{
	m_oListCarbonCopy.clear();
}

void CMailSender::SetSubject(const char* pcSubject)
{
	m_strSubject = "=?";  
	m_strSubject += m_strCharset;  
	m_strSubject += "?B?";  
	m_strSubject += Base64Encode((unsigned char *)pcSubject, strlen(pcSubject));  
	m_strSubject += "?=";  
}

void CMailSender::SetContent(const char* pcContent)
{
 	m_strContent = Base64Encode((unsigned char *)pcContent, strlen(pcContent));  
}

void CMailSender::AddAttachment(const char* pcAttachment)
{
	string strAttachment(pcAttachment);
	m_oListAttachment.push_back(strAttachment);
}

void CMailSender::ClearAttachment()
{
	m_oListAttachment.clear();
}

bool CMailSender::Send(int& nError, char* pBuf, int nBufSize)
{
	if(pBuf && nBufSize>0)
	{
		memset(pBuf, 0, nBufSize);
	}
	
	if(m_strServer.empty() || m_nPort==0 || m_strUser.empty() || m_strPassword.empty())
	{
		nError = -1;
		sprintf_s(pBuf, nBufSize, "SMTP服务器信息不完整。");
		return false;
	}

	if(m_strSenderMail.empty() || (m_oListRecipient.size()<1 && m_oListCarbonCopy.size()<1))
	{
		nError = -2;
		sprintf_s(pBuf, nBufSize, "发信人或收信人信息不完整。");
		return false;
	}

#if 0
	if(m_strSubject.empty() || m_strBody.empty())
	{
		err = -3;
		sprintf_s(pBuf, nBufSize, "邮件信息不完整。");
		return false;
	}
#endif

	CURL* curl = curl_easy_init();
	if (!curl)
	{
		nError = -4;
		sprintf_s(pBuf, nBufSize, "初始化环境失败。curl_easy_init fail.");
		return false;
	}

	stringstream smtp;
	if (m_bUseSSL)
	{
		smtp << "smtps://" << m_strServer << ":" << m_nPort;
	}
	else
	{
		smtp << "smtp://" << m_strServer << ":" << m_nPort;
	}

	curl_easy_setopt(curl, CURLOPT_URL, smtp.str().c_str()); 

	if(m_bUseSSL)
	{
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}

	curl_easy_setopt(curl, CURLOPT_USERNAME, m_strUser.c_str());
	curl_easy_setopt(curl, CURLOPT_PASSWORD, m_strPassword.c_str());

	curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_strSenderMail.c_str());

	struct curl_slist *recipients = NULL;
	for (list<string>::iterator it=m_oListRecipient.begin(); it!=m_oListRecipient.end(); ++it)
	{
		recipients = curl_slist_append(recipients, (*it).c_str());
	}
	for (list<string>::iterator it=m_oListCarbonCopy.begin(); it!=m_oListCarbonCopy.end(); ++it)
	{
		recipients = curl_slist_append(recipients, (*it).c_str());
	}

	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

	ComposeData();
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
	curl_easy_setopt(curl, CURLOPT_READDATA, this);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
	//curl_easy_setopt(curl, CURLOPT_INFILESIZE , m_nTotalDataSize);

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		m_oLogFile.open("MailSender.log", fstream::app);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);
		curl_easy_setopt(curl, CURLOPT_DEBUGDATA, this);
	}

	char pErrorBuf[CURL_ERROR_SIZE] = {0};
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, pErrorBuf);
	nError = curl_easy_perform(curl);
	curl_slist_free_all(recipients);
	curl_easy_cleanup(curl);

	if(m_bDebug)
	{
		m_oLogFile.close();
	}

	if(nError != CURLE_OK)
	{
		sprintf_s(pBuf, nBufSize, pErrorBuf);
		return false;
	}

	return true;
}

size_t CMailSender::payload_source(void* ptr, size_t size, size_t nmemb, void* userp)
{
	CMailSender* sender = (CMailSender*)userp;

	int num = min(size*nmemb, sender->m_nTotalDataSize-sender->m_nSentDataSize);

	if(num > 0)
	{
		string s = sender->m_strSendData.substr(sender->m_nSentDataSize, num);
		memcpy(ptr, s.c_str(), num);

		sender->m_nSentDataSize += num;
	}
	
	if (sender->m_bDebug)
	{
		printf("%0.2f\t%d/%d\n", sender->m_nSentDataSize * 100.0 / sender->m_nTotalDataSize
			, sender->m_nSentDataSize, sender->m_nTotalDataSize);
		if (sender->m_nSentDataSize == sender->m_nTotalDataSize)
		{
			printf("send cmplt,m_nSentDataSize=%d\n", sender->m_nSentDataSize);
		}
	}

	return num;
}

bool CMailSender::IsReadyToSend()
{
	if(m_strServer.empty()
		|| m_nPort == 0
		|| m_strUser.empty()
		|| m_strPassword.empty()
		|| m_strSenderMail.empty())
	{
		return false;
	}

	return true;
}

void CMailSender::EnableDebug(bool bDebug)
{
	m_bDebug = bDebug;
}

void CMailSender::ComposeData()
{
	stringstream SendData;

	//发件人
	mimetic::MailboxList from(m_strSenderMail);

	//收件人列表
	mimetic::AddressList to;
	for (list<string>::iterator it=m_oListRecipient.begin(); it!=m_oListRecipient.end(); ++it)
	{
		to.push_back(mimetic::Address(*it));
	}

	//抄送人列表
	mimetic::AddressList cc;
	for (list<string>::iterator it=m_oListCarbonCopy.begin(); it!=m_oListCarbonCopy.end(); ++it)
	{
		cc.push_back(mimetic::Address(*it));
	}
	
	mimetic::MultipartMixed mail;
	mimetic::Header& header = mail.header();
	mimetic::Body& body = mail.body();
	//写件人
	header.from(from);

	//收件人
	header.to(to);

	//抄送
	header.cc(cc);

	//主题
	header.subject(m_strSubject);

	//MIME协议版本
	header.push_back(mimetic::Field("Mime-Version","1.0"));

	//代理发信的客户端版本: 
	header.push_back(mimetic::Field("X-Mailer","libcurl 7.54.1.0"));

	//添加一段正文
	mimetic::TextHtml* pContent = new mimetic::TextHtml(m_strContent, m_strCharset, ContentTransferEncoding::base64);
	body.parts().push_back(pContent);

	//添加附件
	for (list<string>::iterator it=m_oListAttachment.begin(); it!=m_oListAttachment.end(); ++it)
	{
		string strFilePath = *it;
		string strFileName = GetFileName(strFilePath);
		string strEncodedFileName = GetEncodedFileName(strFileName);

		//创建一个新的块，在这里是附件
		mimetic::MimeEntity* pAttachment = new mimetic::MimeEntity;
		Header& h = pAttachment->header();

		// Content-Type
		h.contentType(ContentType("application","octet-stream"));
		h.contentType().paramList().push_back(ContentType::Param("name", strEncodedFileName));

		// Content-Transfer-Encoding
		h.contentTransferEncoding().mechanism(ContentTransferEncoding::base64);

		// Content-Disposition
		h.contentDisposition().type("attachment");
		h.contentDisposition().paramList().push_back(ContentDisposition::Param("filename", strEncodedFileName));

		//设置完头以后，加载具体内容
		pAttachment->body().load(strFilePath, Base64::Encoder());	//base64
		
		body.parts().push_back(pAttachment);
	}

	SendData << mail;

	m_strSendData = SendData.str();

	if (m_bDebug)
	{
// 		FILE *pFile = fopen("C:\\test2.msg", "wb");  
// 		if (pFile != NULL)
// 		{
// 			fwrite(m_strSendData.c_str(), 1, m_strSendData.length(), pFile);
// 			fclose(pFile);
// 		}
	}

	m_nSentDataSize = 0;
	m_nTotalDataSize = m_strSendData.size();	//不能大于50M(52428800),否则一般发送失败。
}

int CMailSender::debug_callback(void* curl, int type, char* str, size_t size, void* uerp)
{
	CURL* pCurl = (CURL*)curl;
	curl_infotype infotype = (curl_infotype)type;
	CMailSender* sender = (CMailSender*)uerp;
	if(sender->m_oLogFile.is_open())
	{
		if(type==CURLINFO_TEXT||
			type==CURLINFO_HEADER_IN||
			type==CURLINFO_HEADER_OUT)
		{
			sender->m_oLogFile << string(str, size);
		}
	}

	return 0;
}

string CMailSender::Base64Encode(const unsigned char * pucSource, unsigned int nlen)
{
	mimetic::Base64::Encoder b64;
	stringstream temp;
	ostreambuf_iterator<char> out(temp);
	//转为BASE64编码，目标存放至std::stringstream中
	mimetic::code(pucSource,  pucSource + nlen, b64, out);
	return temp.str();
}

string CMailSender::GetEncodedFileName(const string& strFileName)
{  
	string strEncodedFileName = "=?";
	strEncodedFileName += m_strCharset;
	strEncodedFileName += "?B?";//修改
	strEncodedFileName += Base64Encode((unsigned char *)strFileName.c_str(), strFileName.size());
	strEncodedFileName += "?=";
	return strEncodedFileName;
}  

string CMailSender::GetFileName(const string& strFilePath)
{
	string strFileName;
	string::size_type pos = strFilePath.find_last_of('/');  
	if (pos == string::npos)
	{
		pos = strFilePath.find_last_of('\\');
	}
	if (pos != string::npos)
	{
		pos += 1; // get past folder delimeter
		strFileName = strFilePath.substr(pos, strFilePath.length() - pos);
	}
	return strFileName;
}
