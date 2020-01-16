// XpsHelper.h : XpsHelper DLL 的主头文件
//

#pragma once

#ifdef  _AA_DLL_
#define _AA_DLL_EXPORT_		__declspec(dllexport)
#define _AA_DLL_EXPORT_C_ 	extern "C" __declspec(dllexport)
#else
#define _AA_DLL_EXPORT_		__declspec(dllimport)
#define _AA_DLL_EXPORT_C_	extern "C" __declspec(dllimport)
#endif

#include <windows.h>
#include <shlwapi.h>
#include <cstdio>

#include "PrintProcessDef.h"

class _AA_DLL_EXPORT_ CXpsHelper
{
public:
	CXpsHelper();
	~CXpsHelper();

	//打印xps到指定打印机cPrinterName
	BOOL PrintToPrinter(const char *cPrinterName,const char *cDocName,const char *cXpsFilePath,
		int nBeginPage = -1, int nEndPage = -1, int nPagesPerPaper = 1,
		WORD wColor = DMCOLOR_MONOCHROME, WORD wPaperSize = DMPAPER_A4,
		WORD wOrientation = DMORIENT_PORTRAIT, WORD wCopies = 1,
		WORD wDuplex = DMDUP_SIMPLEX, WORD wPaperSource = DMBIN_AUTO,
		FPrintProcessCallBack callBack = NULL, 
		PrintProcessCallBackData* pCallBackData = NULL);

	void PrintProcess(BOOL bError, int nPrintedPage, BOOL bPrintComplete);

	//获取指定的xps文件页数
	int GetPageCount(const char *cXpsFilePath);

	//获取指定的xps打印参数
	BOOL GetPrintParam(const char *cXpsFilePath, int& nCopy, int& nPages, int& nColor
		, int& nOrientation, int& nPaperSize, int& nDuplex);

private:
	FPrintProcessCallBack m_pCallBack;
	PrintProcessCallBackData* m_pCallBackData;
};
