#ifndef _Snmp_Interface_h_27E39E90_744D_4560_95FD_F5C6B2BBC9A6_
#define _Snmp_Interface_h_27E39E90_744D_4560_95FD_F5C6B2BBC9A6_

#include <snmp.h>
#pragma comment(lib,"snmpapi.lib")

#include <mgmtapi.h>
#pragma comment(lib,"mgmtapi.lib")

#include <winsock2.h>
#include <IPExport.h>
#include <icmpapi.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

#include "PrinterMIBv2_Def.h"
#include "PrinterAlert_Def.h"
#include "RFC1213MIB_Def.h"
#include "HostResourcesMIB_Def.h"
#include "../CloudWebService/HttpClient.h"

#define Max_Paper_Index	5
#define Max_Ink_Index	5

#define Max_Tray_Count	10
#define Max_Ink_Count	5

#define SNMP_CMMT_SIZE	100
#define SNMP_CMMT_COUNT 50
struct tag_Snmp_Desc
{
	char szCmmt[SNMP_CMMT_SIZE];
};

enum FX_DC_SCP_STATUS
{
	FX_DC_OP_IDLE = 3,

	FX_DC_DEVICE_OTHER = 11,  
	FX_DC_DEVICE_UNKNOWN = 12,
	FX_DC_DEVICE_IDLE = 13,
	FX_DC_DEVICE_PRINTING = 14,
	FX_DC_DEVICE_WARMUP =15,
	FX_DC_DEVICE_SCANING =16,
	FX_DC_DEVICE_COPYING =17,
	FX_DC_DEVICE_FAXING =18,

	FX_DC_FIR_PAPER_OK = 21, 
	FX_DC_FIR_PAPER_LESS ,
	FX_DC_FIR_PAPER_NO,

	FX_DC_SEC_PAPER_OK = 31, 
	FX_DC_SEC_PAPER_LESS,
	FX_DC_SEC_PAPER_NO,

	FX_DC_FIR_INK_OK = 41,
	FX_DC_FIR_INK_LESS,
	FX_DC_FIR_INK_NO,

	FX_DC_ERR = 50,

	FX_DC_NET_ERR = 60,

	FX_DC_ALERT = 70	//预警信息
};

enum FX_DC_INK_CMYK
{
	FX_DC_INK_NULL = 0,		//空墨盒
	FX_DC_INK_CYAN = 1,		//青
	FX_DC_INK_MAGENTA = 2,	//品红
	FX_DC_INK_YELLOW = 3,	//黄
	FX_DC_INK_BLACK = 4		//黑
};

class CSCPStatus
{
public:
	FX_DC_SCP_STATUS m_eOPStatus;
	FX_DC_SCP_STATUS m_eDeviceStatus;
	FX_DC_SCP_STATUS m_ePrintStatus;
	FX_DC_SCP_STATUS m_eCopyStatus;
	FX_DC_SCP_STATUS m_eScanStatus;
	FX_DC_SCP_STATUS m_eFaxStatus;
	FX_DC_SCP_STATUS m_eFirPaper;
	FX_DC_SCP_STATUS m_eSecPaper;
	FX_DC_SCP_STATUS m_eFirInk;
	FX_DC_SCP_STATUS m_aryPaper[Max_Paper_Index];
	FX_DC_SCP_STATUS m_aryInk[Max_Ink_Index];
};


/*
SNMP需要的OID结点数据
一,纸盒信息
1.纸盒个数
2.纸盒纸型
3.纸盒最大容量
4.纸盒剩余容量

二,墨盒信息
1.墨盒数量
2.墨盒颜色
3.墨盒最大容量
4.墨盒剩余容量

三,设备状态
1.设备的当前工作状态,如空闲,预热,工作,打印中,扫描中等等.
2.设备的当前硬件状态,如卡纸,纸盒抽出,墨盒抽出,缺纸,少纸,缺墨,少墨,盖板打开等等

四,作业信息
1.当前作业信息
A.当前作业总数
B.当前作业类型
C.当前作业操作状态
D.当前作业名称
E.当前作业用户名
F.当前作业纸张数

2.历史作业信息
A.历史作业总数
B.历史作业类型
C.历史作业操作状态
D.历史作业名称
E.历史作业用户名
F.历史作业纸张数
G.历史作业完成状态
*/

//作业类型
enum SCP_JOB_TYPE
{
	SCP_JOB_TYPE_PRINT = 1,
	SCP_JOB_TYPE_COPY = 2,
	SCP_JOB_TYPE_SCAN = 3,
	SCP_JOB_TYPE_FAX = 4,
	SCP_JOB_TYPE_RECEIVE_JOB = 5,
	SCP_JOB_TYPE_SEND_JOB = 6,
	SCP_JOB_TYPE_MULTI = 7,
	SCP_JOB_TYPE_OTHER = 8
};

//作业状态
enum SCP_JOB_STATUS
{
	SCP_JOB_STATUS_START = 0,			//作业开始
	SCP_JOB_STATUS_CANCEL_REQUEST = 1,	//请求取消
	SCP_JOB_STATUS_PENDING = 2,			//等待中
	SCP_JOB_STATUS_PROCESSING = 3,		//处理中
	SCP_JOB_STATUS_SENDING = 4,			//发送中
	SCP_JOB_STATUS_RECEIVING = 5,		//接收中
	SCP_JOB_STATUS_PRINTING = 6,		//打印中
	SCP_JOB_STATUS_SCANNING = 7,		//扫描中
	SCP_JOB_STATUS_PROCESING_STOPED = 8,	//处理中-被中止
	SCP_JOB_STATUS_COMPLETED = 9,		//已完成
	SCP_JOB_STATUS_CAUTION = 10,		//警告
	SCP_JOB_STATUS_ABORTED = 11,		//已放弃
	SCP_JOB_STATUS_CANCELED = 12,		//已取消
	SCP_JOB_STATUS_SEND_COMPLETED = 13,	//发送完成
	SCP_JOB_STATUS_RECE_COMPLETED = 14	//接收完成
};

//作业校验结果
enum SCP_JOB_VERIFY_RESULT
{
	SCP_JOB_VERIFY_RESULT_FAIL = 0,		//校验失败
	SCP_JOB_VERIFY_RESULT_SUCC = 1,		//检验成功
	SCP_JOB_VERIFY_RESULT_PENDING = 2	//校验待确认
};

//作业信息
typedef struct tag_SCP_JobInfo
{
	int				nJobId;			//作业ID
	char			cJobName[MAX_PATH];	//作业名称
	double			fJobStartTime;	//作业开始时间
	double			fJobEndTime;	//作业结束时间	
	int				nTotalPage;		//作业总页数	
	int				nPage;			//作业单个副本页数	
	int				nCopies;		//作业份数
	int				nPrintedPage;	//作业已经打印页数
	int				nScanedPage;	//作业已经扫描页数
	int				nMediaSize;		//作业纸型,如A4
	bool			bColor;			//作业是否彩色
	SCP_JOB_TYPE	JobType;		//作业类型
	SCP_JOB_STATUS	JobStatus;		//作业状态
	char			cDesc[MAX_PATH];	//状态说明
	char			szPrintTime[20];	//作业时间
	char			szOid[MAX_PATH];	//作业的OID
	SCP_JOB_VERIFY_RESULT	JobVerifyResult;	//检验结果
}SCP_JobInfo, *PSCP_JobInfo;
typedef list<SCP_JobInfo> SCP_JobInfo_List, *PSCP_JobInfo_List;
typedef SCP_JobInfo_List::iterator SCP_JobInfo_List_I;

typedef struct tag_PrintJobInfo
{
	int nIdentifier;
	int nDocPrintedPages;
	double dPrintTime;
	char szPrintTime[20];
	char szDocName[MAX_PATH];
	char szOid[MAX_PATH];

}PRINTJOBINFO, *PPRINTJOBINFO;

typedef vector<PRINTJOBINFO> VPRINTJOBINFO;
typedef VPRINTJOBINFO::iterator VPRINTJOBINFOI;


typedef struct tag_PrinterMeterInfo
{
	int nPrintId;			//抄表的打印点id
	double fRecordPrintMeterTime; //抄表的时间
	int nOpAdminId;			//抄表操作管理员ID

	//总数，从打印机抄上来的总数
	int nAllTotalPage;			//总页数
	int nAllTotalDistance;		//总长度
	int nA3HBTotalPage;			//A3黑白总页数
	int nA3ColorTotalPage;		//A3彩色总页数
	int nA4HBTotalPage;			//A4黑白总页数
	int nA4ColorTotalPage;		//A4彩色总页数

	//打印计数
	int nA4PrintTotalPage;	//A4打印总计数
	int nA4PrintColorPage;	//A4打印彩色总计数
	int nA4PrintHBPage;	    //A4打印黑白总计数
	int nA3PrintTotalPage;	//A3打印总计数
	int nA3PrintColorPage;	//A3打印彩色总计数
	int nA3PrintHBPage;	    //A3打印黑白总计数
	int nOtherPrintTotalPage;	//其它纸型打印总计数
	int nOtherPrintColorPage;	//其它纸型打印彩色总计数
	int nOtherPrintHBPage;		//其它纸型打印黑白总计数

	int nPrintDistance; //打印长度
	int nPrintArea;		//打印面积


	//复印计数
	int nA4CopyTotalPage;	//A4复印总计数
	int nA4CopyColorPage;	//A4复印彩色总计数
	int nA4CopyHBPage;	    //A4复印黑白总计数
	int nA3CopyTotalPage;	//A3复印总计数
	int nA3CopyColorPage;	//A3复印彩色总计数
	int nA3CopyHBPage;	    //A3复印黑白总计数
	int nOtherCopyTotalPage;	//其它纸型复印总计数
	int nOtherCopyColorPage;	//其它纸型复印彩色总计数
	int nOtherCopyHBPage;	//其它纸型复印黑白总计数

	int nCopyDistance;			//复印长度
	int nCopyArea;				//复印面积

	//扫描计数
	int nScanTotalPage;			//扫描总计数
	int nScanBigColorPage;		//扫描大幅彩色总计数
	int nScanBigHBPage;			//扫描大幅黑白总计数
	int nScanSmallColorPage;	//扫描小幅彩色总计数
	int nScanSmallHBPage;		//扫描小幅黑白总计数

	//传真计数
	int nA4FaxTotalPage;		//A4传真总计数
	int nA4FaxColorPage;		//A4传真彩色总计数
	int nA4FaxHBPage;	    //A4传真黑白总计数
	int nA3FaxTotalPage;		//A3传真总计数
	int nA3FaxColorPage;		//A3传真彩色总计数
	int nA3FaxHBPage;	    //A3传真黑白总计数
	int nOtherFaxTotalPage;	//其它纸型传真总计数
	int nOtherFaxColorPage;	//其它纸型传真彩色总计数
	int nOtherFaxHBPage;		//其它纸型传真黑白总计数

	//其它操作计数
	int nOtherOpTotalPage;	//其它操作总计数
	int nOtherOpColorPage;	//其它操作彩色总计数
	int nOtherOpHBPage;	    //其它操作黑白总计数

	//以下是为了有等效计数的机器所加
	int nEquivalentHBPrintPage; //等效黑白打印计数
	int nEquivalentHBCopyPage;	//等效黑白复印计数
	int nEquivalentHBFaxPage;	//等效黑白传真计数
	int nEquivalentColorPrintPage; //等效彩色打印计数
	int nEquivalentColorCopyPage;	//等效彩色复印计数
	int nEquivalentColorFaxPage;	//等效彩色传真计数
	

	//纸盒信息
	int nPaper1MaxCapacity;
	int nPaper1CurrentLevel;
	char cPaper1LevelPercent[50];
	int nPaper2MaxCapacity;
	int nPaper2CurrentLevel;
	char cPaper2LevelPercent[50];
	int nPaper3MaxCapacity;
	int nPaper3CurrentLevel;
	char cPaper3LevelPercent[50];
	int nPaper4MaxCapacity;
	int nPaper4CurrentLevel;
	char cPaper4LevelPercent[50];
	int nPaperBypassMaxCapacity;
	int nPaperBypassCurrentLevel;
	char cPaperBypassLevelPercent[50];

	//墨盒粉信息:Cyan\Magenta\Yellow\Black[青色、品红、黄、黑]
	int nTonerCyanMaxCapacity;
	int nTonerCyanLevel;
	char cTonerCyanLevelPercent[50];
	int nTonerMagentaMaxCapacity;
	int nTonerMagentaLevel;
	char cTonerMagentaLevelPercent[50];
	int nTonerYellowMaxCapacity;
	int nTonerYellowLevel;
	char cTonerYellowLevelPercent[50];
	int nTonerBlackMaxCapacity;
	int nTonerBlackLevel;
	char cTonerBlackLevelPercent[50];

	//感光鼓单元:OPC
	int nOPCCyanMaxCapacity;
	int nOPCCyanLevel;
	char cOPCCyanLevelPercent[50];
	int nOPCMagentaMaxCapacity;
	int nOPCMagentaLevel;
	char cOPCMagentaLevelPercent[50];
	int nOPCYellowMaxCapacity;
	int nOPCYellowLevel;
	char cOPCYellowLevelPercent[50];
	int nOPCBlackMaxCapacity;
	int nOPCBlackLevel;
	char cOPCBlackLevelPercent[50];

	//显影单元:Developer
	int nDeveloperCyanMaxCapacity;
	int nDeveloperCyanLevel;
	char cDeveloperCyanLevelPercent[50];
	int nDeveloperMagentaMaxCapacity;
	int nDeveloperMagentaLevel;
	char cDeveloperMagentaLevelPercent[50];
	int nDeveloperYellowMaxCapacity;
	int nDeveloperYellowLevel;
	char cDeveloperYellowLevelPercent[50];
	int nDeveloperBlackMaxCapacity;
	int nDeveloperBlackLevel;
	char cDeveloperBlackLevelPercent[50];
}PrinterMeterInfo, *PPrinterMeterInfo;

class ISCPStatusInterface
{
public:
	virtual void OnStatus(int,char*){ASSERT(0);}
	virtual void OnSCPIdle(int nSec){ASSERT(0);}
	virtual void OnMeterInfo(PPrinterMeterInfo){ASSERT(0);}
	virtual void OnJobInfo(PSCP_JobInfo){ASSERT(0);}
	virtual void OnJobInfoList(PSCP_JobInfo_List){ASSERT(0);}
};

class CSnmpOP
{
public:
	CSnmpOP();
	~CSnmpOP();

protected:
	char* SNMP_AnyToStr(AsnObjectSyntax *sAny, UINT& uLen);	//uLen:字符串长度，包含'\0'
	char* SNMP_AnyToHexStr(AsnObjectSyntax *sAny, UINT& uLen);	//uLen:字符串长度，包含'\0'
	BOOL GetRequest(char* pszOid,int& nVal);
	BOOL GetRequest(char* pszOid,unsigned int& nVal);
    BOOL GetRequest(char* pszOid, std::string& sOutStr);// add by zmm, 2019.1.10
	BOOL GetRequest(char* pszOid,unsigned char* pszStr,int nStrBufferLen);
	BOOL GetHexRequest(char* pszOid,unsigned char* pszStr,int nStrBufferLen);
	BOOL GetNextRequest(char* pszOid,int& nVal);
	BOOL GetRequestStr(char* pszOid,unsigned char* pszStr,int& nStrBufferLen);//nStrBufferLen:字符串长度，包含'\0'
	BOOL GetNextRequestStr(char* pszOid,unsigned char* pszStr,int& nStrBufferLen);//nStrBufferLen:字符串长度，包含'\0'
	BOOL GetNextRequestStrEx(char* pszOid,char* pszStr,int nStrBufferLen,char* pszOidNext,int nOidNextBufferLen);
	int GetOidEndNumber(char* pszOid);
	BOOL OidBeginWithStr(char* pszOid, char* pszStr);
	void TimeTicksToDateTimeStr(TimeTicks timeticks, char* pszDateTimeStr);
	CString GetStrByOid(CString szOid);
	BOOL CheckIPActive();
	BOOL CheckSupportSnmp();
	CString EncryptOID(IN CString szOrgString);
	CString DecryptOID(IN CString szEncryptString);
	char* CStringToChar(CString& szString);

public:
	void SetConnect(CString szIP, CString szCommunity = "public");
	BOOL InitSnmp();
	void ReleaseSnmp();
	void SetCallBack(ISCPStatusInterface* pCallBack);
	CString GetPrtIP();
	void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen );  

protected:
	BOOL m_bIsActiveIP;
	BOOL m_bIsSupportSnmp;
	ISCPStatusInterface* m_pCallBack;
	LPSNMP_MGR_SESSION m_lpMgrSession;
	CString	m_szIP;
	CString m_szCommunity;
};

class CSnmpJobMonitor : public CSnmpOP
{
public:
	CSnmpJobMonitor();
	virtual ~CSnmpJobMonitor();
	virtual void CheckJob() = NULL;
	virtual int CheckLastPrintCount() = NULL;
	virtual int CheckLastPrintCount_Color() = NULL;
	virtual int CheckLastPrintCount_WB() = NULL;
	virtual int CheckLastCopyCount() = NULL;
	virtual int CheckLastCopyCount_Color() = NULL;
	virtual int CheckLastCopyCount_WB() = NULL;
	virtual int CheckLastScanCount() = NULL;
	virtual int CheckLastScanCount_Color() = NULL;
	virtual int CheckLastScanCount_WB() = NULL;
	virtual int CheckLastFaxCount() = NULL;
	virtual int CheckLastFaxCount_Color() = NULL;
	virtual int CheckLastFaxCount_WB() = NULL;
	virtual int CheckLastPageCount_A3() = NULL;
	virtual int CheckLastPageCount_A4() = NULL;
	void Release();
protected:
	typedef CMap<int, int, PSCP_JobInfo, PSCP_JobInfo> CJobMap;
	CJobMap m_oJobMap;
	typedef CList<PSCP_JobInfo> CJobList;
	CJobList m_oJobList;
	int m_nLastPrintCount;
	int m_nLastPrintCount_Color;
	int m_nLastPrintCount_WB;
	int m_nLastCopyCount;
	int m_nLastCopyCount_Color;
	int m_nLastCopyCount_WB;
	int m_nLastScanCount;
	int m_nLastScanCount_Color;
	int m_nLastScanCount_WB;
	int m_nLastFaxCount;
	int m_nLastFaxCount_Color;
	int m_nLastFaxCount_WB;
	int m_nA3PageCount;
	int m_nA4PageCount;
};

//打印机抄表类
class CSnmpPrinterMeterMonitor : public CSnmpOP
{
public:
	CSnmpPrinterMeterMonitor();
	virtual ~CSnmpPrinterMeterMonitor();
	void CheckMeterInfo();
	BOOL GetLastMeterInfo(PrinterMeterInfo* pInfo);	//获取最后一次正确的抄表信息
	void SetMeterInfo(PrtInputEntryMap& map);
	void SetMeterInfo(PrtMarkerSuppliesEntryMap& map);
	void SetBrandType(BRAND_TYPE eType);
	void Release();

protected:
	virtual BOOL GetMeterInfo();		//本函数只能在CheckMeterInfo只内部调用
	void InitMeter();
	void ResetPaperMeter();
	BOOL IsMeterValid(PrinterMeterInfo* pInfo);
	BRAND_TYPE m_eType;	//品牌类型
	PrinterMeterInfo m_oMeterInfo;
	PrinterMeterInfo m_oLastMeterInfo;	//最后一次正确的抄表信息
	CCriticalSection2 m_cs4MeterInfo;
	BOOL m_bFirstSucc;	//第一次抄表成功标识

protected:
	void InitOID();
	CString m_szPrtMarkerLifeCountOID;
};

//打印机监视类
class CSnmpMonitorHelper : public CSnmpOP
{
public:
	CSnmpMonitorHelper();
	virtual ~CSnmpMonitorHelper();

public:
	void Init();
	void Release();
	void SetPrinterMeterMonitor(CSnmpPrinterMeterMonitor* pMeter);
	void SetPaperConfig(CString sConfig, int nMin);
	void SetInkConfig(CString sConfig, int nMin);
	void CheckMonitor();
	void SetBrandType(BRAND_TYPE eType);

protected:
	//通过SNMP获取OID信息
	virtual BOOL GetAll();
	virtual BOOL GetGeneral();
	virtual BOOL GetStorageRef();
	virtual BOOL GetDeviceRef();
	virtual BOOL GetCover();
	virtual BOOL GetLocalization();
	virtual BOOL GetInput();
	virtual BOOL GetOutput();
	virtual BOOL GetMarker();
	virtual BOOL GetMarkerSupplies();
	virtual BOOL GetMarkerColorant();
	virtual BOOL GetMediaPath();
	virtual BOOL GetChannel();
	virtual BOOL GetInterpreter();
	virtual BOOL GetConsoleDisplayBuffer();
	virtual BOOL GetConsoleLight();
	virtual BOOL GetAlert();
    virtual BOOL GetPrinterTable(); // 获得打印机表, add by zmm, 2019.1.11

	//检查SNMP获取到的OID信息
	virtual void CheckAll();
	virtual void CheckGeneral();
	virtual void CheckStorageRef();
	virtual void CheckDeviceRef();
	virtual void CheckCover();
	virtual void CheckLocalization();
	virtual void CheckInput();
	virtual void CheckOutput();
	virtual void CheckMarker();
	virtual void CheckMarkerSupplies();
	virtual void CheckMarkerColorant();
	virtual void CheckMediaPath();
	virtual void CheckChannel();
	virtual void CheckInterpreter();
	virtual void CheckConsoleDisplayBuffer();
	virtual void CheckConsoleLight();
	virtual void CheckAlert();
    virtual BOOL CheckPrinterTable(); // 打印机表, add by zmm, 2019.1.11

	//清理SNMP获取到的OID信息
	virtual void ClearAll();
	virtual void ClearGeneral();
	virtual void ClearStorageRef();
	virtual void ClearDeviceRef();
	virtual void ClearCover();
	virtual void ClearLocalization();
	virtual void ClearInput();
	virtual void ClearOutput();
	virtual void ClearMarker();
	virtual void ClearMarkerSupplies();
	virtual void ClearMarkerColorant();
	virtual void ClearMediaPath();
	virtual void ClearChannel();
	virtual void ClearInterpreter();
	virtual void ClearConsoleDisplayBuffer();
	virtual void ClearConsoleLight();
	virtual void ClearAlert();
    virtual BOOL ClearPrinterTable(); // 打印机表, add by zmm, 2019.1.11

	//扩展功能检测,在PrintMIB和打印机私有MIB的基础上,进行扩展功能的发掘
	virtual void CheckMeter();		//扩展功能(非标准PrintMib)，抄表
	virtual void CheckAlertEx();	//扩展功能(非标准PrintMib)，过滤故障代码和其它预警信息
	virtual BOOL IsFalutInfo(CString szDetial);	//判断信息是否为故障信息
	virtual CString GetFaultCode(CString szDetial);	//获取故障代码
	virtual void CheckFaultCode();	//单独的故障代码检测，从打印机的私有节点获取

protected:
	//获取纸盒纸型
	CString GetPaperTypeByPrtInputEntry(PPrtInputEntry pEntry);
	void InitOID();
	
protected:
	//友元打印机抄表信息类
	CSnmpPrinterMeterMonitor* m_pPrinterMeterMonitor;
	//保存通过SNMP获取OID信息的MAP
	PrtGeneralEntryMap m_oGeneralMap;
	PrtStorageRefEntryMap m_oStorageRefMap;
	PrtDeviceRefEntryMap m_oDeviceRefMap;
	PrtCoverEntryMap m_oCoverMap;
	PrtLocalizationEntryMap m_oLocalizationMap;
	PrtInputEntryMap m_oInputMap;
	PrtOutputEntryMap m_oOutputMap;
	PrtMarkerEntryMap m_oMarkerMap;
	PrtMarkerSuppliesEntryMap m_oMarkerSuppliesMap;
	PrtMarkerColorantEntryMap m_oMarkerColorantMap;
	PrtMediaPathEntryMap m_oMediaPathMap;
	PrtChannelEntryMap m_oChannelMap;
	PrtInterpreterEntryMap m_oInterpreterMap;
	PrtConsoleDisplayBufferEntryMap m_oConsoleDisplayBufferMap;
	PrtConsoleLightEntryMap m_oConsoleLightMap;
	PrtAlertEntryMap m_oAlertMap;
    HrPrinterEntryMap m_oPrinterMap;
    HrDeviceEntry m_oDeviceEntryOfPrinter; // 打印机设备信息, 与m_oPrinterMap 关联 add by zmm, 2019.1.11

	//通过SNMP获取OID信息的MAP的线程锁
	CCriticalSection2 m_cs4GeneralMap;
	CCriticalSection2 m_cs4StorageRefMap;
	CCriticalSection2 m_cs4DeviceRefMap;
	CCriticalSection2 m_cs4CoverMap;
	CCriticalSection2 m_cs4LocalizationMap;
	CCriticalSection2 m_cs4InputMap;
	CCriticalSection2 m_cs4OutputMap;
	CCriticalSection2 m_cs4MarkerMap;
	CCriticalSection2 m_cs4MarkerSuppliesMap;
	CCriticalSection2 m_cs4MarkerColorantMap;
	CCriticalSection2 m_cs4MediaPathMap;
	CCriticalSection2 m_cs4ChannelMap;
	CCriticalSection2 m_cs4InterpreterMap;
	CCriticalSection2 m_cs4ConsoleDisplayBufferMap;
	CCriticalSection2 m_cs4ConsoleLightMap;
	CCriticalSection2 m_cs4AlertMap;
    CCriticalSection2 m_cs4PrinterMap;

	//纸盒配置
	CString m_szPaperConfig;
	int m_nPaperMinRatio;
	//墨盒配置
	CString m_szInkConfig;
	int m_nInkMinRatio;
	//品牌类型
	BRAND_TYPE m_eType;

	//OIDs
    CString m_szHrDeviceEntryOID; //add by zmm, 2019.1.11
    CString m_szHrPrinterEntryOID; // add by zmm, 2019.1.11

	//printmib.prtGeneral.prtGeneralTable.prtGeneralEntry
	CString m_szPrtGeneralEntryOID;
	CString m_szPrtGeneralConfigChangesOID;
	CString m_szPrtGeneralCurrentLocalizationOID;
	CString m_szPrtGeneralResetOID;
	CString m_szPrtGeneralCurrentOperatorOID;
	CString m_szPrtGeneralServicePersonOID;
	CString m_szPrtInputDefaultIndexOID;
	CString m_szPrtOutputDefaultIndexOID;
	CString m_szPrtMarkerDefaultIndexOID;
	CString m_szPrtMediaPathDefaultIndexOID;
	CString m_szPrtConsoleLocalizationOID;
	CString m_szPrtConsoleNumberOfDisplayLinesOID;
	CString m_szPrtConsoleNumberOfDisplayCharsOID;
	CString m_szPrtConsoleDisableOID;
	CString m_szPrtAuxiliarySheetStartupPageOID;
	CString m_szPrtAuxiliarySheetBannerPageOID;
	CString m_szPrtGeneralPrinterNameOID;
	CString m_szPrtGeneralSerialNumberOID;
	CString m_szPrtAlertCriticalEventsOID;
	CString m_szPrtAlertAllEventsOID;

	//printmib.prtCover.prtCoverTable.prtCoverEntry
	CString m_szPrtCoverEntryOID;
	CString m_szPrtCoverIndexOID;
	CString m_szPrtCoverDescriptionOID;
	CString m_szPrtCoverStatusOID;

	//printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry
	CString m_szPrtLocalizationEntryOID;
	CString m_szPrtLocalizationIndexOID;
	CString m_szPrtLocalizationLanguageOID;
	CString m_szPrtLocalizationCountryOID;
	CString m_szPrtLocalizationCharacterSetOID;

	//printmib.prtInput.prtInputTable.prtInputEntry
	CString m_szPrtInputEntryOID;
	CString m_szPrtInputIndexOID;
	CString m_szPrtInputTypeOID;
	CString m_szPrtInputDimUnitOID;
	CString m_szPrtInputMediaDimFeedDirDeclaredOID;
	CString m_szPrtInputMediaDimXFeedDirDeclaredOID;
	CString m_szPrtInputMediaDimFeedDirChosenOID;
	CString m_szPrtInputMediaDimXFeedDirChosenOID;
	CString m_szPrtInputCapacityUnitOID;
	CString m_szPrtInputMaxCapacityOID;
	CString m_szPrtInputCurrentLevelOID;
	CString m_szPrtInputStatusOID;
	CString m_szPrtInputMediaNameOID;
	CString m_szPrtInputNameOID;
	CString m_szPrtInputVendorNameOID;
	CString m_szPrtInputModelOID;
	CString m_szPrtInputVersionOID;
	CString m_szPrtInputSerialNumberOID;
	CString m_szPrtInputDescriptionOID;
	CString m_szPrtInputSecurityOID;
	CString m_szPrtInputMediaWeightOID;
	CString m_szPrtInputMediaTypeOID;
	CString m_szPrtInputMediaColorOID;
	CString m_szPrtInputMediaFormPartsOID;
#ifdef RFC_3805
	CString m_szPrtInputMediaLoadTimeoutOID;
	CString m_szPrtInputNextIndexOID;
#endif

	//printmib.prtOutput.prtOutputTable.prtOutputEntry
	CString m_szPrtOutputEntryOID;
	CString m_szPrtOutputIndexOID;
	CString m_szPrtOutputTypeOID;
	CString m_szPrtOutputCapacityUnitOID;
	CString m_szPrtOutputMaxCapacityOID;
	CString m_szPrtOutputRemainingCapacityOID;
	CString m_szPrtOutputStatusOID;
	CString m_szPrtOutputNameOID;
	CString m_szPrtOutputVendorNameOID;
	CString m_szPrtOutputModelOID;
	CString m_szPrtOutputVersionOID;
	CString m_szPrtOutputSerialNumberOID;
	CString m_szPrtOutputDescriptionOID;
	CString m_szPrtOutputSecurityOID;
	CString m_szPrtOutputDimUnitOID;
	CString m_szPrtOutputMaxDimFeedDirOID;
	CString m_szPrtOutputMaxDimXFeedDirOID;
	CString m_szPrtOutputMinDimFeedDirOID;
	CString m_szPrtOutputMinDimXFeedDirOID;
	CString m_szPrtOutputStackingOrderOID;
	CString m_szPrtOutputPageDeliveryOrientationOID;
	CString m_szPrtOutputBurstingOID;
	CString m_szPrtOutputDecollatingOID;
	CString m_szPrtOutputPageCollatedOID;
	CString m_szPrtOutputOffsetStackingOID;

	//printmib.prtMarker.prtMarkerTable.prtMarkerEntry
	CString m_szPrtMarkerEntryOID;
	CString m_szPrtMarkerIndexOID;
	CString m_szPrtMarkerMarkTechOID;
	CString m_szPrtMarkerCounterUnitOID;
	CString m_szPrtMarkerLifeCountOID;
	CString m_szPrtMarkerPowerOnCount;
	CString m_szPrtMarkerProcessColorantsOID;
	CString m_szPrtMarkerSpotColorantsOID;
	CString m_szPrtMarkerAddressabilityUnitOID;
	CString m_szPrtMarkerAddressabilityFeedDirOID;
	CString m_szPrtMarkerAddressabilityXFeedDirOID;
	CString m_szPrtMarkerNorthMarginOID;
	CString m_szPrtMarkerSouthMarginOID;
	CString m_szPrtMarkerWestMarginOID;
	CString m_szPrtMarkerEastMarginOID;
	CString m_szPrtMarkerStatusOID;

	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry
	CString m_szPrtMarkerSuppliesEntryOID;
	CString m_szPrtMarkerSuppliesIndexOID;
	CString m_szPrtMarkerSuppliesMarkerIndexOID;
	CString m_szPrtMarkerSuppliesColorantIndexOID;
	CString m_szPrtMarkerSuppliesClassOID;
	CString m_szPrtMarkerSuppliesTypeOID;
	CString m_szPrtMarkerSuppliesDescriptionOID;
	CString m_szPrtMarkerSuppliesSupplyUnitOID;
	CString m_szPrtMarkerSuppliesMaxCapacityOID;
	CString m_szPrtMarkerSuppliesLevelOID;

	//printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry
	CString m_szPrtMarkerColorantEntryOID;
	CString m_szPrtMarkerColorantIndexOID;
	CString m_szPrtMarkerColorantMarkerIndexOID;
	CString m_szPrtMarkerColorantRoleOID;
	CString m_szPrtMarkerColorantValueOID;
	CString m_szPrtMarkerColorantTonalityOID;

	//printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry
	CString m_szPrtMediaPathEntryOID;
	CString m_szPrtMediaPathIndexOID;
	CString m_szPrtMediaPathMaxSpeedPrintUnitOID;
	CString m_szPrtMediaPathMediaSizeUnitOID;
	CString m_szPrtMediaPathMaxSpeedOID;
	CString m_szPrtMediaPathMaxMediaFeedDirOID;
	CString m_szPrtMediaPathMaxMediaXFeedDirOID;
	CString m_szPrtMediaPathMinMediaFeedDirOID;
	CString m_szPrtMediaPathMinMediaXFeedDirOID;
	CString m_szPrtMediaPathTypeOID;
	CString m_szPrtMediaPathDescriptionOID;
	CString m_szPrtMediaPathStatusOID;

	//printmib.prtChannel.prtChannelTable.prtChannelEntry
	CString m_szPrtChannelEntryOID;
	CString m_szPrtChannelIndexOID;
	CString m_szPrtChannelTypeOID;
	CString m_szPrtChannelProtocolVersionOID;
	CString m_szPrtChannelCurrentJobCntlLangIndexOID;
	CString m_szPrtChannelDefaultPageDescLangIndexOID;
	CString m_szPrtChannelStateOID;
	CString m_szPrtChannelIfIndexOID;
	CString m_szPrtChannelStatusOID;
	CString m_szPrtChannelInformationOID;

	//printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry
	CString m_szPrtInterpreterEntryOID;
	CString m_szPrtInterpreterIndexOID;
	CString m_szPrtInterpreterLangFamilyOID;
	CString m_szPrtInterpreterLangLevelOID;
	CString m_szPrtInterpreterLangVersionOID;
	CString m_szPrtInterpreterDescriptionOID;
	CString m_szPrtInterpreterVersionOID;
	CString m_szPrtInterpreterDefaultOrientationOID;
	CString m_szPrtInterpreterFeedAddressabilityOID;
	CString m_szPrtInterpreterXFeedAddressabilityOID;
	CString m_szPrtInterpreterDefaultCharSetInOID;
	CString m_szPrtInterpreterDefaultCharSetOutOID;
	CString m_szPrtInterpreterTwoWayOID;

	//printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry
	CString m_szPrtConsoleDisplayBufferEntryOID;
	CString m_szPrtConsoleDisplayBufferIndexOID;
	CString m_szPrtConsoleDisplayBufferTextOID;

	//printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry
	CString m_szPrtConsoleLightEntryOID;
	CString m_szPrtConsoleLightIndexOID;
	CString m_szPrtConsoleOnTimeOID;
	CString m_szPrtConsoleOffTimeOID;
	CString m_szPrtConsoleColorOID;
	CString m_szPrtConsoleDescriptionOID;

	//printmib.prtAlert.prtAlertTable.prtAlertEntry
	CString m_szPrtAlertEntryOID;
	CString m_szPrtAlertIndexOID;
	CString m_szPrtAlertSeverityLevelOID;
	CString m_szPrtAlertTrainingLevelOID;
	CString m_szPrtAlertGroupOID;
	CString m_szPrtAlertGroupIndexOID;
	CString m_szPrtAlertLocationOID;
	CString m_szPrtAlertCodeOID;
	CString m_szPrtAlertDescriptionOID;
	CString m_szPrtAlertTimeOID;
};

#endif //_Snmp_Interface_h_27E39E90_744D_4560_95FD_F5C6B2BBC9A6_