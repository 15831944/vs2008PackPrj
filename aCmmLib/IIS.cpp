// IIS.cpp: implementation of the CIIS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IIS.h"
#include "CommonFun.h"
#include "adserr.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define heapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define heapfree(x)  (HeapFree(GetProcessHeap(), 0, x))

//FTP用户名和密码
#define FTP_USERNAME					"AsResourceUser"
#define FTP_PASSWORD					"$&*Rsc2002;~!@#%"

CIIS::CIIS()
{

}

CIIS::~CIIS()
{

}
BOOL CIIS::AddOperator()
{
	DWORD dwResult = -1;
	// Add account
	dwResult = AddMachineAccount();
	if( dwResult != ERROR_SUCCESS )
	{
		if( dwResult == NERR_UserExists )// User existing 2224
		{
			LPWSTR wszUserName = CString(FTP_USERNAME).AllocSysString();
			dwResult = NetUserDel( NULL, wszUserName);
		}
		else if( dwResult == ERROR_ACCESS_DENIED )// No Privilege
		{
			goto ERROR_END;
		}
		else
		{
			goto ERROR_END;
		}
		
		// Add account again
		dwResult = AddMachineAccount();
		if( dwResult != ERROR_SUCCESS )
		{
			goto ERROR_END;
		}
	}

	// Add account to local group
	dwResult = AddGroupMembers();
	if( dwResult != ERROR_SUCCESS )
	{
		//goto ERROR_END;
	}
	return TRUE;

ERROR_END:

	return FALSE;
}

DWORD CIIS::AddMachineAccount()
{
	USER_INFO_1 ui;
	DWORD dwLevel = 1;
	DWORD dwError = 0;
		
	ui.usri1_name = CString(FTP_USERNAME).AllocSysString();
	ui.usri1_password = CString(FTP_PASSWORD).AllocSysString();
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT|UF_DONT_EXPIRE_PASSWD;
	ui.usri1_script_path = NULL;
	
	return NetUserAdd( NULL, dwLevel, (LPBYTE)&ui, &dwError);
}

DWORD CIIS::AddGroupMembers()
{
	NET_API_STATUS nStatus;

	LOCALGROUP_MEMBERS_INFO_3 localgroup_members;
	localgroup_members.lgrmi3_domainandname = CString(FTP_USERNAME).AllocSysString();

	nStatus = NetLocalGroupAddMembers(
		NULL,
		L"Users",//L"Administrators",
		3,
		(LPBYTE)&localgroup_members,
		1);
	
	return nStatus;
}


void CIIS::RemoveAllList(CPtrList &pList)
{
	POSITION pos = pList.GetHeadPosition();
	while(pos)
	{
		void *pInfo = pList.GetNext(pos);
		if( pInfo )
		{
			delete pInfo;
		}
		pInfo=NULL;		
	}
	pList.RemoveAll();
}

CString CIIS::GetServerBinding(VARIANT FAR* vArray)
{	
	CString sBinding;
	SAFEARRAY FAR* psa = NULL;
	HRESULT hr;
	LONG cElements, lLBound, lUBound;
		
	if (V_VT(vArray) != (VT_ARRAY | VT_VARIANT))
	{
		return sBinding;
	}
    psa = V_ARRAY(vArray);
	// Check dimensions of the array.
	if (SafeArrayGetDim(psa) != 1)
	{
	}
	// Get array bounds.
	hr = SafeArrayGetLBound(psa, 1, &lLBound);
	if (FAILED(hr))
	{
	}
	hr = SafeArrayGetUBound(psa, 1, &lUBound);
	if (FAILED(hr))
	{
	}
	// Get a pointer to the elements of the array.
	VARIANT *pVar;

	hr = SafeArrayAccessData(psa, (void HUGEP* FAR*)&pVar);
	if (FAILED(hr))
	{
	}
	cElements = lUBound-lLBound+1;
	if( cElements>0 )
	{	
		if( pVar[0].vt == VT_BSTR )
			sBinding = pVar[0].bstrVal;
	}
	hr = SafeArrayUnaccessData(psa);
	if (FAILED(hr))
	{
	}
	return sBinding;
}



bool CIIS::GetAnonymousUserName(LPSTR pUserName)
{
	if(pUserName==NULL)
		return false;

	CoInitialize (NULL);
	CHAR pTmp[]="IIS://LocalHost/W3SVC/1/root";
	PBYTE pBuf=NULL;
	pBuf=new BYTE [200];
	MultiByteToWideChar (CP_GB2312,
		0,
		(LPSTR)pTmp,
		strlen((LPSTR)pTmp)+1,
		(LPWSTR)pBuf,
		200);

	IADsContainer *pCont = NULL;
	IADs *pADs = NULL;

	HRESULT hresError = ADsGetObject((LPWSTR)pBuf, IID_IADsContainer, (void **) &pCont);
	if (FAILED(hresError))	
	{
		delete []pBuf;	//add by zfq,2015-04-29
		return false;
	}
	
	hresError = pCont->QueryInterface (IID_IADs, (void **)&pADs);
	if (FAILED(hresError))
	{
		pCont->Release ();
		delete []pBuf;	//add by zfq,2015-04-29
		return false;
	}
	VARIANT var;	
	VariantInit(&var);
	var.vt = VT_BSTR ;	
	pADs->Get(L"AnonymousUserName", &var);

	CString str;
	str = CCommonFun::GetVariantStr(var);
	strcpy(pUserName, str);

	pADs->Release ();
	pCont->Release ();
	CoUninitialize ();
	
	delete pBuf;
	return true;
}

BOOL CIIS::InitIISAnonymousSA(LPSECURITY_ATTRIBUTES pSA)
{
	PSECURITY_DESCRIPTOR  pSD=(PSECURITY_DESCRIPTOR)::LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
	PACL pACLNew=0;
	OSVERSIONINFO verInfo ;
	DWORD cbSID=1024;
	PSID pSID=0;
	PSID_NAME_USE psnuType=0;
	LPSTR lpszDomain=0;
	DWORD cchDomainName=80;
	char lpAnonyUser[64];

	memset(pSA,0,sizeof(SECURITY_ATTRIBUTES));

	if(pSD==NULL)
	{
		TRACE("localalloc field");
		return FALSE;
	}
	if(!::InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))
	{
		TRACE("INIFTIALIZESECURITYDESCRIPTOR field");
		return FALSE;
	}

	pACLNew=(PACL)::LocalAlloc(LPTR,1024);
	if(pACLNew==NULL)
	{
		TRACE("INITIALLIZE acl field");
		return FALSE;
	}

	ZeroMemory(&verInfo,sizeof(OSVERSIONINFO)) ;
    verInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&verInfo);

	if (verInfo.dwMajorVersion>4L)
	{
		if(!::InitializeAcl(pACLNew,1024,ACL_REVISION_DS))
		{
			TRACE("initialize acl failed");
			return FALSE;
		}
		
	}
	else
	{
		if(!::InitializeAcl(pACLNew,1024,ACL_REVISION2))
		{
			TRACE("initialize acl failed");
			return FALSE;
		}
	}
	cbSID=1024;
	pSID=(PSID)::LocalAlloc(LPTR,cbSID);
	psnuType=(PSID_NAME_USE)::LocalAlloc(LPTR,1024);
	cchDomainName=80;

	lpszDomain=(LPSTR)::LocalAlloc(LPTR,cchDomainName);
	if(pSID==NULL||psnuType==NULL||lpszDomain==NULL)
	{
		TRACE("LOCALALLOC field");
		return FALSE;
	}

	memset(lpAnonyUser,0,64);
	if(GetAnonymousUserName(lpAnonyUser)==false)
	{
		TRACE("GetAnonymouseUserName::failed");
		return FALSE;
	}

	if(!::LookupAccountName((LPSTR)NULL,
		(LPCTSTR)lpAnonyUser,
		pSID,
		&cbSID,
		lpszDomain,
		&cchDomainName,
		psnuType))
	{
		TRACE("LookupAccountName field");
		return FALSE;
	}
	if(!::IsValidSid(pSID))
	{
		TRACE("sid is not valid");
		return FALSE;
	}
	if (verInfo.dwMajorVersion>4L)
	{
		
		if(!::AddAccessAllowedAce(pACLNew,ACL_REVISION_DS,GENERIC_ALL,pSID))
		{
			TRACE("AddAuditAccessObjectAce is not valid");
			return FALSE;
		}
	}
	else
	{
		if(!::AddAccessAllowedAce(pACLNew,ACL_REVISION,GENERIC_ALL,pSID))
		{
			TRACE("AddAuditAccessObjectAce is not valid");
			return FALSE;
		}
	}
	if(!::SetSecurityDescriptorDacl(pSD,TRUE,pACLNew,FALSE))
	{
		TRACE("SetSecurityDescriptorDacl is not valid");
		return FALSE;
	}

	pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
	pSA->lpSecurityDescriptor = pSD;
	pSA->bInheritHandle = true;

	return TRUE;
}



BOOL CIIS::AddAccessRights(LPCTSTR lpszFileName,LPCTSTR szUserName, DWORD dwAccessMask)
{

	  // SID variables.SECURITY_DESCRIPTOR
   SID_NAME_USE   snuType;
   char *         szDomain       = NULL;
   DWORD          cbDomain       = 0;
   LPVOID         pUserSID       = NULL;
   DWORD          cbUserSID      = 0;

   // User name variables.
  // char *         szUserName     = NULL;
   DWORD          cbUserName     = 0;

   // File SD variables.
   PSECURITY_DESCRIPTOR pFileSD  = NULL;
   DWORD          cbFileSD       = 0;

   // New SD variables.
   PSECURITY_DESCRIPTOR pNewSD   = NULL;

   // ACL variables.
   PACL           pACL           = NULL;
   BOOL           fDaclPresent;
   BOOL           fDaclDefaulted;
   ACL_SIZE_INFORMATION AclInfo;

   // New ACL variables.
   PACL           pNewACL        = NULL;
   DWORD          cbNewACL       = 0;

   // Temporary ACE.
   LPVOID         pTempAce       = NULL;
   UINT           CurrentAceIndex;

   // Assume function will fail.
   BOOL           fResult        = FALSE;
   BOOL           fAPISuccess;

   __try {

      // 
      // STEP 1: Get the logged on user name.
      // 
/*
      fAPISuccess = GetUserName(szUserName, &cbUserName);

      // API should have failed with insufficient buffer.
      if (fAPISuccess)
         __leave;
      else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
         printf("GetUserName() failed.  Error %d\n", GetLastError());
         __leave;
      }

      szUserName = (char *) heapalloc(cbUserName);
      if (!szUserName) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
         __leave;
      }

      fAPISuccess = GetUserName(szUserName, &cbUserName);
      if (!fAPISuccess) {
         printf("GetUserName() failed.  Error %d\n", GetLastError());
         __leave;
      }
*/

      // 
      // STEP 2: Get SID for current user.
      // 
      fAPISuccess = LookupAccountName((LPSTR) NULL, szUserName,
            pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);

      // API should have failed with insufficient buffer.
      if (fAPISuccess)
         __leave;
      else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
         printf("LookupAccountName() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      pUserSID = heapalloc(cbUserSID);
      if (!pUserSID) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
         __leave;
      }

      szDomain = (char *) heapalloc(cbDomain);
      if (!szDomain) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
         __leave;
      }

      fAPISuccess = LookupAccountName((LPSTR) NULL, szUserName,
            pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
      if (!fAPISuccess) {
         printf("LookupAccountName() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      // 
      // STEP 3: Get security descriptor (SD) for file.
      // 
      fAPISuccess = GetFileSecurity(lpszFileName, 
            DACL_SECURITY_INFORMATION, pFileSD, 0, &cbFileSD);

      // API should have failed with insufficient buffer.
      if (fAPISuccess)
         __leave;
      else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
         printf("GetFileSecurity() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      pFileSD = heapalloc(cbFileSD);
      if (!pFileSD) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
         __leave;
      }

      fAPISuccess = GetFileSecurity(lpszFileName, 
            DACL_SECURITY_INFORMATION, pFileSD, cbFileSD, &cbFileSD);
      if (!fAPISuccess) {
         printf("GetFileSecurity() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      // 
      // STEP 4: Initialize new SD.
      // 
      pNewSD = heapalloc(cbFileSD); // Should be same size as FileSD.为什么？？？？？？？？？？？？？？
      if (!pNewSD) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
         __leave;
      }

      if (!InitializeSecurityDescriptor(pNewSD, 
            SECURITY_DESCRIPTOR_REVISION)) {
         printf("InitializeSecurityDescriptor() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      // 
      // STEP 5: Get DACL from SD.
      // 
      if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL,
            &fDaclDefaulted)) {
         printf("GetSecurityDescriptorDacl() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      // 
      // STEP 6: Get size information for DACL.
      // 
      AclInfo.AceCount = 0; // Assume NULL DACL.
      AclInfo.AclBytesFree = 0;
      AclInfo.AclBytesInUse = sizeof(ACL);

      // If not NULL DACL, gather size information from DACL.
      if (fDaclPresent && pACL) {    
         
         if(!GetAclInformation(pACL, &AclInfo, 
               sizeof(ACL_SIZE_INFORMATION), AclSizeInformation)) {
            printf("GetAclInformation() failed.  Error %d\n",
               GetLastError());
            __leave;
         }
      }

      // 
      // STEP 7: Compute size needed for the new ACL.
      // 
      cbNewACL = AclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE) 
            + GetLengthSid(pUserSID) - sizeof(DWORD);

      // 
      // STEP 8: Allocate memory for new ACL.
      // 
      pNewACL = (PACL) heapalloc(cbNewACL);
      if (!pNewACL) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
         __leave;
      }

      // 
      // STEP 9: Initialize the new ACL.
      // 
      if(!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2)) {
         printf("InitializeAcl() failed.  Error %d\n", GetLastError());
         __leave;
      }

      // SID
      // STEP 10: If DACL is present, copy it to a new DACL.
      // 
      if (fDaclPresent) 
	  {

         // 
         // STEP 11: Copy the file's ACEs to the new ACL.
         // 
         if (AclInfo.AceCount) 
		 {

            for (CurrentAceIndex = 0; 
                  CurrentAceIndex < AclInfo.AceCount;
                  CurrentAceIndex++) {

               // 
               // STEP 12: Get an ACE.
               // ACE
               if(!GetAce(pACL, CurrentAceIndex, &pTempAce)) {
                  printf("GetAce() failed.  Error %d\n", 
                        GetLastError());
                  __leave;
               }


               // 
               // STEP 13: Add the ACE to the new ACL.
               // 
               if(!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,
                     ((PACE_HEADER) pTempAce)->AceSize)) {
                  printf("AddAce() failed.  Error %d\n", 
                        GetLastError());
                  __leave;
               }
            }
         }
      }

      // 
      // STEP 14: Add the access-allowed ACE to the new DACL.
      // 
	  ///

      if (!AddAccessAllowedAce(pNewACL, ACL_REVISION2, dwAccessMask,
            pUserSID)) {
         printf("AddAccessAllowedAce() failed.  Error %d\n",
               GetLastError());
         __leave;
      }
	  //-==================设置继承性===========================

	  if( GetAce(pNewACL,AclInfo.AceCount,&pTempAce) )
	  {
		  ((PACE_HEADER) pTempAce)->AceFlags = CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE;
	  }

	  //====================================================

	  /*
	  if( !AddAccessAllowedAceEx(pNewACL,ACL_REVISION2,INHERITED_ACE,dwAccessMask,pUserSID) )
	  {
         printf("AddAccessAllowedAce() failed.  Error %d\n",
               GetLastError());
         __leave;
	  }

	  */
      // 
      // STEP 15: Set the new DACL to the file SD.
      // 
      if (!SetSecurityDescriptorDacl(pNewSD, TRUE, pNewACL, 
            FALSE)) {
         printf("() failed.  Error %d\n", GetLastError());
         __leave;
      }

      // 
      // STEP 16: Set the SD to the File.
      // 
      if (!SetFileSecurity(lpszFileName, DACL_SECURITY_INFORMATION,
            pNewSD)) {
         printf("SetFileSecurity() failed.  Error %d\n", 
               GetLastError());
         __leave;
      }

      fResult = TRUE;

   } __finally {

      // 
      // STEP 17: Free allocated memory
      // 
   //   if (szUserName)
    ///     heapfree(szUserName);
      
      if (pUserSID)
         heapfree(pUserSID);

      if (szDomain)
         heapfree(szDomain);

      if (pFileSD)
         heapfree(pFileSD);

      if (pNewSD)
         heapfree(pNewSD);

      if (pNewACL)
         heapfree(pNewACL);
   }
   
   return fResult;
}

BOOL CIIS::GrantRightToUser(CString szDir)
{
	DWORD dwMask = GENERIC_ALL|STANDARD_RIGHTS_ALL;
	char cIIsUserName[100] = {0};

	//if(GetAnonymousUserName(cIIsUserName))
	//	AddAccessRights(szDir,(LPCTSTR)cIIsUserName,dwMask);

	//Sleep(200);
	//AddAccessRights(szDir,(LPCTSTR)CString(FTP_USERNAME),dwMask);//FTP上传屏幕用户

	//Sleep(200);
	AddAccessRights(szDir,(LPCTSTR)CString("Everyone"),dwMask);

	//Sleep(200);
	//AddAccessRights(szDir,(LPCTSTR)CString("NETWORK SERVICE"),dwMask);//ASP.NET删除屏幕日志用户
	return true;
}
