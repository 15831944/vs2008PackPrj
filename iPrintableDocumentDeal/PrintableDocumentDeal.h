#pragma once

class _AA_DLL_EXPORT_ CPrintableDocumentDeal
{
public:
	CPrintableDocumentDeal(void);
	~CPrintableDocumentDeal(void);

	//获取PDF文档页数,成功返回TRUE,否则返回FALSE
	static BOOL GetPdfPageCount(CString szPdfFilePath, DWORD &dwPageCount);
	//将pdf文件转换成tif文件,转换成功返回TRUE,否则返回FALSE
	static BOOL PdfToTiff(CString szPdfFilePath, CString szTiffFilePath, BOOL bColor = FALSE, int nDpi = Image_Deal_Max_Dpi, BOOL bDelPdf = FALSE);
	//将Word文件转换成PDF文件,转换成功返回TRUE,否则返回FALSE
	static BOOL WordToPdf(CString szWordFilePath, CString szPdfFilePath, BOOL bDelWord = FALSE);
	//将Word文件转换成tif文件,转换成功返回TRUE,否则返回FALSE
	static BOOL WordToTiff(CString szWordFilePath, CString szTiffFilePath, BOOL bColor = FALSE, int nDpi = Image_Deal_Max_Dpi, BOOL bDelWord = FALSE);
	//获取Word文档页数,成功返回TRUE,否则返回FALSE
	static BOOL GetWordPageCount(CString szWordFilePath, DWORD &dwPageCount);
	//将Excel文件转换成PDF文件,转换成功返回TRUE,否则返回FALSE
	static BOOL ExcelToPdf(CString szExcelFilePath, CString szPdfFilePath, BOOL bDelExcel = FALSE, WORD dwPaperSize = DMPAPER_A4, WORD dwPageOrientation = DMORIENT_PORTRAIT);
	//将Excel文件转换成tif文件,转换成功返回TRUE,否则返回FALSE
	static BOOL ExcelToTiff(CString szExcelFilePath, CString szTiffFilePath, BOOL bColor = FALSE, int nDpi = Image_Deal_Max_Dpi, BOOL bDelExcel = FALSE, WORD dwPaperSize = DMPAPER_A4, WORD dwPageOrientation = DMORIENT_PORTRAIT);
	//获取Excel文档页数,成功返回TRUE,否则返回FALSE
	static BOOL GetExcelPageCount(CString szExcelFilePath, DWORD &dwPageCount);
	//将PPT文件转换成PDF文件,转换成功返回TRUE,否则返回FALSE
	static BOOL PPTToPdf(CString szPPTFilePath, CString szPdfFilePath, BOOL bDelPpt = FALSE);
	//将PPT文件转换成tif文件,转换成功返回TRUE,否则返回FALSE
	static BOOL PPTToTiff(CString szPPTFilePath, CString szTiffFilePath, BOOL bColor = FALSE, int nDpi = Image_Deal_Max_Dpi, BOOL bDelPpt = FALSE);
	//获取PPT文档页数,成功返回TRUE,否则返回FALSE
	static BOOL GetPPTPageCount(CString szPPTFilePath, DWORD &dwPageCount);
	//获取Tif文档页数
	static BOOL GetTiffPageCount(CString szTifFilePath, DWORD &dwPageCount);
	//预处理普通的图片文件,使其转换成正常打印的tif文件
	static BOOL PictureToPrintTifFile(CString szPicFilePath, CString szTifFilePath, int nPapeType, int nPapeOrientation
		, BOOL bColor = FALSE, int nMaxDpi = Image_Deal_Max_Dpi, BOOL bDelPic = FALSE
		, PicturePostion pp = PicturePostion_Cetter, PictureSize ps = PictureSize_Full);
	//调整图片的打印位置
	static void AdjustPicturePostion(Size PrintSize, Size ImageSize, Rect& ImageDestRect
		, PicturePostion pp = PicturePostion_Cetter, PictureSize ps = PictureSize_Full);
	//根据打印纸型和打印dpi，获取物理区域大小和打印区域图片大小
	static void GetPageSize(int nPapeType, float fXDpi, float fYDpi, Size& PhysicalSize, Size& PrintSize);
	//根据图片和最大可打印区域，获取最佳的打印dpi
	static void GetImagePrintDpi(Image& image, Size& MaxPrintSize, float& fXMaxDpi, float& fYMaxDpi);
	//根据图片和打印参数，获取图片的最佳打印参数
	static void GetImagePrintParam(Image& image, int nPapeType, float& fXMaxDpi, float& fYMaxDpi
		, Size& PhysicalSize, Size& PrintSize, Size& ImageSize, Rect& ImageDestRect
		, PicturePostion pp = PicturePostion_Cetter, PictureSize ps = PictureSize_Full);
	static Image* ConvertCmykToRgb(Image* cmykImage);

};
