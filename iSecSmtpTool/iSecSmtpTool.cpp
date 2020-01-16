// iSecSmtpTool.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;

string m_sIniPath;
std::string GetVal( TCHAR* sKey)
{
	TCHAR inBuf[MAX_PATH] = {0};
	DWORD dw = GetPrivateProfileString ("iSecMailSender",sKey, "", inBuf, 
		MAX_PATH, m_sIniPath.c_str()); 
	return string(inBuf);
}

//��ȡ��־Ŀ¼
char* GetLogDir()
{
	static char g_szLogDir[MAX_PATH] = {0};
	if (strlen(g_szLogDir) <= 0)
	{
		OutputDebugString("##CCommonFun::GetLogDir");

		TCHAR path[MAX_PATH];
		if(GetModuleFileName(0,path,MAX_PATH))
		{
			TCHAR *pdest;
			pdest=_tcsrchr(path,_T('\\'));
			if(pdest) 
				*pdest=0;
			strcpy_s(g_szLogDir, path);
			strcat_s(g_szLogDir, "\\xlog");
		}

		if (strlen(g_szLogDir) <= 0)
		{
			strcpy_s(g_szLogDir, ".\\xlog");
		}
	}

	return g_szLogDir;
}
void WriteLogEx(const char* lpszFormat, ...)
{
	char bysBuf[2048];
	int nBufLen = sizeof(bysBuf);

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	memset(bysBuf, 0x0, nBufLen);
	sprintf(bysBuf, "%s\\%d.%d.%d_iSecSmtpTool.log", GetLogDir(), curTime.wYear, curTime.wMonth, curTime.wDay);
	FILE *hFile = fopen(bysBuf, "a+");
	if(!hFile)
	{
		return;
	}

	int nNeedWrLen = 0;
	memset(bysBuf, 0x0, nBufLen);
	sprintf(bysBuf, "%02d:%02d:%02d ", curTime.wHour, curTime.wMinute, curTime.wSecond);

	nNeedWrLen = strlen(bysBuf);
	fwrite(bysBuf, nNeedWrLen, 1, hFile);


	memset(bysBuf, 0x0, nBufLen);
	va_list argList;
	va_start(argList, lpszFormat);
	_vsntprintf (bysBuf, nBufLen, lpszFormat, argList) ;
	va_end(argList);

	nNeedWrLen = strlen(bysBuf);
	fwrite(bysBuf, nNeedWrLen, 1, hFile);

	fwrite("\r\n", strlen("\r\n"), 1, hFile);
	fclose(hFile);

	return;
}

string GetDefaultPath()
{
	TCHAR path[MAX_PATH];
	string sDefaultPath;
	if(GetModuleFileName(0,path,MAX_PATH)==0) 
		return _T("");
	TCHAR *pdest;
	pdest=_tcsrchr(path,_T('\\'));
	if(pdest) 
		*pdest=0;
	sDefaultPath=path;
	sDefaultPath+=_T("\\");
	return sDefaultPath;
}

#if 1
#include "../Public/xabzipPort.h"
BOOL UncompressGzFile(string &szFilePath)
{
	char bysMsg[1024];
	memset(bysMsg, 0x0, sizeof(bysMsg) / sizeof(bysMsg[0]));

	int nLen = szFilePath.length();
	if(4 >= nLen)
	{
		WriteLogEx("!!UncompressGzFile,0.1,len=%d,szFilePath=[%s]", nLen, szFilePath.c_str());
		return FALSE;
	}
	if(0 != szFilePath.compare(nLen - 3, nLen - 1, ".gz"))
	{
		WriteLogEx("!!UncompressGzFile,0.2,len=%d,NOT GZ,szFilePath=[%s],subStr=[%s]", nLen, szFilePath.c_str(), szFilePath.substr(nLen - 3, nLen - 1));
		return FALSE;
	}
	FILE *hFile = ::fopen(szFilePath.c_str(), "r+");
	if(!hFile)
	{
		WriteLogEx("!!UncompressGzFile,1,fopen fail,err=%d,szFilePath=%s", GetLastError(), szFilePath.c_str());
		return FALSE;
	}
	::fclose(hFile);
	hFile = NULL;
	int nRet = file_uncompress_ex((char*)szFilePath.c_str(), 0);	//��ѹ������ļ�������.gz��׺ȥ���������ò�������
	if(0 > nRet)
	{
		WriteLogEx("!!UncompressGzFile,2,file_uncompress_ex fail,err=%d,szFilePath=%s", GetLastError(), szFilePath.c_str());
		return FALSE;
	}

	string szNewFile = szFilePath.substr(0, nLen - 3);

	WriteLogEx("==UncompressGzFile,3,file_uncompress_ex OK,szNewFile=[%s],szFilePath=[%s]", szNewFile.c_str(), szFilePath.c_str());
	szFilePath = szNewFile;

	return TRUE;
}
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	bool bError = false;
	bool bSSL = false;	
	m_sIniPath = GetDefaultPath();
	m_sIniPath += "config\\configMail.ini";

	string mailAddr = GetVal("SendMail");
	string user = GetVal("User");
	string pwd = GetVal("Pwd");
	string svr = GetVal("SmtpSvr");
	
	string port = GetVal("SmtpPort");
	string ssl =GetVal("SmtpSSL");
	string szUseGzFile = GetVal("UseGzFile");	//add by zfq,2013.06.24

	bool bDelFile = false;	//�Ƿ�ɾ�������ļ�

	if(mailAddr.empty())
	{
		mailAddr = "zhandb@isafetec.cn";
	}
	if(port.empty())
	{
		port = "25";
	}
	if (svr.empty())
	{
		svr = "192.168.2.11";
	}
	bSSL = atoi(ssl.c_str()) == 1 ? true : false;

	string mailto,mailFile;

	if (argc >= 3)
	{
		mailto = argv[1];
		mailFile = argv[2];

		const char *pFlagStr = "DeleteOriFile";
		if(argc >= 4 && 0 == strncmp(pFlagStr, argv[3], strlen(pFlagStr)))
		{
			bDelFile = true;
		}

		if(0 != szUseGzFile.compare("1"))	//add by zfq,2013.06.24,�ڡ�iSecMailSender��,����UseGzFile��,ֵ��1��������gzѹ���ļ���Ĭ�ϲ����á�
		{
			WriteLogEx("==_tmain,12������ѹ���ļ�,�ڡ�iSecMailSender��,����UseGzFile��,ֵ��1��������gzѹ���ļ���Ĭ�ϲ����á�");

			bDelFile = true;	//add by zxl,2014.04.04,��Ϊɨ���ļ�Ĭ����ѹ����ʽ�ϴ����������ϵ�,Ҫ��ѹ��,ɾ�����ļ�
			UncompressGzFile(mailFile);
		}
		else
		{
			WriteLogEx("==_tmain,13������ѹ���ļ�,�ڡ�iSecMailSender��,����UseGzFile��,ֵ��1��������gzѹ���ļ���Ĭ�ϲ����á�");
		}

		FILE *hFile = ::fopen(mailFile.c_str(), "r+");
		if(!hFile)
		{
			WriteLogEx("==_tmain,14,open fail,err=%d,mailFile=[%s]", errno, mailFile.c_str());
			return 0;
		}
		fclose(hFile);
	}
	else if (argc == 2)
	{
		mailto = argv[1];
	}
	else if (argc <= 1)
	{
		return 0;
	}

	WriteLogEx("_tmain,14.1");

	{
		int nError;
		int nBufSize = 1024;
		char* pBuf = new char[nBufSize];
		memset(pBuf, 0x0, nBufSize);

		DWORD dwTick1 = GetTickCount();
		CMailSender mailsender;
		mailsender.SetCharset("GB2312");
		mailsender.EnableDebug(false);
		mailsender.SetServer(svr.c_str(), atoi(port.c_str()), user.c_str(), pwd.c_str(), bSSL);
		mailsender.SetSender(mailAddr.c_str(), user.c_str());
		mailsender.AddRecipient(mailto.c_str());
		//mailsender.AddCarbonCopy("");
		mailsender.SetSubject("ɨ��");
		//mailsender.SetContent("");
		if (argc >= 3)
		{
			mailsender.AddAttachment(mailFile.c_str());
		}

		if (!mailsender.Send(nError, pBuf, nBufSize))
		{
			bError = true;
			WriteLogEx("Send Fail,error=%d,%s,time=%u(ms)", nError, pBuf, GetTickCount() - dwTick1);
		}
		else
		{
			WriteLogEx("Send Succ,time=%u(ms)", GetTickCount() - dwTick1);
		}

		delete[] pBuf;
	}

	return 0;
}

