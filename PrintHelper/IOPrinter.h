#pragma once

#include "CopyInterface.h"

#include "../Public/PdfHelperDef.h"

class JobSecNode
{
public:
	CString footer;
	CString header;
	CString water;
};

//处理多合一打印时的页面缩放和位置参数
typedef struct tag_AllPageInOneParam 
{
	double x_offset;
	double y_offset;
	double rotate_radians;
}AllPageInOneParam, *PAllPageInOneParam;

#include <gdiplus.h>
using namespace Gdiplus;

//这个枚举是在eMessage项目中printermessage.pb.h
//Paper_Source参考Printer_Paper_Source
enum Paper_Source {
	Paper_Source_AUTO = 1000,
	Paper_Source_1 = 1001,
	Paper_Source_2 = 1002,
	Paper_Source_3 = 1003,
	Paper_Source_4 = 1004,
	Paper_Source_5 = 1005,
	Paper_Source_MANUAL = 1011,
	Paper_Source_ENVMANUAL = 1012,
	Paper_Source_ENVELOPE = 1013,
	Paper_Source_TRACTOR = 1014,
	Paper_Source_SMALLFMT = 1015,
	Paper_Source_LARGEFMT = 1016,
	Paper_Source_LARGECAPACITY = 1017,
	Paper_Source_CASSETTE = 1018
};

class _AA_DLL_EXPORT_ CIOPrinter
{
public:
	CIOPrinter(void);
	~CIOPrinter(void);
	BOOL PrintPdfFile(CString szPdfPath,CString sDocName,IPrtStationPrintCallback* pCall,int nFrom = -1,int nTo = -1);	//add in 2016.04.18
	BOOL PrintPdfFileBreakPoint(CString szPdfPath,CString sDocName,IPrtStationPrintCallback* pCall,int nFrom = -1,int nTo = -1);	//add in 2016.04.18
	BOOL PrintSplFile(CString szSplPath,CString sDocName,IPrtStationPrintCallback* pCall, int nTotalPage);	//add in 2013.06.13
	BOOL PrintTiffFile(CString sPath,CString sDocName,IPrtStationPrintCallback* pCall,int nFrom = -1,int nTo = -1, BOOL bUsePhysicalArea = FALSE);
	BOOL PrintTiffFileBreakPoint(CString sPath,CString sDocName,IPrtStationPrintCallback* pCall,int nFrom = -1,int nTo = -1, BOOL bUsePhysicalArea = FALSE);
	BOOL PrintEmfFile(CString sPath,CString sDocName,IPrtStationPrintCallback* pCall,int nFrom = -1,int nTo = -1);
	BOOL PrintEmfFileBreakPoint(CString sPath,CString sDocName,IPrtStationPrintCallback* pCall,int nFrom = -1,int nTo = -1);

	CString GetDefault();
	void SetSelPrinter(CString sName);
	void SetFooter(CString sFooter);
	void SetHeader(CString sHeader);
	void SetWatermark(CString sVal);
	BOOL IsWater();
	void EnableColor(BOOL b);
	void SetDuplex(WORD dw);
	void SetOrientation(WORD dw);
	void SetPaperSize(WORD dw);
	void SetPaperLength(WORD dw);
	void SetPaperWidth(WORD dw);
	void SetScale(WORD dw);
	void SetCopies(WORD dw);
	void EnableCollate(BOOL b);
	void SetDefaultSource(WORD dw);
	void SetDefaultSourceByBrand(BRAND_TYPE eType, Paper_Source PaperSource);
	void SetPrintQuality(WORD dw);
	void SetPagesPerPaper(WORD dw);
	void SetPrinted(int nPrinted);
	void KeepJob(HANDLE hPrint,BOOL bEnable ,BOOL& bKeep);
	void EnableDefaultSource(BOOL bEnable);
	void EnableAutoPaperSize(BOOL bEnable);
	BOOL IsEnableDefaultSource();

public:
	static DWORD GetPrinterStatus(CString sName);
	static void WINAPI PrintProcessCallBack(PrintProcessCallBackData* pCallBackData);

protected:
	BOOL PrintPdf(CString szPrinter, CString szDocName, CString szPdfPath);	//add in 2016.04.18
	BOOL PrintSpl(CString szPrinter, CString szSplPath);	//add in 2013.06.13
	//获取指定索引的HENHMETAFILE
	HENHMETAFILE GetEmf(CParseSpl& spl, DWORD dwPageIndex);
	//获取指定索引的PDF图片数据
	Image* GetPdf(CPdfHelper& pdf, DWORD dwPageIndex, BOOL bColor);
	//获取打印区域
	void GetPrintDeviceArea(HDC& hdcPrinter, Image& image, Rect& printRect, Graphics& graphics, BOOL bUsePhysicalArea = FALSE);
	//获取多合一打印
	void GetAllPageInOneParam(Rect& printRect, Image& image, AllPageInOneParam& param);
	//重置HDC
	void ResetPrinterHDC(HDC& hdcPrinter, PDEVMODE pDevMode, PENHMETAHEADER pEmfHeader);
	//重置HDC
	void ResetPrinterHDC(HDC& hdcPrinter, PDEVMODE pDevMode, Image& image);

	//设置打印机纸张来源
	void SetSLPrinterSource(Paper_Source PaperSource);	//施乐
	void SetDZPrinterSource(Paper_Source PaperSource);	//东芝
	void SetKMPrinterSource(Paper_Source PaperSource);	//柯美
	void SetLMPrinterSource(Paper_Source PaperSource);	//利盟
	void SetLGPrinterSource(Paper_Source PaperSource);	//理光
	void SetJCPrinterSource(Paper_Source PaperSource);	//京瓷
	void SetZDPrinterSource(Paper_Source PaperSource);	//震旦
	void SetXPPrinterSource(Paper_Source PaperSource);	//夏普
	void SetSXPrinterSource(Paper_Source PaperSource);	//三星
	void SetHPPrinterSource(Paper_Source PaperSource);	//惠普

	BOOL IsXpsFileFormat(CString szFilePath);	//判断文件是否为XPS格式
	void VerifyDocName(CString& szDocName);		//验证文档名合法性

private:
	CString m_sPrinter;
	JobSecNode m_oSec;

	WORD m_dwColor;			//打印色彩
	WORD m_dwDuplex;		//双面打印
	WORD m_dwOrientation;	//打印方向(横打/竖打)
	WORD m_dwPaperSize;		//打印纸型
	WORD m_dwPaperLength;	//打印纸张长度-自定义纸型可能用到
	WORD m_dwPaperWidth;	//打印纸张宽度-自定义纸型可能用到
	WORD m_dwScale;			//打印缩放
	WORD m_dwCopies;		//打印份数
	WORD m_dwCollate;		//逐份打印
	WORD m_dwDefaultSource;	//纸张来源
	WORD m_dwPrintQuality;	//打印质量
	WORD m_dwPagesPerPaper;	//每页纸打印的内容页数，多合一多打印
	int m_nPrinted;			//断点续打功能,已经打印的页数
	BOOL m_bEnableDefaultSource;	//启用打印选择纸张来源
	BOOL m_bAutoPaperSize;	//是否根据tiff和emf的实际大小，自动识别纸型
	BRAND_TYPE m_eType;		//打印机品牌
	int m_nCurrentPage;		//当前打印页，从0计数

private:
	BOOL m_bUseA4Print;	//add by zxl,20150113,非A3/A4的打印任务，转换成A4纸型打印
	BOOL m_bForceMonochromePrint;	//add by zxl,20150728,在一些打印机驱动中无法在打印时设置色彩模式，所以强制使用黑白转换
	BOOL m_bForceXpsDriverPrint;	//add by zxl, 20151014,在一些打印机驱动中无法在打印XPS文件，所以强制使用新的打印机驱动(XPS驱动)
};
