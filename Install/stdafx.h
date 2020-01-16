
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#include "Resource.h"
#include <afxsock.h>		// MFC socket extensions

#include <atlsecurity.h>
#include "../Public/Global.h"
#include "../Public/Include/glCmdAndStructDef.h"
#include "../public/Include/NetCmdAndStructDef.h"
#include "../Public/Include/MultiLanguageDef.h"	//add by zfq,2013.10.22,动态切换语言
#include <afxdlgs.h>
#include "commonfun.h"
#include "FileLog.h"
#include "ado.h"
#include "DbInstaller.h"
#include "IISWeb.h"
#include "IISFTP.h"
#include "RegistryEx.h"
#include "winfirewallop.h"
#define PLT_DB_NAME		_T("iSec_Platform")
#define PRINT_DB_NAME	_T("iSec_Print")
#define PRINT_WEB_SITE	_T("iSec_Print_Web_Site")
#define VIRTUALNAME		_T("XabPrint")
#define SCAN_DIR		_T("ScanDir")
#define PRINT_DIR		_T("PrintDir")
#define TRANSCRIPT_DIR	_T("TranscriptDir")
#define DB_SVC_NAME		_T("DbSvcHost")
#define PRTSTATION_DB_NAME    _T("iSec_PrinterAPP")

#define PRINT_SVC_NAME _T("iSecASvcHost")
#define PRINT_SVC_EXE _T("iSecASvcHost.exe")


#define PRINT_EDU_SVC_NAME _T("iSecBSvcHost")
#define PRINT_EDU_SVC_EXE _T("iSecBSvcHost.exe")

#define INTRANET_SVC_NAME _T("iSecCSvcHost")
#define INTRANET_SVC_EXE _T("iSecCSvcHost.exe")

#define PRODUCT_CUSTOM_REGISTY_KEY_AMD64	_T("SOFTWARE\\Wow6432Node\\iSecStar") 
#define PRODUCT_CUSTOM_REGISTY_KEY			_T("SOFTWARE\\iSecStar") 


extern CFileLog theLog;
extern BOOL MyExecuteCmdComd();
extern BOOL Enable32BitAppOnWin64(BOOL bEnable);
extern BOOL MyStartAspnet_StateService();

extern CString LoadS(int nText);
extern BOOL IsWow64();
extern void Install(CString sPath,CString sParam,DWORD dwWait /* = TRUE*/,DWORD dwShowCmd);

#define MBI(x) ::MessageBox(GetSafeHwnd(), LoadS(x),LoadS(IDS_TIP), MB_OK|MB_ICONINFORMATION)
#define MBW(x) ::MessageBox(GetSafeHwnd(), LoadS(x),LoadS(IDS_TIP), MB_OK|MB_ICONWARNING)
#define MBHI(x, y) ::MessageBox(x, LoadS(y),LoadS(IDS_TIP), MB_OK|MB_ICONINFORMATION)
#define MBHW(x, y) ::MessageBox(x, LoadS(y),LoadS(IDS_TIP), MB_OK|MB_ICONWARNING)
#define MBYN(x) ::MessageBox(GetSafeHwnd(), x,LoadS(IDS_TIP), MB_YESNO|MB_ICONQUESTION)
#define MBHYN(x, y) MessageBox(x, LoadS(y),LoadS(IDS_TIP), MB_YESNO|MB_ICONQUESTION)

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define ENABLE_PWD_CRY

#include "../encryptlib/PWDCry.h"
#include <afxdhtml.h>
// #ifdef _DEBUG
// #pragma comment(lib,"../debug/encryptlib.lib")
// #else
// #pragma comment(lib,"../release/encryptlib.lib")
// #endif

//FTP用户名和密码
#define FTP_USERNAME					"isecftpuser"
#define FTP_PASSWORD					"123456"

extern BOOL AddObjectAccess(CString strFileName, const CSid &rSid, ACCESS_MASK accessmask,SE_OBJECT_TYPE eType /*= SE_OBJECT_TYPE*/);
extern BOOL AddRegAccess(CString szRegPath);
extern CString LoadS(UINT nID);

extern CIniFile g_oSetupConfig;
extern CIniFile g_oIniFileConfig;
extern CIniFile g_oIniFileSet;
extern CIniFile g_oIniFileCurretnConfig;	//config_X.ini
extern CIniFile g_oIniFileClientSetup;

#ifndef CP_GB2312
#define  CP_GB2312	936	//Code Page Identifiers ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
#endif
