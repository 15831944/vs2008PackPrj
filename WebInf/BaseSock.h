#if !defined(AFX_BASESOCK_H__98F035E5_74C5_4C8A_8541_4A545D0A27D7__INCLUDED_)
#define AFX_BASESOCK_H__98F035E5_74C5_4C8A_8541_4A545D0A27D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CBaseSock command target
class CBaseSock //: public CSocket
{
// Attributes
public:
	BOOL Create(LPCTSTR pLocal = NULL,UINT nPort = 0);						//初始化socket
	BOOL Bind(LPCTSTR pLocal,UINT nPort = 0);
	BOOL Connect(LPCTSTR pIP, int nPort,LPCTSTR pLocal = NULL);	//连接服务器socket
	BOOL SetSockDefaultParam();
	void Close();
	int Send(BYTE* pData,int nLen /*= 0*/);
	BOOL RecvUtilCmd(DWORD dwCmd, DWORD dwTimeoutMs);
	//ULONG GetBufferDataLen();
	int ReceiveData(BYTE* pData,int nLen);
// Operations
public:
	CBaseSock();
	virtual ~CBaseSock();
protected:
	int Send(SOCKET s ,BYTE* pData,int nLen /*= 0*/);
	CString GetIpFromHostname(CString szHostName);	//add by zxl,20160717
	SOCKET m_hSocket;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //!defined(AFX_BASESOCK_H__98F035E5_74C5_4C8A_8541_4A545D0A27D7__INCLUDED_)
