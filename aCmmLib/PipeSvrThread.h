#pragma once

// CPipeSvrThread
// 
#include "../Public/Include/CmmInterface.h"
#pragma pack(push, 1)
#if 0
typedef struct _IST_PIPE_STRUCT
{
	UINT size;
	UINT message;
	int  para1;
	int  para2;
	BYTE pdata[1];
}IST_PIPE_STRUCT, *PIST_PIPE_STRUCT;
#endif
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
/*
typedef struct tag_Net_Pack_Head
{
	UINT		nPackBodySize;		//包的大小，不包括包头
	UINT		nMainCmd;			//主命令
	UINT		nSubCmd;			//子命令
	WORD		nIndentify;			//包的标识
	WORD		nPackMask;			//分大包，小包，是否为	压缩包，用掩码区分
	UINT		nReserve;			//tcp时，保留字段，上层不允许使用此字段，当为压缩包时，此字段标识未压缩前包的大小
	//组播包接收时，标识组播数据源的ip或学生id
}NET_PACK_HEAD,*PNET_PACK_HEAD;
*/
#pragma pack(pop)

typedef struct 
{ 
	OVERLAPPED oOverlap; 
	HANDLE hPipeInst; 
	BYTE* pRequestBuf; 
	DWORD cbRead;
	DWORD cbRequestBufSize;
	BYTE* pReplyBuf; 
	DWORD cbToWrite; 
	DWORD cbReplyBufSize;
	IPipeCallback* pCallback;
} PIPEINST, *LPPIPEINST; 


class CAsyncPipeClt;
class CAsyncPipeCltList;

class _AA_DLL_EXPORT_ CAsyncPipeClt : public PIPEINST
{
public:
	CAsyncPipeClt();
	CAsyncPipeClt(CAsyncPipeCltList* p);
	~CAsyncPipeClt();
public:
	void Release();
	void Close();
	void Init(int nBufDefSize,IPipeCallback* pCall);
	BOOL Write(int nCmd,int nSub,BYTE* pData = 0,int nSize = 0);
	BOOL Write(BYTE* pData,int nSize);
	int OnWriteCmpt();
	int OnReadCmpt(IST_PIPE_STRUCT* pInfo);
	int OnReadCmpt(BYTE* pData,int nSize);
	BOOL PostRead();
	BOOL GrowRequestBuf();
private:
	CCriticalSection2 m_cs;
	CAsyncPipeCltList* m_pOwner;
};

class _AA_DLL_EXPORT_ CAsyncPipeCltList :public CList<CAsyncPipeClt*,CAsyncPipeClt*>
{
public:
	CAsyncPipeCltList();
	~CAsyncPipeCltList();

	CCriticalSection2 m_cs;
	IPipeCallback* m_pCallback;

	CAsyncPipeClt* Alloc();
	BOOL Release(CAsyncPipeClt* p);
	BOOL ReleaseAll();
	BOOL Check(CAsyncPipeClt* p);
};

class _AA_DLL_EXPORT_ CPipeClt
{
public:

	CPipeClt();
	CPipeClt(IPipeCallback* pCall);
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
	IPipeCallback* m_pCallBack;
	CCriticalSection2 m_cs;
};

class _AA_DLL_EXPORT_ CPipeSvrThread : public CWinThread
{
	DECLARE_DYNCREATE(CPipeSvrThread)

protected:
	CPipeSvrThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CPipeSvrThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	static CPipeSvrThread* Create(CString sName,IPipeCallback* pCall = NULL);
	static BOOL End(CPipeSvrThread*& pThread);
	BOOL CheckPipeInstance(CAsyncPipeClt* p);
protected:
	DECLARE_MESSAGE_MAP()

	HANDLE m_hPipe;
public:
	CString m_sPipeName;
	CAsyncPipeCltList m_oPipeList;

	IPipeCallback* m_pCallback;
	BOOL CreateAndConnectInstance(LPOVERLAPPED lpoOverlap); 
	BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED); 

	void OnNewPipe(void* pVoid);
	
//  	CAsyncPipeClt* GetPipeById(DWORD dwId)
//  	{
// 		
//  	}
};

