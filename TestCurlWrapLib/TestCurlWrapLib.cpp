// TestCurlWrapLib.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestCurlWrapLib.h"

#include "../Public/CurlWrapPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;
CFileLog theLog;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	theLog.SetLogName("TestCurlWrapLib");

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
#if 0 //Test Email
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		CMailSender mail;
		int nPort = 25;

		char cMailServer[100] = {"\0"};		//�ʼ���������ַ
		int nMailPort = 25;				//�ʼ��������˿�
		char cMailFrom[100] = {"\0"};		//����������
		char cMailUser[100] = {"\0"};		//�������û���
		char cMailPwd[100] = {"\0"};			//����������
		char cSendMailTo[100] = {"\0"};		//�ʼ�����Ŀ��

		strcpy(cMailServer,"mail.isecstar.com");
		strcpy(cMailFrom,"liuzhengjun@isecstar.com");
		//strcpy(cMailUser,"liuzhengjun");//����
		//strcpy(cMailUser,"liuzhengjun@isecstar.com");����
		strcpy(cMailUser,"liuzhengjun");
		strcpy(cMailPwd,"lzj2019@");
		strcpy(cSendMailTo,"1007482035@qq.com");
		

	


		//�ж��Ƿ�ʹ��SSL�����ж��Ƿ���25�˿ڣ�������ǣ�����Ϊ��SSL
		bool bSSL = (nPort == 25) ? false : true;

		//���������������Ϣ����Ҫ���״���־
		//theLog.Write("CScanDocDealThread::ScanJobSendMail,cMailServer=%s,nMailPort=%d,cMailUser=%s,cMailPwd=%s,cMailFrom=%s,cSendMailTo=%s,bSSL=%d"
		//	, pJob->cMailServer, pJob->nMailPort, pJob->cMailUser, pJob->cMailPwd, pJob->cMailFrom,pJob->cSendMailTo,bSSL);


		//���÷�������Ϣ
		mail.SetServer(cMailServer, nMailPort, cMailUser, cMailPwd, bSSL);

		//���÷�������Ϣ
		mail.SetSender(cMailFrom, cMailUser);

		//����ռ���
		mail.AddRecipient(cSendMailTo);

		//�������
		mail.SetSubject("Scan to mail");

		//�������
		mail.SetContent("Scan to mail");

		//��Ӹ���
	
		
		CString szScanFilePath;
		//szScanFilePath.Format("E:\\TestMail\\test1.pdf");
		szScanFilePath.Format("D:\\testdoc_pdf\\Go���Ա�׼���ĵ����İ�.pdf");
		
		//1.���˿�·�����ļ���ֻ��Ŀ¼��û���ļ���,�������ʼ������ڲ�����ᵼ�³��������
		//2.�ļ�����(37.5MB)���ᷢ��ʧ�ܣ������ڴ�ռ�û�ǳ��󣬵��³��������37MB=38797312��
		//����ʹ��37MB������Ϊ�ʼ������ݳ����ļ�����Ҫ��������mime������Ԥ��0.5MB��
		DWORD dwFileSize = CCommonFun::GetFileSizeByte(szScanFilePath);
		if (!PathFileExists(szScanFilePath) || (dwFileSize <= 0) || (dwFileSize > 38797312))
		{
			printf("!!CScanDocDealThread::ScanJobSendMail,2,PathFileExists,szScanFilePath=%s,dwFileSize=%u"
				, szScanFilePath, dwFileSize);
		}

		mail.AddAttachment(szScanFilePath.GetString());

		//�����ʼ�
		int nError;
		int nBufSize = 300;
		char pBuf[300] = {0};
		printf("=========����ǰ");
		if (mail.Send(nError, pBuf, nBufSize))
		{
			theLog.Write("!!CScanDocDealThread::ScanJobSendMail, success.");
			printf("���ͳɹ�\n");
		}
		else
		{
			theLog.Write("!!CScanDocDealThread::ScanJobSendMail,3,mail.Send fail,nError=%d,pBuf=%s", nError, pBuf);
			printf("����ʧ��\n");
		}
		printf("=========���ͺ�");

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
