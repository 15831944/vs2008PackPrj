// iSecSmtpTool.cpp : 定义控制台应用程序的入口点。
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

//获取日志目录
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
	int nRet = file_uncompress_ex((char*)szFilePath.c_str(), 0);	//解压缩后的文件，仅把.gz后缀去掉，用配置参数控制
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

	bool bDelFile = false;	//是否删除附件文件

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

		if(0 != szUseGzFile.compare("1"))	//add by zfq,2013.06.24,节【iSecMailSender】,键【UseGzFile】,值【1代表启用gz压缩文件，默认不启用】
		{
			WriteLogEx("==_tmain,12，不用压缩文件,节【iSecMailSender】,键【UseGzFile】,值【1代表启用gz压缩文件，默认不启用】");

			bDelFile = true;	//add by zxl,2014.04.04,因为扫描文件默认是压缩格式上传到服务器上的,要解压后,删除新文件
			UncompressGzFile(mailFile);
		}
		else
		{
			WriteLogEx("==_tmain,13，启用压缩文件,节【iSecMailSender】,键【UseGzFile】,值【1代表启用gz压缩文件，默认不启用】");
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
		mailsender.SetSubject("扫描");
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

