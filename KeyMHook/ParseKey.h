#pragma once

typedef void (WINAPI *PFN_OnReadCardByKeyHook)(char *pCardNum, void *pContext);

class CKeyHkUdpSock;

//解析铁匠的
class CParseKey
{
public:
	CParseKey(void);
	virtual ~CParseKey(void);

	void SaveKey(WPARAM wp);

//	void OnReadCardByKeyBoard(CString &szCardNum);
	void OnReadCardByKeyBoard(BYTE *pszCardNum);

	void SetCallBack(PFN_OnReadCardByKeyHook pfn, void *pContext);
protected:
	BOOL IsThisKeyValid(WPARAM wp);
	BOOL IsEndFlag(WPARAM wp);
protected:
//	CString		m_szKeyCard;
	BYTE		*m_pKeyCard;
	DWORD		m_dwTickCountLastInput;
	DWORD		m_dwMaxSpan;	//刷卡器模拟键盘输入，两次输入的最大间隔毫秒数
	DWORD		m_dwMaxCount;	//卡号的最大位数
	BOOL		m_bFirst;		//是否是第一个

	PFN_OnReadCardByKeyHook m_pfnOnReadCard;
	void *		m_pContext;
	CKeyHkUdpSock *m_pUdpSock;
};
