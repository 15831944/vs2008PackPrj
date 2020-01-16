// TestCurlWrapLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestCurlWrapLib.h"

#include "../Public/CurlWrapPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;
CFileLog theLog;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	theLog.SetLogName("TestCurlWrapLib");

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
#if 0 //Test Email
		// TODO: 在此处为应用程序的行为编写代码。
		CMailSender mail;
		int nPort = 25;

		char cMailServer[100] = {"\0"};		//邮件服务器地址
		int nMailPort = 25;				//邮件服务器端口
		char cMailFrom[100] = {"\0"};		//发件人邮箱
		char cMailUser[100] = {"\0"};		//发件人用户名
		char cMailPwd[100] = {"\0"};			//发件人密码
		char cSendMailTo[100] = {"\0"};		//邮件发送目的

		strcpy(cMailServer,"mail.isecstar.com");
		strcpy(cMailFrom,"liuzhengjun@isecstar.com");
		//strcpy(cMailUser,"liuzhengjun");//可以
		//strcpy(cMailUser,"liuzhengjun@isecstar.com");不行
		strcpy(cMailUser,"liuzhengjun");
		strcpy(cMailPwd,"lzj2019@");
		strcpy(cSendMailTo,"1007482035@qq.com");
		

	


		//判断是否使用SSL，简单判断是否是25端口，如果不是，则认为是SSL
		bool bSSL = (nPort == 25) ? false : true;

		//包含密码等敏感信息，不要轻易打开日志
		//theLog.Write("CScanDocDealThread::ScanJobSendMail,cMailServer=%s,nMailPort=%d,cMailUser=%s,cMailPwd=%s,cMailFrom=%s,cSendMailTo=%s,bSSL=%d"
		//	, pJob->cMailServer, pJob->nMailPort, pJob->cMailUser, pJob->cMailPwd, pJob->cMailFrom,pJob->cSendMailTo,bSSL);


		//设置服务器信息
		mail.SetServer(cMailServer, nMailPort, cMailUser, cMailPwd, bSSL);

		//设置发送人信息
		mail.SetSender(cMailFrom, cMailUser);

		//添加收件人
		mail.AddRecipient(cSendMailTo);

		//添加主题
		mail.SetSubject("Scan to mail");

		//添加内容
		mail.SetContent("Scan to mail");

		//添加附件
	
		
		CString szScanFilePath;
		//szScanFilePath.Format("E:\\TestMail\\test1.pdf");
		szScanFilePath.Format("D:\\testdoc_pdf\\Go语言标准库文档中文版.pdf");
		
		//1.过滤空路径的文件（只有目录，没有文件）,否则在邮件发送内部代码会导致程序崩溃。
		//2.文件过大(37.5MB)，会发送失败，并且内存占用会非常大，导致程序崩溃，37MB=38797312，
		//而不使用37MB，是因为邮件的内容除了文件本身还要包含其它mime，所以预留0.5MB。
		DWORD dwFileSize = CCommonFun::GetFileSizeByte(szScanFilePath);
		if (!PathFileExists(szScanFilePath) || (dwFileSize <= 0) || (dwFileSize > 38797312))
		{
			printf("!!CScanDocDealThread::ScanJobSendMail,2,PathFileExists,szScanFilePath=%s,dwFileSize=%u"
				, szScanFilePath, dwFileSize);
		}

		mail.AddAttachment(szScanFilePath.GetString());

		//发送邮件
		int nError;
		int nBufSize = 300;
		char pBuf[300] = {0};
		printf("=========发送前");
		if (mail.Send(nError, pBuf, nBufSize))
		{
			theLog.Write("!!CScanDocDealThread::ScanJobSendMail, success.");
			printf("发送成功\n");
		}
		else
		{
			theLog.Write("!!CScanDocDealThread::ScanJobSendMail,3,mail.Send fail,nError=%d,pBuf=%s", nError, pBuf);
			printf("发送失败\n");
		}
		printf("=========发送后");

#endif

#if 0
		//CString szUrl = "https://jxut.educationgroup.cn/pos/api/preCreate?mchid=7c595dcd58644b9dbd6401476d4afcc4&payid=20200107194731986258&price=0.010000&state=1:20200107194731986258:2017007141:1578397651&sign=0c0ca55397679010d57f17c33db96ac4";
		CString szUrl = "http://jw.shiep.edu.cn/eams/teach/grade/transcript/report-interface!gradeReport.action?stdCode=20152267&template=report_chs&extraProviders=transcriptOtherGradeProvider&printBy=LG01202001080015&sign=80ab9a8e43ed669f71cd95a4383f3538";
		
		CHttpClient oClient;
		oClient.SetDebug(true);
		string sResponse;

		oClient.Get(szUrl.GetString(),sResponse);
		printf("sResponse=%s\n",sResponse.c_str());
#endif
		CString szUrl = "http://jwc.hyit.edu.cn/cjzkAction.do?method=printChineseTranscript&xh=1161111126&sfzjh=32100219971003001X&sjm=isec&sigeData=57F1C4DC1245C9027131A6A6640B81A9";
		CString sFile = "D:/MyProject/vs2008Prj/vs2008PackPrj/Release/transcript.pdf";
		CurlHttp::CHttpClient oClient;
		//oClient.SetDebug(true);
		oClient.HttpDownLoadeFile(szUrl.GetString(),sFile.GetString());
		//oClient.Post

	}
	system("pause");
	return nRetCode;
}
