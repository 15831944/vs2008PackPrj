#pragma once

class CKeyHkUdpSock
{
public:
	CKeyHkUdpSock(void);
	virtual ~CKeyHkUdpSock(void);
	BOOL Init(char *pMultiIP, int nMultiPort, char *pLocalIP);

	BOOL IsInitOK();

	BOOL SendCardNum(BYTE *pCardNum);
protected:
	void Release();
protected:
	BOOL m_bInit;
	SOCKET m_hSocket;

	sockaddr_in m_addrDest;
	char	m_bysMultiIP[16];
	int		m_nMultiPort;
};
