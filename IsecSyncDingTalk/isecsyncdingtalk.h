#ifndef ISECSYNCDINGTALK_H
#define ISECSYNCDINGTALK_H

#include <QtGui/QWidget>
#include "ui_isecsyncdingtalk.h"


class IsecSyncDingTalk : public QWidget,public Ui::IsecSyncDingTalkClass
{
	Q_OBJECT

public:
	IsecSyncDingTalk(QWidget *parent = 0);
	~IsecSyncDingTalk();
protected:
	CWorkThread *m_pWorkThread;
private:
	BOOL m_bInit;
	CString m_szCorpId;
	CString m_szCorpSecret;
private slots:
	void on_pushButton_test_clicked();
	void on_pushButton_config_clicked();
	void on_pushButton_start_clicked();
public:
	void OnUiLog(CString szLog);
	void OntTipMessageBox(CString szTip);
protected:
	void LoadConfig();


};

#endif // ISECSYNCDINGTALK_H
