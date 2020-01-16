// IIS.h: interface for the CIIS class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_IIS_H__11B61858_60DD_4429_AFD2_29DE4C6BA243__INCLUDED_)
#define AFX_IIS_H__11B61858_60DD_4429_AFD2_29DE4C6BA243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <lm.h>
#include "iads.H"
#include "Adshlp.h"
#pragma comment(lib,"Netapi32.lib")
#pragma comment(lib,"adsiid.lib")
#pragma comment(lib,"activeds.lib")

#pragma	pack (push,1)
// Site info
typedef struct S_SiteInfo
{
	int		nNumber;//ÐòºÅ
	char 	szPort[8];	//¶Ë¿Ú
	char	szIP[40]; //IP
	char	szComment[256]; // Comment
	char	szVPath[256];	//ÐéÄâÂ·¾¶
}	S_SITEINFO,*P_S_SITEINFO;

#pragma	pack (pop)

class _AA_DLL_EXPORT_ CIIS  
{
public:
	CIIS();
	virtual ~CIIS();
public:
	BOOL GrantRightToUser(CString szDir);
	BOOL AddAccessRights(LPCTSTR lpszFileName,LPCTSTR szUserName,DWORD dwAccessMask);
	bool GetAnonymousUserName(LPSTR pUserName);
	BOOL InitIISAnonymousSA(LPSECURITY_ATTRIBUTES pSA);

	DWORD AddGroupMembers();
	DWORD AddMachineAccount();
	BOOL AddOperator();

	CString GetServerBinding(VARIANT FAR* vArray);

	void RemoveAllList(CPtrList &pList);


};

#endif // !defined(AFX_IIS_H__11B61858_60DD_4429_AFD2_29DE4C6BA243__INCLUDED_)
