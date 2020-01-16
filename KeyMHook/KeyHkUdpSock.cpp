#include "stdafx.h"
#include "KeyHkUdpSock.h"
#include <Winsock2.h>	//for IP_TTL

extern void WriteLogExKH(LPCTSTR lpszFormat, ...);

CKeyHkUdpSock::CKeyHkUdpSock(void)
{
	m_bInit = FALSE;
	m_hSocket = NULL;
	memset(&m_addrDest, 0x0, sizeof(m_addrDest));

	memset(m_bysMultiIP, 0x0, sizeof(m_bysMultiIP) / sizeof(m_bysMultiIP[0]) );
	m_nMultiPort = 0;
}

CKeyHkUdpSock::~CKeyHkUdpSock(void)
{
	Release();
}

BOOL CKeyHkUdpSock::Init(char *pMultiIP, int nMultiPort, char *pLocalIP)
{
	m_bInit = FALSE;
	if(!pMultiIP || 0 >= nMultiPort || !pLocalIP)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,1,pMultiIP=%s,nMultiPort=%d,pLocalIP=%s", pMultiIP, nMultiPort, pLocalIP);
		return FALSE;
	}
	m_hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	
	if(m_hSocket==INVALID_SOCKET)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,2,socket fail,err=%d,pMultiIP=%s,nMultiPort=%d,pLocalIP=%s"
			, GetLastError(), pMultiIP, nMultiPort, pLocalIP);
		return FALSE;
	}

	//Set Reuse Addr
	BOOL bValue=1;
	int nRet = 0;
	nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&bValue, sizeof(BOOL));
	if(nRet == SOCKET_ERROR)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,2.1,setsockopt SO_REUSEADDR Error:[%d]",GetLastError());
		//		return FALSE;
	}

#if 0
	//set ttl
	DWORD nTTL = 16;
	nRet = setsockopt(m_hSocket, IPPROTO_IP, IP_TTL, (char *)&nTTL, sizeof(nTTL));   
	if(nRet == SOCKET_ERROR)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,2.12,setsockopt IP_TTL Error:[%d]",GetLastError());
	}
#endif

#if 0
	//Set Send Buf
	int nBufSize = 0;
	nRet = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char *)&nBufSize,sizeof(int));
	if(nRet==SOCKET_ERROR)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,2.2,setsockopt SO_REUSEADDR Error:[%d]",GetLastError());
	}
#endif


	//设置loopback
	int nLoopback = 1;
	DWORD dwRet = 0;
	nRet = WSAIoctl(m_hSocket, SIO_MULTIPOINT_LOOPBACK, &nLoopback,sizeof(nLoopback), NULL,0,&dwRet,NULL,NULL);
	if(SOCKET_ERROR == nRet)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,2.2,setsockopt loopback Error:[%d]",GetLastError());
	}

#if 0
	sockaddr_in binda;
	//然后赋值给两个地址，一个用来绑定套接字，一个用来从网络上的广播地址接收消息；
	binda.sin_family = AF_INET;
	binda.sin_addr.s_addr = inet_addr(pLocalIP);//所有地址都接收

	BOOL bBindOK = FALSE;
	for(int i = 0; i < 200; i++)
	{
		binda.sin_port = htons(nMultiPort + 1 + i); 
		if(SOCKET_ERROR != bind(m_hSocket, (sockaddr *)&binda, sizeof(binda)))
		{
			bBindOK = TRUE;
			break;
		}
	}

	if(!bBindOK)
	{
		WriteLogExKH("!!CKeyHkUdpSock::Init,8,bind fail,err=%d,pMultiIP=%s,nMultiPort=%d,pLocalIP=%s"
			, GetLastError(), pMultiIP, nMultiPort, pLocalIP);
		::closesocket(m_hSocket);
		m_hSocket  = NULL;
		return FALSE;
	}

#endif

	m_addrDest.sin_family = AF_INET;
//	m_addrDest.sin_addr.s_addr = ::inet_addr(pMultiIP);
	m_addrDest.sin_addr.s_addr = ::inet_addr(pLocalIP);	//用UDP
	m_addrDest.sin_port = htons(nMultiPort);
	strcpy(m_bysMultiIP, pLocalIP);
	m_nMultiPort = nMultiPort;

	m_bInit = TRUE;
	return TRUE;
}

void CKeyHkUdpSock::Release()
{
	if(m_hSocket)
	{
		closesocket(m_hSocket);
		m_hSocket = NULL;
	}
	m_bInit = FALSE;
}

BOOL CKeyHkUdpSock::IsInitOK()
{
	return m_bInit;
}

#define DEF_Use_AckInsteadSleep 1

BOOL CKeyHkUdpSock::SendCardNum(BYTE *pCardNum)
{
	if(!m_bInit || !pCardNum || !m_hSocket)
	{
		WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,1,m_bInit=%d,pCardNum=0x%x,m_hSocket=0x%x", m_bInit, pCardNum, m_hSocket);
		return FALSE;
	}

//#ifdef DEF_Use_AckInsteadSleep
#if 0
	unsigned   long   ul   =   0;	//设置为阻塞
//	unsigned   long   ul   =   1;	//设置为非阻塞 //设置为非阻塞模式发，对端收不到数据
	int   ret   =   ioctlsocket(m_hSocket,   FIONBIO,   (unsigned   long*)&ul);   
	if(ret   ==   SOCKET_ERROR)   
	{   
		WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,1.1,pCardNum=%s,m_hSocket=0x%x,ioctlsocket fail,err=%d", pCardNum, m_hSocket, GetLastError());
	}
#endif

	int nLen = strlen((char*)pCardNum) + 1;
	sockaddr_in stAddrDest;
	memcpy(&stAddrDest, &m_addrDest, sizeof(stAddrDest));
	int nRet = sendto(m_hSocket, (char*)pCardNum, nLen, 0, (sockaddr *)&stAddrDest, sizeof(stAddrDest));

	WriteLogExKH("##CKeyHkUdpSock::SendCardNum,1.2,sendto nRet=%d,pCardNum=%s,Err=%d", nRet, pCardNum, GetLastError());

#ifndef DEF_Use_AckInsteadSleep
	Sleep(100);
#else
	Sleep(5);
#endif

#ifndef DEF_Use_AckInsteadSleep
	nRet = sendto(m_hSocket, (char*)pCardNum, nLen, 0, (sockaddr *)&stAddrDest, sizeof(stAddrDest));
	Sleep(150); //要等到一段时间，不然数据只是投递到底层，然后此dll被卸载，socket关闭，数据就不会发出去
#else
	Sleep(5);
#endif
//	nRet = sendto(m_hSocket, (char*)pCardNum, nLen, 0, (sockaddr *)&stAddrDest, sizeof(stAddrDest));
	if(nRet != nLen)
	{
		WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,2,sendto fail,len=%d,nRet=%d,err=%d"
			, nLen, nRet, GetLastError());
		nRet = sendto(m_hSocket, (char*)pCardNum, nLen, 0, (sockaddr *)&stAddrDest, sizeof(stAddrDest));
		Sleep(100);
//		return FALSE;
	}

	//====================等待对端发确认信息,begin
#ifdef DEF_Use_AckInsteadSleep
#if 0
	ul   =   1;	//设置为非阻塞
	ret   =   ioctlsocket(m_hSocket,   FIONBIO,   (unsigned   long*)&ul);   
	if(ret   ==   SOCKET_ERROR)   
	{   
		WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,2.1,pCardNum=%s,m_hSocket=0x%x,ioctlsocket fail,err=%d", pCardNum, m_hSocket, GetLastError());
		return TRUE;
	}
#endif

	char buf[512] = {0};
	SOCKADDR stAddrFrom = {0x0};
	int nAddrLen = sizeof(stAddrFrom);

#if 0
	for(int i = 0; i < 25; i++)	//最长250MS
	{
		WriteLogExKH("##CKeyHkUdpSock::SendCardNum,4.%02d,A,LastErr=%d", i, GetLastError());
		MSG_OOB
		int nRet = recvfrom(m_hSocket, buf, 512, 0, &stAddrFrom, &nAddrLen);
		if(SOCKET_ERROR == nRet)
		{
			nRet = sendto(m_hSocket, (char*)pCardNum, nLen, 0, (sockaddr *)&stAddrDest, sizeof(stAddrDest));
//			WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,3,recvfrom fail,err=%d", GetLastError());
			Sleep(10);
			continue;
		}

		WriteLogExKH("##CKeyHkUdpSock::SendCardNum,4.%02d,recvfrom OK,buf=%s", i, buf);
		break;
	}
#else	//用select超时的方法收数据
	/* set recvfrom from server timeout */
	for(int i = 0; i < 3; i++)
	{
		BOOL bGet = FALSE;
		struct timeval tv;
		fd_set readfds;
		tv.tv_sec = 0;
		tv.tv_usec = 100000;//100MS，这里的usec是微秒
		FD_ZERO(&readfds);
		FD_SET(m_hSocket, &readfds);
		if (select(m_hSocket+1, &readfds,NULL, NULL, &tv) > 0)
		{
			nRet = recvfrom(m_hSocket, buf, 512, 0, &stAddrFrom, &nAddrLen);
			if (SOCKET_ERROR == nRet)
			{
				WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,23,i=%d,recvfrom fail,nRet=%d,err=%d", i, nRet, GetLastError());
			}
			else
			{
				WriteLogExKH("##CKeyHkUdpSock::SendCardNum,24,recvfrom OK,buf=%s", buf);
				bGet = TRUE;
				break;
			}
		}
		else
		{
			WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,25,i=%d,recvfrom timeout", i);
		}

		///没收到确认，再发一遍
		nRet = sendto(m_hSocket, (char*)pCardNum, nLen, 0, (sockaddr *)&stAddrDest, sizeof(stAddrDest));
		if(0 >= nRet)
		{
			WriteLogExKH("!!CKeyHkUdpSock::SendCardNum,26,i=%d,sendto fail,err=%d", i, GetLastError);
		}
	}//for

#endif

#endif
	//====================等待对端发确认信息,begin


	return TRUE;
}