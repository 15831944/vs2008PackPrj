// XSharedMemoryQueue.h: interface for the XSharedMemoryQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSHAREDMEMORYQUEUE_H__AB2703F8_7B45_42FA_8870_7FEDD19E4A50__INCLUDED_)
#define AFX_XSHAREDMEMORYQUEUE_H__AB2703F8_7B45_42FA_8870_7FEDD19E4A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SME_SHAREMEMORY_CMD_NAME			"SME_SHAREMEMORY_CMD_NAME"
#define SME_SHAREMEMORY_SEMAPHORE_CMD_NAME	"SME_SHAREMEMORY_SEMAPHORE_CMD_NAME"
#define SME_SENDCMD_INTERVAL				10
#define SME_SENDCMD_TIMEOUT					(8*1000/SME_SENDCMD_INTERVAL)

class XSharedMemoryQueue;

struct _AA_DLL_EXPORT_ XSemaphore
{
public:
	XSemaphore(const char *name, bool create_new);
	~XSemaphore();
	int P();
	int V();
	
	HANDLE sem_;
	
	bool owned;
	const char* name_;
	
private:
	XSemaphore(const XSemaphore&);
	XSemaphore& operator=(const XSemaphore&);
};

void _AA_DLL_EXPORT_ SleepInMilliseconds(int msec);

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define SME_SHAREMEMORY_SEMAPHORE_NUM 1

struct IPCEventInfo 
{
	int SourceProcessID;
	int DestProcessID;
	int IPCEventStatus;
	int EventID;
	unsigned long ulParameter1;
	unsigned long ulParameter2;
};

typedef int (*SME_IPC_EVENT_CALLBACK_T)(void* pInEventInfo);

//////////////////////////////////////////////////////////////////////////////////////////////////////

class _AA_DLL_EXPORT_ XExtIPCEvent 
{
public:
	XExtIPCEvent();
	~XExtIPCEvent();

	enum {IPC_EVENT_BUFFER_SIZE = 1024};
	enum {IPC_QUEUE_NUMBER = 4};
	enum {IPC_QUEUE_RESOURCE = 0, IPC_QUEUE_SENDING = 1, IPC_QUEUE_POSTING = 3};
	
	//Four queues: empty, send queue, wait for sending, and post
	
	enum TmCommandStatus
	{
		EVENT_STATUS_UNUSED = 0, EVENT_STATUS_SENDING, EVENT_STATUS_POSTING, EVENT_STATUS_CONSUMED
	};
	
	
	
	//static XExtIPCEvent* GetInstance ();
	
	int Create(int ProcessID, CStringA sMapName, CStringA sEventName, bool bIsMasterProcess = false);
	
	int Create(int ProcessID, CStringA strMapName, CStringA strEventName, int nMaxDataSize, bool bIsMasterProcess); 
	
	void Release (bool bIsMasterProcess = false);
	
	int SendIPCEvent(IPCEventInfo* pInEventInfo, IPCEventInfo* pOutEventInfo, int nTimeout = -1);
	
	int PostIPCEvent(IPCEventInfo* pInEventInfo);
	int PostIPCEvent(IPCEventInfo* pInEventInfo, PBYTE pData, int nSize);

	int QueryIPCEvent(IPCEventInfo* pOutEventInfo, SME_IPC_EVENT_CALLBACK_T pfnCallbak);
	int QueryIPCEvent(OUT IPCEventInfo* pOutEventInfo, OUT PBYTE pdata, IN int nSize, SME_IPC_EVENT_CALLBACK_T pfnCallbak);
	
	void RecycleEvent(int idProcess);
	bool DumpInfo ();

// 	void Shutdown();
// 	BOOL MasterActive(BOOL bEnable,SME_IPC_EVENT_CALLBACK_T pCall = NULL);
// 	static DWORD WINAPI IPCMonThread(VOID* pVoid);
	BOOL Exist();
private:
	
	int Lock ();
	int UnLock();
	
private: 
	XSharedMemoryQueue* m_pSMQueue;
	XSemaphore* m_pSem;
	int m_idProcess;
	int m_nMaxDataSize;
// 	HANDLE	m_hThread;
// 	SME_IPC_EVENT_CALLBACK_T m_pfDefaultProc;
};


class _AA_DLL_EXPORT_ XSharedMemoryQueue  
{
public:
	XSharedMemoryQueue();
	virtual ~XSharedMemoryQueue();
	
	bool Initialize (const char* strMapName, int iNodeSize, int iNodeCount, int iQueueCount);
	void Release ();

	void* CreateSharedMemoryQueue ();
	void DeleteSharedMemoryQueue ();

	void* OpenSharedMemoryQueue ();
	void CloseSharedMemoryQueue ();
	
	void* GetAt(int iIndex);
	int GetIndex(void* pNode);
	
	void* GetQueueHead(int iQueue);
	void* GetQueueTail(int iQueue);
	void* GetNext(void* pNode);
	void* GetPrev(void* pNode);
	
	bool MoveToDestQueueTail(void* pNode, int iDestQueue);
	int LocateQueue(void* pNode);
	
	
	void DumpQueue (int iQueue, bool bDumpNode);
	void DumpNode (void* pNode);
	
private: 
	char* m_strMapName;
	int m_iNodeSize; // the size of internal node which contains additional pointers besides user data.
	int m_iNodeCount; // the node count 
	int m_iQueueCount; // the number of queues
	int* m_pQueueArray; // the queue array 
	HANDLE m_hMapMem;
    void* m_pMappedPointer;
};

#endif // !defined(AFX_XSHAREDMEMORYQUEUE_H__AB2703F8_7B45_42FA_8870_7FEDD19E4A50__INCLUDED_)
