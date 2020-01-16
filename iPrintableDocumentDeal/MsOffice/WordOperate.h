#pragma once

#include "WordHeader.h"

class _AA_DLL_EXPORT_ CWordOperate 
{
public:
	CWordOperate(void);
	~CWordOperate(void);

public:  
    //操作  
    //**********************创建新文档*******************************************  
    BOOL CreateApp();                    //创建一个新的WORD应用程序  
    BOOL CreateDocuments();                //创建一个新的Word文档集合  
    BOOL CreateDocument();                //创建一个新的Word文档  
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
    BOOL SaveDocumentAs(CString fileName,WdSaveFormat wdFormat);//文档以指定形式，保存。  
    BOOL ExportDocumentAs(CString fileName,WdExportFormat wdFormat);//文档以指定形式，导出。  
    BOOL CloseDocument();  
    void CloseApp();   
  
    //**********************文本书写操作*****************************************  
    void WriteText(CString szText);        //当前光标处写文本  
    void WriteNewLineText(CString szText, int nLineCount = 1); //换N行写字  
    void WriteEndLine(CString szText);    //文档结尾处写文本  
    void WholeStory();                    //全选文档内容  
    void Copy();                        //复制文本内容到剪贴板  
    void InsertFile(CString fileName);    //将本地的文件全部内容写入到当前文档的光标处。  
    //----------------------add by zxx--------------------------------------  
    //***********************光标操作********************************************  
    //上下按行选择  
    void SelectMoveDown(short lineCount, short unit);//有选择操作的移动  
    void NoneSelectMoveDown(short lineCount, short unit);//仅仅移动光标，不选中  
    void SelectMoveUp(short lineCount, short unit);//有选择操作的移动  
    void NoneSelectMoveUp(short lineCount, short unit);//仅仅移动光标，不选中  
    //左右按列选择  
    void SelectMoveLeft(short charCount, short unit);//有选择操作的移动  
    void NoneSelectMoveLeft(short charCount, short unit);//  
    void SelectMoveRight(short charCount, short unit);//有选择操作的移动  
    void NoneSelectMoveRight(short charCount, short unit);//  
  
  
    void MoveToFirst();  
    void MoveToNextPage();  
    void TypeParagraph();  
    void PasteAndFormat();  
    void Paste();  
    void TypeBackspace(int count);  

	//打印文件
	BOOL Print();

private:  
	CApplication m_oApp;  
	CDocuments m_oDocuments;  
	CDocument0 m_oDocument;  
	CSelection m_oSelecton;  
	CRange     m_oRange;

	BOOL m_bInitOk;
};
