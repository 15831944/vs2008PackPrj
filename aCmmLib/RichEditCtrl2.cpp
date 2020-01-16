// RichEditCtrl2.cpp : 实现文件
//

#include "stdafx.h"
#include "RichEditCtrl2.h"
#include "acmmlib.h"
#include "DlgReSetting.h"
#include <atlimage.h>


class CImageDataObject : IDataObject
{
public:
	// This static fumction accepts those parameters:
	// IRichEditOle* : a pointer to IRochEditOle interface for the RichEdit Control
	// HBITMAP : the bitmap handle.
	//
	// After calling the function, it inserts the image in the current 
	//    position of the RichEdit
	//
	static void InsertBitmap(IRichEditOle* pRichEditOle, HBITMAP hBitmap);

private:
	ULONG	m_ulRefCnt;
	BOOL	m_bRelease;

	// The data being bassed to the richedit
	//
	STGMEDIUM m_stgmed;
	FORMATETC m_fromat;
	HBITMAP		m_hBitmap;
public:
	CImageDataObject() : m_ulRefCnt(0)
	{
		m_bRelease = FALSE;
		m_hBitmap = NULL;
	}

	~CImageDataObject()
	{
		if (m_bRelease)
			::ReleaseStgMedium(&m_stgmed);
		//DeleteObject(m_hBitmap);
	}

	// Methods of the IUnknown interface
	// 
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{
		if (iid == IID_IUnknown || iid == IID_IDataObject)
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}
	STDMETHOD_(ULONG, AddRef)(void)
	{
		m_ulRefCnt++;
		return m_ulRefCnt;
	}
	STDMETHOD_(ULONG, Release)(void)
	{
		if (--m_ulRefCnt == 0)
		{
			delete this;
		}
		return m_ulRefCnt;
	}

	// Methods of the IDataObject Interface
	//
	STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
	{
		HANDLE hDst;
		hDst = ::OleDuplicateData(m_stgmed.hBitmap, CF_BITMAP, NULL);
		if (hDst == NULL)
		{
			return E_HANDLE;
		}

		pmedium->tymed = TYMED_GDI;
		pmedium->hBitmap = (HBITMAP)hDst;
		pmedium->pUnkForRelease = NULL;
		m_hBitmap = pmedium->hBitmap;
		//DeleteObject(pmedium->hBitmap);
		return S_OK;
	}
	STDMETHOD(GetDataHere)(FORMATETC* pformatetc, STGMEDIUM*  pmedium )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(QueryGetData)(FORMATETC*  pformatetc )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC*  pformatectIn ,FORMATETC* pformatetcOut ) 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetData)(FORMATETC* pformatetc , STGMEDIUM*  pmedium , BOOL  fRelease )
	{
		m_fromat = *pformatetc;
		m_stgmed = *pmedium;
		//添加本行，则相同hbitmap只能显示一次
		//m_bRelease = fRelease;//////////////////////////改////////////////////////////////////////////////
		return S_OK;
	}
	STDMETHOD(EnumFormatEtc)(DWORD  dwDirection , IEnumFORMATETC**  ppenumFormatEtc )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DAdvise)(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink,
		DWORD *pdwConnection) 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DUnadvise)(DWORD dwConnection) 
	{
		return E_NOTIMPL;
	}
	STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppenumAdvise) 
	{
		return E_NOTIMPL;
	}

	// Some Other helper functions
	//
	void SetBitmap(HBITMAP hBitmap);
	IOleObject *GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage);

};

CRHFontConfig::CRHFontConfig()
{
	m_bgColor = GetSysColor(COLOR_WINDOW);
	LoadSettings();
}
CRHFontConfig::~CRHFontConfig()
{
	SaveSettings();
}
void CRHFontConfig::LoadSettings()
{
	CWinApp* pApp = &theApp;
	if (pApp == NULL)
		return;

	COLORREF cr = GetSysColor(COLOR_WINDOW);

	m_bgColor = RGB(pApp->GetProfileInt(STRING_SEC_SETTING, "bgColorR", GetRValue(cr)),
		pApp->GetProfileInt(STRING_SEC_SETTING, "bgColorG", GetGValue(cr)), 
		pApp->GetProfileInt(STRING_SEC_SETTING, "bgColorB", GetBValue(cr)));


	RICHEDITFONT* pArray[5] = {&m_rfTime, &m_rfName, &m_rfInfo, &m_rfNormal, &m_rfError};
	COLORREF      crDefault[5]= {RGB(40, 40, 40), RGB(0, 128, 255),  RGB(0, 128, 0), RGB(0, 0, 0),RGB(255, 0, 0)};
	CString str;
	for (int i = 0; i < 5; i++)
	{
		cr = crDefault[i];
		str.Format("%d", i);
		pArray[i]->lfHeight = pApp->GetProfileInt(STRING_SEC_SETTING, "FontHeight" + str, -12);
		pArray[i]->lfBold = pApp->GetProfileInt(STRING_SEC_SETTING, "Bold"+ str, 1);
		pArray[i]->lfItalic = pApp->GetProfileInt(STRING_SEC_SETTING, "Italic"+ str, 0);
		pArray[i]->lfStrikeOut = pApp->GetProfileInt(STRING_SEC_SETTING, "Strikeout"+ str, 0);
		pArray[i]->lfUnderline = pApp->GetProfileInt(STRING_SEC_SETTING, "Underline"+ str, 0);
		strcpy(pArray[i]->lfFaceName, pApp->GetProfileString(STRING_SEC_SETTING, "Facename"+ str, CCommonFun::GetFaceName()));
		pArray[i]->color = RGB(pApp->GetProfileInt(STRING_SEC_SETTING, "FontColorR"+ str, GetRValue(cr)),
			pApp->GetProfileInt(STRING_SEC_SETTING, "FontColorG"+ str, GetGValue(cr)), 
			pApp->GetProfileInt(STRING_SEC_SETTING, "FontColorB"+ str, GetBValue(cr)));

	}
}

void CRHFontConfig::SaveSettings()
{
	CWinApp* pApp = &theApp;
	if (pApp == NULL)
		return;

	pApp->WriteProfileInt(STRING_SEC_SETTING, "bgColorR", GetRValue(m_bgColor));
	pApp->WriteProfileInt(STRING_SEC_SETTING, "bgColorG", GetGValue(m_bgColor));
	pApp->WriteProfileInt(STRING_SEC_SETTING, "bgColorB", GetBValue(m_bgColor));


	RICHEDITFONT* pArray[5] = {&m_rfTime, &m_rfName, &m_rfInfo, &m_rfNormal, &m_rfError};
	CString str;
	for (int i = 0; i < 5; i++)
	{
		COLORREF cr = pArray[i]->color;
		str.Format("%d", i);
		pApp->WriteProfileInt(STRING_SEC_SETTING, "FontHeight"+ str, pArray[i]->lfHeight);
		pApp->WriteProfileInt(STRING_SEC_SETTING, "Bold"+ str, pArray[i]->lfBold);
		pApp->WriteProfileInt(STRING_SEC_SETTING, "Italic"+ str, pArray[i]->lfItalic);
		pApp->WriteProfileInt(STRING_SEC_SETTING, "Strikeout"+ str, pArray[i]->lfStrikeOut);
		pApp->WriteProfileInt(STRING_SEC_SETTING, "Underline"+ str, pArray[i]->lfUnderline);
		pApp->WriteProfileString(STRING_SEC_SETTING, "Facename"+ str, pArray[i]->lfFaceName);
		pApp->WriteProfileInt(STRING_SEC_SETTING, "FontColorR"+ str, GetRValue(cr));
		pApp->WriteProfileInt(STRING_SEC_SETTING, "FontColorG"+ str, GetGValue(cr)); 
		pApp->WriteProfileInt(STRING_SEC_SETTING, "FontColorB"+ str, GetBValue(cr));
	}
}
CRHFontConfig& CRHFontConfig::GetInstance()
{
	static CRHFontConfig theConfig;
	return theConfig;
}
// CRichEditCtrl2
#ifdef _RH_SHOW_GIF
const GUID GIFINFO::pDimensionIDs = FrameDimensionTime;//gif
#endif

IMPLEMENT_DYNAMIC(CRichEditCtrl2, CRichEditCtrl)

CRichEditCtrl2::CRichEditCtrl2()
{
	m_DefaultBmp = NULL;
	m_pIRichEditOleCallback = NULL;

	m_pOwnerWnd = NULL;
	m_bAutoDisable = FALSE;
	m_pRichEditOle = NULL;
	//	m_PicBgColor.SetValue(0x11FFFFFF);

	m_nMaxLen =  INT_MAX;

#ifdef _RH_SHOW_GIF
	m_nGifTimer = 1;
#endif
	ZeroMemory(&m_DefaultLinkFormat,   sizeof(CHARFORMAT2));//   
	m_DefaultLinkFormat.cbSize   =   sizeof(CHARFORMAT2);   
	m_DefaultLinkFormat.dwMask   =   CFM_LINK | CFM_SIZE | CFM_FACE | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_STRIKEOUT | CFM_UNDERLINE;
	m_DefaultLinkFormat.crTextColor = m_DefaultFont.color;
	m_DefaultLinkFormat.dwEffects = 0;
	strcpy(m_DefaultLinkFormat.szFaceName, m_DefaultFont.lfFaceName);

	//不设置这些个乱七八糟的东西
	/*
	if ( m_DefaultFont.lfBold)
	m_DefaultLinkFormat.dwEffects |= CFE_BOLD;
	if (m_DefaultFont.lfItalic)
	m_DefaultLinkFormat.dwEffects |= CFE_ITALIC;
	if (m_DefaultFont.lfUnderline)
	m_DefaultLinkFormat.dwEffects |= CFE_UNDERLINE;
	if (m_DefaultFont.lfStrikeOut)
	m_DefaultLinkFormat.dwEffects |= CFE_STRIKEOUT;
	*/
	m_bEnableOption = FALSE;
	m_bAutoDelete = FALSE;
	m_nMaxMsgCount = 30;
	m_nMaxPicCount = 30;
	m_nMsgCount = 0;
	m_DefaultLinkFormat.yHeight =  - 15 * m_DefaultFont.lfHeight;

	CString strName;
	strName.Format("re2-%s", AfxGetAppName());
	m_log.SetLogName(strName);
	m_bWordPaste = FALSE;
	m_bNoContextMenu = FALSE;
	m_nColorTimer = 1001;
}

CRichEditCtrl2::~CRichEditCtrl2()
{
	if (m_hWnd != NULL)
		ClearContent();

	if (m_pRichEditOle)
	{
		m_pRichEditOle->Release();
		m_pRichEditOle = NULL;
	}
}


BEGIN_MESSAGE_MAP(CRichEditCtrl2, CRichEditCtrl)
	ON_NOTIFY_REFLECT(EN_LINK, &CRichEditCtrl2::OnEnLink)
	ON_WM_DROPFILES()
	ON_CONTROL_REFLECT(EN_CHANGE, &CRichEditCtrl2::OnEnChange)
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_COMMAND(ID_RICHEDIT_COPY, &CRichEditCtrl2::OnRicheditCopy)
	ON_UPDATE_COMMAND_UI(ID_RICHEDIT_COPY, &CRichEditCtrl2::OnUpdateRicheditCopy)
	ON_COMMAND(ID_RICHEDIT_PATSTE, &CRichEditCtrl2::OnRicheditPatste)
	ON_UPDATE_COMMAND_UI(ID_RICHEDIT_PATSTE, &CRichEditCtrl2::OnUpdateRicheditPatste)
	ON_COMMAND(ID_RICHEDIT_OPTION, &CRichEditCtrl2::OnRicheditOption)
	ON_UPDATE_COMMAND_UI(ID_RICHEDIT_OPTION, &CRichEditCtrl2::OnUpdateRicheditOption)
	ON_WM_RBUTTONUP()
	//	ON_MESSAGE(WM_P_RE_ADDTEXT_TYPE, OnAddTextType)
	ON_MESSAGE(WM_P_RE_ADD_LINE, OnAddLine)
	ON_MESSAGE(WM_P_RE_ADDTEXT_RF, OnAddTextRF)
	ON_MESSAGE(WM_P_RE_ADD_SYSMSG, OnAddSysMsg)
END_MESSAGE_MAP()



// CRichEditCtrl2 消息处理程序


//点击超链接
void CRichEditCtrl2::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
#pragma message(__LOC__"点两次的问题，与button区别")
	//ENLINK *pEnLink = ;
	ENLINK  *penLink =  reinterpret_cast<ENLINK *>(pNMHDR);
	TRACE("penLink->msg = %d\n", penLink->msg);
	if(penLink->msg   ==   WM_LBUTTONUP)   
	{   
		//Notify the parent

		DWORD dwCode = 0;
		for ( m_LinkIter = m_LinkMap.begin(); m_LinkIter != m_LinkMap.end(); m_LinkIter++)
		{
			if ((m_LinkIter->second).cpMin == penLink->chrg.cpMin && (m_LinkIter->second).cpMax == penLink->chrg.cpMax)
			{
				dwCode  = m_LinkIter->first;
				break;
			}
		}

		ASSERT(m_pOwnerWnd != NULL);
		if (m_pOwnerWnd)
			m_pOwnerWnd->SendMessage(WM_P_CLICK_LINK, WPARAM(GetDlgCtrlID ()), LPARAM(dwCode));

		//Remove the link if needed
		if (m_bAutoDisable)
			RemoveLink(dwCode);
	}
	*pResult = 0;
}

//支持超链接点击响应
void CRichEditCtrl2::SetLinkMode(BOOL bLink)
{
	if (bLink)
	{
		SetEventMask(ENM_LINK);
		m_pOwnerWnd = GetParent();
	}
	else
		SetEventMask(~ENM_LINK);
}
//添加链接文本
void CRichEditCtrl2::AddLinkText(CString strText, UINT nID)
{
	ASSERT(nID != 0);

	CHARFORMAT2   cf2 = m_DefaultLinkFormat;
	cf2.dwEffects   |=   CFE_LINK; 
#if 1
	//Add text
	CString   strTotalText;
	GetWindowText(strTotalText);
	int line = GetNewLineCount(strTotalText);   
	long iTotalTextLength =  MultiByteToWideChar(CP_GB2312,   0,   strTotalText,   (int)strlen(strTotalText),   NULL,   0);   
	SetSel(iTotalTextLength, iTotalTextLength);
	ReplaceSel(strText);

	//Add Link
	long iStartPos = iTotalTextLength;

	iStartPos -= line ;
	GetWindowText(strTotalText);      
	long iEndPos =  MultiByteToWideChar(CP_GB2312,   0,   strTotalText,   (int)strlen(strTotalText),   NULL,   0);   
	line = GetNewLineCount(strTotalText);
	iEndPos -= line;
	SetSel(iStartPos, iEndPos);
	long nStart = iStartPos;
	SetSelectionCharFormat(cf2);
	SetSel(-1, -1);
	long nEnd = iEndPos;
#else
	CHARFORMAT2   Oldcf;
	long iStartPos = 0;
	long iEndPos = 0;
	GetSel(iStartPos, iEndPos);

	long nStart = iStartPos;
	ASSERT(::IsWindow(m_hWnd));
	ReplaceSel(strText);
	long iLeft = iStartPos;
	GetSel(iStartPos, iEndPos);

	SetSel(iLeft, iStartPos);

	GetSelectionCharFormat(Oldcf);
	SetSelectionCharFormat(cf2);
	SetSel(-1, -1);
	SetSelectionCharFormat(Oldcf);
	long nEnd = iStartPos;
#endif
	m_LinkMap[nID].cpMin = nStart;
	m_LinkMap[nID].cpMax = nEnd;

	//滚动到最后一行
	PostMessage(WM_VSCROLL, SB_BOTTOM,0);
}

//添加非连接文本
/*
void CRichEditCtrl2::AddText(CString strText,int iType = TEXT_NORMAL)
{
long iTotalTextLength = GetWindowTextLength();
SetSel(iTotalTextLength, iTotalTextLength);
ReplaceSel(strText);
}
*/

void CRichEditCtrl2::AddText(CString strText, int iType/* = TEXT_NORMAL*/)
{
	if (m_bEnableOption)
	{
		RICHEDITFONT rf;
		switch (iType)
		{
		case TEXT_ERROR:
			rf = CRHFontConfig::GetInstance().m_rfError;
			break;
		case TEXT_NAME:
			rf = CRHFontConfig::GetInstance().m_rfName;
			break;
		case TEXT_NORMAL:
			rf = CRHFontConfig::GetInstance().m_rfNormal;
			break;
		case TEXT_INFORMATION:
			rf = CRHFontConfig::GetInstance().m_rfInfo;
			break;
		case TEXT_TIME:
			rf = CRHFontConfig::GetInstance().m_rfTime;
			break;
		}
		AddText(strText,rf);
	}
	else
	{

		COLORREF crNewColor ;
		switch (iType)
		{
		case TEXT_ERROR:
			crNewColor = RGB(255, 0, 0);
			break;
		case TEXT_NAME:
			crNewColor = RGB(0, 128, 255);
			break;
		case TEXT_NORMAL:
			crNewColor = RGB(0, 0, 0);
			break;
		case TEXT_INFORMATION:
			crNewColor = RGB(0, 128, 0);
			break;
		case TEXT_TIME:
			crNewColor = RGB(40, 40, 40);
			break;
		}
		AddText(strText,crNewColor);
	}
}

void CRichEditCtrl2::AddText(CString strText, COLORREF &crNewColor)
{
	RICHEDITFONT ft = m_DefaultFont;
	ft.color = crNewColor;
	AddText(strText, ft);
}

void CRichEditCtrl2::AddText(LPCTSTR lpSTR, RICHEDITFONT& REFont)
{
	::SendMessage(m_hWnd, WM_P_RE_ADDTEXT_RF, (WPARAM)lpSTR, (LPARAM)&REFont);
}
//删除某一命令的所有链接
BOOL CRichEditCtrl2::RemoveLink(UINT nID,BOOL bDel)
{
	CHARFORMAT2   cf2 = m_DefaultLinkFormat;   

	cf2.dwEffects   &=   ~CFE_LINK; 

	CHARRANGE cr;
	cr.cpMin = m_LinkMap[nID].cpMin;
	cr.cpMax = m_LinkMap[nID].cpMax;
	
	SetSel(cr);
	SetSelectionCharFormat(cf2);
	m_LinkMap.erase(nID);

	// 如果删除链接的同时还需要删除
	if (bDel)
	{
		int nDelLen = cr.cpMax - cr.cpMin;
		for(m_LinkIter = m_LinkMap.begin(); m_LinkIter != m_LinkMap.end(); m_LinkIter++)
		{
			if (m_LinkIter->second.cpMin > cr.cpMax)
			{
				m_LinkIter->second.cpMin -= nDelLen + 3;
				m_LinkIter->second.cpMax -= nDelLen + 3;
			}
		}
		cr.cpMax += 3;
		SetSel(cr);
		ReplaceSel(_T(""));
	}
	SetSel(-1, -1);
	return TRUE;
}

void CRichEditCtrl2::SetAutoDisable(BOOL bAutoDisable /* = TRUE */)
{
	m_bAutoDisable = TRUE;
}

BOOL CRichEditCtrl2::GetAutoDisable()const
{
	return m_bAutoDisable;
}
void CRichEditCtrl2::SetNotifyWnd(CWnd* pWnd)
{
	m_pOwnerWnd = pWnd;
}
void CRichEditCtrl2::AddMessage(CString strName, CString strMsg)
{
	AddText(strName, TEXT_NAME);
	AddText("  ");
	AddTime();
	//AddText(":", TEXT_TIME);
	NewLine();
	AddText(strMsg);
	NewLine();
}
void CRichEditCtrl2::AddTime()
{
	CString str;
	CTime t;
	t=t.GetCurrentTime();
	str.Format("%02d:%02d:%02d ", t.GetHour(),t.GetMinute(),t.GetSecond());
	AddText(str, TEXT_TIME);
}

void CRichEditCtrl2::NewLine()
{
	AddText("\n");
	//long iTotalTextLength = GetWindowTextLength();
	//SetSel(iTotalTextLength, iTotalTextLength);
	//ReplaceSel("\n");
}
void CRichEditCtrl2::AddSpace()
{
	AddText(" ");
}
void CRichEditCtrl2::AddSysMessage(LPCTSTR lpSTR, int iType /* = TEXT_INFORMATION */)
{
	::SendMessage(m_hWnd, WM_P_RE_ADD_SYSMSG, WPARAM(lpSTR), iType);
}

BOOL  CRichEditCtrl2::RemoveAllLinks()
{
	while (m_LinkMap.size() > 0)
	{
		m_LinkIter = m_LinkMap.begin(); 
		if (m_LinkIter != m_LinkMap.end())
		{
			RemoveLink(m_LinkIter->first);	
		}
	}
	return TRUE;
}

/*
Versions for Microsoft Windows include the following:
Year Released Name Comments 
1989 Word for Windows 1.0 code-named Opus 
1990 Word for Windows 1.1 code-named Bill the Cat 
1990 Word for Windows 1.1a for Windows 3.1 
1991 Word for Windows 2.0 code-named Spaceman Spiff 
1993 Word for Windows 6.0 code-named T3 (renumbered 6 to bring Windows version numbering in line with that of DOS version, Macintosh version and also WordPerfect, the main competing word processor at the time; also a 32-bit version for Windows NT only) 
1995 Word 95 Word 95 (version 7.0) – included in Office 95 
1997 Word 97 (version 8.0) included in Office 97 
1998 Word 98 (version 8.5) only included in Office 97 Powered By Word 98—only released in Japan and Korea 
1999 Word 2000 (version 9.0) included in Office 2000 
2001 Word 2002 (version 10) included in Office XP 
2003 Word 2003 officially "Microsoft Office Word 2003") – (ver. 11) included in Office 2003 
2006 Word 2007 (officially "Microsoft Office Word 2007") – (ver. 12) included in Office 2007; released to businesses on November 30, 2006, released worldwide to consumers on January 30, 2007 
2010 Word 2010 (version 14) Included in Office 2010 
*/
class CDisplayWordParam
{
public:
	CRichEditCtrl2* pCtrl;
	BOOL*			pbWordPaste;
	char			szPath[MAX_PATH + 1];
};

UINT DisplayWordThread(LPVOID lParam)
{
	CDisplayWordParam* pParam = (CDisplayWordParam*)lParam;
	if (pParam == NULL)
		return 0;
	if (!IsWindow(pParam->pCtrl->GetSafeHwnd()))
		return 0;
	if (!PathFileExists(pParam->szPath))
		return 0;
	CoInitialize(NULL);
	HINSTANCE hIns = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	pParam->pCtrl->SetWindowText(CFFS(IDS_WAITING));
	AfxSetResourceHandle(hIns);
#if 0
	try
	{
		CClipboardBackup	ClipBrdBackUp;
		_Application m_App; 
		Documents m_Docs; 
		_Document m_Doc; 
		Range m_Range; 

		if(!m_App.CreateDispatch("Word.Application")) 
		{ 
			TRACE("Word.CreateDispatch failed!!");
			return FALSE; 
		} 
		m_Docs=m_App.GetDocuments();

		VARIANT     varFalse;   
		VariantInit(&varFalse);   varFalse.vt=VT_BOOL;   varFalse.boolVal=false;   

		m_Doc = m_Docs.Open(COleVariant(pParam->szPath), &varFalse, &varFalse,     
			&varFalse, &vtMissing, &vtMissing, &vtMissing, &vtMissing, &vtMissing, &vtMissing,     
			&vtMissing, &varFalse, &vtMissing, &vtMissing, &vtMissing, &vtMissing);

		m_Range = m_Doc.GetContent(); 

		pParam->pCtrl->SetReadOnly(FALSE);
		ClipBrdBackUp.Backup();
		m_Range.Copy();
		pParam->pCtrl->SetSel(0, -1);
		*(pParam->pbWordPaste) = TRUE;
		pParam->pCtrl->Paste();
		*(pParam->pbWordPaste) = FALSE;
		ClipBrdBackUp.Restore();
		pParam->pCtrl->SetReadOnly(TRUE);
		m_App.Quit(&vtMissing, &vtMissing, &vtMissing); 
	}
	catch(...)
	{
		TRACE("Quit app failed.\n");
	}
#else
	int nVer = GetCurrentWordVersion();
	try
	{
		if (nVer <= 0)
		{
			HINSTANCE hIns = AfxGetResourceHandle();
			AfxSetResourceHandle(theApp.m_hInstance);
			pParam->pCtrl->AddText(CFFS(IDS_WORD_NOT_INSTALL), TEXT_ERROR);
			AfxSetResourceHandle(hIns);
		}
		else if (nVer < 9)
		{
			HINSTANCE hIns = AfxGetResourceHandle();
			AfxSetResourceHandle(theApp.m_hInstance);
			pParam->pCtrl->AddText(CFFS(IDS_WORD_LOW_VER), TEXT_ERROR);
			AfxSetResourceHandle(hIns);
		}
		else if (nVer == 9 || nVer == 10)
		{
			//word 2000, or 2000/xp
			CClipboardBackup	ClipBrdBackUp;
			_Application m_App;//定义Word提供的应用程序对象；
			Documents m_Docs;//定义Word提供的文档对象；
			Selection m_Sel;//定义Word提供的选择对象；
			m_Docs.ReleaseDispatch();
			m_Sel.ReleaseDispatch();
			m_App.m_bAutoRelease=true;
			if(!m_App.CreateDispatch("Word.Application"))
			{ 
				TRACE("创建Word服务失败!"); 
				return 0;
			} 
			//下面是定义VARIANT变量；
			COleVariant varFilePath(pParam->szPath);
			COleVariant varstrNull("");
			COleVariant varZero((short)0);
			COleVariant varTrue(short(1),VT_BOOL);
			COleVariant varFalse(short(0),VT_BOOL);
			m_Docs.AttachDispatch(m_App.GetDocuments());//将Documents类对象m_Docs和Idispatch接口关 联起来；
			m_Docs.Open2000(varFilePath,varFalse,varFalse,varFalse,
				varstrNull,varstrNull,varFalse,varstrNull,
				varstrNull,varTrue,varTrue,varTrue);
			//打开Word文档； 
			m_Sel.AttachDispatch(m_App.GetSelection());//将Selection类对象m_Sel和Idispatch接口关联 起来；
			m_Sel.WholeStory ();//选择文档中的全部内容；
			pParam->pCtrl->SetReadOnly(FALSE);
			ClipBrdBackUp.Backup();
			m_Sel.Copy();//将数据拷贝到剪贴板
			m_Docs.ReleaseDispatch();//断开关联；
			m_Sel.ReleaseDispatch();
			m_App.Quit(&varFalse, &varZero, &varZero);

			pParam->pCtrl->SetSel(0, -1);
			*(pParam->pbWordPaste) = TRUE;
			pParam->pCtrl->Paste();
			//*(pParam->pbWordPaste) = FALSE;
			ClipBrdBackUp.Restore();
			pParam->pCtrl->SetReadOnly(TRUE);
			//滚动到第一行
			pParam->pCtrl->PostMessage(WM_VSCROLL, SB_TOP,0);
		}
		/*
		else if (nVer == 10)
		{
			//word 2002/xp
			//验证过，可以使用open2000。
			CCommonFun::WriteFileLog("word 2000.");
			//throw 10;
		}
		*/
		else if (nVer >= 11)
		{
			//word 2003 or higher
			CClipboardBackup	ClipBrdBackUp;
			_Application m_App; 
			Documents m_Docs; 
			_Document m_Doc; 
			Range m_Range; 

			if(!m_App.CreateDispatch("Word.Application")) 
			{ 
				TRACE("Word.CreateDispatch failed!!");
				return FALSE; 
			} 
			m_Docs=m_App.GetDocuments();

			VARIANT     varFalse;   
			VariantInit(&varFalse);   varFalse.vt=VT_BOOL;   varFalse.boolVal=false;   

			m_Doc = m_Docs.Open(COleVariant(pParam->szPath), &varFalse, &varFalse,     
				&varFalse, &vtMissing, &vtMissing, &vtMissing, &vtMissing, &vtMissing, &vtMissing,     
				&vtMissing, &varFalse, &vtMissing, &vtMissing, &vtMissing, &vtMissing);

			m_Range = m_Doc.GetContent(); 

			pParam->pCtrl->SetReadOnly(FALSE);
			ClipBrdBackUp.Backup();
			m_Range.Copy();
			//Sleep(1000);
			pParam->pCtrl->SetSel(0, -1);
			*(pParam->pbWordPaste) = TRUE;
			pParam->pCtrl->Paste();

			//*(pParam->pbWordPaste) = FALSE;

			ClipBrdBackUp.Restore();
			pParam->pCtrl->SetReadOnly(TRUE);
			m_App.Quit(&vtMissing, &vtMissing, &vtMissing); 
		}
	}
	catch(...)
	{
		CCommonFun::WriteFileLog("exception caught when display word.");
	}
	//在word2000下使用msword9.h正常，在word2003下使用msword.h有问题，2003下弹出无法保存的框。
	delete pParam;
	pParam = NULL;
#endif
	return 0;
}
BOOL CRichEditCtrl2::DisplayWord(CString strWordFilePath)
{
	if (m_pIRichEditOleCallback == NULL )
	{
		m_pIRichEditOleCallback = new IExRichEditOleCallback;
		if (!SetOLECallback( m_pIRichEditOleCallback ))
		{
			ASSERT(0);
			return FALSE;
		}
	}
#if 0
	CWaitCursor wc;
	_Application m_App; 
	Documents m_Docs; 
	_Document m_Doc; 
	Range m_Range; 

	if(!m_App.CreateDispatch("Word.Application")) 
	{ 
		TRACE("\nmyWord.CreateDispatch failed!!");
		return FALSE; 
	} 
	m_Docs=m_App.GetDocuments();

	VARIANT     varFalse;   
	VariantInit(&varFalse);   varFalse.vt=VT_BOOL;   varFalse.boolVal=false;   

	m_Doc = m_Docs.Open(COleVariant(strWordFilePath), &varFalse, &varFalse,     
		&varFalse, &vtMissing, &vtMissing, &vtMissing, &vtMissing, &vtMissing, &vtMissing,     
		&vtMissing, &varFalse, &vtMissing, &vtMissing, &vtMissing, &vtMissing);

	m_Range = m_Doc.GetContent(); 

	SetReadOnly(FALSE);
	m_ClipBrdBackUp.Backup();
	m_Range.Copy();
	SetSel(0, -1);
	m_bWordPaste = TRUE;
	Paste();
	m_bWordPaste = FALSE;
	m_ClipBrdBackUp.Restore();
	try
	{
		m_App.Quit(&vtMissing, &vtMissing, &vtMissing); 
	}
	catch(...)
	{
		TRACE("Quit app failed.\n");
	}
#else
	CDisplayWordParam* dwp = new CDisplayWordParam;
	dwp->pbWordPaste = &m_bWordPaste;
	strcpy(dwp->szPath, strWordFilePath);
	dwp->pCtrl = this;
	AfxBeginThread(DisplayWordThread, dwp);
#endif
	return TRUE;
}

BOOL CRichEditCtrl2::InsertPic(CString strPic)
{
	return InsertPicHelper(strPic);
}
BOOL CRichEditCtrl2::InsertPic(HBITMAP hBitmap)
{
	if (m_pRichEditOle == NULL)
	{
		m_pRichEditOle = GetIRichEditOle();

		ASSERT(m_pRichEditOle != NULL);
	}
#ifdef _BMPLIST
	if (hBitmap != m_DefaultBmp)
		m_bmpList.AddTail(hBitmap);
#endif
	//m_log.Write("InsertPic HBITMAP: pricheditole:%d--hBitmap:%X",m_pRichEditOle, hBitmap);
	CImageDataObject::InsertBitmap(m_pRichEditOle, hBitmap);
	return 0;
}

#ifdef _RH_SHOW_GIF
long CRichEditCtrl2::TryToShowGif(CString strGif)
{
	if (m_pRichEditOle == NULL)
	{
		m_pRichEditOle = GetIRichEditOle();

		ASSERT(m_pRichEditOle != NULL);
	}

	USES_CONVERSION;
	Bitmap* pBitmap = new Bitmap(A2W(strGif));
	GIFINFO gi;
	//gi.pDimensionIDs = (GUID*)malloc(sizeof(GUID));
	//pBitmap->GetFrameDimensionsList(gi.pDimensionIDs, 1);
	gi.nCount = pBitmap->GetFrameCount(&gi.pDimensionIDs);
	pBitmap->SelectActiveFrame(&(gi.pDimensionIDs), 0);
	gi.nCurrent = 0;
	gi.lStartPos = GetWindowTextLength();
	gi.pBitmap = pBitmap;
	m_GifMap[m_nGifTimer] = gi;
	SetTimer(m_nGifTimer, 100, NULL);
	m_nGifTimer++;
	HBITMAP hBitmap = NULL;
	pBitmap->GetHBITMAP(m_PicBgColor, &hBitmap);
	if (hBitmap != NULL)
	{
		//记录图片的位置 
		m_PicMap[gi.lStartPos] = strGif;

		SetSel(-1, -1);
#ifdef _BMPLIST
		if (hBitmap != m_DefaultBmp)
			m_bmpList.AddTail(hBitmap);	
#endif
		CImageDataObject::InsertBitmap(m_pRichEditOle, hBitmap);
	}
	return gi.lStartPos;
}
#endif
BOOL CRichEditCtrl2::InsertIcon(int nIndex, CString strDir, CString strExt)
{
	if (m_pRichEditOle == NULL)
	{
		m_pRichEditOle = GetIRichEditOle();

		ASSERT(m_pRichEditOle != NULL);
	}

	USES_CONVERSION;
	CString strPic;
	strPic.Format("%s%d.%s", strDir, nIndex, strExt);
	InsertPicHelper(strPic, nIndex);
	return TRUE;
}
BOOL CRichEditCtrl2::InsertPicHelper(CString strPic, int nIndex /*= -1*/)
{
	if (m_pRichEditOle == NULL)
	{
		m_pRichEditOle = GetIRichEditOle();

		ASSERT(m_pRichEditOle != NULL);
	}

	HBITMAP hBitmap = NULL;

	if (!PathFileExists(strPic))
		hBitmap = m_DefaultBmp;
	else
	{
#if 1
		hBitmap = LoadAnImage(strPic);
#else
		USES_CONVERSION;
		Bitmap* pBitmap = new Bitmap(A2W(strPic));
		pBitmap->GetHBITMAP(m_PicBgColor, &hBitmap);
		delete pBitmap;
		pBitmap = NULL;
		if (hBitmap == NULL)
		{
			//不是图片
			return FALSE;
		}
#endif
	}
	m_log.Write("InsertPic str:%s-- hbitmap:%X", strPic, hBitmap);
	if (hBitmap != NULL)
	{
		//记录图片的位置
		CString strText;
		GetWindowText(strText);
		int nLen = GetWindowTextLength();
		if (nIndex == -1)
			m_PicMap[nLen] = strPic;
		else
			m_PicMap[nLen] = i2s(nIndex);

		SetSel(-1, -1);

#ifdef _BMPLIST
		if (hBitmap != m_DefaultBmp)
			m_bmpList.AddTail(hBitmap);
#endif
		CImageDataObject::InsertBitmap(m_pRichEditOle, hBitmap);
		if (hBitmap != m_DefaultBmp)
			DeleteObject(hBitmap);

		// 		if (m_pRichEditOle)
		// 		{
		// 			m_pRichEditOle->Release();
		// 			m_pRichEditOle = NULL;
		// 		}
		return TRUE;
	}
	return FALSE;
}
void IDOWriteLog(CString str)
{
	return;
	//	if (!g_AllowDebugLog)
	//		return;
	TRACE(str);
	TRACE("\n");
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	CString strTime;
	strTime.Format("[%d/%d/%d %d:%d:%d.%d]",curTime.wYear,curTime.wMonth,curTime.wDay,
		curTime.wHour,curTime.wMinute,curTime.wSecond,curTime.wMilliseconds);

	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format("%y%m%d");

	CString sTime = time.Format( "%Y-%m-%d %H:%M:%S" );
	CString sTemp;
	sTemp.Format("%s,	 %s",sTime,str);
	sTemp += "\r\n";

	CFile file;
	CString strFile;
	strFile.Format("%s\\Xab_re2-ChatRoomDll_%s.Log", CCommonFun::GetLogDir(), sDate);
	if(file.Open(strFile,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.Write(sTemp,sTemp.GetLength());
		file.Write("\r\n",strlen("\r\n"));
		file.Close();
	}
}
void CImageDataObject::InsertBitmap(IRichEditOle* pRichEditOle, HBITMAP hBitmap)
{
	SCODE sc;

	// Get the image data object
	//
	CImageDataObject *pods = new CImageDataObject;
	LPDATAOBJECT lpDataObject;
	pods->QueryInterface(IID_IDataObject, (void **)&lpDataObject);

	pods->SetBitmap(hBitmap);

	// Get the RichEdit container site
	//
	IOleClientSite *pOleClientSite;	
	pRichEditOle->GetClientSite(&pOleClientSite);


	// Initialize a Storage Object
	//
	IStorage *pStorage;	

	LPLOCKBYTES lpLockBytes = NULL;
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(pStorage != NULL);
	IDOWriteLog("::StgCreateDocfileOnILockBytes");
	// The final ole object which will be inserted in the richedit control
	//
	IOleObject *pOleObject; 
	pOleObject = pods->GetOleObject(pOleClientSite, pStorage);
	if (pOleObject == NULL)
		goto RET;
	IDOWriteLog("pods->GetOleObject");

	// all items are "contained" -- this makes our reference to this object
	//  weak -- which is needed for links to embedding silent update.
	OleSetContainedObject(pOleObject, TRUE);

	// Now Add the object to the RichEdit 
	//
	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);

	CLSID clsid;
	sc = pOleObject->GetUserClassID(&clsid);
	if (sc != S_OK)
		AfxThrowOleException(sc);

	//IDOWriteLog("pOleObject->GetUserClassID");

	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj = pOleObject;
	reobject.polesite = pOleClientSite;
	reobject.pstg = pStorage;

	// Insert the bitmap at the current location in the richedit control
	//
	pRichEditOle->InsertObject(&reobject);
	//IDOWriteLog("pRichEditOle->InsertObject");

	// Release all unnecessary interfaces
	//
	//MessageBox(NULL, "", "", MB_OK);
RET:
	SAFE_RELEASE(pOleObject);//->Release();
	SAFE_RELEASE(pOleClientSite);//->Release();

	//添加本行，清空后可以释放
	SAFE_RELEASE(lpLockBytes);//->Release(); ///////////////////改///////////////////////////////////////////////////////
	SAFE_RELEASE(pStorage);//->Release();
	SAFE_RELEASE(lpDataObject);//->Release();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	ASSERT(hBitmap);

	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle		
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL;				// Use ReleaseStgMedium

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;				// Clipboard format = CF_BITMAP
	fm.ptd = NULL;							// Target Device = Screen
	fm.dwAspect = DVASPECT_CONTENT;			// Level of detail = Full content
	fm.lindex = -1;							// Index = Not applicaple
	fm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle

	this->SetData(&fm, &stgm, TRUE);		
}

IOleObject *CImageDataObject::GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage)
{
	ASSERT(m_stgmed.hBitmap);

	SCODE sc;
	IOleObject *pOleObject;
	sc = ::OleCreateStaticFromData(this, IID_IOleObject, OLERENDER_FORMAT, 
		&m_fromat, pOleClientSite, pStorage, (void **)&pOleObject);
	if (sc != S_OK)
	{
		//ASSERT(0);
		TCHAR szText[164];
		//_i64toa(sc, szText, 10);
		//strcat(szText, " == sc");
		wsprintf(szText, "sc == %x", sc);
		IDOWriteLog(szText);
		return NULL;
	}
	//AfxThrowOleException(sc);
	return pOleObject;
}
void CRichEditCtrl2::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szPath[MAX_PATH + 1];
	int nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPath, MAX_PATH);
	{
		int nBmpCount = 0;
		for (int i = 0; i < nCount; i++)
		{
			DragQueryFile(hDropInfo, i, szPath, MAX_PATH);
			TRACE(szPath);
			TRACE("\n");
#if 1
			//由程序处理
			if (m_pOwnerWnd == NULL)
				m_pOwnerWnd = GetParent();
			if (m_pOwnerWnd->SendMessage(WM_P_ACCEPTFILE, GetDlgCtrlID (), LPARAM(szPath)) == 1)
			{
				nBmpCount++;
			}
#endif	
			//一次只搞一副图
			//if (nBmpCount > 0)
			//	break;
		}
		if (nBmpCount > 0)
		{
			//通知父窗口所有图片丢完，发送
			if (m_pOwnerWnd == NULL)
				m_pOwnerWnd = GetParent();
			m_pOwnerWnd->SendMessage(WM_P_DROPBMPOVER, GetDlgCtrlID (), nBmpCount);
		}
	}
	CRichEditCtrl::OnDropFiles(hDropInfo);
}

void CRichEditCtrl2::EnableDragFiles(BOOL bEnable /* = TRUE */)
{
	DragAcceptFiles(bEnable);
}

void CRichEditCtrl2::EnableLenLimit(BOOL bEnable /* = TRUE */)
{
	if (bEnable)
		SetEventMask(ENM_CHANGE);
	else
		SetEventMask(~ENM_CHANGE);
}

void CRichEditCtrl2::SetMaxTextLen(int nLen, CString strMsg)
{
	m_strMaxLenMsg = strMsg;
	m_nMaxLen = nLen;
}

void CRichEditCtrl2::OnEnChange()
{
	/*
	if (GetContent().GetLength() >m_nMaxLen)
	{
	::MessageBox(GetParent()->m_hWnd, m_strMaxLenMsg, "", MB_OK | MB_ICONEXCLAMATION);
	}
	*/
}

void CRichEditCtrl2::OnKillFocus(CWnd* pNewWnd)
{
	CRichEditCtrl::OnKillFocus(pNewWnd);
	/*

	if (GetContent().GetLength() > m_nMaxLen)
	{
	::MessageBox(GetParent()->m_hWnd, m_strMaxLenMsg, "", MB_OK | MB_ICONEXCLAMATION);
	SetFocus();
	}
	*/
}
CString CRichEditCtrl2::GetContent()
{
	CString str;
	GetWindowText(str);

	if (str.GetLength() == 0)
		return str;

	if(m_PicMap.size() > 0)
	{
		str.Append(SEPARATOR_TEXT_PIC);
		for (m_PicIter = m_PicMap.begin(); m_PicIter != m_PicMap.end(); m_PicIter++)
		{
			str.Append(m_PicIter->second);
			str.Append(SEPARATOR_PATH_POS);
			str.Append(i2s(m_PicIter->first));
			str.Append(SEPARATOR_PIC_PIC);
		}
	}

	//如果内容超多指定长度，返回空
	if (str.GetLength() > m_nMaxLen)
	{
		CString  strTitle;
		strTitle.LoadString(theApp.m_hInstance, IDS_TIP);
		::MessageBox(GetParent()->m_hWnd, m_strMaxLenMsg, strTitle, MB_OK | MB_ICONEXCLAMATION);
		return "";
	}
	return str;
}
void CRichEditCtrl2::AddFormatText(CString strMsg, CString strDir,
								   RICHEDITFONT& REFont, CString strIconDir,
								   CString strIconExt, int* pnPicCount/* = NULL*/)
{
	int nMapStart = strMsg.Find(SEPARATOR_TEXT_PIC);
	if (pnPicCount != NULL)
	{
		*pnPicCount = 0;
	}
	if (nMapStart == -1)
	{
		//不含图片
#ifdef USE_HANDWRITING
		if (strMsg.Find(BEGIN_OF_HANDWRITING) == 0)
		{
			//画的
			AddHandWriting(strMsg);
		}
		else
#endif
			AddText(strMsg, REFont);
	}
	else
	{
		//含有图片
		CString strMap = strMsg.Right(strMsg.GetLength() - nMapStart - strlen(SEPARATOR_TEXT_PIC));
		strMsg = strMsg.Left(nMapStart);

		int nMsgStart = 0;
		while (strMap.GetLength() > 0)
		{
			int nPathEnd = strMap.Find(SEPARATOR_PATH_POS);
			if (nPathEnd  == -1)
				break;
			CString strPath = strMap.Left(nPathEnd);
			strMap = strMap.Right(strMap.GetLength() - nPathEnd - strlen(SEPARATOR_PATH_POS));
			int nPosEnd = strMap.Find(SEPARATOR_PIC_PIC);
			if (nPosEnd == -1)
				break;
			int nPos = atoi(strMap.Left(nPosEnd));
			strMap = strMap.Right(strMap.GetLength() - nPosEnd - strlen(SEPARATOR_PIC_PIC));

			nPos -= nMsgStart;
			CString strAdd = strMsg.Left(nPos);

			strMsg = strMsg.Right(strMsg.GetLength() - nPos - 1);
			nMsgStart += nPos + 1;

			AddText(strAdd, REFont);
			if (IconIndex(strPath) == -1)
			{
				if (strPath.Find("\\") > 0 || strPath.Find("/") > 0)
					InsertPic(strPath);//已经是绝对路径了不需要再添加目录
				else
					InsertPic(strDir + strPath);
			}
			else
			{
				InsertIcon(IconIndex(strPath), strIconDir, strIconExt);
			}
			if (pnPicCount != NULL)
			{
				(*pnPicCount)++;
			}
		}
		if (strMsg.GetLength() > 0)
		{
			//添加最后一张图片后面的文字
			AddText(strMsg, REFont);
		}
	}
	NewLine();

#if 0
	//清空需要考虑linkText。
	int nLine = GetLineCount();
	for (int i = 0; i <= nLine; i++)
	{
		TRACE("Line %d: Length:%d, lineIndex:%d\n", i, LineLength(i), LineIndex(i));
	}
	//ASSERT(0);
	while (nLine > 3 * 5)
	{
		int nLength = LineIndex(1);
		SetSel(0, nLength);
		ReplaceSel("");
		nLine = GetLineCount();
	}
#endif
}

void CRichEditCtrl2::AddHandWriting(CString& strPainting)
{
#ifdef USE_HANDWRITING
	int nWidth = 100;
	int nHeight = 100;
	int nPenWidth = 1;
	int nR, nG, nB;
	int iFind = -1;
	Point pt;
	CList<Point, Point>* ptlist;
	CList<CList<Point, Point>*, CList<Point, Point>*> LineList;
	iFind = strPainting.Find(BEGIN_OF_HANDWRITING);
	if (iFind == -1)
		return;

	//记录搜索开始位置
	int iStart = strlen(BEGIN_OF_HANDWRITING);

	//宽度和高度
	iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
	nWidth = atoi(strPainting.Mid(iStart, iFind - iStart));
	iStart =  iFind + strlen(SEPARATOR_COORDINATE);

	iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
	nHeight = atoi(strPainting.Mid(iStart, iFind - iStart));
	iStart =  iFind + strlen(SEPARATOR_COORDINATE);

	//颜色
	iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
	nR = atoi(strPainting.Mid(iStart, iFind - iStart));
	iStart =  iFind + strlen(SEPARATOR_COORDINATE);

	iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
	nG = atoi(strPainting.Mid(iStart, iFind - iStart));
	iStart =  iFind + strlen(SEPARATOR_COORDINATE);

	iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
	nB = atoi(strPainting.Mid(iStart, iFind - iStart));
	iStart = iFind + strlen(SEPARATOR_COORDINATE);

	iFind = strPainting.Find(SEPARATOR_LINE, iStart);
	if (iFind == -1)
	{
		//一条线都没有
		return;
	}

	nPenWidth = atoi(strPainting.Mid(iStart, iFind - iStart));
	iStart = iFind + strlen(SEPARATOR_LINE);

	int iLineEnd = strPainting.Find(SEPARATOR_LINE, iStart);

	while (iLineEnd > 0)
	{
		//找到一条线
		iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
		char ch = strPainting.GetAt(iStart);
		ptlist = new CList<Point, Point>;
		while (1)
		{
			if (iFind == -1 || iFind > iLineEnd)
				break;
			//找到一个点
			pt.X = atoi(strPainting.Mid(iStart, iFind - iStart));
			iStart = iFind + strlen(SEPARATOR_COORDINATE);

			iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
			if (iFind == -1 || iFind > iLineEnd)
			{
				TRACE("只有x坐标，没有y坐标\n");
				break;
			}
			pt.Y = atoi(strPainting.Mid(iStart, iFind - iStart));
			iStart = iFind + strlen(SEPARATOR_COORDINATE);
			ptlist->AddTail(pt);
			//寻找下个点
			iFind = strPainting.Find(SEPARATOR_COORDINATE, iStart);
		}
		LineList.AddTail(ptlist);
		//寻找下一条线
		iLineEnd += strlen(SEPARATOR_LINE);
		iLineEnd = strPainting.Find(SEPARATOR_LINE, iLineEnd);
		iStart += strlen(SEPARATOR_LINE);
	}
	Point prePt;
	BOOL bFirst = TRUE;
	Bitmap bmp(nWidth, nHeight) ;
	Graphics g(&bmp);
	Pen MyPen(Color(255, nR, nG, nB), nPenWidth);

	for (POSITION pos = LineList.GetHeadPosition(); pos != NULL;)
	{
		CList<Point, Point>* pList = LineList.GetNext(pos);
		{
			bFirst = TRUE;
			for (POSITION ps = pList->GetHeadPosition(); ps != NULL;)
			{
				pt = pList->GetNext(ps);
				//第一次画点不画线
				if (bFirst)
				{
					bFirst = FALSE;
				}
				else
				{
					g.DrawLine(&MyPen, prePt, pt);
				}
				prePt = pt;
			}
		}
	}
	Color color;
	color.SetFromCOLORREF(RGB(0, 0, 0));
	HBITMAP hBitmap = NULL;
	bmp.GetHBITMAP(color, &hBitmap);
	InsertPic(hBitmap);
	for (POSITION pos = LineList.GetHeadPosition(); pos != NULL;)
	{
		CList<Point,Point>* pList = LineList.GetNext(pos);
		delete pList;
	}
	LineList.RemoveAll();
#endif
}
void CRichEditCtrl2::AddMessage(CString strName, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt)
{
	AddText(strName, TEXT_NAME);
	AddText("  ");
	AddTime();
	//AddText(":", TEXT_TIME);
	NewLine();
	AddFormatText(strMsg, strDir, REFont, strIconDir, strIconExt);
}
void CRichEditCtrl2::AddMessage(CString strNameFrom, CString strNameTo, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt)
{
	CHARRANGE cr;
	if (m_bAutoDelete)
	{
		SetSel(-1, -1);
		GetSel(cr);
		m_MsgRangeMap[m_nMsgCount].cpMin = cr.cpMin;
	}

	AddText(strNameFrom, TEXT_NAME);
	if (strNameTo.GetLength() > 0)
	{
		AddText("-->");
		AddText(strNameTo, TEXT_NAME);
	}
	AddText("  ");
	AddTime();
	//AddText(":", TEXT_TIME);
	NewLine();
	int nPicCount = 0;
	AddFormatText(strMsg, strDir, REFont, strIconDir, strIconExt, &nPicCount);

	if (m_bAutoDelete)
	{	
		SetSel(-1, -1);
		GetSel(cr);
		m_MsgRangeMap[m_nMsgCount].cpMax = cr.cpMax;
		m_MsgPicCountList.AddHead(nPicCount);

		//检查，自动滚动
		m_nMsgCount++;
		int nTotal = MsgCountToDelete();
		while(nTotal-- > 0)
		{
			RemoveFirstMsg();
		}
	}
}
int  CRichEditCtrl2::MsgCountToDelete()
{
	int nCount = 0;
	int nTotalPic = 0;
	for (POSITION pos = m_MsgPicCountList.GetHeadPosition(); pos != NULL;)
	{
		nTotalPic += m_MsgPicCountList.GetNext(pos);
		nCount++;
		if (nTotalPic > m_nMaxPicCount)
		{
			break;
		}
	}
	if (nCount > 0 && nTotalPic > m_nMaxPicCount)
		nCount = m_MsgPicCountList.GetCount() - nCount + 1;
	else if (m_nMsgCount - 1 == m_nMaxMsgCount)
		return 1;
	else
		return 0;
	return nCount;
}
void CRichEditCtrl2::RemoveFirstMsg()
{
	CHARRANGE crDelete = m_MsgRangeMap[0];
	int nWidth = crDelete.cpMax - crDelete.cpMin;

	//删除文本
	SetSel(crDelete);
	ReplaceSel(_T(""));

	//检查链接，在本消息后面的链接减去删除文本宽度
	for ( m_LinkIter = m_LinkMap.begin(); m_LinkIter != m_LinkMap.end(); m_LinkIter++)
	{
		if ((m_LinkIter->second).cpMin >= crDelete.cpMin && (m_LinkIter->second).cpMax <= crDelete.cpMax)
		{
			m_log.Write("消息中含有链接，出错。");
			break;
		}
		if ((m_LinkIter->second).cpMin > crDelete.cpMax)
		{
			m_LinkIter->second.cpMin -= nWidth;
			m_LinkIter->second.cpMax -= nWidth;
		}
	}

	//其余消息前移， 都减去文本宽度

	m_nMsgCount--;

	for (int i = 0; i < m_nMsgCount; i++)
	{
		m_MsgRangeMap[i] = m_MsgRangeMap[i + 1];
		m_MsgRangeMap[i].cpMax -= nWidth;
		m_MsgRangeMap[i].cpMin -= nWidth;
	}
	//最后一个无效
	m_MsgRangeMap.erase(m_nMsgCount);
	if (m_MsgPicCountList.GetCount() > 0)
	{
		m_MsgPicCountList.RemoveTail();
	}
	//图片是记录的是length，而link记录的是pos。
}
void CRichEditCtrl2::AddMessage(CString strNameFrom, CString strNameTo, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt, __time64_t time)
{
	AddText(strNameFrom, TEXT_NAME);
	if (strNameTo.GetLength() > 0)
	{
		AddText("-->");
		AddText(strNameTo, TEXT_NAME);
	}
	AddText("  ");
	CString str;
	CTime t(time);
	str.Format("%d-%02d-%02d %02d:%02d:%02d", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(),t.GetMinute(),t.GetSecond());
	AddText(str, TEXT_TIME);
	NewLine();
	AddFormatText(strMsg, strDir, REFont, strIconDir, strIconExt);
}

void CRichEditCtrl2::AddMessage(CString strNameFrom, CString strNameTo, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt, CString strTime)
{
	AddText(strNameFrom, TEXT_NAME);
	if (strNameTo.GetLength() > 0)
	{
		AddText("-->");
		AddText(strNameTo, TEXT_NAME);
	}
	AddText("  ");
	AddText(strTime, TEXT_TIME);
	NewLine();
	AddFormatText(strMsg, strDir, REFont, strIconDir, strIconExt);
}
BOOL CRichEditCtrl2::ClearContent(BOOL bForce)
{
	if (!bForce)
	{
		//检查是否含有linkText
		if (!m_LinkMap.empty())
			return FALSE;
	}
	if (m_hWnd != NULL)
		SetWindowText("");
	m_PicMap.clear();
	m_PicPathMap.RemoveAll();
#if 0
	for (POSITION pos = m_bmpList.GetHeadPosition(); pos != NULL;)
	{
		HBITMAP hBitmap = m_bmpList.GetNext(pos);
		DeleteObject(hBitmap);
	}
	m_bmpList.RemoveAll();
#endif

#ifdef _RH_SHOW_GIF
	POSITION pos = m_GifMap.GetStartPosition();
	GIFINFO  gi;
	INT_PTR   ip;
	while (pos != NULL)
	{
		m_GifMap.GetNextAssoc( pos, ip, gi );
		delete gi.pBitmap;
		gi.pBitmap = NULL;
	}
	m_GifMap.RemoveAll();
#endif
	m_LinkMap.clear();

	m_MsgRangeMap.clear();
	m_nMsgCount = 0;
	return TRUE;
}

void CRichEditCtrl2::SetDefaultFont(RICHEDITFONT& REFont)
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_SIZE | CFM_FACE | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_STRIKEOUT | CFM_UNDERLINE;
	cf.crTextColor = REFont.color;
	cf.dwEffects = 0;
	if ( REFont.lfBold)
		cf.dwEffects |= CFE_BOLD;
	if (REFont.lfItalic)
		cf.dwEffects |= CFE_ITALIC;
	if (REFont.lfUnderline)
		cf.dwEffects |= CFE_UNDERLINE;
	if (REFont.lfStrikeOut)
		cf.dwEffects |= CFE_STRIKEOUT;
	strcpy(cf.szFaceName, REFont.lfFaceName);
	cf.yHeight =  -15 * REFont.lfHeight;
	SetDefaultCharFormat(cf);
	m_DefaultFont = REFont;
}

BOOL CRichEditCtrl2::ReplacePathName(CStringList& pStrList, DWORD dwID)
{
	CString strPath;
	CTime time = CTime::GetCurrentTime();
	for (m_PicIter = m_PicMap.begin(); m_PicIter != m_PicMap.end(); m_PicIter++)
	{
		//只有当是文件的时候才替换，图标保留索引
		if (IconIndex(m_PicIter->second) == -1)
		{
			pStrList.AddTail(m_PicIter->second);
			strPath.Format("-%d%02d%02d%02d%02d%02d%d-", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(),time.GetMinute(), time.GetSecond(),rand());
			CString strRelativePath = i2s(dwID) + strPath + GetFileExt(m_PicIter->second, TRUE);
			m_PicPathMap[m_PicIter->second] = strRelativePath;
			m_PicIter->second = strRelativePath;
		}
	}
	return TRUE;
}

CString CRichEditCtrl2::GetRelativePath(CString strPath)
{
	return m_PicPathMap[strPath];
}
void CRichEditCtrl2::LOGFONT2RICHEDITFONT(LOGFONT& LogFont, RICHEDITFONT& REFont,  COLORREF color/* = RGB(0,0,0)*/)
{
	strcpy(REFont.lfFaceName, LogFont.lfFaceName);
	REFont.lfItalic = LogFont.lfItalic;
	REFont.lfHeight = LogFont.lfHeight;
	REFont.lfBold = (LogFont.lfWeight == FW_BOLD);
	REFont.lfUnderline = LogFont.lfUnderline;
	REFont.lfStrikeOut = LogFont.lfStrikeOut;
	REFont.color = color;
}

COLORREF CRichEditCtrl2::RICHEDITFONT2LOGFONT(RICHEDITFONT& REFont, LOGFONT& LogFont)
{
	strcpy(LogFont.lfFaceName, REFont.lfFaceName);

	LogFont.lfHeight = REFont.lfHeight;
	LogFont.lfItalic = REFont.lfItalic;
	LogFont.lfStrikeOut = REFont.lfStrikeOut;
	LogFont.lfUnderline = REFont.lfUnderline;

	if (REFont.lfBold)
		LogFont.lfWeight = FW_BOLD;
	else
		LogFont.lfWeight = FW_DONTCARE;

	return REFont.color;
}

int CRichEditCtrl2::IconIndex(CString strPath)
{
	if (strPath.Find('.') >= 0)
		return -1;
	return atoi(strPath);
}

void CRichEditCtrl2::OnTimer(UINT_PTR nIDEvent)
{

#ifdef _RH_SHOW_GIF
	GIFINFO gi = m_GifMap[nIDEvent];
	if (0 == gi.nCount || 0 == gi.pBitmap)
		goto RET;
	CHARRANGE cr;

	if (gi.nCurrent == gi.nCount - 1)
		gi.nCurrent = 0;
	else
		gi.nCurrent++;
	gi.pBitmap->SelectActiveFrame(&(gi.pDimensionIDs), gi.nCurrent);

	HBITMAP hBitmap = NULL;
	gi.pBitmap->GetHBITMAP(m_PicBgColor, &hBitmap);
	if (hBitmap != NULL)
	{
		GetSel(cr);
		SetSel(gi.lStartPos, gi.lStartPos + 1);

#ifdef _BMPLIST
		if (hBitmap != m_DefaultBmp)
			m_bmpList.AddTail(hBitmap);
#endif
		CImageDataObject::InsertBitmap(m_pRichEditOle, hBitmap);
		SetSel(cr);
	}
	m_GifMap[nIDEvent] = gi;
RET:
	0 == 0;
#endif
	if (m_nColorTimer == nIDEvent)
	{
		if (m_bEnableOption && IsWindow(m_hWnd))
			SetBackgroundColor(FALSE, CRHFontConfig::GetInstance().m_bgColor);
	}
	CRichEditCtrl::OnTimer(nIDEvent);
}

void CRichEditCtrl2::SetDefaultBmp(HBITMAP hBitmap)
{
	ASSERT(hBitmap);
	m_DefaultBmp = hBitmap;
	//此图片由外面负责销毁,注意deleteobject.
}
int CRichEditCtrl2::GetNewLineCount(CString strText)
{
	int nCount = 0;
	for (int i = 0; i <= strText.GetLength() - 2; i++)
	{
		if (strText.GetAt(i) == 0x0d && strText.GetAt(i + 1) == 0x0a)
			nCount++;
	}
	return nCount;
}
void CRichEditCtrl2::AddLine(LPCTSTR lpszText, int iType)
{
	::SendMessage(m_hWnd, WM_P_RE_ADD_LINE, (WPARAM)lpszText, iType);
}
void CRichEditCtrl2::OnRicheditCopy()
{
	Copy();
}

void CRichEditCtrl2::OnUpdateRicheditCopy(CCmdUI *pCmdUI)
{
	CHARRANGE cr;
	GetSel(cr);
	if (cr.cpMax > cr.cpMin)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CRichEditCtrl2::OnRicheditPatste()
{
	Paste();
}
void CRichEditCtrl2::OnUpdateRicheditPatste(CCmdUI *pCmdUI)
{
	if (CanPaste())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CRichEditCtrl2::OnRicheditOption()
{
	HINSTANCE hIns = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	CDlgReSetting dlg(CRHFontConfig::GetInstance().m_bgColor, CRHFontConfig::GetInstance().m_rfTime, 
		CRHFontConfig::GetInstance().m_rfName, CRHFontConfig::GetInstance().m_rfNormal,
		CRHFontConfig::GetInstance().m_rfInfo, CRHFontConfig::GetInstance().m_rfError);
	if (dlg.DoModal() == IDCANCEL)
	{
		AfxSetResourceHandle(hIns);
		return;
	}
	dlg.GetSettings(CRHFontConfig::GetInstance().m_bgColor, CRHFontConfig::GetInstance().m_rfTime,
		CRHFontConfig::GetInstance().m_rfName, CRHFontConfig::GetInstance().m_rfNormal,
		CRHFontConfig::GetInstance().m_rfInfo, CRHFontConfig::GetInstance().m_rfError);
	SetBackgroundColor(FALSE, CRHFontConfig::GetInstance().m_bgColor);
	AfxSetResourceHandle(hIns);
}

void CRichEditCtrl2::OnUpdateRicheditOption(CCmdUI *pCmdUI)
{
	if (m_bEnableOption)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CRichEditCtrl2::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bNoContextMenu)
	{
		CMenu menu;
		HINSTANCE hIns = AfxGetResourceHandle();
		AfxSetResourceHandle(theApp.m_hInstance);
		if (menu.LoadMenu(IDR_MENU_RE_RIGHT))
		{
			CMenu* pSub = menu.GetSubMenu(0);  
			CPoint pt = point;
			ClientToScreen(&pt);
			if (pSub != NULL)
			{
				CHARRANGE cr;
				GetSel(cr);
				if (cr.cpMax <= cr.cpMin  || m_bWordPaste)
					pSub->EnableMenuItem(ID_RICHEDIT_COPY, MF_BYCOMMAND | MF_GRAYED);
				if (!CanPaste())
					pSub->EnableMenuItem(ID_RICHEDIT_PATSTE, MF_BYCOMMAND | MF_GRAYED);
				if (!m_bEnableOption)
				{
					//删了吧
					pSub->RemoveMenu(ID_SEPARATOR, MF_BYCOMMAND);
					pSub->RemoveMenu(ID_RICHEDIT_OPTION, MF_BYCOMMAND);
					//pSub->EnableMenuItem(ID_RICHEDIT_OPTION, MF_BYCOMMAND | MF_GRAYED);
				}
				UINT nRet = pSub->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, 
					pt.y, this);
				switch (nRet)
				{
				case 0:
					break;
				case ID_RICHEDIT_COPY:
					OnRicheditCopy();
					break;
				case ID_RICHEDIT_OPTION:
					OnRicheditOption();
					break;
				case ID_RICHEDIT_PATSTE:
					OnRicheditPatste();
					break;
				case ID_RICHEDIT_ALL:
					SetSel(0, -1);
					break;
				}
			}
		}
		AfxSetResourceHandle(hIns);
	}
	CRichEditCtrl::OnRButtonUp(nFlags, point);
}

void CRichEditCtrl2::PreSubclassWindow()
{
	if (m_bEnableOption && IsWindow(m_hWnd))
		SetBackgroundColor(FALSE, CRHFontConfig::GetInstance().m_bgColor);
	m_nColorTimer = SetTimer(m_nColorTimer, 3000, NULL);
	CRichEditCtrl::PreSubclassWindow();
}
void CRichEditCtrl2::EnableOption()
{
	m_bEnableOption = TRUE;
}


CString CRichEditCtrl2::i2s(int i)
{
	CString str;
	str.Format("%d", i);
	return str;
}

CString CRichEditCtrl2::GetFileExt(CString strPath, BOOL bDot)
{
	if (bDot)
		return strPath.Right(strPath.GetLength() - strPath.ReverseFind('.'));
	else
		return strPath.Right(strPath.GetLength() - strPath.ReverseFind('.') - 1);
}

// Function LoadAnImage: accepts a file name and returns a HBITMAP.
// On error, it returns 0.
HBITMAP CRichEditCtrl2::LoadAnImage(CString FileName)
{
	// Use IPicture stuff to use JPG / GIF files
	IPicture* p;
	IStream* s;
	//	IPersistStream* ps;
	HGLOBAL hG;
	void* pp;
	FILE* fp;

	// Read file in memory
	fp = fopen(FileName,"rb");
	if (!fp)
		return NULL;

	fseek(fp,0,SEEK_END);
	int fs = ftell(fp);
	fseek(fp,0,SEEK_SET);
	hG = GlobalAlloc(GPTR,fs);
	if (!hG)
	{
		fclose(fp);
		return NULL;
	}
	pp = (void*)hG;
	fread(pp,1,fs,fp);
	fclose(fp);

	// Create an IStream so IPicture can
	CreateStreamOnHGlobal(hG,false,&s);
	if (!s)
	{
		GlobalFree(hG);
		return NULL;
	}
	//m_log.Write("hG--%s--%X", FileName, hG);
	OleLoadPicture(s,0,false,IID_IPicture,(void**)&p);

	if (!p)
	{
		s->Release();
		GlobalFree(hG);
		return NULL;
	}
	s->Release();
	GlobalFree(hG);

	HBITMAP hB = 0;
	p->get_Handle((unsigned int*)&hB);

	//m_log.Write("hB--%s--%X", FileName, hB);
	// Copy the image. Necessary, because upon p's release,
	// the handle is destroyed.
	HBITMAP hBB = (HBITMAP)CopyImage(hB,IMAGE_BITMAP,0,0,
		LR_COPYRETURNORG);
	//m_log.Write("hBB--%s--%X", FileName, hBB);
	p->Release();
	return hBB;
}


/////////////////////////////////////////////////////////////////////////////

CRichEditCtrl2::IExRichEditOleCallback::IExRichEditOleCallback()
{
	pStorage = NULL;
	m_iNumStorages = 0;
	m_dwRef = 0;

	// set up OLE storage

	HRESULT hResult = ::StgCreateDocfile(NULL,
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE /*| STGM_DELETEONRELEASE */|STGM_CREATE ,
		0, &pStorage );

	if ( pStorage == NULL ||
		hResult != S_OK )
	{
		AfxThrowOleException( hResult );
	}
}

CRichEditCtrl2::IExRichEditOleCallback::~IExRichEditOleCallback()
{
}

HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::GetNewStorage(LPSTORAGE* lplpstg)
{
	m_iNumStorages++;
	WCHAR tName[50];
	swprintf(tName, L"REOLEStorage%d", m_iNumStorages);

	HRESULT hResult = pStorage->CreateStorage(tName, 
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ,
		0, 0, lplpstg );

	if (hResult != S_OK )
	{
		::AfxThrowOleException( hResult );
	}

	return hResult;
}

HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::QueryInterface(REFIID iid, void ** ppvObject)
{

	HRESULT hr = S_OK;
	*ppvObject = NULL;

	if ( iid == IID_IUnknown ||
		iid == IID_IRichEditOleCallback )
	{
		*ppvObject = this;
		AddRef();
		hr = NOERROR;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	return hr;
}



ULONG STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::AddRef()
{
	return ++m_dwRef;
}



ULONG STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::Release()
{
	if ( --m_dwRef == 0 )
	{
		delete this;
		return 0;
	}

	return m_dwRef;
}


HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
														  LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::DeleteObject(LPOLEOBJECT lpoleobj)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
														DWORD reco, BOOL fReally, HGLOBAL hMetaPict)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::ContextSensitiveHelp(BOOL fEnterMode)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::GetClipboardData(CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
CRichEditCtrl2::IExRichEditOleCallback::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
													   HMENU FAR *lphmenu)
{
	return S_OK;
}


CClipboardBackup::CClipboardBackup()
{
	//Backup();
}

CClipboardBackup::~CClipboardBackup()
{
	ClearBackupDatas();
}

BOOL CClipboardBackup::Backup()
{
	try
	{

		if( ! ::OpenClipboard(NULL) )
			return FALSE;

		ClearBackupDatas();

		UINT format = 0;
		while( (format = ::EnumClipboardFormats(format)) != 0 )
		{
			ClipboardData data;
			data.m_nFormat = format;

			if( format <= 14 )
				data.m_szFormatName[0] = 0;
			else if( GetClipboardFormatName(format, data.m_szFormatName, 100) == 0 )
				data.m_szFormatName[0] = 0;

			HANDLE hMem = ::GetClipboardData( format );
			if( hMem == NULL )
				continue;

			data.m_nLength = ::GlobalSize(hMem);

			LPVOID pMem = ::GlobalLock( hMem );
			data.m_pData = new byte[data.m_nLength];

			memcpy(data.m_pData, pMem, data.m_nLength);

			m_lstData.AddTail(data);
		}

		::CloseClipboard();
	}
	catch(...)
	{
		CCommonFun::WriteFileLog("Faild to backup.");
		return FALSE;
	}
	return TRUE;
}

BOOL CClipboardBackup::Restore()
{
	try
	{

		if( ! ::OpenClipboard(NULL) )
			return FALSE;

		::EmptyClipboard();

		POSITION pos = m_lstData.GetHeadPosition();
		while( pos != NULL )
		{
			ClipboardData & data = m_lstData.GetNext( pos );

			UINT format = data.m_nFormat;

			if( data.m_szFormatName[0] != 0 )
			{
				UINT u = RegisterClipboardFormat( data.m_szFormatName );
				if( u > 0 ) format = u;
			}

			HANDLE hMem = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, data.m_nLength );
			LPVOID pMem = ::GlobalLock( hMem );

			memcpy( pMem, data.m_pData, data.m_nLength );

			::GlobalUnlock( hMem );

			::SetClipboardData( format, hMem );
		}

		::CloseClipboard();
	}
	catch(...)
	{
		CCommonFun::WriteFileLog("Failed to restore.");
		return FALSE;
	}
	return TRUE;
}

VOID CClipboardBackup::ClearBackupDatas()
{
	POSITION pos = m_lstData.GetHeadPosition();
	while( pos != NULL )
	{
		ClipboardData & data = m_lstData.GetNext( pos );

		delete [] data.m_pData;
	}

	m_lstData.RemoveAll();
}
void CRichEditCtrl2::Paste()
{
	if (m_bWordPaste)
	{
		//word粘贴不设置格式
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PASTE, 0, 0);
		return;
	}
	CHARFORMAT2 cf, Oldcf;
	int line = 0;
	long nStartChar = 0;
	long nEndChar = 0;
	GetSel(nStartChar, nEndChar);

	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_PASTE, 0, 0);
	//ASSERT(0);
	long iLeft = nStartChar;
	GetSel(nStartChar, nEndChar);

	SetSel(iLeft, nStartChar);

	GetSelectionCharFormat(Oldcf);
	GetDefaultCharFormat(cf);
	SetSelectionCharFormat(cf);

	SetSel(nStartChar, nStartChar);

	SetSelectionCharFormat(Oldcf);
}
BOOL CRichEditCtrl2::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 0x56 /*vk_v*/)
		{
			if ( HIBYTE(GetKeyState(VK_CONTROL)) == 0xFF )
			{
				Paste();
				return TRUE;
			}
		}
	}
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}
void CRichEditCtrl2::DisableContextMenu()
{
	m_bNoContextMenu = TRUE;
}
void CRichEditCtrl2::EnableAutoDelete(BOOL bAutoDelete /* = TRUE */, int nMaxMsgCount /*= 30*/, int nMaxPicCount /*= 20*/)
{
	ASSERT(nMaxMsgCount >= 1);
	if (nMaxMsgCount < 1)
		nMaxMsgCount = 1;

	ASSERT(nMaxPicCount >= 1);
	if (nMaxPicCount < 1)
		nMaxPicCount = 1;
	m_nMaxPicCount = nMaxPicCount;
	m_nMaxMsgCount = nMaxMsgCount;
	m_bAutoDelete = bAutoDelete;
}


LRESULT CRichEditCtrl2::OnAddTextRF(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam);
	if (wParam == NULL)
		return 0;
	ASSERT(lParam);
	if (lParam == NULL)
		return 0;
	CString strText((LPCTSTR)wParam);
	RICHEDITFONT& REFont = *(RICHEDITFONT*)lParam;
#if 0
	CHARFORMAT2 cf, Oldcf;
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_SIZE | CFM_FACE | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_STRIKEOUT | CFM_UNDERLINE;
	cf.crTextColor = REFont.color;
	cf.dwEffects = 0;
	if ( REFont.lfBold)
		cf.dwEffects |= CFE_BOLD;
	if (REFont.lfItalic)
		cf.dwEffects |= CFE_ITALIC;
	if (REFont.lfUnderline)
		cf.dwEffects |= CFE_UNDERLINE;
	if (REFont.lfStrikeOut)
		cf.dwEffects |= CFE_STRIKEOUT;
	strcpy(cf.szFaceName, REFont.lfFaceName);
	cf.yHeight = -15 * REFont.lfHeight;

	int line = 0;
	long nStartChar = 0;
	long nEndChar = 0;
	GetSel(nStartChar, nEndChar);

	ASSERT(::IsWindow(m_hWnd));
	ReplaceSel(strText);
	long iLeft = nStartChar;
	GetSel(nStartChar, nEndChar);

	SetSel(iLeft, nStartChar);

	GetSelectionCharFormat(Oldcf);
	SetSelectionCharFormat(cf);
	SetSel(-1, -1);
	SetSelectionCharFormat(Oldcf);

#else
	CHARFORMAT2 cf, Oldcf;
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_SIZE | CFM_FACE | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_STRIKEOUT | CFM_UNDERLINE;
	cf.crTextColor = REFont.color;
	cf.dwEffects = 0;
	if ( REFont.lfBold)
		cf.dwEffects |= CFE_BOLD;
	if (REFont.lfItalic)
		cf.dwEffects |= CFE_ITALIC;
	if (REFont.lfUnderline)
		cf.dwEffects |= CFE_UNDERLINE;
	if (REFont.lfStrikeOut)
		cf.dwEffects |= CFE_STRIKEOUT;
	strcpy(cf.szFaceName, REFont.lfFaceName);
	cf.yHeight = -15 * REFont.lfHeight;

	//int line = GetLineCount();
	int line = 0;

	//Add text
	CString   strTotalText;
	GetWindowText(strTotalText); 
	//line = GetNewLineCount(strTotalText);
	long iTotalTextLength =  MultiByteToWideChar(CP_GB2312,   0,   strTotalText,   (int)strlen(strTotalText),   NULL,   0);   
	SetSel(iTotalTextLength, iTotalTextLength);
	ReplaceSel(strText);

	//Set charfomat
	long iStartPos = iTotalTextLength;
	//int LineAdded = GetLineCount() - line;


	//line = GetLineCount();
	line = GetNewLineCount(strTotalText);
	//iStartPos -= line - 1;     
	iStartPos -= line ; 
	GetWindowText(strTotalText);   
	long iEndPos =  MultiByteToWideChar(CP_GB2312,   0,   strTotalText,   (int)strlen(strTotalText),   NULL,   0);   

	line = GetNewLineCount(strTotalText);
	iEndPos -= line ;
	SetSel(iStartPos, iEndPos);

	GetSelectionCharFormat(Oldcf);
	SetSelectionCharFormat(cf);

	//滚动到最后一行
	PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	SetSel(-1, -1);
	SetSelectionCharFormat(Oldcf);
#endif
	return 1;
}

LRESULT CRichEditCtrl2::OnAddSysMsg(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam);
	if (wParam == NULL)
		return 0;
	CString strText((LPCTSTR)wParam);
	int iType = lParam;
	AddTime();
	AddText(strText, iType);
	NewLine();
	return 1;
}
LRESULT CRichEditCtrl2::OnAddLine(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam);
	if (wParam == NULL)
		return 0;
	CString strText((LPCTSTR)wParam);
	int iType = lParam;
	AddText(strText, iType);
	NewLine();
	return 1;
}