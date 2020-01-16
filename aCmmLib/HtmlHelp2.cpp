#include "stdafx.h"
#include "HtmlHelp2.h"
#include <Htmlhelp.h>		//帮助API
#pragma comment(lib,"htmlhelp.lib")
CHtmlHelp::CHtmlHelp(void)
{
}

CHtmlHelp::~CHtmlHelp(void)
{
}

void CHtmlHelp::SetHelpWndTitle(CString sName)
{
	m_sWndName = sName;
}

void CHtmlHelp::OpenHelp(CString sIndex)
{
	HWND hWnd = ::FindWindow(NULL,m_sWndName);
	if(hWnd != NULL)	//帮助已经打开则直接显示
	{
		//构造注册表类并读取注册表	
		CRegistry2 reg(2,FALSE);
		CString ss;	
		ss = PROJECT_REGISTY_KEY;				//公共根键
		CString sLocationKeywords;
		if(reg.Open(ss))
		{
			//打开后读取数值
			if(!reg.Read(_T("HelpLocationKeywords"),sLocationKeywords))
			{
				sLocationKeywords.Empty();
			}
		}
		if(sLocationKeywords.CompareNoCase(sIndex) == 0)		//关键字相同则直接返回
		{
			RECT rc;
			GetWindowRect(hWnd,&rc);
			::SetWindowPos(hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,0);		//显示
			reg.Close();
			return;
		}
		else		//将新关键字写入注册表
		{
			reg.Write(_T("HelpLocationKeywords"),sIndex);
			reg.Close();
		}
	}

	CString sHelpFilePath;
	sHelpFilePath.Format(_T("%shelp.chm"),CCommonFun::GetDefaultPath());
	
	HH_AKLINK link;
	link.cbStruct =     sizeof(HH_AKLINK) ;
	link.fReserved =    FALSE ;
	link.pszKeywords =  sIndex ; 
	link.pszUrl =       NULL ; 
	link.pszMsgText =   NULL ; 
	link.pszMsgTitle =  NULL ; 
	link.pszWindow =    NULL ;
	link.fIndexOnFail = TRUE ;

	HtmlHelp(
		GetDesktopWindow(),
		sHelpFilePath,
		HH_KEYWORD_LOOKUP,				//通过索引来定位到具体的页面
		(DWORD)&link);
}

