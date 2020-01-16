// IISWeb.h: interface for the CIISWeb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IISWEB_H__6D9570B0_B2C1_474D_9DE0_F55791A0849D__INCLUDED_)
#define AFX_IISWEB_H__6D9570B0_B2C1_474D_9DE0_F55791A0849D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IIS.h"
#include "CommonFun.h"
class  CIISWeb : public CIIS  
{
public:
	CIISWeb();
	virtual ~CIISWeb();
public:
	void StartWebSite(int nIndex);
	BOOL DeleteVirtualDir(LPCTSTR sVirDir,int nSvIndex);
	BOOL SetWebDirWriteAccess(int nIndex,CString sVName,CString sDir);
	BOOL SetWebSiteDefaultDoc(int nIndex, CString sDoc);
	BOOL SetWebSiteDefaultDoc(CString sIISPath, CString sDoc);
	BOOL SetWebSitePath(int nIndex, CString sPath);
	void GetWebSiteList(CPtrList *pList);
	void GetWebSitePath(int nWebNum,CString &sVPath);
	void GetWebSitePath(CString sIISPath,CString &sVPath);
	CString GetWebBinding(int nWebNum);

	void GetVirtualWebDirPath(int nWebNum,CString sVDir,CString &sVPath);
	BOOL CreateVirtualWebDir(int nSvIndex,LPCTSTR sDiskPath,LPCTSTR sVirName,LPCTSTR sUserName=NULL,LPCTSTR sPwd=NULL);
	BOOL CreateVirtualWebDir(LPCTSTR sIISPath,LPCTSTR sDiskPath,LPCTSTR sVirName,LPCTSTR sUserName=NULL,LPCTSTR sPwd=NULL);

	BOOL CreateWebSite(LPCTSTR sComment,LPCTSTR sVirName,LPCTSTR sPath,int nPort,BOOL bStart=TRUE);

	BOOL IsVirtualDirExist(LPCTSTR sVirDir,int nSvIndex);
	BOOL IsVirtualDirExist(LPCTSTR sIISPath,LPCTSTR sVirDir);
	BOOL DeleteWebSite(int nIndex);

	void CreateAdminLnk(CString szSerIp = "", CString szSerPort = "80");
	void RemoveAdminLnk();
	
    void CreateEmbededWebLnk(CString szSerIp = "", CString szSerPort = "80", CString szWebName = "AppWeb");
    void RemoveEmbededWebLnk(CString szWebName);
private:
	CCommonFun m_fun;
	int GetUnusedWebNum();


};

#endif // !defined(AFX_IISWEB_H__6D9570B0_B2C1_474D_9DE0_F55791A0849D__INCLUDED_)
