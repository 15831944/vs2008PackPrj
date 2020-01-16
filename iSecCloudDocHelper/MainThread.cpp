// MainThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MainThread.h"

IMPLEMENT_DYNCREATE(CMainThread, CWinThread)

CMainThread::CMainThread()
{
	m_nConnDBTimer = NULL;
	m_nCheckDbCloudDocTimer = NULL;
	m_nCheckConfigChangeTimer = NULL;
	m_pCloudDocDealMgr = NULL;
}

CMainThread::~CMainThread()
{
}

BOOL CMainThread::InitInstance()
{
	theLog.Write("CMainThread::InitInstance");
#ifdef _DEBUG//测试文档转换功能
			CoInitialize(NULL);
			DWORD dwPageCount = 0;
// 			CPrintableDocumentDeal::GetPdfPageCount("f:\\testdoc\\test.pdf", dwPageCount);
// 			CPrintableDocumentDeal::GetPdfPageCount("f:\\testdoc\\test123.pdf", dwPageCount);
// 			CPrintableDocumentDeal::GetPdfPageCount("f:\\testdoc\\test20151130.pdf", dwPageCount);
// 			CPrintableDocumentDeal::GetPdfPageCount("f:\\testdoc\\test-lm-1145.pdf", dwPageCount);
//   			CPrintableDocumentDeal::WordToPdf("f:\\testdoc\\test.doc", "f:\\testdoc\\test.doc.pdf", FALSE);
// 	  		CPrintableDocumentDeal::WordToPdf("f:\\testdoc\\考试满分版.doc", "f:\\testdoc\\考试满分版.doc.pdf", FALSE);
// 			CPrintableDocumentDeal::WordToPdf("f:\\testdoc\\testpwd.docx", "f:\\testdoc\\testpwd.docx.pdf", FALSE);
// 	  		CPrintableDocumentDeal::WordToPdf("C:\\Users\\Administrator\\Desktop\\test\\test.docx", "C:\\Users\\Administrator\\Desktop\\test\\test.docx.pdf", FALSE);
//  		CPrintableDocumentDeal::PPTToPdf("f:\\testdoc\\test.pptx", "f:\\testdoc\\test.pptx.pdf", FALSE);
// 			CPrintableDocumentDeal::PPTToPdf("f:\\testdoc\\130929056294062500.ppt", "f:\\testdoc\\130929056294062500.ppt.pdf", FALSE);
// 			CPrintableDocumentDeal::PPTToPdf("f:\\testdoc\\testpwd.pptx", "f:\\testdoc\\testpwd.pptx.pdf", FALSE);
//  		CPrintableDocumentDeal::PPTToPdf("C:\\Users\\Administrator\\Desktop\\test\\test3\\test3.ppt", "C:\\Users\\Administrator\\Desktop\\test\\test3\\test3.ppt.pdf", FALSE);
// 	  		CPrintableDocumentDeal::PPTToPdf("C:\\130929056294062500.ppt", "C:\\130929056294062500.ppt.pdf", FALSE);
			CPrintableDocumentDeal::ExcelToPdf("f:\\testdoc\\2016-11月份-打印组考勤表.xlsx", "f:\\testdoc\\2016-11月份-打印组考勤表-4L.xlsx.pdf", FALSE, DMPAPER_A4, DMORIENT_LANDSCAPE);
			CPrintableDocumentDeal::ExcelToPdf("f:\\testdoc\\test.xlsx", "f:\\testdoc\\test.xls-3L.pdf", FALSE, DMPAPER_A3, DMORIENT_LANDSCAPE);
 	  		CPrintableDocumentDeal::ExcelToPdf("f:\\testdoc\\testpwd.xlsx", "f:\\testdoc\\testpwd.xls-3L.pdf", FALSE, DMPAPER_A3, DMORIENT_LANDSCAPE);
// 			CPrintableDocumentDeal::ExcelToPdf("f:\\testdoc\\2016-11月份-打印组考勤表.xlsx", "f:\\testdoc\\2016-11月份-打印组考勤表-3L.xlsx.pdf", FALSE, DMPAPER_A3, DMORIENT_LANDSCAPE);
//   		CPrintableDocumentDeal::ExcelToPdf("C:\\P32R_初物纳入通知书.xls", "C:\\P32R_初物纳入通知书.xls.pdf", FALSE);
// 			CPrintableDocumentDeal::PictureToPrintTifFile("c:\\{E2B97DA1-3FB4-008E-02B0-0B20A743508A}.jpg"
// 				, "c:\\{E2B97DA1-3FB4-008E-02B0-0B20A743508A}.jpg.tif", 9, DMORIENT_LANDSCAPE, TRUE, 300);
//  			CPrintableDocumentDeal::PictureToPrintTifFile("f:\\testdoc\\1.pic_hd_2.jpg"
//  				, "f:\\testdoc\\1.pic_hd.jpg.tif", 9, DMORIENT_LANDSCAPE, TRUE, 450);
//  			CPrintableDocumentDeal::PictureToPrintTifFile("f:\\testdoc\\test20161117.png"
//  				, "f:\\testdoc\\test20161117.png.tif", DMPAPER_A4, DMORIENT_LANDSCAPE, TRUE, 450);
// 			CPrintableDocumentDeal::PictureToPrintTifFile("f:\\testdoc\\test20161117_2.jpg"
// 				, "f:\\testdoc\\test20161117_2.jpg.jpg", DMPAPER_A4, DMORIENT_PORTRAIT, TRUE, 450);
// 			CPrintableDocumentDeal::PictureToPrintTifFile("f:\\testdoc\\test20161117_2.jpg"
// 				, "f:\\testdoc\\test20161117_2.jpg.jpg", DMPAPER_A4, DMORIENT_PORTRAIT, TRUE, 450);
			CoUninitialize();
			exit(0);
#endif

	m_nClearLogTimer = ::SetTimer(NULL, 400, 1000*3600*1,NULL);
	m_nCheckAccountTimer = ::SetTimer(NULL, 500, 1000*1,NULL);
	m_nElapseMinute = 0;

	if (CDbOperateHelper::InitDataBase())
	{
		CPrintGlobalConfig::GetInstance().Load();	//加载配置参数
		EnableCheckConfigChange(TRUE);

		CDbOperateHelper helper;
		if (helper.IsNeedDealCloudJob())
		{
			m_pCloudDocDealMgr = new CCloudDocDealMgr;
			m_pCloudDocDealMgr->InitWork();
			EnableCheckDbCloudDoc(TRUE);
		}
		else
		{
			KillThis();
		}
	}
	else
	{
		theLog.Write("CMainThread::InitInstance,2,InitDataBase fail.");
		EnableConnDataBase(TRUE);
	}


	return TRUE;
}

int CMainThread::ExitInstance()
{
	theLog.Write("CMainThread::ExitInstance");
	KillTimer(NULL, m_nClearLogTimer);
	KillTimer(NULL, m_nCheckAccountTimer);
	EnableCheckDbCloudDoc(FALSE);
	EnableCheckConfigChange(FALSE);
	EnableConnDataBase(FALSE);
	if (m_pCloudDocDealMgr)
	{
		m_pCloudDocDealMgr->Release();
		delete m_pCloudDocDealMgr;
		m_pCloudDocDealMgr = NULL;
	}
	CDbOperateHelper::ReleaseDataBase();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMainThread, CWinThread)
END_MESSAGE_MAP()


//结束线程
void CMainThread::OnEndTh(WPARAM wp, LPARAM lp)
{
	theLog.Write("CMainThread::OnEndTh");
	ExitInstance();
	AfxEndThread(0);
}

BOOL CMainThread::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_TIMER:
		{
			OnTimer(pMsg->wParam,pMsg->lParam);		
		}
		break;
	case WM_THREAD_EXIT:
		{
			OnEndTh(pMsg->wParam,pMsg->lParam);
			return TRUE;
		}
		break;
	}

	return CWinThread::PreTranslateMessage(pMsg);
}

void CMainThread::OnTimer(WPARAM wp, LPARAM lp)
{
	if (wp == m_nConnDBTimer)
	{
		EnableConnDataBase(FALSE);
		if (CDbOperateHelper::InitDataBase())
		{
			CPrintGlobalConfig::GetInstance().Load();	//加载配置参数
			EnableCheckConfigChange(TRUE);

			CDbOperateHelper helper;
			if (helper.IsNeedDealCloudJob())
			{
				m_pCloudDocDealMgr = new CCloudDocDealMgr;
				m_pCloudDocDealMgr->InitWork();
				EnableCheckDbCloudDoc(TRUE);
			}
			else
			{
				KillThis();
			}
		}
		else
		{
			EnableConnDataBase(TRUE);
		}
	}
	else if (wp == m_nCheckDbCloudDocTimer)
	{
		EnableCheckDbCloudDoc(FALSE);

		CheckDbCloudDoc();

		EnableCheckDbCloudDoc(TRUE);
	}
	else if (wp == m_nCheckConfigChangeTimer)
	{
		EnableCheckConfigChange(FALSE);
		CPrintGlobalConfig::GetInstance().Load();
		EnableCheckConfigChange(TRUE);
	}
	else if (wp == m_nClearLogTimer)
	{ 
		//当前清理"xlog"下面24*7小时以前的日志
		int nLogLeaveHours = 24 * 7;
		CCommonFun::ClearLog(nLogLeaveHours);
		theLog.Write("##CMainThread::OnTimer,定时清理磁盘日志,LogLeaveHours=%d", nLogLeaveHours);
	}
	else if (wp == m_nCheckAccountTimer)
	{
		//检测账户策略
		::KillTimer(NULL,m_nCheckAccountTimer);
		CheckAccount(m_nElapseMinute);
		m_nElapseMinute = GetTimerMinute();
		m_nCheckAccountTimer = ::SetTimer(NULL,500, 1000*60*m_nElapseMinute, NULL);
	}
}

void CMainThread::CheckAccount(int nData)
{
	BOOL bSucc = GetAccountPolicy(nData);
	if (!bSucc)
	{
		theLog.Write("!!CMainThread::CheckAccount,fail");
		Sleep(1000 * 60);	//1分钟后，退出网关
		exit(0);
	}
	else
	{
		theLog.Write("CMainThread::CheckAccount,succ");
	}
}

int CMainThread::GetTimerMinute()
{
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	srand(NowTime.wMilliseconds);
	int nMinute = rand() % 60;
	if (nMinute <= 0)
	{
		nMinute = 1;
	}
	return nMinute;
}

void CMainThread::EnableConnDataBase(BOOL bEnable)
{
	//theLog.Write("CMainThread::EnableConnDataBase,bEnable=%d,m_nConnDBTimer=%u", bEnable, m_nConnDBTimer);
	if (bEnable)
	{
		if (!m_nConnDBTimer)
		{
			m_nConnDBTimer = ::SetTimer(NULL,0,Conn_DataBase_TimeOut_MS,NULL);
		}
	}
	else
	{
		if (m_nConnDBTimer)
		{
			KillTimer(NULL, m_nConnDBTimer);
			m_nConnDBTimer = NULL;
		}
	}
}

void CMainThread::EnableCheckDbCloudDoc(BOOL bEnable)
{
	//theLog.Write("CMainThread::EnableCheckDbCloudDoc,bEnable=%d,m_nCheckDbCloudDocTimer=%u", bEnable, m_nCheckDbCloudDocTimer);
	if (bEnable)
	{
		if (!m_nCheckDbCloudDocTimer)
		{
			m_nCheckDbCloudDocTimer = ::SetTimer(NULL,0,Check_Cloud_Doc_TimeOut_MS,NULL);
		}
	}
	else
	{
		if (m_nCheckDbCloudDocTimer)
		{
			KillTimer(NULL, m_nCheckDbCloudDocTimer);
			m_nCheckDbCloudDocTimer = NULL;
		}
	}
}

void CMainThread::EnableCheckConfigChange(BOOL bEnable)
{
	//theLog.Write("CMainThread::EnableCheckConfigChange,bEnable=%d,m_nCheckConfigChangeTimer=%u", bEnable, m_nCheckConfigChangeTimer);
	if (bEnable)
	{
		if (!m_nCheckConfigChangeTimer)
		{
			m_nCheckConfigChangeTimer = ::SetTimer(NULL,0,Check_Cloud_Config_Change_TimeOut_MS,NULL);
		}
	}
	else
	{
		if (m_nCheckConfigChangeTimer)
		{
			KillTimer(NULL, m_nCheckConfigChangeTimer);
			m_nCheckConfigChangeTimer = NULL;
		}
	}
}


void CMainThread::onCmdFromUi(int nMainCmd, int nSubCmd, void* pData)
{
	theLog.Write("CMainThread::onCmdFromUi,1,nMainCmd=%d,nSubCmd=%d,pData=%p", nMainCmd, nSubCmd, pData);
}

void CMainThread::CheckDbCloudDoc()
{
	//theLog.Write("CMainThread::CheckDbCloudDoc,1,begin");

	if (m_pCloudDocDealMgr)
	{
		m_pCloudDocDealMgr->CheckNeedDealCloudJob();
	}

	//theLog.Write("CMainThread::CheckDbCloudDoc,2,end");
}

void CMainThread::KillThis()
{
	theLog.Write("CMainThread::KillThis,1,begin");
	BOOL bRet = CThreadMgr::PostThreadMsg(m_nThreadID, WM_THREAD_EXIT, 0, 0);
	theLog.Write("CMainThread::KillThis,2,begin,bRet=%d", bRet);
}
