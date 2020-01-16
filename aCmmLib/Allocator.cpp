// Allocator.cpp: implementation of the CAllocator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Allocator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAllocator::CAllocator()
{
	m_nBufferSize = 1024 * 2;
	m_nMaxFreeBuffers = 100;
}

CAllocator::~CAllocator()
{
	Flush();
}

CBuffer* CAllocator::Allocate()
{
	CCriticalSection2::Owner lock(m_cs);
	
	CBuffer *pBuffer = 0;
	
	if (!m_freeList.IsEmpty())
	{
		pBuffer = (CBuffer*)m_freeList.RemoveHead();
		
		pBuffer->AddRef();
	}
	else
	{
		pBuffer = new CBuffer(this, m_nBufferSize);
		
		if (!pBuffer)
		{
			//throw CException(_T("CIOBuffer::Allocator::Allocate()"),_T("Out of memory"));
		}
		
		OnBufferCreated();
	}
	
	POSITION pos = m_activeList.AddTail(pBuffer);
	pBuffer->m_pos = pos;
	
	OnBufferAllocated();
	
	return pBuffer;
}

void CAllocator::Release(CBuffer *pBuffer)
{
	ASSERT(pBuffer);
	if (!pBuffer)
	{
		return ;
	}
	
	CCriticalSection2::Owner lock(m_cs);
	
	//lint -e{1933} call to unqualified virtual function
	OnBufferReleased();
	
	// unlink from the in use list
	
	RemoveFromList(pBuffer,m_activeList);
	
	//是一个临时增长的大包就释放，防止占用内存过大
	if (pBuffer->GetSize() > m_nBufferSize)
	{
		DestroyBuffer(pBuffer);
		return ;
	}

	if (m_nMaxFreeBuffers == 0 || 
		m_freeList.GetCount() < (int)m_nMaxFreeBuffers)
	{
		pBuffer->Empty();           
		
		// add to the free list
		
		m_freeList.AddTail(pBuffer);
	}
	else
	{
		DestroyBuffer(pBuffer);
	}
}

void CAllocator::Flush()
{
	CCriticalSection2::Owner lock(m_cs);
	
	while (!m_activeList.IsEmpty())
	{
		//lint -e{1933} call to unqualified virtual function
		OnBufferReleased();
		
		DestroyBuffer((CBuffer*)m_activeList.RemoveHead());
	}
	
	while (!m_freeList.IsEmpty())
	{
		DestroyBuffer((CBuffer*)m_freeList.RemoveHead());
	}
}

void CAllocator::DestroyBuffer(CBuffer *pBuffer)
{
	delete pBuffer;
	pBuffer = NULL;
	//lint -e{1933} call to unqualified virtual function
	OnBufferDestroyed();
}

void CAllocator::RemoveFromList(CBuffer *pBuffer, CPtrList &list)
{
	if(pBuffer->m_pos)
	{
		list.RemoveAt(pBuffer->m_pos);
	}
}

//---------------------------------------------------
//CBuffer
//

CBuffer::CBuffer(CAllocator* pAloc,size_t nSize)
{
	ASSERT(pAloc);
	m_pAlloc = pAloc;
	m_pBuf = new BYTE[nSize];
	memset(m_pBuf,0,nSize);
	m_pos = NULL;
	m_size = nSize;
	m_used = 0;
	m_ref = 0;
	
	m_minSize = 8;
	AddRef();

}

CBuffer::CBuffer()
{
	m_pAlloc = NULL;
	m_pBuf = NULL;
	m_pos = NULL;
	m_size = 4096;
	m_pBuf = new BYTE[m_size];
	memset(m_pBuf,0,m_size);
	m_used = 0;
	m_ref = 0;
	m_minSize = 8;
	AddRef();
}

CBuffer::~CBuffer()
{
	if(m_pBuf)
	{
		delete []m_pBuf;
		m_pBuf = NULL;
	}
}

void CBuffer::AddRef()
{
   ::InterlockedIncrement(&m_ref);
}

void CBuffer::Release()
{
   if (m_ref == 0)
   {
		ASSERT(0);
   }

   if (0 == ::InterlockedDecrement(&m_ref))
   {
      m_used = 0;
	  ASSERT(m_pAlloc);
	  if(m_pAlloc)
	  {
		  m_pAlloc->Release(this);
	  }
	  else
	  {
		  Empty();
	  }
   }
}
void CBuffer::CheckBufSize(int nSize)
{
	if (nSize > m_size)
	{
		int nGrow = nSize - m_size + 1;
		GrowBuffer(nGrow);
	}
}
void CBuffer::GrowBuffer(int nSize)
{
	BYTE* pTmp = new BYTE[m_size + nSize];
	ASSERT(pTmp);
	if (!pTmp)
	{
		return ;
	}

	memset(pTmp,0,m_size + nSize);
	if(m_used > 0)
	{
		memcpy(pTmp,m_pBuf,m_used);
	}
	delete []m_pBuf;
	m_pBuf = pTmp;
	m_size = m_size + nSize;
}

void CBuffer::AddData(char *pData, int nDataLen)
{
	AddData((BYTE*)pData,nDataLen);
}

void CBuffer::AddData(BYTE *pData, int nDataLen)
{
	if(nDataLen > (int)(m_size - m_used))
	{
		GrowBuffer(1024 + nDataLen);
	}
	memcpy(m_pBuf + m_used,pData,nDataLen);
	m_used += nDataLen;
}

void CBuffer::AddData(CStringA sData)
{
	char* p = sData.GetBuffer(0);
	AddData(p,sData.GetLength());
	sData.ReleaseBuffer();
}

//从头分割出一个包
CBuffer* CBuffer::SplitBuffer(int nSize)
{
	CBuffer *pNewBuffer = m_pAlloc->Allocate();
	
	pNewBuffer->SetMinPackSize(m_minSize);
	pNewBuffer->AddData(m_pBuf, nSize);
	
	m_used -= nSize;
	
	memmove(m_pBuf, m_pBuf + nSize, m_used);
	memset(m_pBuf + m_used,0,m_size - m_used);
	
	return pNewBuffer;
}

void CBuffer::Empty()
{
	m_used = 0;
	memset(m_pBuf,0,m_size);
	m_ref = 0;
	m_pos = NULL;
}

int CBuffer::GetUsed()
{
	return m_used;
}

//获取未使用的缓冲
BYTE* CBuffer::GetUnusedBufData()
{
	return m_pBuf + m_used;
}

int CBuffer::GetSize()
{
	return m_size;
}

void CBuffer::AddUsed(int nLen)
{
	ASSERT(nLen <= (int)(m_size - m_used));
	m_used += nLen;	
}

int CBuffer::GetUnused()
{
	return m_size - m_used;
}

void CBuffer::ReadData(int nStart, int &nData)
{
	ASSERT(nStart+(int)sizeof(int) <= GetUsed());

	memcpy(&nData,m_pBuf + nStart,sizeof(int));
}

BYTE* CBuffer::GetBufData(int nStart)
{
	ASSERT(nStart <= GetUsed());
	return m_pBuf + nStart;
}

size_t CBuffer::GetSequenceNumber()
{
	return m_sequenceNumber;
}

void CBuffer::SetSequenceNumber(size_t nSeq)
{
	m_sequenceNumber = nSeq;
}

//----------------------------
//包的最小长度
int CBuffer::GetMinPackSize()
{
	return m_minSize;
}

void CBuffer::SetMinPackSize(int nSize)
{
	m_minSize = nSize;
}

#ifdef _WINSOCK2API_
WSABUF* CBuffer::GetRcvWsBuf()
{
	m_wsBuf.buf = (char*)(m_pBuf + m_used);
	m_wsBuf.len = m_size - m_used;

	return &m_wsBuf;
}

WSABUF* CBuffer::GetSndWsBuf()
{
	m_wsBuf.buf = (char*)(m_pBuf);
	m_wsBuf.len = m_used;

	return &m_wsBuf;
}

#endif

//从缓冲开始删除数据
void CBuffer::DeleteData(int nSize)
{
	ASSERT(nSize <= (int)m_used);
	//
	if(nSize == (int)m_size || nSize == (int)m_used)
	{
		Empty();
		return ;
	}


	memmove(m_pBuf, m_pBuf + nSize, m_used - nSize);
	m_used -= nSize;
	memset(m_pBuf + m_used,0,m_size - m_used);
	
}

void CBuffer::AddData(CBuffer *pBuf)
{
	ASSERT(pBuf);
	AddData((BYTE*)pBuf->GetBufData(),pBuf->GetUsed());
}

BOOL CBuffer::IsEmpty()
{
	return m_used == 0 ? TRUE : FALSE;
}

//------------------------------------------------
//CInOrderBufferList
//
CInOrderBufferList::CInOrderBufferList()
{
	Empty();
}

CInOrderBufferList::~CInOrderBufferList()
{
	Empty();
}

void CInOrderBufferList::Empty()
{
	m_nTotalCount = 0;
	m_nTotalLen = 0;
	m_nTotalBodyLen = 0;
	m_bLostOrder = FALSE;
	CBuffer* pBuf = NULL;
	while(m_list.GetCount() > 0)
	{
		pBuf = (CBuffer*)m_list.RemoveHead();
		pBuf->Release();
	}
	m_list.RemoveAll();
}

void CInOrderBufferList::AddBuffer(CBuffer *pBuf)
{
	ASSERT(pBuf);

	CCriticalSection2::Owner lock(m_cs);
	
	m_list.AddTail(pBuf);
	m_nTotalLen += pBuf->GetUsed();
	m_nTotalCount ++;
	m_nTotalBodyLen += (pBuf->GetUsed() - pBuf->GetMinPackSize());
}

//----------------
//获取所有的包信息
//参数：bOnlyBody = TRUE 只获取包体部分，不包括包头信息

BYTE* CInOrderBufferList::GetAllBuffer(int &nSize ,BOOL bOnlyBody)
{
	CCriticalSection2::Owner lock(m_cs);
	
	if(bOnlyBody)
	{
		nSize = m_nTotalBodyLen;
	}
	else
	{
		nSize = m_nTotalLen;
	}
	ASSERT(nSize);

	BYTE* pByte = new BYTE[nSize];
	ASSERT(pByte);
	ZeroMemory(pByte,nSize);


	CBuffer* pBuf = NULL;
	int nPos = 0;
	while(m_list.GetCount() > 0)
	{
		pBuf = (CBuffer*)m_list.RemoveHead();
		if(pBuf)
		{
			if(bOnlyBody)
			{
				CopyMemory(pByte + nPos,pBuf->GetBufData(pBuf->GetMinPackSize()),pBuf->GetUsed() - pBuf->GetMinPackSize());
				nPos += pBuf->GetUsed() - pBuf->GetMinPackSize();
			}
			else
			{
				CopyMemory(pByte + nPos,pBuf->GetBufData(),pBuf->GetUsed());
				nPos += pBuf->GetUsed();
			}
			pBuf->Release();
		}
	}

	ASSERT(m_list.GetCount() == 0);
//	Empty();
	return pByte;
}

void CInOrderBufferList::SetRightCount(int nCount)
{
	CCriticalSection2::Owner lock(m_cs);
	m_nRightCount = nCount;
}

BOOL CInOrderBufferList::IsNext(int nIndex)
{
	CCriticalSection2::Owner lock(m_cs);

	return nIndex == (m_nTotalCount + 1 ) && nIndex <= m_nRightCount ? TRUE : FALSE;
}

void CInOrderBufferList::SetLostOrder(BOOL b)
{
	CCriticalSection2::Owner lock(m_cs);
	m_bLostOrder = b;

}

BOOL CInOrderBufferList::IsLostOrder()
{
	CCriticalSection2::Owner lock(m_cs);
	return m_bLostOrder;
}


void CInOrderBufferList::Reset()
{
	CCriticalSection2::Owner lock(m_cs);
	Empty();
}

CBuffer* CInOrderBufferList::GetAllBuffer(int &nSize)
{
	CCriticalSection2::Owner lock(m_cs);
	
	nSize = m_nTotalLen;
	
	ASSERT(nSize);

	int nGrowSize = 0;
	CBuffer  *pBuf = NULL,
			 *pTmp = NULL;
	if(m_list.GetCount() > 0)
	{
		pBuf = (CBuffer*)m_list.RemoveHead();
		ASSERT(pBuf);
		nGrowSize = nSize - pBuf->GetSize();
		if(nGrowSize > 0)
		{
			pBuf->GrowBuffer(nGrowSize);
		}

	}
	else
	{
		ASSERT(FALSE);
	}

	while(m_list.GetCount() > 0)
	{
		pTmp = (CBuffer*)m_list.RemoveHead();
		if(pTmp)
		{
			pBuf->AddData(pTmp);
			
			pTmp->Release();
		}
	}

	ASSERT(m_list.GetCount() == 0);
//	Empty();
	return pBuf;
}


void CAllocator::SetMaxBufSize(int nSize)
{
	m_nBufferSize = nSize;
}
