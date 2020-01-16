// FileLog.cpp: implementation of the CFileLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFileLog::CFileLog()
{
	m_sLogPath = _T("");
	m_pLogCrit = NULL;
}

CFileLog::~CFileLog()
{
	if(m_pLogCrit)
	{
		DeleteCriticalSection(m_pLogCrit);
		m_pLogCrit = NULL;
	}	
}

void CFileLog::Write(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	Write(sTmp);
}

void CFileLog::Write(CString sLog)
{
	if( sLog.GetLength() > 250 )
		TRACE(sLog.Left(250));
	else
		TRACE(sLog);
	TRACE(_T("\n"));
	
	if(!IsDebug())
		return;
		
	try
	{
		CFile fileLog;
		if(!fileLog.Open(m_sLogPath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
			return;
		
		CTime time = CTime::GetCurrentTime();
		CString sTime = time.Format( _T("%Y-%m-%d %H:%M:%S") );
		CString sTmp;
		
		sTmp.Format(_T("%s,	 %s"),sTime,sLog);
		sTmp += _T("\r\n");
		fileLog.SeekToEnd();
		fileLog.Write(sTmp,sTmp.GetLength());
		fileLog.Close();
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}	
}

void CFileLog::WriteW(LPWSTR sLog)
{
	if( !IsDebug() )
		return;

	CString sSysDir = CCommonFun::GetLogDir();

	CFileFind fd;

	if( !fd.FindFile(sSysDir) )
	{
		if( !::CreateDirectory(sSysDir,NULL) )
			return;
	}

	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format(_T("%y%m%d"));
	try
	{
		CFile fileLog;
		if(!fileLog.Open(m_sLogPath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
			return;

		CString sTime = time.Format( _T("%Y-%m-%d %H:%M:%S") );
		CString sTmp;

		sTmp.Format(_T("%s,	 "),sTime);
		fileLog.SeekToEnd();

		fileLog.Write(sTmp,sTmp.GetLength());
		fileLog.Write(sLog, wcslen(sLog));
		fileLog.Write(_T("\r\n"), 2);
		
		fileLog.Close();
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}

}
BOOL CFileLog::IsDebug()
{
	return TRUE;
}

void CFileLog::SetLogName(CString sName)
{
	ASSERT(!sName.IsEmpty());
	CString sSysDir = CCommonFun::GetLogDir();

	CFileFind fd;

	if(!fd.FindFile(sSysDir))
	{
		if(!::CreateDirectory(sSysDir,NULL))
			return;
	}

	m_sLogPath.Format(_T("%s\\%s.Log"),sSysDir,sName);
}

void CFileLog::Read(BYTE*& pData, int& nLen,CString sLogName)
{
	try
	{
		CFile fileLog;
		if(!fileLog.Open(sLogName,CFile::modeRead))
			return;
		if (!pData && nLen == 0)
		{
			nLen = fileLog.GetLength();
			pData = new BYTE[nLen];

		}

		fileLog.Read(pData,nLen);
		fileLog.Close();
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}
}

void CFileLog::Write(BYTE *pData, int nLen,CString sLogName)
{
	try
	{
		CFile fileLog;
		if(!fileLog.Open(sLogName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
			return;
		
	
		fileLog.SeekToEnd();
		fileLog.Write(pData,nLen);
		fileLog.Close();
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}
}

void CFileLog::DbgPrint(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	OutputDebugString(sTmp);
}

void CFileLog::Writeld(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	WriteEx(1, sTmp);
}

void CFileLog::Writeli(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	WriteEx(2, sTmp);
}

void CFileLog::Writelw(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	WriteEx(4, sTmp);
}

void CFileLog::Writele(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	WriteEx(8, sTmp);
}

void CFileLog::WriteEx(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);
	WriteEx(2, sTmp);
}

void CFileLog::WriteEx(int nlevel, CString slog)
{
	if(!IsDebug())
		return;

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	CString shead;
	shead.Format(_T("[%s][%03x.%03x][%02d:%02d:%02d.%03d] "), LevelToStr(nlevel), 
		GetCurrentProcessId(), GetCurrentThreadId(),
		curTime.wHour,curTime.wMinute,curTime.wSecond,curTime.wMilliseconds);
	CString sline = shead+slog;

	CFile file;
	if(m_pLogCrit)
		EnterCriticalSection(m_pLogCrit);
	if(file.Open(m_sLogPath, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sline,sline.GetLength());
		file.Write(_T("\r\n"),_tcslen(_T("\r\n"))*sizeof(TCHAR));
		file.Close();
	}

	if(m_pLogCrit)
		LeaveCriticalSection(m_pLogCrit);
}

CString CFileLog::LevelToStr(int nLevel)
{
	if(nLevel == 1)
		return _T("d");
	else if(nLevel == 2)
		return _T("i");
	else if(nLevel == 4)
		return _T("w");
	else if(nLevel == 8)
		return _T("e");
	else
		return _T("u");
}

void CFileLog::InitCritSection()
{
	if(!m_pLogCrit)
	{
		InitializeCriticalSection(&m_logCrit);
		m_pLogCrit = &m_logCrit;
	}
}