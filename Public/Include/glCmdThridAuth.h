#ifndef __glCmdThridAuth_h__2014_11_28__
#define __glCmdThridAuth_h__2014_11_28__

//此头文件定义所有第三方认证的消息

#include "glCmdAndStructDef.h"


typedef struct tag_AuthMsg
{
	int nMainCmd;
	int nSubCmd;
	int nReserve;
	int nDataSize;
	BYTE pData;
	int HeadSize(){return sizeof(int)*4;}
}AuthMsg, *PAuthMsg;

#define WM_ThridAuth_Base	(WM_IST+7000)

#define WM_ThridAuth_Req	WM_ThridAuth_Base + 1	//请求向第三方认证系统认证
#define WM_ThridAuth_Ack	WM_ThridAuth_Base + 2	//请求向第三方认证系统认证后的应答
#define WM_ThridAuth_CheckUser_Fail	WM_ThridAuth_Base + 3	//向第三方认证系统认证后失败
#define WM_ThridAuth_CheckUser_Succ	WM_ThridAuth_Base + 4	//向第三方认证系统认证后成功

#define WM_ThridAuth_Thread_Exit			WM_ThridAuth_Base + 101	//线程退出


#endif //#ifndef __glCmdThridAuth_h__2014_11_28__

