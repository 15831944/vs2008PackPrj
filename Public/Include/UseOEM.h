// UseOEM.h: interface for the CUseOEM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USEOEM_H__3536999F_D47F_43B6_ABB8_900EFC27ABFA__INCLUDED_)
#define AFX_USEOEM_H__3536999F_D47F_43B6_ABB8_900EFC27ABFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

typedef int (*PGetCfgValue)(LPCTSTR szCfgName,char* pCfgValueBuf, int nCfgValueBufLen);

#define DEF_VERSION_SYX		1	//苏亚星自己的版本
#define DEF_VERSION_OYI		2	//武汉噢易的OEM版本

class CUseOEM  
{
public:
	/*
		szExeName可以是"Global", "SaturnMain.exe","Client.exe"
    */
	int GetCfgValue(LPCTSTR szCfgName,char* pCfgValueBuf, int nCfgValueBufLen);
	CString GetShortCorpName();
	CString GetFullCorpName();
	CString GetAppTitle();
	CString GetHttp();
	int GetVersion();
	bool CheckNumAndChar(CString &szCheck);
	CUseOEM();
	virtual ~CUseOEM();

private:
	PGetCfgValue m_pfnGetCfgValue;
	HMODULE m_hDll;
	bool m_bInitOK;
};

//字符串键定义
#define DEF_KEY2_GLOBAL_CorpFullName		("公司全称")							//_T("南京苏亚星资讯科技开发有限公司"));
#define DEF_KEY2_GLOBAL_CorpShortName		("公司简称")							//_T("苏亚星")
#define DEF_KEY2_GLOBAL_AppTitle			("程序标题")							//_T("苏亚星机房管理系统 V5.0"));
#define DEF_KEY2_GLOBAL_Http				("网站地址")							//

#endif // !defined(AFX_USEOEM_H__3536999F_D47F_43B6_ABB8_900EFC27ABFA__INCLUDED_)
