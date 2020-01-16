#include "stdafx.h"
#include "mulitscreennews.h"
#include <QtGui/QApplication>

#pragma data_seg("iSecMulitScreenNews_data") 
long g_lInstance = 0;
#pragma data_seg() 
#pragma comment(linker,"/Section:iSecMulitScreenNews_data,rws")

int main(int argc, char *argv[])
{
	if(InterlockedIncrement(&g_lInstance) > 1)
	{
		WriteLogEx("ÒÑ¾­´æÔÚ MulitScreenNews.exe = %d",g_lInstance);
		return FALSE;
	}

	QApplication a(argc, argv);
	MulitScreenNews w;
	w.show();
	int nRet = a.exec();
	InterlockedDecrement(&g_lInstance);
	return nRet;
}
