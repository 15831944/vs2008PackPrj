#ifndef _DLL_EXPORT_HEADER_LOGINC_MGR
#define _DLL_EXPORT_HEADER_LOGINC_MGR

#include "../LogicMgr/BlockBufferHelper.h"
#include "../LogicMgr/MainNetFLowMgr.h"
#include "../LogicMgr/ClientNetFlowMgr.h"
#include "../LogicMgr/TerminalInfo.h"

#ifdef _DEBUG
	#pragma comment(lib,"..\\debug\\logicmgr.lib")
#else
	#pragma comment(lib,"..\\Release\\logicmgr.lib")
#endif



#endif 