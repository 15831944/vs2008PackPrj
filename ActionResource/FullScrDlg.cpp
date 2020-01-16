// FullScrDlg.cpp : implementation file
//
#include "stdafx.h"
#include "ActionResource.h"
#include "FullScrDlg.h"
#include <atlimage.h>
extern CFileLog theLog;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFullScrDlg dialog

#define BK_COLOR RGB(0, 0, 0)
#define TEXT_COLOR RGB(255, 255, 255)
CFullScrDlg::CFullScrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFullScrDlg::IDD, pParent)
{
	m_hNotifyWnd = NULL;
	m_sText.LoadString(IDS_COMP_IS_LOCKED);
	theLog.Write("黑屏：%s",m_sText);
	m_crText = RGB(255,255,255);
	m_crBk = RGB(0, 0, 0);
	m_bTransBk = FALSE;
	m_hHideFBD = NULL;
}


void CFullScrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFullScrDlg)
	DDX_Control(pDX, IDC_STATIC_BK, m_wndBk);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_wndStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFullScrDlg, CDialog)
	//{{AFX_MSG_MAP(CFullScrDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ENDSESSION()
	ON_WM_QUERYENDSESSION()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFullScrDlg message handlers

BOOL CFullScrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ShowWindow(SW_SHOW);
	m_tip.Create(this, TTF_ALWAYSTIP);

	//隐藏FBD
	if(m_hHideFBD && ::IsWindow(m_hHideFBD) && ::IsWindowVisible(m_hHideFBD))
		::ShowWindow(m_hHideFBD, SW_HIDE);
	else
		m_hHideFBD = NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
}

int CFullScrDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

void CFullScrDlg::DrawBkPic(CDC* pdc, CRect& rc)
{
	CImage img;	//背景图片，如未指定或找不到，则使用默认的黑色背景
	if(img.Load(m_sPic) == NO_ERROR)
	{
		CBitmap bkbmp;
		bkbmp.Attach(img.Detach());
		BITMAP bi;
		bkbmp.GetBitmap(&bi);
		CDC bkdc;
		bkdc.CreateCompatibleDC(pdc);
		CBitmap* pbkold = bkdc.SelectObject(&bkbmp);
		int nmode = pdc->SetStretchBltMode(COLORONCOLOR);
		pdc->StretchBlt(0,0,rc.Width(), rc.Height(), &bkdc, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
		pdc->SetStretchBltMode(nmode);
		bkdc.SelectObject(pbkold);
	}
	else
		pdc->FillSolidRect(rc, BK_COLOR);
}

void CFullScrDlg::DrawText(CDC* pdc, CRect& rc)
{
	int nPt = 400;	//初始大小
	CFont font;
	BOOL b = font.CreatePointFont(nPt,"宋体");	//Winlogon桌面下，获取的这种字体不能正确显示中文
	if(!b)
	{
		theLog.Write("DrawText error");
		return;
	}

	CFont* pOldFont = pdc->SelectObject(&font);
	COLORREF oldColor = pdc->SetTextColor(m_crText);
	int nmode;
	COLORREF oldbk;
	if(m_bTransBk)
		nmode = pdc->SetBkMode(TRANSPARENT);
	else
		oldbk = pdc->SetBkColor(m_crBk);

	//正中显示的文字
	CSize size = pdc->GetTextExtent(m_sText);
	float fW = (rc.Width()*1.0)/size.cx,
		fH = (rc.Height()*1.0)/size.cy/2;
	nPt = nPt*(min(fW, fH)*2/3);
	int nr = m_sText.Replace("\n", "\n");
	rc.top = (rc.Height()-size.cy*(nr+1))/2;
	pdc->DrawText(m_sText, rc, DT_CENTER | DT_VCENTER);

	//需额外显示的文字
	if(!m_sExtraText.IsEmpty())
	{
		rc.top = rc.bottom-size.cy*1.5;
		pdc->DrawText(m_sExtraText, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);	//不允许多行
	}

	//清理
	if(m_bTransBk)
		pdc->SetBkMode(nmode);
	else
		pdc->SetBkColor(oldbk);

	pdc->SetTextColor(oldColor);
	pdc->SelectObject(pOldFont);
}

void CFullScrDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcWin;
	GetWindowRect(rcWin);

	//-- 背景图片
	DrawBkPic(&dc, rcWin);

	//-- 文字
	DrawText(&dc, rcWin);
}

HBRUSH CFullScrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CFullScrDlg::DestroyWindow()
{
	if(m_hHideFBD)
		ShowWindow(SW_SHOW);

	return CDialog::DestroyWindow();
}

CFullScrDlg::~CFullScrDlg()
{

}

void CFullScrDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_wndBk.GetSafeHwnd())
	{
		m_wndBk.MoveWindow(0,0,cx,cy);
		m_wndBk.Invalidate();
	}
}

void CFullScrDlg::OnEndSession(BOOL bEnding) 
{
	CDialog::OnEndSession(bEnding);
}

BOOL CFullScrDlg::OnQueryEndSession() 
{
	if (!CDialog::OnQueryEndSession())
		return FALSE;
	return TRUE;
}

void CFullScrDlg::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
//	CString str;
//	str.Format("ONNNNNNNNNNNNNN  0x%x\n", m_hNotifyWnd);
//	AfxMessageBox(str);
#pragma message(__LOC__"此消息接收不到？")
	TRACE("******* ONKEYDOWN *************!!!!!!!!\n");
	if(m_hNotifyWnd)
	{
		::PostMessage(m_hNotifyWnd, 1333, 0, 0);
	}
}

BOOL CFullScrDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CFullScrDlg::ResizeToFull()
{
	int nScreenX=GetSystemMetrics(SM_CXSCREEN);
	int nScreenY=GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL, 0, 0,nScreenX, nScreenY, SWP_FRAMECHANGED|SWP_NOZORDER);

	CWnd* pDesk = GetDesktopWindow();
	if(pDesk)
	{
		CWnd* pChild = pDesk->GetWindow(GW_CHILD);
		if(pChild)
		{
			BOOL b1 = SetWindowPos(pChild, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			BOOL b2 = pChild->SetWindowPos(this,0, 0,0, 0, SWP_NOSIZE|SWP_NOMOVE);

			SimMouseClick();	//再点击一次
		}
	}
}

int CFullScrDlg::GetCurZOrder()
{
	CWnd* pDesk = GetDesktopWindow();
	if(pDesk)
	{
		int i=1;
		CWnd* pChild = pDesk->GetWindow(GW_CHILD);
		while(pChild)
		{
			if(pChild->GetSafeHwnd() == GetSafeHwnd())
				return i;

			pChild = pChild->GetWindow(GW_HWNDNEXT);
			i++;
		}
	}

	return -1;
}

//模拟鼠标点击，激活当前窗口
void CFullScrDlg::SimMouseClick()
{
	if(!IsWindowVisible())
		return;

	CPoint pt;
	GetCursorPos(&pt);

	int nX = GetSystemMetrics(SM_CXSCREEN),
		nY = GetSystemMetrics(SM_CYSCREEN);
	
	//测试一组点，防止指定点的光标下是其它窗口
	CArray<CPoint> ary;
	ary.Add(CPoint(nX/2, nY/2));
	ary.Add(CPoint(10, 10));
	ary.Add(CPoint(nX-10, 10));
	ary.Add(CPoint(nX/2, 10));
	ary.Add(CPoint(10, nY/2));
	ary.Add(CPoint(nX-10, nY/2));

	for(int i=0; i<ary.GetCount(); i++)
	{
		HWND hwnd = ::WindowFromPoint(ary[i]);
		if(hwnd == m_hWnd)
		{
			SetCursorPos(ary[i].x, ary[i].y);
			INPUT input[2]={0};
			input[0].type = INPUT_MOUSE;
			input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			input[1].type = INPUT_MOUSE;
			input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(2, input, sizeof(INPUT));

			break;	//点击一次就可以了
		}
	}
}

LRESULT CFullScrDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialog::WindowProc(message, wParam, lParam);
}
