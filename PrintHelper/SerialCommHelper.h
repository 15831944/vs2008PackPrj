// SerialCommHelper.h: interface for the CSerialCommHelper class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CopyInterface.h"
#include <map>

typedef enum tagSERIAL_STATE
{
	SS_Unknown,
	SS_UnInit,
	SS_Init,
	SS_Started ,
	SS_Stopped ,
	
} SERIAL_STATE;

class Owner
{
public:
	
	explicit Owner(
		CRITICAL_SECTION &crit): m_csLock(crit)
	{
		EnterCriticalSection ( &m_csLock );
	}
	
	~Owner()
	{
		LeaveCriticalSection (&m_csLock);
	}
	
private :
	
	CRITICAL_SECTION &m_csLock;
	
	// No copies do not implement
	Owner(const Owner &rhs);
	Owner &operator=(const Owner &rhs);
};
#define SERIAL_CMD_BUF_SIZE 4
class CSerialCommHelper  
{

public:
	CSerialCommHelper();
	virtual ~CSerialCommHelper();

	//初始化com接口
	BOOL Init2(int nCom,ICopyCmdInterface* pCall);
	//
	void UnInit();
	//板子初始化
	BOOL Logon(BYTE* pBuf = 0,int nLen = 0);
	//检测状态
	BOOL CheckState(BYTE* pBuf = 0,int nLen = 0);
	//解锁或者锁定
	BOOL Lock(BYTE* pBuf = 0,int nLen = 0);
	BOOL UnLock(BYTE* pBuf = 0,int nLen = 0);
	void OnRead();
	static unsigned __stdcall ThreadFn(void*pvParam);

	BOOL Write(BYTE* pCmd,int nLen,DWORD dwTimeOut = 2000);

	//设置品牌
	void InitBrand(BRAND_TYPE eType);
protected:
	inline void		LockThis()			{EnterCriticalSection ( &m_csLock );}	
	inline void		UnLockThis()		{LeaveCriticalSection (&m_csLock); }
	inline void		InitLock()			{InitializeCriticalSection (&m_csLock );}
	inline void		DelLock()				{DeleteCriticalSection (&m_csLock );}

	HRESULT			Init(std::string szPortName= _T("COM1"), DWORD dwBaudRate = 9600,BYTE byParity = 78,BYTE byStopBits = 1,BYTE byByteSize  = 8);

	void OnCommand(BYTE* szCmd,int nSize);
	void OnCommand2(BYTE* szCmd,int nSize);
	void OnByte(BYTE c);
	
private:
	HANDLE	m_hCommPort;
	HANDLE	m_hThreadTerm ;
	HANDLE	m_hThread;
	HANDLE	m_hThreadStarted;

	HANDLE m_hComEvent;
	ICopyCmdInterface* m_pCallback;
	CRITICAL_SECTION m_csLock;
	BYTE  m_szCmd[SERIAL_CMD_BUF_SIZE];
	long  m_lCmdPos;
	BRAND_TYPE m_eType;
};

