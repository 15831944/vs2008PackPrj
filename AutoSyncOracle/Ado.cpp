// Ado.cpp: implementation of the CAdo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ado.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void WriteLogEx(LPCTSTR lpszFormat, ...);
CAdo::CAdo()
{

}

CAdo::~CAdo()
{
	Release();
}

BOOL CAdo::ConnectDB(_bstr_t bConnStr)
{ 

	m_pComm=NULL;
	m_pConn=NULL;
	pTempRs=NULL;
	m_pTempRs = NULL;
	try
	{
		m_pConn.CreateInstance(__uuidof(Connection));
		pTempRs.CreateInstance(__uuidof(Recordset));
		
	//	m_pTempRs.CreateInstance(__uuidof(Record));
	//    m_pStream.CreateInstance(__uuidof(Stream));
		m_pConn->ConnectionTimeout = 20;
		m_pConn->Open(bConnStr,"","",adConnectUnspecified);

	}
	catch(_com_error e)
	{
		if(m_pConn)
			PrintErrorMessage(e);
		return false;
	}
	return true;
}

void CAdo::PrintErrorMessage(_com_error e)
{
	ErrorPtr    pErr = NULL;
	long      nCount = 0;    
	long      i  = 0;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());	

	WriteLogEx("\n Source : %s \n Description : %s \n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);		
	
	if( (m_pConn->Errors->Count) > 0)
	{
		nCount = m_pConn->Errors->Count;
		// Collection ranges from 0 to nCount -1.
		for(i = 0; i < nCount; i++)
		{
			pErr = m_pConn->Errors->GetItem(i);
			WriteLogEx("Error number: %x\n Error Description: %s\n", pErr->Number,(LPCSTR) pErr->Description);
		}
	}
}

BOOL CAdo::ExecuteSql(BSTR bstrSql)
{
	bool bRet = false;
	if(IsOpen())
	{	
		HRESULT hResult=NULL;
		try
		{
			hResult=m_pComm.CreateInstance(__uuidof(Command));
			if(SUCCEEDED(hResult))
			{	
				m_pComm->ActiveConnection=m_pConn;
				m_pComm->CommandText=_bstr_t(bstrSql);
				m_pComm->CommandTimeout=60;
				m_pComm->CommandType=adCmdText;
				m_pComm->Execute(NULL,NULL,adCmdText);	
				bRet = true;
			}
		}
		catch(_com_error e)
		{
			PrintErrorMessage(e);
		}	
	}
	return bRet;
}

bool CAdo::IsOpen()
{
	if( m_pConn!=NULL )
	{
		if( m_pConn->State == adStateOpen )
			return true;
	}
	return false;
}

bool CAdo::GetRecordSet(BSTR bstrSql, _RecordsetPtr &pRs)
{
	try
	{
		pTempRs->Open(_variant_t(bstrSql),(IDispatch *)m_pConn,adOpenStatic,adLockReadOnly,adCmdText);
		pRs = pTempRs.GetInterfacePtr();
   //     HRESULT hr=m_pStream->Open(_variant_t(pTempRs),adModeRead, adOpenStreamFromRecord,(_bstr_t)"sa",(_bstr_t)"");
   //     HRESULT hr=m_pTempRs->Open(_variant_t(bstrSql),(IDispatch *)m_pConn,adModeRead,adOpenIfExists,
	//		adOpenRecordUnspecified,(_bstr_t)"sa",(_bstr_t)"");

	//	_variant_t var;
	//	var.vt=VT_BSTR;
	//	var=m_pStream->Read(10);
 
	}
	catch(_com_error e)
	{

		PrintErrorMessage(e);
		return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

BOOL CAdo::SetOracleConStr(CString sIP,CString sUser,CString sPwd,CString sDb,CString sPort)
{
	CString sCon;
	sCon.Format(_T("Provider=OraOLEDB.Oracle;Password=%s;User ID=%s;"
		"Data Source=(DESCRIPTION =(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = 1521))"
		"(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = %s)));Persist Security Info=True"),
		sPwd,sUser,sIP,sDb);
	_bstr_t bConnStr = sCon.AllocSysString();
	BOOL b = ConnectDB(bConnStr);
	::SysFreeString(bConnStr);
	return b;
}
int CAdo::InitialDB(BOOL bSQLServer,BSTR bServerName, BSTR bDBName, BSTR bUid, BSTR bPwd,BSTR bPort)
{
	_bstr_t bConnStr;
	if(bSQLServer)
	{
		bConnStr="driver={sql server};server=";
		bConnStr+=_bstr_t(bServerName);
		bConnStr+=_bstr_t(",");
		bConnStr+=_bstr_t(bPort);
		bConnStr+=";database=";
		bConnStr+=_bstr_t(bDBName);
		bConnStr+=";user id=";
		bConnStr+=_bstr_t(bUid);
		bConnStr+=";password=";
		bConnStr+=_bstr_t(bPwd);
	}
	else
	{
		///*
		
		bConnStr= "Provider=OraOLEDB.Oracle;Password=libsys;User ID=libsys;Data Source=(DESCRIPTION =(ADDRESS = (PROTOCOL = TCP)(HOST = 219.230.55.68)(PORT = 1521))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = orcl)));Persist Security Info=True";
		
		//*/	
		/*//	
		bConnStr="Provider=OraOLEDB.Oracle;Data Source=(DESCRIPTION =(ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.2.152)(PORT = 1521))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = song)));Persist Security Info=True";
		bConnStr+=_bstr_t(bDBName);
		bConnStr+=";User ID=";
		bConnStr+=_bstr_t(bUid);
		bConnStr+=";Password=";
		bConnStr+=_bstr_t(bPwd);
		*/
		/*
		bConnStr="Provider=OraOLEDB.Oracle;Data Source=";
		bConnStr+=_bstr_t(bDBName);
		bConnStr+=";User ID=";
		bConnStr+=_bstr_t(bUid);
		bConnStr+=";Password=";
		bConnStr+=_bstr_t(bPwd);
		*/
		//AfxMessageBox(bConnStr);
	}

/*	
	bConnStr="DSN=Saturn;";
	bConnStr+=_bstr_t(bServerName);
	bConnStr+=";UID=";
	bConnStr+=_bstr_t(bUid);
	bConnStr+=";PWD=";
	bConnStr+=_bstr_t(bPwd);
*/	

/*
	bConnStr = "driver={Microsoft ODBC for Oracle};SERVER=DXS;UID=scott;PWD=scott";
	bConnStr = "Provider=OraOLEDB.Oracle;User ID=scott;Password=scott;Data Source=DXS;";
	bConnStr = "Provider=MSDAORA;User ID=scott;Password=scott;Data Source=DXS";//这一种能连接成功，但是取记录失败
	*/
	return ConnectDB(bConnStr);
}



bool CAdo::SearchRecord(CString &szSql, CStringArray &RsAry, long &nRow, long &nCol)
{
	nRow = -1;
	nCol = -1;
	RsAry.RemoveAll();

	_RecordsetPtr pRs = NULL;
	bool bRet = GetRecordSet(_bstr_t(szSql),pRs);
	if(!bRet || pRs == NULL)
		return false;
	
	nCol = pRs->Fields->GetCount();
	nRow = pRs->RecordCount;
	nRow = pRs->GetRecordCount();
	if(nRow>0)
	{
		_variant_t tempvariant = pRs->GetRows(nRow);////第一维是列

		VARIANT result;
		long rgIndices[2];
		for(int i=0;i<nRow;i++)
		{
			for(int j=0;j<nCol;j++)
			{
				rgIndices[0] = j; 
				rgIndices[1] = i;
				HRESULT hr1= SafeArrayGetElement(tempvariant.parray, 
					rgIndices, &result);
				if(result.vt == VT_NULL || result.vt == VT_EMPTY)
					RsAry.Add("");
				else
				{
					VariantChangeType (&result, &result, 0, VT_BSTR);
					RsAry.Add(CString(result.bstrVal));
				}
				VariantClear(&result);
			}
		}	
		tempvariant.Clear();
	}
	else
	{
		nRow = nCol = 0;//空二维数组
	}
	pRs->Close();
	pRs = NULL;
	return true;
}

CString CAdo::FieldValue(CString &szSql)
{
	CStringArray ary;
	long nRow,nCol;
	bool bRet = SearchRecord(szSql,ary,nRow,nCol);
	if(!bRet)
		return "";
	if(nRow == 1 && nCol == 1)
		return ary.GetAt(0);
	

	return "";
}

BOOL CAdo::ExecuteSql(CString &szSql)
{
	return ExecuteSql(_bstr_t(szSql));
}

void CAdo::Release()
{
	if(m_pConn!=NULL)
	{
		if(m_pConn->State==adStateOpen)
		{
			m_pConn->Close();
			m_pConn=NULL;
		}
	}
}
