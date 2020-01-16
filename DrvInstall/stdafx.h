// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
#define  _BIND_TO_CURRENT_VCLIBS_VERSION 1
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _AMD64_
#include <newdev.h>
#pragma comment(lib,"Newdev.lib")
#pragma message("Newdev on _AMD64_")
#else
#include "newdev.h"
#pragma comment(lib,"Newdevx32.lib")
#endif


#pragma warning(disable : 4996)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4200)

#include <winsvc.h>
#include <SetupAPI.h>
#include "SetupEx.h"
#include "RegistryEx.h"
#include "DriverOp.h"
#include "../Public/Global.h"
#include "../Public/VirtualDriverDef.h"
extern void WriteLogEx(LPCTSTR lpszFormat, ...);
extern BOOL MfgCallback(CInf* inf, const CString& mfgname, const CString& modelname, PVOID context, DWORD& code);
extern BOOL DeviceCallback(CInf* inf, const CString& devname, const CString& instsecname, const CStringArray& devid, PVOID context, DWORD& code);

extern int RemoveHardware(LPCTSTR lpszHardwareID);
extern void RegDriverStopState();
extern void RegUnloadInf(LPCTSTR lpszRunOnceInfo,LPCTSTR lpszExePath);
extern BOOL IsWow64();
extern BOOL IsWinVistaOrLater();
extern void StringSplit(const CString sSource, CStringArray *parArray , CString sSplitter,BOOL bTriNUll);

#include "IniFile.h"

extern CIniFile theIniFile;

#ifndef CP_GB2312
#define  CP_GB2312	936	//Code Page Identifiers ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
#endif
