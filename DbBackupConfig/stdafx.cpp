
// stdafx.cpp : 只包括标准包含文件的源文件
// DbBackupConfig.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

CString LoadS(UINT nID)
{
	CString s;
	s.LoadString(nID);
	return s;
}



