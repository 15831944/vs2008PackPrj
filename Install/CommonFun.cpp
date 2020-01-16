// CommonFun.cpp: implementation of the CCommonFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <comutil.h>
#include "CommonFun.h"
#include <Shobjidl.h>
#include <intshcut.h>
#include <Tlhelp32.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



#define myHeapAlloc(x) (HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,x))

typedef BOOL (WINAPI *SetSecurityDescriptorControlFnPtr)(
    IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
    IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,
    IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);

typedef BOOL (WINAPI *AddAccessAllowedAceExFnPtr)(
    PACL pAcl,
    DWORD dwAceRevision,
    DWORD AceFlags,
    DWORD AccessMask,
    PSID pSid
    );


CCommonFun::CCommonFun()
{

}

CCommonFun::~CCommonFun()
{

}

/*****************************************************************************
Author		:	zhouhonglei
CopyRight	:	zhouhonglei 2002
函数名		:	Split( CString str, CStringArray *psArray , 
					char chDelimiter=' ', int nCount=-1 )
说明		:	分割字符串函数
参数		:	
str			:	传入参数,必选项。
				字符串表达式，包含子字符串和分隔符。
				如果 str 为零长度字符串，Split 返回空数组，即不包含元素和数据的数组。
psArray:		传出参数,返回分割后的字符串数组，其中包含指定数目的子字符串.
chDelimiter	:	可选项。用于标识子字符串界限的字符。如果省略，使用空格 (",") 作为分隔符。		
nCount		:	可选项。被返回的子字符串数目，-1 指示返回所有子字符串。

返回值		:	void
*******************************************************************************/

void CCommonFun::StringSplit(CString str, CStringArray *psArray, char chDelimiter, int nCount)
{
	CString sTmp="";
	if( str.IsEmpty( ) )
	{
		return;
	}
	for( int i=0 ; i<str.GetLength( ) ; i++ )
	{
		if( str.GetAt( i ) != chDelimiter )
		{
			sTmp += str.GetAt( i );
		}
		else
		{
			if( nCount == 0 || nCount == 1 )
				break;

			psArray->Add( sTmp );
			sTmp="";
		}
		if( nCount > 1  )
		{
			if( psArray->GetSize( ) >= nCount-1 )
			{
				sTmp = str.Right( str.GetLength( ) - i - 1 );
				break;
			}			
		}
	}	
	if( nCount != 0 )
		psArray->Add(sTmp);	
}

void CCommonFun::StringSplit(const CString sSource, CStringArray *parArray , CString sSplitter,BOOL bTriNUll)
{
	int nSplitLen = sSplitter.GetLength();

	CString sTemp(sSource);
	if(sTemp.IsEmpty())
		return ;
	while(sTemp)
	{
		int nSite = sTemp.Find(sSplitter);
		if(nSite == -1)
		{
			parArray->Add(sTemp);
			break;
		}

		CString sNew = sTemp.Left(nSite);
		if(bTriNUll)
		{
			sNew.TrimLeft(" ");
			sNew.TrimLeft("\t");
			sNew.TrimRight(" ");
			sNew.TrimRight("\t");
		}
		parArray->Add(sNew);

		sTemp = (LPCTSTR)sTemp + nSplitLen +nSite;
	}
}


void CCommonFun::WriteFileLog(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);

	WriteFileLog(sTmp);
}
extern BOOL g_bIsDebug ;
void CCommonFun::WriteFileLog(CString sLog)
{
	if( sLog.GetLength() > 250 )
		TRACE(sLog.Left(250));
	else
		TRACE(sLog);
	TRACE("\n");




	CString sSysDir = GetLogDir();

	CFileFind fd;

	if( !fd.FindFile(sSysDir) )
	{
		if( !::CreateDirectory(sSysDir,NULL) )
		return;
	}

	CString sFileName;
	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format("%y%m%d");
	sFileName.Format("%s\\Install_%s.Log",sSysDir,sDate);
	
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

//--------------------------------
//将数字转变成字符串
//--------------------------------
CString CCommonFun::NToS(int nNum)
{
	CString sTmp;
	sTmp.Format("%d",nNum);
	return sTmp;
}

//--------------------------------
//将数字转变成字符串
//--------------------------------
CString CCommonFun::NToS(double dbNum)
{
	CString sTmp;
	sTmp.Format("%f",dbNum);
	return sTmp;

}
//--------------------------------
//将数字转变成字符串
//--------------------------------
CString CCommonFun::NToS(DWORD dwNum)
{
	CString sTmp;
	sTmp.Format("%u",dwNum);
	return sTmp;
}

void CCommonFun::ClearArrayElement(CObArray *pAry)
{
	if( pAry != NULL )
	{
		for( int i=0; i<pAry->GetSize(); i++ )
		{
			delete pAry->GetAt(i);
		}
		pAry->RemoveAll();
	}
	pAry = NULL;
}

int CCommonFun::SToN(CString str)
{
	return atoi(str);
}


BOOL CCommonFun::SHGetSpecialPath(int nFolder,CString &sPath)
{
	::CoInitialize(NULL);
	BOOL	bRet = TRUE;
	try
	{
		LPITEMIDLIST pidl;	
		TCHAR	szBuf[MAX_PATH] = {0};

		HRESULT hRet = NOERROR;
		hRet = SHGetSpecialFolderLocation(NULL, nFolder, &pidl);	
		bRet = ( hRet == NOERROR ) ? SHGetPathFromIDList(pidl, szBuf) : FALSE;
		sPath.Format(_T("%s"),szBuf);
	}
	catch(...)
	{
	}
	::CoUninitialize();
	return bRet;
}

BOOL CCommonFun::CreateDir(CString sPath)
{
	sPath.TrimRight("\\");
	CFileFind fd;
	if( fd.FindFile(sPath) )
	{
		return TRUE;
	}
	CStringArray sAry;
	StringSplit(sPath,&sAry,'\\');
	
	if( sAry.GetSize() < 2 )
	{
		TRACE("请输入一个绝对路径！");
		return FALSE;
	}

	CString sDriver = sAry.GetAt(0);
	if( sDriver.GetLength() != 2 || sDriver.GetAt(sDriver.GetLength()-1) != ':' )
	{
		TRACE("请输入一个带盘符的路径！");
		return FALSE;;
	}
	CString sDir=sAry.GetAt(0);
	for( int i= 1;i<sAry.GetSize();i++)
	{
		sDir += "\\" +sAry.GetAt(i) ;
		CFileFind fd;
		if( fd.FindFile(sDir) )
		{
			fd.FindNextFile();
			if (fd.IsDirectory())
			{				
				continue;			
			}		
		}

		
		if( !CreateDirectory(sDir,NULL) )
		{
			TRACE("创建目录"+sDir+"失败！");
			return FALSE;
		}
	}
	TRACE("创建目录"+sDir+"成功！");
	return TRUE;
}

BOOL CCommonFun::IsWin2003()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if( osv.dwMinorVersion == 2 )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CCommonFun::IsWin9X()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if( osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		return TRUE;
	}
	return FALSE;
}

PLATFORM CCommonFun::GetPlatform()
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osvi))
		return UNKNOWN;

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:
		return WIN_9X;
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 4)
			return WIN_NT;
		if((osvi.dwMajorVersion == 5)&&(osvi.dwMinorVersion==0))
			return WIN_2K;
		if((osvi.dwMajorVersion == 5)&&(osvi.dwMinorVersion==1))
			return WIN_XP;
		if((osvi.dwMajorVersion == 5)&&(osvi.dwMinorVersion==2))
			return WIN_2003;
	}
	return UNKNOWN;
}

CString CCommonFun::ChangePathToShort(CString sPath)
{
	char chShortPathName[MAX_PATH];
	DWORD dw = GetShortPathName(sPath,chShortPathName,MAX_PATH);
	DWORD dwErr = GetLastError();
	CString sTmp = chShortPathName;

	return sTmp;
}


CString CCommonFun::GetLastErrorInfo()
{
	CString sInfo;
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	
	// Display the string.
	sInfo = (LPTSTR)lpMsgBuf;

	// Free the buffer.
	LocalFree( lpMsgBuf );

	return sInfo;
}
BOOL CCommonFun::IsWinVistaOrLater()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if( osv.dwMajorVersion >= 6)
	{
		return TRUE;
	}
	return FALSE;
}

//-----------add by zhandongbao 2005.12.7--------------
//-----------获得目前程序运行的路径
CString CCommonFun::GetDefaultPath()
{
	TCHAR path[MAX_PATH];
	CString sDefaultPath;
	if(GetModuleFileName(0,path,MAX_PATH)==0) 
		return "";
	TCHAR *pdest;
	pdest=_tcsrchr(path,_T('\\'));
	if(pdest) 
		*pdest=0;
	sDefaultPath=path;
	sDefaultPath+=_T("\\");
	return sDefaultPath;
}

//------------------判断当前系统是否为NT系统
BOOL CCommonFun::IsWinNT()
{
	OSVERSIONINFO osv;
	ZeroMemory(&osv,sizeof(OSVERSIONINFO));

	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bSuc = GetVersionEx(&osv);
	if( osv.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		return TRUE;
	}
	return FALSE;
}


/******************************
 * Enable/Disable privilege.  *
 * Called with SE_DEBUG_NAME. *
 ******************************/
BOOL CCommonFun::EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable)
{
	HANDLE				hToken;
	TOKEN_PRIVILEGES	tp;
	LUID				luid;
	BOOL				ret;

	if (!OpenProcessToken(GetCurrentProcess(),
					      TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_READ,
					      &hToken))
		return FALSE;

	if (!LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
		return FALSE;

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

	ret = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

	CloseHandle(hToken);

	return ret;
}

DWORD CCommonFun::GetSystemPageSize()
{
	SYSTEM_INFO Info = {0};
	GetSystemInfo(&Info);
	return Info.dwPageSize;
}

BOOL CCommonFun::IsAdmin()
{
    HANDLE                   hAccessToken;
    BYTE                     *InfoBuffer;
    PTOKEN_GROUPS            ptgGroups;
    DWORD                    dwInfoBufferSize;
    PSID                     psidAdministrators;
    SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;
    UINT                     i;
    BOOL                     bRet = FALSE;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hAccessToken))
       goto cleanup;
    
    InfoBuffer = new BYTE[1024];
    if(!InfoBuffer)
       goto cleanup;

    bRet = GetTokenInformation(hAccessToken,
                               TokenGroups,
                               InfoBuffer,
                               1024,
                               &dwInfoBufferSize);

    CloseHandle( hAccessToken );

    if(!bRet)
       goto cleanup;

    if(!AllocateAndInitializeSid(&siaNtAuthority,
                                 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0,0,0,0,0,0,
                                 &psidAdministrators))
       goto cleanup;

    bRet = FALSE;

    ptgGroups = (PTOKEN_GROUPS)InfoBuffer;

    for(i=0;i<ptgGroups->GroupCount;i++)
    {
        if(EqualSid(psidAdministrators,ptgGroups->Groups[i].Sid))
        {
            bRet = TRUE;
            break;
        }
    }

    FreeSid(psidAdministrators);

cleanup:

    if (InfoBuffer)
        delete InfoBuffer;

    return bRet;
}

BOOL CCommonFun::StrToTime(CString &sStr, CTime &tm)
{
	if(sStr.Find(".") > 0)
	{
		CStringArray ary;
		StringSplit(sStr,&ary,".");
		if(ary.GetSize() != 3)
			return FALSE;
		int nYear = atoi(ary.GetAt(0));
		int nMonth = atoi(ary.GetAt(1));
		int nDay = atoi(ary.GetAt(2));
		
		CTime tmp(nYear,nMonth,nDay,0,0,0);
		tm = tmp;
		return TRUE;
	}
	CString sDate,
			sTime;
	int nIndex = sStr.Find(" ",0);
	sDate = sStr.Left(nIndex);
	sTime = sStr.Right(sStr.GetLength() - nIndex - 1);

	CStringArray ary;
	StringSplit(sDate,&ary,"-");
	if(ary.GetSize() != 3)
		return FALSE;
	int nYear = atoi(ary.GetAt(0));
	int nMonth = atoi(ary.GetAt(1));
	int nDay = atoi(ary.GetAt(2));
	
	ary.RemoveAll();
	StringSplit(sTime,&ary,":");
	if(ary.GetSize() != 3)
		return FALSE;
	int nHour = atoi(ary.GetAt(0));
	int nMinute = atoi(ary.GetAt(1));
	int nSecond = atoi(ary.GetAt(2));
	CTime tmp(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	tm = tmp;
	return TRUE;
}

BOOL CCommonFun::Is256ColorSupported()
{
	BOOL bRetval = FALSE;

	// return TRUE if screen deivce supports 256 colors or better

	HDC hdc = GetDC(NULL);
	if (hdc != NULL)
	{
		if(GetDeviceCaps(hdc, BITSPIXEL) >= 8)
			bRetval = TRUE;
		ReleaseDC(NULL, hdc);
	}

	return bRetval;
}

CString CCommonFun::GetVariantStr(_variant_t varValue)
{
	if( varValue.vt == VT_NULL || varValue.vt == VT_EMPTY )
		varValue.SetString("");
	varValue.ChangeType(VT_BSTR);
	return CString(varValue.bstrVal);
}

int CCommonFun::CmpDate(CTime &tm1, CTime &tm2)
{
	CTimeSpan ts = tm1 - tm2;
	int n = ts.GetTotalHours();
	return n > 24 ? 1 : ( n <= 24 && n >= 0 ? 0 : -1);
}

CString CCommonFun::GetLocalIP(void)
{
	CString sIP;
	/*if(InitSockDll())*/
	{
		char localHost[255];
		memset(localHost,0,255);
		gethostname(localHost, 255);
		int nError = GetLastError();
		struct hostent *host = gethostbyname(localHost);	//Get local host IP / 
		if(host)
		{
			sIP = inet_ntoa (*(struct in_addr*)*host->h_addr_list);
		}
		/*ReleaseSockDll();*/
	}
	return sIP;
}
CString CCommonFun::GetLoaclHost(void)
{
	CString sHost = _T("");
	/*if(InitSockDll())*/
	{
		char localHost[255];
		memset(localHost,0,255);
		gethostname(localHost, 255);
		sHost = localHost;
		/*ReleaseSockDll();*/
	}
	return sHost;
}

int CCommonFun::GetLocalIPList(CStringArray& ary)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	DWORD ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		//GlobalFree (pAdapterInfo);
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
	}

	MIB_IF_TYPE_ETHERNET;
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{
			CString sMac;
			BYTE* pbHexMac = pAdapter->Address;
			sMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),pbHexMac[0],pbHexMac[1],pbHexMac[2],pbHexMac[3],pbHexMac[4],pbHexMac[5]);



			PIP_ADDR_STRING pAddr = &(pAdapter->IpAddressList);
			int nCurIPIndex = 0;
			while (pAddr)
			{

				CString sIP(pAddr->IpAddress.String);
				if (!sIP.IsEmpty() && sIP.CompareNoCase(_T("0.0.0.0")) != 0)
				{
					ary.Add(sIP);
				}

				pAddr = pAddr->Next;
			}

			pAdapter = pAdapter->Next;

		}
	}
	else 
	{
		ASSERT(0);
	}
	free(pAdapterInfo);
	return ary.GetSize();
}


BOOL CCommonFun::IsLocalIP(CString sIP)
{
	CStringArray ary;
	GetLocalIPList(ary);
	for(int n = 0; n < ary.GetSize(); n++)
	{
		if(ary.GetAt(n).CompareNoCase(sIP) == 0 )
			return TRUE;
	}
	return FALSE;
}
void CCommonFun::ModifyFileAttribute(CString strPath, BYTE bToRemove, BYTE bToAdd)
{
	CFileStatus status;
	if(CFile::GetStatus(strPath, status))
	{
		status.m_attribute &= ~bToRemove;
		status.m_attribute |= bToAdd;
		try
		{
			CFile::SetStatus(strPath, status);
		}
		catch(...)
		{
			//CFileException 拒绝访问？
		}
	}
}
void CCommonFun::DeleteDir(CString sDir, BOOL bOnlyDelFile)
{
	CFileFind	finder;				
	CString		sFullPath = _T("");					
	CString		strWildcard;

	sDir.TrimRight(_T("\\"));

	strWildcard = sDir + _T("\\*.*");
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			DeleteDir(finder.GetFilePath());		
		}
		else
		{
			CString sName = finder.GetFileName();
			CString sPath = finder.GetFilePath();
			/*CFileStatus st ;
			CFile::GetStatus(sTmp,st);
			st.m_attribute = 0;
			CFile::SetStatus(sTmp,st);*/
			theLog.Write("del file %s",sPath);
			if(!DeleteFile(sPath) && (GetLastError() == 5))
			{
				ModifyFileAttribute(sPath, 0x01 | 0x02 | 0x04, NULL);
				if (!DeleteFile(sPath))
				{
					CCommonFun::WriteFileLog(_T("%s删除失败，err = %d, 重启后删除"), sPath, GetLastError());//?s
					MoveFileEx(sPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
				}
			}
		}
	}
	finder.Close();	
	//删除目录
	if(!bOnlyDelFile)
	{
		if(!::RemoveDirectory(sDir) && (GetLastError() == 5))
		{
			ModifyFileAttribute(sDir, 0x01 | 0x02 | 0x04, NULL);
			::RemoveDirectory(sDir);
		}
	}
}

BOOL CCommonFun::DeleteFileOrDir(CString szPath)	//add by zfq, 删除一个文件或目录
{
	BOOL BRet = FALSE;
	if(!::PathFileExists(szPath))
	{
		ASSERT(0);
		theLog.Write("DeleteFileOrDir %s not exist",szPath);
		return FALSE;
	}

	if(PathIsDirectory(szPath))
	{//是目录
		DeleteDir(szPath, FALSE);
		BRet = TRUE;
	}
	else
	{//是文件
		BRet = ::DeleteFile(szPath);
	}

	return BRet;
}


BOOL CCommonFun::ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken    = NULL;

	// Open the process token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken ))
	{
		TRACE(_T("Failed OpenProcessToken\n"));
		return ERROR_FUNCTION_FAILED;
	}

	// Get the local unique id for the privilege.
	if ( !LookupPrivilegeValue( NULL,
		szPrivilege,
		&luid ))
	{
		CloseHandle( hToken );
		TRACE(_T("Failed LookupPrivilegeValue\n"));
		return ERROR_FUNCTION_FAILED;
	}

	// Assign values to the TOKEN_PRIVILEGE structure.
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes = 
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);

	// Adjust the token privilege.
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		sizeof(NewState),
		NULL,
		NULL))
	{
		TRACE(_T("Failed AdjustTokenPrivileges\n"));
		hr = ERROR_FUNCTION_FAILED;
	}

	// Close the handle.
	CloseHandle(hToken);

	return hr;
}

BOOL CCommonFun::CreateShellLink(LPCTSTR filePath, LPCTSTR lnkPath, LPCTSTR iconPath, int iconIndex, LPCTSTR params)
{
	//theLog.Write("CreateShellLink [%s][%s][%s][%d][%s]", filePath, lnkPath, iconPath?iconPath:"", iconIndex, params?params:"");

	HRESULT hres;
	IShellLink *psl = NULL;
	IPersistFile *pPf = NULL;
	WCHAR wsz[256];
	hres = CoCreateInstance(CLSID_ShellLink,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellLink,
		(LPVOID*)&psl);
	if(FAILED(hres))
		goto cleanup;
	hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&pPf);
	if(FAILED(hres))
		goto cleanup;
	hres = psl->SetPath(filePath);
	if(FAILED(hres))
		goto cleanup;
	if(params!=NULL) {
		hres = psl->SetArguments(params);
		if(FAILED(hres))
			goto cleanup;
	}

	MultiByteToWideChar(CP_GB2312, 0, lnkPath, -1, wsz, MAX_PATH);
	hres = pPf->Save(wsz, TRUE);
	if(FAILED(hres))
		goto cleanup;

	if(iconPath != NULL) 
	{
		hres = psl->SetIconLocation(iconPath, iconIndex);
		if(FAILED(hres))
			goto cleanup;
		pPf->Save(wsz, TRUE);
	}
cleanup:
	if(pPf)
		pPf->Release();
	if(psl)
		psl->Release();

	return TRUE;
}

BOOL CCommonFun::CreateURLLink(LPCTSTR url, LPCTSTR lnkPath, LPCTSTR iconPath, int iconIndex)
{
	//theLog.Write("CreateShellLink [%s][%s][%s][%d]", url, lnkPath, iconPath, iconIndex);

	HRESULT hres;
	IUniformResourceLocator *purl;
	IPersistFile *ppf;
	IPropertySetStorage* psps;
	IPropertyStorage *pps;
	WCHAR wsz[256]; // buffer for Unicode string
	hres = CoCreateInstance(CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER,
		IID_IUniformResourceLocator, (void **)&purl);
	hres = purl->QueryInterface(IID_IPersistFile, (void **)&ppf);
	// Set URL
	hres = purl->SetURL(url, IURL_SETURL_FL_GUESS_PROTOCOL );

	MultiByteToWideChar(CP_GB2312, 0, lnkPath, -1, wsz, MAX_PATH);
	// Save shortcut (with remember)
	hres = ppf->Save( wsz, TRUE );

	if(iconPath != NULL)
	{
		// get PropertySetStorage-Interface
		hres = purl->QueryInterface(IID_IPropertySetStorage, (void**)&psps );
		// Open ProertySet
		hres = psps->Open(FMTID_Intshcut, STGM_READWRITE, &pps );
		// Array of Propertyps to write (by PROP_ID)
		PROPSPEC ppids[2] = { {PRSPEC_PROPID, PID_IS_ICONINDEX}, {PRSPEC_PROPID, PID_IS_ICONFILE} };
		// Array of Property-Variants
		PROPVARIANT ppvar[2];

		MultiByteToWideChar(CP_GB2312, 0, iconPath, -1, wsz, MAX_PATH);
		// Initialize PropVars
		PropVariantInit( ppvar );
		PropVariantInit( ppvar + 1 );
		ppvar[0].vt = VT_I4; // Index is I4
		ppvar[0].lVal = iconIndex;
		ppvar[1].vt = VT_LPWSTR; // Iconfile is LPWSTR
		ppvar[1].pwszVal = wsz; // Name of Iconfile (shell32.dll)
		// Write Propertydata
		hres = pps->WriteMultiple(2, ppids, ppvar, 0 );
		// Commit Propertydata (flush)
		hres = pps->Commit(STGC_DEFAULT);
		// Save icon (remembered)
		hres = ppf->Save(NULL, TRUE);
		psps->Release();
		pps->Release();
	}

	ppf->Release();
	purl->Release(); 

	return TRUE;
}

CString CCommonFun::GetProgramDir()
{
	TCHAR buf[256];
	LPITEMIDLIST pidl;
	SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidl);
	SHGetPathFromIDList(pidl, buf);
	CoTaskMemFree(pidl);

	return CString(buf);
}

CString CCommonFun::GetDesktopDir()
{
	TCHAR buf[256];
	LPITEMIDLIST pidl;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
	SHGetPathFromIDList(pidl, buf);
	CoTaskMemFree(pidl);

	return CString(buf);
}

CString CCommonFun::GetCommonDesktopDir()
{
	TCHAR buf[256];
	LPITEMIDLIST pidl;
	SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_DESKTOPDIRECTORY, &pidl);
	SHGetPathFromIDList(pidl, buf);
	CoTaskMemFree(pidl);

	return CString(buf);
}


//
// 弹出对话框, 允许用户选择一个文件夹, 并返回该文件夹路径
// 参数:bNewStyle	为TRUE时使用新的对话框风格
//					为FALSE时使用旧的对话框风格
//					参数MSDN: SHBrowseForFolder
CString CCommonFun::GetDirFromShell(BOOL bNewStyle, HWND hOwner/*=NULL*/)
{
	CoInitialize(NULL);
	BROWSEINFO bi; 
	ITEMIDLIST * pidl; 
	char szDir[MAX_PATH] = {0};
	bi.hwndOwner=hOwner; 
	bi.pidlRoot=NULL; 
	bi.pszDisplayName=szDir; 
	bi.lpszTitle= "选择文件夹"; 
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	if(bNewStyle)
		bi.ulFlags |= BIF_NEWDIALOGSTYLE; 
	bi.lpfn=NULL; 
	bi.lParam=0; 
	bi.iImage=0; 

	pidl=SHBrowseForFolder(&bi);
	if(pidl==NULL)
		szDir[0] = 0;
	else
	{
		if(!SHGetPathFromIDList(pidl,szDir))
			szDir[0] = 0; 
	}
	// AfxMessageBox(szDir);

	CoUninitialize();

	return szDir;
}

DWORD CCommonFun::StartSvc(CString strSvcName)
{
	SC_HANDLE  schService;
	BOOL       ret;
	DWORD      err;

	SC_HANDLE SchSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	schService = OpenService (SchSCManager,
		strSvcName,
		SERVICE_ALL_ACCESS
		);

	if (schService == NULL)
	{
		TRACE (("failure: OpenService error\n"));
		return FALSE;
	}
	ret = StartService (schService,    // service identifier
		0,             // number of arguments
		NULL           // pointer to arguments
		);
	err = GetLastError();
	if (ret)
	{
		TRACE (("StartService SUCCESS\n"));
	}
	else
	{
		if (err == ERROR_SERVICE_ALREADY_RUNNING)
		{
			//
			// A common cause of failure (easier to read than an error code)
			//

			TRACE (("failure: StartService, ERROR_SERVICE_ALREADY_RUNNING\n"));
		}
		else
		{
			TRACE (("failure: StartService error\n"));
		}
	}
	CloseServiceHandle (schService);
	return ret;
}

DWORD CCommonFun::StopSvc(CString strSvcName)
{
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm!=NULL)
	{
		SC_HANDLE svc = OpenService(scm,strSvcName,SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			QueryServiceStatus(svc,&ssa);
			if (ssa.dwCurrentState == SERVICE_RUNNING || ssa.dwCurrentState == SERVICE_PAUSED)
			{
				int nret=ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
				if(!nret)
				{
					int nerr=GetLastError();
					return nerr;
				}
			}
			CloseServiceHandle(svc);
		}                           
		CloseServiceHandle(scm);
	}
	return 0;
}

static BOOL /*CCommonFun::*/WaitForState(DWORD dwDesiredState, SC_HANDLE svc) 
{
	BOOL bResult = FALSE;
	// Loop until driver reaches desired state or error occurs
	while (1)
	{
		SERVICE_STATUS ssa;
		// Get current state of driver
		bResult = ::QueryServiceStatus(svc, &ssa);
		// If we can't query the driver, we're done
		if (!bResult)
		{
			CCommonFun::WriteFileLog("!!WaitForState QueryServiceStatus fail err=%d,svc=%X",GetLastError(), svc);
			break;
		}
		// If the driver reaches the desired state
		if (ssa.dwCurrentState == dwDesiredState) 
			break;
		// We're not done, wait the specified period of time
		DWORD dwWaitHint = ssa.dwWaitHint / 10;    // Poll 1/10 of the wait hint
		if (dwWaitHint <  1000) dwWaitHint = 1000;  // At most once a second
		if (dwWaitHint > 10000) dwWaitHint = 10000; // At least every 10 seconds
		::Sleep(dwWaitHint);
	} // while

	return bResult;
}

BOOL CCommonFun::StopSMService(CString sServiceName)
{
	WriteFileLog("StopSMService,1,sServiceName=[%s]",sServiceName);
	BOOL bRet = FALSE;
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		SC_HANDLE svc = OpenService(scm,sServiceName,SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			QueryServiceStatus(svc,&ssa);
			if (ssa.dwCurrentState == SERVICE_RUNNING)
			{
				bRet = ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
				if(bRet)
				{
					bRet = WaitForState(SERVICE_STOPPED, svc);
					if (!bRet)
					{
						WriteFileLog("!!StopSMService,6,WaitForState fail,err=[%u]",GetLastError());
					}
				}
				else
				{
					WriteFileLog("!!StopSMService,5,ControlService fail,err=[%u]",GetLastError());
				}
			}
			else
			{
				WriteFileLog("!!StopSMService,4,QueryServiceStatus fail,err=[%u]",GetLastError());
			}
			CloseServiceHandle(svc);
		}                           
		else
		{
			WriteFileLog("!!StopSMService,3,OpenService fail,err=[%u]",GetLastError());
		}
		CloseServiceHandle(scm);
	}
	else
	{
		WriteFileLog("!!StopSMService,2,OpenSCManager fail,err=[%u]",GetLastError());
	}
	return bRet;
}

BOOL CCommonFun::StartSMService(CString sServiceName)
{
	WriteFileLog("StartSMService,1,sServiceName=[%s]",sServiceName);
	BOOL bRet = FALSE;
	SC_HANDLE SchSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (SchSCManager != NULL)
	{
		SC_HANDLE  schService;
		schService = OpenService(SchSCManager, sServiceName, SERVICE_ALL_ACCESS);
		if (schService != NULL)
		{
			bRet = StartService(schService, 0, NULL);
			if (bRet)
			{
				bRet = WaitForState(SERVICE_RUNNING, schService);
				if (!bRet)
				{
					WriteFileLog("!!StartSMService,6,WaitForState fail,err=[%u]",GetLastError());
				}
			}
			else
			{
				DWORD dwErr = GetLastError();
				if (dwErr == ERROR_SERVICE_ALREADY_RUNNING)
				{
					bRet = TRUE;
					WriteFileLog("StartSMService,5,StartService fail,err=[ERROR_SERVICE_ALREADY_RUNNING]");
				}
				else
				{
					WriteFileLog("!!StartSMService,4,StartService fail,err=[%u]",dwErr);
				}
			}
			CloseServiceHandle(schService);
		}
		else
		{
			WriteFileLog("!!StartSMService,3,OpenService fail,err=[%u]",GetLastError());
		}
		CloseServiceHandle(SchSCManager);
	}
	else
	{
		WriteFileLog("!!StartSMService,2,OpenSCManager fail,err=[%u]",GetLastError());
	}

	return bRet;
}

BOOL CCommonFun::CreateProcessAndWait(LPSTR cmd)
{
	STARTUPINFO si={0};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi={0};

	if(!CreateProcess(0,
		cmd,
		0,
		0,
		0,
		0,
		0,
		0,
		&si,
		&pi))
		return FALSE;

	//WaitForSingleObject(pi.hProcess, INFINITE);
	AtlWaitWithMessageLoop(pi.hProcess);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return TRUE;
}

/*
#define EX_WIN_LOGOFF	1
#define	EX_WIN_REBOOT	2
#define EX_WIN_SHUTDOWN	3
*/
BOOL CCommonFun::ExitWin(int iMode)
{

	//1-注销当前用户；2-重新启动；3-关闭;

	OSVERSIONINFOEX stOSVI ;
	ZeroMemory(&stOSVI,sizeof(OSVERSIONINFO)) ;
	stOSVI.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	BOOL bRet = FALSE;
	CCommonFun::WriteFileLog(_T("----Come in OperateMachine-------"));
	if(!CCommonFun::IsWin9X())
	{
		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp;  
		if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken)) 
		{
			CCommonFun::WriteFileLog(_T("----OpenProcessToken faile-------"));
			return FALSE;
		}

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);  
		tkp.PrivilegeCount=1;
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;  

		AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);

		switch(iMode)
		{
		case EX_WIN_LOGOFF:
			bRet = ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
			break;
		case EX_WIN_REBOOT:
			CCommonFun::WriteFileLog(_T("ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0)"));
			bRet = ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
			if(!bRet)
			{
				DWORD dw = ::GetLastError();
				CCommonFun::WriteFileLog(_T("---------bRet = %d ----"),bRet);
			}
			break;
		case EX_WIN_SHUTDOWN:
			CCommonFun::WriteFileLog(_T("EWX_SHUTDOWN|EWX_FORCE|EWX_POWEROFF,0)"));
			bRet = ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE|EWX_POWEROFF,0);
			if(!bRet)
			{
				DWORD dw = ::GetLastError();
				CCommonFun::WriteFileLog(_T("---------bRet = %d ----"),bRet);
			}
			break;
		default:
			break;
		}

	}
	else
	{
		switch(iMode)
		{
		case EX_WIN_LOGOFF:
			bRet = ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
			break;
		case EX_WIN_REBOOT:
			bRet = ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
			break;
		case EX_WIN_SHUTDOWN:
			bRet = ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);
			break;
		default:
			break;
		}
	}
	CCommonFun::WriteFileLog(_T("bRet = %d"), bRet);
	return bRet;
}
CStringA CCommonFun::GetUninstallString(CStringA sDisplayName)
{
	HKEY   hKey = NULL; 
	CStringA sKeyPath = "";

	if (IsWow64() && IsWinVistaOrLater())
		sKeyPath = "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";  
	else
		sKeyPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";  
	DWORD  dwAttr = REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;  
	CStringA sUninstallString;
	DWORD dwIndex = 0;
	char szSubKeyName[MAX_PATH] = {0};
	DWORD dwSubKeyNameLen = MAX_PATH;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,sKeyPath,0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)   
	{   
		while(RegEnumKeyExA(hKey,dwIndex ++,szSubKeyName,&dwSubKeyNameLen,0,NULL,NULL,0) != ERROR_NO_MORE_ITEMS)   
		{   
			if (strcmp(szSubKeyName ,"") != NULL)   
			{  
				CStringA sSubKeyPath;
				sSubKeyPath.Format("%s\\%s",sKeyPath,szSubKeyName);

				char szDisplayName[MAX_PATH] = {0};
				DWORD dwDisplayNameSize = MAX_PATH; 

				char szUninstall[MAX_PATH] = {0};
				DWORD dwUnistallSize = MAX_PATH;

				HKEY hSubKey = NULL;
				//再打开，查找软件显示现实名称和卸载命令行   
				if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,sSubKeyPath,0,KEY_ALL_ACCESS,&hSubKey) == ERROR_SUCCESS)   
				{   
					RegQueryValueExA(hSubKey,"DisplayName",0,&dwAttr,LPBYTE(szDisplayName),&dwDisplayNameSize);   
					RegQueryValueExA(hSubKey,"UninstallString",0,&dwAttr,LPBYTE(szUninstall),&dwUnistallSize);

					if ((stricmp(szDisplayName,sDisplayName) == 0))   
					{  
						sUninstallString = szUninstall;
						::RegCloseKey(hSubKey);
						break;
					}   
					::RegCloseKey(hSubKey);
				}   

			}   
			dwSubKeyNameLen = MAX_PATH;
			memset(szSubKeyName,0,MAX_PATH);
		}//end   while   
		RegCloseKey(hKey);   
	} 
	theLog.Write("last error is %d",::GetLastError());
	return sUninstallString;
}

BOOL CCommonFun::UninstallSoftware(CString sDisplayName)
{
	CString s = GetUninstallString(sDisplayName);
	BOOL bRet = TRUE;
	do 
	{
		if (s.IsEmpty())
		{
			bRet = FALSE;
			break;
		}

		theLog.Write("卸载数据库字符串 %s",s);
		CStringArray ary;
		CCommonFun::StringSplit(s,&ary,".exe ");

		// 得到卸载的参数
		CString sPath = "";
		CString sParam = "";
		if (ary.GetCount() > 1)
		{
			char szSysWindow[MAX_PATH] = {0};
			::GetSystemDirectory(szSysWindow,MAX_PATH);
			sPath.Format("%s\\%s.exe",szSysWindow,ary.GetAt(0));
			sParam.Format("%s /q",ary.GetAt(1));
			sParam.Replace("/I","/X");
		}
		else
		{
			sPath.Format("%s",ary.GetAt(0));
			sPath.TrimLeft("\"");
			sPath.TrimRight("\"");
		}

		theLog.Write("卸载数据库组件 path = %s,param = %s",sPath,sParam);	
		Install(sPath,sParam,INFINITE,SW_SHOW);
		//	bRet = CSecurityTool::CreateProcByCurUsr(sPath.GetBuffer(),sParam.GetBuffer(),SW_SHOWNORMAL);
		sParam.ReleaseBuffer();
		sParam.ReleaseBuffer();
	} while (0);
	return bRet;
}

// 添加共享目录
BOOL CCommonFun::AddNetShare(CString sShareName,CString sShareDesc,CString sDirPath)
{	
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // 如果目录不存在
    if (!PathFileExists(sDirPath))
        return FALSE;

    NET_API_STATUS res;
    SHARE_INFO_2 p;
    DWORD parm_err = 0;
    WCHAR pName[MAX_PATH];
    WCHAR pRemark[MAX_PATH];
    WCHAR pDirPath[MAX_PATH];

    // Fill in the SHARE_INFO_2 structure.
    swprintf(pName,sShareName.AllocSysString());
    p.shi2_netname = pName;
    p.shi2_type = STYPE_DISKTREE; // disk drive
    swprintf(pRemark, sShareDesc.AllocSysString());
    p.shi2_remark = pRemark;
    p.shi2_permissions = ACCESS_ALL;
    p.shi2_max_uses = -1;
    p.shi2_current_uses = 0; 
    memset(pDirPath,0,MAX_PATH);
    MultiByteToWideChar(CP_GB2312,0,sDirPath,-1,pDirPath,sDirPath.GetLength());
    p.shi2_path = pDirPath;   
    p.shi2_passwd = NULL; 

    // Call the NetShareAdd function,
    res=NetShareAdd(NULL,2,(LPBYTE)&p,&parm_err);
    if(res == NERR_Success)
        return TRUE;
    else
        return FALSE;
}

// 判断共享目录是否存在
BOOL CCommonFun::CheckNetShare(CString sDirPath)
{
    NET_API_STATUS res;
    DWORD devType = 0;
    WCHAR pBuf[MAX_PATH];
    memset(pBuf,0,MAX_PATH);
    MultiByteToWideChar(CP_GB2312,0,sDirPath,-1,pBuf,sDirPath.GetLength());
    res=NetShareCheck(NULL,pBuf,&devType);
    if(res == NERR_Success)
        return TRUE;
    else
        return FALSE;
}
// 删除共享目录
BOOL CCommonFun::DelNetShare(CString sShareName)
{
    NET_API_STATUS res;
    WCHAR pBuf[MAX_PATH];
    memset(pBuf,0,MAX_PATH);
    MultiByteToWideChar(CP_GB2312,0,sShareName,-1,pBuf,sShareName.GetLength());
    res=NetShareDel(NULL,pBuf,0);
    if(res == NERR_Success)
        return TRUE;
    else
        return FALSE;
    return TRUE;
}



BOOL CCommonFun::AddRights(TCHAR* lpFileName,TCHAR* lpAccountName,DWORD dw)
{
    //lookupAccountName相关变量
    PSID pUserID=NULL;
    DWORD dUserID=0;
    TCHAR* pDomainName=NULL;
    DWORD dDomainName=0;
    SID_NAME_USE snuType;

    //第一步，取得传入的用户或用户组的SID
    BOOL check1=LookupAccountName(NULL,lpAccountName,pUserID,&dUserID,pDomainName,&dDomainName,&snuType);
    pUserID=(PSID)myHeapAlloc(dUserID);
    pDomainName=(TCHAR*)myHeapAlloc(dDomainName*sizeof(TCHAR));
    check1=LookupAccountName(NULL,lpAccountName,pUserID,&dUserID,pDomainName,&dDomainName,&snuType);

    //第二步，取得文件的安全描述符(SD)
    PSECURITY_DESCRIPTOR pFileSD=NULL;
    DWORD dFileSD=0;
    SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;

    check1=GetFileSecurity(lpFileName,secInfo,pFileSD,0,&dFileSD);
    pFileSD=(PSECURITY_DESCRIPTOR)myHeapAlloc(dFileSD);
    check1=GetFileSecurity(lpFileName,secInfo,pFileSD,dFileSD,&dFileSD);

    //第三步，初始一个新的SD
    SECURITY_DESCRIPTOR newSD;
    InitializeSecurityDescriptor(&newSD,SECURITY_DESCRIPTOR_REVISION);

    //第四步，从SD中取出DACL
    BOOL bPresent;
    BOOL bDaclDefault;
    PACL pACL=NULL;
    GetSecurityDescriptorDacl(pFileSD,&bPresent,&pACL,&bDaclDefault);

    //第五步，取DACL的内存大小
    ACL_SIZE_INFORMATION aclInfo;
    aclInfo.AceCount=0;
    aclInfo.AclBytesFree=0;
    aclInfo.AclBytesInUse=sizeof(ACL);

    // 如果DACL不为空，则取其信息
    if(bPresent){
        GetAclInformation(pACL,&aclInfo,sizeof(ACL_SIZE_INFORMATION),AclSizeInformation);
    }

    //第六步，计算新的ACL的size
    DWORD dNewAcl=aclInfo.AclBytesInUse+sizeof(ACL_SIZE_INFORMATION)+GetLengthSid(pUserID)-2;

    //第七步，为新ACL分配内存
    PACL pNewAcl=(PACL)myHeapAlloc(dNewAcl);

    //第八步，初始化新的acl
    InitializeAcl(pNewAcl,dNewAcl,ACL_REVISION2);

    //第九步，如果文件（目录）的DACL有数据则拷贝数据到新的DACL
    UINT newAclIndex=0;
    UINT curAclIndex=0;
    LPVOID pTempAce=NULL;
    if (bPresent&&aclInfo.AceCount)
    {
        for (curAclIndex=0;curAclIndex!=aclInfo.AceCount;curAclIndex++)
        {
            GetAce(pACL,curAclIndex,&pTempAce);
            if (((ACCESS_ALLOWED_ACE*)pTempAce)->Header.AceFlags==INHERITED_ACE)
                break;

            if (EqualSid(pUserID,&(((ACCESS_ALLOWED_ACE*)pTempAce)->SidStart)))
            {
                printf("suc");
                continue;
            }

            AddAce(pNewAcl,ACL_REVISION,MAXDWORD,pTempAce,((PACE_HEADER)pTempAce)->AceSize);
            newAclIndex++;
        }
    }

    SetSecurityDescriptorControlFnPtr _SetSecurityDescriptorControl = NULL;
    AddAccessAllowedAceExFnPtr _AddAccessAllowedAceEx = NULL;
    //动态装载函数
    _AddAccessAllowedAceEx = (AddAccessAllowedAceExFnPtr)
        GetProcAddress(GetModuleHandle(TEXT("advapi32.dll")),
        "AddAccessAllowedAceEx");
    _AddAccessAllowedAceEx(pNewAcl, ACL_REVISION2,
        CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE ,
        dw, pUserID);

    //拷贝继承的ACE
    if (bPresent&&aclInfo.AceCount)
    {
        for (;curAclIndex!=aclInfo.AceCount;curAclIndex++)
        {
            GetAce(pACL,curAclIndex,&pTempAce);
            AddAce(pNewAcl,ACL_REVISION,MAXDWORD,pTempAce,((PACE_HEADER)pTempAce)->AceSize);
        }
    }

    //把新的acl设置到新的sd中
    check1=SetSecurityDescriptorDacl(&newSD,TRUE,pNewAcl,FALSE);
    check1=SetFileSecurity(lpFileName,secInfo,&newSD);
    DWORD err=GetLastError();
    return TRUE;
}

BOOL CCommonFun::TerminalProcess(CString sProc)
{
	DWORD dwRet = 0;

	//查找
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hSnapshot != INVALID_HANDLE_VALUE)
	{
		sProc.MakeUpper();
		PROCESSENTRY32 ProcessEntry;  
		ZeroMemory(&ProcessEntry,sizeof(PROCESSENTRY32));
		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		BOOL bFlag = Process32First(hSnapshot,&ProcessEntry);
		while(bFlag)
		{
			CString strExePath
				= _T(ProcessEntry.szExeFile);
			strExePath.MakeUpper();
			if(strExePath.Find(sProc) != -1)
			{
				dwRet = ProcessEntry.th32ProcessID;
				break;
			}
			ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
			bFlag = Process32Next(hSnapshot,&ProcessEntry);
		}
		CloseHandle(hSnapshot);
	}
	if (dwRet == 0)
		return 0;

	//终结
	//提升权限
	CCommonFun::ModifyPrivilege(SE_DEBUG_NAME,TRUE);
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwRet);
	if (hProcess == NULL)
		return 0;
	dwRet = TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	return dwRet ;
}

BOOL CCommonFun::TerminaPID(DWORD dw)
{
	DWORD fResult  = FALSE;
	HANDLE hProc  = NULL;

	__try
	{
		ModifyPrivilege(SE_DEBUG_NAME,TRUE);
		// Open the process with PROCESS_QUERY_INFORMATION access
		hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dw);
		if (hProc == NULL)
		{
			__leave;
		}
		fResult = ::TerminateProcess(hProc,0);
		::CloseHandle(hProc);
	}
	__finally
	{
	}
	return fResult;
}

//获取日志目录
CString& CCommonFun::GetLogDir()
{
	static CString g_szLogDir = "";
	if (g_szLogDir.IsEmpty())
	{
		OutputDebugString("##CCommonFun::GetLogDir");

		TCHAR path[MAX_PATH];
		CString sDefaultPath;
		if(GetModuleFileName(0,path,MAX_PATH))
		{
			TCHAR *pdest;
			pdest=_tcsrchr(path,_T('\\'));
			if(pdest) 
				*pdest=0;
			sDefaultPath=path;
			sDefaultPath += "\\xlog";
		}

		if (sDefaultPath.IsEmpty())
		{
			sDefaultPath = ".\\xlog";
		}

		g_szLogDir = sDefaultPath;
	}

	return g_szLogDir;
}
