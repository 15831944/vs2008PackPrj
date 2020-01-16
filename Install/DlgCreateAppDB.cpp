// DlgCreateAppDB.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "DlgCreateAppDB.h"


// CDlgCreateAppDB 对话框

IMPLEMENT_DYNAMIC(CDlgCreateAppDB, CDialog)

CDlgCreateAppDB::CDlgCreateAppDB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreateAppDB::IDD, pParent)
	, m_sDbAddr(_T("Localhost"))
	, m_sDbUser(_T(""))
	, m_sDbPwd(_T(""))
	, m_sDbPort(_T(""))
{

}

CDlgCreateAppDB::~CDlgCreateAppDB()
{
}

void CDlgCreateAppDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DBTYPE2, m_wndCmb);
	DDX_Text(pDX, IDC_EDIT_DBADDR2, m_sDbAddr);
	DDX_Text(pDX, IDC_EDIT_DBNAME2, m_sDbUser);
	DDX_Text(pDX, IDC_EDIT_DBPWD2, m_sDbPwd);
	DDX_Text(pDX, IDC_EDIT_DBPORT2, m_sDbPort);
}


BEGIN_MESSAGE_MAP(CDlgCreateAppDB, CDialog)
	ON_BN_CLICKED(IDC_TEST2, &CDlgCreateAppDB::OnBnClickedTest2)
	ON_BN_CLICKED(IDOK, &CDlgCreateAppDB::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCreateAppDB::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_DBTYPE2, &CDlgCreateAppDB::OnCbnSelchangeComboDbtype2)
END_MESSAGE_MAP()


// CDlgCreateAppDB 消息处理程序

void CDlgCreateAppDB::OnBnClickedTest2()
{
	UpdateData(true);

	m_sDbUser.TrimLeft();
	m_sDbUser.TrimRight();

	if(m_sDbUser.CompareNoCase("") == 0) 
	{
		MessageBox("数据库用户名不能为空!","提示",MB_ICONEXCLAMATION);
		return;
	}
	m_sDbPwd.TrimLeft();
	m_sDbPwd.TrimRight();
	BOOL bLocal = FALSE;
#if 0
	CString m_sDbIP;
	if (IsDlgButtonChecked(IDC_CHECK1))
	{

		BYTE nField0=192,nField1=168,nField2=2,nField3=1;
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_DBIP))->GetAddress(nField0,nField1,nField2,nField3);
		m_sDbIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
	}
	else
	{
		m_sDbIP = CCommonFun::GetLocalIP();
		bLocal = TRUE;
	}
#endif
	bLocal = CCommonFun::IsLocalIP(m_sDbAddr);
	if (bLocal)
	{
		m_sDbAddr = "localhost";
	}
	if (m_sDbAddr.IsEmpty() || m_sDbAddr.CompareNoCase("0.0.0.0") == 0)
	{
		MessageBox("请输入数据库服务器地址！","提示",MB_ICONEXCLAMATION);
		return ;
	}
	CoInitialize(0);
	CAdo ado;
	BOOL bRet = FALSE;
	do
	{
		CString sDbName;
		for (int n = 0; n < m_aryDb.GetCount() ; n++)
		{
			sDbName = m_aryDb.GetAt(n);
			ado.SetConnStr(m_sDbAddr,sDbName,m_sDbUser,m_sDbPwd,m_sDbPort);
			bRet = ado.Connect();
			if (!bRet)
			{
				break;
			}
		}

	}while(0);
	if (!bRet)
	{
		MessageBox("连接失败！","提示",MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBox("连接成功！","提示",MB_ICONEXCLAMATION);
	}
	CoUninitialize();
}

void CDlgCreateAppDB::OnBnClickedOk()
{
	bool bSuccess = false;

	UpdateData(true);

	m_sDbUser.TrimLeft();
	m_sDbUser.TrimRight();

	if(m_sDbUser.CompareNoCase("") == 0) 
	{
		MessageBox("数据库用户名不能为空!","提示",MB_ICONEXCLAMATION);
		return;
	}
	m_sDbPwd.TrimLeft();
	m_sDbPwd.TrimRight();

	BOOL bLocal = FALSE;
#if 0
	if (IsDlgButtonChecked(IDC_CHECK1))
	{

		BYTE nField0=192,nField1=168,nField2=2,nField3=1;
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_DBIP))->GetAddress(nField0,nField1,nField2,nField3);
		m_sDbIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3); 
	}
	else
	{
		m_sDbIP = "localhost";//CCommonFun::GetLocalIP();
		bLocal = TRUE;
	}
#endif
	if (m_sDbAddr.CompareNoCase("localhost")==0)
	{
		bLocal = TRUE;
	}
	else
	{
		bLocal = CCommonFun::IsLocalIP(m_sDbAddr);
	}
	if (bLocal)
	{
		m_sDbAddr = "localhost";
	}
	if (m_sDbAddr.IsEmpty() || m_sDbAddr.CompareNoCase("0.0.0.0") == 0)
	{
		MessageBox("请输入数据库服务器地址！","提示",MB_ICONEXCLAMATION);
		return ;
	}
	HCURSOR waitcursor;
	HCURSOR oldcursor;
	CString szPath = CCommonFun::GetDefaultPath();
	szPath += "res\\wait.ani";
	waitcursor=LoadCursorFromFile(szPath);
	oldcursor=SetCursor(waitcursor);
	//安装平台数据库
	CString sPath; 
	OLDDB_INSTALL_TYPE eType = OIT_QUERY;
	CDbInstaller db;
	int nSucCount = 0;
	do
	{
		if (bLocal)
		{
			CString sDbName;
			for (int n = 0; n < m_aryDb.GetCount() ; n++)
			{
				sDbName = m_aryDb.GetAt(n);
				sPath.Format(_T("%sdata\\%s.bak"),CCommonFun::GetDefaultPath(),sDbName);
				theLog.Write("sPath=%s", sPath);
				if(!db.InstallDbByDbbackupFile(m_sDbAddr,sDbName,m_sDbUser,m_sDbPwd,m_sDbPort,sPath,eType))
				{
					MessageBox(db.m_sErr,"提示",MB_ICONEXCLAMATION);
					break;
				}
				nSucCount ++;
			}

		}
		else
		{
			CString sDbName;
			for (int n = 0; n < m_aryDb.GetCount() ; n++)
			{
				sDbName = m_aryDb.GetAt(n);
				sPath.Format(_T("%sdata\\%s.sql"),CCommonFun::GetDefaultPath(),sDbName);
				theLog.Write("sPath=%s", sPath);
				if(!db.InstallDbBySqlFile(m_sDbAddr,sDbName,m_sDbUser,m_sDbPwd,m_sDbPort,sPath,eType))
				{
					MessageBox(db.m_sErr,"提示",MB_ICONEXCLAMATION);
					break;
				}
				nSucCount ++;
			}
		}
	} while (0);

	SetCursor(oldcursor);
	m_bInstalledSuc = (nSucCount == m_aryDb.GetCount()) ? TRUE :FALSE;
	if (!m_bInstalledSuc)
	{
		return ;
	}
	theLog.Write("CDlgCreateAppDB::OnBnClickedOk,安装成功");
	int nSel = m_wndCmb.GetCurSel();
	int nData = m_wndCmb.GetItemData(nSel);

	theConfig.SaveEmbedWebConnStr(m_sDbAddr,m_aryDb.GetAt(0),m_sDbUser,m_sDbPwd,m_sDbPort,nData);

	OnOK();
}

void CDlgCreateAppDB::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgCreateAppDB::OnCbnSelchangeComboDbtype2()
{
	int nSel = m_wndCmb.GetCurSel();
	int nData = m_wndCmb.GetItemData(nSel);
	if (nData == 0)
	{
		m_sDbAddr = CCommonFun::GetLocalIP();
		m_sDbPort;
		CDbInstaller::GetMsdeListenPort("iSecStar",m_sDbPort);
		m_sDbUser = "sa";
		m_sDbPwd = "sa";
		GetDlgItem(IDC_EDIT_DBNAME2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DBPWD2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DBPORT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DBADDR2)->EnableWindow(FALSE);
	}
	else
	{
		m_sDbPort = "1433";
		m_sDbUser = "sa";
		GetDlgItem(IDC_EDIT_DBNAME2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DBPWD2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DBPORT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DBADDR2)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CDlgCreateAppDB::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString sPort;
	if(CDbInstaller::GetMsdeListenPort("iSecStar",sPort))
	{
		int n = m_wndCmb.AddString("桌面数据库（msde）");
		m_wndCmb.SetItemData(n,0);
	}
	int n = m_wndCmb.AddString("Microsoft SQL Server 2000及以后");
	m_wndCmb.SetItemData(n,1);
	m_wndCmb.SetCurSel(0);
	OnCbnSelchangeComboDbtype2();

	// 	this->CheckDlgButton(IDC_CHECK1,BST_UNCHECKED);
	// 
	// 	GetDlgItem(IDC_IPADDRESS_DBIP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgCreateAppDB::CreateDbOnLocalMsde( CString sDbName )
{
	m_aryDb.Add(sDbName);

	m_sDbAddr = CCommonFun::GetLocalIP();
	m_sDbPort;
	CDbInstaller::GetMsdeListenPort("iSecStar",m_sDbPort);
	m_sDbUser = "sa";
	m_sDbPwd = "sa";
	BOOL bLocal = TRUE;

	//安装平台数据库
	CString sPath; 
	OLDDB_INSTALL_TYPE eType = OIT_BACKANDNEW;
	CDbInstaller db;
	int nSucCount = 0;
	do
	{
		if (bLocal)
		{
			CString sDbName;
			for (int n = 0; n < m_aryDb.GetCount() ; n++)
			{
				sDbName = m_aryDb.GetAt(n);
				sPath.Format(_T("%sdata\\%s.bak"),CCommonFun::GetDefaultPath(),sDbName);
				if(!db.InstallDbByDbbackupFile(m_sDbAddr,sDbName,m_sDbUser,m_sDbPwd,m_sDbPort,sPath,eType))
				{
					//MessageBox(db.m_sErr,"提示",MB_ICONEXCLAMATION);
					break;
				}
				nSucCount ++;
			}
		}

	} while (0);

	theLog.Write("CDlgCreateAppDB::CreateDbOnLocalMsde,安装成功");
	theConfig.SaveEmbedWebConnStr(m_sDbAddr,m_aryDb.GetAt(0),m_sDbUser,m_sDbPwd,m_sDbPort,0);

	CString szMsg;
	szMsg.Format("Data Source=%s,%s;Initial Catalog=%s;User ID=%s;Password=%s",
		m_sDbAddr,m_sDbPort,m_aryDb.GetAt(0),m_sDbUser,m_sDbPwd);

	HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
	typedef void (* PFSetDoNetAppDBStr)(char* pStr,int nSize);
	PFSetDoNetAppDBStr fSetDoNetAppDBStr = NULL;
	if(hModule)
	{
		fSetDoNetAppDBStr = 
			(PFSetDoNetAppDBStr)::GetProcAddress(hModule,"SetDoNetAppDBStr");

		if (fSetDoNetAppDBStr)
			fSetDoNetAppDBStr(szMsg.GetBuffer(), szMsg.GetLength());

		::FreeLibrary(hModule);
	}

	return TRUE;
}