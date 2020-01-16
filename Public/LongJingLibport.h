#ifndef _LONGJINGLIB_PORT_HEADER
#define _LONGJINGLIB_PORT_HEADER
// LongJingLibport.h: 
//
//////////////////////////////////////////////////////////////////////

#include "..\LongJingDll\LongJingOp.h"

#ifdef _DEBUG
	#pragma comment(lib,"..\\Debug\\LongJingDll.lib")
#else
	#pragma comment(lib,"..\\Release\\LongJingDll.lib")
#endif


#endif