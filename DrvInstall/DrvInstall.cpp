// DrvInstall.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DrvInstall.h"

#include "InstallDispDrv.h"
#include "PrinterInstall.h"
#include "NdisInstall.h"


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

// CDrvInstallApp

BEGIN_MESSAGE_MAP(CDrvInstallApp, CWinApp)
END_MESSAGE_MAP()


// CDrvInstallApp ����

CDrvInstallApp::CDrvInstallApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CIniFile theIniFile;

// Ψһ��һ�� CDrvInstallApp ����

CDrvInstallApp theApp;


// CDrvInstallApp ��ʼ��

BOOL CDrvInstallApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CInstallMirrDisp   g_Install;

BOOL IsWinVistaOrLater()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if( osv.dwMajorVersion >= 6)
	{
		return TRUE;
	}
	return FALSE;
}
void WriteLog(LPCTSTR szMsg,LPCTSTR szFileName)
{
	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime = time.Format("%Y-%m-%d  %H:%M:%S");

	CFile file;
	CString sTemp = strTime;
	sTemp += "----->>>";
	sTemp += szMsg;
	if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sTemp,sTemp.GetLength());
		file.Write("\r\n",strlen("\r\n"));
		file.Close();
	}
}
//��ȡ��־Ŀ¼
CString& GetLogDir()
{
	static CString g_szLogDir = "";
	if (g_szLogDir.IsEmpty())
	{
		OutputDebugString("##CCommonFun::GetLogDir");

		TCHAR path[MAX_PATH];
		CString sDefaultPath;
		if(GetModuleFileName(0,path,MAX_PATH))
		{
			TCHAR *pdest;
			pdest=_tcsrchr(path,_T('\\'));
			if(pdest) 
				*pdest=0;
			sDefaultPath=path;
			sDefaultPath += "\\xlog";
		}

		if (sDefaultPath.IsEmpty())
		{
			sDefaultPath = ".\\xlog";
		}

		g_szLogDir = sDefaultPath;
	}

	return g_szLogDir;
}
void WriteLogEx(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);

	TRACE("%s\n",sTmp);

	CString szLogFilePath;
	szLogFilePath.Format("%s\\Install.log", GetLogDir());

	WriteLog(sTmp,szLogFilePath);
}
//�л�������������Կ�����
DWORD WINAPI MirrorHelpThread(void* pVoid)
{
	HDESK   hDesk =  OpenInputDesktop(DF_ALLOWOTHERACCOUNTHOOK, 0, DESKTOP_READOBJECTS | GENERIC_ALL);   
	DWORD   dwLength = 0; 
	if(hDesk)
	{
		BOOL b = SetThreadDesktop(hDesk);
		WriteLogEx("setthreaddesktop %d,err=%d",b,::GetLastError());
	}
	else
	{
		WriteLogEx("MirrorHelpThread openinputddesk faile");
		return 0;
	}
	CInstallMirrDisp cls;
	if(cls.StartIoDrv())
	{
		cls.GetSysImortFuncAddr(true);
		cls.StartMirror(true);
		cls.DelTempFile();
		return true;
	}

	return 0;
}

bool __declspec(dllexport) StartMirror2()
{

	HANDLE hThread = NULL;
	DWORD dwThreadId = 0;
	hThread = ::CreateThread(0,0,MirrorHelpThread,0,0,&dwThreadId);
	if(hThread)
	{
		DWORD dwRet = ::WaitForSingleObject(hThread,3000);
		WriteLogEx("MirrorHelpThread wait = %d",dwRet);
		return true;
	}
	else
	{
		WriteLogEx("MirrorHelpThread create faile");
	}

#if 0
	CInstallClass cls;
	if(cls.StartWintMars())
	{
		cls.GetSysImortFuncAddr(true);
		cls.StartMirror(true);
		cls.DelTempFile();
		return true;
	}
#endif

	return false;
}
bool __declspec(dllexport) StopMirror2()
{
	CInstallMirrDisp cls;
	cls.StartMirror(false);
	return true;
}



bool __declspec(dllexport) InstallHookDisp(char * pCurrPath)
{
	char pPath[MAX_PATH];
	strcpy(pPath,pCurrPath);
	int ilen=strlen(pPath);
	if(pPath[ilen-1]=='\\')
	{
		pPath[ilen-1]=0;
	}
	WriteLogEx("CurPath = %s,%s",pCurrPath,pPath);
	CInstallHookDisp io;
	io.UpdateDrvFile(pPath);
	io.InstallDisplayDrv("ASSC",TRUE);
	io.InstallOpenGLDrv("ASGL",TRUE);
	return true;
}

bool __declspec(dllexport) UnInstallHookDisp(char * pCurrPath)
{
	CInstallHookDisp io;
	io.InstallDisplayDrv("ASSC",FALSE);
	io.InstallOpenGLDrv("ASGL",FALSE);
	return true;
}

bool __declspec(dllexport) InstallMirrDisp(char * pCurrPath)
{
	char pPath[MAX_PATH];
	strcpy(pPath,pCurrPath);
	int ilen=strlen(pPath);
	if(pPath[ilen-1]=='\\')
	{
		pPath[ilen-1]=0;
	}

	g_Install.m_strLocalPath.Format("%s",pPath);   
	g_Install.InstallDisp();
	g_Install.InstallIoDrv();
	g_Install.StartIoDrv();

	//g_Install.WriteAskReg(InstInfo);

	Sleep(500);

	return true;
}

bool __declspec(dllexport) UnInstallMirrDisp(char * pCurrPath)
{
	char pPath[MAX_PATH];
	strcpy(pPath,pCurrPath);
	int ilen=strlen(pPath);
	if(pPath[ilen-1]=='\\')
	{
		pPath[ilen-1]=0;
	}
	g_Install.m_strLocalPath.Format("%s",pPath);
	//g_Install.RemoveAskReg(InstInfo);

	if(g_Install.bAskInstAsSoftAllRemove())
	{
		WriteLogEx("��ʼж��");
		g_Install.StopDisp();
		::Sleep(1000);
		g_Install.UninstallDisp();
		g_Install.UninstallIoDrv();
		g_Install.DelAllDispFile();
	}
	else
	{
		WriteLogEx("�����������ʹ�ñ������Կ�,����ж�������Կ�����ִ��!");
	}
	return true;
}
//��װndis
bool __declspec(dllexport) InstallNdis(char* pCurPath)
{
	CNdisInstall ndsi;
	ndsi.Install(1);
	return true;
}

bool __declspec(dllexport) UnInstallNdis(char* pCurPath)
{
	CNdisInstall ndsi;
	ndsi.UnInstall(1);
	return true;
}
//��װ�����ӡ��
bool __declspec(dllexport) InstallPrinter(char* pCurPath)
{
	WriteLogEx("InstallPrinter,1,pCurPath=%s", pCurPath);

	CPrinterInstallHelper prt;
	prt.SetDefaultPath(pCurPath);
	prt.Install();
	return true;
}

bool __declspec(dllexport) UnInstallPrinter(char* pCurPath)
{
	CPrinterInstallHelper prt;
	prt.SetDefaultPath(pCurPath);
	prt.Uninstall();
	return true;
}

bool __declspec(dllexport) ConfigPrinter(char* pCurPath)
{
	CPrinterInstallHelper prt;
	prt.SetDefaultParam();
	return true;
}