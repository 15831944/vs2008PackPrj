#ifndef _PDFHELPER_517B7F85_B0C5_480E_B35B_199AC523DB5A_H_ 
#define _PDFHELPER_517B7F85_B0C5_480E_B35B_199AC523DB5A_H_

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

#include "pdfTypes.h"

class _AA_DLL_EXPORT_ CPdfHelper
{
public:
    /** 启用日志, add by zmm, 2019.4.17*/
    static void EnableLog(bool bEnabled);

	CPdfHelper();
	CPdfHelper(const char *cPdfFilePath, const char *cOwnerPassword = NULL, const char *cUserPassword = NULL);
	~CPdfHelper();

	BOOL OpenFile(const char *cPdfFilePath, const char *cOwnerPassword = NULL, const char *cUserPassword = NULL);
	void CloseFile();
	BOOL IsOK();
	int GetPageCount();
	const char* GetPdfFilePath();
	BOOL GetBitmap(int nPageIndex, double hDPI, double vDPI, ImageColorMode color,
		int& nWidth, int& nHeight, int& nStride, unsigned char **scan0);	//scan0在使用完后,必须调用FreeBitmapData释放内存
	void FreeBitmapData(unsigned char *scan0);	//释放GetBitmap接口返回的位图内存数据

    #pragma message("PrintToPrinter 该方法已废弃, 请使用 PrintToPrinterZ 替换")
	//打印pdf到指定打印机cPrinterName, 
	BOOL PrintToPrinter(const char *cPrinterName, const char *cDocName = NULL,
		int nBeginPage = -1, int nEndPage = -1, int nPagesPerPaper = 1,
		WORD wColor = DMCOLOR_MONOCHROME, WORD wPaperSize = DMPAPER_A4,
		WORD wOrientation = DMORIENT_PORTRAIT, WORD wCopies = 1,
		WORD wCollate = DMCOLLATE_TRUE,
		WORD wDuplex = DMDUP_SIMPLEX, WORD wPaperSource = DMBIN_AUTO,
		WORD wPaperWidth = 0, WORD wPaperHeight = 0,
		BOOL bUseCropBox = FALSE, BOOL bUseFullPage = TRUE,
		BOOL bUsePDFPageSize = TRUE, BOOL bForceGray = FALSE, 
		WaterInfo* pWaterInfo = NULL,
		FPrintProcessCallBack callBack = NULL, 
		PrintProcessCallBackData* pCallBackData = NULL,
		BOOL bPauseJob = FALSE, int* pJobId = NULL);
    // 打印pdf到指定打印机, add by zmm, 2019.5.13
    BOOL PrintToPrinterZ(ZPrintParams& oParams);

	//获取PDF中指定页码页面属性
	BOOL GetPageBox(int nPageIndex, PageBox& box, PageBoxType type = MediaBox);
	int GettPageRotate(int nPageIndex);

	//判断指定的页面是否为空白页
	BOOL IsBlankPage(int nPageIndex);

	//设置作业所属用户
	void SetJobUser(const char *cJobUser);

protected:
	void InitEnvironment();
	void SetPagesPerPaper(int nPagesPerPaper);
	BOOL IsBlankBitmap(ImageColorMode color, int nWidth, int nHeight, int nStride, unsigned char *scan0);

public:
	void *m_doc;			//PDFDoc
	void *m_surface;		//cairo_surface_t
	void *m_fileName;		//GooString
	void *m_outputName;		//GooString
	void *m_outputFileName;	//GooString
	void *m_imageFileName;	//GooString
	void *m_ownerPW;		//GooString
	void *m_userPW;			//GooString
	void *m_printer;		//GooString

	int m_firstPage;
	int m_lastPage;
	int m_pagesPerPaper;
	int m_currentPage;

	//
	bool m_png;
	bool m_jpeg;
	bool m_ps;
	bool m_eps;
	bool m_pdf;
	bool m_printToWin32;
	bool m_onePaperPrintCompleted;	//一张纸内容打印完成
	bool m_printdlg;
	bool m_svg;
	bool m_tiff;

	bool m_printOnlyOdd;
	bool m_printOnlyEven;
	bool m_singleFile;
	double m_max_resolution;// in PPI, 即:每英寸所拥有的像素数量
	double m_x_resolution;// in PPI
	double m_y_resolution;// in PPI
	int m_scaleTo;// in pixels
	int m_x_scaleTo;// in pixels
	int m_y_scaleTo;// in pixels
	int m_crop_x;// in pixels
	int m_crop_y;// in pixels
	int m_crop_w;// in pixels
	int m_crop_h;// in pixels
	bool m_useCropBox;
	bool m_mono;
	bool m_gray;
	bool m_transp;

	bool m_level2;
	bool m_level3;
	bool m_origPageSizes;
	char m_paperSize[15];
	double m_paperWidth;// 单位: point_p (指的是poppler 中的单位长度),  这里转换关系是以每英寸72dpi为准, 即: 1 point_p == 1/72.0 inch
	double m_paperHeight;// 单位: point_p 
	bool m_noCrop;
	bool m_expand;
	bool m_noShrink;
	bool m_noCenter;
	bool m_duplex;
	char m_tiffCompressionStr[16];

	bool m_setupdlg;

	bool m_useWin32PageSize;	//强制使用DEVMODE中的dmPaperSize，而不使用dmPaperWidth和dmPaperLength
	bool m_printing;
	bool m_printingAbort;		//打印被中止
	bool m_printingOutOfMemory;	//打印内存不足
	FILE *m_output_file;
	bool m_usePDFPageSize;
	bool m_useFullPage;
	bool m_forceGray;	//强制灰度(黑白色)打印,
						//某些打印机(已知:施乐)不能控制黑白和彩色打印,
						//在提交作业时,强制将作业渲染成灰度图.

	//
	HDC m_hdc;
	HGLOBAL m_hDevmode;
	HGLOBAL m_hDevnames;
	DEVMODEA *m_devmode;
	char* m_printerName;
	char* m_jobUser;		//打印作业的所属用户

	bool m_useWater;
	WaterInfo* m_waterInfo;

	bool m_pauseJob;
	int m_jobId;
	bool m_imagePrint;
};

#endif	//_PDFHELPER_517B7F85_B0C5_480E_B35B_199AC523DB5A_H_