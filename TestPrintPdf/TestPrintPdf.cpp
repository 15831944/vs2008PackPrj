// TestPrintPdf.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestPrintPdf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "../Public/Global.h"
#include "../Public/CmmLibport.h"
#include "../Public/Include/glCmdAndStructDef.h"
#include "../Public/printhelperPort.h"




// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

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
		CIOPrinter helper;
		
		BOOL bRet = helper.PrintPdfFile("D:\\bj\\testdoc\\pdf\\test1.pdf", job->doc_name().c_str(), this, nFrom, nTo);

	}

	return nRetCode;
}
