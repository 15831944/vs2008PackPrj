#pragma once

struct _EXCEPTION_POINTERS;

class _AA_DLL_EXPORT_ CMiniDumper
{
public:
	static void Enable(LPCTSTR pszAppName, bool bShowErrors);
	static void DumpNow(LPCTSTR szDumpPath);

private:
	static TCHAR m_szAppName[MAX_PATH];

	static HMODULE GetDebugHelperDll(FARPROC* ppfnMiniDumpWriteDump, bool bShowErrors);
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo);
};

// extern CMiniDumper theCrashDumper;
