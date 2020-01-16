// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
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
//解压、压缩文件
extern BOOL UnCompressGzFile(IN CString szFileGzPath, OUT CString& szFileOrgPath, BOOL bDelGzFile);
extern BOOL CompressGzFile(IN CString szFileOrg, OUT CString& szFileGzPath, BOOL bDelOrgzFile);

//文件二进制与base64编码转换
extern BOOL FileToBase64(IN CString szFilePath, OUT CString& szBase64);
extern BOOL Base64ToFile(IN CString& szBase64, IN CString szFilePath);

extern CString GetFileExt(CString sFile);
extern CString GetFileExt2(CString sFile);
extern BOOL GetFileExt3(CString szFile, CString &szFileName, CString &szExtName);


#define iSecASvcHost_Name	TEXT("iSecASvcHost")		//企业版中心服务器
#define iSecBSvcHost_Name	TEXT("iSecBSvcHost")		//校园版中心服务器
#define PrintDbSvcHost_Name	TEXT("PrintDbSvcHost")		//日志服务器
#define iSecHttpHost_Name	TEXT("iSecHttpHost")		//升级服务器
#define ISecDbHelper_Name	TEXT("ISecDbHelper")		//数据库备份服务器

#define iSecASvcHost_Index	1			//企业版中心服务器
#define iSecBSvcHost_Index	2			//校园版中心服务器
#define PrintDbSvcHost_Index	3		//日志服务器
#define iSecHttpHost_Index	4			//升级服务器
#define ISecDbHelper_Index	5			//数据库备份服务器

//常见可打印文档格式
#define Cloud_Doc_Format_Suffix_Word				TEXT(".doc")		//office word 97-2003
#define Cloud_Doc_Format_Suffix_Word_GZ				TEXT(".doc.gz")		//office word 97-2003 压缩包
#define Cloud_Doc_Format_Suffix_WordX				TEXT(".docx")		//office word 2007-later
#define Cloud_Doc_Format_Suffix_WordX_GZ			TEXT(".docx.gz")	//office word 2007-later 压缩包
#define Cloud_Doc_Format_Suffix_PowerPoint			TEXT(".ppt")		//office PowerPoint 97-2003
#define Cloud_Doc_Format_Suffix_PowerPoint_GZ		TEXT(".ppt.gz")		//office PowerPoint 97-2003 压缩包
#define Cloud_Doc_Format_Suffix_PowerPointX			TEXT(".pptx")		//office PowerPoint 2007-later
#define Cloud_Doc_Format_Suffix_PowerPointX_GZ		TEXT(".pptx.gz")	//office PowerPoint 2007-later 压缩包
#define Cloud_Doc_Format_Suffix_Excel				TEXT(".xls")		//office Excel 97-2003
#define Cloud_Doc_Format_Suffix_Excel_GZ			TEXT(".xls.gz")		//office Excel 97-2003 压缩包
#define Cloud_Doc_Format_Suffix_ExcelX				TEXT(".xlsx")		//office Excel 2007-later
#define Cloud_Doc_Format_Suffix_ExcelX_GZ			TEXT(".xlsx.gz")	//office Excel 2007-later 压缩包
#define Cloud_Doc_Format_Suffix_PDF					TEXT(".pdf")		//Adobe Portable Document Format (PDF)
#define Cloud_Doc_Format_Suffix_PDF_GZ				TEXT(".pdf.gz")		//Adobe Portable Document Format (PDF) 压缩包
#define Cloud_Doc_Format_Suffix_XPS					TEXT(".xps")		//Microsoft XML Paper Specification (XPS)
#define Cloud_Doc_Format_Suffix_XPS_GZ				TEXT(".xps.gz")		//Microsoft XML Paper Specification (XPS) 压缩包
#define Cloud_Doc_Format_Suffix_WMF					TEXT(".wmf")		//Windows Metafile Format (WMF)
#define Cloud_Doc_Format_Suffix_WMF_GZ				TEXT(".wmf.gz")		//Windows Metafile Format (WMF) 压缩包
#define Cloud_Doc_Format_Suffix_EMF					TEXT(".emf")		//Windows Enhanced MetaFile format (EMF)
#define Cloud_Doc_Format_Suffix_EMF_GZ				TEXT(".emf.gz")		//Windows Enhanced MetaFile format (EMF) 压缩包
#define Cloud_Doc_Format_Suffix_PS					TEXT(".ps")			//Adobe PostScript (PS)
#define Cloud_Doc_Format_Suffix_PS_GZ				TEXT(".ps.gz")		//Adobe PostScript (PS) 压缩包
#define Cloud_Doc_Format_Suffix_PCL					TEXT(".pcl")		//Printer Control Language (PCL)
#define Cloud_Doc_Format_Suffix_PCL_GZ				TEXT(".pcl.gz")		//Printer Control Language (PCL) 压缩包
#define Cloud_Doc_Format_Suffix_SPL					TEXT(".spl")		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式)
#define Cloud_Doc_Format_Suffix_SPL_GZ				TEXT(".spl.gz")		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式) 压缩包
#define Cloud_Doc_Format_Suffix_EMF_SPL				TEXT(".emf.spl")		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式)
#define Cloud_Doc_Format_Suffix_EMF_SPL_GZ			TEXT(".efm.spl.gz")		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式) 压缩包
#define Cloud_Doc_Format_Suffix_JPG					TEXT(".jpg")		//Joint Photographic Experts Group（联合图像专家小组/JPEG）
#define Cloud_Doc_Format_Suffix_JPG_GZ				TEXT(".jpg.gz")		//Joint Photographic Experts Group（联合图像专家小组/JPEG） 压缩包
#define Cloud_Doc_Format_Suffix_JPEG				TEXT(".jpeg")		//Joint Photographic Experts Group（联合图像专家小组/JPEG）
#define Cloud_Doc_Format_Suffix_JPEG_GZ				TEXT(".jpeg.gz")	//Joint Photographic Experts Group（联合图像专家小组/JPEG） 压缩包
#define Cloud_Doc_Format_Suffix_TIF					TEXT(".tif")		//Tagged Image File Format（标签图像文件格式/TIFF）
#define Cloud_Doc_Format_Suffix_TIF_GZ				TEXT(".tif.gz")		//Tagged Image File Format（标签图像文件格式/TIFF） 压缩包
#define Cloud_Doc_Format_Suffix_TIFF				TEXT(".tiff")		//Tagged Image File Format（标签图像文件格式/TIFF）
#define Cloud_Doc_Format_Suffix_TIFF_GZ				TEXT(".tiff.gz")	//Tagged Image File Format（标签图像文件格式/TIFF） 压缩包
#define Cloud_Doc_Format_Suffix_PNG					TEXT(".png")		//Portable Network Graphic Format (可移植网络图形格式/PNG)
#define Cloud_Doc_Format_Suffix_PNG_GZ				TEXT(".png.gz")		//Portable Network Graphic Format (可移植网络图形格式/PNG) 压缩包
#define Cloud_Doc_Format_Suffix_BMP					TEXT(".bmp")		//Bitmap (位图)
#define Cloud_Doc_Format_Suffix_BMP_GZ				TEXT(".bmp.gz")		//Bitmap (位图) 压缩包
#define Cloud_Doc_Format_Suffix_GIF					TEXT(".gif")		//Graphics Interchange Format (图像互换格式/GIF)
#define Cloud_Doc_Format_Suffix_GIF_GZ				TEXT(".gif.gz")		//Graphics Interchange Format (图像互换格式/GIF) 压缩包

#define Cloud_Doc_Format_Suffix_GZ					TEXT(".gz")			//云提交文档的压缩格式的后缀


