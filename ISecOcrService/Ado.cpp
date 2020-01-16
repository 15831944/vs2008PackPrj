#include "StdAfx.h"
#include "Ado.h"


#define CONECTTIMOUT         30
#define COMMITTIMOUT         600

CAdo::CAdo(void)
{
	try
	{
		::CoInitialize(NULL);
		m_pConn = NULL;
	}
	catch(_com_error &e)
	{
		e;
	}
}

CAdo::~CAdo(void)
{
	if(m_pConn)
	{
		try
		{
			m_pConn->Close();
			m_pConn = NULL;
		}
		catch(_com_error &e)
		{
			e;
		}
	}
	::CoUninitialize();
}

BOOL CAdo::Connect()
{
	HRESULT hResult;
	try
	{
		WriteLogEx("连接字符串：%s",m_sConnStr);
		BSTR bsTmp = m_sConnStr.AllocSysString();
		hResult = m_pConn.CreateInstance(__uuidof(Connection));
		hResult = m_pConn->Open(bsTmp,"","",adConnectUnspecified);
		::SysFreeString(bsTmp);
	}
	catch(_com_error &e)
	{
		e;
		this->PrintComError(e);
		return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CAdo::IsConnected()
{
	BOOL bRet = FALSE;
	try
	{
		if( m_pConn != NULL )
		{
			try
			{
				if( m_pConn->State == adStateOpen )
				{
					bRet = TRUE;
				}
			}
			catch(_com_error &e)
			{
				//PrintProviderError(m_pConn);
				//PrintComError(e);
			}
		}
	}
	catch(...)
	{
		CString szMsg;
		szMsg.Format("%d",GetLastError());

	}
	return bRet;
}
void CAdo::ReleaseConn()
{
	try
	{
		if( m_pConn != NULL )
		{
			try
			{
				if( m_pConn->State == adStateOpen )
				{
					m_pConn->Close();
					m_pConn = NULL;
					//m_pConn.Release();
				}
			}
			catch(_com_error &e)
			{
				//PrintProviderError(m_pConn);
				//PrintComError(e);
			}
		}
	}
	catch(...)
	{
		CString szMsg;
		szMsg.Format("%d",GetLastError());

	}

}

void CAdo::PrintComError(_com_error &e)//打印执行sql语句时出现的错误
{
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	CString strError;
	strError.Format("Error(tCode): %08lx\r\n,"
					"ErrorMessage(tCode Meaning): %s\r\n,"
					"tSource: %s\r\n,"
					"tDescription: %s\r\n,"
					"tErrorSql: %s\r\n",
					e.Error(),
					e.ErrorMessage(),
					(LPCSTR)bstrSource,
					(LPCSTR)bstrDescription,
					m_sErrorSql);
	WriteLogEx(strError);
}

void CAdo::SetTextConnStr(CString sPath)
{
	m_sConnStr = "Driver={Microsoft Text Driver (*.txt; *.csv)};";
	m_sConnStr += "DBQ=";
	m_sConnStr += sPath;
	m_sConnStr += ";";
	m_sConnStr += "DefaultDir=";
	m_sConnStr += sPath;
	m_sConnStr += ";";
}

void CAdo::SetSqlConnStr(CString sDBSrc,CString sDBName,CString sUserName,CString sPsw)
{
	m_sConnStr.Format("Provider = sqloledb; "
						"Data Source = %s; "
						"Initial Catalog = %s; "
						"User Id = %s; "
						"Password = %s;",
						sDBSrc,
						sDBName,
						sUserName,
						sPsw);
}

void CAdo::SetExcelConnStr(CString sExcelPath)
{
	m_sConnStr.Format("Provider = Microsoft.Jet.OLEDB.4.0; Password="";User ID=Admin;"
						"Data Source = %s; Mode=ReadWrite;"
						"Extended Properties = Excel 8.0",
						sExcelPath);
}

void CAdo::SetAccessConnStr(CString sDBPath)
{
	m_sConnStr = "DRIVER={Microsoft Access Driver (*.mdb)};";
	m_sConnStr += "DBQ=";
	m_sConnStr += sDBPath;
	m_sConnStr += ";";
}

CString CAdo::GetMExcelSheetName()
{
	_RecordsetPtr pTempRs = NULL;
	
	CString szRet = "";
	try
	{		
		if(m_pConn != NULL)
		{
			pTempRs = m_pConn->OpenSchema(adSchemaTables);
			_bstr_t table_name = pTempRs->Fields->GetItem(_variant_t("TABLE_NAME"))->Value;
			szRet.Format("%s",(LPCSTR)table_name);
		}
		if(pTempRs != NULL)
		{
			pTempRs->Close();
		}

	}
	catch(_com_error  &e)
	{
		e;
		throw;
		return "com_error";
	}
	catch(...)
	{
		throw;
		return "发生异常！";
	}
	return szRet;
}

BOOL CAdo::ExecuteSQL(BSTR bsSQL)
{
	HRESULT hResult;
	_CommandPtr comm;
	try
	{
		hResult  = comm.CreateInstance(__uuidof(Command));
		if(SUCCEEDED(hResult))
		{
			comm->ActiveConnection = m_pConn;
			comm->CommandTimeout = COMMITTIMOUT;
			comm->CommandText = bsSQL;
			comm->Execute(NULL,NULL,adCmdText);
			comm.Release();
		}
	}
	catch(_com_error &e)
	{
		throw;
		return FALSE;
	}
	catch(...)
	{
		throw;
		return FALSE;
	}
	return TRUE;
}

BOOL CAdo::GetRecordSet(BSTR bSqlStr,_Recordset **pRs,BOOL bReadOnly)
{
	_RecordsetPtr pTempRs = NULL;
	try
	{
		pTempRs.CreateInstance(__uuidof(Recordset));
		if(bReadOnly)
		{
			pTempRs->Open((_bstr_t)bSqlStr,
				(IDispatch *)m_pConn,
				adOpenStatic,
				adLockReadOnly,
				adCmdText);
		}
		else
		{
			pTempRs->Open((_bstr_t)bSqlStr,
				(IDispatch *)m_pConn,
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);
		}
		if(pRs != NULL)
		{
			*pRs = pTempRs.Detach();
		}
		else
		{
			pTempRs->Close();
		}
	}
	catch(_com_error &e)
	{
		e;
		/*throw;*/
		return FALSE;
	}
	catch(...)
	{
		/*throw;*/
		return FALSE;
	}
	return TRUE;
}

BOOL CAdo::GetRecordSet(CString sSqlStr,_Recordset **pRs,BOOL nOpenType)
{
	BSTR bsSql = sSqlStr.AllocSysString();
	BOOL b = GetRecordSet(bsSql,pRs,nOpenType);
	SysFreeString(bsSql);
	return b;
}
BOOL CAdo::GetRecordSet(CString sSQL,CStringArray& RsAry,long &lRow,long &lCol)
{
	lRow = -1;
	lCol = -1;
	RsAry.RemoveAll();

	_RecordsetPtr pRs = NULL;
	try
	{
		BOOL bRet = GetRecordSet(sSQL,&pRs);
		if(!bRet || pRs == NULL)
			return false;

		lCol = pRs->Fields->GetCount();
		lRow = pRs->RecordCount;

		if(lRow>0)
		{
			_variant_t tempvariant = pRs->GetRows(lRow);////第一维是列

			RsAry.SetSize(lRow * lCol);
			int nIdx = 0;
			VARIANT result;
			long rgIndices[2];
			for(int i=0;i<lRow;i++)
			{
				for(int j=0;j<lCol;j++)
				{
					rgIndices[0] = j; 
					rgIndices[1] = i;
					HRESULT hr1= SafeArrayGetElement(tempvariant.parray, 
						rgIndices, &result);
					if(result.vt == VT_NULL || result.vt == VT_EMPTY)
						RsAry.SetAt(nIdx++,"");
					else
					{
						VariantChangeType (&result, &result, 0, VT_BSTR);
						RsAry.SetAt(nIdx++,CString(result.bstrVal));
					}
					VariantClear(&result);
				}
			}	
		}
		else
		{
			lRow = lCol = 0;//空二维数组
		}
	}
	catch (_com_error &e)
	{
		PrintComError(e);
	}
	if(pRs)
	{
		pRs->Close();
		pRs = NULL;
	}
	return true;
}

BOOL CAdo::GetAry(CString sSQL, CStringArray &ary)
{
	long nRow = 0;
	long nCol = 0;
	if(GetRecordSet(sSQL,ary,nRow,nCol))
	{
		return ary.GetSize() > 0 ? TRUE : FALSE;
	}
	return FALSE;
}
BOOL CAdo::ExecuteSQL(CString sSQL)
{
	BSTR bsSql = sSQL.AllocSysString();
	BOOL b =  ExecuteSQL(bsSql);
	SysFreeString(bsSql);
	return b;
}
void CAdo::SetConnStr(int nType,CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort)
{
	if (0 == nType)
	{
		CString sConnString="";
		sConnString+="Provider=sqloledb;";
		//bsConn+="driver={SQL SERVER};";
		sConnString+="Data Source=";
		//bsConn+="server=";
		sConnString+=sDbSvr;
		//sConnString += ";Port=";
		sConnString += ",";
		sConnString += szDbPort;
		sConnString+=";Initial Catalog=";
		//bsConn+=";database=";
		sConnString+=sDbName;
		sConnString+=";User Id=";
		sConnString+=sDbUser;
		sConnString+=";Password=";
		sConnString+=sDbPwd;
		sConnString+=";";


		m_sConnStr=sConnString;
	}
	else if (1 == nType)
	{
		CString sConnString="";
		sConnString.Format("Provider=OraOLEDB.Oracle;Password=%s;User ID=%s;"
			"Data Source=(DESCRIPTION =(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s))"
			"(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = %s)));Persist Security Info=True"
			,sDbPwd,sDbUser,sDbSvr,szDbPort,sDbName);
		m_sConnStr = sConnString;
	}
	
	WriteLogEx("CAdo::SetConnStr connectstr = %s",m_sConnStr);
}

void CAdo::SetConnStr2(CString sConnStr)
{
	m_sConnStr = sConnStr;
	WriteLogEx("CAdo:: SetConnStr2,connectstr = %s",sConnStr);
}