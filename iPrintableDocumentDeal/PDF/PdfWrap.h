#pragma once

/************************************************************************/
/* 
	CPdfWrap类是调用了PdfHelper.dll的接口，
	而PdfHelper.dll是Popplert和Cairo的包装类库。
*/
/************************************************************************/

class _AA_DLL_EXPORT_ CPdfWrap
{
public:
	CPdfWrap(void);
	~CPdfWrap(void);

	static int GetPdfPageCount(CString csPdfFilePath);
	static BOOL PDFToTiff(CString csPdfFilePath, CString csTiffFilePath, BOOL bColor, int nDpi = Image_Deal_Max_Dpi);

protected:
};
