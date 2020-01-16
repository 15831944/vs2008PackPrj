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

	//��ʼ��ϵͳ���̹���
	InitSystemTray();
}

iSecCloudDocHelper::~iSecCloudDocHelper()
{
	ReleaseSystemTray();
	ReleaseWorkThread();
}

void iSecCloudDocHelper::showEvent( QShowEvent * event )
{
	//��ӡ���ݿ�����
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

	//���ù��ܰ�ť
	if (m_bStarted)
	{
		pushButton_start->setText(tr("ֹͣת��"));
	}
	else
	{
		pushButton_start->setText(tr("����ת��"));
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
		QMessageBox::information(this, tr("��ʾ"), tr("���ӳɹ�."), tr("ȷ��"));
	}
	else
	{
		QMessageBox::information(this, tr("��ʾ"), tr("����ʧ��!"), tr("ȷ��"));
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
		if (QMessageBox::information(this, tr("��ʾ"), tr("�������ݿ�ʧ��,ȷ��Ҫ����������")
			, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
		{
			return;
		}
	}

	//��ӡ���ݿ�����
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

	//����QIcon���󣬲�����ͼ����Դ��ֵΪ��Ŀ����Դ�ļ���ͼ��ĵ�ַ
	QIcon iconmain(":/iSecCloudDocHelper/Resources/iSecCloudDocHelper.ico");
	QIcon iconquit(":/iSecCloudDocHelper/Resources/quit.ico");
	QIcon iconshow(":/iSecCloudDocHelper/Resources/show.ico");
	QIcon iconabout(":/iSecCloudDocHelper/Resources/about.ico");

	m_pTrayMenu = new QMenu(/*this*/);	//QSystemTrayIcon�������Ĳ˵�ʧȥ������Ȼ��ʾ������

	//Ϊ���̲˵���Ӳ˵���
	QAction* pShowAction = m_pTrayMenu->addAction(iconshow, tr("��ʾ������"));

	//Ϊϵͳ���̰󶨵����źŵĲ� ��ͼ�꼤��ʱ
	connect(pShowAction, SIGNAL(triggered()), this, SLOT(on_Show_triggered()));
	
	//Ϊ���̲˵���Ӳ˵���
	QAction* pAboutAction = m_pTrayMenu->addAction(iconabout, tr("����"));

	//Ϊϵͳ���̰󶨵����źŵĲ� ��ͼ�꼤��ʱ
	connect(pAboutAction, SIGNAL(triggered()), this, SLOT(on_About_triggered()));

 	//Ϊ���̲˵���ӷָ���
 	m_pTrayMenu->addSeparator();

	//Ϊ���̲˵���Ӳ˵���
	QAction* pQuitAction = m_pTrayMenu->addAction(iconquit, tr("�˳�"));

	//Ϊϵͳ���̰󶨵����źŵĲ� ��ͼ�꼤��ʱ
	connect(pQuitAction, SIGNAL(triggered()), this, SLOT(on_Quit_triggered()));

	//��������QIcon������Ϊϵͳ����ͼ��
	m_pSystemTray->setIcon(iconmain);

	//��ʾ����ͼ��
	m_pSystemTray->show();

	//����ϵͳ������ʾ
	m_pSystemTray->setToolTip(tr("�ƴ�ӡת������"));

	//�������˵���Ϊϵͳ���̲˵�   
	m_pSystemTray->setContextMenu(m_pTrayMenu);

// 	//��ϵͳ������ʾ������Ϣ��ʾ
// 	m_pSystemTray->showMessage(tr("��ʾ"), tr("�ƴ�ӡת�������������С�"), QSystemTrayIcon::Information, 5000);

	//Ϊϵͳ���̰󶨵����źŵĲ� ��ͼ�꼤��ʱ
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
		int ret = QMessageBox::warning(this, tr("��ʾ"), tr("�������ĵ����ڽ���ת����ȷ��Ҫ�˳�������")
			, QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
		if (ret == QMessageBox::No)
		{
			this->showMinimized();
			m_bQuit = false;
		}
		else
		{
			m_bQuit = true;	//���Ϊ�˳�
			this->hide();
			this->close();
		}
	}
	else
	{
		m_bQuit = true;	//���Ϊ�˳�
		this->hide();
		this->close();
	}
}
void iSecCloudDocHelper::on_About_triggered()
{
	this->show();
	CString szVersion = theIniFile.GetVal("Version", "Version", "");
	CString szAbout = theIniFile.GetVal("Version", "About", "");
	QString m_qsAbout = QString(tr("�ƴ�ӡ�ĵ�ת����\n�汾��%1\n%2"))
		.arg(QString::fromAscii(szVersion)).arg(QString::fromAscii(szAbout));

	QMessageBox::about(this, tr("����"), m_qsAbout);
}

void iSecCloudDocHelper::on_Show_triggered()
{
	//�ָ�������ʾ
	this->show();
	this->setWindowState(Qt::WindowActive);
}


void iSecCloudDocHelper::on_SystemTrayIcon_Clicked(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
	case QSystemTrayIcon::Trigger:	//����
		{

		}
		break;
	case QSystemTrayIcon::DoubleClick:	//˫��
		{
			if (this->isVisible())
			{
				this->hide();
			}
			else
			{
				//�ָ�������ʾ
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
			//��ϵͳ������ʾ������Ϣ��ʾ
			m_pSystemTray->showMessage(tr("��ʾ"), tr("�ƴ�ӡ�ĵ�ת�������ں�̨���С�"), QSystemTrayIcon::Information, 5000);
		}
	}
}

void iSecCloudDocHelper::StartWork()
{
	InitWorkThread();
	m_bStarted = true;
	pushButton_start->setText(tr("ֹͣת��"));
	//��ϵͳ������ʾ������Ϣ��ʾ
	m_pSystemTray->showMessage(tr("��ʾ"), tr("�ƴ�ӡת���ѿ�����"), QSystemTrayIcon::Information, 5000);
}

void iSecCloudDocHelper::StopWork()
{
	ReleaseWorkThread();
	m_bStarted = false;
	pushButton_start->setText(tr("����ת��"));
	//��ϵͳ������ʾ������Ϣ��ʾ
	m_pSystemTray->showMessage(tr("��ʾ"), tr("�ƴ�ӡת����ֹͣ��"), QSystemTrayIcon::Information, 5000);
}
