#ifndef __GL_CMDANDSTRUCTDEF__
#define __GL_CMDANDSTRUCTDEF__


/************************************************************************/
/* 项目各位成员注意：add by zhandongbao 2008.12.17

1.定义一个结构体的某字段长度不定时，使用宏控制一下，如学生名称，
	#define NAME_SIZE 50
	char szName[STU_NAME_SIZE];
2.定义一个结构体要有描述，包括结构体描述和结构体的各项字段描述
2.网络上使用的结构体要注意对齐方式
3.本地使用的结构体，最好使用默认对齐方式
                                                                     */
/************************************************************************/

//pipe
#define ISEC_POLICY_SVC_PIPE_NAME		_T("\\\\.\\pipe\\{0B4EDF1B-C891-4e17-9A3B-2C89D8616C95}")
#define ISEC_CLT_SVC_PIPE				_T("\\\\.\\pipe\\{98D10522-0BE1-4397-8E8E-564ABF9AA327}")
#define ISEC_CLT_UI_PIPE				_T("\\\\.\\pipe\\{3F4A9BC5-2E06-45e1-838C-80300EF114F5}")
#define ISEC_CLT_TOOL_UI_PIPE			_T("\\\\.\\pipe\\{ECEBBCFF-BC67-4435-8155-CC71523B2A03}")

//-------------------------------------
#define ISEC_SVC_IPC_MAP_NAME			_T("Global\\{812EFCDE-25F7-4d05-B06A-5437CADA89A3}")
#define ISEC_SVC_IPC_EVENT_NAME			_T("Global\\{912775AA-EC86-4feb-A2EE-74B9A04A9391}")

//终端互斥变量
#define IST_PLT_CLT_MUTEX				_T("Global\\{F1BF5045-C9BD-4965-9F05-ABFA402C96A7}")


//中心服务器线程状态
#define TH_NO_LOGON						0//
#define TH_EQUIP_LOGON					1
#define TH_USER_LOGON					2
#define TH_USER_LOGOFF					3
#define TH_USER_LOGING					4//正在登录中，服务器已经指定学生，等待学生机登录

#define INPUT_PWD_SIZE 20		//设置密码时允许用户输入的最大长度

#define DB_SVR_SIZE	  50		//数据库服务器 
#define DB_NAME_SIZE  50		//数据库名称
#define DB_USER_SIZE  50		//数据库用户名
#define DB_PWD_SIZE	  50		//数据库连接密码


#define HDD_SIZE	50
#define MAC_SIZE	50
#define IP_SIZE		20
#define HOST_SIZE	50
#define WIN_USER_SIZE 50
#define FUN_NAME_SIZE 50		//函数名称长度
#define DLL_NAME_SIZE 50		//dll名字长度

#define CRP_SIZE	50
#define SOFT_SIZE	50
#define SRL_NUM_SIZE 30

#define MAX_PATH_2			(MAX_PATH * 2)

#define TERMINAL_ONLINE_INTERVAL	30000	//终端检测在线服务器的时间间隔
#define SERVER_ONLINE_INTERVAL		60000	//服务器检测在线服务器的时间间隔

// 终端标识
#define	TERMINAL_ID_MAC					0x01
#define TERMINAL_ID_HDD					0x02
#define TERMINAL_ID_IP					0x04
#define TERMINAL_ID_HOST				0x08


// 产品类型
enum PRODUCT_TYPE
{
	TYPE_INTRANET_SERVER = 0,
	TYPE_INTRANET_CLIENT = 1,
	TYPE_PRINTER = 2,
	TYPE_INTRANET_MANAGER = 3
};

//客户端网络状态
enum ICLT_NET_STATE{ ICLT_UNCONNECTED,ICLT_CONNECTED,ICLT_LOGONED};

//策略类型
enum POLICY_TYPE {
	PLY_PROCESS_FLT,	/*程序过滤*/
	PLY_URL_FLT,		/*网址过滤*/
	PLY_IPANDPORT_FLT,	/*ip和端口过滤*/

	PLY_LOG_UPLOAD,		/*日志上传*/
	PLY_FILE_CRYPT,		/*文档加密*/
	PLY_USB_STORE_CTRL,	/*usb存储设备控制策略*/
	
	PLY_SCAN_RES,		/*资产扫描*/
	PLY_CHECK_PC_IDLE,	/*检测计算机空闲时间*/

	PLY_DEVICE_CTRL,	/*设备控制策略*/
	PLY_PRINT_CTRL,		/*打印控制*/
	PLY_SYSTEM_CTRL,	/*系统控制*/
	PLY_FILECOPY_CTRL,	/*文件拷贝控制*/

	PLY_PROCESS_ALARM,	/*程序预警*/
	PLY_URL_ALARM,		/*网址预警*/
	PLY_DEVICE_ALARM,	/*设备预警*/
	PLY_NET_STREAM_ALARM,/*网络流量预警*/
	PLY_PC_ALARM,		/*计算机预警，如磁盘空间不足，*/	

	PLY_ALL_LOG,		/*所有日志策略*/
	PLY_MAIL_LOG,		/*邮件日志策略*/
	PLY_ALL_ANNUNCIAT,	/*预警策略*/
	PLY_STREAM_CONTROL, /*流量控制*/
	PLY_COMMON__CONTROL, /*通用控制*/
	PLY_QQ_FLT,			/*QQ过滤*/
	FILELOG_CTRL		/*文件日志*/
};

//过滤模式
enum FLT_MODE {FLT_BLACK/*黑名单*/,FLT_WHITE/*白名单*/};

// 过滤类型
enum FLT_TYPE {
	FLT_ONLINE,		/*在线策略*/
	FLT_EXCLINE,	/*离线策略*/
	FLT_ALWAYS		/*通用策略*/
};

//日志类型
enum LOG_TYPE {
	NULL_LOG,		/**/
	PROCESS_LOG,	/*程序日志*/
	URL_LOG,		/*网址日志*/
	SCREEN_LOG,		/*屏幕日志*/
	PRINT_LOG,		/*打印日志*/
	FILE_OP_LOG,	/*文件操作日志*/
	MAIL_LOG,		/*邮件日志*/
	NET_LOG,		/*网络流量日志*/
	PC_IDLE_LOG,	/*pc空闲日志*/
	PC_LOGON_LOG,	/*pc登陆与关机日志*/
	DEVICE_LOG,		/*设备插拔日志*/
	QQCHAT_LOG,		/*实时聊天日志*/
	PRINT_EVENT_LOG,/*打印日志*/
	ANNUNCIAT_LOG,	/*预警日志*/
	PRINT_DOC_LOG,	/*打印文档日志*/
	Direct_PRINT_LOG,		//直打
};

//直打反馈
enum EN_DIRECT_TYPE {
	enDirectIng,
	enDirectComplete,
	enDirectPrintPointBusy,
	enDirectFalse,
};

//资产更新状态
enum EQUIP_RES_STATE{
	EQP_RES_VAILD,		/*资产已经认证*/
	EQP_RES_INVAILD,	/*资产未认证*/
	EQP_RES_DELETED		/*资产已经删除*/
};

enum ALARM_TYPE
{
	ALARM_BY_MAIL,		/*邮件形式通知*/
	ALARM_BY_SMS,		/*短信形式通知*/
	ALARM_BY_IMSG		/*即时消息形式通知*/ 
};

enum ANNUNCIAT_TYPE
{
	ANNUNCIAT_URL,		/*网址预警*/
	ANNUNCIAT_PROC,		/*程序预警*/
	ANNUNCIAT_RESCHANGED,/*资产异动*/
	ANNUNCIAT_USBUSE,	/*USB设备使用*/
	ANNUNCIAT_ILLEGALIN,/*非法接入*/
	ANNUNCIAT_IPCHANGED,/*IP异动*/
	ANNUNCIAT_PCNAMECHANGED,/*机器名称变动*/
	ANNUNCIAT_DISKUNENOUGH/*磁盘空间不足*/
};

// 资产状态
enum ASSET_TYPE{
	ASSET_REMOVED = 0,	/*已经移除的设备*/
	ASSET_INUSED,		/*正在使用的设备*/
	ASSET_NEWADDED		/*新接入的设备*/
}; 

//一般过滤类型
enum COMMON_TYPE
{
	CM_TYPE_SCAN    = 0,        //扫描类型
	CM_TYPE_ON        = 1,        //开机类型
	CM_TYPE_OFF     = 2,        //关机类型
	CM_TYPE_DISIPCHANGE = 3,        // 禁止修改IP
	CM_TYPE_HIDEDIR   = 4,        // 安装目录隐藏
	CM_TYPE_HIDEPROC  = 5,         // 进程隐藏
	CM_TYPE_DISPRINTSCR = 6		// 禁止使用Printscreen键
};

// 行为管理当中所选择的客户类型
enum CLIENT_TYPE{
	CLIENT_UNKNOWN = -1,		/*未知类型*/
	CLIENT_DEPARTMENT,			/*选择的为部门*/
	CLIENT_USER,				/*选择的为员工*/
	CLIENT_DEVICE				/*选择的为机器*/
};

// added by jarry date:2011/06/26
// 行为管理当中所关系到的类别
enum MANAGE_TYPE{
	TISSUE_MANAGE = 0x01,					// 组织管理
	FILTER_MANAGE = 0x02,					// 策略管理
	LOGRECORD_MANAGE = 0x04,				// 日志管理
	REMOTE_MANAGE = 0x08,					// 远程管理
	INFOVIEW_MANAGE = 0x10					// 部门管理
};

// 管理员权限类型
enum ADMIN_POPEDOM{
	SYSTEM_POPEDOM = 0x01,					// 超级权限
	PERSONNEL_POPEDOM = 0x02,				// 人事权限
	DEPARTMENT_POPEDOM = 0x04,				// 部门权限
	MANAGER_POPEDOM = 0x08,					// 管理权限
	VINDICATE_POPEDOM = 0x10				// 维护权限
};

// 应用管理控制类别
enum APP_MANAGETYPE{
	KILL_PROCESS,					// 杀进程
	SET_PROC_LEVEL,					// 设置程序的级别
	START_SERVICE,					// 开启服务
	PAUSE_SERVICE,					// 暂停服务
	RECOVER_SERVICE,				// 回复服务
	STOP_SERVICE,					// 停止服务
	RESTART_SERVICE,				// 重启服务
	ENABLE_DEVICE,					// 启用设备
	DISABLE_DEVICE,					// 禁用设备
	ADD_USER,						// 添加用户
	BAN_USER,						// 禁止用户
	LOCK_USER,						// 锁定用户
	UNLOCK_USER,					// 解锁用户
	START_USER,						// 启用用户
	DELETE_USER,					// 删除用户
	DELETE_START,					// 删除启动
	DELETE_SHARE,					// 删除共享
	AUTO_SERVICE,					// 服务自动
	BAN_SERVICE,                    // 服务禁用
	MANUAL_SERVICE,					// 服务手动
	START_FIREWALL,					// 开启防火墙
	OFF_FIREWALL,					// 关闭防火墙
	BEGIN_BLACKSCR,					// 开启黑屏
	END_BLACKSCR					// 关闭黑屏
};

// 远程卸载类型
enum UNINSTALL_TYPE
{	
	UNINSTALL_SELF = 0,
	UNINSTALL_DEFSOFT
};

//登陆验证
enum ISEC_USER_LOGON_STATE
{
	ISEC_LOGON_SUC,
	ISEC_LOGON_ERR_PWD,
	ISEC_LOGON_ERR_NOUSER,
	ISEC_LOGON_ERR_DISABLE,
	ISEC_LOGON_ERR_NO_PRINT_ACCOUNT,
	ISEC_LOGON_ERR_NO_PROJECT_USER,
	ISEC_LOGON_ERR_NO_ACCOUNT_QUOTA,//账号没有配额
	ISEC_LOGON_ERR_NO_PRJ_QUOTA,//项目没有配额
	ISEC_LOGON_PRINT_CHARGE_SUC,	//扣费成功
	ISEC_LOGON_PRINT_CHARGE_ERR,		//扣费失败
	ISEC_LOGON_ERR_EXCEED_SIZE,		//超出最大上传大小限额
	ISEC_LOGON_COPY_CHARGE_ERR,
	ISEC_LOGON_SCAN_CHARGE_ERR,
	ISEC_LOGON_ERR_NORIGHT		//账号没有权限
};

//用户状态
enum ISEC_USER_STATE
{
	ISEC_USER_ENABLE = 1,
	ISEC_USER_DISABLE = 0,
	ISEC_USER_PRINT_ACCOUNT_ENABLE = 1,
	ISEC_USER_PRINT_ACCOUNT_DISABLE = 0
};

#pragma pack(push, 1)

//-----------------------------------
//产品信息
typedef struct tag_Production_Version
{
	int  nSize;						//结构体大小
	char szRegCrp[CRP_SIZE];		//注册公司或单位
	char szRegSoft[SOFT_SIZE];		//注册产品名称
	char szNumber[SRL_NUM_SIZE];	//产品序列号
	BYTE bDemo : 4;					//演示版
	BYTE bVersionType : 4;			//版本类型,0:全功能1电子教室
	WORD wUserSum;					//用户数量
	DWORD dwVersion;				//版本号
	DWORD dwUpdateVersion;			//更新版本号
	char pData[1];					//公司信息,主要包括名称，联系方式，地址，从加密文件中读取
} PRODUCTION_VERSION,*PPRODUCTION_VERSION;


//------------------------------------
//设备信息包
typedef struct tag_Equip_Info
{
	int  nEquipID;
	char szIP[IP_SIZE];		
	char szSubnet[IP_SIZE];		
	char szHost[HOST_SIZE];		
	char szMac[MAC_SIZE];		
	char szHdd[HDD_SIZE];
	char szWinUser[WIN_USER_SIZE];
	BYTE nEquipType;	//教师机，学生机
	BYTE nPltType;		//运行的平台，windows,wince
	WORD wSession;		//使用的登录session
	DWORD dwProductCode;//客户端运行的产品
} EQUIP_INFO,*PEQUIPINFO;

//登录到windows需要的帐号
typedef struct tag_Win_Logon_Info
{
	char	szDomain[WIN_USER_SIZE];	//名字前缀
	char	szUser[WIN_USER_SIZE];
	char	szPwd[WIN_USER_SIZE];
}WIN_LOGON_INFO,*PWIN_LOGON_INFO;

//数据流类型
enum StreamType {ORG_BINARY,ZIP_BINARY,ADO_ADTG,ADO_XML,PROTOBUF};
//流数据信息定义
typedef struct tag_Stream_Data_Info
{
	int	nSize;		//包括自身
	BYTE eDataType;	//参考streamtype
	BYTE pData[0];	//
} STREAM_DATA_INFO,*PSTREAM_DATA_INFO;
//-------------------------------------------------------
//资产信息
#define EQP_RES_GUID_LEN 100
typedef struct tag_equip_reource_Info
{
	int nSize;
	int nType;
	char szGuid[EQP_RES_GUID_LEN];
	double scan_time;
	BYTE pData[0];
} EQUIP_RESOURCE_INFO,*PEQUIP_RESOURCE_INFO;

//ado信息
typedef struct tag_Ado_Info
{
	char szIP[DB_SVR_SIZE];
	char szPort[10];			//这个值的长度一般不会超过10
	char szDbName[DB_NAME_SIZE];
	char szUser[DB_USER_SIZE];
	char szPwd[DB_PWD_SIZE];
	
} ADO_INFO,*PADO_INFO;
//-----------------------------------------------------

//连接信息
//ado信息
typedef struct tag_Connect_Info
{
	char szIP[IP_SIZE];
	int nPort;
	DWORD dwParam;
	DWORD dwTimeInterval;
} CONNECT_INFO,*PCONNECT_INFO;

//未添加打印站点
class CPrinterAtri
{
public:
	char szName[MAX_PATH];
	DWORD dwResolution;
	DWORD dwSpeed;
	DWORD dwColor;
	char szDesc[MAX_PATH];

	float fA3HB;
	float fA3Color;
	float fA4HB;
	float fA4Color;

	bool bDefault;
	BYTE szComment[MAX_PATH];
	BYTE szParam[MAX_PATH];		//方面以后扩展
};

enum REQ_PRINTSTATIONINFO_TYPE
{
	REQ_FROM_DB = 0,		//从数据库中请求
	REQ_AUTO_ADD_BY_PRT = 1,//打印站点自动添加
	REQ_AUTO_ADD_BY_TOOL = 2,	//通过工具自动添加
	REQ_CHECK_PRT_INFO = 3	//检测打印站点配置信息
};

#define AUTO_PRINTER_DATA_NAME	_T("printer.dat")
#define PRT_STATION_JOB_DIR		_T("print_cache")
#define PRT_CLIENT_JOB_DIR		_T("print_job")
#define DB_SERVER_JOB_DIR		_T("print_content")
#define DB_SERVER_SCREEN_DIR	_T("screen_content")
#define SYS_CONFIG_DIR			_T("config")
#define SYS_CONFIG_FILE			_T("config\\config.ini")
#define SYS_BRAND_FILE			_T("config\\brand.ini")
#define SYS_SETTINGS_FILE		_T("config\\settings.ini")
#define SYS_MAIL_FILE			_T("config\\configMail.ini")
#define SYS_ClientSetup_FILE	_T("Client_Setup\\config.ini")
#define SYS_FILECONTROL_INI		_T("config\\filefig.ini")
#define ISEC_LOCAL_DB_NAME		_T("ISecDb.dll")
#define JOB_FILE_SUFFIX			_T(".tif")
#define JOB_FILE_GZ_SUFFIX		_T(".tif.gz")
#define ISECUPDATE_NAME			_T("iSecUpdate.exe")
#define ISAFECLIENT_NAME		_T("iSafeClient.exe")
#define IPATCHUPDATESVC_NAME	_T("PatchUpdateSvc.exe")
#define DISPOSE_SOFTWARE		_T("software")
#define NTRANET_UPDATE			_T("Intranet")
#pragma pack(pop)



//提示信息，本地使用，不需要pop -push
enum TIP_TYPE {TIP_TXT,TIP_LINK_FILE,TIP_LINK_URL};
typedef struct tag_Tip_Info
{
	CString		sTitle;
	CString		sCnnt;
	TIP_TYPE	eType;
}TIP_INFO,*PTIP_INFO;

enum TIP_MSG_TYPE
{
	/*LOG_TYPE*/
	TM_ONLIY_STR = 100
};
typedef struct tag_Tip_Msg_Param
{
	TIP_MSG_TYPE eType;
	DWORD dwParam1;
	DWORD dwParam2;
	char szMsg[1024];
} TIP_MSG_PARAM,*PTIP_MSG_PARAM;

#define  IST_BASE	100
#define  WM_IST		WM_USER + IST_BASE //1034
//-----------------------------
//网络交互消息	

#define WM_CHECK_VERSION					WM_IST + 0	//检测版本
#define WM_ONLINE_FLAG						WM_IST + 1	//在线检测标识
#define WM_ENABLE_ONLINE_CHECK				WM_IST + 2	//开启或关闭在线检测
#define WM_ENABLE_DEBUG_LOG					WM_IST + 3	//开启调试日志 
#define WM_IPC_XEVENT						WM_IST + 4	
#define WM_MSG_TIP							WM_IST + 5	//设置提示信息
#define WM_MSG_TIP_STR						WM_IST + 6
#define WM_MSG_TIP_LINKSTR					WM_IST + 7	//带超链接的提示消息
#define WM_MSG_TIP_LOG						WM_IST + 8	//过滤使用的提示消息
#define WM_IST_SERVER_DETAIL_INFO			WM_IST + 9	//服务器发送的服务器信息
#define WM_SET_FLOATBAR_DLG_BTNS			WM_IST + 10	//设置浮动按钮栏
#define WM_LOG_SVR_ADDR_INFO				WM_IST + 11	//日志服务器地址					
#define WM_ORIG_DB_DATA						WM_IST + 12 //数据库查询的数据，转发到客户端，然后保存成单独的文件
#define WM_CLT_NOTIN_SERV_IPRANGE			WM_IST + 13
#define WM_SUB_PRODUCT_INFO					WM_IST + 14	//服务期发送给客户端平台
#define WM_CLT_SUB_PRODUCT_LOGON			WM_IST + 15	//子产品登陆product
#define WM_CHECK_CLT_POLICY					WM_IST + 16	//检测子产品的策略
#define WM_CLT_REQUEST_POLICY				WM_IST + 17	//客户端请求最新的策略
#define WM_UPDATE_CLT_POLICY				WM_IST + 18 //更新最新的策略信息
#define WM_CLIENT_BUFFER_MESSAGE			WM_IST + 19	//客户端消息
#define WM_SEND_PACK_DIRECT					WM_IST + 20 //转发消息
#define WM_ONLYSEND_PACKMSG					WM_IST + 21 //消息
#define WM_CHECK_EQUIP						WM_IST + 22 

#define WM_CONNECT_TO_MGR					WM_IST + 23 //连接到管理机
#define WM_POLICY_CHANGED					WM_IST + 24 //管理机通知策略配置发生变化

//管理机消息
#define WM_MGR_LOGON						WM_IST + 25//管理机登陆
#define WM_MGR_INFO							WM_IST + 26//管理机可以登陆的管理员信息
#define WM_ADO_INFO							WM_IST + 27//ado 连接信息

#define WM_PRINT_CLT_LOGON					WM_IST + 28 //打印终端验证账号
#define WM_NOTIFY_PRINT_LOGON				WM_IST + 29 //打印站点验证账号
#define WM_SUBMIT_PRINT_JOB					WM_IST + 30	//提交打印任务
#define WM_PRINT_FILE_DATA					WM_IST + 31	//打印作业文件信息
#define WM_PRINT_FILE_END					WM_IST + 32	//打印文件结束

#define WM_PRINT_STATION_LOGON				WM_IST + 33 //打印站点登陆
#define WM_PRINT_STATTON_INFO				WM_IST + 34 //打印站点信息
#define WM_PRINT_STATION_PRINT_JOB			WM_IST + 35 //打印站点预计扣费
#define WM_PRINT_STATION_CHARGE_ON_PRINT	WM_IST + 36 //打印站点预计扣费
#define WM_PRINT_CLT_REQ_CHECK				WM_IST + 37//客户端请求打印信息，如是否可以使用项目打印，是否可以预约，等
#define WM_PRINT_STATION_REQ_ORDER			WM_IST + 38 //打印站点请求预约信息
#define WM_PRINT_STATION_REQ_DIRECT_PRINT	WM_IST + 39 //打印站点直接打印的文档
#define WM_PRINT_STATION_DEL_JOB			WM_IST + 40 //打印站点删除作业
#define WM_PRINT_STATION_CHARGE_ERR			WM_IST + 41 //扣费失败
#define WM_PRINT_STATION_CHARGE_ON_COPY		WM_IST + 42 //复印计费
#define WM_PRINT_STATION_CHARGE_ON_SCAN		WM_IST + 43 //扫描计费
#define WM_PRINT_STATION_RECHARGE_FROM_CARD	WM_IST + 44 //从一卡通中充值


#define WM_CLT_REQ_ONLINE_CMD				WM_IST + 45 //客户端请求在线命令
#define WM_SYNC_SVR_TIME					WM_IST + 46 //同步服务器时间
#define WM_LOCAL_NET_STATE					WM_IST + 47 //网络状态
#define WM_CLT_LOGON						WM_IST + 48 //客户端用户登陆
#define WM_QUERY_LOCAL_EQUIP_INFO			WM_IST + 49 //	

//-------------------------------
//日志上传相关消息
#define WM_UPLOAD_HW				WM_IST + 50
#define WM_UPLOAD_SW				WM_IST + 51
#define WM_UPLOAD_LOG				WM_IST + 52
#define WM_UPLOAD_SCREEN			WM_IST + 53
#define WM_UPLOAD_SCREEN_DATA		WM_IST + 54
#define WM_UPLOAD_SCREEN_CMPLT		WM_IST + 55
#define WM_EXECUTE_SQL_CMD			WM_IST + 56		
#define WM_EXECUTE_SQL_RECORD		WM_IST + 57

//屏幕日志
#define WM_REQ_SCREEN_LOG_FILE		WM_IST + 58		// 请求日志文件
#define WM_REQ_SCREEN_LOG_FILE_END	WM_IST + 59

//打印日志
#define WM_REQ_PRINT_LOG			WM_IST + 60
#define WM_REQ_PRINT_LOG_FILE		WM_IST + 61
#define WM_REQ_PRINT_LOG_FILE_END	WM_IST + 62
#define WM_PUSH_PRINT_LOG_FILE		WM_IST + 63
#define WM_ALL_PRINT_LOG_END		WM_IST + 64

//打印文档日志
#define WM_UPLOAD_DOC_PRINT			WM_IST + 70
#define WM_UPLOAD_DOC_PRINT_DATA	WM_IST + 71
#define WM_UPLOAD_DOC_PRINT_CMPLT	WM_IST + 72

//文件传输
#define WM_TRANSMIT_FILE_PARAM		WM_IST + 80
#define WM_TRANSMIT_FILE_BEGIN		WM_IST + 81
#define WM_TRANSMIT_FILE_DATA		WM_IST + 82
#define WM_TRANSMIT_FILE_END		WM_IST + 83
#define WM_TRANSMIT_COMPLETED		WM_IST + 84

//验证码登录
#define WM_PRINT_CLT_GET_VERIFY_CODE			WM_IST + 85 //获取验证码
#define WM_PRINT_CLT_LOGIN_WITH_VERIFY_CODE		WM_IST + 86 //验证码模式登录

#define WM_DirectPrtint			WM_IST + 87
#define WM_GetEntInfo			WM_IST + 89

#define WM_SubmitJob					WM_IST + 90

#define WM_SUBMIT_DIRECT_PRINT_JOB		WM_IST + 91	//直打任务

//传输文件类型
enum TSF_FILE_TYPE
{
	TSF_DISPOSE_SOFTWATRE = 0,	//软件分发
	TSF_SCAN_FILE = 1,	//扫描文件
	TSF_CLIENT_UPDATE = 2	// 客户端更新
};
//管理机关注客户端状态事件
//enum MGR_SUBSCRIBE_EVENT{ MSE_EQUIP_LOGON,MSE_EQUIP_LOGOFF,MSE_USER_LOGON,MSE_USER_LOGOFF};
#define MSE_EVENT_COUNT		4
#define MSE_EQUIP_LOGON		0x1
#define	MSE_EQUIP_LOGOFF	0x2
#define MSE_USER_LOGON		0x4
#define MSE_USER_LOGOFF		0x8
#define WM_MGR_SUBSCRIBE_EVENT			WM_IST + 90
/*
#define WM_EQUIP_LOGON				WM_IST + 90
#define WM_EQUIP_LOGOFF				WM_IST + 91
#define WM_USER_LOGON				WM_IST + 92
#define WM_USER_LOGOFF				WM_IST + 93
#define WM_MGR_REQ_CLT_INFO			WM_IST + 94
*/
//-----------------------------------
#define WM_CHECK_PC_IDLE					WM_IST + 100	//检测
//本地响应的消息
#define WM_TRAY_NOTIFY						WM_IST + 200	//托盘图标
#define WM_PROCESS_PACK						WM_IST + 201	//线程之间消息包的转发
#define WM_PIPE_MSG							WM_IST + 202
#define WM_ALARM_NOTIFY						WM_IST + 203
#define WM_PIPE_REQ_LOGON_INFO				WM_IST + 204

//--------------------------------------
//应用管理类命令
#define WM_APP_CONTROL						WM_IST + 220	// 进程的查杀，服务的开启，停止，设备的禁用与启用
#define WM_BEGIN_STREAM						WM_IST + 231	// 开始新的流量获取
#define WM_END_STREAM						WM_IST + 232	// 停止流量获取
#define WM_UPDATE_TIMEINTERVAL				WM_IST + 233	// 更新时间

// 远程关机/开机/重启命令及命令行命令
#define WM_SHUTDOWN_CMD						WM_IST + 237	// 关机
#define WM_LOGOFF_CMD						WM_IST + 238	// 注销
#define WM_REBOOT_CMD						WM_IST + 239	// 重启
#define WM_STARTPC_CMD						WM_IST + 240	// 开机
#define WM_REMOTE_CMD						WM_IST + 241	// 命令行命令
#define WM_UNSTAILL_CMD						WM_IST + 242	// 远程卸载
#define WM_SCREEN_BLACK_CMD					WM_IST + 243	// 黑屏
#define WM_SCREEN_CANCEL_CMD				WM_IST + 244	// 取消黑屏
#define WM_SCREENBC_CMD						WM_IST + 245	// 屏幕演示
#define WM_SCREENM_CMD						WM_IST + 246	// 屏幕监视
#define WM_SCREENMC_CMD						WM_IST + 247	// 屏幕控制
#define WM_SCANASSERT_CMD					WM_IST + 248	// 扫描资产
#define WM_CLEANLOG_CMD						WM_IST + 249	// 日志清除
#define WM_DISPOSE_CMD						WM_IST + 250	// 软件布署
#define WM_OUTLINE_DISPOSE_CMD				WM_IST + 251	// 离线部署
#define WM_CLIENT_UPDATE					WM_IST + 252	// 客户端升级
#define WM_CLIENT_OS_UPDATE					WM_IST + 253	// 客户端操作系统升级

//充值工具
#define WM_PRINT_ADMIN_INFO					WM_IST + 300
#define WM_PRINT_CHARGE_TOOL_LOGON			WM_IST + 301
#define WM_PRINT_CHARGE_OPLOG				WM_IST + 302
#define WM_PRINT_CHARGE_CHECK_CARD			WM_IST + 303

#define WM_PRINT_CHARGE_EX_ADDUSER			WM_IST + 304	//添加用户
#define WM_PRINT_CHARGE_EX_CLEARMONEY		WM_IST + 305	//清除用户余额
#define WM_PRINT_CHARGE_EX_TICKETINFO		WM_IST + 306	//打印小票
//--------------------------------------

//安全选项
#define SEC_LOCK_SRC			0x00000001//锁屏幕
#define SEC_LOCK_KEYBROAD		0x00000002//锁键盘
#define SEC_LOCK_MOUSE			0x00000004//锁鼠标
#define SEC_LOCK_IP				0x00000008//锁ip
#define SEC_PARAM				0x00000010//参数
#define SEC_FORBID_POWER_MGR	0x00000020//禁止电源管理
#define SEC_FORBID_SCR_SAVE		0x00000040//禁止屏幕保护
#define SEC_FORBID_IE			0x00000080//禁止IE上网
#define SEC_MASK_DISCONN		SEC_LOCK_MOUSE|SEC_LOCK_KEYBROAD|SEC_LOCK_SRC
#define SEC_MASK_NORM			SEC_FORBID_POWER_MGR|SEC_FORBID_SCR_SAVE|SEC_LOCK_IP
#define SEC_MASK_WEB			SEC_FORBID_IE
#define SEC_MASK_ALL			0xffffffff

#define IsLockKey(a)			(a & SEC_LOCK_KEYBROAD)
#define IsLockMouse(a)			(a & SEC_LOCK_MOUSE)
#define IsLockSrc(a)			(a & SEC_LOCK_SRC)


//------------------------
//利盟打印终端需要的命令
#define WM_FTP_INFO			WM_IST + 400	//ftp信息


//加密
#define WM_EXCEED_ENCRYPT_NUM				WM_IST + 2000

#endif