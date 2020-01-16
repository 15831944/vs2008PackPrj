// GetBootInfo.h : header file

#if !defined(AFX_GETBOOTINFO_H__45E2413F_3F80_11D4_92EE_009027E66C24__INCLUDED_)
#define AFX_GETBOOTINFO_H__45E2413F_3F80_11D4_92EE_009027E66C24__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_ABINFO_NOTIFY_OK	WM_USER+100 // wparam = user sum

class _MSXAI_PORT_ CGetBootInfo : public CObject
{
// Construction
public:
	CGetBootInfo();
	virtual ~CGetBootInfo();
// Attributes
public:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetBootInfo)
	//}}AFX_VIRTUAL
// Implementation
public:
	DWORD GetMtdID(DWORD dPtr);
	DWORD GetNtyID(DWORD dHwnd);
	int   Method1(DWORD dMtdID);
	int	  Method2(DWORD dNtyID,DWORD dMtdID);
	int	  Method3(DWORD dNtyID,DWORD dMtdID);
	// Generated message map functions
protected:
	//{{AFX_MSG(CGetBootInfo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_GETBOOTINFO_H__45E2413F_3F80_11D4_92EE_009027E66C24__INCLUDED_)
