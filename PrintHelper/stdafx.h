// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

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


#include <Winspool.h>

#define _AA_DLL_
#include "../Public/Global.h"
#include "..\Public\include\glCmdAndStructDef.h"
#include "../Public/CloudPrintCommDef.h"
#include "../Public/CmmLibport.h"
#include "../Public/XabZipPort.h"
#include "../Public/eCCPort.h"
/*#include "../Public/messageport.h"*/
#include "HwConfig.h"
#include "CopyInterface.h"
#include "SerialCommHelper.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

extern CFileLog theLog;
extern CIniFile g_oIniFile;
extern int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

#ifndef HIGH32
#define HIGH32(param)	((UINT32)(param >> 32)) 
#endif

#ifndef LOW32
#define LOW32(param)	((UINT32)param)
#endif

#ifndef MAKE64
#define MAKE64(high32, low32)	(((UINT64)high32 << 32) + low32)
#endif