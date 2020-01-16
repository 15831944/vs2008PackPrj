#ifndef _ECCHEADER_404E0530_5CE8_4D95_9F3F_013F79B22316_H_
#define _ECCHEADER_404E0530_5CE8_4D95_9F3F_013F79B22316_H_

#ifdef  _AA_DLL_
#define _AA_DLL_EXPORT_		__declspec(dllexport)
#define _AA_DLL_EXPORT_C_ 	extern "C" __declspec(dllexport)
#else
#define _AA_DLL_EXPORT_		__declspec(dllimport)
#define _AA_DLL_EXPORT_C_	extern "C" __declspec(dllimport)
#endif

_AA_DLL_EXPORT_ BOOL GetAccountPolicyInfoEx(OUT char* pData1, OUT char* pData2, OUT char* pData3, OUT char* pData4, OUT char* pData5, OUT char* pData6, OUT char* pData7, OUT char* pData8);		//获取本机的软件注册参数
_AA_DLL_EXPORT_ BOOL GetAccountPolicy(IN int nData);			//检测加密是否合法
_AA_DLL_EXPORT_ BOOL GetAccountPolicyVersion(OUT int* nData);	//获取是否为正式版本,0-演示版，1-正式版
_AA_DLL_EXPORT_ BOOL GetAccountPolicyCount(OUT int* nData);		//获取加密点数
_AA_DLL_EXPORT_ BOOL GetAccountPolicyOver(OUT char* pData);		//获取加密截止有效时间
_AA_DLL_EXPORT_ BOOL GetAccountPolicyInfo(OUT char* pData);		//获取版本信息,(pData要足够大的缓冲区)
_AA_DLL_EXPORT_ BOOL InstallA();		//注册当前机器为身份证打印测试版
_AA_DLL_EXPORT_ BOOL InstallB();		//注册当前机器为云打印测试版
_AA_DLL_EXPORT_ BOOL InstallC();		//注册当前机器为常规打印测试版
_AA_DLL_EXPORT_ CString EncryptString(IN CString szOrgString);		//加密字符串
_AA_DLL_EXPORT_ CString DecryptString(IN CString szEncryptString);	//解密字符串

#endif	//_ECCHEADER_404E0530_5CE8_4D95_9F3F_013F79B22316_H_