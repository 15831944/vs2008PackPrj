#include "StdAfx.h"
#include "WordOperate.h"

CWordOperate::CWordOperate(void)
{
	CloseApp();
}

CWordOperate::~CWordOperate(void)
{
	CloseApp();
}

BOOL CWordOperate::CreateApp()  
{
	if (m_bInitOk)
	{
		return TRUE;
	}

	CloseApp();

	try
	{
		if (!m_oApp.CreateDispatch(_T("Word.Application")))  
		{  
			theLog.Write("!!CWordOperate::CreateApp,创建失败,1，请确保安装了word 2000或以上版本!err=%d", GetLastError());    
			return FALSE;  
		}

		m_oApp.put_DisplayAlerts(FALSE);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::CreateApp,创建失败,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	if (m_oApp.m_lpDispatch)
	{
		theLog.Write("CWordOperate::CreateApp,创建成功,3，Version=%s", GetVersion());
		m_bInitOk = TRUE;
		return TRUE;  
	}
	else
	{
		theLog.Write("!!CWordOperate::CreateApp,创建失败,4");
		return FALSE;  
	}	
}  

BOOL CWordOperate::CreateDocuments()  
{  
	if (!CreateApp())   
	{
		theLog.Write("!!CWordOperate::CreateDocuments,1,CreateApp fail!");  
		return FALSE;  
	}

	if (m_oDocuments.m_lpDispatch)
	{
		theLog.Write("##CWordOperate::CreateDocuments,2,m_lpDispatch=%p", m_oDocuments.m_lpDispatch);  
		return TRUE;  
	}

	try
	{
		m_oDocuments.AttachDispatch(m_oApp.get_Documents()); 
		if (!m_oDocuments.m_lpDispatch)   
		{  
			theLog.Write("!!CWordOperate::CreateDocuments,3,m_oDocuments fail!");  
			return FALSE;  
		}
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::CreateDocuments,4,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}  

BOOL CWordOperate::CreateDocument()  
{  
	if (!m_oDocuments.m_lpDispatch)   
	{  
		theLog.Write("!!CWordOperate::CreateDocument,1,Documents为空!");  
		return FALSE;  
	}  

	try
	{
		COleVariant varTrue(short(1),VT_BOOL);
		COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		CComVariant Template(_T(""));    //没有使用WORD的文档模板  
		CComVariant NewTemplate(false);
		COleVariant DocumentType((long)0, VT_I4);
		COleVariant Visible;  

		m_oDocuments.Add(&Template,&NewTemplate,&DocumentType,&Visible);      

		//得到document变量  
		m_oDocument = m_oApp.get_ActiveDocument();  
		if (!m_oDocument.m_lpDispatch)   
		{  
			theLog.Write("!!CWordOperate::CreateDocument,2,Document获取失败!");  
			return FALSE;  
		}  
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::CreateDocument,5,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}  

BOOL CWordOperate::Create()  
{  
	if (!CreateDocuments())   
	{
		theLog.Write("!!CWordOperate::Create,1,CreateDocuments fail.");
		return FALSE;  
	}  
	return CreateDocument();  
}  

//设置app显示、隐藏状态
void CWordOperate::SetVisible(BOOL bVisible)
{
	try
	{
		m_oApp.put_Visible(bVisible);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::SetVisible,catch CException [%s],err=%d,bVisible=[%d]", tcErrMsg, GetLastError(), bVisible);
	}
}

CString CWordOperate::GetVersion()
{
	CString szVersion("UnKnow");
	try
	{
		szVersion = m_oApp.get_Version();	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::GetVersion,catch CException [%s],err=%d", tcErrMsg, GetLastError());
	}

	return szVersion;
}

BOOL CWordOperate::OpenDocument(CString fileName)  
{
	if (!m_oDocuments.m_lpDispatch)   
	{  
		theLog.Write("!!CWordOperate::OpenDocument,1,Documents为空!");  
		return FALSE;  
	}  

	try
	{
		COleVariant vTrue((short)TRUE);    
		COleVariant	vFalse((short)FALSE);
		COleVariant	vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		COleVariant vFileName(_T(fileName));  
		COleVariant vPassWord(_T("nullpwd"));	//设置一个错误的密码，防止弹出输入密码框，从而阻塞线程，
												//效果就是：一，当有密码的文档，直接抛出异常，打开失败，
												//二，当没有密码的文档，直接打开文档。

		//得到document变量  
		m_oDocument.AttachDispatch(m_oDocuments.Open(  
			vFileName,        // FileName  
			vFalse,            // Confirm Conversion.  
			vTrue,            // ReadOnly.  
			vFalse,            // AddToRecentFiles.  
			vPassWord,        // PasswordDocument.  
			vPassWord,        // PasswordTemplate.  
			vOptional,        // Revert.  
			vPassWord,        // WritePasswordDocument.  
			vPassWord,        // WritePasswordTemplate.  
			vOptional,        // Format. // Last argument for Word 97  
			vOptional,        // Encoding // New for Word 2000/2002  
			vOptional,        // Visible  
			//如下4个是word2003需要的参数。本版本是word2000。  
			vOptional,    // OpenAndRepair  
			vOptional,            // DocumentDirection wdDocumentDirection LeftToRight  
			vOptional,    // NoEncodingDialog  
			vOptional  

			)                // Close Open parameters  
			);                    // Close AttachDispatch  

		if (!m_oDocument.m_lpDispatch)   
		{  
			theLog.Write("!!CWordOperate::OpenDocument,2,Document获取失败!");  
			return FALSE;  
		}  
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::OpenDocument,5,catch CException [%s],err=%d,file=[%s]", tcErrMsg, GetLastError(), fileName);
		return FALSE;  
	}

	return TRUE;  
}  

BOOL CWordOperate::Open(CString fileName)  
{  
	if (!CreateDocuments())   
	{  
		theLog.Write("!!CWordOperate::Open,CreateDocuments fail");
		return FALSE;  
	}  

	return OpenDocument(fileName);  
}  

BOOL CWordOperate::SetActiveDocument(short i)  
{  
	try
	{
		COleVariant vIndex(_T(i));
		COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

		m_oDocument.AttachDispatch(m_oDocuments.Item(vIndex));  
		m_oDocument.Activate();  
		if (!m_oDocument.m_lpDispatch)   
		{  
			theLog.Write("!!CWordOperate::SetActiveDocument,1,Document获取失败!");  
			return FALSE;  
		}  
		//得到selection变量  
		m_oSelecton = m_oApp.get_Selection();  
		if (!m_oSelecton.m_lpDispatch)   
		{  
			theLog.Write("!!CWordOperate::SetActiveDocument,2,Select获取失败!");  
			return FALSE;  
		}  
		//得到全部DOC的Range变量  
		m_oRange = m_oDocument.Range(vOptional,vOptional);  
		if(!m_oRange.m_lpDispatch)  
		{  
			theLog.Write("!!CWordOperate::SetActiveDocument,3,Range获取失败!");  
			return FALSE;  
		}  	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::SetActiveDocument,4,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}  

BOOL CWordOperate::SaveDocument()  
{  
	if (!m_oDocument.m_lpDispatch)   
	{  
		//theLog.Write("!!CWordOperate::SaveDocument,1,Document获取失败!");  
		return TRUE;  
	}  
	try
	{
		m_oDocument.Save();  
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::SaveDocument,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}
	return TRUE;  
}  

BOOL CWordOperate::SaveDocumentAs(CString fileName)  
{  
	if (!m_oDocument.m_lpDispatch)   
	{  
		theLog.Write("!!CWordOperate::SaveDocumentAs,1,Document获取失败!fileName=[%s]", fileName);  
		return FALSE;  
	}  

	try
	{
		COleVariant vTrue((short)TRUE);    
		COleVariant	vFalse((short)FALSE);
		COleVariant	vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		COleVariant vFileName(_T(fileName));  

		m_oDocument.SaveAs(  
			vFileName,    //VARIANT* FileName  
			vOptional,    //VARIANT* FileFormat  
			vOptional,    //VARIANT* LockComments  
			vOptional,    //VARIANT* Password  
			vOptional,    //VARIANT* AddToRecentFiles  
			vOptional,    //VARIANT* WritePassword  
			vOptional,    //VARIANT* ReadOnlyRecommended  
			vOptional,    //VARIANT* EmbedTrueTypeFonts  
			vOptional,    //VARIANT* SaveNativePictureFormat  
			vOptional,    //VARIANT* SaveFormsData  
			vOptional,    //VARIANT* SaveAsAOCELetter  
			vOptional,    //VARIANT* ReadOnlyRecommended  
			vOptional,    //VARIANT* EmbedTrueTypeFonts  
			vOptional,    //VARIANT* SaveNativePictureFormat  
			vOptional,    //VARIANT* SaveFormsData  
			vOptional    //VARIANT* SaveAsAOCELetter  
			);  	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::SaveDocumentAs,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}  

BOOL CWordOperate::SaveDocumentAs(CString fileName,WdSaveFormat wdFormat)  
{  
	if (!m_oDocument.m_lpDispatch)   
	{  
		theLog.Write("!!CWordOperate::SaveDocumentAs,1,Document获取失败!fileName=[%s],wdFormat=%d", fileName, wdFormat);  
		return FALSE;  
	}  

	try
	{
		COleVariant vTrue((short)TRUE);    
		COleVariant	vFalse((short)FALSE);
		COleVariant	vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		COleVariant vFileName(_T(fileName));  
		COleVariant	vFileFormat((long)wdFormat, VT_I4);

		//SaveAs函数会使用word最后一次保存使用的参数作为默认参数，
		//如果这个不是我们程序想要的结果，可以使用ExportAsFixedFormat。
		m_oDocument.SaveAs(  
			vFileName,    //VARIANT* FileName  
			vFileFormat,    //VARIANT* FileFormat  
			vOptional,    //VARIANT* LockComments  
			vOptional,    //VARIANT* Password  
			vOptional,    //VARIANT* AddToRecentFiles  
			vOptional,    //VARIANT* WritePassword  
			vOptional,    //VARIANT* ReadOnlyRecommended  
			vOptional,    //VARIANT* EmbedTrueTypeFonts  
			vOptional,    //VARIANT* SaveNativePictureFormat  
			vOptional,    //VARIANT* SaveFormsData  
			vOptional,    //VARIANT* SaveAsAOCELetter  
			vOptional,    //VARIANT* ReadOnlyRecommended  
			vOptional,    //VARIANT* EmbedTrueTypeFonts  
			vOptional,    //VARIANT* SaveNativePictureFormat  
			vOptional,    //VARIANT* SaveFormsData  
			vOptional    //VARIANT* SaveAsAOCELetter  
			);	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::SaveDocumentAs,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	if (!PathFileExists(fileName))
	{
		theLog.Write("!!CWordOperate::SaveDocumentAs,3,PathFileExists fail,fileName=[%s]", fileName);
		return FALSE;
	}

	return TRUE;  
}  

BOOL CWordOperate::ExportDocumentAs(CString fileName,WdExportFormat wdFormat)
{
	if (!m_oDocument.m_lpDispatch)   
	{  
		theLog.Write("!!CWordOperate::ExportDocumentAs,1,Document获取失败!fileName=[%s],wdFormat=%d", fileName, wdFormat);  
		return FALSE;  
	}  

	try
	{
		COleVariant	vtMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		m_oDocument.ExportAsFixedFormat(
			(LPCTSTR)fileName,				//LPCTSTR OutputFileName
			wdFormat,						//long ExportFormat
			FALSE,							//BOOL OpenAfterExport
			wdExportOptimizeForPrint,		//long OptimizeFor
			wdExportAllDocument,			//long Range
			1,								//long From
			1,								//long To
			wdExportDocumentContent,		//long Item
			FALSE,							//BOOL IncludeDocProps
			FALSE,							//BOOL KeepIRM	IRM信息权限管理(Information Rights Management) 
			wdExportCreateHeadingBookmarks,	//long CreateBookmarks
			TRUE,							//BOOL DocStructureTags
			TRUE,							//BOOL BitmapMissingFonts
			FALSE,							//BOOL UseISO19005_1
			vtMissing						//VARIANT * FixedFormatExtClassPtr
			);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::ExportDocumentAs,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	if (!PathFileExists(fileName))
	{
		theLog.Write("!!CWordOperate::ExportDocumentAs,3,PathFileExists fail,fileName=[%s]", fileName);
		return FALSE;
	}

	return TRUE;  
}

BOOL CWordOperate::CloseDocument()  
{  
	try
	{
		if (m_oDocument.m_lpDispatch)
		{
			COleVariant vTrue((short)TRUE);
			COleVariant	vFalse((short)FALSE);
			COleVariant	vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
			m_oDocument.Close(vFalse, vTrue, vFalse);
			m_oDocument.ReleaseDispatch();
		}
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::CloseDocument,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}
	return TRUE;  
}  

void CWordOperate::CloseApp()  
{ 
	m_bInitOk = FALSE;

	try
	{
		//释放内存申请资源  
		if (m_oRange.m_lpDispatch)
		{
			m_oRange.ReleaseDispatch();  
		}

		if (m_oSelecton.m_lpDispatch)
		{
			m_oSelecton.ReleaseDispatch();
		}

		if (m_oDocument.m_lpDispatch)
		{
			CloseDocument();
		}

		if (m_oDocuments.m_lpDispatch)
		{
			m_oDocuments.ReleaseDispatch();  
		}

		if (m_oApp.m_lpDispatch)
		{
			COleVariant vTrue((short)TRUE);
			COleVariant	vFalse((short)FALSE);
			COleVariant	vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
			m_oApp.Quit(vFalse, vTrue,	vFalse);  
			m_oApp.ReleaseDispatch();  
		}	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::CloseApp,2,catch CException [%s]", tcErrMsg);
	}
}  

void CWordOperate::WriteText(CString szText)  
{  
	m_oSelecton.TypeText(szText);  
}  

void CWordOperate::WriteNewLineText(CString szText, int nLineCount /**//* = 1 */)  
{  
	int i;  
	if (nLineCount <= 0)  
	{  
		nLineCount = 0;  
	}  
	for (i = 0; i < nLineCount; i++)  
	{  
		m_oSelecton.TypeParagraph();  
	}  
	WriteText(szText);  
}  

void CWordOperate::WriteEndLine(CString szText)  
{  
	m_oRange.InsertAfter(szText);  
}  

void CWordOperate::WholeStory()  
{  
	m_oRange.WholeStory();  
}  

void CWordOperate::Copy()  
{  
	m_oSelecton.Copy();  
	//m_oSelecton.CopyFormat();  
}  

void CWordOperate::TypeParagraph()  
{  
	m_oSelecton.TypeParagraph();  
}  

void CWordOperate::PasteAndFormat()  
{  
	m_oSelecton.PasteAndFormat(0);  
}  

void CWordOperate::Paste()  
{  
	m_oSelecton.Paste();  
	//m_oSelecton.PasteFormat();  
}  

void CWordOperate::TypeBackspace(int count)  
{  
	for(int i = 0; i < count; i++)  
		m_oSelecton.TypeBackspace();  
} 

void CWordOperate::InsertFile(CString fileName)  
{  
    COleVariant     vFileName(fileName),  
                 vTrue((short)TRUE),  
                 vFalse((short)FALSE),  
                 vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),  
                 vNull(_T(""));  
    /**//* 
    void InsertFile(LPCTSTR FileName, VARIANT* Range, VARIANT* ConfirmConversions, VARIANT* Link, VARIANT* Attachment); 
    */  
    m_oSelecton.InsertFile(  
                    fileName,  
                    vNull,  
                    vFalse,  
                    vFalse,  
                    vFalse  
                    );  
}  
  
void CWordOperate::SelectMoveDown(short lineCount, short unit)//有选择操作的移动  
{  
    m_oSelecton.MoveDown(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdExtend));  
}  
  
void CWordOperate::NoneSelectMoveDown(short lineCount, short unit)//仅仅移动光标，不选中  
{  
    m_oSelecton.MoveDown(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdMove));  
}  
  
void CWordOperate::SelectMoveUp(short lineCount, short unit)//有选择操作的移动  
{  
    m_oSelecton.MoveUp(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdExtend));  
}  
  
void CWordOperate::NoneSelectMoveUp(short lineCount, short unit)//仅仅移动光标，不选中  
{  
    m_oSelecton.MoveUp(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdMove));  
}  
  
void CWordOperate::SelectMoveLeft(short charCount, short unit)//有选择操作的移动  
{  
    m_oSelecton.MoveLeft(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdExtend));  
}  
  
void CWordOperate::NoneSelectMoveLeft(short charCount, short unit)//  
{  
    m_oSelecton.MoveLeft(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdMove));  
}  
void CWordOperate::SelectMoveRight(short charCount, short unit)//有选择操作的移动  
{  
    m_oSelecton.MoveRight(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdExtend));  
}  
void CWordOperate::NoneSelectMoveRight(short charCount, short unit)//  
{  
    m_oSelecton.MoveRight(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdMove));  
}  
void CWordOperate::MoveToFirst()  
{  
    m_oSelecton.GoTo(COleVariant((short)1), COleVariant((short)2), COleVariant((short)0), COleVariant("1"));  
}  
  
void CWordOperate::MoveToNextPage()  
{  
    m_oSelecton.GoTo(COleVariant((short)1), COleVariant((short)2), COleVariant((short)1), COleVariant(""));  
}

//打印文件
BOOL CWordOperate::Print()
{
	if (!m_oDocument.m_lpDispatch)
	{
		theLog.Write("!!CWordOperate::Print,1,m_oDocument fail");
		return FALSE;
	}

	try
	{
		//m_oDocument.PrintPreview();

		// 用COleVariant定义变量比较方便
		COleVariant covTrue((short)TRUE);
		COleVariant	covFalse((short)FALSE);
		COleVariant	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		COleVariant covOutputFileName(_T("C:\\word.emf.spl"));
		m_oDocument.PrintOut(
			covFalse,              // Background.
			covOptional,           // Append.
			covOptional,           // Range.
			covOutputFileName,     // OutputFileName.
			covOptional,           // From.
			covOptional,           // To.
			covOptional,           // Item.
			COleVariant((long)2),  // Copies.
			covOptional,           // Pages.
			covOptional,           // PageType.
			covTrue,			   // PrintToFile.
			covOptional,           // Collate.
			covOptional,           // ActivePrinterMacGX.
			covOptional,            // ManualDuplexPrint.
			covOptional,           // PrintZoomColumn  New with Word 2002
			covOptional,           // PrintZoomRow          ditto
			covOptional,           // PrintZoomPaperWidth   ditto
			covOptional);          // PrintZoomPaperHeight  ditto
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CWordOperate::Print,2,catch CException [%s]", tcErrMsg);
		return FALSE;
	}

	return TRUE;
}