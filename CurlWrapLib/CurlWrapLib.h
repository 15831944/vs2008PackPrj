// CurlWrapLib.h : CurlWrapLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCurlWrapLibApp
// �йش���ʵ�ֵ���Ϣ������� CurlWrapLib.cpp
//

class CCurlWrapLibApp : public CWinApp
{
public:
	CCurlWrapLibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
