#if !defined(AFX_BASESOCK_H__98F035E5_74C5_4C8A_8541_4A545D0A27D7__INCLUDED_)
#define AFX_BASESOCK_H__98F035E5_74C5_4C8A_8541_4A545D0A27D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CBaseSock command target

class CBaseSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CBaseSock();
	virtual ~CBaseSock();
	bool m_bSetTime;
	UINT m_nTimerID;

// Overrides
public:
	bool m_bTimeOut;
	BOOL KillTimeOut();
	BOOL SetTimeOut(UINT uTimeOut);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseSock)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual BOOL OnMessagePending();
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CBaseSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASESOCK_H__98F035E5_74C5_4C8A_8541_4A545D0A27D7__INCLUDED_)
