// CommonFun.cpp: implementation of the CCommonFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <comutil.h>
#include "CommonFun.h"
#include <Shobjidl.h>
#include <intshcut.h>
#include "shlwapi.h"
#include "atlbase.h"
#include "Resource.h"


#include "Lm.h"
#pragma comment(lib, "Netapi32.lib")

//RunProcessByGUISessionId接口内部需要用到Wtsapi32.lib和Userenv.lib
#include <UserEnv.h>
#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")

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


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
//获取当前活动的session ID
DWORD CCommonFun::WTSGetActiveConsoleSessionIdEx()
{
	HMODULE hModule = ::LoadLibrary(_T("Kernel32.dll"));
	typedef DWORD (WINAPI* PFNWTSGetActiveConsoleSessionId)(VOID);
	PFNWTSGetActiveConsoleSessionId m_fnWTSGetActiveConsoleSessionId = NULL;
	if(hModule)
	{
		m_fnWTSGetActiveConsoleSessionId = 
			(PFNWTSGetActiveConsoleSessionId)::GetProcAddress(hModule,"WTSGetActiveConsoleSessionId");
		::FreeLibrary(hModule);
	}
	if (m_fnWTSGetActiveConsoleSessionId)
		return m_fnWTSGetActiveConsoleSessionId();
	return 0;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

BOOL CCommonFun::IsWow64()
{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")),"IsWow64Process");

	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		//AfxMessageBox(_T("aa"));
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
			//AfxMessageBox(_T("errr"));
		}
	}
	return bIsWow64;
}

//获取进程的session ID
DWORD CCommonFun::GetProcessSessionID(DWORD dwPID)
{
	DWORD dSessionID(0);
	HANDLE Kenelhandle=::LoadLibrary(_T("kernel32.dll"));
	if(Kenelhandle)
	{		
		BOOL (WINAPI *HandleProcessIdToSessionId)(DWORD,DWORD *);		
		HandleProcessIdToSessionId =(BOOL(WINAPI *)(DWORD,DWORD *))GetProcAddress((HMODULE)Kenelhandle,"ProcessIdToSessionId");
		if(HandleProcessIdToSessionId)
		{
			HandleProcessIdToSessionId(dwPID,&dSessionID);
		}
		FreeLibrary((HMODULE)Kenelhandle);
		Kenelhandle =NULL;
	}
	return dSessionID;
}

BOOL CCommonFun::IsProcAlive(DWORD dwProID)	//add by zfq,判断某个进程是否存在
{
	BOOL BRet = FALSE;
	HANDLE hProc = NULL;
	try
	{
		hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProID);
	}
	catch (...)
	{
		ASSERT(0);
		CCommonFun::WriteFileLog(_T("CCommonFun::IsProcAlive(), dwProID=%u, OpenProcess() catch"), dwProID);
		return FALSE;
	}

	if(!hProc)
	{
		return FALSE;
	}

	DWORD dwExitCode = 0;
	try
	{
		BRet = ::GetExitCodeProcess(hProc, &dwExitCode);
	}
	catch (...)
	{
		ASSERT(0);
		CCommonFun::WriteFileLog(_T("CCommonFun::IsProcAlive(), dwProID=%u, GetExitCodeProcess() catch"), dwProID);
		BRet = FALSE;
	}

	if(BRet)
	{
		BRet = (STILL_ACTIVE == dwExitCode);
	}

	::CloseHandle(hProc);
	return BRet;
}
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
			CCommonFun::WriteFileLog("del file %s",sPath);
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
	if(!PathFileExists(szPath))
	{
		ASSERT(0);
		CCommonFun::WriteFileLog("DeleteFileOrDir %s not exist",szPath);
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
	//CCommonFun::WriteFileLog("CreateShellLink [%s][%s][%s][%d][%s]", filePath, lnkPath, iconPath?iconPath:"", iconIndex, params?params:"");

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

	MultiByteToWideChar(CP_ACP, 0, lnkPath, -1, wsz, MAX_PATH);
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
	//CCommonFun::WriteFileLog("CreateShellLink [%s][%s][%s][%d]", url, lnkPath, iconPath, iconIndex);

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

	MultiByteToWideChar(CP_ACP, 0, lnkPath, -1, wsz, MAX_PATH);
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

		MultiByteToWideChar(CP_ACP, 0, iconPath, -1, wsz, MAX_PATH);
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
	bi.lpszTitle= LoadS(IDS_STRING_Choose_Folder); 
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
	CloseServiceHandle(SchSCManager);	//add by zfq,2013.06.08
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

BOOL CCommonFun::CreateProcessAndWait(LPSTR cmd)
{
	STARTUPINFO si={0};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
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
	MultiByteToWideChar(CP_ACP,0,sDirPath,-1,pDirPath,sDirPath.GetLength());
	p.shi2_path = pDirPath;   
	p.shi2_passwd = NULL; 

	// Call the NetShareAdd function,
	res=NetShareAdd(NULL,2,(LPBYTE)&p,&parm_err);
	if(res == NERR_Success)
		return TRUE;
	else
	{
		CCommonFun::WriteFileLog("!!CCommonFun::AddNetShare,fail,res=%d,sShareName=%s,sDirPath=%s"
			, res, sShareName, sDirPath);
		return FALSE;
	}
}

// 判断共享目录是否存在
BOOL CCommonFun::CheckNetShare(CString sDirPath)
{
	NET_API_STATUS res;
	DWORD devType = 0;
	WCHAR pBuf[MAX_PATH];
	memset(pBuf,0,MAX_PATH);
	MultiByteToWideChar(CP_ACP,0,sDirPath,-1,pBuf,sDirPath.GetLength());
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
	MultiByteToWideChar(CP_ACP,0,sShareName,-1,pBuf,sShareName.GetLength());
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

#include "Lm.h"
#pragma comment(lib, "Netapi32.lib")

//添加用户
BOOL CCommonFun::AddUser(CString szUserName, CString szPassword, CString szComment /*= ""*/)
{
	WCHAR pwcUserName[MAX_PATH] = {0};
	WCHAR pwcPassword[MAX_PATH] = {0};
	WCHAR pwcComment[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,szUserName.GetString(),-1,pwcUserName,szUserName.GetLength());
	MultiByteToWideChar(CP_ACP,0,szPassword.GetString(),-1,pwcPassword,szPassword.GetLength());
	MultiByteToWideChar(CP_ACP,0,szComment.GetString(),-1,pwcComment,szComment.GetLength());

	USER_INFO_1 ui;
	ui.usri1_name = pwcUserName;
	ui.usri1_password = pwcPassword;
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL; 
	ui.usri1_comment = pwcComment; 
	ui.usri1_flags = UF_SCRIPT|UF_DONT_EXPIRE_PASSWD;	//默认密码永不过期
	ui.usri1_script_path = NULL;

	//设置权限
	// 	ModifyPrivilege(SE_MACHINE_ACCOUNT_NAME,TRUE);

	DWORD dwError = 0;
	NET_API_STATUS status = NetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);
	if (status != NERR_Success /*&& status != NERR_UserExists*/)
	{
		CCommonFun::WriteFileLog("AddUser fail,status=%u,dwError=%u\n", status, dwError);
		return FALSE;
	}

	return TRUE;
}

//删除用户
BOOL CCommonFun::DeleteUser(CString szUserName)
{
	WCHAR pwcUserName[MAX_PATH] = {0};
	WCHAR pwcPassword[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,szUserName.GetString(),-1,pwcUserName,szUserName.GetLength());

	//设置权限
	// 	ModifyPrivilege(SE_MACHINE_ACCOUNT_NAME,TRUE);

	NET_API_STATUS status = NetUserDel(NULL, pwcUserName);
	if (status != NERR_Success /*&& status != NERR_UserNotFound*/)
	{
		printf("DeleteUser fail,status=%u\n", status);
		return FALSE;
	}

	return TRUE;
}

//添加用户到组中
BOOL CCommonFun::AddUserToGroup(CString szUserName, CString szGroupName)
{
	WCHAR pwcUserName[MAX_PATH] = {0};
	WCHAR pwcGroupName[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,szUserName.GetString(),-1,pwcUserName,szUserName.GetLength());
	MultiByteToWideChar(CP_ACP,0,szGroupName.GetString(),-1,pwcGroupName,szGroupName.GetLength());

	LOCALGROUP_MEMBERS_INFO_3 account; 
	account.lgrmi3_domainandname = pwcUserName;

	//设置权限
	// 	ModifyPrivilege(SE_MACHINE_ACCOUNT_NAME,TRUE);

	NET_API_STATUS status = NetLocalGroupAddMembers(NULL,pwcGroupName,3,(LPBYTE)&account,1);
	if (status != NERR_Success /*&& status!=ERROR_MEMBER_IN_ALIAS*/)
	{
		printf("AddUserToGroup fail,status=%u\n", status);
		return FALSE;
	}

	return TRUE;
}

//从组中删除用户
BOOL CCommonFun::DeleteUserFromGroup(CString szUserName, CString szGroupName)
{
	WCHAR pwcUserName[MAX_PATH] = {0};
	WCHAR pwcGroupName[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,szUserName.GetString(),-1,pwcUserName,szUserName.GetLength());
	MultiByteToWideChar(CP_ACP,0,szGroupName.GetString(),-1,pwcGroupName,szGroupName.GetLength());

	LOCALGROUP_MEMBERS_INFO_3 account; 
	account.lgrmi3_domainandname = pwcUserName;

	//设置权限
	// 	ModifyPrivilege(SE_MACHINE_ACCOUNT_NAME,TRUE);

	NET_API_STATUS status = NetLocalGroupDelMembers(NULL,pwcGroupName,3,(LPBYTE)&account,1);
	if (status != NERR_Success /*&& status!=ERROR_MEMBER_NOT_IN_ALIAS*/)
	{
		printf("DeleteUserFromGroup fail,status=%u\n", status);
		return FALSE;
	}

	return TRUE;
}

BOOL CCommonFun::RunProcess(CString sPath,CString sParam,DWORD dwWaitMilliseconds /* = INFINITE*/,WORD dwShowCmd /*= SW_SHOW*/)
{
	BOOL bOk = FALSE;
	int nLen = sPath.GetLength() + sParam.GetLength() + 10;
	char* pszPath = new char[nLen];
	ASSERT(pszPath);
	memset(pszPath,0,nLen);

	sprintf(pszPath,"\"%s\" %s",sPath,sParam);
	STARTUPINFOA si = { sizeof si, 0, "" };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = dwShowCmd;

	WriteFileLog(_T("Install pathandParam = %s"),pszPath);
	PROCESS_INFORMATION pi;
	if(CreateProcessA(0, 
		pszPath,// command line
		0, 0,  // security attributes
		FALSE, // handle inheritance
		0,     // creation flags
		0,     // environment block
		0,     // current directory
		&si, &pi ))
	{
		WriteFileLog(_T("createprocess pid = %d"),pi.dwProcessId);
		//	if(bWait)
		{
			::WaitForSingleObject(pi.hProcess,dwWaitMilliseconds);
		}
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
		bOk = TRUE;
	}
	else
	{
		CString s;
		s.Format(_T("Install 失败 err = %d \n"),::GetLastError());
		WriteFileLog(s);
	}
	if (pszPath)
	{
		delete pszPath;
		pszPath = NULL;
	}
	return bOk;
}

//RunProcessByGUISessionId是提供给服务进程调用，其它进程可能失败。
BOOL CCommonFun::RunProcessByGUISessionId(CString sPath, CString sParam, DWORD dwSessionId /*= 0xFFFFFFFF*/, DWORD dwWaitMilliseconds /*= INFINITE*/,WORD dwShowCmd /*= SW_SHOW*/)
{
	WriteFileLog("CCommonFun::RunProcessByGUISessionId,1,sPath=%s,sParam=%s,dwSessionId=%u,dwWaitMilliseconds=%u,dwShowCmd=%d"
		, sPath, sParam, dwSessionId, dwWaitMilliseconds, dwShowCmd);
	if (dwSessionId == 0xFFFFFFFF)
	{
		//获取当前处于活动状态SessionId
		dwSessionId = WTSGetActiveConsoleSessionId();
		if (dwSessionId == 0xFFFFFFFF)
		{
			WriteFileLog("!!CCommonFun::RunProcessByGUISessionId,2,WTSGetActiveConsoleSessionId fail,error=%u", GetLastError());
			return FALSE;
		}
	}

	HANDLE hToken = NULL;
	HANDLE hTokenDup = NULL;
	LPVOID pEnv = NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//获取当前处于活动状态用户的Token
	//To call this function successfully, the calling application must be running within 
	//the context of the LocalSystem account and have the SE_TCB_NAME privilege. 
	if (!WTSQueryUserToken(dwSessionId, &hToken))
	{
		WriteFileLog("!!CCommonFun::RunProcessByGUISessionId,3,WTSQueryUserToken fail,dwSessionId=%u,err=%d", dwSessionId, GetLastError());
		CloseHandle(hToken);
		return FALSE;
	}

	//复制新的Token
	if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hTokenDup))
	{
		WriteFileLog("!!CCommonFun::RunProcessByGUISessionId,4,DuplicateTokenEx fail,err=%d", GetLastError());
		CloseHandle(hToken);
		return FALSE;
	}

	//创建环境信息
	if (!CreateEnvironmentBlock(&pEnv, hTokenDup, FALSE))
	{
		WriteFileLog("!!CCommonFun::RunProcessByGUISessionId,5,CreateEnvironmentBlock fail,err=%d", GetLastError());
		CloseHandle(hTokenDup);
		CloseHandle(hToken);
		return FALSE;
	}

	//设置启动参数
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = _TEXT("winsta0\\default");
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = dwShowCmd;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	//开始创建进程
	CString szCommandLine;
	szCommandLine.Format("\"%s\" %s", sPath, sParam);
	DWORD dwCreateFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;
	if (!CreateProcessAsUser(hTokenDup, NULL, szCommandLine.GetBuffer(), NULL, NULL, FALSE, dwCreateFlag, pEnv, NULL, &si, &pi))
	{
		WriteFileLog("!!CCommonFun::RunProcessByGUISessionId,6,CreateProcessAsUser fail,err=%d", GetLastError());
		DestroyEnvironmentBlock(pEnv);
		CloseHandle(hTokenDup);
		CloseHandle(hToken);
		return FALSE;
	}

	//附加操作,回收资源

	WriteFileLog("CCommonFun::RunProcessByGUISessionId,7,CreateProcessAsUser succ,dwWaitMilliseconds=%u", dwWaitMilliseconds);

	//等待启动的进程结束
	WaitForSingleObject(pi.hProcess, dwWaitMilliseconds);

	WriteFileLog("CCommonFun::RunProcessByGUISessionId,8,end");

	DestroyEnvironmentBlock(pEnv);
	CloseHandle(hTokenDup);
	CloseHandle(hToken);

	return TRUE;
}

//获取指定文件所在的目录,"C:\test\test.doc" => ""C:\test""
CString CCommonFun::GetFileDirByFullPath(CString szFullPath)
{
	szFullPath.Replace("/", "\\");
	szFullPath.TrimRight("\\");
	int nPos = szFullPath.ReverseFind('\\');
	return szFullPath.Left(nPos);
}

//获取指定文件名,"C:\test\test.doc" => "test.doc"
CString CCommonFun::GetFileNameByFullPath(CString szFullPath)
{
	szFullPath.Replace("/", "\\");
	szFullPath.TrimRight("\\");
	int nPos = szFullPath.ReverseFind('\\');
	return szFullPath.Right(szFullPath.GetLength() - nPos - 1);
}

BOOL CCommonFun::RegisterSessionNotification(HWND hWnd)
{
	ASSERT(hWnd && ::IsWindow(hWnd));
	HMODULE hModule = ::LoadLibrary(_T("Wtsapi32"));
	if(!hModule)
		return FALSE;
	typedef BOOL(WINAPI* PFNWTSRegisterSessionNotification)(HWND,DWORD);
	BOOL bRet = FALSE;
	PFNWTSRegisterSessionNotification pfnWTSRegisterSessionNotification =
		(PFNWTSRegisterSessionNotification)::GetProcAddress(hModule,"WTSRegisterSessionNotification");
	if(pfnWTSRegisterSessionNotification)
	{
		bRet = pfnWTSRegisterSessionNotification(hWnd,NOTIFY_FOR_ALL_SESSIONS);//NOTIFY_FOR_THIS_SESSION
	}
	if(hModule)
		::FreeLibrary(hModule);
	return bRet;
}

void CCommonFun::UnRegisterSessionNotification(HWND hWnd)
{
	ASSERT(hWnd && ::IsWindow(hWnd));
	HMODULE hModule = ::LoadLibrary(_T("Wtsapi32"));
	if(!hModule)
		return ;

	typedef BOOL(WINAPI* PFNWTSUnRegisterSessionNotification)(HWND);
	PFNWTSUnRegisterSessionNotification pfnWTSUnRegisterSessionNotification =
		(PFNWTSUnRegisterSessionNotification)::GetProcAddress(hModule,"WTSUnRegisterSessionNotification");
	if(pfnWTSUnRegisterSessionNotification)
	{
		pfnWTSUnRegisterSessionNotification(hWnd);
	}
	if(hModule)
		::FreeLibrary(hModule);
}

//获取本地路径的一个网络路径，方便其它计算机能够正常访问
BOOL CCommonFun::GetSharePath(CString szLocalPath, CString szShareName, CString& szShareDir, CString& szSharePath)
{
	BOOL bRet = TRUE;
	CString szLocalDir = GetFileDirByFullPath(szLocalPath);
	CString szLocalFile = GetFileNameByFullPath(szLocalPath);
	CCommonFun::WriteFileLog("CCommonFun::GetSharePath,szLocalPath=%s,szShareName=%s,szLocalDir=%s,szLocalFile=%s"
		, szLocalPath, szShareName, szLocalDir, szLocalFile);

	//设置权限
	CCommonFun::AddRights(szLocalDir.GetBuffer(), "Everyone", GENERIC_ALL|STANDARD_RIGHTS_ALL);
	if (PathFileExists(szLocalPath))
	{
		CCommonFun::AddRights(szLocalPath.GetBuffer(), "Everyone", GENERIC_ALL|STANDARD_RIGHTS_ALL);
	}

	//删除共享目录
	if (CheckNetShare(szLocalDir))
	{
		DelNetShare(szShareName);
	}

	//创建共享目录
	bRet = AddNetShare(szShareName, "", szLocalDir);
	if (bRet)
	{
		szShareDir.Format("\\\\%s\\%s", CCommonFun::GetLocalIP(), szShareName);
		szSharePath.Format("\\\\%s\\%s\\%s", CCommonFun::GetLocalIP(), szShareName, szLocalFile);
	}

	return bRet;
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
