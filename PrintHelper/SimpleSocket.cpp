#include "StdAfx.h"
#include "SimpleSocket.h"

CSimpleSocket::CSimpleSocket(void)
{
	InitNet();
	m_hSocket = INVALID_SOCKET;
}

CSimpleSocket::~CSimpleSocket(void)
{
	Close();
	ReleaseNet();
}

BOOL CSimpleSocket::InitNet()
{
	int iResult = NO_ERROR;

	//初始化网络环境
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != NO_ERROR)
	{
		theLog.Write("!!CSimpleSocket::InitNet,WSAStartup fail. errno=%d", WSAGetLastError());
		return FALSE;
	}

	return TRUE;
}

void CSimpleSocket::ReleaseNet()
{
	int iResult = NO_ERROR;
	iResult = WSACleanup();
	if(iResult != NO_ERROR)
	{
		theLog.Write("!!CSimpleSocket::ReleaseNet,WSACleanup fail. errno=%d", WSAGetLastError());
	}
}

BOOL CSimpleSocket::Connect(const char* pszPrinterIP, int nPrinterPort)
{
	int iResult = NO_ERROR;

	if (IsConnected())
	{
		Close();
	}

	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hSocket == INVALID_SOCKET) 
	{
		theLog.Write("!!CSimpleSocket::Connect,create socket fail. errno=%d", WSAGetLastError());
		return FALSE;
	}

	//设置IP地址重用
	int nValue = 1;
	iResult = setsockopt(m_hSocket,  SOL_SOCKET, SO_REUSEADDR, (const char*)&nValue, sizeof(nValue));

	//设置发送超时时间
	int nMillisecond = 5000;
	iResult = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&nMillisecond, sizeof(int));

	//设置接收超时时间
	iResult = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nMillisecond, sizeof(int));

	//设置发送缓冲区大小
	int nBufSize = 4096;
	iResult = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufSize, sizeof(int));

	//设置接收缓冲区大小
	iResult = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nBufSize, sizeof(int));

	//设置连接参数
	SOCKADDR_IN sockaddr; 
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.S_un.S_addr = inet_addr(pszPrinterIP);
	sockaddr.sin_port = htons(nPrinterPort);

	//开始连接打印机
	if (connect(m_hSocket, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) 
	{
		theLog.Write("!!CSimpleSocket::Connect,connect fail. errno=%d", WSAGetLastError());
		NetError();
		return FALSE;
	}

	return TRUE;
}

void CSimpleSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CSimpleSocket::SendData(const char* pData, int nDataSize)
{
	if (!pData || nDataSize<=0)
	{
		theLog.Write("!!CSimpleSocket::SendData, pData=%p,nDataSize=%d", pData, nDataSize);
		return FALSE;
	}

	int nSend = 0;
	int nRet = 0;
	while (nSend < nDataSize)
	{
		nRet = send(m_hSocket, pData+nSend, nDataSize-nSend, 0);
		if (nRet == SOCKET_ERROR)
		{
			theLog.Write("!!CSimpleSocket::SendData, send fail errno=%d", WSAGetLastError());
			NetError();
			break;
		}
		nSend += nRet;
	}
	return (nSend == nDataSize) ? TRUE : FALSE;
}

BOOL CSimpleSocket::RecvData(char* pBuf, int nBufSize)
{
	if (!pBuf || nBufSize<=0)
	{
		theLog.Write("!!CSimpleSocket::RecvData, pBuf=%p,nBufSize=%d", pBuf, nBufSize);
		return FALSE;
	}

	int nRecv = 0;
	int nRet = 0;
	while (nRecv < nBufSize)
	{
		nRet = recv(m_hSocket, pBuf+nRecv, nBufSize-nRecv, 0);
		if (nRet == SOCKET_ERROR)
		{
			theLog.Write("!!CSimpleSocket::RecvData, recv fail errno=%d", WSAGetLastError());
			NetError();
			break;
		}
		nRecv += nRet;
	}
	return (nRecv == nBufSize) ? TRUE : FALSE;
}

void CSimpleSocket::DumpData(const char* pData, int nDataSize)
{
	if (!pData || nDataSize<=0)
	{
		theLog.Write("!!CSimpleSocket::DumpData, pData=%p,nDataSize=%d", pData, nDataSize);
		return;
	}

	CString szDump;
	for (int index=0; index<nDataSize; index++)
	{
		BYTE b = pData[index];
		szDump.AppendFormat("%02X", b);
	}
	theLog.Write("CSimpleSocket::DumpData,szDump=%s", szDump);
}

void CSimpleSocket::NetError()
{
	theLog.Write("CSimpleSocket::NetError,error=%d", WSAGetLastError());
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CSimpleSocket::IsConnected()
{
	return (m_hSocket == INVALID_SOCKET) ? FALSE : TRUE;
}

