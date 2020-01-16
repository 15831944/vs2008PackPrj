// PrintHelper.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "PrintHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CPrintHelperApp

BEGIN_MESSAGE_MAP(CPrintHelperApp, CWinApp)
END_MESSAGE_MAP()


// CPrintHelperApp 构造

CPrintHelperApp::CPrintHelperApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPrintHelperApp 对象

CPrintHelperApp theApp;
CFileLog theLog;
CIniFile g_oIniFile;	//add by zxl,20150825,本项目操作config.ini文件统一使用本变量

// CPrintHelperApp 初始化

BOOL CPrintHelperApp::InitInstance()
{
	CWinApp::InitInstance();

	theLog.SetLogName("prthelper");
	g_oIniFile.m_sPath.Format("%s\\%s", CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);

	return TRUE;
}

extern "C" __declspec(dllexport) void WINAPI  SetPrintHelperBindPrinter(char* pszBindPrinter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString szBindPrinter(pszBindPrinter);
	if (szBindPrinter.GetLength() > 0)
	{
		g_oIniFile.m_sPath.Format("%s%s\\config_%s.ini",CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR,szBindPrinter);
		theLog.SetLogName("prthelper_" + szBindPrinter);
	}
	else
	{
		theLog.SetLogName("prthelper");
		g_oIniFile.m_sPath.Format("%s%s",CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
	}
}
