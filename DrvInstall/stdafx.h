// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once
#define  _BIND_TO_CURRENT_VCLIBS_VERSION 1
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _AMD64_
#include <newdev.h>
#pragma comment(lib,"Newdev.lib")
#pragma message("Newdev on _AMD64_")
#else
#include "newdev.h"
#pragma comment(lib,"Newdevx32.lib")
#endif


#pragma warning(disable : 4996)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4200)

#include <winsvc.h>
#include <SetupAPI.h>
#include "SetupEx.h"
#include "RegistryEx.h"
#include "DriverOp.h"
#include "../Public/Global.h"
#include "../Public/VirtualDriverDef.h"
extern void WriteLogEx(LPCTSTR lpszFormat, ...);
extern BOOL MfgCallback(CInf* inf, const CString& mfgname, const CString& modelname, PVOID context, DWORD& code);
extern BOOL DeviceCallback(CInf* inf, const CString& devname, const CString& instsecname, const CStringArray& devid, PVOID context, DWORD& code);

extern int RemoveHardware(LPCTSTR lpszHardwareID);
extern void RegDriverStopState();
extern void RegUnloadInf(LPCTSTR lpszRunOnceInfo,LPCTSTR lpszExePath);
extern BOOL IsWow64();
extern BOOL IsWinVistaOrLater();
extern void StringSplit(const CString sSource, CStringArray *parArray , CString sSplitter,BOOL bTriNUll);

#include "IniFile.h"

extern CIniFile theIniFile;

#ifndef CP_GB2312
#define  CP_GB2312	936	//Code Page Identifiers ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
#endif
