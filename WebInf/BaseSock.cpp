// BaseSock.cpp : implementation file
//

#include "stdafx.h"
#include "BaseSock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseSock

CBaseSock::CBaseSock()
{
	m_hSocket = INVALID_SOCKET;
}

CBaseSock::~CBaseSock()
{
	Close();
}

void CBaseSock::Close()
{

	if(INVALID_SOCKET != m_hSocket)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}
BOOL CBaseSock::Bind(LPCTSTR pLocal,UINT nPort /*= 0*/)
{
	ASSERT(m_hSocket);
	ASSERT(m_hSocket != INVALID_SOCKET);
	if (!m_hSocket || m_hSocket == INVALID_SOCKET)
		return FALSE;

	int nRet = 0;

	//指定ip进行连接
	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A((LPTSTR)pLocal);
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
		{
			//WSASetLastError(WSAEINVAL);
			WriteLogEx("bind ip(%s) faile ,ip is none ",pLocal);
			lResult = htonl(INADDR_ANY);
			//return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nPort);

	nRet = bind(m_hSocket,(const sockaddr *)&sockAddr, sizeof(sockAddr));
	if (nRet != SOCKET_ERROR)
	{
		return TRUE;
	}
	WriteLogEx("bind ip(%s) faile(%d)",pLocal,::WSAGetLastError());
	return FALSE;
}

BOOL CBaseSock::Connect(LPCTSTR pIP, int nPort,LPCTSTR pLocal)
{
	if(m_hSocket == INVALID_SOCKET || !m_hSocket)
	{
		if(!Create(pLocal))
			return FALSE;
	}

	ASSERT(m_hSocket);
	ASSERT(m_hSocket != INVALID_SOCKET);

	if(INVALID_SOCKET == m_hSocket || nPort<1 || !pIP)
		return false;

	int nError = 0;
	int nRet = 0;

	sockaddr_in stServAddr;
	memset(&stServAddr, 0x0, sizeof(stServAddr));
	stServAddr.sin_port = htons(nPort);
	unsigned long ulAddr = inet_addr(pIP);
	if (ulAddr == INADDR_NONE)
	{
		//添加对域名解析IP的操作
		ulAddr = inet_addr(GetIpFromHostname(pIP));
		if (ulAddr == INADDR_NONE)
		{
			WriteLogEx("!!CBaseSock::Connect,ServIP=%s,Port=%d,GetIpFromHostname fail", pIP, nPort);
		}
	}
	stServAddr.sin_addr.s_addr = ulAddr;
	stServAddr.sin_family = AF_INET;


#if 0
	//套接字是阻塞的
	nRet = connect(m_hSocket, (const sockaddr*)&stServAddr, sizeof(stServAddr));
	if(SOCKET_ERROR == nRet)
	{
		int nError = GetLastError();
		return FALSE;
	}
	return TRUE;
#else
	//套接字是非阻塞的
	while( (nRet = connect(m_hSocket, (const sockaddr*)&stServAddr, sizeof(stServAddr)) ) == SOCKET_ERROR  )
	{
		nError = WSAGetLastError();
		if(WSAEWOULDBLOCK == nError)
		{
			Sleep(5);
			continue;
		}
		else if(WSAEISCONN == nError)
		{//只有这里才表明已经连接上
			nRet = 0;
			break;
		}
		else
		{//其他错误
			//TODO: this指针无效后走到这里会报错
			WriteLogEx("In CBaseSock::Connect(),ip = %s,port = %d,Connect err=%d m_hSocket:0x%x\n",pIP,nPort, nError, m_hSocket);
			nRet = -1;
			break;
		}
	}//while
#endif
	if(nRet == 0)
		return TRUE;

	return FALSE;
}
BOOL CBaseSock::SetSockDefaultParam()
{
	ASSERT(INVALID_SOCKET != m_hSocket);
	return 1;
	BOOL bRet = FALSE;
	int nBufSize = 1024*64;
#if 0
	do 
	{
		if(!SetReuseAddr())
			break;

		if(!SetRcvTimeOut(8*1000))
			break;

		if(!SetSndTimeOut(8*1000))
			break;

		if(!SetRcvBufSize(nBufSize))
			break;

		if(!SetSndBufSize(nBufSize))
			break;
		bRet = TRUE;

	} while (0);
	
#endif	

/*
	//工作到这里,测试禁用nagle算法后效率能提高多少
	BOOL BValue = TRUE;
	if(setsockopt(m_hSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&BValue,sizeof(BOOL)) < 0)
	{
		ASSERT(0);
		m_oLog.Write("setsockopt IPPROTO_TCP TCP_NODELAY Error:[%d]",GetLastError());
		return false;
	}
*/

	
	return bRet;
}


BOOL CBaseSock::Create(LPCTSTR pLocal /*= NULL*/,UINT nPort /*= 0*/)
{
	ASSERT(m_hSocket == INVALID_SOCKET);
	if(m_hSocket != INVALID_SOCKET)
		return TRUE;
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == m_hSocket)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		WriteLogEx("In CBaseSock::InitSock(),socket err=%d\n", WSAGetLastError());
		return FALSE;
	}

	if(!SetSockDefaultParam())
	{
		WriteLogEx(_T("SetSockDefaultParam err"));
		return FALSE;
	}

	if(!Bind(pLocal,nPort))
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		WriteLogEx("In CBaseSock::Bind(),socket err=%d\n", WSAGetLastError());
		return FALSE;
	}

	return TRUE;
}
int CBaseSock::Send(BYTE* pData,int nLen /*= 0*/)
{
	return Send(m_hSocket,pData,nLen);
}
int CBaseSock::Send(SOCKET s ,BYTE* pData,int nLen /*= 0*/)
{
	if(s == NULL || s == INVALID_SOCKET)
	{
		ASSERT(0);
		return FALSE;
	}
// 	int nHeadLen = sizeof(NET_PACK_HEAD);
// 	NET_PACK_HEAD stHead;
// 	memset(&stHead, 0x0, nHeadLen);
// 	stHead.nIndentify = PACKINDENTIFY;
// 	stHead.nMainCmd = nCmd;
// 	stHead.nSubCmd = nSub;
// 	stHead.nPackMask = PACK_SMALL;
// 
// 
// 	BYTE* pData = (BYTE*)(&stHead);
// 	int nLen = sizeof(NET_PACK_HEAD);
	int nTotal = 0;
	int nErrCount = 0;
	while(1)
	{
		int nRet = send(s, (char*)pData+nTotal,nLen-nTotal,0);
		if(nRet <= 0)
		{
			int nErrno = WSAGetLastError();
			if(WSAEWOULDBLOCK == nErrno)
			{
				nErrCount ++;
				Sleep(1);
				if (nErrCount > 5)
				{
					WriteLogEx("CBaseSock::Send Err = %d,count = %d",::WSAGetLastError(),nErrCount);
					break;
				}
				continue;
			}
			else
				return nRet;
		}
		nTotal += nRet;
		if(nTotal >= nLen)
			return nLen;
	}
	return nLen;
}

CString CBaseSock::GetIpFromHostname(CString szHostName)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1,1),&wsaData);

	CString strIP = _T("");   
	struct hostent *host;   
	struct in_addr *ptr;   

	host = gethostbyname(szHostName.GetString());   
	if(host == NULL)
	{
		WriteLogEx("!!CBaseSock::GetIpFromHostname fail, szHostName=%s,err=%d",szHostName, WSAGetLastError());
	}
	else
	{
		ptr = (struct in_addr*) host->h_addr_list[0];  

		int a = ptr->S_un.S_un_b.s_b1;
		int b = ptr->S_un.S_un_b.s_b2;
		int c = ptr->S_un.S_un_b.s_b3;
		int d = ptr->S_un.S_un_b.s_b4;
		strIP.Format(_T("%d.%d.%d.%d"),a,b,c,d); 
		WriteLogEx("CBaseSock::GetIpFromHostname succ, hostname=%s, ip=%s",szHostName,strIP);
	}
	WSACleanup(); //调用WSACleanup函数进行WinSock的清理工作，以便释放其占用的资源
	return strIP;
}

BOOL CBaseSock::RecvUtilCmd( DWORD dwCmd, DWORD dwTimeoutMs )
{
    int HeadLen = sizeof(NET_PACK_HEAD);
    NET_PACK_HEAD* pHead = NULL;
    int nBodySzie = 0;

    DWORD dwTickBegin = ::GetTickCount();
    DWORD dwTickCur = 0;
    int nRec = 0;
    DWORD dwNewBufLen = 0;
    BYTE *pNewBuf = NULL;
    DWORD dwBufLen = 4096;
    BYTE *pSockData = new BYTE[dwBufLen];
    if(!pSockData)
    {
        WriteLogEx("!!CBaseSock::RecvUntilCmd,1,new[%d] fail,err=%d,%s", dwBufLen, errno, strerror(errno));
        return FALSE;
    }
    pHead = (NET_PACK_HEAD*)pSockData;
    do
    {
        dwTickCur = ::GetTickCount();
        if(dwTimeoutMs < dwTickCur - dwTickBegin)
        {
            WriteLogEx("!!CBaseSock::RecvUntilCmd,1.2,TimeOUT,dwCmd=%d", dwCmd);
            goto ERR1;
        }
/*
        if(0 >= GetBufferDataLen())
        {
            Sleep(10);
            continue;
        }*/

        nRec = ReceiveData(pSockData, HeadLen);
        if(0 >= nRec)
        {
            WriteLogEx(_T("!!CBaseSock::RecvUntilCmd,2,ReceiveData fail,nRec=%d,err=%d,%s"), nRec, errno, strerror(errno));
            goto ERR1;
        }

        pHead = (NET_PACK_HEAD*)pSockData;

        if(pHead->nIndentify != PACKINDENTIFY)
        {
            WriteLogEx(_T("!!CBaseSock::RecvUntilCmd,3,invalid pack,%d,%d,%d\n"), pHead->nIndentify, PACKINDENTIFY, pHead->nPackBodySize);
            goto ERR1;
        }

        if(dwCmd == pHead->nMainCmd)
        {//已经收到指定的命令了
            WriteLogEx("CBaseSock::RecvUntilCmd,3.2,RECV OK,dwCmd=%d", dwCmd);
            break;
        }

        //接收完整
        if (pHead->nPackBodySize == 0)
        {
            Sleep(5);
            continue;
        }
        nBodySzie = pHead->nPackBodySize;
        if (nBodySzie + HeadLen > dwBufLen)
        {
            dwNewBufLen = nBodySzie + HeadLen + 1024;
            pNewBuf = new BYTE[dwBufLen];
            if(!pNewBuf)
            {
                WriteLogEx(_T("!!CBaseSock::RecvUntilCmd,4,new[%d] fail,nRec=%d,err=%d,%s"), dwBufLen, nRec, errno, strerror(errno));
                goto ERR1;
            }
            memcpy(pNewBuf, pSockData, dwBufLen);
            delete []pSockData;
            pSockData = pNewBuf;
            dwBufLen = dwNewBufLen;
            pHead = (NET_PACK_HEAD*)pSockData;
        }

        nRec = ReceiveData(pSockData + HeadLen, nBodySzie);
        if(0 >= nRec)
        {
            WriteLogEx(_T("!!CBaseSock::RecvUntilCmd,5,ReceiveData fail,nRec=%d,err=%d,%s"), nRec, errno, strerror(errno));
            goto ERR1;
        }

        Sleep(5);
    }while(1);

    delete []pSockData;
    return TRUE;
ERR1:
    delete []pSockData;
    return FALSE;
}

int CBaseSock::ReceiveData( BYTE* pData,int nLen )
{
    if(!m_hSocket)
    {
        WriteLogEx("!!CWBaseSock::ReceiveData,1,m_hSocket=%d", m_hSocket);
        return 0;
    }

    DWORD dwStart = ::GetTickCount();
    int nRet = 0;
    int nTotal = 0;
    while(nTotal< nLen )
    {
        nRet = recv(m_hSocket, (char*)pData+nTotal, nLen-nTotal, 0);
        if( nRet < 0 )
        {
            /*
            int nErrno = errno;
            if(EWOULDBLOCK != nErrno && EINTR != nErrno)//add in linux
            {
                theLog.Write("!!CWBaseSock::ReceiveData,2,nErrno=%d,EWOULDBLOCK=%d,EAGAIN=%d,%s"
                    , nErrno, EWOULDBLOCK, EAGAIN, strerror(nErrno));
                return nRet;
            }
            else
            {
                ::Sleep(1000);
            }
            */
            DWORD dwSpan = ::GetTickCount() - dwStart;
            if (3000 < dwSpan)
            {
                WriteLogEx("!!CWBaseSock::ReceiveData,3,TimeOUT %d", dwSpan);
                return -1;
            }
        }
        else if(0 == nRet)
            return nRet;
        else
            nTotal += nRet;
    }
    return nLen;
}