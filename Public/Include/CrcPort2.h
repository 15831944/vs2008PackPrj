
#ifndef _CRCPORT2_H_
#define _CRCPORT2_H_

#define AFX_INLINE_API	__stdcall

#ifdef  __cplusplus
extern "C" 
{
#endif

int AFX_INLINE_API InitCrcCom2(HANDLE m_hInstance);
int AFX_INLINE_API InitCrcCom3(HANDLE m_hInstance);
int AFX_INLINE_API ExitCrcCom2();
int AFX_INLINE_API SetErrMode2(int iMode);

#define IWVM_DEMO	1
#define IWVM_STD	2
int AFX_INLINE_API IsWhatVersion_Mercury2();
//ret:: 1=demo 2=std other=error

#ifdef PRO_MERAK_WEB
int AFX_INLINE_API GetDwParamFromSvr(int iDwNo,DWORD * pDwParam);
//ret:: 1==sucess	0==error
#endif

int AFX_INLINE_API AFX_ENTERMODULE_INITCOM10();
int AFX_INLINE_API AFX_ENTERMODULE_INITCOM12();

#ifdef  __cplusplus
}
#endif


#endif //_CRCPORT2_H_
