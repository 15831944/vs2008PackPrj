// WebInf.h : WebInf DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWebInfApp
// �йش���ʵ�ֵ���Ϣ������� WebInf.cpp
//

class CWebInfApp : public CWinApp
{
public:
	CWebInfApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
