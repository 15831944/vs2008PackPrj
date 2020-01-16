// HookProc.h : HookProc DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
class CHookProcApp : public CWinApp
{
public:
	CHookProcApp();

// 重写
public:
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL InitInstance();
};

class CLockCS
{
public:
	CLockCS(CRITICAL_SECTION& cs)
	{
		m_cs= cs;
		EnterCriticalSection(&m_cs);
	}
	~CLockCS()
	{
		LeaveCriticalSection(&m_cs);
	}
	CRITICAL_SECTION m_cs;
};
#define HOOKTHREAD_COUNT  10

//////////////////////////////////////////////////////////////////////////
// 导出函数

BOOL __declspec(dllexport)StartHook(char*);									// 开启反截屏钩子
BOOL __declspec(dllexport)StopHook();									// 关闭反截屏钩子
