// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>


#ifdef _DEBUG
#pragma comment(lib,"..\\public\\lib\\debug\\libprotobuf.lib")
//#pragma comment(lib,"..\\public\\lib\\debug\\libprotoc.lib")
#else
#pragma comment(lib,"..\\public\\lib\\release\\libprotobuf.lib")
//#pragma comment(lib,"..\\public\\lib\\release\\libprotoc.lib")

#endif