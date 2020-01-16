#pragma once

//json库的编译参数要求与CloudPrintRouterDemo的一致。【/MD】和【在共享DLL中使用MFC】

#include "../Public/jsoncpp-src-0.5.0/include/json/json.h"

#ifdef _DEBUG
#pragma comment(lib,"../Public/jsoncpp-src-0.5.0/lib/debug/json.lib")
#else
#pragma comment(lib,"../Public/jsoncpp-src-0.5.0/lib/release/json.lib")
#endif
