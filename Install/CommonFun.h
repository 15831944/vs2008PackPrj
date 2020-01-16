// CommonFun.h: interface for the CCommonFun class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


typedef enum tagPLATFORM
{
	WIN_9X,WIN_NT,WIN_2K,WIN_XP,WIN_2003,UNKNOWN
} PLATFORM;

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#include <Winspool.h>
class  CCommonFun  
{
public:
	static int CmpDate(CTime& tm1,CTime& tm2);
	static CString GetVariantStr(_variant_t varValue);
	static BOOL CCommonFun::Is256ColorSupported();
	static BOOL StrToTime(CString &sStr,CTime& tm);
	static CString GetDirFromShell(BOOL bNewStyle, HWND hOwner = NULL);

	static DWORD GetSystemPageSize();
	
	//version
	static BOOL IsWin9X();
	static BOOL IsWinNT();
	static BOOL IsWin2003();
	static PLATFORM GetPlatform();
	static BOOL IsWinVistaOrLater();
#define EX_WIN_LOGOFF	1
#define	EX_WIN_REBOOT	2
#define EX_WIN_SHUTDOWN	3
	static BOOL ExitWin(int nMode);

	static CString GetDefaultPath();
	static CString GetLocalIP(void);
	static BOOL IsLocalIP(CString sIP);
	static int GetLocalIPList(CStringArray& ary);
	static CString GetLoaclHost(void);

	static CString GetLastErrorInfo();
	CString ChangePathToShort(CString sPath);
	
	static BOOL CreateDir(CString sPath);
	static void ClearArrayElement(CObArray *pAry);
	static BOOL SHGetSpecialPath(int nFolder,CString &sPath);
	static int SToN(CString str);
	static CString NToS(int nNum);
	static CString NToS(double dbNum);
	static CString NToS(DWORD dwNum);
	
	static void WriteFileLog(LPCTSTR lpszFormat, ...);
	static void WriteFileLog(CString sLog);
	static void StringSplit(const CString str, CStringArray *psArray , char chDelimiter=',', int nCount=-1);
	static void StringSplit(const CString sSource, CStringArray *parArray , CString sSplitter,BOOL bTriNUll = FALSE);


	static BOOL EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable);
	static BOOL IsAdmin();

	static DWORD StartSvc(CString strSvcName);
	static DWORD StopSvc(CString strSvcName);
	static BOOL StartSMService(CString sServiceName);
	//static BOOL WaitForState(DWORD dwDesiredState, SC_HANDLE svc);
	static BOOL StopSMService(CString sServiceName);

	static void EnumeratePrinters( CStringArray& ary )
	{
		DWORD dwNeeded=0, 
			dwReturned=0;
		EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &dwNeeded, &dwReturned);
		if (dwNeeded <=0)
		{
			return;
		}

		LPBYTE lpBuffer = new BYTE[dwNeeded];
		EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, lpBuffer, dwNeeded, &dwNeeded, &dwReturned);

		PPRINTER_INFO_2 p2 = (PPRINTER_INFO_2) lpBuffer;
		
		for (DWORD x = 0; x < dwReturned; x++)
		{
			ary.Add(p2->pPrinterName);
// 			if (p2->Attributes & PRINTER_ATTRIBUTE_DEFAULT)
// 			{
// 				nDefaultIndex = x;
// 			}
			p2++;
		}

		delete[] lpBuffer;
	}

	//获取所有com口
	static void GetAllCom(CStringArray& ary)
	{
		char szCom[20] = {0};
		for (int n = 1; n < 20;n ++)
		{
			sprintf(szCom,"COM%d",n);
			//open the COM Port
			HANDLE m_hCommPort = ::CreateFile(szCom,
				GENERIC_READ|GENERIC_WRITE,//access ( read and write)
				0,	//(share) 0:cannot share the COM port						
				0,	//security  (None)				
				OPEN_EXISTING,// creation : open_existing
				FILE_FLAG_OVERLAPPED,// we want overlapped operation
				0// no templates file for COM port...
				);
			if ( m_hCommPort == INVALID_HANDLE_VALUE )
			{
				//break;
				continue;	//2013.07.26,modify by zxl,com串口序号不一定是按顺序排列的,可能有中断
			}
			else
			{
				CloseHandle(m_hCommPort);
				ary.Add(szCom);
			}
			
		}
	}

	CCommonFun();
	virtual ~CCommonFun();

	static BOOL TerminaPID(DWORD dw);
	static BOOL TerminalProcess(CString sProc);

	static BOOL ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable);

	static void DeleteDir(CString sDir,BOOL bOnlyDelFile  = FALSE );
	static void ModifyFileAttribute(CString strPath, BYTE bToRemove, BYTE bToAdd);//修改文件或文件夹属性
	static BOOL DeleteFileOrDir(CString szPath);	//add by zfq, 删除一个文件或目录

	static BOOL CreateShellLink(LPCTSTR filePath, LPCTSTR lnkPath, LPCTSTR iconPath=NULL, int iconIndex=0, LPCTSTR params=NULL);
	static BOOL CreateURLLink(LPCTSTR url, LPCTSTR lnkPath, LPCTSTR iconPath=NULL, int iconIndex=0);

	static CString GetProgramDir();
	static CString GetDesktopDir();
	static CString GetCommonDesktopDir();

	static BOOL CreateProcessAndWait(LPSTR cmd);
	static CStringA GetUninstallString(CStringA sDisplayName);
	static BOOL UninstallSoftware(CString sDisplayName);
	
	//add by szy 20150514 copy from aCmmlib
	static BOOL	AddNetShare(CString sShareName,CString sShareDesc,CString sDirPath);// 添加共享目录
    static BOOL CheckNetShare(CString sDirPath);						// 判断共享目录是否存在
    static BOOL DelNetShare(CString sShareName);						// 删除共享目录
    
    //add by szy 20150515
    static BOOL AddRights(TCHAR* lpFileName,TCHAR* lpAccountName,DWORD dw);
    
	//获取日志目录
	static CString& GetLogDir();
};
