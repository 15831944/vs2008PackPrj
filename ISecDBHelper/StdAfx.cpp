// stdafx.cpp : source file that includes just the standard includes
//	AsSpy.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
CString LoadS(UINT nID)
{
	CString s;
	s.LoadString(nID);
	return s;
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
