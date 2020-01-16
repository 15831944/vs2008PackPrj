#ifndef _LmPrintStationCommDef_H_A9C189B3_0FFE_4026_8247_14EFE044029B_
#define _LmPrintStationCommDef_H_A9C189B3_0FFE_4026_8247_14EFE044029B_

#define LmProcessParamMemMapName		_T("Global\\{1C7897E4-ABB4-409f-B6F6-05192CC789F8}")

typedef struct tag_LmProcessParam
{
	DWORD dwPrinterIndex;
	DWORD dwProcessId;
	DWORD dwInstanceState;
	DWORD dwRunState;
	char pszPrinterName[MAX_PATH];
	char pszModuleExePath[MAX_PATH];
}LmProcessParam, *PLmProcessParam;

//企业打印点进程共享实例变量运行状态
enum LM_Instance_State
{
	LM_Instance_State_Orig	= 0,	//初始状态
	LM_Instance_State_Run	= 1,	//运行
	LM_Instance_State_Stop	= 2,	//停止
	LM_Instance_State_Disable	= 3		//实例禁用
};

//企业打印点进程运行状态
enum LM_Run_State
{
	LM_Run_State_UnKnown	= 0,	//未知
	LM_Run_State_Running	= 1,	//已运行
	LM_Run_State_Stoped		= 2,	//已停止
	LM_Run_State_RunningAndAbnormal		= 3,	//运行但程序异常
	LM_Run_State_ExceptionExit			= 4,	//异常退出
	LM_Run_State_ParameterErrorExit		= 5,	//参数错误退出
	LM_Run_State_MultiInstanceExit		= 6,	//多实例异常退出
	LM_Run_State_NormalExit				= 7,	//正常退出
	LM_Run_State_Exiting				= 8,	//正在退出
	LM_Run_State_Starting				= 9,	//正在启动
	LM_Run_State_Disable				= 10,	//实例被禁用，要求重启实例
	LM_Run_State_MAX					= 11	//最大值
};

static TCHAR *gc_LM_Run_State_Table[] = {
	TEXT("未知"),
	TEXT("已运行"),
	TEXT("已停止"),
	TEXT("运行有异常"),
	TEXT("异常退出"),
	TEXT("参数错误退出"),
	TEXT("多实例异常退出"),
	TEXT("已退出"),
	TEXT("正在退出"),
	TEXT("正在启动"),
	TEXT("已禁用")
};


#define MAX_PRINTER_COUNT		128			//最大控制打印机数量

#define LmPrintStationEXE		TEXT("LmPrintStation.exe")
#define LmPrintStationSetFile	TEXT("config\\settings.ini")
#define LmPrintStationRunState	TEXT("LmPrintStationRunState")	//保存所有企业打印点进程运行状态的节。[A=B],A表示打印点编号，B表示打印点运行状态
#define LmPrintStationRunPid	TEXT("LmPrintStationRunPid")	//保存所有企业打印点进程运行状态的节。[A=B],A表示打印点编号，B表示打印点进程ID
#define LmPrintStationMap		TEXT("LmPrintStationMap")		//保存所有企业打印点的配置信息。[A=B],A表示打印点编号，B表示打印点绑定的打印机名
#define LmPrintStationMgr		TEXT("LmPrintStationMgr")		//
#define LmPrintStationHelper	TEXT("LmPrintStationHelper")	//

//命令行参数【LmPrintStation.exe [操作类型] [打印机编号] [打印机名]】
#define LmPrintStationStartFlag		TEXT("/Start")		//命令格式一：LmPrintStation.exe /Start	1	LmX860
#define LmPrintStationStartFlag2	TEXT("-Start")		//命令格式二：LmPrintStation.exe /Start
#define LmPrintStationReStartFlag	TEXT("/ReStart")	//命令格式一：LmPrintStation.exe /ReStart	1	LmX860
#define LmPrintStationReStartFlag2	TEXT("-ReStart")	//命令格式二：LmPrintStation.exe /ReStart
#define LmPrintStationStopFlag		TEXT("/Stop")		//命令格式三：LmPrintStation.exe /Stop	1
#define LmPrintStationStopFlag2		TEXT("-Stop")
#define LmPrintStationCheckRunStateFlag		TEXT("/CheckRunState")	//命令格式四：LmPrintStation.exe /CheckRunState	1
#define LmPrintStationCheckRunStateFlag2	TEXT("-CheckRunState")	//返回值参考：LM_Run_State

#define LmPrintStationRunWaitTimeMS	5000	//打印点进程运行默认等待时间

#define LmPrintStationQuitCmd		TEXT("quit")		//退出命令行指令

#endif //_LmPrintStationCommDef_H_A9C189B3_0FFE_4026_8247_14EFE044029B_