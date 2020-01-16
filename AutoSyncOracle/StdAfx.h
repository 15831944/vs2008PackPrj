// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3A0F9A6B_2DFB_430B_9F35_8AB09B01579D__INCLUDED_)
#define AFX_STDAFX_H__3A0F9A6B_2DFB_430B_9F35_8AB09B01579D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER 0x501
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>


#define _SATURN_IMPORT_ADO
//#include  "..\..\..\public\include\glbdef.h"
//#include  "..\..\..\public\include\OwnerMsg.h"
#import "e:\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")
#include "Ado.h"



//--------------------------------------------
extern CString fieldValue2(CAdo &ado,LPCSTR sql);
extern BOOL searchrecord2(CAdo &ado,CString sqlexe, CStringArray *recordlist, long *columnnum, long *recordnum);
extern bool Sockcommitsql(CAdo &ado,CString sqlexe);

extern void GetPrintConfig(CString& sIP,CString& sUser,CString& sPwd,CString& sDbName,CString& sPort);
extern void GetOracleConfig(CString& sUser,CString& sPwd,CString& sDb,CString& sDataSrc,UINT& nHourSpan);
extern void GetOtherReg();
void WriteLogEx2(LPCTSTR lpszFormat, ...);

#define ENABLE_PWD_CRY
#include "..\EncryptLib\PWDCry.h"
//------------------------------------
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3A0F9A6B_2DFB_430B_9F35_8AB09B01579D__INCLUDED_)
