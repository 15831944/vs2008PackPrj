//ʵ��һ���򵥵�Windows Socket�����࣬����TCP���ݵļ򵥷��ͺͽ���

#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class CSimpleSocket
{
public:
	CSimpleSocket(void);
	~CSimpleSocket(void);

	BOOL Connect(const char* pszIP, int nPort);
	void Close();
	BOOL SendData(const char* pData, int nDataSize);
	BOOL RecvData(char* pBuf, int nBufSize);
	void DumpData(const char* pData, int nDataSize);

	BOOL IsConnected();

protected:
	BOOL InitNet();
	void ReleaseNet();

	void NetError();

protected:
	SOCKET m_hSocket;
};
