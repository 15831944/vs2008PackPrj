// FontDialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "FontDialog2.h"
#include "acmmlib.h"

int CALLBACK EnumFontFamProc( const LOGFONT FAR* lpelf,  const TEXTMETRIC FAR* lpntm, DWORD FontType,  LPARAM lParam)
{
	CListBox* pListBox = (CListBox*)lParam;
	if (pListBox == NULL)
		return 0;
	pListBox->AddString(lpelf->lfFaceName);
	return 1;
}
// CFontDialog2 对话框

IMPLEMENT_DYNAMIC(CFontDialog2, CDialog)

CFontDialog2::CFontDialog2(CWnd* pParent /*=NULL*/)
: CDialog(CFontDialog2::IDD, pParent)
{

}
CFontDialog2::CFontDialog2(RICHEDITFONT& rf, CWnd* pParent /*=NULL*/)
: CDialog(CFontDialog2::IDD, pParent)
{
	m_Font = rf;
}

CFontDialog2::~CFontDialog2()
{
}

void CFontDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_btColor);
	DDX_Control(pDX, IDC_LIST_FONT, m_listFont);
	DDX_Control(pDX, IDC_EDIT_FONT, m_editFont);
	DDX_Control(pDX, IDC_CHECK_UNDERLINE, m_checkUnderline);
	DDX_Control(pDX, IDC_CHECK_STRICKOUT, m_checkStrikeout);
	DDX_Control(pDX, IDC_CHECK_ITALIC, m_checkItalic);
	DDX_Control(pDX, IDC_CHECK_BOLD, m_checkBold);
	DDX_Control(pDX, IDC_BUTTON_EXP, m_btExp);
	DDX_Control(pDX, IDC_LIST_SIZE, m_listSize);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_editSize);
}


BEGIN_MESSAGE_MAP(CFontDialog2, CDialog)
	ON_BN_CLICKED(IDOK, &CFontDialog2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFontDialog2::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_FONT, &CFontDialog2::OnLbnSelchangeListFont)
	ON_EN_CHANGE(IDC_EDIT_FONT, &CFontDialog2::OnEnChangeEditFont)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CFontDialog2::OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, &CFontDialog2::OnBnClickedCheckUnderline)
	ON_BN_CLICKED(IDC_CHECK_STRICKOUT, &CFontDialog2::OnBnClickedCheckStrickout)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, &CFontDialog2::OnBnClickedCheckItalic)
	ON_BN_CLICKED(IDC_CHECK_BOLD, &CFontDialog2::OnBnClickedCheckBold)
	ON_LBN_SELCHANGE(IDC_LIST_SIZE, &CFontDialog2::OnLbnSelchangeListSize)
	ON_EN_CHANGE(IDC_EDIT_SIZE, &CFontDialog2::OnEnChangeEditSize)
END_MESSAGE_MAP()


// CFontDialog2 消息处理程序

void CFontDialog2::OnBnClickedOk()
{
	CString strText;
	int nIndex = m_listFont.GetCurSel();
	if (nIndex != LB_ERR)
	{
		m_listFont.GetText(nIndex, strText);
		strcpy(m_Font.lfFaceName, strText);
	}

	m_Font.lfItalic = m_checkItalic.GetCheck();
	m_Font.lfStrikeOut = m_checkStrikeout.GetCheck();
	m_Font.lfBold = m_checkBold.GetCheck();
	m_Font.lfUnderline = m_checkUnderline.GetCheck();
	m_Font.color = m_btColor.GetColor();
	OnOK();
}

void CFontDialog2::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CFontDialog2::OnInitDialog()
{
	CDialog::OnInitDialog();

	//	磅      字号
#if 0
	m_FontSizeMap[42] = "初号";
	m_FontSizeMap[36] = "小初";
	m_FontSizeMap[26] = "一号";
	m_FontSizeMap[24] = "小一";
	m_FontSizeMap[22] = "二号";
	m_FontSizeMap[18] = "小二";
	m_FontSizeMap[16] = "三号";
	m_FontSizeMap[15] = "小三";
	m_FontSizeMap[14] = "四号";
	m_FontSizeMap[12] = "小四";
	m_FontSizeMap[10.5] = "五号"; 
	m_FontSizeMap[9] = "小五";
	m_FontSizeMap[7.5] = "六号";  
	m_FontSizeMap[6.5] = "小六";
	m_FontSizeMap[5.5] = "七号";
	m_FontSizeMap[5] = "八号";
#endif
#if 0
	//与height对应
	m_FontSizeMap[56] = "初号";
	m_FontSizeMap[48] = "小初";
	m_FontSizeMap[35] = "一号";
	m_FontSizeMap[32] = "小一";
	m_FontSizeMap[29] = "二号";
	m_FontSizeMap[24] = "小二";
	m_FontSizeMap[21] = "三号";
	m_FontSizeMap[20] = "小三";
	m_FontSizeMap[19] = "四号";
	m_FontSizeMap[16] = "小四";
	m_FontSizeMap[14] = "五号"; 
	m_FontSizeMap[12] = "小五";
	m_FontSizeMap[10] = "六号";  
	m_FontSizeMap[9] = "小六";
	m_FontSizeMap[7] = "七号";
	m_FontSizeMap[6] = "八号";
#endif
#if 1
// 	 char   *strFontSize[]   =     
// 	{"初号","小初","一号","小一","二号","小二","三号","小三",   
// 	"四号","小四","五号","小五","六号","小六","七号","八号","5",   
// 	"5.5","6.5","7.5","8","9","10","10.5","11","12","14","16","18",   
// 	"20","22","24","26","28","36","48","72"};   
	CStringArray fsary;
	fsary.Add(CFFS(IDS_FONT_CHUHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOCHU));
	fsary.Add(CFFS(IDS_FONT_YIHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOYI));
	fsary.Add(CFFS(IDS_FONT_ERHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOER));
	fsary.Add(CFFS(IDS_FONT_SANHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOSAN));
	fsary.Add(CFFS(IDS_FONT_SIHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOSI));
	fsary.Add(CFFS(IDS_FONT_WUHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOWU));
	fsary.Add(CFFS(IDS_FONT_LIUHAO));
	fsary.Add(CFFS(IDS_FONT_XIAOLIU));
	fsary.Add(CFFS(IDS_FONT_QIHAO));
	fsary.Add(CFFS(IDS_FONT_BAHAO));
	 char* strFontSize[]   =     
	{"5","5.5","6.5","7.5","8","9","10","10.5","11","12","14","16","18",   
	"20","22","24","26","28","36","48","72"};
	for(int f=0; f<(sizeof(strFontSize)/sizeof(strFontSize[0])); f++)
		fsary.Add(strFontSize[f]);

	 int   nFontSizes[]   =   
	{-56,         -48,       -35,         -32,       -29,         -24,     -21,         -20,   
	-19   ,         -16,       -14,         -12,       -10,         -9,       -7   ,       -6,     -6,   
	-7,       -9,   -10,   -11,   -12,-13,     -14,       -15,   -16,-19,     -21,   -24,   
	-27,   -29,   -32,   -35,   -37,   -48,   -64,   -96};   
#endif
	EnumFontFamilies (GetDC()->m_hDC, NULL,&EnumFontFamProc, (LPARAM)&m_listFont);

	m_btColor.SetMode(CClickedButton::MODE_COLOR);

	//初始化字体
	m_btColor.SetColor(m_Font.color);
	m_checkItalic.SetCheck(m_Font.lfItalic);
	m_checkStrikeout.SetCheck(m_Font.lfStrikeOut);
	m_checkUnderline.SetCheck(m_Font.lfUnderline);
	m_checkBold.SetCheck(m_Font.lfBold);
	m_editFont.SetWindowText(m_Font.lfFaceName);
	CString str;
	str.LoadString(theApp.m_hInstance, IDS_EXP);
	m_btExp.SetMode(m_Font, str);
	BOOL bFind = FALSE;
	for (int i = 0; i < sizeof(nFontSizes) / sizeof(int); i++)
	{
		//m_listSize.AddString(strFontSize[i]);
		m_listSize.AddString(fsary[i]);
		m_listSize.SetItemData(i, nFontSizes[i] * (-1));
		if ((nFontSizes[i] == m_Font.lfHeight) && !bFind)
		{
			m_listSize.SetCurSel(i);
			bFind = TRUE;
		}
	}
#if 0
	float fSize = 0;
	CString strSize;
	for (POSITION pos = m_FontSizeMap.GetStartPosition(); pos != NULL;)
	{
		m_FontSizeMap.GetNextAssoc(pos, fSize, strSize);
		int nIndex = m_listSize.AddString(strSize);
		m_listSize.SetItemData(nIndex, fSize);
	}
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFontDialog2::OnLbnSelchangeListFont()
{
	int nIndex = m_listFont.GetCurSel();
	if (nIndex == LB_ERR)
		return;
	CString strText;
	m_listFont.GetText(nIndex, strText);
	CString strTemp;
	m_editFont.GetWindowText(strTemp);
	if (strText != strTemp)
		m_editFont.SetWindowText(strText);
	strcpy(m_Font.lfFaceName, strText);
	m_btExp.SetRFont(m_Font);
}

void CFontDialog2::OnEnChangeEditFont()
{
	CString strText;
	m_editFont.GetWindowText(strText);
	int nIndex = m_listFont.FindString(-1, strText);
	if (nIndex == LB_ERR)
	{
		m_listFont.SetTopIndex(0);
		return;
	}
	m_listFont.SetCurSel(nIndex);
	//	m_listFont.SetTopIndex(nIndex);
}

void CFontDialog2::GetFont(RICHEDITFONT& rf)
{
	rf = m_Font;
}
void CFontDialog2::OnBnClickedButtonColor()
{
	m_Font.color = m_btColor.GetColor();
	m_btExp.SetRFont(m_Font);
}

void CFontDialog2::OnBnClickedCheckUnderline()
{
	m_Font.lfUnderline = m_checkUnderline.GetCheck();
	m_btExp.SetRFont(m_Font);
}

void CFontDialog2::OnBnClickedCheckStrickout()
{
	m_Font.lfStrikeOut = m_checkStrikeout.GetCheck();
	m_btExp.SetRFont(m_Font);
}

void CFontDialog2::OnBnClickedCheckItalic()
{
	m_Font.lfItalic = m_checkItalic.GetCheck();
	m_btExp.SetRFont(m_Font);
}

void CFontDialog2::OnBnClickedCheckBold()
{
	m_Font.lfBold = m_checkBold.GetCheck();
	m_btExp.SetRFont(m_Font);
}

void CFontDialog2::OnLbnSelchangeListSize()
{
	int nIndex = m_listSize.GetCurSel();
	if (nIndex == LB_ERR)
		return;
	m_Font.lfHeight = -1 * m_listSize.GetItemData(nIndex);
	m_btExp.SetRFont(m_Font);
	CString strText;
	CString strTemp;
	m_listSize.GetText(nIndex, strText);
	m_editSize.GetWindowText(strTemp);
	if (strText != strTemp)
		m_editSize.SetWindowText(strText);
}

void CFontDialog2::OnEnChangeEditSize()
{
	CString strText;
	m_editSize.GetWindowText(strText);
	int nIndex = m_listSize.FindString(-1, strText);
	if (nIndex == LB_ERR)
	{
		m_listSize.SetTopIndex(0);
		return;
	}
	m_listSize.SetCurSel(nIndex);
}
