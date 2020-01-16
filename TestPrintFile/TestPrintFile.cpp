// TestPrintFile.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestPrintFile.h"
#include "../Public/Global.h"
#include "../Public/CmmLibport.h"
#include "../Public/Include/glCmdAndStructDef.h"
#include "../Public/printhelperPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;
class CPrintJob:public IPrtStationPrintCallback
{
public:
	CPrintJob(){}
	~CPrintJob(){}
public:
int OnNewPagePrinted(int a)
{
	return 1;
}
public:
	BOOL PrintTiff()
	{	
		CIOPrinter helper;
		CString szPrintJobPath;
		szPrintJobPath.Format("D:\\bj\\testdoc\\png\\background.png");
		BOOL bUsePhysicalArea = 1;
		BOOL bPrintSucc = helper.PrintTiffFile(szPrintJobPath,"testpng", this, 0, 0, bUsePhysicalArea);
		return bPrintSucc;
	}
	

};

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		CPrintJob printTest;
		printTest.PrintTiff();
		system("pause");
		
	}

	return nRetCode;
}
