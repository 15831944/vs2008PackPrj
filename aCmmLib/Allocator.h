// Allocator.h: interface for the CAllocator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLOCATOR_H__12751161_065E_4724_A989_D183F1F0688C__INCLUDED_)
#define AFX_ALLOCATOR_H__12751161_065E_4724_A989_D183F1F0688C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
#include <afxtempl.h>

class CBuffer;
class CCriticalSection2;
/*
class _AA_DLL_EXPORT_ CCriticalSection2
{
public:
	 class Owner
     {
         public:

            explicit Owner(
				CCriticalSection2 &crit): m_crit(crit)
			{
				crit.Enter();
			}

            ~Owner()
			{
				m_crit.Leave();
			}
      
         private :

            CCriticalSection2 &m_crit;

            // No copies do not implement
            Owner(const Owner &rhs);
            Owner &operator=(const Owner &rhs);
     };

	CCriticalSection2()
	{
		::InitializeCriticalSection(&m_crit);
	}
	~CCriticalSection2()
	{
		::DeleteCriticalSection(&m_crit);
	}
	void Enter()
	{
		::EnterCriticalSection(&m_crit);
	}
	void Leave()
	{
		::LeaveCriticalSection(&m_crit);
	}

	CRITICAL_SECTION m_crit;

};
*/
class _AA_DLL_EXPORT_ CAllocator  
{
	friend CBuffer;
public:
	void SetMaxBufSize(int nSize);
	void RemoveFromList(CBuffer* pBuffer,CPtrList& list);
	void DestroyBuffer(CBuffer* pBuffer);
	void Flush();
	void Release(CBuffer* pBuffer);
	CBuffer* Allocate();
	CAllocator();
	virtual ~CAllocator();

protected:
	CPtrList	m_freeList;
	CPtrList	m_activeList;
	CCriticalSection2 m_cs;

	size_t m_nBufferSize;
	size_t m_nMaxFreeBuffers;
	
	virtual void OnBufferCreated() {}
	virtual void OnBufferAllocated() {}
	virtual void OnBufferReleased() {}
	virtual void OnBufferDestroyed() {}


};

class _AA_DLL_EXPORT_ CBuffer  
{
	friend CAllocator;
public:
	void AddUsed(int nLen);
	void GrowBuffer(int nSize);
	void SetMinPackSize(int nSize);
	void CheckBufSize(int nSize);
	BOOL IsEmpty();
	void AddData(CBuffer* pBuf);
	void DeleteData(int nSize);

	void SetSequenceNumber(size_t nSeq);
	size_t GetSequenceNumber();
	int GetMinPackSize();
	BYTE* GetBufData(int nStart = 0);
	void ReadData(int nStart,int &nData);
	int GetUnused();
	int GetSize();
	BYTE* GetUnusedBufData();
	int GetUsed();
	void Empty();
	CBuffer* SplitBuffer(int nSize);
	void AddData(CStringA sData);
	void AddData(BYTE* pData,int nDataLen);
	void AddData(char* pBuf,int nDataLen);
	void AddRef();
	void Release();
	CBuffer(CAllocator* pAloc,size_t nSize);
	CBuffer();
	virtual ~CBuffer();
#ifdef _WINSOCK2API_
	WSABUF* GetSndWsBuf();
	WSABUF* GetRcvWsBuf();
#endif
	

private:
	CAllocator* m_pAlloc;
#ifdef _WINSOCK2API_
	WSABUF	m_wsBuf;
#endif

	BYTE*	m_pBuf;
	size_t	m_size;
	size_t	m_used;
	size_t  m_minSize;
	long	m_ref;
	POSITION m_pos;					//内存列表中的位置
	size_t  m_sequenceNumber;		//顺序读写的下一个包
};

class _AA_DLL_EXPORT_ CInOrderBufferList
{
public:
	CBuffer* GetAllBuffer(int& nSize);
	void Reset();
	BOOL IsLostOrder();
	void SetLostOrder(BOOL b = TRUE);
	BOOL IsNext(int nIndex);
	void SetRightCount(int nCount);
	BYTE* GetAllBuffer(int &nSize,BOOL bOnlyBody );
	void AddBuffer(CBuffer* pBuf);

	CInOrderBufferList();
	~CInOrderBufferList();

private:
	void Empty();

	int								m_next;
	CCriticalSection2				m_cs;
//	CMap<int,int,CBuffer*,CBuffer*>	m_list;
	CPtrList						m_list;
	int								m_nTotalCount;
	int								m_nTotalLen;
	int								m_nTotalBodyLen;
	int								m_nRightCount;
	BOOL							m_bLostOrder;
};


#endif // !defined(AFX_ALLOCATOR_H__12751161_065E_4724_A989_D183F1F0688C__INCLUDED_)
