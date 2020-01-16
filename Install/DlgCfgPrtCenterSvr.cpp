// DlgCfgPrtCenterSvr.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "DlgCfgPrtCenterSvr.h"


// CDlgCfgPrtCenterSvr 对话框

IMPLEMENT_DYNAMIC(CDlgCfgPrtCenterSvr, CDialog)

CDlgCfgPrtCenterSvr::CDlgCfgPrtCenterSvr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgPrtCenterSvr::IDD, pParent)
{

}

CDlgCfgPrtCenterSvr::~CDlgCfgPrtCenterSvr()
{
}

void CDlgCfgPrtCenterSvr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IP, m_wndCmbIP);
}


BEGIN_MESSAGE_MAP(CDlgCfgPrtCenterSvr, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCfgPrtCenterSvr::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCfgPrtCenterSvr 消息处理程序

BOOL CDlgCfgPrtCenterSvr::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szSvrIP = "";
	CString szSvrPort = 0;
	theConfig.LoadPrtCenterSvrConfig(szSvrIP, szSvrPort, m_sBindIp);

	if (szSvrIP.IsEmpty())
	{
		szSvrIP = CCommonFun::GetLocalIP();
	}
	BYTE nField0=199,nField1=99,nField2=99,nField3=1;
	int Ipoint0,Ipoint1,Ipoint2;
	Ipoint0=szSvrIP.Find('.',0);
	nField0=atoi(szSvrIP.Mid(0,Ipoint0));
	Ipoint1=szSvrIP.Find('.',Ipoint0+1);
	nField1=atoi(szSvrIP.Mid(Ipoint0+1,Ipoint1));
	Ipoint2=szSvrIP.Find('.',Ipoint1+1);
	nField2=atoi(szSvrIP.Mid(Ipoint1+1,Ipoint2));
	nField3=atoi(szSvrIP.Mid(Ipoint2+1, szSvrIP.GetLength()));
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP))->SetAddress(nField0,nField1,nField2,nField3);

	CStringArray ary;
	CCommonFun::GetLocalIPList(ary);
	int nSel = 0;
	if (ary.GetCount() > 0)
	{
		for (int n = 0; n < ary.GetCount(); n++)
		{
			if(0 == m_sBindIp.CompareNoCase(ary.GetAt(n)))
			{
				nSel = n;
			}
			m_wndCmbIP.AddString(ary.GetAt(n));
		}
		m_wndCmbIP.SetCurSel(nSel);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCfgPrtCenterSvr::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString szServIP;
	BYTE nField0=199,nField1=99,nField2=99,nField3=1;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP))->GetAddress(nField0,nField1,nField2,nField3);
	szServIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
	if(szServIP.CompareNoCase("0.0.0.0") == 0 )
	{
		MessageBox("请输入正确的服务器地址！","提示",MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_IPADDRESS_IP)->SetFocus();
		return;
	}
	int nSel = m_wndCmbIP.GetCurSel();
	if (nSel >= 0)
	{
		m_wndCmbIP.GetLBText(nSel, m_sBindIp);
	}
	else
	{
		MessageBox("请绑定正确的本机地址！","提示",MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_COMBO_IP)->SetFocus();
		return;
	}

	CString szServPort = "";
	theConfig.SavePrtCenterSvrConfig(szServIP, szServPort, m_sBindIp);

	OnOK();
}
