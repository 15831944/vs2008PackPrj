// DrvInstall.h : DrvInstall DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDrvInstallApp
// �йش���ʵ�ֵ���Ϣ������� DrvInstall.cpp
//

class CDrvInstallApp : public CWinApp
{
public:
	CDrvInstallApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
