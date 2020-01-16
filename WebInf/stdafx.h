// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

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
#include <afxsock.h>		// MFC socket extensions
#include "../Public/Global.h"
#include "../Public/Include/glCmdAndStructDef.h"
#include "../Public/Include/glCmdAndStructPrint2.h"
#include "../public/Include/NetCmdAndStructDef.h"
#include "../Public/Include/printersvctDef.h"
#include "../Public/WebNotifyWaitAck.h"
#include "../Public/xabzipPort.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <shlwapi.h>

#include "Base64.h"
#include "mem_image.h"
#include "ParseSpl.h"
#include "ParseShd.h"

//#define ENABLE_PWD_CRY
// #pragma comment(lib,"../release/encryptlib.lib")
#include "../encryptlib/PWDCry.h"
//#include "Encrypt/"
extern void WriteLogEx(LPCTSTR lpszFormat, ...);
extern int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
extern int GetEncoderClsidEx(const CString imagePath, CLSID* pClsid);
//��ѹ��ѹ���ļ�
extern BOOL UnCompressGzFile(IN CString szFileGzPath, OUT CString& szFileOrgPath, BOOL bDelGzFile);
extern BOOL CompressGzFile(IN CString szFileOrg, OUT CString& szFileGzPath, BOOL bDelOrgzFile);

//�ļ���������base64����ת��
extern BOOL FileToBase64(IN CString szFilePath, OUT CString& szBase64);
extern BOOL Base64ToFile(IN CString& szBase64, IN CString szFilePath);

extern CString GetFileExt(CString sFile);
extern CString GetFileExt2(CString sFile);
extern BOOL GetFileExt3(CString szFile, CString &szFileName, CString &szExtName);


#define iSecASvcHost_Name	TEXT("iSecASvcHost")		//��ҵ�����ķ�����
#define iSecBSvcHost_Name	TEXT("iSecBSvcHost")		//У԰�����ķ�����
#define PrintDbSvcHost_Name	TEXT("PrintDbSvcHost")		//��־������
#define iSecHttpHost_Name	TEXT("iSecHttpHost")		//����������
#define ISecDbHelper_Name	TEXT("ISecDbHelper")		//���ݿⱸ�ݷ�����

#define iSecASvcHost_Index	1			//��ҵ�����ķ�����
#define iSecBSvcHost_Index	2			//У԰�����ķ�����
#define PrintDbSvcHost_Index	3		//��־������
#define iSecHttpHost_Index	4			//����������
#define ISecDbHelper_Index	5			//���ݿⱸ�ݷ�����

//�����ɴ�ӡ�ĵ���ʽ
#define Cloud_Doc_Format_Suffix_Word				TEXT(".doc")		//office word 97-2003
#define Cloud_Doc_Format_Suffix_Word_GZ				TEXT(".doc.gz")		//office word 97-2003 ѹ����
#define Cloud_Doc_Format_Suffix_WordX				TEXT(".docx")		//office word 2007-later
#define Cloud_Doc_Format_Suffix_WordX_GZ			TEXT(".docx.gz")	//office word 2007-later ѹ����
#define Cloud_Doc_Format_Suffix_PowerPoint			TEXT(".ppt")		//office PowerPoint 97-2003
#define Cloud_Doc_Format_Suffix_PowerPoint_GZ		TEXT(".ppt.gz")		//office PowerPoint 97-2003 ѹ����
#define Cloud_Doc_Format_Suffix_PowerPointX			TEXT(".pptx")		//office PowerPoint 2007-later
#define Cloud_Doc_Format_Suffix_PowerPointX_GZ		TEXT(".pptx.gz")	//office PowerPoint 2007-later ѹ����
#define Cloud_Doc_Format_Suffix_Excel				TEXT(".xls")		//office Excel 97-2003
#define Cloud_Doc_Format_Suffix_Excel_GZ			TEXT(".xls.gz")		//office Excel 97-2003 ѹ����
#define Cloud_Doc_Format_Suffix_ExcelX				TEXT(".xlsx")		//office Excel 2007-later
#define Cloud_Doc_Format_Suffix_ExcelX_GZ			TEXT(".xlsx.gz")	//office Excel 2007-later ѹ����
#define Cloud_Doc_Format_Suffix_PDF					TEXT(".pdf")		//Adobe Portable Document Format (PDF)
#define Cloud_Doc_Format_Suffix_PDF_GZ				TEXT(".pdf.gz")		//Adobe Portable Document Format (PDF) ѹ����
#define Cloud_Doc_Format_Suffix_XPS					TEXT(".xps")		//Microsoft XML Paper Specification (XPS)
#define Cloud_Doc_Format_Suffix_XPS_GZ				TEXT(".xps.gz")		//Microsoft XML Paper Specification (XPS) ѹ����
#define Cloud_Doc_Format_Suffix_WMF					TEXT(".wmf")		//Windows Metafile Format (WMF)
#define Cloud_Doc_Format_Suffix_WMF_GZ				TEXT(".wmf.gz")		//Windows Metafile Format (WMF) ѹ����
#define Cloud_Doc_Format_Suffix_EMF					TEXT(".emf")		//Windows Enhanced MetaFile format (EMF)
#define Cloud_Doc_Format_Suffix_EMF_GZ				TEXT(".emf.gz")		//Windows Enhanced MetaFile format (EMF) ѹ����
#define Cloud_Doc_Format_Suffix_PS					TEXT(".ps")			//Adobe PostScript (PS)
#define Cloud_Doc_Format_Suffix_PS_GZ				TEXT(".ps.gz")		//Adobe PostScript (PS) ѹ����
#define Cloud_Doc_Format_Suffix_PCL					TEXT(".pcl")		//Printer Control Language (PCL)
#define Cloud_Doc_Format_Suffix_PCL_GZ				TEXT(".pcl.gz")		//Printer Control Language (PCL) ѹ����
#define Cloud_Doc_Format_Suffix_SPL					TEXT(".spl")		//Microsoft? Windows Spool File Format (SPL) (������pcl/ps/emf/xps�ȸ�ʽ)
#define Cloud_Doc_Format_Suffix_SPL_GZ				TEXT(".spl.gz")		//Microsoft? Windows Spool File Format (SPL) (������pcl/ps/emf/xps�ȸ�ʽ) ѹ����
#define Cloud_Doc_Format_Suffix_EMF_SPL				TEXT(".emf.spl")		//Microsoft? Windows Spool File Format (SPL) (������pcl/ps/emf/xps�ȸ�ʽ)
#define Cloud_Doc_Format_Suffix_EMF_SPL_GZ			TEXT(".efm.spl.gz")		//Microsoft? Windows Spool File Format (SPL) (������pcl/ps/emf/xps�ȸ�ʽ) ѹ����
#define Cloud_Doc_Format_Suffix_JPG					TEXT(".jpg")		//Joint Photographic Experts Group������ͼ��ר��С��/JPEG��
#define Cloud_Doc_Format_Suffix_JPG_GZ				TEXT(".jpg.gz")		//Joint Photographic Experts Group������ͼ��ר��С��/JPEG�� ѹ����
#define Cloud_Doc_Format_Suffix_JPEG				TEXT(".jpeg")		//Joint Photographic Experts Group������ͼ��ר��С��/JPEG��
#define Cloud_Doc_Format_Suffix_JPEG_GZ				TEXT(".jpeg.gz")	//Joint Photographic Experts Group������ͼ��ר��С��/JPEG�� ѹ����
#define Cloud_Doc_Format_Suffix_TIF					TEXT(".tif")		//Tagged Image File Format����ǩͼ���ļ���ʽ/TIFF��
#define Cloud_Doc_Format_Suffix_TIF_GZ				TEXT(".tif.gz")		//Tagged Image File Format����ǩͼ���ļ���ʽ/TIFF�� ѹ����
#define Cloud_Doc_Format_Suffix_TIFF				TEXT(".tiff")		//Tagged Image File Format����ǩͼ���ļ���ʽ/TIFF��
#define Cloud_Doc_Format_Suffix_TIFF_GZ				TEXT(".tiff.gz")	//Tagged Image File Format����ǩͼ���ļ���ʽ/TIFF�� ѹ����
#define Cloud_Doc_Format_Suffix_PNG					TEXT(".png")		//Portable Network Graphic Format (����ֲ����ͼ�θ�ʽ/PNG)
#define Cloud_Doc_Format_Suffix_PNG_GZ				TEXT(".png.gz")		//Portable Network Graphic Format (����ֲ����ͼ�θ�ʽ/PNG) ѹ����
#define Cloud_Doc_Format_Suffix_BMP					TEXT(".bmp")		//Bitmap (λͼ)
#define Cloud_Doc_Format_Suffix_BMP_GZ				TEXT(".bmp.gz")		//Bitmap (λͼ) ѹ����
#define Cloud_Doc_Format_Suffix_GIF					TEXT(".gif")		//Graphics Interchange Format (ͼ�񻥻���ʽ/GIF)
#define Cloud_Doc_Format_Suffix_GIF_GZ				TEXT(".gif.gz")		//Graphics Interchange Format (ͼ�񻥻���ʽ/GIF) ѹ����

#define Cloud_Doc_Format_Suffix_GZ					TEXT(".gz")			//���ύ�ĵ���ѹ����ʽ�ĺ�׺


