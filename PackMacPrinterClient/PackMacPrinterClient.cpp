// PackMacPrinterClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PackMacPrinterClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFileLog theLog;

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		theLog.SetLogName("PackMacPrinterClient");
		theLog.EnableLog(CCommonFun::IsDebug());

		theLog.Write("PackMacPrinterClient,begin");
		
		//mac客户端打包
		CString szPackPath;
		szPackPath.Format("%sPrinterClient.zip", CCommonFun::GetDefaultPath());
		CString szOrgFilePath = szPackPath.Left(szPackPath.GetLength() - 4);
#if 0
		if (CFileHelper::UnCompressZipFile(szPackPath, szOrgFilePath, FALSE))
		{
			//配置打印参数

			CIniFile file;
			file.m_sPath.Format("%sconfig.ini", CCommonFun::GetDefaultPath());
			CString szPrinterName = file.GetVal("ClientConfig", "PrinterName", "XabVPrinter");
			CString sWebDomainName = file.GetVal("ClientConfig","urldomainname",""); //打印查询的服务器域名
			CString sWebAppName = file.GetVal("ClientConfig","WebAppName","XabPrint"); //打印查询的服务器应用程序名
			CString sWebIP = file.GetVal("ClientConfig", "SvrIP", CCommonFun::GetLocalIP());
			int nWebPort = file.GetVal("ClientConfig", "urlport", 80);

			CString szUrl;	//http://192.168.2.15:80/XabPrint/WebService/iSecMacPrint.aspx
			if(sWebDomainName.GetLength()>0)
			{
				szUrl.Format("http://%s:%d",sWebDomainName,nWebPort);
			}
			else
			{
				szUrl.Format("http://%s:%d",sWebIP,nWebPort);
			}

			if (sWebAppName.GetLength()>0)
			{
				szUrl = szUrl + "/" + sWebAppName;
			}
			else
			{
				szUrl = szUrl;
			}

			CStringArray ary;
			CString szPrinterNameSet;szPrinterNameSet.Format("PrinterName=%s", szPrinterName);
			CString szHostNameSet;szHostNameSet.Format("HostName=%s/WebService/iSecMacPrint.aspx", szUrl);
			CString szCloudPrintSet;szCloudPrintSet.Format("CloudPrint=0");	//0-表示企业打印

			CString szSetupPath;
			szSetupPath.Format("%s\\PrinterClient.pkg\\Contents\\Resources\\setup.ini", szOrgFilePath);
			CStdioFile oFile;
			if (oFile.Open(szSetupPath, CFile::modeNoTruncate|CFile::modeReadWrite))
			{
				CString szRead;
				while (oFile.ReadString(szRead))
				{
					if (szRead.Find("PrinterName=") >= 0)
					{
						ary.Add(szPrinterNameSet);
					}
					else if (szRead.Find("HostName=") >= 0)
					{
						ary.Add(szHostNameSet);
					}
					else if (szRead.Find("CloudPrint=") >= 0)
					{
						ary.Add(szCloudPrintSet);
					}
					else
					{
						ary.Add(szRead);
					}
				}

				//关闭文件
				oFile.Close();
			}
			else
			{
				theLog.Write("!!,2,Open fail,err=%u, szSetupPath=%s", GetLastError(), szSetupPath);
			}

			//重新打开文件,以二进制方式写入，否则换行符可能与MAC系统不兼容
			CFile oFile2;
			if (oFile2.Open(szSetupPath, CFile::modeWrite))
			{
				//回写参数到文件
				for (int i=0; i<ary.GetCount(); i++)
				{
					CString szLine;
					szLine.Format("%s\n", ary.GetAt(i));
					oFile2.Write(szLine.GetString(), szLine.GetLength());
				}

				//关闭文件
				oFile2.Close();
			}
			else
			{
				theLog.Write("!!,3,Open fail,err=%u, szSetupPath=%s", GetLastError(), szSetupPath);
			}

			CString szZipFilePath;
			szZipFilePath.Format("%sMacPrinterClient.zip", CCommonFun::GetDefaultPath());
			if (CFileHelper::CompressZipFile(szOrgFilePath, szZipFilePath, FALSE))
			{
				//C:\WorkSpace\iSecPrint5.5-20171226\iSecStar\WebServer\bin\Client_Setup\MacPrinterClient.zip
				//C:\WorkSpace\iSecPrint5.5-20171226\iSecStar\WebServer\down\MacPrinterClient.zip

				CString szDownZipFilePath = szZipFilePath;
				szDownZipFilePath.Replace("bin\\Client_Setup", "down");
				if (!MoveFileEx(szZipFilePath, szDownZipFilePath, MOVEFILE_REPLACE_EXISTING))
				{
					theLog.Write("!!,4,MoveFileEx fail,error=%u,szOrgFilePath=%s,szZipFilePath=%s"
						, GetLastError(), szOrgFilePath, szZipFilePath);
				}
			}
			else
			{
				theLog.Write("!!,5,CompressZipFile fail,szOrgFilePath=%s,szZipFilePath=%s", szOrgFilePath, szZipFilePath);
			}

			CCommonFun::DeleteDir(szOrgFilePath);
		}

		else
		{
			theLog.Write("!!,6,UnCompressZipFile fail,szPackPath=%s,szOrgFilePath=%s", szPackPath, szOrgFilePath);
		}
#endif
	}
	theLog.Write("PackMacPrinterClient,end");

	return nRetCode;
}
