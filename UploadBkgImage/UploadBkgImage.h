
// UploadBkgImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUploadBkgImageApp:
// �йش����ʵ�֣������ UploadBkgImage.cpp
//

class CUploadBkgImageApp : public CWinAppEx
{
public:
	CUploadBkgImageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUploadBkgImageApp theApp;