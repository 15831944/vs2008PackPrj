#include "stdafx.h"
#include "iSecCloudDocHelper.h"
#include <QtGui/QApplication>

#pragma data_seg("iSecCloudDocHelper_data") 
long g_lInstance = 0;
long g_lSession[100] = {0};
#pragma data_seg() 
#pragma comment(linker,"/Section:iSecCloudDocHelper_data,rws")

CFileLog theLog;
CIniFile theIniFile;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	CMiniDumper::Enable("CloudPrintDocConverter", false);	//崩溃时生成dump

	theLog.SetLogName("CloudPrintDocConverter");
	theLog.EnableLog(IsDebug());

	theIniFile.m_sPath.Format("%s%s",CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);

	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

	// QMessageBox的按钮中文化需要
	QTranslator translator(0);
	translator.load(":/iSecCloudDocHelper/Resources/qt_zh_CN.qm");
	a.installTranslator(&translator); 

	/*
	if(argc==2 && stricmp(argv[1], "/install")==0) 
	{
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"iSecCloudDocHelper.exe", 
			CCommonFun::GetCommonDesktopDir()+"\\云打印数据库配置.lnk", CCommonFun::GetDefaultPath()+"res\\iSecCloudDocHelper.ico", 0, "");
		return 0;	
	} 
	else if(argc==2 && stricmp(argv[1], "/uninstall")==0) 
	{
		CString sdsk = CCommonFun::GetCommonDesktopDir();
		if(PathFileExists(sdsk+"\\云打印数据库配置.lnk"))
			CFile::Remove(sdsk+"\\云打印数据库配置.lnk");

		return 0;
	}
	else*/ if (argc==2 &&  stricmp(argv[1], "/auto")==0)
	{
		if(InterlockedIncrement(&g_lInstance) > 1)
		{
			theLog.Write("已经存在 g_lInstance = %d",g_lInstance);
			return FALSE;
		}

		iSecCloudDocHelper w;
		w.hide();
		w.StartWork();
		return a.exec();
	}
	else/* if (argc==2 &&  stricmp(argv[1], "/config")==0)*/
	{
		iSecCloudDocHelper w;
		w.show();
		return a.exec();
	}
}
