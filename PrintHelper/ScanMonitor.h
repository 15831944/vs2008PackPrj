// ScanMonitor.h: interface for the CScanMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANMONITOR_H__9B2360E0_7B0D_479A_9792_E1BF869B5C6B__INCLUDED_)
#define AFX_SCANMONITOR_H__9B2360E0_7B0D_479A_9792_E1BF869B5C6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IScanChargeInterface
{
public:
	virtual void OnOneScanCmplt(void*){ASSERT(0);}
	virtual void OnScanErr(int ){ASSERT(0);}
};

class _AA_DLL_EXPORT_ CScanInfo
{
public:
	CScanInfo()
	{
		m_nSize = 0;
		m_nColor = 1;
		m_nUnit = 1024;
		m_nPageCount = 0;
		m_nPrice = 0;
		m_nShouldMoney = 0;
		m_nPayMoney = 0;
		m_nJobid = 0;
		m_bIsLastFile = FALSE;
	}
	CString m_sPath;
	
	CString m_sZipFilePath;
	CString m_sFileName;
	CString m_sDir;

	int m_nSize;
	int m_nColor;
	int m_nPageCount;
	double m_dt;
	int m_nPrice;
	int m_nShouldMoney;
	int m_nPayMoney;
	int m_nJobid;
	int m_nUnit;
	BOOL m_bIsLastFile;

	BOOL Attach(CString& sPath);
protected:
	BOOL CompressTif();
};

class _AA_DLL_EXPORT_ CScanMonitor  
{
public:
	void Stop();
	BOOL Start(CString sDir);
	void SetCallback(IScanChargeInterface*);
	CScanMonitor();
	virtual ~CScanMonitor();
	BOOL IsTiff(CString& sPath);
	void SetScanData(CString& s);
	void Shutdown();
	BOOL IsActive();
private:
	HANDLE m_hShutdown;
	HANDLE	m_hThread;
	HANDLE	m_hThreadStarted;
	long	m_lActive;
	IScanChargeInterface* m_pCallback;
	CStringArray m_aryData;

	CString m_sDir;
	void ReadChanges2(CString sDir);
// 	BOOL IsActive();
	void DumpFNIBuffer(FILE_NOTIFY_INFORMATION* pInfo);
	void WriteToMemFile(FILE_NOTIFY_INFORMATION* pInfo);
	
	static DWORD WINAPI ThreadFn(void* pVoid);
};

#endif // !defined(AFX_SCANMONITOR_H__9B2360E0_7B0D_479A_9792_E1BF869B5C6B__INCLUDED_)
