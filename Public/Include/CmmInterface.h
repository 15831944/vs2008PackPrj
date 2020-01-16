
#ifndef _ICLTLOGCALLBACK_HEADER
#define _ICLTLOGCALLBACK_HEADER
//----------------------------
//资产信息
struct _AA_DLL_EXPORT_ ICltLogCallback
{
public:
	//资产更新
	virtual void OnAssetsUpdated(int nType,void* pHard){ASSERT(0);}
	//文件操作日志
	virtual void OnFileSystemLog(void* pLogList, int nCount){ASSERT(0);}
	//网址
	virtual void OnUrlAccessed(int nLocal,int nRemote,CString sWebstation,CString sUrl){ASSERT(0);}
	//程序创建
	virtual void OnProcessCreated(DWORD dwPid,CString sPath,BOOL bAllowed){ASSERT(0);}
	//程序关闭
	virtual void OnProcessShutdown(DWORD dwPid){ASSERT(0);}
	//屏幕日志
	virtual void OnScreenCaptrued(void*){ASSERT(0);}
	//打印日志
	virtual void OnPrinted(void*){ASSERT(0);}
	//打印作业信息
	virtual void OnJobInfo(DWORD dwType, void*){ASSERT(0);}
	//打印事件日志
	virtual void OnPrintedEvent(void*,BOOL bAllowed){ASSERT(0);}
	//设备插入日志
	virtual void OnDeviceAttached(void*){ASSERT(0);}
	//设备插入日志
	virtual void OnDeviceDettached(void*){ASSERT(0);}
	//网络流量日志
	virtual void OnNetStreamLog(void* ,int){ASSERT(0);}
	// web邮件发送日志
	virtual void OnSendWebMailLog(void* ){ASSERT(0);}
	
	//本地提示
	virtual void OnTipMsg(int,CString,DWORD,DWORD){ASSERT(0);}

	//预警管理
	//访问非法网址
	virtual void OnUrlAlarmed(CString sUrl){ASSERT(0);}
	//使用非法程序
	virtual void OnProcessAlarmed(){ASSERT(0);}
	//硬件资产变化
	virtual	void OnHardwareChanged(){ASSERT(0);}
	//软件资产变化
	virtual void OnSoftwareChanged(){ASSERT(0);}
	//ip地址修改
	virtual void OnIpChanged(){ASSERT(0);}
	//机器名修改
	virtual void OnHostNameChaned(){ASSERT(0);}
	//上传日志
	virtual int OnLogUpload(int,int,const void*,int,void*){ASSERT(0);return 0;}
	//本地查询
	virtual int OnLogCallback(int,int,const void* ,int){ASSERT(0);return 0;}
};


//模块调用主框架的接口
struct _AA_DLL_EXPORT_ IModuleCallback
{
public:
	//通知主框架
	virtual int NotifyMainFrm(DWORD dwCmd,WPARAM w,LPARAM l){ASSERT(0);return 0;}
	//通知网络线程
	virtual int NotifyNetTh(DWORD dwCmd,WPARAM w,LPARAM l){ASSERT(0);return 0;}
	//通知网络辅助线程
	virtual int NotifyNetHelperTh(DWORD dwCmd,WPARAM w,LPARAM l){ASSERT(0);return 0;}
	//通过辅助网络线程发送数据
	virtual int SendByNetHelper(int nCmd,int nSub,BYTE* pData,int nSize,int nCompress = -1 ){ASSERT(0);return 0;}
	//通过主网络线程发送数据
	virtual int SendByNetTh(int nCmd,int nSub,BYTE* pData,int nSize,int nCompress = -1 ){ASSERT(0);return 0;}
	//查询当前用户信息
	virtual void QueryUserInfo(int& nUserId,int& nAccountId,CString& sName){ASSERT(0);}
	//运行当前session的界面进程
	virtual BOOL RunUiProcess(){ASSERT(0);return 0;}
	//请求提交打印任务
	virtual BOOL OnRequestSubmitPrintJob(int nId){ASSERT(0);return 0;}
	//上传日志信息反馈
	virtual void OnUploadLogResult(int type,BOOL bSuc,void* other = 0){ASSERT(0);}
	//日志上传
	virtual void OnUploadLog(int type,void* log){ASSERT(0);}
	//本地提示
	virtual void OnTipMsg(int,CString,DWORD,DWORD){ASSERT(0);}
	//网络超时
	virtual void OnSocketTimeOut(){ASSERT(0);}
	//网络关闭
	virtual void OnSocketClosed(){ASSERT(0);}
};
//管道接口
struct _AA_DLL_EXPORT_ IPipeCallback
{
public:
	virtual void OnPipeRcv(void*,BYTE* pData,int nSize){ASSERT(0);}
	virtual int OnNewPipe(void* ){ASSERT(0);return 0;}
	virtual void OnPipeClose(void*){ASSERT(0);}
};
//应用程序接口
struct _AA_DLL_EXPORT_ IAppMgrCallback
{
public:
	//程序枚举
	virtual void OnPopProcess(void*){ASSERT(0);}
	//服务枚举
	virtual void OnPopSvc(void*){ASSERT(0);}
	//共享枚举
	virtual void OnPopShare(void*){ASSERT(0);}
	//组信息枚举
	virtual void OnPopGroup(void*){ASSERT(0);}
	//用户信息枚举
	virtual void OnPopUser(void*){ASSERT(0);}
	//启动项枚举
	virtual void OnPopStartItem(void*){ASSERT(0);}
	//磁盘枚举
	virtual void OnPopDiskDriver(void*){ASSERT(0);}
	//设备枚举
	virtual void OnPopDevice(void*){ASSERT(0);}
	//设备枚举
	virtual void onPopFirewall(void*){ASSERT(0);}
	//设备枚举
	virtual void onPopAntiSofeware(void*){ASSERT(0);}
};

//屏幕接口
struct _AA_DLL_EXPORT_ IScreenCallback
{
public:
	//屏幕信息;
	virtual void OnScreenInit(){ASSERT(0);}
	virtual void OnPopScreen(){ASSERT(0);}
	virtual void OnScreenExit(){ASSERT(0);}
};
//文件传输
struct _AA_DLL_EXPORT_ ITransmitFileCallback
{
public:
	virtual void OnBegin(int ,char* ){ASSERT(0);}
	virtual void OnTransmit(int ,char*,int){ASSERT(0);}
	virtual void OnFinished(int ,char*){ASSERT(0);}
	virtual void OnCompleted(int){ASSERT(0);}
	virtual void OnError(int,int){ASSERT(0);}
};

//-----------------------------------------------
//qt界面回调接口
struct _AA_DLL_EXPORT_ IQtAppMgrUiCallback
{
public:
	virtual void OnPopData(void*){ASSERT(0);}
};

//qt界面条用网络线程接口
struct _AA_DLL_EXPORT_ IQtNetThreadCallback
{
public:
	//发送数据
	virtual int SendToSvr(int nCmd,int nSub,BYTE* pData,int nSize,int nCompress = -1 ){ASSERT(0);return 0;}
	
};

//打印管理系统打印计费
struct _AA_DLL_EXPORT_ IPrtStationPrintCallback
{
	virtual int OnPrinted(int){ASSERT(0);return 0;}
	virtual int OnNewPagePrinted(int){ASSERT(0);return 0;}
	virtual int OnPrintedCmplt(int){ASSERT(0);return 0;}
	virtual int OnPrintedErr(int ,int){ASSERT(0);return 0;}
};

//服务进程与子模块之间的接口
struct _AA_DLL_EXPORT_ ISvcCallback
{
	//启动当前session的界面进程
	virtual int OnRunCurrentSessionUiProcess(char* ){ASSERT(0);return 0;}
	
	// [12/27/2011 Songqx] clienthelper通知服务进程不存在时是否需要创建进程
	virtual void SetShowUIFlag(BOOL bShowUI){ASSERT(0);}
	
	//用户登陆事件
	virtual void OnSessionChange(DWORD dwEventType,LPVOID lpEventData){ASSERT(0);}
	//
	virtual void SetCallback(ISvcCallback* ){ASSERT(0);}
	//设备插入日志
	virtual void OnDeviceAttached(void*){ASSERT(0);}
	//设备插入日志
	virtual void OnDeviceDettached(void*){ASSERT(0);}
	
};
//----------------------------------------------
#endif