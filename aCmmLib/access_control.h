/* ==============================================================================================================================
 * This notice must be untouched at all times.
 *
 * Copyright  IntelliWizard Inc. 
 * All rights reserved.
 * LICENSE: LGPL. 
 * Redistributions of source code modifications must send back to the Intelliwizard Project and republish them. 
 * Web: http://www.intelliwizard.com
 * eMail: info@intelliwizard.com
 * We provide technical supports for UML StateWizard users.
 * ==============================================================================================================================*/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECURITYFUNC_H__158BAEEB_7A63_43C4_BD3E_813E5EDF3FA1__INCLUDED_)
#define AFX_SECURITYFUNC_H__158BAEEB_7A63_43C4_BD3E_813E5EDF3FA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef BOOL (WINAPI fn_ConvertStringSecurityDescriptorToSecurityDescriptorW)(
																			   LPCWSTR,          // security descriptor string
																			   DWORD,                    // revision level
																			   PSECURITY_DESCRIPTOR*,  // SD
																			   PULONG                   // SD size
																			   );
typedef BOOL (fnVisitFunc)(const char* strFile,void* para);
class _AA_DLL_EXPORT_ XAccessControl  
{
public:
	static BOOL mbSecLowSAInit;
	static BOOL mbSecSAInit;
	static BOOL mbSecSDInit;
	static BOOL mbSecNameEnabled;

	static SECURITY_ATTRIBUTES* getHighPrivSA();
	static SECURITY_DESCRIPTOR* getHighPrivSD();

	//need user to free it outside by LocalFree
	static SECURITY_ATTRIBUTES* getLowPrivSA();

	static void GoThroughDir(const char* strPath,fnVisitFunc* visit,void* para);
	static BOOL SetPrivilege(	HANDLE hToken,          // access token handle
						LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
						BOOL bEnablePrivilege   // to enable or disable privilege
						) ;
	static BOOL EnableSecurityPriv();
	static fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* GetSecurityFuncAddress();
	static BOOL SetFileSecurityInDir(const char* szDirFileName,BOOL bRecursive=TRUE);
	static BOOL SetSingleFileSecurity(const char* szFileName,void* psd=NULL);
};

#endif // !defined(AFX_SECURITYFUNC_H__158BAEEB_7A63_43C4_BD3E_813E5EDF3FA1__INCLUDED_)
