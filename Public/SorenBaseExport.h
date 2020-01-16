/*********************************************************************
* 版权所有 (C)2011，ISafeTec。
* 
* 文件名称：  SorenBaseExport.h
* 内容摘要：  安密基础动态链接库导出类的头文件
* 说    明：  
* 当前版本：  1.0.0.0
* 作    者：  SongQX
* 日    期：  2011/06/24
***********************************************************************/
 
#pragma once

#include "../SorenBase/SorenBase.h"
#include "../Public/CmmLibport.h"
#include "../SorenBase/SorenFile.h"
#include "../SorenBase/SorenPolicy.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\debug\\Sorenbase.lib")
#else
#pragma comment(lib,"..\\Release\\Sorenbase.lib")
#endif