// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C9693375_3657_11D6_A165_00B0D09CB784__INCLUDED_)
#define AFX_STDAFX_H__C9693375_3657_11D6_A165_00B0D09CB784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include "Winsvc.h"
#include "../DbBackupConfig/CommonFun.h"
#include "../DbBackupConfig/DBBackFun.h"
#include "../DbBackupConfig/RegistryEx.h"
#include "SecurityTool.h"

#define SYS_CONFIG_FILE			_T("config\\config.ini")
#define PRODUCT_REGISTY_KEY		_T("SOFTWARE\\iSecStar\\iSecPrint5.5")			//×¢²á±í¼üÖµ

#import "e:\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")
#define _SATURN_IMPORT_ADO
#pragma warning(disable : 4996)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4200)
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C9693375_3657_11D6_A165_00B0D09CB784__INCLUDED_)
extern CString LoadS(UINT nID);
