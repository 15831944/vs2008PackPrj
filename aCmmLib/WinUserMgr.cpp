#include "stdafx.h"
#include "WinUserMgr.h"
#include <Userenv.h>
#pragma comment (lib, "Userenv.lib")
#pragma comment (lib,"Netapi32.lib")
#pragma comment (lib,"adsiid.lib")
#pragma comment (lib,"ActiveDS.lib")

CWinUserMgr::CWinUserMgr()
{
	CoInitialize(NULL);
}

CWinUserMgr::~CWinUserMgr()
{
	ReleaseADs();
	CoUninitialize();
}

BOOL CWinUserMgr::EnumMembers(IADsMembers *pMembers,char* sUser)
{
	IUnknown *pUnk;
	HRESULT hr;
	hr = pMembers->get__NewEnum(&pUnk);
	if (FAILED(hr))
	{
		TRACE("pMembers->get__NewEnum(&pUnk) faile = %d\n",::GetLastError());
		return FALSE;
	}

	IEnumVARIANT *pEnum;
	hr = pUnk->QueryInterface(IID_IEnumVARIANT,(void**)&pEnum);
	if (FAILED(hr)) 
	{
		TRACE("pUnk->QueryInterface(IID_IEnumVARIANT,(void**)&pEnum) faile = %d\n",::GetLastError());
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

BOOL CWinUserMgr::IsGroupMembers(IADsGroup *pGroup,char* sUser)
{
	IADsMembers *pMembers;
	HRESULT hr = S_OK;
	hr = pGroup->Members(&pMembers);

	if (FAILED(hr)) 
	{
		TRACE("pGroup->Members faile = %d",::GetLastError());
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
BOOL CWinUserMgr::AddUserToGroup(char* sGroup, char* sUser)
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
			break;	
		}
		bRet = TRUE;

	}while(false);

	if(pGroup)
		pGroup->Release(); 

	return bRet;
}

BOOL CWinUserMgr::LockUser(char* sUser,BOOL bLock /*= TRUE*/)
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
		return bRet;
	}
	VARIANT_BOOL vt = bLock ? -1 : 0;
	hr = pUser->put_IsAccountLocked(vt);
	if(SUCCEEDED(hr))
		bRet = TRUE;

	hr = pUser->SetInfo();
	pUser->Release();

	return bRet;  
}
BOOL CWinUserMgr::DisableUser(char* sUser,BOOL bDisable /*= TRUE*/)
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
		return bRet;
	}
	VARIANT_BOOL vt = bDisable ? -1 : 0;
	hr = pUser->put_AccountDisabled(vt);
	if(SUCCEEDED(hr))
		bRet = TRUE;
	hr = pUser->SetInfo();
	CCommonFun::WriteFileLog("DisableUser %s,%d,%X",sUser,bDisable,hr);
	pUser->Release();

	return bRet;  
}
//-------------------------------------------
//修改密码
//
BOOL CWinUserMgr::ChangeUserPsw(char* sUser, char* sPsw)
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

//-------------------------------------------------------------------
//添加用户
//
BOOL CWinUserMgr::AddUser(char* sUser,char* sPsw,char* sGrp)
{
	BOOL			bRet = FALSE;
	HRESULT			hr;
	IADsContainer	*pCont = NULL;
	IADs			*pADs = NULL;
	IDispatch		*pDisp = NULL;

	_bstr_t			bstr,
		bstr2;
	DWORD			ll_len = 255;
	char			lbBuffer[255];
	::GetComputerName(lbBuffer,&ll_len);
	bstr="WinNT://"+_bstr_t(lbBuffer);

	do
	{
		hr = ADsGetObject(	bstr, 
			IID_IADsContainer, 
			(void**) &pCont 
			);
		if (!SUCCEEDED(hr) )
		{
			break;
		}
		//----添加用户
		hr = pCont->Create(L"user",_bstr_t(sUser), &pDisp );
		pCont->Release();
		if( !SUCCEEDED(hr) ) 
		{ 
			printf("Create User Faile\n");
			break;
		}
		hr = pDisp->QueryInterface( IID_IADs, (void**) &pADs );	
		pDisp->Release();
		if ( !SUCCEEDED(hr) ) 
		{ 
			break;
		}		
		if(pADs)
		{
			pADs->SetInfo(); // Commit
			pADs->Release(); // Release
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
			break;
		}
		//	if(!AddUserToGroup("Debugger Users",sUser))
		//	{
		//		break;
		//	}

		if(!ChangeUserPsw(sUser,sPsw))
		{
			printf("ChangeUserPsw Faile\n");
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

#if 1
//----------------------------------------------------
//删除用户
//
BOOL CWinUserMgr::DeleteUser(char* sUser)
{
	DWORD			ll_len = 255;
	char			szComputerName[255];
	::GetComputerName(szComputerName, &ll_len);

	BOOL			bRet = FALSE;
	HRESULT			hr;
	IADsContainer	*pCont = NULL;
	IADs			*pADs = NULL;
	IDispatch		*pDisp = NULL;

	_bstr_t			bstr, bstr2;
	bstr="WinNT://"+_bstr_t(szComputerName);

	do
	{
		hr = ADsGetObject(	bstr, 
			IID_IADsContainer, 
			(void**) &pCont 
			);
		if (!SUCCEEDED(hr) )
		{
			CCommonFun::WriteFileLog("hr = %x, err=%d", hr, GetLastError());
			break;
		}
		//----添加用户
		hr = pCont->Delete(L"user",_bstr_t(sUser));
		pCont->Release();
		if( !SUCCEEDED(hr) ) 
		{ 
			CCommonFun::WriteFileLog("hr = %x, err=%d, user=%s", hr, GetLastError(), sUser);
			//printf("Create User Faile\n");
			break;
		}
		
#if 1
		//删除用户文件
		char szProfilePath[MAX_PATH + 1];
		DWORD dwLength = MAX_PATH;
		if (!GetProfilesDirectory(szProfilePath, &dwLength))
		{
			CCommonFun::WriteFileLog("Failed to get profileDirectory, err = %d", GetLastError());
			return bRet;
		}
		strcat(szProfilePath, "\\");
		strcat(szProfilePath, sUser);
		CCommonFun::DeleteDir(szProfilePath);
#endif

		bRet = TRUE;

	}while(false);
	return bRet;	
}
#else

//----------------------------------------------------
//删除用户
//
BOOL CWinUserMgr::DeleteUser(char* sUser)
{
#define heapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define heapfree(x)  (HeapFree(GetProcessHeap(), 0, x))
	char *         szDomain       = NULL;
	LPVOID         pUserSID       = NULL;
	DWORD			ll_len = 255;
	char			szComputerName[255];
	::GetComputerName(szComputerName, &ll_len);
	do 
	{
		SID_NAME_USE   snuType;
		DWORD          cbDomain       = 0;
		DWORD          cbUserSID      = 0;
		BOOL bRet  = LookupAccountName((LPSTR) NULL, sUser,
			pUserSID, &cbUserSID, NULL, &cbDomain, NULL);
		if (cbUserSID == 0 || cbDomain == 0)
			break;
		//移到删除前面
		pUserSID = heapalloc(cbUserSID);
		szDomain = (char *) heapalloc(cbDomain);
		bRet = LookupAccountName((LPSTR) NULL, sUser,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		if (!bRet)
			break;
		//os below windows 2000 must load userenv.dll dynamically.
		bRet = DeleteProfile((LPCSTR)pUserSID, NULL, szComputerName);
		CCommonFun::WriteFileLog("delete profile ret:%d, err = %d", bRet, GetLastError());
	}while(false);

	BOOL			bRet = FALSE;
	HRESULT			hr;
	IADsContainer	*pCont = NULL;
	IADs			*pADs = NULL;
	IDispatch		*pDisp = NULL;

	_bstr_t			bstr, bstr2;
	bstr="WinNT://"+_bstr_t(szComputerName);

	do
	{
		hr = ADsGetObject(	bstr, 
			IID_IADsContainer, 
			(void**) &pCont 
			);
		if (!SUCCEEDED(hr) )
		{
			break;
		}
		//----添加用户
		hr = pCont->Delete(L"user",_bstr_t(sUser));
		pCont->Release();
		if( !SUCCEEDED(hr) ) 
		{ 
			printf("Create User Faile\n");
			break;
		}

		bRet = TRUE;

	}while(false);

	//删除用户文件
	//szUserName = "everyone";

	if (pUserSID)
		heapfree(pUserSID);
	if (szDomain)
		heapfree(szDomain);

	return bRet;	
}
#endif
//----------------------------------------
//隐藏账户
//
void CWinUserMgr::HideUser(char* sUser)
{
	CRegistry2 reg(2,FALSE);
	CString sPath;
	sPath.Format("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList");
	if (reg.CreateKey(sPath))
	{
		reg.Write(sUser,0);
	}
}

//显示账户
void CWinUserMgr::ShowUser(char* sUser)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString sPath;
	sPath.Format("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList");
	if (RegOpenKeyEx (hKey, sPath, 0L,
		KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, sUser);
	}
}


BOOL CWinUserMgr::PopAllGroups()
{
	HRESULT hr = S_OK;

	CString csName,csFullName,csDesc,csHomeDir,csProfile,csLoginScript;

	do 
	{
		//Get the container object
		if (!GetADsCnt())
		{
			break;
		}
		//Now Enumerate through the container
		IEnumVARIANT* pEnum = NULL; 
		ADsBuildEnumerator (m_pADsCnt,&pEnum);
		int cnt=0;
		IADsGroup* pChild = NULL;
		while(1)
		{
			_variant_t vChild;
			hr = ADsEnumerateNext (pEnum,1,&vChild,NULL);

			//Iterate as long as you get S_OK 
			if(hr!=S_OK)
				break;
			hr = vChild.pdispVal->QueryInterface (IID_IADsGroup,(void **)&pChild);
			if(hr!=S_OK)
				continue;

			//If the object in the container is user then get properties
			BSTR bstrName;
			pChild->get_Name(&bstrName);
			csName=bstrName;
			
			pChild->Release();
		}	
		ADsFreeEnumerator(pEnum);
	} while (0);
	return 1;
}

void CWinUserMgr::ReleaseADs()
{
	//auto release
}

BOOL CWinUserMgr::GetADsCnt()
{
	if(m_pADsCnt)
	{
		return TRUE;
	}
	//Get the Information about system name and domain name
	IADsWinNTSystemInfo *pNTsys = NULL;
	HRESULT hr = CoCreateInstance(CLSID_WinNTSystemInfo,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IADsWinNTSystemInfo,
		(void**)&pNTsys);

	if (hr != S_OK)
	{
		return FALSE;
	}

	BSTR bstrCompName;
	CString sCompName;
	CString sDomainName;
	CString sDCName;
	pNTsys->get_ComputerName(&bstrCompName);
	if (SUCCEEDED(hr)) 
	{
		sCompName=bstrCompName;
		SysFreeString(bstrCompName);
	}

	BSTR bstrDomainName;
	pNTsys->get_DomainName(&bstrDomainName);
	if (SUCCEEDED(hr)) 
	{
		sDomainName = bstrDomainName;
		SysFreeString(bstrDomainName);
	}

	BSTR bstrDC;
	hr = pNTsys->get_PDC(&bstrDC);
	if (SUCCEEDED(hr)) 
	{
		sDCName = bstrDC;
		SysFreeString(bstrDC);
	}

	pNTsys->Release();
	CString sADSPath;
	if(sDCName.IsEmpty())
	{
		sADSPath.Format("WinNT://%s",sDomainName);
	}
	else
	{
		sADSPath.Format("WinNT://%s/%s",sDomainName,sCompName);
	}
	hr= ADsGetObject(sADSPath.AllocSysString(),IID_IADsContainer,(void **)&m_pADsCnt);
	if (SUCCEEDED(hr))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CWinUserMgr::PopAllUsers()
{
	HRESULT hr = S_OK;

	CString csName,csFullName,csDesc,csHomeDir,csProfile,csLoginScript;
	CStringArray grpAry;
	do 
	{
		//Get the container object
		if (!GetADsCnt())
		{
			break;
		}
		//Now Enumerate through the container
		IEnumVARIANT* pEnum = NULL; 
		ADsBuildEnumerator (m_pADsCnt,&pEnum);
		int cnt=0;
		IADsUser* pChild = NULL;
		while(1)
		{
			_variant_t vChild;
			hr = ADsEnumerateNext (pEnum,1,&vChild,NULL);

			//Iterate as long as you get S_OK 
			if(hr!=S_OK)
				break;
			hr = vChild.pdispVal->QueryInterface (IID_IADsUser,(void **)&pChild);
			if(hr!=S_OK)
				continue;

			//If the object in the container is user then get properties
			BSTR bstrName; 
			pChild->get_Name(&bstrName);
			csName=bstrName;
			SysFreeString(bstrName);

			BSTR bstrDesc;
			pChild->get_Description(&bstrDesc);
			csDesc=bstrDesc;
			SysFreeString(bstrDesc);

			BSTR bstrFullName;
			pChild->get_FullName(&bstrFullName);
			csFullName=bstrFullName;
			SysFreeString(bstrFullName);

			BSTR bstrHomeDir; 
			pChild->get_HomeDirectory(&bstrHomeDir);
			csHomeDir=bstrHomeDir;
			SysFreeString(bstrHomeDir);

			BSTR bstrProfile; 
			pChild->get_Profile(&bstrProfile);
			csProfile=bstrProfile;
			SysFreeString(bstrProfile);

			BSTR bstrLoginScript; 
			pChild->get_LoginScript(&bstrLoginScript);
			csLoginScript=bstrLoginScript;
			SysFreeString(bstrLoginScript);

			VARIANT_BOOL bAccountDisabled;
			pChild->get_AccountDisabled(&bAccountDisabled); 

			VARIANT_BOOL bAccountLocked;
			pChild->get_IsAccountLocked (&bAccountLocked); 

			//get user belong to group
			grpAry.RemoveAll();
			GetGroupsOfUser(pChild,grpAry);

			pChild->Release();

		}	
		ADsFreeEnumerator(pEnum);
	} while (0);
	return 1;
}

BOOL CWinUserMgr::GetGroupsOfUser(IADsUser* pUser,CStringArray& ary)
{
	ASSERT(pUser);
	IADsMembers *pGroups = NULL;
	HRESULT hr = S_OK;
	hr = pUser->Groups(&pGroups);
	//pChild->Release();

	IUnknown *pUnk = NULL;
	hr = pGroups->get__NewEnum(&pUnk);
	pGroups->Release();

	do 
	{
		IEnumVARIANT *pEnum;
		hr = pUnk->QueryInterface(IID_IEnumVARIANT,(void**)&pEnum);
		if (FAILED(hr)) 
		{
			break;
		}
		pUnk->Release();

		// Now Enumerate
		BSTR bstr;
		VARIANT var;
		IADs *pADs;
		ULONG lFetch = 0;
		IDispatch *pDisp;
		CString sGroup;
		VariantInit(&var);
		hr = pEnum->Next(1, &var, &lFetch);
		while(hr == S_OK)
		{
			if (lFetch == 1)
			{
				pDisp = V_DISPATCH(&var);
				pDisp->QueryInterface(IID_IADs, (void**)&pADs);
				pADs->get_Name(&bstr);

				//group name
				sGroup = bstr;
				//
		//		TRACE("aaa=%s",sGroup);
				ary.Add(sGroup);

				SysFreeString(bstr);
				pADs->Release();
			}
			VariantClear(&var);
			pDisp=NULL;
			hr = pEnum->Next(1, &var, &lFetch);
		}
		hr = pEnum->Release();
	} while (0);
	return TRUE;
}