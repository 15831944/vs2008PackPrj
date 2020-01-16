#include "stdafx.h"
#include "IPRange.h"


//--------------------------
//iprange
CIPRange::CIPRange(IPRange* pRg)
{
	ASSERT(pRg);
	CopyMemory(this,pRg,sizeof(IPRange));

}

CIPRange::CIPRange(UINT b,UINT e)
{
	nBIP = b;
	nEIP = e;
}

BOOL CIPRange::IsInRng(UINT nIP)
{
	if (nIP >= nBIP && nIP <= nEIP)
	{
		return TRUE;
	}
	return FALSE;
}

DWORD CIPRange::StringToDWORD(CStringA strIP)
{
	DWORD dwIP = 0;

	BOOL bSuc = FALSE;

	BYTE b1 = 0, b2 = 0, b3 = 0, b4 = 0;

	do 
	{
		int n1 = 0, n2;
		n2	= strIP.Find('.');
		if(n2 == -1)
			break;

		b1 = (char)atoi(strIP.Mid(n1, n2));

		n1 = n2+1;
		n2 = strIP.Find('.', n1);
		if(n2 == -1)
			break;

		b2 = (char)atoi(strIP.Mid(n1, n2));

		n1 = n2+1;
		n2 = strIP.Find('.', n1);
		if(n2 == -1)
			break;

		b3 = (char)atoi(strIP.Mid(n1, n2));

		n1 = n2+1;
		if(n1 >= strIP.GetLength())
			break;
		b4 = (char)atoi(strIP.Mid(n1));

		bSuc = TRUE;
	} while(0);

	if(!bSuc)
	{
		TRACE("传入IP字符串非法:%s\n", strIP);
		return dwIP;
	}

	dwIP = MAKEIPADDRESS(b1, b2, b3, b4);

	return dwIP;
}

//----------------------------------
//机器
CIPRangeList::~CIPRangeList()
{
	Release();
}
void CIPRangeList::Release()
{
	POSITION old = NULL;
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		old = pos;
		CIPRange* pRng = GetNext(pos);
		if (pRng)
		{
			delete pRng;
		}
		RemoveAt(old);
	}
}
void CIPRangeList::Attach(IPRange* pRng,int nCount)
{
	ASSERT(pRng);
	if (!pRng || nCount < 1)
	{
		return ;
	}
	for (int n = 0; n < nCount ; n ++)
	{
		CIPRange* pInfo = new CIPRange(pRng);
		ASSERT(pInfo);
		if (pInfo)
		{
			AddTail(pInfo);
		}
		pRng += 1;
	}
}

BOOL CIPRangeList::GetListData(BYTE* pData,int nLen)
{
	if (!AfxIsValidAddress(pData,nLen))
	{
		ASSERT(0);
		return FALSE;
	}

	POSITION pos = GetHeadPosition();
	IPRange* pName = (IPRange*)pData;
	int nIndex = 0;
	while (pos)
	{
		if ((nIndex + 1) * sizeof(IPRange) > nLen)
		{
			break;
		}
		CIPRange* pRng = GetNext(pos);
		if (pRng)
		{
			CopyMemory(pName,pRng,sizeof(IPRange));
			pName += 1;
			nIndex ++;
		}

	}
	return TRUE;
}

BOOL CIPRangeList::IsConflict(UINT nBIP,UINT nEIP)
{
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		CIPRange* pRng = GetNext(pos);
		if (pRng)
		{
			if (pRng->IsInRng(nBIP) || pRng->IsInRng(nEIP) || (nBIP < pRng->nBIP && nEIP > pRng->nEIP))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CIPRangeList::DeleteOne(CIPRange* pIP)
{
	POSITION pos = Find(pIP);
	if (pos)
	{
		delete pIP;
		RemoveAt(pos);
		return TRUE;
	}
	return FALSE;
}

CIPRange* CIPRangeList::AddOne(UINT nBIP,UINT nEIP)
{
	CIPRange* pInfo = new CIPRange(nBIP,nEIP);
	ASSERT(pInfo);
	if (pInfo)
	{
		AddTail(pInfo);
		return pInfo;
	}
	return NULL;
}

CIPRange* CIPRangeList::AddOne(CStringA sBIP,CStringA sEIP)
{
	CIPRange* pInfo = new CIPRange(CIPRange::StringToDWORD(sBIP),CIPRange::StringToDWORD(sEIP));
	if (pInfo)
	{
		AddTail(pInfo);
		return pInfo;
	}
	ASSERT(0);
	return NULL;
}

BOOL CIPRangeList::IsInList(UINT nIP)
{
	POSITION pos = GetHeadPosition();
	while (pos)
	{

		CIPRange* pRng = GetNext(pos);
		if (pRng && pRng->IsInRng(nIP))
		{
			return TRUE;
		}
	}
	return FALSE;
}


//--------------------------
//机器
CHostNameRange::CHostNameRange(NameRange* pRg)
{
	ASSERT(pRg);
	CopyMemory(this,pRg,sizeof(NameRange));
}
CHostNameRange::CHostNameRange(CStringA sPre,CStringA sB,CStringA sE)
{
	strcpy(cPrefix,sPre);
	strcpy(cBName,sB);
	strcpy(cEName,sE);
}
BOOL CHostNameRange::IsInRng(CStringA sIP)
{
	BOOL bRet = FALSE;
	ASSERT(!sIP.IsEmpty());
	do 
	{
		sIP.MakeUpper();
		::_strupr(cPrefix);
		int nIndex = sIP.Find(cPrefix);
		if (nIndex < 0)
		{
			break;
		}
		sIP = sIP.Right(sIP.GetLength() - nIndex - strlen(cPrefix));
		int nIP = atoi(sIP);

		int nBIP = atoi(cBName);
		int nEIP = atoi(cEName);
		if (nIP >= nBIP && nIP <= nEIP)
		{
			bRet = TRUE;
			break;
		}
	} while(0);
	return bRet;
}

//------------------------------
//机器列表
CHostNameRangeList::~CHostNameRangeList()
{
	Release();
}
void CHostNameRangeList::Release()
{
	POSITION old = NULL;
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		old = pos;
		CHostNameRange* pRng = GetNext(pos);
		if (pRng)
		{
			delete pRng;
		}
		RemoveAt(old);
	}
}
BOOL CHostNameRangeList::GetListData(BYTE* pData,int nLen)
{
	if (!AfxIsValidAddress(pData,nLen))
	{
		ASSERT(0);
		return FALSE;
	}

	POSITION pos = GetHeadPosition();
	NameRange* pName = (NameRange*)pData;
	int nIndex = 0;
	while (pos)
	{
		if ((nIndex + 1) * sizeof(NameRange) > nLen)
		{
			break;
		}
		CHostNameRange* pRng = GetNext(pos);
		if (pRng)
		{
			CopyMemory(pName,pRng,sizeof(NameRange));
			pName += 1;
			nIndex ++;
		}	
	}
	return TRUE;
}
BOOL CHostNameRangeList::IsConflict(CStringA sPre,CStringA sB,CStringA sE)
{
	if (IsInList(sPre + sB) || IsInList(sPre + sE))
	{
		return TRUE;
	}

	POSITION pos = GetHeadPosition();
	while (pos)
	{
		CHostNameRange* pRng = GetNext(pos);
		if (pRng )
		{
			if (sPre.CompareNoCase(pRng->cPrefix) == 0)
			{
				int nBCode = atoi(pRng->cBName);
				int nECode = atoi(pRng->cEName);
				int nCurBCode = atoi(sB);
				int nCurECode = atoi(sE);
				if ( (nCurBCode >= nBCode && nCurBCode <= nECode) ||
					(nCurECode >= nBCode && nCurECode <= nECode) ||
					(nCurBCode < nBCode && nCurECode >nECode) )
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
BOOL CHostNameRangeList::DeleteOne(CHostNameRange* pRng)
{
	POSITION pos = Find(pRng);
	if (pos)
	{
		delete pRng;
		RemoveAt(pos);
		return TRUE;
	}
	return FALSE;
}
CHostNameRange* CHostNameRangeList::AddOne(CString sPre,CString sB,CString sE)
{
	CHostNameRange* pInfo = new CHostNameRange(sPre,sB,sE);
	ASSERT(pInfo);
	if (pInfo)
	{
		AddTail(pInfo);
		return pInfo;
	}
	return NULL;
}
void CHostNameRangeList::Attach(NameRange* pRng,int nCount)
{
	ASSERT(pRng);
	if (!pRng || nCount < 1)
	{
		return ;
	}
	for (int n = 0; n < nCount ; n ++)
	{
		CHostNameRange* pInfo = new CHostNameRange(pRng);
		ASSERT(pInfo);
		if (pInfo)
		{
			AddTail(pInfo);
		}
		pRng += 1;
	}
}
BOOL CHostNameRangeList::IsInList(CString sHost)
{
	POSITION pos = GetHeadPosition();
	while (pos)
	{

		CHostNameRange* pRng = GetNext(pos);
		if (pRng && pRng->IsInRng(sHost))
		{
			return TRUE;
		}
	}
	return FALSE;
}
