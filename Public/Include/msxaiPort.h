// msxaiPort.h
// msxai -- App Boot Info for msxai workspace and external useage to msxai DLL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef  _MSXAI_DLL_
#define _MSXAI_PORT_		__declspec(dllexport)
#define _MSXAI_PORT_C_ 	extern "C" __declspec(dllexport)
#else
#define _MSXAI_PORT_		__declspec(dllimport)
#define _MSXAI_PORT_C_	extern "C" __declspec(dllimport)
#endif

#define MAX_TITLESIZE 2048

#include "GetBootInfo.h"