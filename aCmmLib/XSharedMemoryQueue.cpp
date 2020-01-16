// XSharedMemoryQueue.cpp: implementation of the XSharedMemoryQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XSharedMemoryQueue.h"
#include "access_control.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
XSemaphore::XSemaphore( 
		const char *name, 
		bool create_new)
: sem_(0), owned(create_new), name_(name)
{

	if(create_new)
	{
		SECURITY_DESCRIPTOR sd;

		InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);

		SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);

		SECURITY_ATTRIBUTES sa = { sizeof(sa), &sd, FALSE };

		sem_ = CreateEventA(&sa, FALSE, TRUE, name);
	}
	else
	{
		sem_ = OpenEventA(EVENT_ALL_ACCESS, FALSE, name);
		if (sem_ == 0)
		{
			//TCHAR szText[64];
			//wsprintf(szText, "sem_ == 0, err = %d", GetLastError());
			//WriteLog(szText);
		}
	}
}

XSemaphore::~XSemaphore()
{
	if((sem_!=INVALID_HANDLE_VALUE) && (sem_!=0))
		CloseHandle(sem_);
}

int XSemaphore::P()
{
	DWORD ret=WaitForSingleObject(sem_, 30*1000);
// 	TCHAR szText[64];
// 	wsprintf(szText, "sem_ == %d", sem_);
// 	WriteLog(szText);
	switch(ret)
	{
	case WAIT_OBJECT_0:
		return 0;
	default:
		//dirty code, set the event if timeout
		SetEvent(sem_);
		return -1;
	}
}

int XSemaphore::V()
{
	return SetEvent(sem_)?0:-1;
}

void SleepInMilliseconds(int msec)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Sleep(msec);
}

//XExtIPCEvent ipc;


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class XExtIPCEvent 

XExtIPCEvent::XExtIPCEvent()
      : m_pSMQueue(0),
        m_pSem(0),
        m_idProcess(-1)

{
// 	m_pfDefaultProc = 0;
// 	m_hThread = NULL;
	m_nMaxDataSize = 0;
}

XExtIPCEvent::~XExtIPCEvent()
{
	//MasterActive(FALSE);
}

int XExtIPCEvent::Lock ()
{
	return m_pSem->P();
}

int XExtIPCEvent::UnLock()
{
	return m_pSem->V();
}

// BOOL XExtIPCEvent::MasterActive(BOOL bEnable,SME_IPC_EVENT_CALLBACK_T pCall)
// {
// 	if (bEnable)
// 	{
// 		m_pfDefaultProc = pCall;
// 		DWORD dw = 0;
// 		m_hThread = ::CreateThread(0,0,(LPTHREAD_START_ROUTINE)(&XExtIPCEvent::IPCMonThread),this,0,&dw);
// 	}
// 	else
// 	{
// 		if (m_hThread)
// 		{
// 			Shutdown();
// 			WaitForSingleObject(m_hThread,INFINITE);
// 		}
// 	}
// 	return TRUE;
// }

// void XExtIPCEvent::Shutdown()
// {
// 	XExtIPCEvent ipc;
// 	int ret = ipc.Create(1,false);
// 
// 	if (ret<0)
// 	{
// 		printf("Error");
// 		return;
// 	}
// 
// 	IPCEventInfo EventInfo = {0};
// 
// 	EventInfo.EventID =-1;
// 	ret= ipc.PostIPCEvent(&EventInfo);
// 	if (ret ==0 )
// 	{
// 		TRACE("An external IPC event (EXIT) is post.\n");
// 	}
// 	ipc.Release(FALSE);
// }
// 
// DWORD WINAPI XExtIPCEvent::IPCMonThread(VOID* pVoid)
// {
// 	XExtIPCEvent* pThis = (XExtIPCEvent*)pVoid;
// 	ASSERT(pThis);
// 	if (!pThis)
// 	{
// 		return 0;
// 	}
// 	XExtIPCEvent ipc;
// 	int ret= ipc.Create(1,true);
// 
// 	do 
// 	{
// 		IPCEventInfo EventInfo;
// 		EventInfo.EventID=0;
// 
// 		ret= ipc.QueryIPCEvent(&EventInfo, NULL);
// 
// 		if (ret > 0 )
// 		{
// 			if (EventInfo.EventID<0)
// 				break;
// 			if (pThis && pThis->m_pfDefaultProc)
// 			{
// 				pThis->m_pfDefaultProc(&EventInfo);
// 			}
// 		}
// 
// 		SleepInMilliseconds(1000);
// 	} while (1);
// 
// 
// 
// 	ipc.Release(TRUE?true:false);
// 	return 0;
// }
// XExtIPCEvent* XExtIPCEvent::GetInstance ()
// {
// 	return NULL;
// //	return &ipc;
// }

int XExtIPCEvent::Create(int ProcessID,CStringA sMapName,CStringA sEventName, bool bIsMasterProcess)
{
	return Create(ProcessID, sMapName, sEventName, 0, bIsMasterProcess);
}
int XExtIPCEvent::Create(int ProcessID, CStringA strMapName, CStringA strEventName, int nMaxDataSize, bool bIsMasterProcess)
{
	ASSERT(!strMapName.IsEmpty());
	ASSERT(!strEventName.IsEmpty());
	if (strEventName.IsEmpty() || strMapName.IsEmpty())
	{
		//WriteLog("strEventName.IsEmpty()");
		return -1;
	}
	m_pSMQueue = new XSharedMemoryQueue();
	if(m_pSMQueue == 0)
		return -1;
	if(!m_pSMQueue->Initialize(strMapName/*SME_SHAREMEMORY_CMD_NAME*/, 
		sizeof(IPCEventInfo) + nMaxDataSize, IPC_EVENT_BUFFER_SIZE, IPC_QUEUE_NUMBER))
	{
		Release();
		//WriteLog("Initialize");
		return -1;
	}

	void* pCommandInfoPoint = 0;
	if(bIsMasterProcess)
		pCommandInfoPoint = m_pSMQueue->CreateSharedMemoryQueue();
	else
		pCommandInfoPoint = m_pSMQueue->OpenSharedMemoryQueue();

	if(pCommandInfoPoint==0)
	{
		Release();
		//WriteLog("pCommandInfoPoint==0");
		return -1;
	}

	m_pSem = new XSemaphore(strEventName/*SME_SHAREMEMORY_SEMAPHORE_CMD_NAME*/, bIsMasterProcess);
	if(m_pSem == 0)
	{
		Release();
		//WriteLog("sem == 0000000000000000000000000000");
		return -1;
	}

	m_idProcess = ProcessID;
	m_nMaxDataSize = nMaxDataSize;
	return 0;
}
BOOL XExtIPCEvent::Exist()
{
	return ((m_pSem != NULL) && (m_pSMQueue != NULL));
}
void XExtIPCEvent::Release (bool bIsMasterProcess)
{
	if(m_pSem)
	{
		delete m_pSem;
		m_pSem = NULL;
	}

	if(m_pSMQueue!=0)
	{
		m_pSMQueue->Release();
		if(bIsMasterProcess)
			m_pSMQueue->DeleteSharedMemoryQueue();
		else
			m_pSMQueue->CloseSharedMemoryQueue();
		
		delete m_pSMQueue;
		m_pSMQueue = 0;
	}
}

/**************************************************************************************************************
Allocate a data block and move it to the event sending queue. 

Sends the specified event to a process. This function does not return until the event status is consumed (EVENT_STATUS_CONSUMED). 

**************************************************************************************************************/

int XExtIPCEvent::SendIPCEvent(IPCEventInfo* pInEventInfo, IPCEventInfo* pOutEventInfo, int nTimeout /* = -1 */)
{
	int err = 0;

	if(pInEventInfo==0||pOutEventInfo==0||m_pSMQueue==0)
		return -1;
	else
		memset(pOutEventInfo, 0, sizeof(IPCEventInfo));

	pInEventInfo->IPCEventStatus = EVENT_STATUS_SENDING;

	if (-1 != m_idProcess)
		pInEventInfo->SourceProcessID = m_idProcess;

	//Get a block and set data
	int index = -1;
	if(Lock()<0) return -1;
	IPCEventInfo* pTmpInfo = (IPCEventInfo*)m_pSMQueue->GetQueueHead(IPC_QUEUE_RESOURCE);
	if(pTmpInfo!=0&&pTmpInfo!=pInEventInfo)
	{
		memcpy(pTmpInfo, pInEventInfo, sizeof(IPCEventInfo));
		m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_SENDING);
	}
	else
		err = -2;
	if(UnLock()<0) return -1;

	if(err!=0) return err;

    const int timeout = (nTimeout<0 ? SME_SENDCMD_TIMEOUT : nTimeout/SME_SENDCMD_INTERVAL);

	//Get the result
	int Status;
	int i=0;
	for(i=0;i<timeout;i++)
	{
		if(Lock()<0) return -1;
		Status = pTmpInfo->IPCEventStatus;
		if(UnLock()<0) return -1;
		if(EVENT_STATUS_CONSUMED == Status) break;
		SleepInMilliseconds(SME_SENDCMD_INTERVAL);
	}

	//return the result
	if(i==timeout)
	{
		memset(pOutEventInfo, 0, sizeof(IPCEventInfo));
		err = -1;
	}
	else
	{
		if(Lock()<0) return -1;
		memcpy(pOutEventInfo, pTmpInfo, sizeof(IPCEventInfo));
		m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
		if(UnLock()<0) return -1;
	}

	return err;
}

// Allocate a data block and move it to the event posting queue. 

int XExtIPCEvent::PostIPCEvent(IPCEventInfo* pInEventInfo, PBYTE pData, int nSize)
{
	if (pData != NULL)
	{
		ASSERT(nSize > 0);
		ASSERT(nSize <= m_nMaxDataSize);
		if (nSize <= 0 || nSize > m_nMaxDataSize)
			return -3;
	}
	if (nSize > 0)
	{
		ASSERT(pData != NULL);
		ASSERT(nSize <= m_nMaxDataSize);
		if (pData == NULL || nSize > m_nMaxDataSize)
			return -3;
	}

	int err = 0;

	if(pInEventInfo == 0 || m_pSMQueue == 0)
	{
		return -1;
	}
	pInEventInfo->IPCEventStatus = EVENT_STATUS_POSTING;
	if (-1 != m_idProcess)
		pInEventInfo->SourceProcessID = m_idProcess;

	//Get a block and set data
	if(Lock()<0)
	{
		//WriteLog("Lock < 0");
		return -1;
	}
	IPCEventInfo* pTmpInfo = (IPCEventInfo*)m_pSMQueue->GetQueueHead(IPC_QUEUE_RESOURCE);
	if(pTmpInfo!=0)
	{
		memcpy(pTmpInfo, pInEventInfo, sizeof(IPCEventInfo));
		memcpy(pTmpInfo + 1, pData, nSize);
		m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_POSTING);
	}
	else
	{
		//WriteLog("ptmpInfo = 0");
		err = -2;
	}
	if(UnLock()<0) 
	{
		//WriteLog("UnLock < 0");
		return -1;
	}
	return err;
}
int XExtIPCEvent::PostIPCEvent(IPCEventInfo* pInEventInfo)
{
	return PostIPCEvent(pInEventInfo, NULL, 0);
}

int XExtIPCEvent::QueryIPCEvent(OUT IPCEventInfo* pOutEventInfo, OUT PBYTE pdata, IN int nSize, SME_IPC_EVENT_CALLBACK_T pfnCallbak)
{
	int err = 0;
	if(pOutEventInfo==0||m_pSMQueue==0)
		return -1;
	else
		memset(pOutEventInfo, 0, sizeof(IPCEventInfo));

	// Get event from sending queue.
	if(Lock()<0) return -1;
	IPCEventInfo* pTmpFirstInfo = (IPCEventInfo*)m_pSMQueue->GetQueueHead(IPC_QUEUE_SENDING);
	IPCEventInfo* pTmpInfo = pTmpFirstInfo;
	while(pTmpInfo)
	{
		if(pTmpInfo->DestProcessID==m_idProcess)
		{
			memcpy(pOutEventInfo, pTmpInfo, sizeof(IPCEventInfo));
			m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
			err=1; // available
			break;
		}
		pTmpInfo = (IPCEventInfo*)m_pSMQueue->GetNext(pTmpInfo);
		if(pTmpInfo==pTmpFirstInfo)
		{
			memset(pOutEventInfo, 0, sizeof(IPCEventInfo));
			err = -1;
			break;
		}
	}
	if(UnLock()<0) return -1;

	if (err<0)
		return err;

	if(pTmpInfo!=0&&err>=0)
	{
		if(pfnCallbak&&!pfnCallbak(pOutEventInfo))
			err = -3;
		if(Lock()<0) return -1;
		pTmpInfo->ulParameter1=pOutEventInfo->ulParameter1;
		pTmpInfo->ulParameter2=pOutEventInfo->ulParameter2;
		pTmpInfo->IPCEventStatus=EVENT_STATUS_CONSUMED;
		if(UnLock()<0) return -1;
		return err;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Get event from posting queue.
	err = 0;
	if(Lock()<0) return -1;
	pTmpFirstInfo = (IPCEventInfo*)m_pSMQueue->GetQueueHead(IPC_QUEUE_POSTING);
	pTmpInfo = pTmpFirstInfo;
	while(pTmpInfo)
	{
		//DestProcessID针对所有进程
		if(pTmpInfo->DestProcessID==m_idProcess || pTmpInfo->DestProcessID == 0)
		{
			memcpy(pOutEventInfo, pTmpInfo, sizeof(IPCEventInfo));
			if (nSize > 0)
			{
				ASSERT(nSize <= m_nMaxDataSize);
				if (nSize > m_nMaxDataSize)
				{
					err = -4;
					break;
				}
				memcpy(pdata, pTmpInfo + 1, nSize);
			}
			m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
			err=1; // available
			break;
		}
		pTmpInfo = (IPCEventInfo*)m_pSMQueue->GetNext(pTmpInfo);
		if(pTmpInfo==pTmpFirstInfo)
		{
			memset(pOutEventInfo, 0, sizeof(IPCEventInfo));
			if (nSize > 0)
			{
				ASSERT(nSize <= m_nMaxDataSize);
				if (nSize > m_nMaxDataSize)
				{
					err = -4;
					break;
				}
				memcpy(pdata, pTmpInfo + 1, nSize);
			}
			err = -1;
			break;
		}
	}
	if(UnLock()<0) return -1;

	if (err<0)
		return err;

	if(pTmpInfo!=0&&err>=0)
	{
		if(pfnCallbak&&!pfnCallbak(pOutEventInfo))
			err = -3;
		return err;
	}

	return err;
}
/************************************************************************************************************
// Return 
// 1: IPC event is available.
// 0: No IPC event.
// <0: Error
************************************************************************************************************/
int XExtIPCEvent::QueryIPCEvent(IPCEventInfo* pOutEventInfo, SME_IPC_EVENT_CALLBACK_T pfnCallbak)
{
	return QueryIPCEvent(pOutEventInfo, NULL, 0, NULL);
}


void XExtIPCEvent::RecycleEvent(int idProcess)
{
	if(Lock()<0) return;
	for(int i=1;i<IPC_QUEUE_NUMBER;i++)
	{
		IPCEventInfo* pTmpFirstInfo = (IPCEventInfo*)m_pSMQueue->GetQueueHead(i);
		IPCEventInfo* pTmpInfo = pTmpFirstInfo;
		while(pTmpInfo)
		{
			IPCEventInfo* pTmpNextInfo = (IPCEventInfo*)m_pSMQueue->GetNext(pTmpInfo);
			if(pTmpInfo->DestProcessID==idProcess)
				m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
			if(pTmpInfo->SourceProcessID==idProcess)
				m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);

			pTmpInfo = pTmpNextInfo;
			if(m_pSMQueue->GetQueueHead(i)==0||pTmpInfo==(IPCEventInfo*)m_pSMQueue->GetQueueHead(i)) break;
		}
	}
	if(UnLock()<0) return;
}


bool XExtIPCEvent::DumpInfo ()
{
	printf("Commands Information:\n");
	if(Lock()<0) 
		return false;
	for(int i=0;m_pSMQueue!=0&&i<IPC_QUEUE_NUMBER;i++)
	{
		if(i!=0)
			m_pSMQueue->DumpQueue(i, true);
		else
			m_pSMQueue->DumpQueue(i, false);
	}
	if(UnLock()<0) return false;
	//printf("\n");

	return true;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSharedMemoryQueue::XSharedMemoryQueue()
      : m_strMapName(0),
        m_iNodeSize(0),
        m_iNodeCount(0),
        m_iQueueCount(0),
        m_pQueueArray(0),
        m_hMapMem(0),
        m_pMappedPointer(0)
{
}

XSharedMemoryQueue::~XSharedMemoryQueue()
{
	if(m_strMapName)
	{
		free(m_strMapName);
		m_strMapName = 0;
	}
}

bool XSharedMemoryQueue::Initialize (const char* strMapName, int iNodeSize, int iNodeCount, int iQueueCount)
{
	if(strMapName==0||iNodeSize<=0||iNodeCount<=0||iQueueCount<=0)
		return false;
	m_strMapName = strdup(strMapName);
	m_iNodeSize = iNodeSize + 2*sizeof(int); // Node, Prevous Node Pointer, Next Node Pointer
	m_iNodeCount = iNodeCount;
	m_iQueueCount = iQueueCount;
	if(m_strMapName==0)
	{
		Release();
		return false;
	}
	
	return true;
}

void XSharedMemoryQueue::Release ()
{
	m_iNodeSize = 0;
	m_iNodeCount = 0;
	m_iQueueCount = 0;
}

// Link all nodes together as a double linked list: queue 0.
void* XSharedMemoryQueue::CreateSharedMemoryQueue ()
{
	SECURITY_DESCRIPTOR sd;

	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);

	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);

	SECURITY_ATTRIBUTES sa = { sizeof(sa), &sd, FALSE };

	m_hMapMem = CreateFileMappingA(INVALID_HANDLE_VALUE,
		&sa,
		PAGE_READWRITE,
		0,
		m_iNodeSize*m_iNodeCount + m_iQueueCount*sizeof(int),
		m_strMapName);

	if (m_hMapMem == NULL||GetLastError()==ERROR_ALREADY_EXISTS)
		return 0;
	
	m_pMappedPointer = MapViewOfFile(m_hMapMem,
		FILE_MAP_WRITE | FILE_MAP_READ,
		0, 0, m_iNodeSize*m_iNodeCount+m_iQueueCount*sizeof(int));


	if (m_pMappedPointer == NULL)
		return 0;

	// Link all nodes together as a double linked list: queue 0.
	int i=0;
	for(i=0;i<m_iNodeCount;i++)
	{
		void* pNode = (char*)m_pMappedPointer+i*m_iNodeSize;
		int *pNext = (int*)((char*)pNode+m_iNodeSize-sizeof(int));
		int *pPrev = (int*)((char*)pNext-sizeof(int));

		if(i==m_iNodeCount-1)
			*pNext = 0;
		else
			*pNext = (i+1)*m_iNodeSize;
		if(i==0)
			*pPrev = (m_iNodeCount-1)*m_iNodeSize;
		else
			*pPrev = (i-1)*m_iNodeSize;
	}

	m_pQueueArray = (int*)((char*)m_pMappedPointer+m_iNodeSize*m_iNodeCount);
	for(i=0;i<m_iQueueCount;i++)
		m_pQueueArray[i] = -1;
	m_pQueueArray[0] = 0;

	return m_pMappedPointer;
}

void* XSharedMemoryQueue::OpenSharedMemoryQueue ()
{

	m_hMapMem= OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE,
		TRUE,
		m_strMapName);
	if (m_hMapMem == NULL)
	{
// 		TCHAR szText[64];
// 		wsprintf(szText, "Openfilemapping failed.%d", GetLastError());
// 		WriteLog(szText);
		return 0;
	}
	m_pMappedPointer = MapViewOfFile(m_hMapMem,
		FILE_MAP_WRITE | FILE_MAP_READ,
		0, 0, m_iNodeSize*m_iNodeCount+m_iQueueCount*sizeof(int));
	if (m_pMappedPointer == NULL)
	{
// 		TCHAR szText[64];
// 		wsprintf(szText, "MapViewOfFile failed.%d", GetLastError());
// 		WriteLog(szText);
		return 0;
	}
	m_pQueueArray = (int*)((char*)m_pMappedPointer+m_iNodeSize*m_iNodeCount);
	return m_pMappedPointer;
}

////////////////////////////////////////////////////////////////////////////////////////
void XSharedMemoryQueue::DeleteSharedMemoryQueue ()
{
	if (m_pMappedPointer)
	{
		UnmapViewOfFile (m_pMappedPointer);
		m_pMappedPointer = NULL;
	}
	
	if (m_hMapMem)
	{
		CloseHandle (m_hMapMem);
		m_hMapMem = NULL;
	}
}


void XSharedMemoryQueue::CloseSharedMemoryQueue ()
{

	if (m_pMappedPointer)
	{
		UnmapViewOfFile (m_pMappedPointer);
		m_pMappedPointer = NULL;
	}
	
	if (m_hMapMem)
	{
		CloseHandle (m_hMapMem);
		m_hMapMem = NULL;
	}
}

void* XSharedMemoryQueue::GetAt(int iIndex)
{
	if(iIndex<0||iIndex>=m_iNodeCount) return 0;
	return (char*)m_pMappedPointer+m_iNodeSize*iIndex;
}

void* XSharedMemoryQueue::GetNext(void* pNode)
{
	if(pNode==0) return 0;
	int *pNext = (int*)((char*)pNode+m_iNodeSize-sizeof(int));
	int *pPrev = (int*)((char*)pNext-sizeof(int));
	return (char*)m_pMappedPointer+*pNext;
}

void* XSharedMemoryQueue::GetPrev(void* pNode)
{
	if(pNode==0) return 0;
	int *pNext = (int*)((char*)pNode+m_iNodeSize-sizeof(int));
	int *pPrev = (int*)((char*)pNext-sizeof(int));
	return (char*)m_pMappedPointer+*pPrev;
}

////////////////////////////////////////////////////////////////////////////////////

void* XSharedMemoryQueue::GetQueueHead(int iQueue)
{
	if(iQueue<0||iQueue>=m_iQueueCount) return 0;
	if(m_pQueueArray[iQueue]==-1) return 0;
	return (char*)m_pMappedPointer+m_pQueueArray[iQueue];
}

void* XSharedMemoryQueue::GetQueueTail (int iQueue)
{
	if(iQueue<0||iQueue>=m_iQueueCount) return 0;
	if(m_pQueueArray[iQueue]==-1) return 0;

	void* pNode = (char*)m_pMappedPointer+m_pQueueArray[iQueue];
	int *pNext = (int*)((char*)pNode+m_iNodeSize-sizeof(int));
	int *pPrev = (int*)((char*)pNext-sizeof(int));
	return (char*)m_pMappedPointer+*pPrev;
}



/**********************************************************************************************
Remove the given node from the source queue and add this node to the destination queue.
If the destination queue is empty, add the node as the head of the queue.
If the destination queue is not empty, append the node to the destination queue. 

The souce and destination queues are double linked lists. 
**********************************************************************************************/
bool XSharedMemoryQueue::MoveToDestQueueTail(void* pNode, int iDestQueue)
{
	if(iDestQueue<0||iDestQueue>=m_iQueueCount) return false;
	if(pNode==0) return false;

	//Check if the pNode is the first node of a queue
	int i=0;
	for(i=0;i<m_iQueueCount;i++)
	{
		if(m_pQueueArray[i]!=-1&&(char*)m_pMappedPointer+m_pQueueArray[i]==pNode)
			break;
	}

	void* pNewNode = pNode;
	int *pNodeNext = (int*)((char*)pNewNode+m_iNodeSize-sizeof(int));
	int *pNodePrev = (int*)((char*)pNodeNext-sizeof(int));

	if(i==m_iQueueCount)
	{
		// Not the first node of a queue, remove the node from the double linked quque.
		// Update the previous node' next node pointer
		*(int*)(((char*)m_pMappedPointer+*pNodePrev)+m_iNodeSize-sizeof(int))=*pNodeNext; 
		// Update the next node's previous node pointer 
		*(int*)(((char*)m_pMappedPointer+*pNodeNext)+m_iNodeSize-2*sizeof(int))=*pNodePrev; 
	}
	else
	{
		// The first node of a queue
		if(((char*)m_pMappedPointer+*pNodeNext)==pNewNode) // The node'next pointer is the itself.
		{
			//Only one node in the original queue, becomes an empty queue.
			m_pQueueArray[i] = -1;
		}
		else
		{
			//Not only one node in the orginal queue, remove the node from the double linked quque.
			m_pQueueArray[i] = *pNodeNext;
			*(int*)(((char*)m_pMappedPointer+*pNodePrev)+m_iNodeSize-sizeof(int))=*pNodeNext;
			*(int*)(((char*)m_pMappedPointer+*pNodeNext)+m_iNodeSize-2*sizeof(int))=*pNodePrev;
		}
	}

	if(m_pQueueArray[iDestQueue]==-1)
	{
		// Empty queue, become the fist node. 
		// The queue header, the node'next and previous pointer to itself.
		*pNodeNext = (char*)pNewNode-(char*)m_pMappedPointer; // The offset of the node 
		*pNodePrev = *pNodeNext; // The offset of the node
		m_pQueueArray[iDestQueue] = *pNodeNext;
	}
	else
	{
		// Not empty queue, add the new node behind the head of the new queue.
		void* pHeader = (char*)m_pMappedPointer+m_pQueueArray[iDestQueue];
		int *pHeaderNext = (int*)((char*)pHeader+m_iNodeSize-sizeof(int));
		int *pHeaderPrev = (int*)((char*)pHeaderNext-sizeof(int));

		// Append the new node to the queue tail. Become the new tail of the queue.

		// Update the next pointer of the tail node to the new node 
		*(int*)(((char*)m_pMappedPointer+*pHeaderPrev)+m_iNodeSize-sizeof(int)) 
			= (char*)pNewNode-(char*)m_pMappedPointer;
		// Update the previous pointer of the new node to the tail. 
		*pNodePrev = *pHeaderPrev;
		// Update the old header's previous pointer to the new node (new tail).
		*pHeaderPrev = (char*)pNewNode-(char*)m_pMappedPointer;
		// Update the next pointer of the new to the head.
		*pNodeNext = m_pQueueArray[iDestQueue];
	}

	return true;
}

int XSharedMemoryQueue::GetIndex (void* pNode)
{
	return ((char*)pNode-(char*)m_pMappedPointer)/m_iNodeSize;
}

int XSharedMemoryQueue::LocateQueue(void* pNode)
{
	int i=0;
	for(i=0;i<m_iQueueCount;i++)
	{
		void* pFirstTmp = GetQueueHead(i);
		void* pTmp = pFirstTmp;
		while(pTmp)
		{
			if(pTmp==pNode) return i;
			pTmp = GetNext(pTmp);
			if(pTmp==pFirstTmp) break;
		}
	}

	return -1;
}


void XSharedMemoryQueue::DumpQueue (int iQueue, bool bDumpNode)
{
	int iCount = 0;
	void* pTmp = 0;
	void* pFirstTmp = GetQueueHead(iQueue);
	pTmp = pFirstTmp;
	printf("Dump queue %d:\n", iQueue);
	while(pTmp)
	{
		iCount++;
		if(bDumpNode)
			printf("%08x ", pTmp);

		pTmp = GetNext(pTmp);
		if(pTmp==pFirstTmp) break;
	}
	printf("\tnumber %d\n", iCount);
}

void XSharedMemoryQueue::DumpNode (void* pNode)
{
	void* pSrcNode = pNode;
	int *pSrcNext = (int*)((char*)pSrcNode+m_iNodeSize-sizeof(int));
	int *pSrcPrev = (int*)((char*)pSrcNext-sizeof(int));
	printf("Dump Node %08x: Prev\t%08x (%d), Next\t%08x (%d)\n", pNode, 
		(char*)m_pMappedPointer+*pSrcPrev, *pSrcPrev,
		(char*)m_pMappedPointer+*pSrcNext, *pSrcNext);
}

