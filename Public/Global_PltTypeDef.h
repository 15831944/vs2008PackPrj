//add by zfq,2013.01.16
//终端类型的扩展定义，原有的在global.h中定义

#pragma once

#ifdef  _AA_DLL_
#	define _AA_DLL_EXPORT_		__declspec(dllexport)
#	define _AA_DLL_EXPORT_C_ 	extern "C" __declspec(dllexport)
#else
#	define _AA_DLL_EXPORT_		__declspec(dllimport)
#	define _AA_DLL_EXPORT_C_	extern "C" __declspec(dllimport)
#endif


//终端类型
#if 0
#	define ISEC_PLT_CLT			1		//客户端
#	define ISEC_PLT_MGR			2		//管理机
#	define ISEC_PLT_SVR			3		//服务
#	define ISEC_PLT_PRT			4		//打印站点
#	define ISEC_PLT_TOOL		5		//工具
#	define ISEC_PRT_CHARGE_TOOL	6		//充值工具
#	define ISEC_PLT_WEB			7		//web服务
#endif

#define ISEC_PLT_PrtCenterSvr	10		//打印集中管理端
#define ISEC_PLT_TRANSCRIPT_IMPORT_TOOL	11	//成绩导入工具




