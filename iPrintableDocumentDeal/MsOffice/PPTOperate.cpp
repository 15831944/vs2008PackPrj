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

//����  
//**********************�������ĵ�*******************************************  
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
			theLog.Write("!!CPPTOperate::CreateApp,����ʧ��,1����ȷ����װ��PPT 2000�����ϰ汾!err=%d", GetLastError());  
			return FALSE;  
		}

		m_oApp.put_DisplayAlerts(FALSE);
	}
	catch (CException* e)
	{
		TCHAR tcErrMsg[512] = {0};
		e->GetErrorMessage(tcErrMsg, 512);
		theLog.Write("!!CPPTOperate::CreateApp,����ʧ��,2,catch CException [%s]", tcErrMsg);
		return FALSE;  
	}

	if (m_oApp.m_lpDispatch)
	{
		theLog.Write("CPPTOperate::CreateApp,�����ɹ�,3��Version=%s", GetVersion());
		m_bInitOk = TRUE;
		return TRUE;  
	}
	else
	{
		theLog.Write("!!CPPTOperate::CreateApp,����ʧ��,4");
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


//��Ϣ
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

//**********************���ĵ�*********************************************  
BOOL CPPTOperate::OpenDocument(CString fileName)
{
	if (!m_oPresentations.m_lpDispatch)   
	{  
		theLog.Write("!!CPPTOperate::OpenDocument,1,m_oPresentations fail");  
		return FALSE;  
	}  

	try
	{
#if 0	//m_oPresentations.Open,����ĵ������룬���̻߳�����
		m_oPresentation.AttachDispatch(m_oPresentations.Open(
										(LPCTSTR)fileName,	//LPCTSTR FileName
										1,					//long ReadOnly
										1,					//long Untitled
										1));				//long WithWindow
#else	
		//ʹ�ñ�����ͼ��ʽ�����ĵ���
		//����һ�����������(nullpwd)����ֹ������������򣬴Ӷ������̣߳�
		//Ч�����ǣ�һ������������ĵ���ֱ���׳��쳣����ʧ�ܣ�
		//������û��������ĵ���ֱ�Ӵ��ĵ�,Ȼ����ͼ�л�Ϊ�ɱ༭��ͼ��
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
			//��ʾ�ĵ�û�����룬�򽫱�����ͼ�ĵ����ñ༭������
			//��ΪProtectedViewWindows�򿪵��ĵ����ܵ���pdf��
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

//**********************�����ĵ�*********************************************  
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
			msoFalse,					//long FrameSlides	//�õ�Ƭ�ӿ�
			ppPrintHandoutVerticalFirst,	//long HandoutOrder
			ppPrintOutputSlides,		//long OutputType	
										//ppPrintOutputBuildSlides������PPT2010�Լ�֮��İ汾�в�֧�֣�
										//�����ʧ��,����Presentation.ExportAsFixedFormat : Invalid request.  
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
		//�ͷ��ڴ�������Դ  
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

//��ӡ�ļ�
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
