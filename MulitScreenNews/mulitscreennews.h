#ifndef MULITSCREENNEWS_H
#define MULITSCREENNEWS_H

#include <QtGui/QMainWindow>
#include "ui_mulitscreennews.h"

class MulitScreenNews : public QMainWindow, public Ui::MulitScreenNewsClass
{
	Q_OBJECT

public:
	MulitScreenNews(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MulitScreenNews();

private:
	int m_nFromScreenIndex;
	int m_nToScreenIndex;
};

#endif // MULITSCREENNEWS_H
