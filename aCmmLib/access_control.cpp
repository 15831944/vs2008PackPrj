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
#include "StdAfx.h"
#include "access_control.h"


// SecurityFunc.cpp: implementation of the XAccessControl class.
//
//////////////////////////////////////////////////////////////////////

#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* static */
BOOL XAccessControl::mbSecLowSAInit = FALSE;
BOOL XAccessControl::mbSecSAInit = FALSE;
BOOL XAccessControl::mbSecSDInit = FALSE;
BOOL XAccessControl::mbSecNameEnabled = FALSE;
fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfnConvertStringSecurityDescriptorToSecurityDescriptorW=NULL;
#ifndef SDDL_REVISION_1
#define SDDL_REVISION_1     1
#endif


SECURITY_ATTRIBUTES* XAccessControl::getLowPrivSA()
{
	static SECURITY_ATTRIBUTES gsLowSA;
	if(!mbSecLowSAInit)
	{
		gsLowSA.nLength = sizeof(SECURITY_ATTRIBUTES);
		gsLowSA.bInheritHandle = FALSE;

		/* Example
		TCHAR * szSD = TEXT("D:")       // Discretionary ACL
			TEXT("(D;OICI;GA;;;BG)")     // Deny access to built-in guests
			TEXT("(D;OICI;GA;;;AN)")     // Deny access to anonymous logon
			TEXT("(A;OICI;GRGWGX;;;AU)") // Allow read/write/execute to authenticated users
			TEXT("(A;OICI;GA;;;BA)");    // Allow full control to administrators
		*/
		
		wchar_t * wszSD = L"D:"			//Discretionary ACL
			L"(A;OICI;GAFA;;;BA)"		// Allow full control to administrators
			L"(A;OICI;GAFA;;;SY)"		// Allow full control to SYSTEM
			L"(A;OICI;GRGWGX;;;AU)"; // Allow read/write/execute to authenticated users

		fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfn=GetSecurityFuncAddress();
		if(pfn){
			mbSecLowSAInit = pfn(
				wszSD,
				SDDL_REVISION_1,
				&gsLowSA.lpSecurityDescriptor,
				NULL);
		}else{
			gsLowSA.lpSecurityDescriptor=NULL;
		}
	}
	return &gsLowSA;
}

SECURITY_ATTRIBUTES* XAccessControl::getHighPrivSA()
{
	static SECURITY_ATTRIBUTES gsHighSA;
	if(!mbSecSAInit)
	{
		gsHighSA.nLength = sizeof(SECURITY_ATTRIBUTES);
		gsHighSA.bInheritHandle = FALSE;
		
		/*
		The format is a null-terminated string with tokens to indicate each of the four main components of a security 
		descriptor: owner (O:), primary group (G:), DACL (D:), and SACL (S:).
		*/
		wchar_t * wszSD = L"D:"			//Discretionary ACL
			L"(A;OICI;GAFA;;;BA)"		// Allow full control to administrators
			L"(A;OICI;GAFA;;;SY)";		// Allow full control to SYSTEM
		fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfn=GetSecurityFuncAddress();
		if(pfn){
			mbSecSAInit = pfn(
				wszSD,
				SDDL_REVISION_1,
				&gsHighSA.lpSecurityDescriptor,
				NULL);
		}else{
			gsHighSA.lpSecurityDescriptor=NULL;
		}

	}
	return &gsHighSA;
}

SECURITY_DESCRIPTOR* XAccessControl::getHighPrivSD()
{
	static LPVOID psd=NULL;
	wchar_t * wszSD = L"D:"			//Discretionary ACL
		L"(A;OICI;GAFA;;;BA)"		// Allow full control to administrators
		L"(A;OICI;GAFA;;;SY)";		// Allow full control to SYSTEM
	if(!mbSecSDInit)
	{
		DWORD nSize;
		fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfn=GetSecurityFuncAddress();
		if(pfn){
			mbSecSDInit=pfn(wszSD,SDDL_REVISION_1,&psd,&nSize);	
		}
		else{
			return NULL;
		}

	}
	return (SECURITY_DESCRIPTOR*)psd;
}

BOOL XAccessControl::SetPrivilege(
						 HANDLE hToken,          // access token handle
						 LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
						 BOOL bEnablePrivilege   // to enable or disable privilege
						 ) 
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	
	if ( !LookupPrivilegeValue( 
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid ) ) {      // receives LUID of privilege
		return FALSE; 
	}
	
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;
	
	// Enable the privilege or disable all privileges.
	
	BOOL bRet=AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL); 
	
	// Call GetLastError to determine whether the function succeeded.
	
	if (!bRet || GetLastError() != ERROR_SUCCESS) { 
		return FALSE; 
	} 
	
	return TRUE;
}

/*
The ConvertStringSecurityDescriptorToSecurityDescriptor function converts a string-format security descriptor into 
a valid, functional security descriptor. This function retrieves a security descriptor that 
the ConvertSecurityDescriptorToStringSecurityDescriptor function converted to string format.
*/
fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* XAccessControl::GetSecurityFuncAddress()
{
	fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* 
		pfnConvertStringSecurityDescriptorToSecurityDescriptorW=NULL;
	if(pfnConvertStringSecurityDescriptorToSecurityDescriptorW==NULL){
		HMODULE hModule=LoadLibrary(_T("ADVAPI32.DLL"));
		if(hModule==NULL){
			return NULL;
		}
		pfnConvertStringSecurityDescriptorToSecurityDescriptorW=
			(fn_ConvertStringSecurityDescriptorToSecurityDescriptorW*)
			GetProcAddress(hModule,"ConvertStringSecurityDescriptorToSecurityDescriptorW");
		if(pfnConvertStringSecurityDescriptorToSecurityDescriptorW==NULL){
			FreeLibrary(hModule);
			return NULL;
		}
		return pfnConvertStringSecurityDescriptorToSecurityDescriptorW;
	}
	else{
		return pfnConvertStringSecurityDescriptorToSecurityDescriptorW;
	}
}

BOOL XAccessControl::EnableSecurityPriv()
{
	HANDLE hTokenSelf=NULL;
	BOOL bRet=TRUE;
	HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetCurrentProcessId());
	if(!hProcess){
		bRet=FALSE;
	}
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS ,&hTokenSelf)){
		bRet=FALSE;
	}
	if(!SetPrivilege(hTokenSelf,SE_SECURITY_NAME,TRUE)){
		bRet=FALSE;
	}
	if(hProcess) CloseHandle(hProcess);
	if(hTokenSelf) CloseHandle(hTokenSelf);
	return bRet;
}

void XAccessControl::GoThroughDir(const char* strPath,fnVisitFunc* visit,void* para)
{
	if(!strPath) return;
	HANDLE hSearch;
	BOOL bFinish=FALSE;
	WIN32_FIND_DATAA FindData;
	char strSearchPath[MAX_PATH];
	memset(strSearchPath,0,MAX_PATH);
	strncpy(strSearchPath,strPath,MAX_PATH-1);

	if(FALSE==PathFileExistsA(strPath))	
		return;
	
	visit(strPath,para);
	PathAppendA(strSearchPath,"*.*");
	hSearch=FindFirstFileA(strSearchPath,&FindData);
	if(hSearch!=INVALID_HANDLE_VALUE)
	{
		while(!bFinish){
			if(FindNextFileA(hSearch,&FindData)){
				if((FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
					char strFullPathName[MAX_PATH];
					memset(strFullPathName,0,MAX_PATH);
					if(!strcmp(FindData.cFileName,".")) continue;
					if(!strcmp(FindData.cFileName,"..")) continue;
					strncpy(strFullPathName,strPath,MAX_PATH-1);
					
					PathAppendA(strFullPathName,FindData.cFileName);
					(*visit)(strFullPathName,para);
					GoThroughDir(strFullPathName,visit,para);
					
				}else{
					char strFullFileName[MAX_PATH];
					memset(strFullFileName,0,MAX_PATH);
					strncpy(strFullFileName,strPath,MAX_PATH-1);
					PathAppendA(strFullFileName,FindData.cFileName);
					(*visit)(strFullFileName,para);
				}
				
			}
			else if(GetLastError()==ERROR_NO_MORE_FILES){
				bFinish=TRUE;
			}
			else{
				FindClose(hSearch);
				return ;
			}
			
		}
	}
	FindClose(hSearch);
}

BOOL XAccessControl::SetFileSecurityInDir(const char* szDirFileName,BOOL bRecursive)
{
	if(!szDirFileName) 
		return FALSE;
	if(!mbSecNameEnabled){
		mbSecNameEnabled=EnableSecurityPriv();
		if(!mbSecNameEnabled)
			return FALSE;
	}
	BOOL bRet=TRUE;
	if(bRecursive==FALSE){
		bRet=SetSingleFileSecurity(szDirFileName,getHighPrivSD());
	}
	else{
		char szFullName[MAX_PATH]={0};
		GetFullPathNameA(szDirFileName,MAX_PATH,szFullName,NULL);
		char szCurrentPath[MAX_PATH]={0};
		//save current directory
		DWORD nChar=GetCurrentDirectoryA(MAX_PATH,szCurrentPath);
		GoThroughDir(szFullName,SetSingleFileSecurity,getHighPrivSD());
		//restore current directory
		if(nChar) SetCurrentDirectoryA(szCurrentPath);
	}
	return bRet;	
}

BOOL XAccessControl::SetSingleFileSecurity(const char* szFileName,void* psd)
{
	if(!szFileName)
		return FALSE;
	if(!mbSecNameEnabled){
		mbSecNameEnabled=EnableSecurityPriv();
		if(!mbSecNameEnabled)
			return FALSE;
	}
	if(!PathFileExistsA(szFileName))
		return TRUE;
	if(psd==NULL){
		psd=getHighPrivSD();
	}
	BOOL ret=::SetFileSecurityA(szFileName,DACL_SECURITY_INFORMATION,(SECURITY_DESCRIPTOR*)psd);
	return ret;
}

