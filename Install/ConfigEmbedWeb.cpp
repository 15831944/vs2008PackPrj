// ConfigEmbedWeb.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "ConfigEmbedWeb.h"


// ConfigEmbedWeb 对话框

IMPLEMENT_DYNAMIC(ConfigEmbedWeb, CDialog)

ConfigEmbedWeb::ConfigEmbedWeb(CWnd* pParent /*=NULL*/)
	: CDialog(ConfigEmbedWeb::IDD, pParent)
	, m_szTitle(_T(""))
	, m_szColor(_T(""))
	, m_szSize(_T("0px"))
	, m_nJump(0)
{

}

ConfigEmbedWeb::~ConfigEmbedWeb()
{
}

void ConfigEmbedWeb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TITLE, m_szTitle);
	DDX_Text(pDX, IDC_COLOR, m_szColor);
	DDX_Text(pDX, IDC_FONTSIZE, m_szSize);
	DDX_Text(pDX, IDC_SIZE9527, m_szSize);
	DDX_Text(pDX, IDC_JUMP, m_nJump);
	DDV_MinMaxInt(pDX, m_nJump, 1, 100000);
}


BEGIN_MESSAGE_MAP(ConfigEmbedWeb, CDialog)
	ON_BN_CLICKED(IDOK, &ConfigEmbedWeb::OnBnClickedOk)
END_MESSAGE_MAP()


// ConfigEmbedWeb 消息处理程序

void ConfigEmbedWeb::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//更新数据到变量
	theConfig.SaveEmbedWebCfg(m_szTitle, m_szColor, m_szSize);
	theConfig.SaveEmbedDBCfg(m_nPrtStationId, m_nJump);
	OnOK();
}

BOOL ConfigEmbedWeb::OnInitDialog()
{
    CDialog::OnInitDialog();
    theConfig.LoadEmbedWebCfg(m_szTitle, m_szColor, m_szSize);
    theConfig.LoadEmbedDBCfg(m_nPrtStationId, m_nJump);
    UpdateData(FALSE);//数据上传到界面
    return TRUE;
}