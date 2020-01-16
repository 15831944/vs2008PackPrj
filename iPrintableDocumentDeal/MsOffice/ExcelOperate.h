#pragma once

#include "ExcelHeader.h"

class _AA_DLL_EXPORT_ CExcelOperate
{
public:
	CExcelOperate(void);
	~CExcelOperate(void);

public:
    //操作  
    //**********************创建新文档*******************************************  
    BOOL CreateApp();                   //创建一个新的Excel应用程序  
    BOOL CreateWorkbooks();             //创建一个工作簿集合(Workbooks),包含N个工作簿（Workbook）
    BOOL CreateWorkbook();              //创建一个工作簿(Workbook)，包含一个工作表（sheets）集合 
	BOOL CreateWorksheets();			//创建一个工作表集合(Worksheets)，包含N个工作表(Worksheet)
	BOOL CreateWorksheet();			    //创建一个工作表(Worksheet)，也就是我们在Excel中看到的Sheet1、Sheet2、Sheet3，它是操作Excel的基本单位
    BOOL Create();                      //创建新的Excel应用程序并创建一个新的文档  
    void SetVisible(BOOL bVisible);     //设置app显示、隐藏状态
	BOOL SetPageParam(XlPaperSize lPaperSize, XlPageOrientation lPageOrientation);	//设置Excel页面大小和方向

	//信息
	CString GetVersion();				//获取ExcelVersion
  
    //**********************打开文档*********************************************  
    BOOL OpenDocument(CString fileName);//打开已经存在的文档。  
    BOOL Open(CString fileName);        //创建新的Excel应用程序并打开一个已经存在的文档。  
    BOOL SetActiveDocument(short i);    //设置当前激活的文档。  
  
    //**********************保存文档*********************************************  
    BOOL SaveDocument();                //文档是以打开形式，保存。  
    BOOL SaveDocumentAs(CString fileName);//文档以创建形式，保存。  
    BOOL SaveDocumentAs(CString fileName,XlFileFormat xlFormat);//文档以指定形式，保存。
    BOOL ExportDocumentAs(CString fileName,XlFixedFormatType xlFormat);//文档以指定形式，导出。
    BOOL CloseDocument();  
    void CloseApp();  

	//打印
	BOOL Print();

private:
	CApplication0 m_oApp;
	CWorkbooks m_oWorkbooks;
	CWorkbook m_oWorkbook;
	CWorksheets m_oWorksheets; 
	CWorksheet m_oWorksheet;
	CPageSetup0 m_oPageSetup;

	BOOL m_bInitOk;
};
