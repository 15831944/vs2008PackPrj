#ifndef ISECCLOUDDOCHELPER_H
#define ISECCLOUDDOCHELPER_H

#include <QtGui/QWidget>
#include "ui_iSecCloudDocHelper.h"
#include <QSystemTrayIcon>
#include <QMenu>

class iSecCloudDocHelper : public QWidget, public Ui::iSecCloudDocHelperClass
{
	Q_OBJECT

public:
	iSecCloudDocHelper(QWidget *parent = 0, Qt::WFlags flags = 0);
	~iSecCloudDocHelper();

	void StartWork();
	void StopWork();

protected:
	void InitWorkThread();
	void ReleaseWorkThread();
	void InitSystemTray();
	void ReleaseSystemTray();

protected slots:
	void showEvent( QShowEvent *event);
	void closeEvent(QCloseEvent *event); 
	void on_pushButton_test_clicked();
	void on_pushButton_save_clicked();
	void on_pushButton_start_clicked();
	void on_CmdFromWorkThread(int nMainCmd, int nSubCmd, void* pData);
	void on_comboBox_dbtype_currentIndexChanged(int index);

	void on_Show_triggered();
	void on_About_triggered();
	void on_Quit_triggered();
	void on_SystemTrayIcon_Clicked(QSystemTrayIcon::ActivationReason reason);

private:
// 	Ui::iSecCloudDocHelperClass ui;
	CMainThread* m_pMainThread;
	QSystemTrayIcon* m_pSystemTray;
	QMenu* m_pTrayMenu;
	bool m_bQuit;
	bool m_bStarted;
};

#endif // ISECCLOUDDOCHELPER_H
