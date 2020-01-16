#ifndef __glCmdAndStructThridPay_h_2017_03_15__
#define __glCmdAndStructThridPay_h_2017_03_15__

//此头文件定义所有第三方支付平台的消息

#include "glCmdAndStructDef.h"


typedef struct tag_RequestRecord
{
	int	nEquipID;
	DWORD dwMainCmd;
	DWORD dwSubCmd;
	DWORD dwTimeStamp;	//请求源的时间戳
	DWORD dwTickCount;		//用于本地检测超时的tickcount
} REQUEST_RECORD,*PREQUEST_RECORD;

//设备信息包
typedef struct tag_Ser_Equip_Info
{
	int  nEquipID;
	char szIP[IP_SIZE];
	char szSubnet[IP_SIZE];
	char szHost[HOST_SIZE];
	char szMac[MAC_SIZE];
	char szHdd[HDD_SIZE];
	char szWinUser[WIN_USER_SIZE];
	char szHttpSerIp[IP_SIZE];
	BYTE nEquipType;	//教师机，学生机
	BYTE nPltType;		//运行的平台，windows,wince
	WORD wSession;		//使用的登录session
	DWORD dwProductCode;//客户端运行的产品
	DWORD dwOsVersion; // 操作系统的版本
} SER_EQUIP_INFO,*PSEREQUIPINFO;

#define WM_THIRDPAY_BASE	(WM_USER+2000)

#define WM_THIRDPAY_CHECK_USER	(WM_THIRDPAY_BASE + 1) //验证用户帐号和密码
#define WM_THIRDPAY_lOGINPRT	(WM_THIRDPAY_BASE + 2) //登录打印站点
#define WM_THIRDPAY_LOGOUTPRT	(WM_THIRDPAY_BASE + 3) //注销打印站点登录
#define WM_THIRDPAY_REQ_LOGIN_QRCODE	(WM_THIRDPAY_BASE + 4) //请求登录二维码
#define WM_THIRDPAY_STATTON_REQ_PAYQRCODE (WM_THIRDPAY_BASE + 5) //请求支付二维码
#define WM_THIRDPAY_REQ_GENERATE_ORDER	(WM_THIRDPAY_BASE + 6)//请求生成订单
#define WM_THIRDPAY_PAY_RESULT	(WM_THIRDPAY_BASE + 7)//支付结果
#define WM_THIRDPAY_CANCEL_ORDER (WM_THIRDPAY_BASE + 8) //取消订单
#define WM_THIRDPAY_REQ_ORDER_HEADER (WM_THIRDPAY_BASE + 9)//订单号开始信息


#define WM_THIRDPAY_REQUESTACK	(WM_THIRDPAY_BASE + 100) //请求的响应
#define WM_THIRDPAY_THREAD_MESSAGE	(WM_THIRDPAY_BASE + 101)	//第三方支付线程发过来的消息
#define WM_THIRDPAY_THREAD_EXIT			WM_THIRDPAY_BASE + 102	//线程退出
#define WM_THIRDPAY_MAINSVR_MESSAGE			WM_THIRDPAY_BASE + 103	//中心服务器发个第三方线程的消息

#define WM_NS_WebInfoAck					(WM_USER + 8132)

#define	WM_FLAG_REQUEST				0	//请求回应标识--请求
#define WM_FLAG_RESONSE				1	//请求回应标识--回应


//以下定义对应数据库中的类型字典，不是命令
#define CONST_ZD_INPUTLOGIN		1601	//帐号密码登录
#define CONST_ZD_SWIPECARDLOGIN	1602	//刷卡登录
#define CONST_ZD_WECAHTLOGIN	1603	//微信扫码登录
#define CONST_ZD_OTHERLOGIN		1610	//其它登录方式



#endif //#ifndef __glCmdAndStructThridPay_h_2017_03_15__




