// Ado.h: interface for the CAdo class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


//#include <asptlb.h>         // Active Server Pages Definitions

#import "e:\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")

//记录集的打开方式
#define ADO_OPEN_READONLY				0									//以只读方式打开记录集
#define ADO_OPEN_ADDNEW					1									//以ADDNew方式打开记录集
#define ADO_OPEN_UPDATE					2									//以Update方式打开记录集

typedef enum tag_DataBase_Type {SQLSERVER,ORACLE,MYSQL,ACCESS2003,ACCESS2007,REMOTE_AGENT} DB_TYPE;

class  CAdoInterface
{
public:
	virtual BOOL ExecuteSQL(CString sSQL) = 0;
	virtual BOOL GetRecordset(CString sSQL,CStringArray& ary,long &lRow,long &lCol) = 0;
	virtual CString GetOneData(CString sSQL) = 0;
};

//typedef CList<STREAM_DATA_INFO*,STREAM_DATA_INFO*> CStreamDataInfoPtrList;

class  CAdo : public CAdoInterface
{
public:
	_bstr_t m_bConnStr;
	CString m_sErrorSql;
	CAdo();
	virtual ~CAdo();
public:
	CString GetGoToPageSql(const CString &szTabName				//表名
		, const CString &szGetFields = _T("*")	//需要返回的列
		, const CString &szWhere = _T("")		//查询条件,(注意:不要加单词"where",内部已经有了)
		, const CString &szSortName = _T("")	//排序的字段
		, bool bSortDesc = true					//排序类型,是否为降序
		, int nPageSize = 10					//页尺寸,每页多少条记录
		, int nPageIndex = 1					//页码，即需要查询第多少页的数据
		, const CString &szSortName2 = _T("")	//第二个排序字段,可以为空
		, bool bSortDesc2 = true				//第二个排序字段是否为降序
		);	//add by zfq,得到按某字段进行排序，查询某表中的某页的SQL语句
	CString GetGoToPageSql2(int  nClsID,CString sClsName,int nStuID,int nPageSize,int nPageIndex);
	
	
	
	void SetExcelConnstr(CString sPathName);
	BOOL Reconnect();
	void ReleaseConn();

	BOOL GetAry(CString sSQL,CStringArray &ary);
	BOOL GetRecordset(CString sSQL,CStringArray& ary,long &lRow,long &lCol);
	CString GetOneData(CString szSql);
	BOOL GetRecordset(CString sSql, BOOL bGetArray /*= FALSE*/, int nOpenType = ADO_OPEN_READONLY);
	BOOL GetRecordset(BSTR sSql, BOOL bGetArray /*= FALSE*/, int nOpenType = ADO_OPEN_READONLY);

	BOOL GetRecordset(CString sSqlStr,_Recordset **pRs,int nOpenType=ADO_OPEN_READONLY);
	BOOL GetRecordset(BSTR sSqlStr,_Recordset **pRs,int nOpenType=ADO_OPEN_READONLY);

	BOOL GetOneData(CString sSql,_variant_t &varData);
	BOOL GetOneData(BSTR sSql,_variant_t &varData);


	void PrintProviderError(_ConnectionPtr pConnection);
	void PrintComError(_com_error &e);
	static void WriteFileLog(LPCTSTR lpszFormat, ...);
	static void WriteFileLog(CString sLog);


	BOOL IsConnected();
	BOOL Connect(int nTimeOut=0);
	void SetConnStr(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort);
	void SetConnStr2(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort,int nDbType);
	void SetConnStr(CString sConnStr);
	void SetTextConnStr(CString sPath);
	void SetAccessConnStr(CString sDBPath);
	BOOL ExecuteSQL(CString sSQL);
	BOOL ExecuteSQL(BSTR sSQL);
	BOOL ExecuteSQL(CString sSQL,int& nID);	
public:
	CString GetDate();

	BOOL GetXml();
	int  GetRowCount()const;
	int  GetColumnCount()const;

	BOOL GetRow(CStringArray& RowValues, int row = 0); 
	BOOL GetLinearSA(CString sSql, CStringArray& LinearValues);//m_pRs会改变！

	CString GetVariantStr(_variant_t varValue);
	int     GetVariantInt(_variant_t varValue);

	int GetOneInt(CString sSql);
	CString GetOneString(CString sSql);

	
private:
	CStringArray m_aRows;
	int    m_nColumn;
	int    m_nRow;

	_ConnectionPtr m_pConn;
	_RecordsetPtr m_pRs;
	_CommandPtr		m_pCmd;
};


