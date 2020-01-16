#pragma once

#include <string>
#include <list>
#include <fstream>

using namespace std;

class CMailSender
{
public:
	CMailSender(void);
	~CMailSender(void);

	//设置邮件编码
	void SetCharset(const char* pCharset);

	//设置邮件服务器信息
	void SetServer(const char* pcServer, int nPort, const char* pcUser, const char* pcPassword, bool bUseSSL);

	//设置邮件发送者信息信息
	void SetSender(const char* pcFrom, const char* pSenderName = NULL);

	//添加一个收件人
	void AddRecipient(const char* pcTo);

	//清空收件人
	void ClearRecipient();

	//添加一个抄送人
	void AddCarbonCopy(const char* pcCC);

	//清空抄送人
	void ClearCarbonCopy();

	//设置邮件主题
	void SetSubject(const char* pcSubject);

	//设置邮件内容
	void SetContent(const char* pcContent);

	//添加一个附件(文件路径)
	void AddAttachment(const char* pcAttachment);

	//清空附件
	void ClearAttachment();

	//执行发送操作
	bool Send(int& nError, char* pBuf, int nBufSize);

	//发送条件是否已经准备就绪
	bool IsReadyToSend();

	//设置是否启用日志
	void EnableDebug(bool bDebug);

protected:
	string m_strCharset;	//邮件编码
	string m_strServer;		//邮件服务器地址
	int m_nPort;			//邮件服务器端口
	bool m_bUseSSL;			//是否使用SSL加密
	string m_strUser;		//发件人用户名
	string m_strPassword;	//发件人密码

	string m_strSubject;	//邮件主题
	string m_strContent;	//邮件内容
	string m_strSenderName;	//邮件发送者姓名
	string m_strSenderMail;		//邮件发送者邮箱
	list<string> m_oListRecipient;	//邮件收件人列表
	list<string> m_oListCarbonCopy;	//邮件抄送人列表
	list<string> m_oListAttachment;	//邮件附件列表
	string m_strSendData;	//整个MIME协议字符串

	static size_t payload_source(void *ptr, size_t size, size_t nmemb, void* userp);
	static int debug_callback(void* curl, int type, char * str, size_t size, void* uerp);

	void ComposeData();
	int m_nSentDataSize;
	int m_nTotalDataSize;
	fstream m_oLogFile;
	bool m_bDebug;

	string Base64Encode(const unsigned char * pucSource, unsigned int nlen);  
	string GetEncodedFileName(const string& strFileName);
	string GetFileName(const string& strFilePath);
};
