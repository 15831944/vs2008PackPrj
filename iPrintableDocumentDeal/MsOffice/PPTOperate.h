#pragma once

#include "PPTHeader.h"

class _AA_DLL_EXPORT_ CPPTOperate
{
public:
	CPPTOperate(void);
	~CPPTOperate(void);

public:
	   //操作  
    //**********************创建新文档*******************************************  
    BOOL CreateApp();                    //创建一个新的PPT应用程序  
    BOOL CreatePresentations();          //创建一个新的PPT文档集合  
    BOOL CreatePresentation();           //创建一个新的PPT文档  
    BOOL Create();                        //创建新的WORD应用程序并创建一个新的文档  
	void SetVisible(BOOL bVisible);     //设置app显示、隐藏状态

	//信息
	CString GetVersion();				//获取WordVersion
  
    //**********************打开文档*********************************************  
    BOOL OpenDocument(CString fileName);//打开已经存在的文档。  
    BOOL Open(CString fileName);        //创建新的WORD应用程序并打开一个已经存在的文档。  
    BOOL SetActiveDocument(short i);    //设置当前激活的文档。  
  
    //**********************保存文档*********************************************  
    BOOL SaveDocument();                //文档是以打开形式，保存。  
    BOOL SaveDocumentAs(CString fileName);//文档以创建形式，保存。  
    BOOL SaveDocumentAs(CString fileName,PpSaveAsFileType ppFormat);//文档以指定形式，保存。  
    BOOL ExportDocumentAs(CString fileName,PpFixedFormatType ppFormat);//文档以指定形式，导出。  
    BOOL CloseDocument();  
    void CloseApp();   

	//打印文件
	BOOL Print();

private:
	CApplication1 m_oApp;
	CPresentations m_oPresentations;
	CPresentation m_oPresentation;
	CProtectedViewWindow m_oProtectedViewWindow;
	CProtectedViewWindows m_oProtectedViewWindows;
// 	CSlideShowView m_oSlideShowView;
// 	CSlideShowWindow m_oSlideShowWindow;
// 	CSlideShowSettings m_oSlideShowSettings;
// 	CSlides m_oSlides;
// 	CSlide m_oSlide;
// 	CDocumentWindow m_oDocumentwindow;
// 	CView1 m_oView;

	BOOL m_bInitOk;
};
