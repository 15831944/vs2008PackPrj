//msxcp.h

#ifndef _ASCPPORT_

#ifdef  _ASCP_DLL_
#define _ASCP_PORT_		__declspec(dllexport)
#define _ASCP_PORT_C_ 	extern "C" __declspec(dllexport)
#else
#define _ASCP_PORT_		__declspec(dllimport)
#define _ASCP_PORT_C_	extern "C" __declspec(dllimport)
#endif

// flag for ORD_ABA()
#define		ABA_FLAG_ADDPHOTO		0x00000001  //see follow
#define		ABA_FLAG_DELPHOTO		0x00000002  //single using
#define		ABA_FLAG_GETPHOTOS		0x00000004  //single using

#define		ABA_FLAG_CREATEKEY		0x00010000  //Combined with ABA_FLAG_ADDPHOTO
// *pOutBuff:: LOWORD==KeyNo(1,2,..), HIWORD==KeySize(BYTE=1,WORD=2,DWORD=4)
// <sample i.e> to protect user sum: 
// pOutBuff = 0x00010002
// ORD_ABA(&m_pUserSum,sizeof(DWORD),ABA_FLAG_ADDPHOTO|ABA_FLAG_CREATEKEY,pOutBuff);

//#define		ABA_FLAG_ERREXIT		0x10000000  //Combined with ABA_FLAG_ADDPHOTO

// define
#define		ABA_MAXPHOTO			64			//max photos limit to add
#define		ABA_MAXMEMSIZE			(1024*16)   //max memory size for one photo
#define     ACA_FACTOR              0x15376932

_ASCP_PORT_ int ORD_ACA1(DWORD dwPwnd); //initialize
_ASCP_PORT_ int ORD_ACB1(); //exit
_ASCP_PORT_ void ORD_ABA1(LPVOID* ppMemory,DWORD dSize,DWORD dFlag,LPDWORD pOutBuff); //for photo operation
_ASCP_PORT_ int ORD_ACC1(int iRetMode);
_ASCP_PORT_ int ORD_ACD1(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
_ASCP_PORT_ int ORD_ACE1(DWORD dwCrcWnd,DWORD pOutBuffer,DWORD dwOutSize);
_ASCP_PORT_ int ORD_ACF1(DWORD pInBuffer,DWORD dwInSize);

//ppMemory: photo memory
//dSize: photo memory size
//dFlag: above flag
//pOutBuff: return value:DWORD;	 success: >0; fail:<=0

class _ASCP_PORT_ CSrpComAs
{
public:
	CSrpComAs();
   ~CSrpComAs();
public:
	int SrpCom01(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
	int SrpCom02(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
	int SrpCom03(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
	int SrpCom04(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
	int SrpCom05(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
	int SrpCom06(CWnd* pWnd);
public:
	int ORD_ACA(DWORD dwPwnd); //initialize
	int ORD_ACB(); //exit
	void ORD_ABA(LPVOID* ppMemory,DWORD dSize,DWORD dFlag,LPDWORD pOutBuff); //for photo operation
	int ORD_ACC(int iRetMode);
	int ORD_ACD(CWnd* pWnd,LPVOID* ppOutBuffer,LPDWORD pOutSize,LPBYTE pInBuffer,DWORD dwSize);
	int ORD_ACE(DWORD dwCrcWnd,DWORD pOutBuffer,DWORD dwOutSize);
	int ORD_ACF(DWORD pInBuffer,DWORD dwInSize);
	int ORD_ACG(DWORD dwACG);
	int ORD_ACH();
	int ORD_ACI_Saturn();
};

#endif