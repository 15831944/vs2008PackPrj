// Ado.cpp: implementation of the CAdo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ado.h"
//

#define CONECTTIMOUT         600

#define COMMITTIMOUT         600

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//                                                       //
//      PrintProviderError Function                      //
//                                                       //
///////////////////////////////////////////////////////////

void CAdo::PrintProviderError(_ConnectionPtr pConnection)
{
    // Print Provider Errors from Connection object.
    // pErr is a record object in the Connection's Error collection.
	if( pConnection == NULL )
		return;
    ErrorPtr    pErr  = NULL;
	CString sError="";
    if( (pConnection->Errors->Count) > 0)
    {
        long nCount = pConnection->Errors->Count;
        // Collection ranges from 0 to nCount -1.
        for(long i = 0;i < nCount;i++)
        {
            pErr = pConnection->Errors->GetItem(i);
			CString sTmp;
			//sTmp.Format("\r\n\t Error number: %x\t%s", pErr->Number,
            //    pErr->Description);
			//sError += sTmp;
        }
    }

   //WriteFileLog(sError);
}

///////////////////////////////////////////////////////////
//                                                       //
//      PrintComError Function                           //
//                                                       //
///////////////////////////////////////////////////////////

void CAdo::WriteFileLog(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);

	WriteFileLog(sTmp);
}
//extern BOOL g_bIsWriteLog;
void CAdo::WriteFileLog(CString sLog)
{
	if( sLog.GetLength() > 250 )
		TRACE(sLog.Left(250));
	else
		TRACE(sLog);
	TRACE("\n");

	if (!CFileLog::IsDebug())
	{
		return ;
	}
//	if( !g_bIsWriteLog )
//		return;

	CString sSysDir = CCommonFun::GetLogDir();

	CFileFind fd;

	if( !fd.FindFile(sSysDir) )
	{
		if( !::CreateDirectory(sSysDir,NULL) )
		return;
	}

	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format("%y%m%d");
	
	CString sFileName;
	sFileName.Format("%s\\ADO_%s.Log",sSysDir,sDate);
	
	
	try
	{
		CFile fileLog;
		if(!fileLog.Open(sFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
			return;
		
		CString sTime = time.Format( "%Y-%m-%d %H:%M:%S" );
		CString sTmp;
		
		sTmp.Format("%s,	 %s",sTime,sLog);
		sTmp += "\r\n";
		fileLog.SeekToEnd();
		fileLog.Write(sTmp,sTmp.GetLength());
		fileLog.Close();
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}

}

void CAdo::PrintComError(_com_error &e)
{
   _bstr_t bstrSource(e.Source());
   _bstr_t bstrDescription(e.Description());

   CString sError;
    // Print COM errors. 
   sError.Format("\r\nError\r\n,\tCode = %08lx\r\n,\tCode meaning = %s\r\n,"
			"\tSource = %s\r\n,\tDescription = %s\r\n,\tErrorSql = %s\r\n",
			e.Error(), e.ErrorMessage(), (LPCSTR) bstrSource, (LPCSTR) bstrDescription,	m_sErrorSql);


   WriteFileLog(sError);
}
//--------------------------------
//将字符串转变成BSTR,此函数比较危险，暂时停用
//--------------------------------
/*
BSTR CAdo::SToB(const char* pSrc)
{
	return _com_util::ConvertStringToBSTR(pSrc);
}
*/
// 假如bInit=False,则需要手动连接数据库
CAdo::CAdo()
{
	try
	{
		m_pConn = NULL;
		m_pRs = NULL;
		m_sErrorSql = "";		
	}
	catch(_com_error &e)
	{
		PrintProviderError(m_pConn);
        PrintComError(e);
	}
}

CAdo::~CAdo()
{
	ReleaseConn();

}

BOOL CAdo::Connect(int nTimeOut/*=0*/)
{
	ReleaseConn();
	HRESULT hResult;

	try
    {		

		hResult = m_pConn.CreateInstance(__uuidof(Connection));

		if(nTimeOut)
			m_pConn->ConnectionTimeout = nTimeOut;	// 默认是15秒

		hResult = m_pConn->Open(m_bConnStr,"","",adConnectUnspecified);
		//----modify 9.30---
		m_pConn->CursorLocation = adUseClient;
		//------------------
		
		m_pConn->CommandTimeout = CONECTTIMOUT;
	}
	catch(_com_error &e)
	{
        PrintComError(e);

		return FALSE;
    }
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAdo::GetRecordset(CString sSqlStr,_Recordset **pRs,int nOpenType)
{
	BSTR bsSql = sSqlStr.AllocSysString();
	BOOL b = GetRecordset(bsSql,pRs,nOpenType);
	SysFreeString(bsSql);
	return b;
}

CString CAdo::GetGoToPageSql(const CString &szTabName		//表名
					   , const CString &szGetFields			//需要返回的列
					   , const CString &szWhere				//查询条件,(注意:不要加单词"where",内部已经有了)
					   , const CString &szSortName			//排序的字段
					   , bool bSortDesc						//排序类型,是否为降序
					   , int nPageSize						//页尺寸,每页多少条记录
					   , int nPageIndex						//页码，即需要查询第多少页的数据
					   , const CString &szSortName2In			//第二个排序字段,可以为空
					   , bool bSortDesc2					//第二个排序字段是否为降序
					   )	//add by zfq,得到按某字段进行排序，查询某表中的某页的SQL语句
{
	CString szSql, szTmp, szOrder;
	if(szTabName.IsEmpty() || szGetFields.IsEmpty() || szSortName.IsEmpty())
	{
		ASSERT(0);
		return szSql;
	}

	CString szSortName2;
	if(szSortName.CompareNoCase(_T("ID")) && szSortName2In.IsEmpty()) //排序字段不是ID,且第二个排序字段为空
	{//防止Access查询select top N *出现并列第一的情况
		szSortName2 = _T("ID");
	}
	else
	{
		szSortName2 = szSortName2In;
	}

	CString szTmp2;
	if(FALSE == szSortName2.IsEmpty())
	{//有第二个排序字段
		szTmp2.Format(", %s %s"
			, szSortName2
			, bSortDesc2 ? "desc" : "asc");
	}

	if(bSortDesc)
	{//降序
		szTmp = _T("< ( select min");
		szOrder = _T(" order by ") + szSortName + _T(" desc") + szTmp2;
	}
	else
	{//升序
		szTmp = _T("> ( select max");
		szOrder = _T(" order by ") + szSortName + _T(" asc") + szTmp2;
	}//else 升序

	if(1 == nPageIndex)
	{//如果是第一页就执行这里，这样会加快执行速度
		if(szWhere.IsEmpty())
		{//传进来的条件为空
			szSql.Format("select top %d %s from %s %s", nPageSize, szGetFields, szTabName, szOrder);
		}
		else
		{
			szSql.Format("select top %d %s from %s where %s %s ", nPageSize, szGetFields, szTabName, szWhere, szOrder);
		}
	}
	else
	{//不是第一页
		if(szWhere.IsEmpty())
		{
			szSql.Format("select top %d %s from %s where %s %s (%s) from ( select top %u %s from %s %s ) as tabTmp) %s "
				, nPageSize
				, szGetFields
				, szTabName
				, szSortName
				, szTmp
				, szSortName
				, (nPageIndex - 1) * nPageSize
				, szSortName
				, szTabName
				, szOrder
				, szOrder);
			/*
			set @strSQL = 'select top ' + str(@PageSize) +' '+@strGetFields+ '  from '
			+ @tblName + ' where ' + @fldName + '' + @strTmp + '('+ @fldName + ') from (select top ' + str((@PageIndex-1)*@PageSize) + ' '+ @fldName + ' from ' + @tblName + '' + @strOrder + ') as tblTmp)'+ @strOrder
			*/
		}//if(szWhere.IsEmpty())
		else
		{//szWhere
			szSql.Format("select top %d %s from %s where %s %s (%s) from (select top %u %s from %s where %s %s) as TabTmp) and %s %s "
				, nPageSize
				, szGetFields
				, szTabName
				, szSortName
				, szTmp
				, szSortName
				, (nPageIndex - 1) * nPageSize
				, szSortName
				, szTabName
				, szWhere
				, szOrder
				, szWhere
				, szOrder);
			/*
			set @strSQL = 'select top ' + str(@PageSize) +' '+@strGetFields+ '  from '
			+ @tblName + ' where ' + @fldName + '' + @strTmp + '('
			+ @fldName + ') from (select top ' + str((@PageIndex-1)*@PageSize) + ' '
			+ @fldName + ' from ' + @tblName + ' where ' + @strWhere + ' '
			+ @strOrder + ') as tblTmp) and ' + @strWhere + ' ' + @strOrder
			*/
		}//else FALSE == szWhere.IsEmpty()
	}// else 1 != nPageIndex

	return szSql;
}


BOOL CAdo::GetRecordset(BSTR sSqlStr,_Recordset **pRs,int nOpenType)
{

	m_sErrorSql = CString(sSqlStr);
	WriteFileLog(m_sErrorSql);
	if( CString(sSqlStr).IsEmpty() ) 
		return FALSE;

	_RecordsetPtr pTempRs = NULL;
	BOOL bRet = FALSE;
	
	try
	{		
		if( m_pConn == NULL )
		{
			return FALSE;
		}

		if( m_pConn->State != adStateOpen )
		{
			Reconnect();
			bRet = FALSE;
		}
		else
		{
			pTempRs.CreateInstance( __uuidof(Recordset) );
			switch( nOpenType )
			{
			case ADO_OPEN_READONLY:
				pTempRs->Open((_bstr_t)sSqlStr,
							(IDispatch *)m_pConn,
							adOpenStatic,
							adLockReadOnly,
							adCmdText);			
				break;
			case ADO_OPEN_ADDNEW:
				pTempRs->Open((_bstr_t)sSqlStr,
							(IDispatch *)m_pConn,
							adOpenKeyset,
							adLockOptimistic,
							adCmdTable);
				break;
			case ADO_OPEN_UPDATE:
				pTempRs->Open((_bstr_t)sSqlStr,
							(IDispatch *)m_pConn,
							adOpenKeyset,
							adLockOptimistic,
							adCmdText);
				break;

			}
#if 0
			_StreamPtr	pStream ; //declare one first
			pStream.CreateInstance( __uuidof(Stream) ); //create it after
			pTempRs->Save( pStream.GetInterfacePtr(), adPersistADTG );    //old trick, call Save 
			int n = pStream->GetSize();
			//pStream->SaveToFile("c:\\a.xml",adSaveCreateNotExist);
			CComVariant vt = pStream->Read(n);
			vt.
			pStream->Close();
#endif		

			if( pRs != NULL )
			{
				*pRs = pTempRs.Detach();
			}
			else
			{
				pTempRs->Close();
			}
			bRet = TRUE;
		}

	}
	catch(_com_error  &e)
	{
		PrintProviderError(m_pConn);
        PrintComError(e);
		HRESULT hr= e.Error();
		DWORD dwErr = e.HRESULTToWCode(hr);
		bRet = FALSE;
		Reconnect();
	}
	catch(...)
	{
		bRet = FALSE;
	}

	return bRet;
}

void CAdo::SetConnStr(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort)
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


	m_bConnStr=sConnString;
// 	WriteFileLog("connectstr = %s",sConnString);
}

void CAdo::SetConnStr2(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szDbPort,int nDbType)
{
	CString sConnString="";
	if (SQLSERVER == nDbType)
	{//MicroSoft Sql Server数据库
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
	}
	else if (ORACLE == nDbType)
	{//Oracle数据库
		sConnString.Format("Provider=OraOLEDB.Oracle;Password=%s;User ID=%s;"
			"Data Source=(DESCRIPTION =(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s))"
			"(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = %s)));Persist Security Info=True"
			,sDbPwd,sDbUser,sDbSvr,szDbPort,sDbName);
	}
	else if (MYSQL == nDbType)
	{//MySql数据库
		sConnString += "Driver={MySQL ODBC 5.1 Driver};Server=";
		sConnString += sDbSvr;
		sConnString += ";Database=";
		sConnString += sDbName;
		sConnString += ";User=";
		sConnString += sDbUser;
		sConnString += ";Port=";
		sConnString += szDbPort;
		sConnString += ";Password=";
		sConnString += sDbPwd;
		//多语句执行
		sConnString += ";option=67108864";
		//设置gb2312格式
		sConnString += ";Charset=gb2312";
	}
	else if (ACCESS2003 == nDbType)
	{//Access(*.mdb)数据库 [office2003]
		//str = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source= D:\myweb\data\minspace.accdb"
		//str="DBQ= D:\myweb\data\minspace.accdb;DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}"
		//2003:Provider=Microsoft.Jet.OLEDB.4.0;Data Source=XXX.mdb
		//2007:Provider=Microsoft.ACE.OLEDB.12.0;Data Source=XXX.accdb;
		sConnString += "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=\"";
		sConnString += sDbName + "\"";
		sConnString += ";Jet OLEDB:Database Password='";
		sConnString += sDbPwd;
		sConnString += "'";
	}
	else if (ACCESS2007 == nDbType)
	{//Access(*.accdb)数据库 [office2007及以后]
		sConnString += "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=\"";
		sConnString += sDbName + "\"";
		sConnString += ";Jet OLEDB:Database Password='";
		sConnString += sDbPwd;
		sConnString += "'";
	}
	else
	{
		WriteFileLog("!!CAdo::SetConnStr2,nDbType=%d",nDbType);
	}


	m_bConnStr=sConnString;
// 	WriteFileLog("connectstr = %s",sConnString);
}

void CAdo::SetConnStr3(CString sDbSvr,CString sDbName,CString sDbUser,CString sDbPwd,CString szInstanceName)
{
	CString sConnString="";
	sConnString+="Provider=sqloledb;";
	//bsConn+="driver={SQL SERVER};";
	sConnString+="Data Source=";
	//bsConn+="server=";
	sConnString+=sDbSvr;
	//sConnString += ";Instance=";
	sConnString += "\\";
	sConnString += szInstanceName;
	sConnString+=";Initial Catalog=";
	//bsConn+=";database=";
	sConnString+=sDbName;
	sConnString+=";User Id=";
	sConnString+=sDbUser;
	sConnString+=";Password=";
	sConnString+=sDbPwd;
	sConnString+=";";

	m_bConnStr=sConnString;
	// 	WriteFileLog("CAdo::SetConnStr3 connectstr = %s",sConnString);
}

void CAdo::SetConnStr(CString sConnStr)
{
   m_bConnStr=sConnStr;
}
void CAdo::SetAccessConnStr(CString sDBPath)
{	
	CString sConn;
	sConn = "DRIVER={Microsoft Access Driver (*.mdb)};";
	sConn += "DBQ=";
	sConn += sDBPath;	
	sConn += ";";	
	m_bConnStr=sConn;
}
void CAdo::SetTextConnStr(CString sPath)
{
	CString sConn = "Driver={Microsoft Text Driver (*.txt; *.csv)};";
    sConn += "DBQ=";
	sConn += sPath;
	sConn += ";";
    sConn += "DefaultDir=";
	sConn += sPath;
	sConn += ";" ;

   m_bConnStr=sConn;
}

BOOL CAdo::ExecuteSQL(CString sSQL)
{
	BSTR bsSql = sSQL.AllocSysString();
	BOOL b =  ExecuteSQL(bsSql);
	SysFreeString(bsSql);
	return b;
}
BOOL CAdo::ExecuteSQL(BSTR sSQL)
{
	if( CString(sSQL).IsEmpty() ) 
		return FALSE;
	m_sErrorSql = CString(sSQL);
	WriteFileLog(m_sErrorSql);

 	_CommandPtr comm;


	BOOL bRet = FALSE;
	try
    {
		if( m_pConn == NULL  )
		{
			return FALSE;
		}
		if( m_pConn->State != adStateOpen )
		{
			Reconnect();
			return FALSE;
		}
		HRESULT hResult=comm.CreateInstance(__uuidof(Command));
		if(SUCCEEDED(hResult))
		{  
			comm->ActiveConnection=m_pConn;
			comm->CommandTimeout=COMMITTIMOUT;
			comm->CommandText=sSQL;	
			
			comm->Execute(NULL,NULL,adCmdText);
			comm.Release();
			bRet = TRUE;
		}
		
	}
	catch(_com_error  &e)
	{
		PrintProviderError(m_pConn);
        PrintComError(e);
		bRet = FALSE;
		Reconnect();
	}
	catch(...)
	{
		bRet = FALSE;
	}
	
	return bRet;
}

BOOL CAdo::GetOneData(CString sSql, _variant_t &varData)
{
	BSTR bsSql = sSql.AllocSysString();
	BOOL b = GetOneData(bsSql,varData);
	SysFreeString(bsSql);
	return b;
}

BOOL CAdo::GetOneData(BSTR sSql, _variant_t &varData)
{
	varData.Clear();
	m_sErrorSql = CString(sSql);
	WriteFileLog(m_sErrorSql);

	BOOL bRet = FALSE;
	try
	{
		_RecordsetPtr pRs=NULL;
		if( !GetRecordset(sSql,&pRs) )
		{
			return FALSE;
		}
		if( !pRs->EndOfFile )
		{
			varData = pRs->Fields->GetItem(CComVariant(0))->Value;
			bRet = TRUE;
		}

		pRs->Close();
		pRs.Release();
	}
	catch(_com_error &e)
	{
		PrintProviderError(m_pConn);
        PrintComError(e);
		return FALSE;
	}
	return bRet;
}

BOOL CAdo::GetRecordset(CString sSql, BOOL bGetArray /* = FALSE */, int nOpenType /* = ADO_OPEN_READONLY */)
{
	BSTR bs = sSql.AllocSysString();
	BOOL b = GetRecordset(bs, bGetArray, nOpenType);
	SysFreeString(bs);
	return b;
}

BOOL CAdo::GetRecordset(BSTR sSql, BOOL bGetArray /* = FALSE */, int nOpenType /* = ADO_OPEN_READONLY */)
{
	m_sErrorSql = CString(sSql);

	try
	{
		if( m_pRs != NULL )
		{
			if( m_pRs->GetState() == adStateOpen )
			{
				m_pRs->Close();
			}
		}

		if( !GetRecordset(sSql,&m_pRs, nOpenType) )
		{
			return FALSE;
		}
		if (!bGetArray)
			return TRUE;

		int index = 0;
		m_nRow = m_pRs->RecordCount;
		_variant_t vt;
		while(!m_pRs->EndOfFile)
		{
			m_nColumn = m_pRs->Fields->GetCount();
			for(int i = 0; i < m_nColumn; i++)
			{
				vt.Clear();
				vt = m_pRs->Fields->GetItem(CComVariant(i))->Value;
				CString sTmp = GetVariantStr(vt);
				m_aRows.SetAtGrow(index++,sTmp);
			}
			m_pRs->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		PrintProviderError(m_pConn);
        PrintComError(e);
		return FALSE;
	}
	
	return TRUE;
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
				PrintProviderError(m_pConn);
				PrintComError(e);
			}
		}
	}
	catch(...)
	{
		CString szMsg;
		szMsg.Format("%d",GetLastError());

	}

}

BOOL CAdo::Reconnect()
{
	ReleaseConn();
	return Connect();
}


void CAdo::SetExcelConnstr(CString sPathName)
{
	/*CString sConnStr = "Provider=Microsoft.Jet.OLEDB.4.0;";
	sConnStr += "Data Source=";
	sConnStr += sPathName;	
	sConnStr += ";Extended Properties=Excel 8.0;";
	*/
	CString sConnStr = "Provider=MSDASQL; Driver={Microsoft Excel Driver (*.xls)};DBQ=";
	sConnStr += sPathName;
	sConnStr += ";ReadOnly=False;";

	m_bConnStr=sConnStr;
}

CString CAdo::GetOneData(CString szSql)
{
	_variant_t varData;
	BSTR bsSql = szSql.AllocSysString();
	GetOneData(bsSql,varData);
	::SysFreeString(bsSql);
	varData.ChangeType(VT_BSTR);
	return CString(varData.bstrVal);
}

BOOL CAdo::GetRecordset(CString sSQL,CStringArray& RsAry,long &lRow,long &lCol)
{
	lRow = -1;
	lCol = -1;
	RsAry.RemoveAll();

	_RecordsetPtr pRs = NULL;
	try
	{
		BOOL bRet = GetRecordset(sSQL,&pRs);
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
	if(GetRecordset(sSQL,ary,nRow,nCol))
	{
		return ary.GetSize() > 0 ? TRUE : FALSE;
	}
	return FALSE;
}
BOOL CAdo::GetRow(CStringArray& RowValues, int row /* = 0 */)
{
	CString str;

	try
	{
		//RowValues.SetSize(m_nColumn);
		int i = row * m_nColumn;
		if (row >= m_nRow || row < 0)
			return FALSE;
		for (int j = 0; j < m_nColumn; j++)
		{
			str = m_aRows.GetAt(i + j);
			//RowValues.Add(str);
			RowValues.SetAtGrow(j, str);
		}
	}
	catch(_com_error &e)
	{
		PrintProviderError(m_pConn);
		PrintComError(e);
	}
	catch (...)
	{
		TRACE("ERRor\n");
	}
	return TRUE;
}
int CAdo::GetRowCount()const
{
	return m_nRow;
}
int CAdo::GetColumnCount()const
{
	return m_nColumn;
}

//得到当前时间
CString CAdo::GetDate()
{
	CString str;
	_RecordsetPtr pTempRs = NULL;
	try
	{		
		if( m_pConn == NULL )
		{
			return  _T("");
		}

		if( m_pConn->State != adStateOpen )
		{
			Reconnect();
			return _T("");
		}
		else
		{
			pTempRs.CreateInstance( __uuidof(Recordset) );

			pTempRs->Open(/*(_bstr_t)sSqlStr*/"select getdate()",
				(IDispatch *)m_pConn,
				adOpenStatic,
				adLockReadOnly,
				adCmdText);			

			_variant_t vt;
			if (!pTempRs->EndOfFile)
			{				
				vt.Clear();
				vt = pTempRs->Fields->GetItem(CComVariant(0))->Value;
				str = GetVariantStr(vt);
				//TRACE(str);
				//TRACE("\n");
			}
			pTempRs->Close();

		}

	}
	catch(_com_error  &e)
	{
		PrintProviderError(m_pConn);
		PrintComError(e);
		HRESULT hr= e.Error();
		DWORD dwErr = e.HRESULTToWCode(hr);
		str = _T("");
		Reconnect();
	}
	catch(...)
	{
		str = _T("");
	}

	return str;
}

BOOL CAdo::GetLinearSA(CString sSql, CStringArray& LinearValues)
{
	try
	{
		GetRecordset(sSql, TRUE);
		//LinearValues.SetSize(10, 10);
		for (int j = 0; j < m_nRow * m_nColumn; j++)
		{

			//RowValues.Add(str);
			LinearValues.SetAtGrow(j, m_aRows.GetAt(j));
			//TRACE(LinearValues.GetAt(j));
			//	TRACE("   ");F
		}
	}
	catch(_com_error &e)
	{
		PrintProviderError(m_pConn);
		PrintComError(e);
	}
	catch (...)
	{
		TRACE("ERRor\n");
	}
	return TRUE;
}

CString CAdo::GetVariantStr(_variant_t varValue)
{
	if( varValue.vt == VT_NULL || varValue.vt == VT_EMPTY )
		varValue.SetString("");

	varValue.ChangeType(VT_BSTR);
	return CString(varValue.bstrVal);
}

int  CAdo::GetVariantInt(_variant_t varValue)
{
	return atoi(GetVariantStr(varValue));
}
CString CAdo::GetOneString(CString sSql)
{
	_variant_t vt;
	GetOneData(sSql, vt);
	return GetVariantStr(vt);
}
int CAdo::GetOneInt(CString sSql)
{
	_variant_t vt;
	if (GetOneData(sSql, vt))
		return GetVariantInt(vt);
	return INT_MAX;
}

CString CAdo::GetGoToPageSql2(int  nClsID,CString sClsName,int nStuID,int nPageSize,int nPageIndex)	
{	
	CString szSql;
	CString szTmp = _T("< ( select min");
	CString	szOrder = _T(" order by ResName desc");

	if(1 == nPageIndex)
	{
		//如果是第一页就执行这里，这样会加快执行速度
		szSql.Format("select top %d a.* from t_ShareRes a left join t_ResControl b on a.id = b.ResId where a.ClsID = %d and a.ClsName = '%s' and b.StuID = %d %s", nPageSize,nClsID,sClsName,nStuID,szOrder);
	}
	else
	{
		szSql.Format("select top %d a.* from t_ShareRes a left join t_ResControl b on a.id = b.ResId where ResName %s (ResName) from (select top %u a.ResName from t_ShareRes a left join t_ResControl b on a.id = b.ResId where ClsID = %d and a.ClsName = '%s' and b.StuID = %d %s) as TabTmp) and a.ClsID = %d and a.ClsName = '%s' and b.StuID = %d %s",
			nPageSize,szTmp,(nPageIndex-1)*nPageSize,nClsID,sClsName,nStuID,szOrder,nClsID,sClsName,nStuID,szOrder);
	}
	return szSql;
}

BOOL CAdo::GetXml()
{
// 	CString sSqlStr;
// 	sSqlStr.Format("select * from t_c_EquipMent for xml auto");
// 
// 	m_sErrorSql = CString(sSqlStr);
// 	WriteFileLog(m_sErrorSql);
// 	if( CString(sSqlStr).IsEmpty() ) 
// 		return FALSE;
// 
// 	_CommandPtr pTempCmd = NULL;
// 	BOOL bRet = FALSE;
// 
// 	try
// 	{		
// 		if( m_pConn == NULL )
// 		{
// 			return FALSE;
// 		}
// 
// 		if( m_pConn->State != adStateOpen )
// 		{
// 			Reconnect();
// 			bRet = FALSE;
// 		}
// 		else
// 		{
// 			
// 			_variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);  
// 
// 			_StreamPtr stream; 
// 			stream.CreateInstance("ADODB.Stream"); 
// 			stream->raw_Open(varOptional, adModeUnknown, adOpenStreamUnspecified,NULL,NULL); 
// 			stream->put_Type(adTypeText); 
// 			_CommandPtr    Cmd1; 
// 			Cmd1.CreateInstance( __uuidof( Command ) ); 
// 			Cmd1->ActiveConnection = m_pConn; 
// 			Cmd1->CommandText      = _bstr_t("Select * from t_c_EquipMent for XML Auto"); 
// 			PropertiesPtr props; 
// 			props=Cmd1->GetProperties(); 
// 			PropertyPtr prop; 
// 			prop = props->Item[0]; 
// 			prop->PutValue(_variant_t("root")); 
// 			prop =props->Item; 
// 			prop->PutValue(_variant_t((IDispatch *) stream, true)); 
// 			Cmd1->Execute(&varOptional,&varOptional,adExecuteStream); 
// 			stream->SaveToFile("test.xml",adSaveCreateOverWrite); 
// 			printf("save stream to file finished\n"); 
// 			stream->Close(); 
// 			stream->raw_Write()
// 			
// 			bRet = TRUE;
// 		}
// 
// 	}
// 	catch(_com_error  &e)
// 	{
// 		PrintProviderError(m_pConn);
// 		PrintComError(e);
// 		HRESULT hr= e.Error();
// 		DWORD dwErr = e.HRESULTToWCode(hr);
// 		bRet = FALSE;
// 		Reconnect();
// 	}
// 	catch(...)
// 	{
// 		bRet = FALSE;
// 	}

	return 1;
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
				PrintProviderError(m_pConn);
				PrintComError(e);
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

//执行查询语句
//insert into t_test values ('xx') select ? = SCOPE_IDENTITY()
BOOL CAdo::ExecuteSQL(CString sSQL,int& nID)
{
	_CommandPtr comm;
	BOOL bRet = FALSE;
	try
	{
		if( m_pConn == NULL  )
		{
			return FALSE;
		}
		if( m_pConn->State != adStateOpen )
		{
			Reconnect();
			return FALSE;
		}
		HRESULT hResult=comm.CreateInstance(__uuidof(Command));
		if(SUCCEEDED(hResult))
		{  
			
			CComVariant vxxx;
			vxxx.vt = VT_INT;
			vxxx.intVal = nID;

			_ParameterPtr pParam1;
			pParam1 = comm->CreateParameter(_bstr_t ("pParam1"), 
				adInteger,adParamOutput, sizeof(int),( VARIANT ) vxxx);
			comm->Parameters->Append(pParam1);
			comm->ActiveConnection=m_pConn;
			comm->CommandTimeout=COMMITTIMOUT;
			comm->CommandText=_bstr_t(sSQL);	

			comm->Execute(NULL,NULL,adCmdText);
			
			vxxx = comm->Parameters->GetItem(CComVariant(0))->GetValue();
			nID = vxxx.intVal;

			comm.Release();
			bRet = TRUE;
			
		}

	}
	catch(_com_error  &e)
	{
		ASSERT(0);
		PrintProviderError(m_pConn);
		PrintComError(e);
		bRet = FALSE;
		Reconnect();
	}
	catch(...)
	{
		bRet = FALSE;
	}

	return bRet;
}