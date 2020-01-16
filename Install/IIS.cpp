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



CIIS::CIIS()
{
	CoInitialize(NULL);
}

CIIS::~CIIS()
{
	CoUninitialize();
}
BOOL CIIS::AddOperator()
{

	return AddUser(FTP_USERNAME,FTP_PASSWORD,"Users");
	 

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
BOOL CIIS::EnumMembers(IADsMembers *pMembers,char* sUser)
{
	IUnknown *pUnk;
	HRESULT hr;
	hr = pMembers->get__NewEnum(&pUnk);
	if (FAILED(hr))
	{
		TRACE("pMembers->get__NewEnum(&pUnk) faile = %d\n",::GetLastError());
		theLog.Write("!!CIIS::EnumMembers,1,get__NewEnum fail,err=%d,hr=%x", GetLastError(), hr);
		return FALSE;
	}

	IEnumVARIANT *pEnum;
	hr = pUnk->QueryInterface(IID_IEnumVARIANT,(void**)&pEnum);
	if (FAILED(hr)) 
	{
		TRACE("pUnk->QueryInterface(IID_IEnumVARIANT,(void**)&pEnum) faile = %d\n",::GetLastError());
		theLog.Write("!!CIIS::EnumMembers,2,QueryInterface fail,err=%d,hr=%x", GetLastError(), hr);
		return FALSE;
	}
	pUnk->Release();

	// Now Enumerate
	WCHAR szwUser[MAX_PATH] = {0};
	wsprintfW(szwUser, L"%hS", sUser);
	//	wprintf(L"User = %s\n",szwUser);
	//	BSTR	bsUser(sUser);
	BOOL bRet = FALSE;
	BSTR bstr;
	VARIANT var;
	IADs *pADs;
	ULONG lFetch;
	IDispatch *pDisp;

	TRACE("User = %s\n",sUser);
	VariantInit(&var);
	hr = pEnum->Next(1, &var, &lFetch);
	//	wprintf(L"User = %s\n",bsUser);

	if(S_OK != hr) //add by zfq,2012.12.11,log it when fail
	{
		theLog.Write("!!CIIS::EnumMembers,3,Next fail,err=%d,hr=%x,sUser=%s", GetLastError(), hr, sUser);
	}

	while(hr == S_OK)
	{
		if (lFetch == 1)
		{
			pDisp = V_DISPATCH(&var);
			pDisp->QueryInterface(IID_IADs, (void**)&pADs);
			pADs->get_Name(&bstr);

			wprintf(L"get_Name = %s\n",bstr);
			if(wcsicmp(szwUser,bstr) == 0)
				//	 if(bstr == szwUser)
			{
				bRet = TRUE;
				printf("find = TRUE \n");
				break;
			}
			SysFreeString(bstr);
			pADs->Release();
		}
		VariantClear(&var);
		pDisp=NULL;
		hr = pEnum->Next(1, &var, &lFetch);
	};
	hr = pEnum->Release();

	return bRet;
}

BOOL CIIS::IsGroupMembers(IADsGroup *pGroup,char* sUser)
{
	IADsMembers *pMembers;
	HRESULT hr = S_OK;
	hr = pGroup->Members(&pMembers);

	if (FAILED(hr)) 
	{
		TRACE("pGroup->Members faile = %d",::GetLastError());
		theLog.Write("!!CIIS::IsGroupMembers,1,Members fail,err=%d,hr=%x", GetLastError(), hr);
		return FALSE;
	}
	BOOL bRet = EnumMembers(pMembers,sUser);  // See code snippet in
	// IADsMembers::get__NewEnum.
	pMembers->Release();
	return bRet;
}
//---------------------------------------------
//加入到指定的组
//
BOOL CIIS::AddUserToGroup(char* sGroup, char* sUser)
{
	BOOL bRet = FALSE;
	HRESULT hr;
	IADsGroup *pGroup=NULL;
	_bstr_t bName;
	_bstr_t bstr;
	VARIANT_BOOL inG=false;
	DWORD ll_len=255;

	char lbBuffer[255] = {0};
	::GetComputerName(lbBuffer,&ll_len);

	bstr="WinNT://"+_bstr_t(lbBuffer)+"/"+_bstr_t(sGroup);

	do
	{
		hr= ADsGetObject(bstr,IID_IADsGroup,(void **)&pGroup);		
		if( !SUCCEEDED(hr) )
		{
			int nError = GetLastError();
			printf("ADsGetObject faile error = %d\n",nError);
			theLog.Write("!!CIIS::AddUserToGroup,1,ADsGetObject fail,err=%d,hr=%x", GetLastError(), hr);
			break;
		}

		//	bName = "WinNT://"+_bstr_t(lbBuffer)+"/"+ _bstr_t(sUser);
		bName = "WinNT://" + _bstr_t(sUser);

		/*-----------------------------------------
		hr = pGroup->IsMember(_bstr_t(sUser), &inG);
		if (inG) 
		{
		bRet = TRUE;
		break;
		}
		//-------------------------------------------------
		*/
		if(IsGroupMembers(pGroup,sUser))
		{
			printf("IsGroupMembers = TRUE\n");
			bRet = TRUE;
			break;
		}
		
		bName = "WinNT://"+_bstr_t(lbBuffer)+"/"+ _bstr_t(sUser);
		hr = pGroup->Add(bName);		
		if( !SUCCEEDED(hr) ) 
		{ 
			int nError = GetLastError();
			printf("pGroup->Add(bName) faile error = %d\n",nError);
			theLog.Write("!!CIIS::AddUserToGroup,7,Add fail,err=%d,hr=%x", GetLastError(), hr);
			break;	
		}
		bRet = TRUE;

	}while(false);

	if(pGroup)
		pGroup->Release(); 

	return bRet;
}

//添加用户
//
BOOL CIIS::AddUser(char* sUser,char* sPsw,char* sGrp)
{
	BOOL			bRet = FALSE;
	HRESULT			hr;
	IADsContainer	*pCont = NULL;
	IADs			*pADs = NULL;
	IDispatch		*pDisp = NULL;
	IADsUser *pUser = NULL;

	_bstr_t			bstr,
		bstr2;
	DWORD			ll_len = 255;
	char			lbBuffer[255];
	::GetComputerName(lbBuffer,&ll_len);
	bstr="WinNT://"+_bstr_t(lbBuffer);
	char szUser[MAX_PATH] = {0};
	sprintf_s(szUser,sizeof(szUser),"%s",sUser);
	do
	{
		hr = ADsGetObject(	bstr, 
			IID_IADsContainer, 
			(void**) &pCont 
			);
		if (!SUCCEEDED(hr) )
		{
			theLog.Write("!!CIIS::AddUser,1,ADsGetObject fail,err=%d,hr=%x", GetLastError(), hr);
			break;
		}
		//----添加用户
		hr = pCont->Create(L"user",_bstr_t(szUser), &pDisp );
		if( !SUCCEEDED(hr) ) 
		{ 
			printf("Create User Faile\n");
			theLog.Write("!!CIIS::AddUser,2,Create fail,err=%d,szUser=%s,hr=%x", GetLastError(), szUser, hr);
			break;
		}
		hr = pDisp->QueryInterface( IID_IADs, (void**) &pUser );	
		if ( !SUCCEEDED(hr) ) 
		{ 
			theLog.Write("!!CIIS::AddUser,3,QueryInterface fail,err=%d,hr=%x", GetLastError(), hr);
			break;
		}		
		if(pUser)
		{
			VARIANT_BOOL bAccountDisabled=FALSE;
			VARIANT_BOOL bIsLocked=FALSE;

			
			pUser ->put_Description (L"");
			pUser ->put_LoginScript (L"");
			pUser ->put_HomeDirectory (L"");
			pUser ->put_Profile (L"");
			pUser ->put_AccountDisabled (bAccountDisabled);
			pUser ->put_IsAccountLocked (bIsLocked);

			pUser->SetInfo(); // Commit

			pUser->Release();
		}
		else
		{
			theLog.Write("!!CIIS::AddUser,4,QueryInterface fail,err=%d,pUser=%p,hr=%x", GetLastError(), pUser, hr);
		}
		

		if(pCont)
		{
			pCont->Release();
		}

		if(pDisp)
		{
			pDisp->Release();
		}


		char szGrp[100] = {0};
		if (!sGrp)
		{
			strcpy(szGrp,"Administrators");
		}
		else
		{
			strcpy(szGrp,sGrp);
		}
		if(!AddUserToGroup(szGrp,sUser))
		{
			printf("AddUserToGroup Faile\n");
			theLog.Write("!!CIIS::AddUser,5,AddUserToGroup fail,err=%d,sUser=%s,szGrp=%s,hr=%x", GetLastError(), sUser, szGrp, hr);
			break;
		}
		//	if(!AddUserToGroup("Debugger Users",sUser))
		//	{
		//		break;
		//	}

		if(!ChangeUserPsw(sUser,sPsw))
		{
			printf("ChangeUserPsw Faile\n");
			theLog.Write("!!CIIS::AddUser,6,ChangeUserPsw fail,err=%d,sUser=%s,sPsw=%s,hr=%x", GetLastError(), sUser, sPsw, hr);
			break;
		}

		// 		TCHAR szBuf[MAX_COMPUTERNAME_LENGTH + 1] = {0};
		// 		DWORD dw = MAX_COMPUTERNAME_LENGTH + 1;
		// 		::GetComputerName(szBuf,&dw);
		// 		GrantUserLogonSvrRight(szBuf,sUser);


		bRet = TRUE;

	}while(false);

	return bRet;	
}
//-------------------------------------------
//修改密码
//
BOOL CIIS::ChangeUserPsw(char* sUser, char* sPsw)
{
	BOOL bRet = FALSE;
	_bstr_t			bstr;
	DWORD			ll_len = 255;
	char			lbBuffer[255];
	::GetComputerName(lbBuffer,&ll_len);
	bstr="WinNT://"+_bstr_t(lbBuffer) + "/" + _bstr_t(sUser);// + ",user";
	IADsUser *pUser = NULL;
	VARIANT var;
	VariantInit(&var);

	HRESULT hr = ADsGetObject(bstr,IID_IADsUser,(void**)&pUser);
	if (!SUCCEEDED(hr) )
	{
		theLog.Write("!!CIIS::ChangeUserPsw,1,ADsGetObject fail,err=%d,hr=%x", GetLastError(), hr);
		return bRet;
	}

	hr = pUser->SetPassword(_bstr_t(sPsw));
	if(SUCCEEDED(hr))
		bRet = TRUE;

	hr = pUser->Get(L"UserFlags",&var);

	V_I4(&var) |= ADS_UF_DONT_EXPIRE_PASSWD;
	hr = pUser->Put(L"UserFlags",var);

	hr = pUser->SetInfo();
	VariantClear(&var);

	pUser->Release();

	return TRUE;  
}
/*
BOOL CIIS::AddObjectAccess(CString strFileName, const CSid &rSid, ACCESS_MASK accessmask,SE_OBJECT_TYPE eType )
{
	ATL::CSecurityDesc OutSecDesc;
	ATL::AtlGetSecurityDescriptor(strFileName, eType, &OutSecDesc);
	OutSecDesc.MakeAbsolute();
	bool pbPresent = 0;

	ATL::CDacl pDacl;
	if (!OutSecDesc.GetDacl(&pDacl, &pbPresent))
		return FALSE;

	RemoveUsers(pDacl);

	if (!pDacl.AddAllowedAce(rSid, accessmask,  CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE))
	{
		//已经把已有的删掉了！
		return FALSE;
	}

	OutSecDesc.SetControl(SE_DACL_AUTO_INHERITED | SE_DACL_PROTECTED, SE_DACL_AUTO_INHERITED);
	ATL::AtlSetDacl(strFileName, eType, pDacl);
	ATL::CSacl pSacl;
	///* We've already set the Dacl. Now set the SACL. 
	OutSecDesc.GetSacl(&pSacl, &pbPresent);
	if(pbPresent)
	{
		ATL::AtlSetSacl(strFileName, eType, pSacl);
	}
	return TRUE;
}
*/
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
	{
		theLog.Write("!!CIIS::GetAnonymousUserName,1,pUserName=%p", pUserName);
		return false;
	}

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
		theLog.Write("!!CIIS::GetAnonymousUserName,2,ADsGetObject fail,err=%d,hresError=%x", GetLastError(), hresError);
		return false;
	}
	
	hresError = pCont->QueryInterface (IID_IADs, (void **)&pADs);
	if (FAILED(hresError))
	{
		theLog.Write("!!CIIS::GetAnonymousUserName,3,QueryInterface fail,err=%d,hresError=%x", GetLastError(), hresError);
		pCont->Release ();
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
		theLog.Write("!!CIIS::InitIISAnonymousSA,1,LocalAlloc fail,err=%d", GetLastError());
		return FALSE;
	}
	if(!::InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))
	{
		TRACE("INIFTIALIZESECURITYDESCRIPTOR field");
		theLog.Write("!!CIIS::InitIISAnonymousSA,2,InitializeSecurityDescriptor fail,err=%d", GetLastError());
		return FALSE;
	}

	pACLNew=(PACL)::LocalAlloc(LPTR,1024);
	if(pACLNew==NULL)
	{
		TRACE("INITIALLIZE acl field");
		theLog.Write("!!CIIS::InitIISAnonymousSA,3,LocalAlloc fail,err=%d", GetLastError());
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
			theLog.Write("!!CIIS::InitIISAnonymousSA,4,InitializeAcl fail,err=%d,verInfo.dwMajorVersion=%d"
				, GetLastError(), verInfo.dwMajorVersion);
			return FALSE;
		}
		
	}
	else
	{
		if(!::InitializeAcl(pACLNew,1024,ACL_REVISION2))
		{
			TRACE("initialize acl failed");
			theLog.Write("!!CIIS::InitIISAnonymousSA,5,InitializeAcl fail,err=%d,verInfo.dwMajorVersion=%d"
				, GetLastError(), verInfo.dwMajorVersion);

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
		theLog.Write("!!CIIS::InitIISAnonymousSA,6,LocalAlloc fail,err=%d,pSID=%p,psnuType=%p,lpszDomain=%p"
			, GetLastError(), pSID, psnuType, lpszDomain);
		return FALSE;
	}

	memset(lpAnonyUser,0,64);
	if(GetAnonymousUserName(lpAnonyUser)==false)
	{
		TRACE("GetAnonymouseUserName::failed");
		theLog.Write("!!CIIS::InitIISAnonymousSA,7,GetAnonymousUserName fail,err=%d", GetLastError());
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
		theLog.Write("!!CIIS::InitIISAnonymousSA,8,LookupAccountName fail,err=%d", GetLastError());
		return FALSE;
	}
	if(!::IsValidSid(pSID))
	{
		TRACE("sid is not valid");
		theLog.Write("!!CIIS::InitIISAnonymousSA,9,IsValidSid fail,err=%d", GetLastError());
		return FALSE;
	}
	if (verInfo.dwMajorVersion>4L)
	{
		
		if(!::AddAccessAllowedAce(pACLNew,ACL_REVISION_DS,GENERIC_ALL,pSID))
		{
			TRACE("AddAuditAccessObjectAce is not valid");
			theLog.Write("!!CIIS::InitIISAnonymousSA,10,AddAccessAllowedAce fail,err=%d,verInfo.dwMajorVersion=%d"
				, GetLastError(), verInfo.dwMajorVersion);
			return FALSE;
		}
	}
	else
	{
		if(!::AddAccessAllowedAce(pACLNew,ACL_REVISION,GENERIC_ALL,pSID))
		{
			TRACE("AddAuditAccessObjectAce is not valid");
			theLog.Write("!!CIIS::InitIISAnonymousSA,11,AddAccessAllowedAce fail,err=%d,verInfo.dwMajorVersion=%d"
				, GetLastError(), verInfo.dwMajorVersion);
			return FALSE;
		}
	}
	if(!::SetSecurityDescriptorDacl(pSD,TRUE,pACLNew,FALSE))
	{
		TRACE("SetSecurityDescriptorDacl is not valid");
		theLog.Write("!!CIIS::InitIISAnonymousSA,12,SetSecurityDescriptorDacl fail,err=%d", GetLastError());
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
		 theLog.Write("!!CIIS::AddAccessRights,7,LookupAccountName fail,err=%d", GetLastError());
         __leave;
      }

      pUserSID = heapalloc(cbUserSID);
      if (!pUserSID) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,8,heapalloc fail,err=%d,cbUserSID=%d", GetLastError(), cbUserSID);
         __leave;
      }

      szDomain = (char *) heapalloc(cbDomain);
      if (!szDomain) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,9,heapalloc fail,err=%d,cbDomain=%d", GetLastError(), cbDomain);
         __leave;
      }

      fAPISuccess = LookupAccountName((LPSTR) NULL, szUserName,
            pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
      if (!fAPISuccess) {
         printf("LookupAccountName() failed.  Error %d\n", 
               GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,10,LookupAccountName fail,err=%d,szUserName=%d", GetLastError(), szUserName);
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
		 theLog.Write("!!CIIS::AddAccessRights,11,GetFileSecurity fail,err=%d,lpszFileName=%d", GetLastError(), lpszFileName);
         __leave;
      }

      pFileSD = heapalloc(cbFileSD);
      if (!pFileSD) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,12,heapalloc fail,err=%d,cbFileSD=%d,lpszFileName=%d", GetLastError(), cbFileSD, lpszFileName);
         __leave;
      }

      fAPISuccess = GetFileSecurity(lpszFileName, 
            DACL_SECURITY_INFORMATION, pFileSD, cbFileSD, &cbFileSD);
      if (!fAPISuccess) {
         printf("GetFileSecurity() failed.  Error %d\n", 
               GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,13,GetFileSecurity fail,err=%d,lpszFileName=%d", GetLastError(), lpszFileName);
         __leave;
      }

      // 
      // STEP 4: Initialize new SD.
      // 
      pNewSD = heapalloc(cbFileSD); // Should be same size as FileSD.为什么？？？？？？？？？？？？？？
      if (!pNewSD) {
         printf("HeapAlloc() failed.  Error %d\n", GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,14,heapalloc fail,err=%d,cbFileSD=%d,lpszFileName=%d", GetLastError(), cbFileSD, lpszFileName);
         __leave;
      }

      if (!InitializeSecurityDescriptor(pNewSD, 
            SECURITY_DESCRIPTOR_REVISION)) {
         printf("InitializeSecurityDescriptor() failed.  Error %d\n", 
               GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,15,InitializeSecurityDescriptor fail,err=%d", GetLastError());
         __leave;
      }

      // 
      // STEP 5: Get DACL from SD.
      // 
      if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL,
            &fDaclDefaulted)) {
         printf("GetSecurityDescriptorDacl() failed.  Error %d\n", 
               GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,16,GetSecurityDescriptorDacl fail,err=%d", GetLastError());
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
			theLog.Write("!!CIIS::AddAccessRights,17,GetAclInformation fail,err=%d", GetLastError());
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
		 theLog.Write("!!CIIS::AddAccessRights,18,heapalloc fail,err=%d,cbNewACL=%d", GetLastError(), cbNewACL);
         __leave;
      }

      // 
      // STEP 9: Initialize the new ACL.
      // 
      if(!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2)) {
         printf("InitializeAcl() failed.  Error %d\n", GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,19,InitializeAcl fail,err=%d,cbNewACL=%d", GetLastError(), cbNewACL);
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
				  theLog.Write("!!CIIS::AddAccessRights,20,GetAce fail,err=%d", GetLastError());
                  __leave;
               }


               // 
               // STEP 13: Add the ACE to the new ACL.
               // 
               if(!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,
                     ((PACE_HEADER) pTempAce)->AceSize)) {
                  printf("AddAce() failed.  Error %d\n", 
                        GetLastError());
				  theLog.Write("!!CIIS::AddAccessRights,21,AddAce fail,err=%d", GetLastError());
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
		 theLog.Write("!!CIIS::AddAccessRights,22,AddAccessAllowedAce fail,err=%d", GetLastError());
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
		 theLog.Write("!!CIIS::AddAccessRights,23,SetSecurityDescriptorDacl fail,err=%d", GetLastError());
         __leave;
      }

      // 
      // STEP 16: Set the SD to the File.
      // 
      if (!SetFileSecurity(lpszFileName, DACL_SECURITY_INFORMATION,
            pNewSD)) {
         printf("SetFileSecurity() failed.  Error %d\n", 
               GetLastError());
		 theLog.Write("!!CIIS::AddAccessRights,24,SetFileSecurity fail,err=%d", GetLastError());
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
	BOOL BRet = AddAccessRights(szDir,(LPCTSTR)CString(FTP_USERNAME),dwMask);//FTP上传屏幕用户

	if(!BRet)	//add by zfq,2012.12.11,log it when invoke fail
	{
		theLog.Write("!!CIIS::GrantRightToUser,1,AddAccessRights fail,err=%d,szDir=%s,FTP_USERNAME=%s"
			, GetLastError(), szDir, FTP_USERNAME);
	}
	//Sleep(200);
	//AddAccessRights(szDir,(LPCTSTR)CString("Everyone"),dwMask);

	//Sleep(200);
	//AddAccessRights(szDir,(LPCTSTR)CString("NETWORK SERVICE"),dwMask);//ASP.NET删除屏幕日志用户
//	return true;	//delete by zfq,2012.12.11
	return TRUE;	//add by zfq,2012.12.11
}
