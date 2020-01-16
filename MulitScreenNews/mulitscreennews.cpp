#include "stdafx.h"
#include "mulitscreennews.h"

MulitScreenNews::MulitScreenNews(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);

	QDesktopWidget* desktopWidget = QApplication::desktop();
	//获取可用桌面大小
	//QRect deskRect = desktopWidget->availableGeometry();
	//获取设备屏幕大小
	QRect screenRect = desktopWidget->screenGeometry();
	QString qsMsg = QString("screenCount=%1,width0=%2,height0=%3,width1=%4,htight1=%5")
		.arg(desktopWidget->screenCount())
		.arg(desktopWidget->screenGeometry(desktopWidget->primaryScreen()).width())
		.arg(desktopWidget->screenGeometry(desktopWidget->primaryScreen()).height())
		.arg(desktopWidget->screenGeometry(1).width())
		.arg(desktopWidget->screenGeometry(1).height());
	WriteLogEx("MulitScreenNews::MulitScreenNews,1,%s", qsMsg.toStdString().c_str());

	QSettings settings(QCoreApplication::applicationDirPath()+"/"+SYS_CONFIG_DIR+"/settings.ini", QSettings::IniFormat);
	settings.setIniCodec("UTF-8");
	m_nFromScreenIndex = settings.value("FromScreenIndex", 0).toInt();
	m_nToScreenIndex = settings.value("ToScreenIndex", 1).toInt();
	WriteLogEx("MulitScreenNews::MulitScreenNews,2,m_nFromScreenIndex=[%d], m_nToScreenIndex=[%d]", m_nFromScreenIndex, m_nToScreenIndex);

	if (desktopWidget->screenCount()>=2)
	{
		//获取主屏幕矩形尺寸
		QRect primaryRect = desktopWidget->screenGeometry(m_nFromScreenIndex); //screenGeometry(0) 第一块屏
		QRect secondRect  = desktopWidget->screenGeometry(m_nToScreenIndex);  //screenGeometry(1) 第二块屏

		move( primaryRect.width() + secondRect.width()/2 , secondRect.height()/2);
		showFullScreen();
		QString qsHtmlFilePath = QCoreApplication::applicationDirPath()+"/res/MulitScreenNews.html";
		WriteLogEx("MulitScreenNews::MulitScreenNews,3,qsHtmlFilePath=[%s]", qsHtmlFilePath.toStdString().c_str());
		QFile file(qsHtmlFilePath);
		file.open(QIODevice::Text|QIODevice::ReadOnly);
		QString html = QString::fromUtf8(file.readAll());
		file.close();
		webView->setHtml(html);
		webView->setContextMenuPolicy(Qt::NoContextMenu); 
	}
	else
	{
		WriteLogEx("!!MulitScreenNews::MulitScreenNews,4,screenCount=%d,小于2，退出。", desktopWidget->screenCount());
		exit(1);
	}
}

MulitScreenNews::~MulitScreenNews()
{

}
