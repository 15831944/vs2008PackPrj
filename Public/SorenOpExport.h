/*********************************************************************
* 版权所有 (C)2011，ISafeTec。
* 
* 文件名称：  SorenOp.h
* 内容摘要：  
* 说    明：  
* 当前版本：  1.0.0.0
* 作    者：  SongQX
* 日    期：  2011/06/30
***********************************************************************/
#pragma once

// SorenOp依赖的模块，为了包含SorenOP时方便放在这里
#include "../Public/Include/SDefine.h"
#include "../Public/Include/SorenDefine.h"
#include "../Public/SorenCodeExport.h"
#include "../Public/SorenBaseExport.h"
#include "../Public/messageport.h"
#include "../Public/Include/CmmInterface.h"
using namespace isafetec;

// 为了方便设置与获取u盘加密方式开放def.h头文件
#include "../../Drivers/DevDriver/def.h"

// Soren自己的头文件
#include "../SorenOp/FileSystemControl.h"
#include "../SorenOp/SorenOp.h"
#include "../SorenOp/FileSystemLog.h"
#include "../SorenOp/deviceexport.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\sorenop.lib")
#else
#pragma comment(lib,"..\\Release\\sorenop.lib")
#endif