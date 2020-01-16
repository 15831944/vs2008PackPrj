
#ifndef _CRCPORT_H_
#define _CRCPORT_H_

#define AFX_INLINE_API	__stdcall

#define IDI_NULL	0
#define IDI_DEMO	1
#define IDI_STD  	2

#define IDI_UNLIMITED 54321

#ifndef _CRC_CODE_
extern CSrpComAs sSrpDomAs;
#endif

#ifdef  __cplusplus
extern "C" 
{
#endif

int AFX_INLINE_API InitCrc();
int AFX_INLINE_API ExitCrc();
int AFX_INLINE_API InitCrc2S();
int AFX_INLINE_API InitCrc2(int iVersion,int iTLong);
int AFX_INLINE_API SetErrMode(int iMode);
BOOL AFX_INLINE_API IsSoftIceRuning();
void AFX_INLINE_API SetAppWnd(HWND pAppWnd);
int AFX_INLINE_API SetSubFlag(int iParam);
BOOL AFX_INLINE_API IsDemoVer();
int AFX_INLINE_API GetUserSum(LPDWORD* ppDword);
int AFX_INLINE_API GetUserSumEx(LPDWORD* ppDword);
int AFX_INLINE_API GetSerialNO(LPVOID* ppSerial);
int	AFX_INLINE_API GetDemoTimeLong(LPDWORD * pTimeLong);
int	AFX_INLINE_API GetTimeElapsed(LPDWORD * pTimeLong);
int	AFX_INLINE_API SetTimeElapsed(LPDWORD * pTimeLong);
int AFX_INLINE_API IsSupportDanjiban(LPDWORD* ppDword); //ppDword == blDemoVer
int AFX_INLINE_API IsSupportWangluoban(LPDWORD* ppDword); //ppDword == 0

int AFX_INLINE_API GetKeyDataFromDog(LPVOID* ppData,DWORD dwOffset,DWORD dwSize);
//offset based on .m_bData(0,1,2,..) (==GetExtInfo())
int AFX_INLINE_API GetKeyDataFromFile(LPVOID* ppData,DWORD dwKeyNO,DWORD dwSize);
//dwKeyNo(1,2,3,...8): dwSize(1,2,4,8)

int AFX_INLINE_API CheckDog(LPDWORD* ppDemoFlag);		
int AFX_INLINE_API ChkDogCurrentNO(LPDWORD* ppDemoFlag);
int AFX_INLINE_API ChkDogTime();						
int AFX_INLINE_API CheckSelf();							
int AFX_INLINE_API ChkRnd(LPDWORD* ppDemoFlag);			
int AFX_INLINE_API DynamicChk(LPDWORD* ppUserSum);		
int AFX_INLINE_API ChkRnd2(LPDWORD* ppDemoFlag);			
int AFX_INLINE_API IsMyProduction();
int AFX_INLINE_API RandomCheckEncrypt();
int AFX_INLINE_API RemoteCertX(LPCTSTR pszCert);
int AFX_INLINE_API CheckTimeLimit();
int AFX_INLINE_API IsTrialVer();
int AFX_INLINE_API GetProFlag();
int AFX_INLINE_API NotifyCrcInfo(LPCTSTR pInfo);
int AFX_INLINE_API IsA_L();


//***
int AFX_INLINE_API AFX_ENTERMODULE_INIT1();
int AFX_INLINE_API AFX_ENTERMODULE_INIT2();
int AFX_INLINE_API AFX_ENTERMODULE_INIT3();
int AFX_INLINE_API AFX_ENTERMODULE_INIT4();
int AFX_INLINE_API AFX_ENTERMODULE_INIT5();
int AFX_INLINE_API AFX_ENTERMODULE_INIT6();
int AFX_INLINE_API AFX_ENTERMODULE_INIT7();
int AFX_INLINE_API AFX_ENTERMODULE_INIT8();
int AFX_INLINE_API AFX_ENTERMODULE_INIT9();
int AFX_INLINE_API AFX_ENTERMODULE_INIT10();
int AFX_INLINE_API AFX_ENTERMODULE_INIT11();
int AFX_INLINE_API AFX_ENTERMODULE_INIT12();
int AFX_INLINE_API AFX_ENTERMODULE_INIT12x();
int AFX_INLINE_API AFX_ENTERMODULE_INIT14s();
int AFX_INLINE_API AFX_ENTERMODULE_INIT14s1();
int AFX_INLINE_API AFX_ENTERMODULE_INIT15s();
int AFX_INLINE_API AFX_ENTERMODULE_INIT15s1();
int AFX_INLINE_API AFX_ENTERMODULE_INIT16s();
int AFX_INLINE_API AFX_ENTERMODULE_INIT17();
//***

#define _CRC_DEMOMAXUSER_		10
#define _SECPW_PAUSESVR_TIME1_  (24*7)

#define IWVM_DEMO	1
#define IWVM_STD	2
//int AFX_INLINE_API IsWhatVersion_Mercury();
//1=demo 2=std other=error

//ppIsDemo is Demo Flag, if Demo **ppIsdemo=TRUE, or else =FALSE
//ppWhichKey == 1,2,3,4,5,6,7,8
//ppRetValue is return value
//this function works for above jupiter 5.0(1600)
int AFX_INLINE_API GetKeyEx(LPDWORD* ppIsDemo,LPDWORD* ppWhichKey,LPDWORD* ppRetValue);

//help fuction
//int	AFX_INLINE_API AfxGetRunPath(LPSTR lpPath,int iBuffSize);
int AFX_INLINE_API ReadDataByInit();

//void AFX_INLINE_API CloseApp();

#ifdef  __cplusplus
}
#endif


#endif //_CRCPORT_H_
//link lib:adsiid.lib activeds.lib 