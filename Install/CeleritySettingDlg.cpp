// CeleritySettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "CeleritySettingDlg.h"


// CCeleritySettingDlg 对话框

IMPLEMENT_DYNAMIC(CCeleritySettingDlg, CDialog)

CCeleritySettingDlg::CCeleritySettingDlg(CString sServerIP,UINT uUpport,CWnd* pParent /*=NULL*/)
	: CDialog(CCeleritySettingDlg::IDD, pParent)
{
	m_sSerIP = sServerIP;
	m_sUpdatePath = "c:\\ClientUpdate";
	m_nUpdatePort = uUpport;
}

CCeleritySettingDlg::~CCeleritySettingDlg()
{
}

void CCeleritySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCeleritySettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCeleritySettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCeleritySettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CCeleritySettingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCeleritySettingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCeleritySettingDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCeleritySettingDlg 消息处理程序
BOOL CCeleritySettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	BYTE nField0=199,nField1=99,nField2=99,nField3=1;
	int Ipoint0,Ipoint1,Ipoint2;
	Ipoint0=m_sSerIP.Find('.',0);
	nField0=atoi(m_sSerIP.Mid(0,Ipoint0));
	Ipoint1=m_sSerIP.Find('.',Ipoint0+1);
	nField1=atoi(m_sSerIP.Mid(Ipoint0+1,Ipoint1));
	Ipoint2=m_sSerIP.Find('.',Ipoint1+1);
	nField2=atoi(m_sSerIP.Mid(Ipoint1+1,Ipoint2));
	nField3=atoi(m_sSerIP.Mid(Ipoint2+1,m_sSerIP.GetLength()));
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->SetAddress(nField0,nField1,nField2,nField3);
	SetDlgItemInt(IDC_EDIT4,m_nUpdatePort);
	SetDlgItemText(IDC_EDIT2,m_sUpdatePath);
	SetDlgItemText(IDC_EDIT3,CCommonFun::GetDesktopDir() + "\\client_install.exe");
	return TRUE;
}

void CCeleritySettingDlg::OnBnClickedOk()
{
	CString sSourcePath;
	CString sDestPath;
	CString sTempPath;
	CCommonFun::SHGetSpecialPath(CSIDL_COMMON_TEMPLATES,sTempPath);
	GetDlgItemText(IDC_EDIT1,sSourcePath);
	if (sSourcePath.IsEmpty())
	{
		MessageBox("请指定客户端安装包所在目录!","提示",MB_OK | MB_ICONINFORMATION);
		return;
	}

	GetDlgItemText(IDC_EDIT3,sDestPath);
	if (sDestPath.IsEmpty())
	{
		MessageBox("请指定一键安装包保存目录!","提示",MB_OK | MB_ICONINFORMATION);
		return;
	}

	CIniFile file;
	file.m_sPath = sTempPath + "\\config.ini";
	file.SetVal("ClientConfig","SvrIP",m_sSerIP);
	file.SetVal("ClientConfig","iSec",ISEC_CLT_INTRANET);
	file.SetVal("UpdaterClt","LocalAddr",m_sUpdatePath);
	file.SetVal("UpdaterClt","SvrAddr",m_sSerIP);
	file.SetVal("UpdaterClt","SvrPort",m_nUpdatePort);

	CStdioFile iofile;
	CString sBatSavePath = sTempPath + "\\bat.bat";
	if(iofile.Open(sBatSavePath, CFile::modeCreate|CFile::modeWrite)) 
	{
		CString sExeString;	
		sExeString.Format("\"%s7zr.exe\" a \"%s\\Setup.7z\" \"%s\" \"%s\"",CCommonFun::GetDefaultPath(),sTempPath,sSourcePath,file.m_sPath);
		iofile.WriteString(sExeString+"\n");
		sExeString.Format("copy /b \"%s7zS.sfx\" + \"%ssetup_config.txt\" + \"%s\\setup.7z\" \"%s\" /y",CCommonFun::GetDefaultPath(),CCommonFun::GetDefaultPath(),sTempPath,sDestPath);
		iofile.WriteString(sExeString+"\n");
		sExeString.Format("del \"%s\\Setup.7z\"",sTempPath);
		iofile.WriteString(sExeString+"\n");
		iofile.Close();
	}
	CCommonFun::CreateProcessAndWait(sBatSavePath.GetBuffer());
	MessageBox(_T("一键安装包制作完成！"),_T("提示"),MB_OK);
	OnOK();
}

void CCeleritySettingDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CCeleritySettingDlg::OnBnClickedButton1()
{
	CString sPath = CCommonFun::GetDirFromShell(FALSE,this->GetSafeHwnd());
	if (sPath.IsEmpty())
		return;
	if (sPath.Right(1) == "\\" || sPath.Right(1) == "/")
		sPath += "Setup-C.msi";
	else
		sPath += "\\Setup-C.msi";
	if (!PathFileExists(sPath))
	{
		MessageBox("请指定客户端安装包所在目录!","提示",MB_OK | MB_ICONINFORMATION);
		return;
	}
	SetDlgItemText(IDC_EDIT1,sPath);
}

void CCeleritySettingDlg::OnBnClickedButton2()
{
	CString sPath = CCommonFun::GetDirFromShell(FALSE,this->GetSafeHwnd());
	if (sPath.IsEmpty())
		return;
	if (sPath.Right(1) == "\\" || sPath.Right(1) == "/")
		sPath += "client_install.exe";
	else
		sPath += "\\client_install.exe";
	SetDlgItemText(IDC_EDIT3,sPath);
}

void CCeleritySettingDlg::OnBnClickedButton3()
{
	CString sPath = CCommonFun::GetDirFromShell(FALSE,this->GetSafeHwnd());
	if (sPath.IsEmpty())
		return;
	SetDlgItemText(IDC_EDIT2,sPath);
}
