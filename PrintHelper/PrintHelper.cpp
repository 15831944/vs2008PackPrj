// PrintHelper.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "PrintHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CPrintHelperApp

BEGIN_MESSAGE_MAP(CPrintHelperApp, CWinApp)
END_MESSAGE_MAP()


// CPrintHelperApp ����

CPrintHelperApp::CPrintHelperApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPrintHelperApp ����

CPrintHelperApp theApp;
CFileLog theLog;
CIniFile g_oIniFile;	//add by zxl,20150825,����Ŀ����config.ini�ļ�ͳһʹ�ñ�����

// CPrintHelperApp ��ʼ��

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
