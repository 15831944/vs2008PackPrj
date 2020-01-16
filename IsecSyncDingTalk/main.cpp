#include "stdafx.h"
#include "isecsyncdingtalk.h"
#include <QtGui/QApplication>
CFileLog theLog;
CIniFile theIniFile;
IsecSyncDingTalk *g_pIsecSyncDingTalk;
CCloudWebServiceHelper g_webservice;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

	// QMessageBox�İ�ť���Ļ���Ҫ
	QTranslator translator(0);
	translator.load(":/IsecSyncDingTalk/Resources/qt_zh_CN.qm");
	a.installTranslator(&translator); 
	theLog.SetLogName("iSyncDingTalk");
	theLog.EnableLog(TRUE);

	//�����ļ�
	theIniFile.m_sPath.Format("%s%s",CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
	g_pIsecSyncDingTalk = new IsecSyncDingTalk;
	g_pIsecSyncDingTalk->show();
	return a.exec();
}
