// DriverOp.cpp: implementation of the CDriverOp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriverOp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment (lib,"Netapi32.lib")
#pragma comment (lib,"adsiid.lib")
#pragma comment (lib,"ActiveDS.lib")


#define heapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define heapfree(x)  (HeapFree(GetProcessHeap(), 0, x))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriverOp::CDriverOp()
{

}

CDriverOp::~CDriverOp()
{

}

bool CDriverOp::RepairMirrorReg()
{
	 OSVERSIONINFO verInfo;
	memset(&verInfo, 0, sizeof(verInfo));

    verInfo.dwOSVersionInfoSize = sizeof(verInfo);
    GetVersionEx(&verInfo);
	PSECURITY_DESCRIPTOR pSD=(PSECURITY_DESCRIPTOR)::LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
	if(pSD==NULL)
	{
		AfxMessageBox("localalloc field");
		return 0;
	}
	if(!::InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))
	{
		AfxMessageBox("INIFTIALIZESECURITYDESCRIPTOR field");
		return 0;
	}
	DWORD cbACL=1024;
	PACL pACLNew=(PACL)::LocalAlloc(LPTR,cbACL);
	if(pACLNew==NULL)
	{
		AfxMessageBox("INITIALLIZE acl field");
		return 0;
	}

	if (verInfo.dwMajorVersion>4L)
	{
		if(!::InitializeAcl(pACLNew,cbACL,ACL_REVISION_DS))
		{
			AfxMessageBox("initialize acl failed");
		}
		
	}
	else
	{
		if(!::InitializeAcl(pACLNew,cbACL,ACL_REVISION))
		{
			AfxMessageBox("initialize acl failed");
		}
	}
	DWORD cbSID=1024;
	PSID pSID=(PSID)::LocalAlloc(LPTR,cbSID);
	PSID_NAME_USE psnuType=(PSID_NAME_USE)::LocalAlloc(LPTR,1024);
	LPSTR lpszDomain;
	DWORD cchDomainName=80;
	lpszDomain=(LPSTR)::LocalAlloc(LPTR,cchDomainName);
	if(pSID==NULL||psnuType==NULL||lpszDomain==NULL)
	{
		AfxMessageBox("LOCALALLOC field");
		return 0;
	}
	if(!::LookupAccountName((LPSTR)NULL,
		"everyone",
		pSID,
		&cbSID,
		lpszDomain,
		&cchDomainName,
		psnuType))
	{
		AfxMessageBox("LookupAccountName field");
		return 0;
	}
	if(!::IsValidSid(pSID))
	{
		AfxMessageBox("sid is not valid");
	}
	if (verInfo.dwMajorVersion>4L)
	{
		
		if(!::AddAccessAllowedAce(pACLNew,ACL_REVISION2,TOKEN_ALL_ACCESS,pSID))
		{
			AfxMessageBox("AddAuditAccessObjectAce is not valid");
		}
		
	}
	else
	{
		if(!::AddAccessAllowedAce(pACLNew,ACL_REVISION,TOKEN_ALL_ACCESS,pSID))
		{
			AfxMessageBox("AddAuditAccessObjectAce is not valid");
		}
	}
	LPVOID         pTempAce       = NULL;
	if( GetAce(pACLNew,0,&pTempAce) )
	{
		((PACE_HEADER) pTempAce)->AceFlags = CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE|INHERITED_ACE;
		

	}
	if(!::SetSecurityDescriptorDacl(pSD,TRUE,pACLNew,FALSE))
	{
		AfxMessageBox("SetSecurityDescriptorDacl is not valid");
		return 0;
	}
	if(!::SetSecurityDescriptorOwner(pSD,pSID,TRUE))
	{
		AfxMessageBox("sid wrong ");
	}
	if(!::SetSecurityDescriptorGroup(pSD,pSID,TRUE))
	{
		AfxMessageBox("group wrong ");
	}
	
	PSECURITY_DESCRIPTOR pSRSD=NULL;
	DWORD cbSD=0;
	/*
	if(!::MakeSelfRelativeSD(pSD,pSRSD,&cbSD))
	{
		DWORD dw=::GetLastError();
		if(dw!=ERROR_INSUFFICIENT_BUFFER)
		{
			AfxMessageBox("error makeselfrelativesd");
			return 0;
		}
	}
	pSRSD=(PSECURITY_DESCRIPTOR)::LocalAlloc(LPTR,cbSD);
	if(!::MakeSelfRelativeSD(pSD,pSRSD,&cbSD))
	{
		AfxMessageBox("error makeselefra sidd");
		return 0;
	}
	*/
//	DWORD dw;
	 HKEY hkeyService,hcurrent;
	 DWORD dserv,dcon;

	 int kRet=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hcurrent,&dcon);
	 kRet=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hcurrent,&dcon);
	 kRet=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hcurrent,&dcon);
	 kRet=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hcurrent,&dcon);

	 kRet=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hkeyService,&dserv);

 
	HKEY hKeyProfileMirror = (HKEY)0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services"),
		 0,KEY_ALL_ACCESS, &hKeyProfileMirror) != ERROR_SUCCESS)
	{
		return 0;
	}
	DWORD dwError = RegSetKeySecurity ( 
      hKeyProfileMirror, 
      (SECURITY_INFORMATION)( DACL_SECURITY_INFORMATION),
      pSD);
	
     RegCloseKey ( hKeyProfileMirror);

	 //////////////////////
	 DWORD dw;
	 HKEY hkey;
	 int k1=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services\\XabDisp",
		 0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hkey,&dw);
	 //if(k1!=ERROR_SUCCESS)
	// {
	//	 return 0;
	// }	

	 if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services\\XabDisp"),
		 0,KEY_ALL_ACCESS, &hKeyProfileMirror) != ERROR_SUCCESS)
	{
		return 0;
	}
	
	dwError = RegSetKeySecurity ( 
      hKeyProfileMirror, 
      (SECURITY_INFORMATION)( DACL_SECURITY_INFORMATION),
      pSD);

	/////////////////////////////////////////////////////


	k1=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services\\XabDisp\\DEVICE0",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hkey,&dw);
	//if(k1!=ERROR_SUCCESS)
	//{
	//	return 0;
    //}	

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Services\\XabDisp\\DEVICE0"),
		 0,KEY_ALL_ACCESS, &hKeyProfileMirror) != ERROR_SUCCESS)
	{
		return 0;
	}
	dwError = RegSetKeySecurity ( 
      hKeyProfileMirror, 
      (SECURITY_INFORMATION)( DACL_SECURITY_INFORMATION),
      pSD);
	
     RegCloseKey ( hKeyProfileMirror);

////////////////////////////////////////////////////////////////////////////////////////
	::FreeSid(pSID);
	if(pSD!=NULL)
	{
		::LocalFree((HLOCAL)pSD);
	}
	if(pSRSD!=NULL)
	{
		::LocalFree((HLOCAL)pSRSD);
	}	
	if(pACLNew!=NULL)
	{
		::LocalFree((HLOCAL)pACLNew);
	}	
	if(psnuType!=NULL)
	{
		::LocalFree((HLOCAL)psnuType);
	}
	if(lpszDomain!=NULL)
	{
		::LocalFree((HLOCAL)lpszDomain);
	}
	return 1;
}
 

void CDriverOp::RepairWintMars()
{
  		DWORD one = 1;
	HKEY hKeyProfile = (HKEY)0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Services\\XabDispIo"),
		 0,KEY_ALL_ACCESS, &hKeyProfile) != ERROR_SUCCESS)
	{
		return  ;
	}
 
	if (RegSetValueEx(hKeyProfile,
		_T("start"),
		0,
		REG_DWORD,
		(unsigned char *)&one,
		4) != ERROR_SUCCESS)
	{
		Sleep(1);
	}
	RegCloseKey ( hKeyProfile);

}


BOOL CDriverOp::AddAccessRights(CHAR *lpszFileName, DWORD dwAccessMask)
{

// SID variables.SECURITY_DESCRIPTOR
   SID_NAME_USE   snuType;
   char *         szDomain       = NULL;
   DWORD          cbDomain       = 0;
   LPVOID         pUserSID       = NULL;
   DWORD          cbUserSID      = 0;

   // User name variables.
   char *         szUserName     = NULL;
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
	  szUserName = "everyone";
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
