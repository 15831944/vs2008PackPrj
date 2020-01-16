#if !defined(AFX_ADO_H__A5F1600F_2A34_46C4_98A2_7F771F870F0A__INCLUDED_)
#define AFX_ADO_H__A5F1600F_2A34_46C4_98A2_7F771F870F0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAdo
{
public:
	void PrintComError(_com_error &e);
	void SetTextConnStr(CString sPath);
	void SetSqlConnStr(CString sDBSrc,CString sDBName,CString sUserName,CString sPsw);
	void SetExcelConnStr(CString sExcelPath);
	void SetAccessConnStr(CString sDBPath);
	CString GetMExcelSheetName();

	BOOL Connect();
	BOOL ExecuteSQL(CString sSQL);
	BOOL ExecuteSQL(BSTR bsSQL);
	BOOL GetRecordSet(BSTR bSqlStr,_Recordset **pRs,BOOL bReadOnly = TRUE);

	BOOL GetRecordSet(CString sSqlStr,_Recordset **pRs,BOOL bReadOnly = TRUE);
	BOOL GetAry(CString sSQL,CStringArray &ary);
	BOOL GetRecordSet(CString sSQL,CStringArray& ary,long &lRow,long &lCol);

	void SetConnStr(int nType,CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort);
	void SetConnStr2(CString sConnStr);

	BOOL IsConnected();
	void ReleaseConn();
	_ConnectionPtr m_pConn;
	CString m_sConnStr;
	CString m_sErrorSql;

	CAdo(void);
	~CAdo(void);

	
};
#endif // !defined(AFX_ADO_H__A5F1600F_2A34_46C4_98A2_7F771F870F0A__INCLUDED_)