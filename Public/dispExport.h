#ifndef _DLL_DISPLIB_HEADER_
#define _DLL_DISPLIB_HEADER_

#pragma once

//¿ªÊ¼¼àÊÓ
extern "C" __declspec(dllexport) DWORD BeginMonitor(void* pVoid);
extern "C" __declspec(dllexport) void StopMonitor();

extern "C" __declspec(dllexport) BOOL RequestMonitor(void * pVoid);

#ifdef _DEBUG
#pragma comment(lib,"../debug/Displib.lib")
#else
#pragma comment(lib,"../release/Displib.lib")
#endif


#endif