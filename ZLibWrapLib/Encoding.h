//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Encoding.h
//    Author:      Streamlet
//    Create Time: 2010-09-15
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __ENCODING_H_B23F48D1_9E5C_435B_9BDA_B6370F9BAD62_INCLUDED__
#define __ENCODING_H_B23F48D1_9E5C_435B_9BDA_B6370F9BAD62_INCLUDED__


#include <Windows.h>
#include <atlstr.h>

#ifndef CP_GB2312
#define  CP_GB2312	936	//Code Page Identifiers ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
#endif

CStringW ANSIToUCS2(const CStringA &strANSI, UINT uCodePage = CP_GB2312);
CStringA UCS2ToANSI(const CStringW &strUCS2, UINT uCodePage = CP_GB2312);
CStringA UCS2ToANSI(const LPCTSTR lpstr, UINT uCodePage = CP_GB2312);


#endif // #ifndef __ENCODING_H_B23F48D1_9E5C_435B_9BDA_B6370F9BAD62_INCLUDED__
