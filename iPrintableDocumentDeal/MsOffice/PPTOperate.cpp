#include "StdAfx.h"
#include "MSOfficeHeader.h"
#include "PPTOperate.h"

CPPTOperate::CPPTOperate(void)
{
	CloseApp();
}

CPPTOperate::~CPPTOperate(void)
{
	CloseApp();
}

//操作  
//**********************创建新文档*******************************************  
BOOL CPPTOperate::CreateApp()
{
	if (m_bInitOk)
	{
		return TRUE;
	}

	CloseApp();

	try
	{
		if (!m_oApp.CreateDispatch(_T("PowerPoint.Application")))  
		{  
			theLog.Write("!!CPPTOperate::CreateApp,创建失败,1，请确保安装了PPT 2000或以上版本!err=%d", GetLastError());  
			return FALSE;  
		}

		m_oApp.put_DisplayAlerts(FALSE);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::CreateApp,创建失败,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	if (m_oApp.m_lpDispatch)
	{
		theLog.Write("CPPTOperate::CreateApp,创建成功,3，Version=%s", GetVersion());
		m_bInitOk = TRUE;
		return TRUE;  
	}
	else
	{
		theLog.Write("!!CPPTOperate::CreateApp,创建失败,4");
		return FALSE;  
	}	
}

BOOL CPPTOperate::CreatePresentations()
{
	if (!CreateApp())   
	{
		theLog.Write("!!CPPTOperate::CreatePresentations,1,CreateApp fail!");  
		return FALSE;  
	}

	if (m_oPresentations.m_lpDispatch)
	{
		theLog.Write("#CPPTOperate::CreatePresentations,2,m_lpDispatch=%p", m_oPresentations.m_lpDispatch);  
		return TRUE;  
	}

	try
	{
		m_oPresentations.AttachDispatch(m_oApp.get_Presentations()); 
		if (!m_oPresentations.m_lpDispatch)   
		{  
			theLog.Write("!!CPPTOperate::CreatePresentations,3,m_oPresentations fail!");  
			return FALSE;  
		}
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::CreatePresentations,4,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}

BOOL CPPTOperate::CreatePresentation()
{
	if (!m_oPresentations.m_lpDispatch)   
	{  
		theLog.Write("!!CPPTOperate::CreatePresentation,1,m_oPresentations fail");  
		return FALSE;  
	}  

	try
	{
		m_oPresentation.AttachDispatch(m_oPresentations.Add(1));
		if (!m_oPresentation.m_lpDispatch)   
		{  
			theLog.Write("!!CPPTOperate::CreatePresentation,2,m_oPresentation fail");  
			return FALSE;  
		}  
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::CreatePresentation,3,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}

BOOL CPPTOperate::Create()
{
	if (!CreatePresentations())   
	{
		theLog.Write("!!CPPTOperate::Create,1,CreatePresentations fail.");
		return FALSE;  
	}  

	return CreatePresentation();  
}

void CPPTOperate::SetVisible(BOOL bVisible)
{
	try
	{
		m_oApp.put_Visible(bVisible);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::SetVisible,catch CException [%s],err=%d,bVisible=[%d]", tcErrMsg, GetLastError(), bVisible);
	}
}


//信息
CString CPPTOperate::GetVersion()
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
		theLog.Write("!!CPPTOperate::GetVersion,catch CException [%s],err=%d", tcErrMsg, GetLastError());
	}

	return szVersion;
}

//**********************打开文档*********************************************  
BOOL CPPTOperate::OpenDocument(CString fileName)
{
	if (!m_oPresentations.m_lpDispatch)   
	{  
		theLog.Write("!!CPPTOperate::OpenDocument,1,m_oPresentations fail");  
		return FALSE;  
	}  

	try
	{
#if 0	//m_oPresentations.Open,如果文档有密码，则线程会阻塞
		m_oPresentation.AttachDispatch(m_oPresentations.Open(
										(LPCTSTR)fileName,	//LPCTSTR FileName
										1,					//long ReadOnly
										1,					//long Untitled
										1));				//long WithWindow
#else	
		//使用保护视图方式来打开文档，
		//设置一个错误的密码(nullpwd)，防止弹出输入密码框，从而阻塞线程，
		//效果就是：一，当有密码的文档，直接抛出异常，打开失败，
		//二，当没有密码的文档，直接打开文档,然后将视图切换为可编辑视图。
		if (!m_oProtectedViewWindows.m_lpDispatch)
		{
			m_oProtectedViewWindows.AttachDispatch(m_oApp.get_ProtectedViewWindows());
		}
		if (!m_oProtectedViewWindows.m_lpDispatch)   
		{  
			theLog.Write("!!CPPTOperate::OpenDocument,2,m_oProtectedViewWindows fail");  
			return FALSE;  
		}  
		m_oPresentation.AttachDispatch(
			m_oProtectedViewWindows.Open((LPCTSTR)fileName, "nullpwd", 0));
#endif
		if (!m_oPresentation.m_lpDispatch)   
		{  
			theLog.Write("!!CPPTOperate::OpenDocument,3,m_oPresentation fail");  
			return FALSE;  
		}
		else
		{
			//表示文档没有密码，则将保护视图文档启用编辑操作，
			//因为ProtectedViewWindows打开的文档不能导出pdf。
			m_oProtectedViewWindow.AttachDispatch(m_oApp.get_ActiveProtectedViewWindow());
			if (!m_oProtectedViewWindow.m_lpDispatch)   
			{  
				theLog.Write("!!CPPTOperate::OpenDocument,4,m_oProtectedViewWindow fail");  
				return FALSE;  
			}  
			
			m_oPresentation.ReleaseDispatch();
			m_oPresentation.AttachDispatch(m_oProtectedViewWindow.Edit("nullpwd"));
			if (!m_oPresentation.m_lpDispatch)   
			{  
				theLog.Write("!!CPPTOperate::OpenDocument,5,m_oPresentation fail");  
				return FALSE;  
			}
		}
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::OpenDocument,6,catch CException [%s],err=%d,file=[%s]", tcErrMsg, GetLastError(), fileName);
		return FALSE;  
	}

	return TRUE; 
}

BOOL CPPTOperate::Open(CString fileName)
{
	if (!CreatePresentations())   
	{  
		theLog.Write("!!CPPTOperate::Open,CreatePresentations fail");
		return FALSE;  
	}  

	return OpenDocument(fileName);  
}

BOOL CPPTOperate::SetActiveDocument(short i)
{
	theLog.Write("##CPPTOperate::SetActiveDocument,function no support");
	return FALSE;
}

//**********************保存文档*********************************************  
BOOL CPPTOperate::SaveDocument()
{
	if (!m_oPresentation.m_lpDispatch)   
	{  
		//theLog.Write("!!CPPTOperate::SaveDocument,1,m_oPresentation fail");  
		return TRUE;  
	}  
	try
	{
		m_oPresentation.Save();  
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::SaveDocument,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}
	return TRUE;  
}

BOOL CPPTOperate::SaveDocumentAs(CString fileName)
{
	if (!m_oPresentation.m_lpDispatch)   
	{  
		theLog.Write("!!CPPTOperate::SaveDocumentAs,1,m_oPresentation fail,fileName=[%s]", fileName);  
		return FALSE;  
	}  

	try
	{
		m_oPresentation.SaveAs(  
			(LPCTSTR)fileName,		//LPCTSTR FileName  
			ppSaveAsDefault,		//long FileFormat
			msoTrue					//long EmbedTrueTypeFonts 
			);  	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::SaveDocumentAs,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}

BOOL CPPTOperate::SaveDocumentAs(CString fileName,PpSaveAsFileType ppFormat)
{
	if (!m_oPresentation.m_lpDispatch)   
	{  
		theLog.Write("!!CPPTOperate::SaveDocumentAs,1,m_oPresentation fail,fileName=[%s]", fileName);  
		return FALSE;  
	}  

	try
	{
		m_oPresentation.SaveAs(  
			(LPCTSTR)fileName,		//LPCTSTR FileName  
			ppFormat,				//long FileFormat
			msoTrue					//long EmbedTrueTypeFonts 
			);  	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::SaveDocumentAs,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}

BOOL CPPTOperate::ExportDocumentAs(CString fileName,PpFixedFormatType ppFormat)
{
	if (!m_oPresentation.m_lpDispatch)   
	{  
		theLog.Write("!!CPPTOperate::ExportDocumentAs,1,m_oPresentation fail,fileName=[%s]", fileName);  
		return FALSE;  
	}  

	try
	{
		COleVariant	vtMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
 		m_oPresentation.ExportAsFixedFormat(
 			(LPCTSTR)fileName,			//LPCTSTR Path
			ppFormat,					//long FixedFormatType
			ppFixedFormatIntentPrint,	//long Intent
			msoFalse,					//long FrameSlides	//幻灯片加框
			ppPrintHandoutVerticalFirst,	//long HandoutOrder
			ppPrintOutputSlides,		//long OutputType	
										//ppPrintOutputBuildSlides好像在PPT2010以及之后的版本中不支持，
										//会调用失败,报错：Presentation.ExportAsFixedFormat : Invalid request.  
										//This method or property is no longer supported by this version of PowerPoint.
			msoFalse,					//long PrintHiddenSlides
			NULL,						//LPDISPATCH PrintRange
			ppPrintAll,					//long RangeType
			(LPCTSTR)"",				//LPCTSTR SlideShowName
			FALSE,						//BOOL IncludeDocProperties
			FALSE,						//BOOL KeepIRMSettings
			TRUE,						//BOOL DocStructureTags
			TRUE,						//BOOL BitmapMissingFonts
			FALSE,						//BOOL UseISO19005_1
			vtMissing					//VARIANT& ExternalExporter
			);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::ExportDocumentAs,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	return TRUE;  
}

BOOL CPPTOperate::CloseDocument()
{
	try
	{
		if (m_oPresentation.m_lpDispatch)
		{
			m_oPresentation.Close();
			m_oPresentation.ReleaseDispatch();
		}
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::CloseDocument,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}
	return TRUE;  
}

void CPPTOperate::CloseApp()
{
	m_bInitOk = FALSE;
	
	try
	{
		//释放内存申请资源  
		if (m_oPresentation.m_lpDispatch)
		{
			CloseDocument();
		}

		if (m_oPresentations.m_lpDispatch)
		{
			m_oPresentations.ReleaseDispatch();  
		}

		if (m_oApp.m_lpDispatch)
		{
			m_oApp.Quit();
			m_oApp.ReleaseDispatch();  
		}	
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::CloseApp,catch CException [%s]", tcErrMsg);
	}
}

//打印文件
BOOL CPPTOperate::Print()
{
	if (!m_oPresentation.m_lpDispatch)
	{
		theLog.Write("!!CPPTOperate::Print,1,m_oPresentation fail");
		return FALSE;
	}

	try
	{
		m_oPresentation.PrintOut(
			1,				//long From
			1,				//long To
			"C:\\ppt.emf.spl",	//LPCTSTR PrintToFile
			2,				//long Copies
			0);				//long Collate
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::Print,catch CException [%s]", tcErrMsg);
		return FALSE;
	}
	return TRUE;
}
