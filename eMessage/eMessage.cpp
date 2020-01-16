// eMessage.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "eMessage.h"


// 这是导出变量的一个示例
EMESSAGE_API int neMessage=0;

// 这是导出函数的一个示例。
EMESSAGE_API int fneMessage(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 eMessage.h
CeMessage::CeMessage()
{
	return;
}
