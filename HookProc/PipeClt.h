#pragma once

typedef struct _IST_PIPE_STRUCT
{
	union
	{
		//UINT size;
		UINT nPackBodySize;		//包的大小，不包括包头
	};
	union
	{
		UINT message;
		UINT nMainCmd;			//主命令
	};
	union
	{
		UINT para1;
		UINT nSubCmd;			//主命令
	};
	union
	{
		UINT para2;
		WORD nIndentify;			//包的标识
		WORD nPackMask;			//分大包，小包，是否为	压缩包，用掩码区分
	};

	UINT		nReserve;	
}IST_PIPE_STRUCT, *PIST_PIPE_STRUCT;




class CPipeClt
{
public:
	CPipeClt();
	~CPipeClt();

	void Shutdown();
	BOOL Create(CString sname);
	BOOL Open(CString sName);
	BOOL Write(IST_PIPE_STRUCT* phead);
	BOOL Write(int nCmd,int nSub,BYTE* pData = 0,int nSize = 0);
	BOOL Init();
protected:
	BOOL Write(BYTE* pData,int nSize);
	BOOL OnProcess();
	void OnReadCmplt(BYTE* pData,int nSize);
	static DWORD WINAPI ThreadFunc(void* lp);
	BOOL HasData();
private:
	HANDLE m_hPipe; 
	BYTE* m_pRequestBuf;
	int m_nRequestBufSize;
	int m_nRead;
	CString m_sPipename;
	HANDLE m_hShutDown;
	HANDLE m_hEvent;
	HANDLE m_hThread;

	CRITICAL_SECTION m_cs;
};

