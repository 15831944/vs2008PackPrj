// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果必须将位于下面指定平台之前的平台作为目标，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <atlbase.h>
#include <winioctl.h>
#include <comdef.h>
#include <winsock2.h>
//#pragma comment(lib,"Ws2_32.lib")
#include <AfxSock.h>

#include <winsvc.h>
#define _AA_DLL_
#include "../Public/Global.h"
//#include "../Public/Include/glCmdAndStructDef.h"
//#include "../Public/Include/CmmInterface.h"
#include "CommonFun.h"
#include "Registry2.h"

#define DEF_UNDER_ACTION_MANAGE
// 如果是行为管理的情况下调用
#ifdef DEF_UNDER_ACTION_MANAGE
#include "../ActionResource/resource.h"
#else
//安密
#ifdef DEF_UNDER_SOREN
#include "../SorenResource/resource.h"
#endif
#endif


#define SERVICE_ACCEPT_SESSIONCHANGE           0x00000080
#define SERVICE_CONTROL_SESSIONCHANGE          0x0000000E
#define NOTIFY_FOR_ALL_SESSIONS		1
#define NOTIFY_FOR_THIS_SESSION		0
#include "FileLog.h"
#include "ThreadMgr.h"
#include "processexp.h"
extern CFileLog theLog;
struct tag_TButton_Text
{
	char szTxt[MAX_PATH];
};

extern void WriteFileLog(LPCTSTR lpszFormat, ...);


#ifndef CP_GB2312
#define  CP_GB2312	936	//Code Page Identifiers ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
#endif
