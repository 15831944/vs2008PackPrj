
#pragma once
#include "../Public/Include/sqlite3.h"
#include "../Public/include/SystemDef.h"
#include "../Public/CloudPrintCommDef.h"

// 此类是从 SQliteDataSvc.dll 导出的
// typedef int (*hardware_callback)(int,double,const char*,const void*,int);
// typedef int (*software_callback)(double ,const char*,const void*,int);
// typedef int (*log_callback)(int,const void*,int);

class _AA_DLL_EXPORT_ CSQliteDataSvc 
{
protected:
	CSQliteDataSvc(void);
public:
	~CSQliteDataSvc();
	static CSQliteDataSvc& GetInstance();
	BOOL ExecuteSQL(char* szSQL);
	BOOL Insert(char* szSQL);
	BOOL GetRecordStream(char* szSQL,isafetec::datastream& ary);
	BOOL GetRecordStream(char* szSQL,BYTE*& pBuf,int &nBufSize);
	
	void SetsMutexName(CString szMutexName);
	void InitConnt(char* szDbFile /*= NULL*/,char* szPwd = NULL,DWORD dwTimeout = 10 * 1000);
	BOOL Connect();
	void Release();
	BOOL CreateDB(char* szDb);
	
public:
	BOOL GetSimplePolicy(POLICY_TYPE type,SimplePolicyList& list);
	BOOL CheckSimplePolicy(SimplePolicyList* list,SimplePolicyList& rs,SimplePolicyList& rs2);
	
	//更新策略
	BOOL UpdatePolicy(int nId,int nType, double time,BYTE* pData,int nSize);
	BOOL GetPolicy(int nId,BYTE*& pData,int& nSize);
	
	// 资产信息的处理
	BOOL UpdateAssets(SYSTEM_INFO_TYPE eType,double time,BYTE* pData,int nSize,int nUploadSuc = 1);
	BOOL GetAssets(SYSTEM_INFO_TYPE eType,BYTE*& pData,int &nSize,BOOL& bUpSucced);
	BOOL CleanAssets();

	// 日志的处理
	BOOL InsertLog(LOG_TYPE eType,BYTE* pData,int nLen);
	BOOL InsertLog(LOG_TYPE eType,BYTE* pData,int nLen,int &jobId);
	
	//获取数据库中没有上传的日志
	BOOL PopLog(ICltLogCallback* pCall,LOG_TYPE eType = NULL_LOG,void* context = NULL);
	BOOL PopOneLog(ICltLogCallback* pCall,LOG_TYPE eType = NULL_LOG,void* context = NULL);
	BOOL PopOnePrintLog(ICltLogCallback* pCall,int nid,void* context = NULL);
	BOOL QueryLog(ICltLogCallback* pCall,LOG_TYPE eType= NULL_LOG);
	BOOL GetOnePrintLog(SubmitPrintJob* job,int &nId);
	BOOL DelLog(LOG_TYPE eType,int nId = 0);
	int GetLogCount(LOG_TYPE eType = NULL_LOG);

	BOOL SaveConfig(CString sName,CString sVal,BYTE* pData,int nSize);
	BOOL GetConfigVal(CString sName,CString& sVal);
	BOOL GetConfigData(CString sName,BYTE*& pData,int& nSize);
	BOOL GetConfigData(CString sName,CBuffer* pBuf);
	BOOL GetProductInfo(CString sName, ProductInfo* pInfo);
	BOOL GetEquipInfo(EQUIP_INFO* pInfo);

	BOOL AddOnePrinter(PPrinterInfo pInfo);
	BOOL GetOnePrinter(PPrinterInfo pInfo, CString szFlag);		//获取指定szFlag的打印机信息
	BOOL RemoveOnePrinter(CString szFlag);		//删除指定szFlag的打印机信息
	BOOL UpdateOnePrinter(PPrinterInfo pInfo);	//入参pInfo中，cFlag要求被设置
	BOOL GetAllPrinter(CMapStringToPtr& PrinterMap);
	BOOL GetOnePrinterById(PPrinterInfo pInfo, int nStationId);		//获取指定nStationId的打印机信息
	BOOL UpdateOnePrinterById(PPrinterInfo pInfo, int nStationId);	//更新指定nStationId的打印机信息

	BOOL IsTableExist(CString szTableName);
	BOOL IsColumnExist(CString szTableName, CString szColumn);
	void CheckCloudDbTables();

private:
	sqlite3*	m_hDB;
	HANDLE		m_hMutex;
	DWORD		m_dwTimeout;
	char		m_szDbFile[MAX_PATH];
	char		m_szPwd[MAX_PATH];
	BOOL InitMutex();
	CString m_szMutexName;

};

