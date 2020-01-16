/*********************************************************************
* 版权所有 (C)2011
* 
* 文件名称：  SorenCodeExport.h
* 内容摘要：  SorenCode导出头文件及包含库
* 说    明：  调用SorenCode模块时必须包含此文件
* 当前版本：  1.0.0.0
* 作    者：  SongQX
* 日    期：  2011/06/20
***********************************************************************/
#include "../SorenCode/RSA.h"
#include "../SorenCode/AES.h"
#include "../Sorencode/MD5.h"
//#include "../SorenCode/PWDCry.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\sorencode.lib")
#else
#pragma comment(lib,"..\\Release\\sorencode.lib")
#endif