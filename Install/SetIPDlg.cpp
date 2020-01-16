// SetIPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "SetIPDlg.h"


// CSetIPDlg 对话框

IMPLEMENT_DYNAMIC(CSetIPDlg, CDialog)

CSetIPDlg::CSetIPDlg(PRODUCT_TYPE nShowType,CWnd* pParent /*=NULL*/)
	: CDialog(CSetIPDlg::IDD, pParent)
{
	m_sUpdataPath = "";
	m_nUpdataPort = 0;
	m_sIP = "";
	m_nShowType = nShowType;
}

CSetIPDlg::~CSetIPDlg()
{
}

void CSetIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetIPDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetIPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetIPDlg::OnBnClickedSetPath)
END_MESSAGE_MAP()


// CSetIPDlg 消息处理程序

void CSetIPDlg::OnBnClickedOk()
{
	if(!Save())
		return ;
	OnOK();
}

void CSetIPDlg::OnBnClickedSetPath()
{
	SetDlgItemText(IDC_EDIT2,CCommonFun::GetDirFromShell(FALSE,this->GetSafeHwnd()));
}

void CSetIPDlg::Load()
{
	CString szServIP = "";
	CStringArray ary;
	if (CCommonFun::GetLocalIPList(ary) > 0)
		szServIP = ary.GetAt(0);
	else
		szServIP = CCommonFun::GetLocalIP();
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
}

BOOL CSetIPDlg::Save()
{
	CString szServIP;
	BYTE nField0=199,nField1=99,nField2=99,nField3=1;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP))->GetAddress(nField0,nField1,nField2,nField3);
	szServIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
	if(szServIP.CompareNoCase("0.0.0.0") == 0 && m_nShowType != TYPE_INTRANET_SERVER)
		return FALSE;
	m_nUpdataPort = GetDlgItemInt(IDC_EDIT1);
	if ((m_nUpdataPort >= 65535 || m_nUpdataPort <= 0) && m_nShowType != TYPE_PRINTER)
		return FALSE;
	GetDlgItemText(IDC_EDIT2,m_sUpdataPath);
	m_sUpdataPath.TrimLeft();
	m_sUpdataPath.TrimRight();
	if (m_sUpdataPath.Right(1).Compare("\\") == 0)
		m_sUpdataPath = m_sUpdataPath.Left(m_sUpdataPath.GetLength() - 1);
	if (m_sUpdataPath.IsEmpty() && m_nShowType != TYPE_PRINTER)
		return FALSE;
	m_sIP = szServIP;
	return TRUE;
}

BOOL CSetIPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	switch (m_nShowType)
	{
	case TYPE_INTRANET_SERVER:
//		SetWindowText("升级服务器配置");
		SetWindowText(LoadS(IDS_MSG_UpdateServSet));
		GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_1)->EnableWindow(FALSE);
		SetDlgItemText(IDC_EDIT2,"C:\\ServerUpdate");
		SetDlgItemInt(IDC_EDIT1,8080);
		break;

	case TYPE_INTRANET_MANAGER:
//		SetWindowText("管理端升级配置");
		SetWindowText(LoadS(IDS_MSG_MgrUpdateSet));
		SetDlgItemText(IDC_EDIT2,"C:\\ManageUpdate");
		SetDlgItemInt(IDC_EDIT1,8080);
		break;

	case TYPE_INTRANET_CLIENT:
//		SetWindowText("客户端升级配置");
		SetWindowText(LoadS(IDS_MSG_ClntUpdateSet));
		SetDlgItemText(IDC_EDIT2,"C:\\ClientUpdate");
		SetDlgItemInt(IDC_EDIT1,8080);
		break;

	case TYPE_PRINTER:
		GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		break;
	}

	// 加载数据
	Load();
	return TRUE; 
}
