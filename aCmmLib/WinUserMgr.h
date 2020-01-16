#pragma once

#include <Iads.h>
#include <Adshlp.h>
#include <lm.h>
#include <assert.h>




// #include <stdio.h>
// 
// #include <ntsecapi.h>


// #define RTN_OK 0
// #define RTN_USAGE 1
// #define RTN_ERROR 13

// 
// If you have the ddk, include ntstatus.h.
// 
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#endif

typedef _com_ptr_t<_com_IIID<IADsContainer,&IID_IADsContainer>	>  IADsContainerPtr;
typedef _com_ptr_t<_com_IIID<IADs,&IID_IADs>	>  IADsPtr;
typedef _com_ptr_t<_com_IIID<IADsUser,&IID_IADsUser>	>  IADsUserPtr;

class _AA_DLL_EXPORT_ CWinUserMgr
{
public:
	CWinUserMgr(void);
	~CWinUserMgr(void);

	BOOL ChangeUserPsw(char* szUser,char* sPsw);
	

	BOOL AddUser(char* sUser,char* sPsw,char* sGrp = NULL);
	BOOL AddUserToGroup(char* sGroup,char* sUser);
	BOOL DeleteUser(char* sUser);
	void HideUser(char* sUser);
	void ShowUser(char* sUser);
	BOOL LockUser(char* sUser,BOOL bLock = TRUE);
	BOOL DisableUser(char* sUser,BOOL bDisable = TRUE);

	BOOL IsGroupMembers(IADsGroup *pGroup,char* sUser);
	BOOL EnumMembers(IADsMembers *pMembers,char* sUser);

	BOOL PopAllGroups();
	BOOL PopAllUsers();
	
	BOOL GetADsCnt();
	void ReleaseADs();
	BOOL GetGroupsOfUser(IADsUser* pUser,CStringArray& ary);

	IADsContainerPtr m_pADsCnt;
};
