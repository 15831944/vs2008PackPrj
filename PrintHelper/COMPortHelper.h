// COMPortHelper.h : interface for the CCOMPortHelper class.
// ICOMPortInterface可以作业常规COM串口操作的基础类，
// 具体的功能类如CInfraredInduction，可以包含一个ICOMPortInterface的
// 成员变量，用于写COM串口和接收数据回调
//////////////////////////////////////////////////////////////////////

#pragma once

class _AA_DLL_EXPORT_ ICOMPortInterface
{
public:
	virtual void OnByte(BYTE byte) = NULL;
	virtual void OnErr(DWORD dwError) = NULL;
};

class _AA_DLL_EXPORT_ CCOMPortHelper
{
public:
	CCOMPortHelper();
	virtual ~CCOMPortHelper();

	BOOL Open(int nComIndex);	//打开Com接口
	void Close();				//关闭Com接口
	void SetCallBack(ICOMPortInterface* pCall);
	void OnRead();
	BOOL Write(BYTE* pData, int nDataSize, DWORD dwTimeOut = 2000);

	static DWORD WINAPI ThreadFn(LPVOID lpParameter);

protected:
	void OnByte(BYTE byte);
	void OnErr(DWORD dwError);

private:
	DWORD m_dwLastError;
	HANDLE	m_hComPort;
	HANDLE m_hComEvent;
	HANDLE	m_hThreadStarted;
	HANDLE	m_hThreadTerm ;
	HANDLE	m_hThread;
	DWORD	m_dwThreadID;
	ICOMPortInterface* m_pCallback;
	CCriticalSection2 m_cs;
};

