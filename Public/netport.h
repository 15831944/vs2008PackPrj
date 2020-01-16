#ifndef _XX_NET__XXX_NET
#define _XX_NET__XXX_NET

#ifdef  _AA_NET_DLL_
#define _AA_NET_DLL_EXPORT_		__declspec(dllexport)
#define _AA_NET_DLL_EXPORT_C_ 	extern "C" __declspec(dllexport)
#else
#define _AA_NET_DLL_EXPORT_		__declspec(dllimport)
#define _AA_NET_DLL_EXPORT_C_	extern "C" __declspec(dllimport)
#endif

#include "../eNet/BaseSock.h"
#include "../eNet/IOCP.h"
#include "../eNet/MsgManage.h"
#include "../eNet/TcpSOckEt.h"
#include "../eNet/XSOckEt.h"
#include "../eNet/WndSocket.h"
#include "../eNet/BufferManage.h"
#include "../eNet/Packet.h"
#include "../eNet/BCSockEx.h"

#ifdef _DEBUG
	#pragma comment(lib,"..\\debug\\eNet.lib")
#else
	#pragma comment(lib,"..\\Release\\eNet.lib")
#endif



#endif 