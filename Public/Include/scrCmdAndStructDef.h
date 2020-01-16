#ifndef _GLOBEDEFINE_XXXX
#define _GLOBEDEFINE_XXXX

#pragma	pack (push,1)
#define DSK_CHECK_TIMER_INTER 500
//抓屏控制
enum CAP_SCR_CTRL {CAP_STOP,CAP_PAUSE,CAP_RESUME,CAP_MAX_SIZE,CAP_FULL_SCR,CAP_START,CAP_START_CTRL,CAP_START_RECORD,CAP_CURSOR_POS};
//桌面类型
enum DSK_TYPE {DSK_DEFAULT = 0,DSK_WINLOGON = 1,DSK_SCRSAVER = 2};
//抓屏区域设置
//抓全屏、抓指定区域、抓指定窗口、排除特定的窗口
enum SCR_CAP_MODE {SCR_CAP_FULL = 0,SCR_CAP_RECT = 1,SCR_CAP_HWND = 2,SCR_CAP_EXCEPT_HWND = 3};
//更新区域类型
//更新出错、更新区域、更新全屏、线程消息
enum UPDATE_RECT_TYPE {UPDATE_ERR = 0,UPDATE_RECT = 1,UPDATE_FULL = 2,UPDATE_OTHRE_MSG = 3,UPDATE_MOUSE = 4,UPDATE_GL_WND = 5};
//压缩类型
//质量、自动调节、快速压缩、快速无损压缩、不压缩
enum COMPRESS_TYPE {CMP_ZIP = 0,CMP_AUTO_AJUST = 1,CMP_JPG_FAST = 2,CMP_JPG_GOOD = 3,CMP_NONE = 4};
//压缩级别
const int ZIP_CMP_LEVLE = 2;
//屏幕属性
struct CSrcZoomInfo
{
	bool bzoom;
	int  xMax;
	int  yMax;
};
//录制状态
enum RECORD_STATE{RECORD_RUNNING,RECORD_PAUSE,RECORD_STOP};
//语音状态
enum AUDIO_STATE {AUDIO_NONE,AUDIO_SINGLE,AUDIO_DOUBLE};
//语音来源
enum AUDIO_SRC{NO_AUDIO,TEA_AUDIO,STU_AUDIO};
//连接方式
enum CONNET_TYPE {CNNT_PASSIVE,CNNT_INITIATIVE,CNNT_AGENT};
//自动对焦方式
enum TRACE_TYPE {TRACE_NONE,TRACE_LOCAL,TRACE_PEER};
//显示标签方式
enum SHOW_TYPE {SHOW_NAME,SHOW_IP,SHOW_HOST};
//学生机显示方式
enum SCREEN_SHOW_MODE{SHOW_ON_WND,SHOW_ON_FULL,SHOW_ON_ZOOM,SHOW_ON_FIX};
//针对镜像驱动的命令
enum DISPLAY_CMD{DISP_LOAD,DISP_UNLOAD};

typedef struct tag_Screen_Property
{
	DWORD	dwCompressType;	//压缩类型
	int		nBitCount;		//位图位数
	int		nX;				//最大宽度
	int		nY;				//最大长度
	bool	bZoom;
}SCREEN_PROPERTY,*PSCREEN_PROPERTY;

#ifndef IP_SIZE
#define IP_SIZE 20
#endif
typedef BOOL (*PFFillDataToBuffer)(BYTE* pBuffer,UINT nLen,RECT* rc,int nType);	//added by JARRY	


//被监视参数
typedef struct tag_Monitored_Src_Param
{
	int		nViewCount;		//分屏个数
	int		nSwitchInterval;//切换间隔
	BYTE	bFix;			//1:1显示	
	BYTE	bCtrl;			//开始控制
	DWORD	dwCompressType;	//压缩类型
	int		nBitCount;		//位图位数
	int		nMaxX;			//最大宽度
	int		nMaxY;			//最大长度
	BYTE	eNameType;		//显示名称
	BYTE	bWithAudioType; //是否带语音，0不带1带单项2带双向
	BYTE	bSaveAsAVI;			// 是否保存为AVI，FALSE：WMV
	BYTE	bSupport3D;			//是否支持3d变化
	WORD	bResever;
}MONITORED_SRC_PARAM,*PMONITORED_SRC_PARAM;

//远程控制参数
typedef struct tag_Telecontrol_Param
{
	char	szIP[IP_SIZE];
	UINT	nPort;
}TELECONTROL_PARAM,*PTELECONTROL_PARAM;


//屏幕广播参数
typedef struct tag_BroadCast_SRC_Param
{
	BYTE	bFull;				//接收端是否适用全屏
	BYTE	bFixShow;			//接收端1;1显示
	BYTE	bAutoFoucs;			//自动对焦到教师机的鼠标
	BYTE	bAutoSetFull;		//是否自动设置分辨率
	BYTE	eCapMode;			//抓屏模式，全屏，指定区域，指定窗口
	RECT	oRect;				//主屏区域
	HWND	hTraceWnd;			//抓屏的窗口
	BYTE	eCmpType;			//压缩方式
	BYTE	bCanRecord;			//是否可以录制
	int		nDemoStuID;			//演示的学生ID
	int		nDataScreenID;		//要组播的学生ID,=0标识教师机
	BYTE	bWithAudio;
	BYTE	bSaveAsAVI;			// 是否保存为AVI，FALSE：WMV
	BYTE	bSupport3D;			//是否支持3d变化
	WORD	bResever;
}BROADCAST_SRC_PARAM,*PBROADCAST_SRC_PARAM;

//键盘消息
typedef struct tag_Key_Msg_Info
{
	BYTE bVkCode;
	BYTE bFlag;
	DWORD dwScanCode;
}KEY_MSG_INFO,*PKEY_MSG_INFO;

//鼠标消息
typedef struct tag_Mouse_Info
{
	WORD wX;
	WORD wY;
	DWORD dwMsg;
}MOUSE_INFO,*PMOUSE_INFO;

//屏幕信息
typedef struct tag_Remote_Screen_Info
{
	UINT	nSize;				//整个缓冲的大小
	UINT	nUnCompressSize;	//位图压缩之前的大小
	WORD	wSrcX;				//屏幕宽度
	WORD	wSrcY;				//屏幕长度
	//	WORD	nPlatteSize;		//调色板大小
	BYTE	bCompressType;		//压缩类型
	WORD	wLeft;				//目标区域
	WORD	wTop;
	WORD	wRight;
	WORD	wBottom;	
	WORD	wCursorX;
	WORD    wCursorY;
	//后面跟着数据
} REMOTE_SCREEN_INFO,*PREMOTE_SCREEN_INFO;


class CMonitorScreenParam
{
public:
	int nSrcCount;
	int nSwitchInterVal;
	BOOL bCtrl;
};

class CAudioBCParam
{
public:
	BOOL bUseMp3;
	int nSamplesPerSec;
	int nChannelCount;
	int nBuffersCount;
	char szDeviceName[MAX_PATH];
	char szLineName[MAX_PATH];
};
#pragma	pack (pop)

//定义套接字类型
#define SOCK_REQUEST_MONITORED		1		//请求被监视，用于远程监视
#define SOCK_REQUEST_CONTROL		2		//请求控制其他机器屏幕，主要用于学生演示
#define SOCK_REQUEST_CONTROLED		3		//请求被控制，用于远程控制

//定义消息类型
#define WM_DISP				WM_USER + 3000 
#define WM_PROCESS_MSG			WM_DISP + 1
#define WM_SCREEN_DATA			WM_DISP + 2		//屏幕数据
#define WM_SCREEN_SIZE_CHANGED	WM_DISP + 5		//屏幕大小发生变化
#define WM_SOCKET_CONNECT		WM_DISP + 6
#define WM_SWITCH_SCREEN		WM_DISP + 7
#define WM_DETACCH_SCREEN		WM_DISP + 8
#define WM_EXIT_MONITOR			WM_DISP + 9
#define WM_CAP_WINLOGON_SRC		WM_DISP + 10	

#define WM_BEGIN_CTRL			WM_DISP + 11
#define WM_MOUSE_EVENT			WM_DISP + 12
#define WM_KEY_ENVENT			WM_DISP + 13

#define WM_RESET_PACK_INDEX		WM_DISP + 14
#define WM_RECORD_SCREEN		WM_DISP + 15
#define WM_ZOOM_BMP_SIZE		WM_DISP + 16		//放缩位图
#define WM_GET_SCREEN_NAME		WM_DISP + 17		//获取当前显示名称
#define WM_SCREEN_DATA_CTRL		WM_DISP + 18		//控制屏幕监视的数据，暂停，开始等
#define WM_SEND_CAD				WM_DISP + 19     //发送CTRL + ALT + DEL
#define WM_END_CTRL				WM_DISP + 20		//结束控制
#define WM_GET_BC_SCREEN_WND_INFO  WM_DISP + 21	
#define WM_END_BC_SCREEN		WM_DISP + 22		//结束广播
#define WM_SOCK_LISTEN_CMPLT	WM_DISP + 23		//套接字侦听完毕
#define WM_SEND_HOT_KEY			WM_DISP + 24
#define WM_AUDIO_CMD			WM_DISP + 25
#define WM_SCREEN_SNAP			WM_DISP + 26
#define WM_BEGIN_RECORD			WM_DISP + 27
#define WM_SCREEN_RCV_TYPE		WM_DISP + 28		
#define WM_BEGIN_RECORD_MULTI	WM_DISP + 29
#define WM_GET_AUDIO_STATE		WM_DISP + 30		//获取当前语音状态
#define WM_AP_LOCK_MOUSE		WM_DISP + 31		//控制时，沟通的一些常规命令
#define WM_AP_LOCK_KEY			WM_DISP + 32
#define WM_CURSOR_POS			WM_DISP + 33
#define WM_SOCK_BC_CMPLT		WM_DISP + 34
#define WM_SCREEN_CTRL_NOTIFY	WM_DISP + 35
#define WM_TOOLBAR_SHOWTXT		WM_DISP + 36
#define WM_REQUEST_CMD_CODE		WM_DISP + 37
#define WM_REQUEST_LOST_PACK	WM_DISP + 38
#define SATURN_TITLE_MARK  "SaturnUse"



//加密验证返回值
#define AS_SEC_NO_ERR			0		//成功
#define AS_SEC_NO_FILE			1		//没有对应的加密文件
#define AS_SEC_FILE_SIZE_ERR	2		//加密文件大小不等
#define AS_SEC_FILE_DATA_ERR	4		//加密文件数据错误
#define AS_SEC_NO_DEBUG_ENV		8		//没有调试环境

//调用模块代号
#define AS_MARS_SVR_SEC_CODE		0		//教学网服务端
#define AS_MARS_CLT_SEC_CODE		1		//教学网客户端
#define AS_STAURN_SVR_SEC_CODE		2		//机房服务器端		
#define AS_STAURN_CLT_SEC_CODE		3		//机房客户端		
#define AS_POLARIS_SVR_SEC_CODE		4		//电子警察服务器端
#define AS_POLARIS_CLT_SEC_CODE		5		//电子警察客户端
#define AS_FREEINNET_SVR_SEC_CODE	6		//网络管理专家服务端
#define AS_FREEINNET_CLT_SEC_CODE	7		//网络管理专家客户端


typedef struct tag_Init_APNETOP_SUB
{
	int nSize;
	DWORD dwModuleCode;		//调用模块代号
	BYTE* pSecData;		//	//加密数据
	int   nSecDataLen;		//加密数据长度
	union
	{
		DWORD dwSecErrCode;	//返回错误代码
		DISPLAY_CMD eDispCmd;		//命令:
	};
}INIT_APNETOP_SUB,*PINIT_APNETOP_SUB;




#endif