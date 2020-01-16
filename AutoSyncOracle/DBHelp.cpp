// DBHelp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DBHelp.h"
#include "DBHelpDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBHelpApp

BEGIN_MESSAGE_MAP(CDBHelpApp, CWinApp)
	//{{AFX_MSG_MAP(CDBHelpApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBHelpApp construction

CDBHelpApp::CDBHelpApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDBHelpApp object

CDBHelpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDBHelpApp initialization

BOOL CDBHelpApp::InitInstance()
{
	AfxEnableControlContainer();
	HANDLE handle = CreateMutex(NULL,FALSE,"Global\\AutoSyncOracle");
	if(handle == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
	//	AfxMessageBox("只能运行服务器一个实例!");
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	//Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	::CoInitialize(NULL);
	CDBHelpDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
//==========================新的接口==================================================
CString fieldValue2(CAdo &ado,LPCSTR sql)
{
	try
	{
		return ado.FieldValue(CString(sql));
	}
	catch(...)
	{

	}
	return "";
}
BOOL searchrecord2(CAdo &ado,CString sqlexe, CStringArray *recordlist, long *columnnum, long *recordnum)
{
	try
	{
		return ado.SearchRecord(sqlexe,*recordlist,*recordnum,*columnnum);
	}
	catch(...)
	{

	}
	return false;
}
bool Sockcommitsql(CAdo &ado,CString sqlexe)
{
	try
	{
		return ado.ExecuteSql(sqlexe);
	}
	catch(...)
	{

	}
	return false;
}
//======================================================================
void WriteLogEx(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sMsg;
	sMsg.FormatV(lpszFormat, argList);
	va_end(argList);

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%02d:%02d:%02d",time.GetHour(),time.GetMinute(),time.GetSecond());

	CFile file;
	CString sTemp = strTime;
	sTemp += "  ";
	sTemp += sMsg;


	TRACE("%s\n",sTemp);

	CString szFileName;
	szFileName.Format("c:\\xlog\\isecsync_%02d-%02d_%02d.log",time.GetYear(),time.GetMonth(),time.GetDay());

	if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sTemp,sTemp.GetLength());
		file.Write("\r\n",strlen("\r\n"));
		file.Close();
	}
}

void WriteLogEx2(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sMsg;
	sMsg.FormatV(lpszFormat, argList);
	va_end(argList);

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%02d:%02d:%02d",time.GetHour(),time.GetMinute(),time.GetSecond());

	CFile file;
	CString sTemp = strTime;
	sTemp += "  ";
	sTemp += sMsg;


	TRACE("%s\n",sTemp);

	CString szFileName;
	szFileName.Format("c:\\xlog\\isecsync_orcl_%02d-%02d_%02d.log",time.GetYear(),time.GetMonth(),time.GetDay());

	if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sTemp,sTemp.GetLength());
		file.Write("\r\n",strlen("\r\n"));
		file.Close();
	}
}