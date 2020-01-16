#pragma once

/************************************************************************/
/* 
	CGhostScriptWrap类是调用了GhostScriptWrapLib.dll的接口，
	而GhostScriptWrapLib.dll是GhostScript的包装类库。
*/
/************************************************************************/

class _AA_DLL_EXPORT_ CGhostScriptWrap
{
public:
	CGhostScriptWrap(void);
	~CGhostScriptWrap(void);

	//下列接口：PdfToTiff、PdfToTxt，PdfToPs和PsToPdf的参数nFirstPage和nLastPage，
	//表示转换范围，指定起始和结束页码(从1开始,0表示所有页面)。

	static BOOL PdfToTiff(CString szPdfFilePath, CString szTiffFilePath, BOOL bColor = TRUE, int nDpi = Image_Deal_Max_Dpi, int nFirstPage = 0, int nLastPage = 0);
	static BOOL PdfToTxt(CString szPdfFilePath, CString szTxtFilePath, int nFirstPage = 0, int nLastPage = 0);
	static BOOL PdfToPs(CString szPdfFilePath, CString szPsFilePath, int nFirstPage = 0, int nLastPage = 0);
	static BOOL PsToPdf(CString szPsFilePath, CString szPdfFilePath, int nFirstPage = 0, int nLastPage = 0);
};
