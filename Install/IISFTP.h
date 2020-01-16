// IISFTP.h: interface for the CIISFTP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IISFTP_H__F558BE80_C42B_4344_85B2_C07CB23B3261__INCLUDED_)
#define AFX_IISFTP_H__F558BE80_C42B_4344_85B2_C07CB23B3261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IIS.h"
#include "CommonFun.h"

class  CIISFTP : public CIIS  
{
public:
	CIISFTP();
	virtual ~CIISFTP();
public:
	BOOL SetFTPProperty(int nIndex,CString szName,BOOL bValue);
	BOOL SetFtpSitePath(int nIndex,CString sPath);
	void GetFtpSitePath(CString sIISPath, CString &sVPath);
	void GetFtpSitePath(int nNum,CString &sVPath);
	void GetFtpSiteList(CPtrList *pList);

	BOOL CreateVirtualFtpDir(LPCTSTR sIISPath,LPCTSTR sDiskPath,LPCTSTR sVirName);
	BOOL CreateVirtualFtpDir(int nSvIndex,LPCTSTR sDiskPath,LPCTSTR sVirName);
	BOOL CreateFtpSite(LPCTSTR sComment, LPCTSTR sPath,int nPort,BOOL bStart=TRUE);


	CString GetFtpBinding(int nFtpNum);
	BOOL DeleteFtpSite(int nIndex);
	BOOL IsVirtualDirExist(LPCTSTR sVirDir,int nSvIndex);
	BOOL IsVirtualDirExist(LPCTSTR sIISPath,LPCTSTR sVirDir);

private:
	CCommonFun m_fun;
	int GetUnusedFtpNum();

};

#endif // !defined(AFX_IISFTP_H__F558BE80_C42B_4344_85B2_C07CB23B3261__INCLUDED_)
