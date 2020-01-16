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

//������һ��ӡʱ��ҳ�����ź�λ�ò���
typedef struct tag_AllPageInOneParam 
{
	double x_offset;
	double y_offset;
	double rotate_radians;
}AllPageInOneParam, *PAllPageInOneParam;

#include <gdiplus.h>
using namespace Gdiplus;

//���ö������eMessage��Ŀ��printermessage.pb.h
//Paper_Source�ο�Printer_Paper_Source
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
	//��ȡָ��������HENHMETAFILE
	HENHMETAFILE GetEmf(CParseSpl& spl, DWORD dwPageIndex);
	//��ȡָ��������PDFͼƬ����
	Image* GetPdf(CPdfHelper& pdf, DWORD dwPageIndex, BOOL bColor);
	//��ȡ��ӡ����
	void GetPrintDeviceArea(HDC& hdcPrinter, Image& image, Rect& printRect, Graphics& graphics, BOOL bUsePhysicalArea = FALSE);
	//��ȡ���һ��ӡ
	void GetAllPageInOneParam(Rect& printRect, Image& image, AllPageInOneParam& param);
	//����HDC
	void ResetPrinterHDC(HDC& hdcPrinter, PDEVMODE pDevMode, PENHMETAHEADER pEmfHeader);
	//����HDC
	void ResetPrinterHDC(HDC& hdcPrinter, PDEVMODE pDevMode, Image& image);

	//���ô�ӡ��ֽ����Դ
	void SetSLPrinterSource(Paper_Source PaperSource);	//ʩ��
	void SetDZPrinterSource(Paper_Source PaperSource);	//��֥
	void SetKMPrinterSource(Paper_Source PaperSource);	//����
	void SetLMPrinterSource(Paper_Source PaperSource);	//����
	void SetLGPrinterSource(Paper_Source PaperSource);	//���
	void SetJCPrinterSource(Paper_Source PaperSource);	//����
	void SetZDPrinterSource(Paper_Source PaperSource);	//��
	void SetXPPrinterSource(Paper_Source PaperSource);	//����
	void SetSXPrinterSource(Paper_Source PaperSource);	//����
	void SetHPPrinterSource(Paper_Source PaperSource);	//����

	BOOL IsXpsFileFormat(CString szFilePath);	//�ж��ļ��Ƿ�ΪXPS��ʽ
	void VerifyDocName(CString& szDocName);		//��֤�ĵ����Ϸ���

private:
	CString m_sPrinter;
	JobSecNode m_oSec;

	WORD m_dwColor;			//��ӡɫ��
	WORD m_dwDuplex;		//˫���ӡ
	WORD m_dwOrientation;	//��ӡ����(���/����)
	WORD m_dwPaperSize;		//��ӡֽ��
	WORD m_dwPaperLength;	//��ӡֽ�ų���-�Զ���ֽ�Ϳ����õ�
	WORD m_dwPaperWidth;	//��ӡֽ�ſ��-�Զ���ֽ�Ϳ����õ�
	WORD m_dwScale;			//��ӡ����
	WORD m_dwCopies;		//��ӡ����
	WORD m_dwCollate;		//��ݴ�ӡ
	WORD m_dwDefaultSource;	//ֽ����Դ
	WORD m_dwPrintQuality;	//��ӡ����
	WORD m_dwPagesPerPaper;	//ÿҳֽ��ӡ������ҳ�������һ���ӡ
	int m_nPrinted;			//�ϵ�������,�Ѿ���ӡ��ҳ��
	BOOL m_bEnableDefaultSource;	//���ô�ӡѡ��ֽ����Դ
	BOOL m_bAutoPaperSize;	//�Ƿ����tiff��emf��ʵ�ʴ�С���Զ�ʶ��ֽ��
	BRAND_TYPE m_eType;		//��ӡ��Ʒ��
	int m_nCurrentPage;		//��ǰ��ӡҳ����0����

private:
	BOOL m_bUseA4Print;	//add by zxl,20150113,��A3/A4�Ĵ�ӡ����ת����A4ֽ�ʹ�ӡ
	BOOL m_bForceMonochromePrint;	//add by zxl,20150728,��һЩ��ӡ���������޷��ڴ�ӡʱ����ɫ��ģʽ������ǿ��ʹ�úڰ�ת��
	BOOL m_bForceXpsDriverPrint;	//add by zxl, 20151014,��һЩ��ӡ���������޷��ڴ�ӡXPS�ļ�������ǿ��ʹ���µĴ�ӡ������(XPS����)
};
