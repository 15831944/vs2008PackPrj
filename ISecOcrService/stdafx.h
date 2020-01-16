// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>



#import "e:\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")
extern void WriteLogEx(LPCTSTR lpszFormat, ...);
#include "../Public/XabZipPort.h"

#include <boost/asio.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/dll.hpp>
#include "util/log4z.h"
using namespace zsummer::log4z;