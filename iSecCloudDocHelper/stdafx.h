#pragma once
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
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

#define _AA_DLL_
#include "../Public/Global.h"
#include "../Public/eCCPort.h"
#include "../Public/Global_PltTypeDef.h"
#include "../Public/CmmLibport.h"
#include "../Public/Include/glCmdAndStructDef.h"
#include "../Public/Include/glCmdAndStructPrint2.h"
#include "../Public/Include/SystemDef.h"
#include "../Public/messageport.h"
using namespace isafetec;
#include "../Public/Include/NetCmdAndStructDef.h"
#include "../Public/Netport.h"
#include "../Public/Aaport.h"
#include "../Public/DataSvcExport.h"
#include "../Public/xabzipPort.h"

#define ENABLE_PWD_CRY

#include "../encryptlib/PWDCry.h"
#ifdef _DEBUG
#pragma comment(lib,"../debug/encryptlib.lib")
#else
#pragma comment(lib,"../release/encryptlib.lib")
#endif

#include "../Public/iPrintableDocumentDealDef.h"

extern CFileLog theLog;
extern CIniFile theIniFile;
extern BOOL IsDebug();

#include <QtGui>
#include <WinGDI.h>

#include "CloudDocDef.h"
#include "CloudDocDeal.h"
#include "DbOperateHelper.h"
#include "CloudDocDealThread.h"
#include "CloudDocDealMgr.h"
#include "MainThread.h"

