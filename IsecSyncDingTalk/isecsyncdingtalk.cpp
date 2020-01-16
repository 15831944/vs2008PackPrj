#include "stdafx.h"
#include "isecsyncdingtalk.h"


IsecSyncDingTalk::IsecSyncDingTalk(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	stackedWidget->setCurrentIndex(0);
	CString szServerIp = theIniFile.GetVal("DingTalk","ServerIP","127.0.0.1");
	CString szServerPort = theIniFile.GetVal("DingTalk","ServerPort","80");
	lineEdit_ip->setText(QString(szServerIp));
	lineEdit_port->setText(QString(szServerPort));

	m_szCorpId = theIniFile.GetVal("DingTalk","CorpId","");
	m_szCorpSecret = theIniFile.GetVal("DingTalk","CorpSecret","");
	lineEdit_CorpId->setText(QString(m_szCorpId));
	lineEdit_CorpSecret->setText(QString(m_szCorpSecret));
	m_pWorkThread = NULL;

	if (!m_pWorkThread)
	{
		m_pWorkThread = (CWorkThread*)::AfxBeginThread(RUNTIME_CLASS(CWorkThread),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if (m_pWorkThread)
		{
			HANDLE hThMsgQueOK = ::CreateEvent(0,0,0,0);
			m_pWorkThread->SetHandleOfMsgQueOK(hThMsgQueOK);
			m_pWorkThread->ResumeThread();
			DWORD dwWait = WaitForSingleObject(hThMsgQueOK, INFINITE);
			::CloseHandle(hThMsgQueOK);
			hThMsgQueOK = NULL;
		}
	}
	m_bInit = FALSE;
	LoadConfig();
}

IsecSyncDingTalk::~IsecSyncDingTalk()
{
}
void IsecSyncDingTalk::LoadConfig()
{
	CString szServerIp = theIniFile.GetVal("DingTalk","ServerIP","127.0.0.1");
	CString szServerPort = theIniFile.GetVal("DingTalk","ServerPort","80");
	int nServerPort = atoi(szServerPort.GetString());

	CString szWebserviceKey = theIniFile.GetVal("DingTalk", "WebserviceKey", WebServiceKey);
 	CString szServiceUrl = theIniFile.GetVal("DingTalk", "ServiceUrl", "");
 	if ((szServiceUrl.GetLength() <= 0) && (szServerIp.GetLength() <= 0))
 	{
 		szServiceUrl.Format(WebServiceURL);
 	}
 	else if ((szServiceUrl.GetLength() <= 0) && (szServerIp.GetLength() > 0))
 	{
 		szServiceUrl.Format(WebServiceURLFormat, szServerIp, nServerPort);
 	}
 	int nServiceTimeout = theIniFile.GetVal("DingTalk", "ServiceTimeout", WebServiceTimeout);
 	BOOL bIEProxy = theIniFile.GetVal("DingTalk", "IEProxy", FALSE);
 	CString szIEProxyAddr = theIniFile.GetVal("DingTalk", "IEProxyAddr", "");
 	int nIEProxyPort = theIniFile.GetVal("DingTalk", "IEProxyPort", 808);
	g_webservice.ConfigWebService(szWebserviceKey, szServiceUrl, nServiceTimeout, bIEProxy, szIEProxyAddr, nIEProxyPort);
	if (!g_webservice.ConnWebService())
	{
		theLog.Write("!!IsecSyncDingTalk::LoadConfig,连接服务器失败");
		return;
	}
}
void IsecSyncDingTalk::on_pushButton_test_clicked()
{
	QString qsServerIP = lineEdit_ip->text();
	QString qsServerPort = lineEdit_port->text();
	CCloudWebServiceHelper WebService;

	CString szServerIP = qsServerIP.toStdString().c_str();
	int nServerPort = atoi(qsServerPort.toStdString().c_str());
	CString szWebserviceKey = theIniFile.GetVal("DingTalk", "WebserviceKey", WebServiceKey);
	CString szServiceUrl = theIniFile.GetVal("DingTalk", "ServiceUrl", "");
	if ((szServiceUrl.GetLength() <= 0) && (szServerIP.GetLength() <= 0))
	{
		szServiceUrl.Format(WebServiceURL);
	}
	else if ((szServiceUrl.GetLength() <= 0) && (szServerIP.GetLength() > 0))
	{
		szServiceUrl.Format(WebServiceURLFormat, szServerIP, nServerPort);
	}
	int nServiceTimeout = theIniFile.GetVal("DingTalk", "ServiceTimeout", WebServiceTimeout);
	BOOL bIEProxy = theIniFile.GetVal("DingTalk", "IEProxy", FALSE);
	CString szIEProxyAddr = theIniFile.GetVal("DingTalk", "IEProxyAddr", "");
	int nIEProxyPort = theIniFile.GetVal("DingTalk", "IEProxyPort", 808);

	WebService.ConfigWebService(szWebserviceKey, szServiceUrl, nServiceTimeout, bIEProxy, szIEProxyAddr, nIEProxyPort);
	if (!WebService.ConnWebService())
	{
		QMessageBox::information(this, tr("提示"), tr("服务器连接失败，请确认域名配置是否正确！"));
		return;
	}
	m_bInit = TRUE;
	theIniFile.SetVal("DingTalk","ServerIP",szServerIP);
	theIniFile.SetVal("DingTalk","ServerPort",nServerPort);
	QMessageBox::information(this, tr("提示"), tr("连接服务器成功"));
	if (m_bInit)
	{
		stackedWidget->setCurrentIndex(0);
	}

}
void IsecSyncDingTalk::on_pushButton_config_clicked()
{
	stackedWidget->setCurrentIndex(1);
}
void IsecSyncDingTalk::on_pushButton_start_clicked()
{
	CString szCorpId = lineEdit_CorpId->text().toStdString().c_str();
	CString szCorpSecret = lineEdit_CorpSecret->text().toStdString().c_str();
	DWORD dwMsg = WM_SYNC_DING_TALK;
	WPARAM pWPARAM = (WPARAM)m_szCorpId.GetString();
	LPARAM pLPARAM = (LPARAM)m_szCorpSecret.GetString();
	int nRet = CThreadMgr::PostThreadMsg(m_pWorkThread->m_nThreadID, dwMsg, pWPARAM, pLPARAM);
	if (!nRet)
	{
		theLog.Write("!!IsecSyncDingTalk::on_pushButton_start_clicked，同步失败,m_nThreadID=%d,err=%d",m_pWorkThread->m_nThreadID,::GetLastError());
		QMessageBox::information(this, tr("提示"), tr("同步失败！！"));
	}

	theIniFile.SetVal("DingTalk","CorpId",szCorpId);
	theIniFile.SetVal("DingTalk","CorpSecret",szCorpSecret);
}
void IsecSyncDingTalk::OnUiLog(CString szLog)
{
	plainTextEdit_log->appendPlainText(QString(szLog));
	repaint();
}
void IsecSyncDingTalk::OntTipMessageBox(CString szTip)
{
	QMessageBox::information(this, tr("提示"),QString(tr(szTip)));
}