// Ado.h: interface for the CAdo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADO_H__FF332852_0713_4288_B4B4_0B5C249F3A41__INCLUDED_)
#define AFX_ADO_H__FF332852_0713_4288_B4B4_0B5C249F3A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAdo  
{
public:
	void Release();
	BOOL ExecuteSql(CString &szSql);
	CString FieldValue(CString &szSql);
	bool SearchRecord(CString &szSql,CStringArray& RsAry,long &nRow,long &nCol);
	int InitialDB(BOOL bSQLServer,BSTR bServerName,BSTR bDBName,BSTR bUid,BSTR bPwd,BSTR bPort);
	bool GetRecordSet(BSTR bstrSql,_RecordsetPtr &pRs);
	bool IsOpen();
	BOOL ExecuteSql(BSTR bstrSql);
	CAdo();
	void PrintErrorMessage(_com_error e);
	virtual ~CAdo();
	BOOL SetOracleConStr(CString sIP,CString sUser,CString sPwd,CString sDb,CString sPort);

	_ConnectionPtr m_pConn;
	_RecordsetPtr pTempRs;
	_RecordPtr m_pTempRs;
	_StreamPtr m_pStream;
	_CommandPtr m_pComm;

	BOOL ConnectDB(_bstr_t  bConnStr);
};

#endif // !defined(AFX_ADO_H__FF332852_0713_4288_B4B4_0B5C249F3A41__INCLUDED_)
