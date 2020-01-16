#ifndef __glCmdAndStructDefEx_5000_6000_h__2012_12_18__
#define __glCmdAndStructDefEx_5000_6000_h__2012_12_18__
//add by zfq,2012.12.18
//网络命令宏定义，命令号5000~6000

#include "glCmdAndStructDef.h"

#define	WM_NET_FUNCTION_POLICY	(WM_IST+5000)	//功能策略信息，打印、复印、扫描、传真的策略掩码
#define	WM_NET_REGISTER_CARD	(WM_IST+5001)	//注册卡号,命令号即 :WM_USER+(100+5001) 利盟端->LmPrinterSvr->LogicMgr

#define WM_PrtCenterSvr_Base	(WM_IST + 5100)	//iPrintCenterSvr中用的命令号,到WM_IST+5200保留

//打印集中管理端的 主监视线程发给连接中心服务器的网络线程，网络线程再转发给中心服务器，某个打印机的名字，返回它在数据库中的ID
#define WM_NET_PrtCenterSvr_PrtCheck		(WM_PrtCenterSvr_Base + 40)


//中心服务器对WM_NET_PrtCenterSvr_PrtCheck的数据进行验证后，回发给打印集中管理端的消息
#define WM_NET_PrtCenterSvr_PrtCheckRet		(WM_PrtCenterSvr_Base + 41)

//调整打印纸张数量
#define WM_NET_PrtCenterSvr_AdjustPrintedCount		(WM_PrtCenterSvr_Base + 42)

//利盟打印机用户登录后,请求用户常用的扫描到邮箱的邮件地址列表,
#define WM_NET_PrtCenterSvr_ScanToMailAddrList		(WM_PrtCenterSvr_Base + 43)

//利盟打印机用户登录后,请求用户输入的新的扫描到邮箱的邮件地址
#define WM_NET_PrtCenterSvr_ScanToMailNewAddr		(WM_PrtCenterSvr_Base + 44)

//打印集中管理端的 监视线程发给连接中心服务器的网络线程，网络线程再转发给中心服务器
#define WM_NET_PrtCenterSvr_UserPrintReq		(WM_PrtCenterSvr_Base + 50)

//中心服务器对WM_NET_PrtCenterSvr_UserLogon的数据进行验证后，回发给打印集中管理端的消息
#define WM_NET_PrtCenterSvr_UserPrintReqACK		(WM_PrtCenterSvr_Base + 51)

//打印管理端发给日志服务器,提交spl打印任务
#define WM_NET_PrintCenterSvr_SubmitPrintSpl	(WM_PrtCenterSvr_Base + 52)

//打印集中管理端的 辅助网络线程发给日志服务器的消息，打印日志文件传输完毕
#define WM_NET_PRINT_SPL_DATA					(WM_PrtCenterSvr_Base + 53)

//打印集中管理端的 辅助网络线程发给日志服务器的消息，打印日志文件传输完毕
#define WM_NET_PRINT_SPL_END					(WM_PrtCenterSvr_Base + 54)

//调整复印纸张数量
#define WM_NET_PrtCenterSvr_AdjustCopiedCount		(WM_PrtCenterSvr_Base + 55)


#define WM_PrtStation_Base	(WM_IST + 5200)

//用户自己转账，从一卡通账户到打印系统账户,请求消息，从打印点发到中心服务器
#define WM_PRINT_STATTON_UserSelfChargeOpLog	(WM_PrtStation_Base + 1)

//用户自己转账，从一卡通账户到打印系统账户,,确认消息，从中心服务器
#define WM_PRINT_STATTON_UserSelfChargeOpLogACK	(WM_PrtStation_Base + 2)
#	define DEF_UserSelfChargeOpLogACK_OK				1	//转账成功,子命令
#	define DEF_UserSelfChargeOpLogACK_FAIL				2	//转账失败,子命令
#	define DEF_UserSelfChargeOpLogACK_OK_ButSthWrong	3	//转账成功，但某些处理失败
#	define DEF_UserSelfChargeOpLogACK_FAIL_ButLogExit	4	//转账失败, 是因为之前可以已经转账成功过

//打印站点自动扫描转账失败的残留日志文件,并发送本消息到中心服务器请求再次转账
#define WM_PRINT_STATTON_AutoChargeOplog	(WM_PrtStation_Base + 3)

//打印站点自动扫描转账失败的残留日志文件,从中心服务器发送回来的确认消息
#define WM_PRINT_STATTON_AutoChargeOplogACK	(WM_PrtStation_Base + 4)


//一卡通转账日志目录
#define PRT_CARDCHARGE_LOG	TEXT("ecardcharge")	

//自助缴费、代缴费日志目录
#define PRT_BillPaymentCharge_LOG	TEXT("billpaymentcharge")	

//检测转账请求的日志记录状态
#define LOG_STATE_EXIST		1		//转账记录已经存在,
#define LOG_STATE_NOEXIST	2		//转账记录不存在
#define LOG_STATE_ERROR		3		//转账记录获取错误

//测试宏,
//当定义了TEST_ZXL,进入测试.
//当定义了TEST_ZXL0,退出测试
//#define TEST_ZXL0


#endif //#ifndef __glCmdAndStructDefEx_5000_6000_h__2012_12_18__