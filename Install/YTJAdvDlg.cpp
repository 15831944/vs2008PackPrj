// YTJAdvDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "install.h"
#include "YTJAdvDlg.h"

#define Enable_Config_Device_IP		0	//是否在此界面显示和配置设备IP,0-不显示和配置，1-显示和配置

// CYTJAdvDlg 对话框

IMPLEMENT_DYNAMIC(CYTJAdvDlg, CDialog)

CYTJAdvDlg::CYTJAdvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYTJAdvDlg::IDD, pParent)
	, m_sPrinterIP(_T(""))
	, m_sPid(_T(""))
	, m_sSNO(_T(""))
	, m_sPName(_T(""))
{

}

CYTJAdvDlg::~CYTJAdvDlg()
{
}

BOOL CYTJAdvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitPrinterAdvModeUI();
	Load();

	return TRUE;
}

void CYTJAdvDlg::InitPrinterAdvModeUI()
{
	m_cmbPrinterAdvMode.AddString("未配置");			//0
	m_cmbPrinterAdvMode.AddString("施乐龙井系列");		//1

	m_cmbPrinterAdvMode.SetCurSel(0);
}

void CYTJAdvDlg::Load()
{
#if Enable_Config_Device_IP
	m_sPrinterIP = g_oIniFileCurretnConfig.GetVal("PrtCtrlConfig","PrinterAddr","0.0.0.0");
#else
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
#endif
	m_sPid = g_oIniFileCurretnConfig.GetVal("PrtCtrlConfig","PrinterPid","TL200618");
	m_sSNO = g_oIniFileCurretnConfig.GetVal("PrtCtrlConfig","PrinterSNO","369199");
	m_sPName = g_oIniFileCurretnConfig.GetVal("PrtCtrlConfig","PrinterPName","S2520");

	int nPrinterAdvMode = g_oIniFileCurretnConfig.GetVal("PrtCtrlConfig","PrinterAdvMode",PAM_NONE);
	m_cmbPrinterAdvMode.SetCurSel(nPrinterAdvMode);

	UpdateData(FALSE);
}

void CYTJAdvDlg::Save()
{
	UpdateData(TRUE);

#if Enable_Config_Device_IP
	g_oIniFileCurretnConfig.SetVal("PrtCtrlConfig","PrinterAddr",m_sPrinterIP);
#else
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
#endif
	g_oIniFileCurretnConfig.SetVal("PrtCtrlConfig","PrinterPid",m_sPid);
	g_oIniFileCurretnConfig.SetVal("PrtCtrlConfig","PrinterSNO",m_sSNO);
	g_oIniFileCurretnConfig.SetVal("PrtCtrlConfig","PrinterPName",m_sPName);

	int nPrinterAdvMode = m_cmbPrinterAdvMode.GetCurSel();
	g_oIniFileCurretnConfig.SetVal("PrtCtrlConfig","PrinterAdvMode",nPrinterAdvMode);
}

void CYTJAdvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PRINTER_ADV_MODE, m_cmbPrinterAdvMode);
	DDX_Text(pDX, IDC_EDIT_IP, m_sPrinterIP);
	DDX_Text(pDX, IDC_EDIT_productID, m_sPid);
	DDX_Text(pDX, IDC_EDIT_serialNo, m_sSNO);
	DDX_Text(pDX, IDC_EDIT_productName, m_sPName);
}


BEGIN_MESSAGE_MAP(CYTJAdvDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CYTJAdvDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CYTJAdvDlg 消息处理程序

void CYTJAdvDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	Save();

	OnOK();
}
