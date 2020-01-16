#ifndef PDFTYPES_H_7753504E_1AB6_4f2d_8412_9B02EF164FBA
#define PDFTYPES_H_7753504E_1AB6_4f2d_8412_9B02EF164FBA

//打印处理的回调接口和结构体定义
#include "PrintProcessDef.h"

//参考SplashColorMode
enum ImageColorMode {
    imageModeMono1,		// 1 bit per component, 8 pixels per byte,
    //   MSbit is on the left
    imageModeMono8,		// 1 byte per component, 1 byte per pixel
    imageModeRGB8,		// 1 byte per component, 3 bytes per pixel:
    //   RGBRGB...
    imageModeBGR8,		// 1 byte per component, 3 bytes per pixel:
    //   BGRBGR...
    imageModeXBGR8		// 1 byte per component, 4 bytes per pixel:
    //   XBGRXBGR...
};

typedef struct  
{
    char Header[300];
    char Footer[300];
    char Water[300];
}WaterInfo;

enum PageBoxType
{
    MediaBox,
    CropBox,
    BleedBox,
    TrimBox,
    ArtBox
};

typedef struct
{
    double X1;
    double Y1;
    double X2;
    double Y2;
}PageBox;

struct ZPrintParams
{
    ZPrintParams()
        : nBeginPage(-1)
        , nEndPage(-1)
        , nPagesPerPaper(1)
        , wColor(DMCOLOR_MONOCHROME)
        , wPaperSize(DMPAPER_A4)
        , wOrientation(DMORIENT_PORTRAIT)
        , wCopies(1)
        , wCollate(DMCOLLATE_TRUE)
        , wDuplex(DMDUP_SIMPLEX)
        , wPaperSource(DMBIN_AUTO)
        , wPaperWidth(0)
        , wPaperHeight(0)
        , bUseCropBox(FALSE)
        , bUseFullPage(TRUE)
        , bUsePDFPageSize(TRUE)
        , bForceGray(FALSE)
        , pWaterInfo(NULL)
        , callBack(NULL)
        , pCallBackData(NULL)
        , bPauseJob(FALSE)
        , pJobId(NULL)
    {
        memset(cPrinterName, 0, sizeof(cPrinterName));
        memset(cDocName, 0, sizeof(cDocName));
    }
    char cPrinterName[MAX_PATH];
    char cDocName[MAX_PATH];
    int nBeginPage;
    int nEndPage;
    int nPagesPerPaper;
    WORD wColor;
    WORD wPaperSize;
    WORD wOrientation;
    WORD wCopies;
    WORD wCollate;
    WORD wDuplex;
    WORD wPaperSource;
    WORD wPaperWidth; // 单位: 0.1mm
    WORD wPaperHeight; // 单位: 0.1mm
    BOOL bUseCropBox;
    BOOL bUseFullPage;
    BOOL bUsePDFPageSize;
    BOOL bForceGray;
    WaterInfo* pWaterInfo;
    FPrintProcessCallBack callBack; 
    PrintProcessCallBackData* pCallBackData;
    BOOL bPauseJob;
    int* pJobId;
};

#endif