#include "stdafx.h"
#include "iSecCloudDocHelper.h"

iSecCloudDocHelper::iSecCloudDocHelper(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	setupUi(this);
	on_comboBox_dbtype_currentIndexChanged(SQLSERVER);
	setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	m_pMainThread = NULL;
	m_pSystemTray = NULL;
	m_pTrayMenu = NULL;
	m_bQuit = false;
	m_bStarted = false;

	//初始化系统托盘功能
	InitSystemTray();
}

iSecCloudDocHelper::~iSecCloudDocHelper()
{
	ReleaseSystemTray();
	ReleaseWorkThread();
}

void iSecCloudDocHelper::showEvent( QShowEvent * event )
{
	//打印数据库配置
	int nPrintDbType = theIniFile.GetVal("DbConfig","DbType", MYSQL);
	CString szPrintDbIP = theIniFile.GetVal("DbConfig","DbSvr", "192.168.4.240");
	CString szPrintDbPort = theIniFile.GetVal("DbConfig","DbPort", "3306");
	CString szPrintDbName = theIniFile.GetVal("DbConfig","DbName", "iSecCloud");
	CString szPrintDbUser = theIniFile.GetVal("DbConfig","DbUser", "root");
	CString szPrintDbPwd = theIniFile.GetVal("DbConfig","DbPwd", "");
#ifdef ENABLE_PWD_CRY
	char szPwd[MAX_PATH] = {0};
	int nSize = MAX_PATH;
	CPWDCry cry;
	cry.UnCryptPWD(szPrintDbPwd.GetBuffer(),szPwd,nSize);
	szPrintDbPwd.ReleaseBuffer();
	szPrintDbPwd = szPwd;
#endif

	comboBox_dbtype->setCurrentIndex(nPrintDbType);
	lineEdit_ip->setText(szPrintDbIP.GetBuffer());
	lineEdit_port->setText(szPrintDbPort.GetBuffer());
	lineEdit_dbname->setText(szPrintDbName.GetBuffer());
	lineEdit_dbuser->setText(szPrintDbUser.GetBuffer());
	lineEdit_dbpwd->setText(szPrintDbPwd.GetBuffer());

	//设置功能按钮
	if (m_bStarted)
	{
		pushButton_start->setText(tr("停止转换"));
	}
	else
	{
		pushButton_start->setText(tr("开启转换"));
	}
	
}

void iSecCloudDocHelper::on_pushButton_test_clicked()
{
	int nDbType = comboBox_dbtype->currentIndex();
	CString szIP = lineEdit_ip->text().toStdString().c_str();
	CString szPort = lineEdit_port->text().toStdString().c_str();
	CString szDbName = lineEdit_dbname->text().toStdString().c_str();
	CString szUserName = lineEdit_dbuser->text().toStdString().c_str();
	CString szUserPwd = lineEdit_dbpwd->text().toStdString().c_str();

	CAdo ado;
	ado.SetConnStr2(szIP,szDbName,szUserName,szUserPwd,szPort,nDbType);
	if (ado.Connect())
	{
		QMessageBox::information(this, tr("提示"), tr("连接成功."), tr("确定"));
	}
	else
	{
		QMessageBox::information(this, tr("提示"), tr("连接失败!"), tr("确定"));
	}
}

void iSecCloudDocHelper::on_pushButton_save_clicked()
{
	int nDbType = comboBox_dbtype->currentIndex();
	CString szIP = lineEdit_ip->text().toStdString().c_str();
	CString szPort = lineEdit_port->text().toStdString().c_str();
	CString szDbName = lineEdit_dbname->text().toStdString().c_str();
	CString szUserName = lineEdit_dbuser->text().toStdString().c_str();
	CString szUserPwd = lineEdit_dbpwd->text().toStdString().c_str();

	CAdo ado;
	ado.SetConnStr2(szIP,szDbName,szUserName,szUserPwd,szPort,nDbType);
	if (!ado.Connect())
	{
		if (QMessageBox::information(this, tr("提示"), tr("连接数据库失败,确定要保存配置吗？")
			, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
		{
			return;
		}
	}

	//打印数据库配置
	theIniFile.SetVal("DbConfig","DbType", nDbType);
	theIniFile.SetVal("DbConfig","DbSvr", szIP);
	theIniFile.SetVal("DbConfig","DbPort", szPort);
	theIniFile.SetVal("DbConfig","DbName", szDbName);
	theIniFile.SetVal("DbConfig","DbUser", szUserName);
#ifdef ENABLE_PWD_CRY
	CPWDCry cry;
	char *pPwd = cry.CryptPWD(szUserPwd.GetBuffer());
	szUserPwd = pPwd;
	delete[] pPwd;
#endif
	theIniFile.SetVal("DbConfig","DbPwd", szUserPwd);
}

void iSecCloudDocHelper::on_pushButton_start_clicked()
{
	if (m_bStarted)
	{
		StopWork();
	}
	else
	{
		StartWork();
	}
}

void iSecCloudDocHelper::on_CmdFromWorkThread(int nMainCmd, int nSubCmd, void* pData)
{
	theLog.Write("iSecCloudDocHelper::on_CmdFromWorkThread,1,nMainCmd=%d,nSubCmd=%d,pData=%p", nMainCmd, nSubCmd, pData);
}

void iSecCloudDocHelper::on_comboBox_dbtype_currentIndexChanged(int index)
{
	if (index == SQLSERVER)
	{
		lineEdit_port->setText(tr("1433"));
		lineEdit_dbname->setText(tr("iSec_Print"));
		lineEdit_dbuser->setText(tr("sa"));
	}
	else if (index == ORACLE)
	{
		lineEdit_port->setText(tr("1521"));
		lineEdit_dbname->setText(tr(""));
		lineEdit_dbuser->setText(tr(""));
	}
	else if (index == MYSQL)
	{
		lineEdit_port->setText(tr("3306"));
		lineEdit_dbname->setText(tr("iSecCloud"));
		lineEdit_dbuser->setText(tr("root"));
	}
	else
	{
		lineEdit_port->setText(tr("1433"));
		lineEdit_dbname->setText(tr("iSec_Print"));
		lineEdit_dbuser->setText(tr("sa"));
	}
	lineEdit_ip->setText(tr("localhost"));
	lineEdit_dbpwd->setText(tr(""));
}


void iSecCloudDocHelper::InitWorkThread()
{
	theLog.Write("iSecCloudDocHelper::InitWorkThread,m_pMainThread=%p", m_pMainThread);

	if (!m_pMainThread)
	{
		m_pMainThread = (CMainThread*)::AfxBeginThread(RUNTIME_CLASS(CMainThread),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if (m_pMainThread)
		{
			m_pMainThread->ResumeThread();
		}
	}
}

void iSecCloudDocHelper::ReleaseWorkThread()
{
	theLog.Write("iSecCloudDocHelper::ReleaseWorkThread,begin,m_pMainThread=%p", m_pMainThread);

	if (m_pMainThread)
	{
		CThreadMgr::DestroyThread(m_pMainThread, INFINITE);
		m_pMainThread = NULL;
	}

	theLog.Write("iSecCloudDocHelper::ReleaseWorkThread,end,m_pMainThread=%p", m_pMainThread);
}

void iSecCloudDocHelper::InitSystemTray()
{
	m_pSystemTray = new QSystemTrayIcon(this);

	//创建QIcon对象，参数是图标资源，值为项目的资源文件中图标的地址
	QIcon iconmain(":/iSecCloudDocHelper/Resources/iSecCloudDocHelper.ico");
	QIcon iconquit(":/iSecCloudDocHelper/Resources/quit.ico");
	QIcon iconshow(":/iSecCloudDocHelper/Resources/show.ico");
	QIcon iconabout(":/iSecCloudDocHelper/Resources/about.ico");

	m_pTrayMenu = new QMenu(/*this*/);	//QSystemTrayIcon的上下文菜单失去焦点仍然显示的问题

	//为托盘菜单添加菜单项
	QAction* pShowAction = m_pTrayMenu->addAction(iconshow, tr("显示主界面"));

	//为系统托盘绑定单击信号的槽 即图标激活时
	connect(pShowAction, SIGNAL(triggered()), this, SLOT(on_Show_triggered()));
	
	//为托盘菜单添加菜单项
	QAction* pAboutAction = m_pTrayMenu->addAction(iconabout, tr("关于"));

	//为系统托盘绑定单击信号的槽 即图标激活时
	connect(pAboutAction, SIGNAL(triggered()), this, SLOT(on_About_triggered()));

 	//为托盘菜单添加分隔符
 	m_pTrayMenu->addSeparator();

	//为托盘菜单添加菜单项
	QAction* pQuitAction = m_pTrayMenu->addAction(iconquit, tr("退出"));

	//为系统托盘绑定单击信号的槽 即图标激活时
	connect(pQuitAction, SIGNAL(triggered()), this, SLOT(on_Quit_triggered()));

	//将创建的QIcon对象作为系统托盘图标
	m_pSystemTray->setIcon(iconmain);

	//显示托盘图标
	m_pSystemTray->show();

	//设置系统托盘提示
	m_pSystemTray->setToolTip(tr("云打印转换工具"));

	//将创建菜单作为系统托盘菜单   
	m_pSystemTray->setContextMenu(m_pTrayMenu);

// 	//在系统托盘显示气泡消息提示
// 	m_pSystemTray->showMessage(tr("提示"), tr("云打印转换工具正在运行。"), QSystemTrayIcon::Information, 5000);

	//为系统托盘绑定单击信号的槽 即图标激活时
	connect(m_pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_SystemTrayIcon_Clicked(QSystemTrayIcon::ActivationReason)));
}

void iSecCloudDocHelper::ReleaseSystemTray()
{
	if (m_pSystemTray)
	{
		m_pSystemTray->hide();
		delete m_pSystemTray;
		m_pSystemTray = NULL;
	}

	if (m_pTrayMenu)
	{
		delete m_pTrayMenu;
		m_pTrayMenu = NULL;
	}
}

void iSecCloudDocHelper::on_Quit_triggered()
{
	if (m_bStarted)
	{
		int ret = QMessageBox::warning(this, tr("提示"), tr("可能有文档正在进行转换，确定要退出程序吗？")
			, QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
		if (ret == QMessageBox::No)
		{
			this->showMinimized();
			m_bQuit = false;
		}
		else
		{
			m_bQuit = true;	//标记为退出
			this->hide();
			this->close();
		}
	}
	else
	{
		m_bQuit = true;	//标记为退出
		this->hide();
		this->close();
	}
}
void iSecCloudDocHelper::on_About_triggered()
{
	this->show();
	CString szVersion = theIniFile.GetVal("Version", "Version", "");
	CString szAbout = theIniFile.GetVal("Version", "About", "");
	QString m_qsAbout = QString(tr("云打印文档转换器\n版本：%1\n%2"))
		.arg(QString::fromAscii(szVersion)).arg(QString::fromAscii(szAbout));

	QMessageBox::about(this, tr("关于"), m_qsAbout);
}

void iSecCloudDocHelper::on_Show_triggered()
{
	//恢复窗口显示
	this->show();
	this->setWindowState(Qt::WindowActive);
}


void iSecCloudDocHelper::on_SystemTrayIcon_Clicked(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
	case QSystemTrayIcon::Trigger:	//单击
		{

		}
		break;
	case QSystemTrayIcon::DoubleClick:	//双击
		{
			if (this->isVisible())
			{
				this->hide();
			}
			else
			{
				//恢复窗口显示
				this->show();
				this->setWindowState(Qt::WindowActive);
			}
		}
		break;
	default:
		break;
	}
}

void iSecCloudDocHelper::closeEvent(QCloseEvent *event)
{
	if (m_bQuit)
	{
		event->accept();
	}
	else if (m_pSystemTray)
	{
		if(m_pSystemTray->isVisible())
		{
			event->ignore();
			this->hide();
			//在系统托盘显示气泡消息提示
			m_pSystemTray->showMessage(tr("提示"), tr("云打印文档转换器正在后台运行。"), QSystemTrayIcon::Information, 5000);
		}
	}
}

void iSecCloudDocHelper::StartWork()
{
	InitWorkThread();
	m_bStarted = true;
	pushButton_start->setText(tr("停止转换"));
	//在系统托盘显示气泡消息提示
	m_pSystemTray->showMessage(tr("提示"), tr("云打印转换已开启。"), QSystemTrayIcon::Information, 5000);
}

void iSecCloudDocHelper::StopWork()
{
	ReleaseWorkThread();
	m_bStarted = false;
	pushButton_start->setText(tr("开启转换"));
	//在系统托盘显示气泡消息提示
	m_pSystemTray->showMessage(tr("提示"), tr("云打印转换已停止。"), QSystemTrayIcon::Information, 5000);
}
