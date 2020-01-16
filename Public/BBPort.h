
#pragma once



#include "../eBb/MainAppMgrTh.h"
#include "../eBb/MaintransmitfileTh.h"
#include "../eBb/rcvfileTh.h"
#include "../eBb/AppMgrNetTh.h"
#include "../Public/Include/SubSystemInterface.h"
#include "../eBB/cmdinfo.h"
#include "../eBB/cmdinfomgr.h"

#ifdef _DEBUG
	#pragma comment(lib,"..\\debug\\eBB.lib")
#else
	#pragma comment(lib,"..\\Release\\eBB.lib")
#endif

