// DlgReSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "acmmlib.h"
#include "DlgReSetting.h"


// CDlgReSetting 对话框

IMPLEMENT_DYNAMIC(CDlgReSetting, CDialog)

CDlgReSetting::CDlgReSetting(COLORREF bgcr
							 , RICHEDITFONT rfTime, RICHEDITFONT rfName,
							 							 RICHEDITFONT rfNormal, RICHEDITFONT rfInfo, RICHEDITFONT rfError,
							 
							 CWnd* pParent /*=NULL*/)
							 : CDialog(CDlgReSetting::IDD, pParent)
{
// 	m_bgCr = bgcr;
// 	m_rfTime = rfTime;
// 	m_rfName = rfName;
// 	m_rfNormal = rfNormal;
// 	m_rfInfo = rfInfo;
// 	m_rfError = rfError;
	m_btBgColor.SetMode(bgcr);
	m_btTimeFont.SetMode(rfTime, "19:29:59");
	CString str;
	str.LoadString(theApp.m_hInstance, IDS_NAME);
	m_btNameFont.SetMode(rfName, str);
	str.LoadString(theApp.m_hInstance, IDS_EXP);
	m_btInfoFont.SetMode(rfInfo, str);
	m_btErrorFont.SetMode(rfError, str);
	m_btNormalFont.SetMode(rfNormal, str);
}

CDlgReSetting::~CDlgReSetting()
{
}

void CDlgReSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btBgColor);
	DDX_Control(pDX, IDC_BUTTON2, m_btTimeFont);
	DDX_Control(pDX, IDC_BUTTON3, m_btNameFont);
	DDX_Control(pDX, IDC_BUTTON4, m_btNormalFont);
	DDX_Control(pDX, IDC_BUTTON5, m_btInfoFont);
	DDX_Control(pDX, IDC_BUTTON6, m_btErrorFont);
}


BEGIN_MESSAGE_MAP(CDlgReSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgReSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgReSetting 消息处理程序

void CDlgReSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgReSetting::GetSettings(COLORREF& bgcr, RICHEDITFONT& rfTime, RICHEDITFONT& rfName, RICHEDITFONT& rfNormal, RICHEDITFONT& rfInfo, RICHEDITFONT& rfError)
{
	bgcr = m_btBgColor.GetColor();
	rfTime = m_btTimeFont.GetRFont();
	rfName = m_btNameFont.GetRFont();
	rfNormal = m_btNormalFont.GetRFont();
	rfInfo = m_btInfoFont.GetRFont();
	rfError = m_btErrorFont.GetRFont();
}