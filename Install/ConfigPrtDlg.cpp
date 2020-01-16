// ConfigPrtDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "ConfigPrtDlg.h"


// CConfigPrtDlg 对话框

IMPLEMENT_DYNAMIC(CConfigPrtDlg, CDialog)

CConfigPrtDlg::CConfigPrtDlg(CWnd* pParent /*=NULL*/)
	//: CDialog(CConfigPrtDlg::IDD, pParent)
	: CExpandingDialog(CConfigPrtDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_BTN_ADV,
	_T("一体机设置>>"),_T("一体机设置<<"))
	, m_sScanDir(_T(""))
{
	m_nBrandType = 0;
}

CConfigPrtDlg::~CConfigPrtDlg()
{
}

void CConfigPrtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_wndCmb);
	DDX_Text(pDX, IDC_EDIT1, m_sScanDir);
	DDX_Control(pDX, IDC_COMBO_IP, m_wndCmbIP);
	DDX_Control(pDX, IDC_COMBO_BRAND, m_cmbBrand);
}


BEGIN_MESSAGE_MAP(CConfigPrtDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CConfigPrtDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CConfigPrtDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, &CConfigPrtDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfigPrtDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CConfigPrtDlg 消息处理程序


void CConfigPrtDlg::OnBnClickedRadio1()
{
	
}

void CConfigPrtDlg::OnBnClickedRadio2()
{
	if (!IsDlgButtonChecked(IDC_RADIO1))
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow();
	}
	else
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	}
}

void CConfigPrtDlg::Load()
{
	CString szServIP = "";
	CString sPort;
	theConfig.LoadPrtStationConfig(szServIP,sPort);
	if (szServIP.IsEmpty())
	{
		szServIP = CCommonFun::GetLocalIP();
	}
	BYTE nField0=199,nField1=99,nField2=99,nField3=1;
	int Ipoint0,Ipoint1,Ipoint2;
	Ipoint0=szServIP.Find('.',0);
	nField0=atoi(szServIP.Mid(0,Ipoint0));
	Ipoint1=szServIP.Find('.',Ipoint0+1);
	nField1=atoi(szServIP.Mid(Ipoint0+1,Ipoint1));
	Ipoint2=szServIP.Find('.',Ipoint1+1);
	nField2=atoi(szServIP.Mid(Ipoint1+1,Ipoint2));
	nField3=atoi(szServIP.Mid(Ipoint2+1,szServIP.GetLength()));
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP))->SetAddress(nField0,nField1,nField2,nField3);

	CString sPrintIP;
	theConfig.LoadPrtAddr(sPrintIP);
	{
		BYTE nField0=192,nField1=168,nField2=0,nField3=2;
		int Ipoint0,Ipoint1,Ipoint2;
		Ipoint0=sPrintIP.Find('.',0);
		nField0=atoi(sPrintIP.Mid(0,Ipoint0));
		Ipoint1=sPrintIP.Find('.',Ipoint0+1);
		nField1=atoi(sPrintIP.Mid(Ipoint0+1,Ipoint1));
		Ipoint2=sPrintIP.Find('.',Ipoint1+1);
		nField2=atoi(sPrintIP.Mid(Ipoint1+1,Ipoint2));
		nField3=atoi(sPrintIP.Mid(Ipoint2+1,sPrintIP.GetLength()));
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP2))->SetAddress(nField0,nField1,nField2,nField3);
	}
	theConfig.LoadPrtYTJ(m_bEnableYTJ);
	theConfig.LoadPrtComIndex(m_nComIndex);
	theConfig.LoadScanDir(m_sScanDir);
	theConfig.LoadBindIP(m_sBindIp);
	theConfig.LoadBrand(m_nBrandType);
	theConfig.LoadSignalCtrlSet(m_bUseSignalCtrl);
	theConfig.LoadIsEmbed(m_bEnableEmbedWeb);
	theConfig.LoadSNMPPrintMon(m_bEnableSnmpPrintMonitor);
	theConfig.LoadSNMPPrinterMeter(m_bEnableSnmpPrinterMeter);
	CString sData;
	theConfig.LoadScanData(sData);
	CStringArray ary;
	CCommonFun::StringSplit(sData,&ary,",");
	for (int n = 0; n < ary.GetCount(); n ++)
	{
		if(ary.GetAt(n).CompareNoCase(".jpg")==0)
		{
			CheckDlgButton(IDC_CHECK_JPG,1);
		}
		if(ary.GetAt(n).CompareNoCase(".tif")==0)
		{
			CheckDlgButton(IDC_CHECK_TIF,1);
		}
		if(ary.GetAt(n).CompareNoCase(".tiff")==0)
		{
			CheckDlgButton(IDC_CHECK_TIF,1);
		}
		if(ary.GetAt(n).CompareNoCase(".pdf")==0)
		{
			CheckDlgButton(IDC_CHECK_PDF,1);
		}
	}
	CString sAll;
	theConfig.LoadPrtPaperConfig(sAll);
	ary.RemoveAll();
	CCommonFun::StringSplit(sAll,&ary,",");
	for (int n = 0; n < ary.GetCount(); n ++)
	{
		CString s = ary.GetAt(n);
		if (s.GetLength() > 0 )
		{
			int pos = atoi(s);
			s.MakeLower();
			int a4 = 0;
			if(s.Find("a4") > 0)
			{
				a4 = 1;
			}
			switch(pos)
			{
			case 1:
				{
					CheckDlgButton(IDC_CHECK_FIR,1);
					if (a4)
					{
						CheckDlgButton(IDC_RADIO_A4,1);
						CheckDlgButton(IDC_RADIO_A3,0);
					}
					else
					{
						CheckDlgButton(IDC_RADIO_A4,0);
						CheckDlgButton(IDC_RADIO_A3,1);
					}
				}
				break;
			case 2:
				{
					CheckDlgButton(IDC_CHECK_SEC,1);
					if (a4)
					{
						CheckDlgButton(IDC_RADIO_2A4,1);
						CheckDlgButton(IDC_RADIO_2A3,0);
					}
					else
					{
						CheckDlgButton(IDC_RADIO_2A4,0);
						CheckDlgButton(IDC_RADIO_2A3,1);
					}
				}
				break;
			case 3:
				{
					CheckDlgButton(IDC_CHECK_THR,1);
					if (a4)
					{
						CheckDlgButton(IDC_RADIO_3A4,1);
						CheckDlgButton(IDC_RADIO_3A3,0);
					}
					else
					{
						CheckDlgButton(IDC_RADIO_3A4,0);
						CheckDlgButton(IDC_RADIO_3A3,1);
					}
				}
				break;
			case 4:
				{
					CheckDlgButton(IDC_CHECK_FOUR,1);
					if (a4)
					{
						CheckDlgButton(IDC_RADIO_4A4,1);
						CheckDlgButton(IDC_RADIO_4A3,0);
					}
					else
					{
						CheckDlgButton(IDC_RADIO_4A4,0);
						CheckDlgButton(IDC_RADIO_4A3,1);
					}
				}
				break;
			}
		}	
	}

    
    CheckDlgButton(IDC_SIGNAL_CTRL_USE, m_bUseSignalCtrl);
    CheckDlgButton(IDC_SIGNAL_CTRL_NOTUSE, !m_bUseSignalCtrl);

	CheckDlgButton(IDC_RADIO_Enable_Embed, m_bEnableEmbedWeb);
	CheckDlgButton(IDC_RADIO_Disable_Embed, !m_bEnableEmbedWeb);

	CheckDlgButton(IDC_RADIO_Enable_SNMP_Print_Mon, m_bEnableSnmpPrintMonitor);
	CheckDlgButton(IDC_RADIO_Disable_SNMP_Print_Mon, !m_bEnableSnmpPrintMonitor);

	CheckDlgButton(IDC_RADIO_Enable_Prnter_Meter, m_bEnableSnmpPrinterMeter);
	CheckDlgButton(IDC_RADIO_Disable_Printer_Meter, !m_bEnableSnmpPrinterMeter);

	//add by zfq,2013.01.04,begin
	int nComIdx = m_nComIndex;
	theLog.Write("##CConfigPrtDlg::Load,10.1,nComIdx=%d,m_wndCmb.GetCount=%d", nComIdx, m_wndCmb.GetCount());
	if(0 <= nComIdx && nComIdx < m_wndCmb.GetCount())
	{
		m_wndCmb.SetCurSel(nComIdx);
//		theLog.Write("##CConfigPrtDlg::Load,11,CurSel=%d", m_wndCmb.GetCurSel());
	}
	//add by zfq,2013.01.04,end

	theLog.Write("##CConfigPrtDlg::Load,9,m_nBrandType=%d,m_cmbBrand.GetCount=%d", m_nBrandType, m_cmbBrand.GetCount());
	if(0 <= m_nBrandType && m_nBrandType < m_cmbBrand.GetCount())
	{
		m_cmbBrand.SetCurSel(m_nBrandType);
	}
	else
	{
		m_cmbBrand.SetCurSel(3);	//默认利盟
	}
}

BOOL CConfigPrtDlg::Save()
{
	CString szServIP;
	BYTE nField0=199,nField1=99,nField2=99,nField3=1;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP))->GetAddress(nField0,nField1,nField2,nField3);
	szServIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
	if(szServIP.CompareNoCase("0.0.0.0") == 0 )
		return FALSE;
	m_sIP = szServIP;

//	if (!IsDlgButtonChecked(IDC_RADIO1))	//del by zfq,2016-02-23,企业打印，检测打印机状态
	if(1)		//add by zfq,2016-02-23,企业打印，检测打印机状态
	{
		CString szServIP;
		BYTE nField0=192,nField1=168,nField2=0,nField3=2;
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP2))->GetAddress(nField0,nField1,nField2,nField3);
		szServIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
		if(szServIP.CompareNoCase("0.0.0.0") == 0 )
		{
			MessageBox("请输入正确的打印机IP地址！","提示",MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
		m_sPrinterIP = szServIP;
	}
	
	GetDlgItem(IDC_EDIT1)->GetWindowText(m_sScanDir);

	m_nComIndex = m_wndCmb.GetCurSel();
	int nSel = m_wndCmbIP.GetCurSel();
	if (nSel >= 0)
	{
		m_wndCmbIP.GetLBText(nSel,m_sBindIp);
	}

	return TRUE;
}

void CConfigPrtDlg::OnBnClickedOk()
{
	UpdateData();
	if (!Save())
	{
		MessageBox("请输入正确的参数！","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if (m_sScanDir.IsEmpty() && !IsDlgButtonChecked(IDC_RADIO1))
	{
		MessageBox("请选择扫描文件夹！","提示",MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return ;
	}
	if (!IsDlgButtonChecked(IDC_RADIO1))
	{
		theConfig.SavePrtYTJ(TRUE);
	}
	else
	{
		theConfig.SavePrtYTJ(FALSE);
	}

	//if (!IsDlgButtonChecked(IDC_RADIO1))
	{
		theConfig.SetPrtAddr(m_sPrinterIP);
	}

	CString sData;
	if (IsDlgButtonChecked(IDC_CHECK_JPG))
	{
		sData = ".jpg,.jpeg";
	}
	if (IsDlgButtonChecked(IDC_CHECK_TIF))
	{
		sData += ",.tif,.tiff";
	}
	if (IsDlgButtonChecked(IDC_CHECK_PDF))
	{
		sData += ",.pdf";
	}
	theConfig.SaveScanData(sData);
	theConfig.SavePrtStationConfig(m_sIP);
	theConfig.SaveScanDir(m_sScanDir);
#if 0 //delete by zfq,2013.01.04
	theConfig.SavePrtComIndex(m_nComIndex);	
#else //add by zfq,2013.01.04
	int nCurSelOfCom = m_wndCmb.GetCurSel();
	theConfig.SavePrtComIndex(nCurSelOfCom);	
#endif
	theConfig.SavePrtBindIP(m_sBindIp);

	CString sPaper;
	CString sAll;
	if (IsDlgButtonChecked(IDC_CHECK_FIR))
	{
		if (IsDlgButtonChecked(IDC_RADIO_A4))
		{
			sPaper = "1|A4";
		}
		else
		{
			sPaper = "1|A3";
		}
		sAll += sPaper;
		sAll += ",";
	}
	
	if (IsDlgButtonChecked(IDC_CHECK_SEC))
	{
		if (IsDlgButtonChecked(IDC_RADIO_2A4))
		{
			sPaper = "2|A4";
		}
		else
		{
			sPaper = "2|A3";
		}
		sAll += sPaper;
		sAll += ",";
	}
	if (IsDlgButtonChecked(IDC_CHECK_THR))
	{
		if (IsDlgButtonChecked(IDC_RADIO_3A4))
		{
			sPaper = "3|A4";
		}
		else
		{
			sPaper = "3|A3";
		}
		sAll += sPaper;
		sAll += ",";
	}
	if (IsDlgButtonChecked(IDC_CHECK_FOUR))
	{
		if (IsDlgButtonChecked(IDC_RADIO_4A4))
		{
			sPaper = "4|A4";
		}
		else
		{
			sPaper = "4|A3";
		}
		sAll += sPaper;
		sAll += ",";
	}
	theConfig.SetPrtPaperConfig(sAll);

	m_nBrandType = m_cmbBrand.GetCurSel();
	theConfig.SaveBrand(m_nBrandType);
	
	if(IsDlgButtonChecked(IDC_SIGNAL_CTRL_USE))
	{
	    theConfig.SaveSignalCtrlSet(TRUE);
	}
    if(IsDlgButtonChecked(IDC_SIGNAL_CTRL_NOTUSE))
    {
        theConfig.SaveSignalCtrlSet(FALSE);
    }

	if(IsDlgButtonChecked(IDC_RADIO_Enable_SNMP_Print_Mon))
	{
		theConfig.SaveSNMPPrintMon(TRUE);
	}

	if(IsDlgButtonChecked(IDC_RADIO_Disable_SNMP_Print_Mon))
	{
		theConfig.SaveSNMPPrintMon(FALSE);
	}

	if (IsDlgButtonChecked(IDC_RADIO_Enable_Embed))
	{
		theConfig.SaveIsEmbed(TRUE);
	}

	if (IsDlgButtonChecked(IDC_RADIO_Disable_Embed))
	{
		theConfig.SaveIsEmbed(FALSE);
	}

	if (IsDlgButtonChecked(IDC_RADIO_Enable_Prnter_Meter))
	{
		theConfig.SaveSNMPPrinterMeter(TRUE);
	}

	if (IsDlgButtonChecked(IDC_RADIO_Disable_Printer_Meter))
	{
		theConfig.SaveSNMPPrinterMeter(FALSE);
	}

	OnOK();
}

BOOL CConfigPrtDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_RADIO_A4,1);
	CheckDlgButton(IDC_RADIO_2A4,1);
	CheckDlgButton(IDC_RADIO_3A4,1);
	CheckDlgButton(IDC_RADIO_4A4,1);
		
	InitComUI();
	InitBrandUI();
	Load();
	if (!m_bEnableYTJ)
	{
		CheckDlgButton(IDC_RADIO1,BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_RADIO2,BST_CHECKED);
	}

	CStringArray ary;
	ary.RemoveAll();
	CCommonFun::GetLocalIPList(ary);
	int nSel = 0;
	if (ary.GetCount() > 0)
	{
		for (int n = 0; n < ary.GetCount(); n++)
		{
			if(m_sBindIp.CompareNoCase(ary.GetAt(n)) ==0)
			{
				nSel = n;
			}
			m_wndCmbIP.AddString(ary.GetAt(n));
		}
		m_wndCmbIP.SetCurSel(nSel);
	}
	Expand(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigPrtDlg::InitComUI()
{
	//直接添加1-20个com串口选择,不要从系统中读取
	//添加一个0的com口默认是不启用复印控制器。
	CString szCom = "";
	for (int n = 0; n <= 20; n++)
	{
		szCom.Format("COM%d", n);
		m_wndCmb.AddString(szCom);
	}
	m_wndCmb.SetCurSel(0);

}

void CConfigPrtDlg::InitBrandUI()
{
	m_cmbBrand.AddString("富士施乐");	//0
	m_cmbBrand.AddString("东芝");		//1
	m_cmbBrand.AddString("柯尼卡美能达");		//2
	m_cmbBrand.AddString("利盟");		//3
	m_cmbBrand.AddString("未知品牌");	//4
	m_cmbBrand.AddString("理光");		//5
	m_cmbBrand.AddString("富士施乐(新版)");		//6
	m_cmbBrand.AddString("京瓷");		//7
	m_cmbBrand.AddString("震旦");		//8
	m_cmbBrand.AddString("夏普");		//9
	m_cmbBrand.AddString("三星");		//10
	m_cmbBrand.AddString("惠普");		//11

	m_cmbBrand.SetCurSel(0);
}

void CConfigPrtDlg::OnBnClickedButton2()
{

	CoInitialize(NULL);
	BROWSEINFO bi; 
	ITEMIDLIST * pidl; 
	char szDir[MAX_PATH] = {0};
	bi.hwndOwner=0; 
	bi.pidlRoot=NULL; 
	bi.pszDisplayName=szDir; 
	bi.lpszTitle= "选择文件夹"; 
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	
	bi.ulFlags |= BIF_NEWDIALOGSTYLE; 
	bi.lpfn=NULL; 
	bi.lParam=0; 
	bi.iImage=0; 

	pidl=SHBrowseForFolder(&bi);
	if(pidl==NULL)
		szDir[0] = 0;
	else
	{
		if(!SHGetPathFromIDList(pidl,szDir))
			szDir[0] = 0; 
	}
	// AfxMessageBox(szDir);

	m_sScanDir = szDir;
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_sScanDir);
	CoUninitialize();
}
