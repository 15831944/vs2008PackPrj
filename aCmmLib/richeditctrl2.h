#pragma once
//======================^^===========================
// File: RichEditCtrl2.h
// Auth: OnIn
// Date: 2008/11/19 [Wednesday]
// Desc: 自定义RicheditCtrl
// 功能：
// 一. 超链接
//     函数：
//		AddText在最后添加文本
//		AddLinkText：在最后添加带有链接的文本，ID不能重复
//		RemoveLink：删除某个命令，不删除文本
//      SetAutoDisable: 点击后自动调用RemoveLink
//      SetNotifyWnd:设置链接消息通知的窗口
//使用：
//		必须首先调用SetLinkMode函数(OnInitialDialog)，并在对话框中处理WM_P_CLICK_LINK消息，
//		WPARAM:ctrl id, 判断是否是RicheditCtrl发出的消息;
//		LPARAM:command,即命令id
//      ！一行结束后调用newline，否则richedit中链接下面每行的同一位置都会成为链接
// 二.显示word文件
//     函数：
//     DisplayWord:将richedit中所有内容删除，并显示word文件内容。
// 三.图片
//     函数：
//     SetPicBgColor:设置图片的背景颜色，默认为窗口背景色
//     InsertPic:在最后插入一幅图片，参数为图片文件路径。
//四.文件拖放
//     函数：
//     EnableDragFiles
//	   图片会显示出来，其他会通知父窗口，消息：UM_ACCEPTFILE, WPARAM: ctrl id , LPARAM: file path

//===================================================

// CRichEditCtrl2
#include <map>
#include <Richole.h>
#include "msword.h"

#ifdef _RH_SHOW_GIF
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus")
#endif

#include "rf.h"
#include "FileLog.h"
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release(); (p)=NULL; } }
#define TEXT_NORMAL			0
#define TEXT_INFORMATION	1
#define TEXT_ERROR			2
#define TEXT_NAME			3
#define TEXT_TIME			4

#define SEPARATOR_TEXT_PIC	"[TXT_PIC]" //消息中正文和图片的间隔字符串
#define SEPARATOR_PIC_PIC	"[PIC_PIC]" //消息中图片和图片的间隔字符串
#define SEPARATOR_PATH_POS	"[PTH_POS]" //消息中图片中路径和位置的间隔字符串

#define  STRING_SEC_SETTING "RichEditSettings"

#define  WM_P_CLICK_LINK				WM_USER + 2302//listctrl2或CRichEditCtrl2单击一个链接
#define	 WM_P_ACCEPTFILE				WM_USER + 2304//richedit中丢了一个文件
#define  WM_P_DROPBMPOVER				WM_USER + 2305//RICHEDIT图片丢完

/*
void AddText(CString strText, int iType = TEXT_NORMAL);//添加文本
void AddText(CString strText, COLORREF &crNewColor);//添加文本
void AddText(CString strText, RICHEDITFONT&  REFont);//添加文本
void AddSysMessage(CString strText, int iType = TEXT_NORMAL);//添加一行文本 格式：时间：消息\n
*/

//#define	 WM_P_RE_ADDTEXT_TYPE				WM_USER + 2306//
#define	 WM_P_RE_ADD_LINE					WM_USER + 2307//
#define	 WM_P_RE_ADDTEXT_RF					WM_USER + 2308//
#define  WM_P_RE_ADD_SYSMSG					WM_USER + 2309//

class CImageDataObject;

#ifdef _RH_SHOW_GIF
//显示gif格式
class GIFINFO
{
public:
	GIFINFO(){nCount = 0; lStartPos = 0; nCurrent = 0;pBitmap = NULL;}
	long lStartPos;//图片在richedit中的起始位置，结束位置为起始位置加1
	int  nCount;//帧总数
	int  nCurrent;//当前帧
	static const GUID pDimensionIDs;
	Bitmap* pBitmap;
};
#endif

//剪贴板备份
class CClipboardBackup  
{
public:
	CClipboardBackup();
	virtual ~CClipboardBackup();

public:
	BOOL Backup ();
	BOOL Restore();

public:
	struct ClipboardData
	{
		UINT   m_nFormat;
		TCHAR  m_szFormatName[100];
		UINT   m_nLength;
		LPVOID m_pData;
	};

private:
	VOID ClearBackupDatas();
	CList <ClipboardData, ClipboardData&> m_lstData;
};
class CRHFontConfig
{
public:
	static CRHFontConfig& GetInstance();
protected:
	CRHFontConfig();
	~CRHFontConfig();
	void    LoadSettings();
	void    SaveSettings();
public:
	RICHEDITFONT		m_rfTime;
	RICHEDITFONT		m_rfName;
	RICHEDITFONT		m_rfInfo;
	RICHEDITFONT		m_rfNormal;	
	RICHEDITFONT		m_rfError;
	COLORREF			m_bgColor;
};
class _AA_DLL_EXPORT_ CRichEditCtrl2 : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichEditCtrl2)

public:
	CRichEditCtrl2();
	virtual ~CRichEditCtrl2();
protected:
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnChange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRicheditCopy();
	afx_msg void OnUpdateRicheditCopy(CCmdUI *pCmdUI);
	afx_msg void OnRicheditPatste();
	afx_msg void OnUpdateRicheditPatste(CCmdUI *pCmdUI);
	afx_msg void OnRicheditOption();
	afx_msg void OnUpdateRicheditOption(CCmdUI *pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnAddLine(WPARAM, LPARAM);
	//afx_msg LRESULT OnAddTextColor(WPARAM, LPARAM);
	afx_msg LRESULT OnAddTextRF(WPARAM, LPARAM);
	afx_msg LRESULT OnAddSysMsg(WPARAM, LPARAM);
public:
	static void LOGFONT2RICHEDITFONT(LOGFONT& LogFont, RICHEDITFONT& REFont, COLORREF color = RGB(0,0,0));
	static COLORREF RICHEDITFONT2LOGFONT(RICHEDITFONT& REFont, LOGFONT& LogFont);

	//返回图标编号，即aoti，如果是文件路径返回-1 
	static int IconIndex(CString strPath);
public:
	//链接
	void SetNotifyWnd(CWnd* pWnd);
	void SetLinkMode(BOOL bLink = TRUE);
	void SetAutoDisable(BOOL bAutoDisable = TRUE);
	BOOL GetAutoDisable()const;
	void AddLinkText(CString strText, UINT nID);
	BOOL RemoveLink(UINT nID,BOOL bDel = FALSE);
	BOOL RemoveAllLinks();
	//字体
	void Paste();
	void SetDefaultFont(RICHEDITFONT& REFont);
	void AddText(CString strText, int iType = TEXT_NORMAL);//添加文本
	void AddText(CString strText, COLORREF &crNewColor);//添加文本
	void AddText(LPCTSTR lpSTR, RICHEDITFONT&  REFont);//添加文本
	void AddSysMessage(LPCTSTR lpSTR, int iType = TEXT_NORMAL);//添加一行文本 格式：时间：消息\n
	void AddMessage(CString strName, CString strMsg);//添加类似消息的文本（姓名、时间：\n消息）

	void EnableAutoDelete(BOOL bAutoDelete = TRUE, int nMaxMsgCount = 30, int nMaxPicCount = 20);
	//添加类似消息的文本（姓名、时间：\n消息）,其中包含图片，strdir为图片所在文件夹
	//格式：msg + SEPARATOR_TEXT_PIC + picpath + SEPARATOR_PIC_PIC + picPos + SEPARATOR_PIC_PIC + picpath + ...
	//带字体
	void AddMessage(CString strName, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt);
	
	//
	void AddMessage(CString strNameFrom, CString strNameTo, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt);
	
	void AddMessage(CString strNameFrom, CString strNameTo, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt, __time64_t t);
	void AddMessage(CString strNameFrom, CString strNameTo, CString strMsg, CString strDir, RICHEDITFONT& REFont, CString strIconDir, CString strIconExt, CString strTime);

	void AddLine(LPCTSTR lpszText, int iType = TEXT_NORMAL);//添加一行文本[2009-12-38 0:0:59] abc\n
	void NewLine();//换行
	void AddTime();
	void AddSpace();

	//nocontextmenu
	void DisableContextMenu();

	//显示word文件
	BOOL DisplayWord(CString strWordFilePath);

	//Insert a picture:
	void SetPicBgColor();
	BOOL InsertPic(CString strPic);
	BOOL InsertPic(HBITMAP hBitmap);
	BOOL InsertIcon(int index, CString strDir, CString strExt);
	void EnableDragFiles(BOOL bEnable = TRUE);
	void SetDefaultBmp(HBITMAP hBitmap);
	//将图片的绝对路径替换为相对路径，在本地显示GetContent之后调用，在发送GetContent之前调用
	//返回绝对路径链表
	BOOL ReplacePathName(CStringList& pStrList ,DWORD dwID);
	CString GetRelativePath(CString strPath);
	CString  GetContent();

	void     EnableLenLimit(BOOL bEnable = TRUE);
	void     SetMaxTextLen(int nLen, CString strErrMsg);
	BOOL     ClearContent(BOOL bForce = TRUE);
	void     EnableOption();
private:
	long	TryToShowGif(CString strGif);//返回开始位置,插入Gif图片

	//插入消息内容，不包括姓名及时间
	void	AddFormatText(CString strMsg, CString strDir,
		RICHEDITFONT& REFont, CString strIconDir,
		CString strIconExt, int* pnPicCount = NULL);
	void    AddHandWriting(CString& strPainting);

	//得到strtext中主动换行的个数，即0d0a
	int		GetNewLineCount(CString strText);

	CString i2s(int i);
	CString GetFileExt(CString strPath, BOOL bDot);
	HBITMAP LoadAnImage(CString FileName);//文件->HBITMAP
	BOOL	InsertPicHelper(CString strPic, int nIndex = -1);
	void	RemoveFirstMsg();//删除第一条消息
	int		MsgCountToDelete();//计算需要删除多少条消息
private:
	std::map<int, CHARRANGE> m_LinkMap;
	std::map<int, CHARRANGE>::iterator m_LinkIter;

	std::map<int, CString> m_PicMap;//图片左边字符串的长度和路径
	std::map<int, CString>::iterator m_PicIter;
	
	BOOL  m_bAutoDisable;
	CWnd*  m_pOwnerWnd;//链接消息通知的窗口，默认为父窗口

#ifdef _RH_SHOW_GIF
	//显示gif文件需要的变量
	CMap<int, int, GIFINFO, GIFINFO>	m_GifMap; //gif 图片映射，timer
	INT_PTR						m_nGifTimer;
#endif

	CMapStringToString     m_PicPathMap;//绝对路径和相对路径
	
	//CList<HBITMAP, HBITMAP> m_bmpList;//记录图片，最后要销毁，否则gdi泄漏
	HBITMAP				m_DefaultBmp;//图片找不到时显示的图片

	IRichEditOle* 		m_pRichEditOle;
	//Color				m_PicBgColor;

	int					m_nMaxLen;
	CString				m_strMaxLenMsg;

	RICHEDITFONT		m_DefaultFont;//显示的姓名，时间，文本的默认字体
	CHARFORMAT2			m_DefaultLinkFormat;//链接文本的字体
	BOOL				m_bEnableOption;
	BOOL				m_bNoContextMenu;
	UINT				m_nColorTimer;//检查背景颜色是不是变了，字体插入时可以检查

	CClipboardBackup	m_ClipBrdBackUp;
	CFileLog			m_log;

	BOOL				m_bAutoDelete;//自动删除信息
	int					m_nMaxMsgCount;//显示最大消息条数
	int					m_nMaxPicCount;//显示最大图片个数, 超出后即使消息条数少也删除
	std::map<int, CHARRANGE> m_MsgRangeMap;//每条消息的开始和结束位置
	//std::map<int, int>		 m_MsgPicCountMap;
	CList<int, int>			m_MsgPicCountList;//每条消息含有的图片数,从下往上记
	int						m_nMsgCount;
	BOOL					m_bWordPaste;//是否是word粘贴，是的话不需要设置格式
protected:
	virtual void PreSubclassWindow();
	//Getting Images with a StreamIn/ClipBoard/Drag'n'Drop operation
	interface IExRichEditOleCallback : public IRichEditOleCallback
	{
	public:
		IExRichEditOleCallback();
		virtual ~IExRichEditOleCallback();
		int m_iNumStorages;
		IStorage* pStorage;
		DWORD m_dwRef;

		virtual HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE* lplpstg);
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
			LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
		virtual HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL fShow);
		virtual HRESULT STDMETHODCALLTYPE QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
		virtual HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj);
		virtual HRESULT STDMETHODCALLTYPE QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
			DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
		virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
		virtual HRESULT STDMETHODCALLTYPE GetClipboardData(CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj);
		virtual HRESULT STDMETHODCALLTYPE GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
			HMENU FAR *lphmenu);
	};
	IExRichEditOleCallback* m_pIRichEditOleCallback;
	BOOL m_bCallbackSet;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};