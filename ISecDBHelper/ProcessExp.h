#pragma once

#include <Winternl.h>
#include <tlhelp32.h>
#include <vector>

struct _ENVSTRING_t
{
	LPVOID pData;
	int nSize;

	_ENVSTRING_t() // Ctor of structure
	{
		pData = 0; nSize = 0;
	}

	~_ENVSTRING_t() // Ctor of structure
	{ Clear(); }
	void Clear() // dtor of structure
	{
		if( pData )
		{
			delete[] pData;
			pData = 0;
		}
		nSize = 0;
	}
};

typedef std::pair<CString,CString> EnvVariableValuePair;
typedef std::vector<EnvVariableValuePair> EnvVarValArray;

class CProcessExp
{
public:
	CProcessExp(void);
	~CProcessExp(void);
public:
	static BOOL		ReadEnvironmentBlock( CString strProcess, DWORD dwSession, _ENVSTRING_t& stEnvData );
	static DWORD	GetProcessIdByName(CString strExeFullPath/*Full Path*/, DWORD dwSession);//参数为全路径
	static CString	GetProcessFullPath(HANDLE hProcess);
	static BOOL		ProcessExist(CString strProcessName, DWORD dwSession);//检查进程是否存在,不包括目录
	static BOOL		GetProcessUser(CString strProcessName, DWORD dwSession, CString& strUser);
	static BOOL		GetProcessUser(DWORD dwSession, CString& strUser);
	static BOOL     GetProcessToken(CString strProcessName, DWORD dwActiveSessionId, HANDLE& hProcessToken);

	static BOOL		CreateProcessByCurUser(CString sPath,CString sParam,DWORD dwActiveSessionId = 0);
	static BOOL     GetProcessToken(DWORD dwProcessID, HANDLE& hProcess);
	//将动态链接库远程注入到进程中
	static BOOL		InjectDll(DWORD dwTargetPID, char* szDllPath);
	//从远程进程中卸载dll
	static BOOL     UnloadDll(DWORD dwTargetPID, char* szDllPath);

	static LPVOID	WriteRemoteMemory(HANDLE hProcess, LPVOID pData, UINT nSize);
	static PTHREAD_START_ROUTINE GetStartRoutine(char* szProc, char* szLibrary = "Kernel32.dll");
private:
	static NTSTATUS QueryInformationProcesss( IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL );
	static HANDLE	OpenProcessToRead( DWORD dwPID );
	static BOOL		ReadEnvironmentBlock( HANDLE hProcess, _ENVSTRING_t& stEnvData );
	static BOOL		HasReadAccess( HANDLE hProcess, void* pAddress, int& nSize );
};
