// CreateDbDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Install.h"
#include "CreateDbDlg.h"


// CCreateDbDlg �Ի���

IMPLEMENT_DYNAMIC(CCreateDbDlg, CDialog)

CCreateDbDlg::CCreateDbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateDbDlg::IDD, pParent)
	, m_sDbUser(_T("sa"))
	, m_sDbPwd(_T(""))
	, m_sDbPort(_T("1433"))
	, m_sDbAddr(_T("localhost"))
{
	m_bInstalledSuc = TRUE;
}

CCreateDbDlg::~CCreateDbDlg()
{
}

void CCreateDbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_sDbUser);
	DDX_Text(pDX, IDC_EDIT_DBPWD, m_sDbPwd);
	DDX_Text(pDX, IDC_EDIT_DBPORT, m_sDbPort);
	DDX_Text(pDX, IDC_EDIT_DBADDR, m_sDbAddr);
	DDX_Control(pDX,IDC_COMBO_DBTYPE,m_wndCmb);
}


BEGIN_MESSAGE_MAP(CCreateDbDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateDbDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCreateDbDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &CCreateDbDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_TEST, &CCreateDbDlg::OnBnClickedTest)
	ON_CBN_SELCHANGE(IDC_COMBO_DBTYPE, &CCreateDbDlg::OnSelchangeComboDbType)
END_MESSAGE_MAP()


// CCreateDbDlg ��Ϣ�������
void CCreateDbDlg::OnSelchangeComboDbType()
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
		GetDlgItem(IDC_EDIT_DBNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DBPWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DBPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DBADDR)->EnableWindow(FALSE);
	}
	else
	{
		m_sDbPort = "1433";
		m_sDbUser = "sa";
		GetDlgItem(IDC_EDIT_DBNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DBPWD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DBPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DBADDR)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CCreateDbDlg::OnBnClickedOk()
{
	bool bSuccess = false;

	UpdateData(true);

	m_sDbUser.TrimLeft();
	m_sDbUser.TrimRight();

	if(m_sDbUser.CompareNoCase("") == 0) 
	{
		MessageBox("���ݿ��û�������Ϊ��!","��ʾ",MB_ICONEXCLAMATION);
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
		MessageBox("���������ݿ��������ַ��","��ʾ",MB_ICONEXCLAMATION);
		return ;
	}
	HCURSOR waitcursor;
	HCURSOR oldcursor;
	CString szPath = CCommonFun::GetDefaultPath();
	szPath += "res\\wait.ani";
	waitcursor=LoadCursorFromFile(szPath);
	oldcursor=SetCursor(waitcursor);
	//��װƽ̨���ݿ�
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
				if(!db.InstallDbByDbbackupFile(m_sDbAddr,sDbName,m_sDbUser,m_sDbPwd,m_sDbPort,sPath,eType))
				{
					MessageBox(db.m_sErr,"��ʾ",MB_ICONEXCLAMATION);
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
				if(!db.InstallDbBySqlFile(m_sDbAddr,sDbName,m_sDbUser,m_sDbPwd,m_sDbPort,sPath,eType))
				{
					MessageBox(db.m_sErr,"��ʾ",MB_ICONEXCLAMATION);
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
	theLog.Write("��װ�ɹ�");
	int nSel = m_wndCmb.GetCurSel();
	int nData = m_wndCmb.GetItemData(nSel);

	theConfig.SaveConnStr(m_sDbAddr,m_aryDb.GetAt(0),m_sDbUser,m_sDbPwd,m_sDbPort,nData);
	
	OnOK();
}

void CCreateDbDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CCreateDbDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sPort;
	if(CDbInstaller::GetMsdeListenPort("iSecStar",sPort))
	{
		int n = m_wndCmb.AddString("�������ݿ⣨msde��");
		m_wndCmb.SetItemData(n,0);
	}
	int n = m_wndCmb.AddString("Microsoft SQL Server 2000���Ժ�");
	m_wndCmb.SetItemData(n,1);
	m_wndCmb.SetCurSel(0);
	OnSelchangeComboDbType();

// 	this->CheckDlgButton(IDC_CHECK1,BST_UNCHECKED);
// 
// 	GetDlgItem(IDC_IPADDRESS_DBIP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCreateDbDlg::OnBnClickedCheck1()
{
// 	if (IsDlgButtonChecked(IDC_CHECK1))
// 	{
// 		GetDlgItem(IDC_IPADDRESS_DBIP)->EnableWindow();
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_IPADDRESS_DBIP)->EnableWindow(FALSE);
// 	}
}

void CCreateDbDlg::OnBnClickedTest()
{
	UpdateData(true);

	m_sDbUser.TrimLeft();
	m_sDbUser.TrimRight();

	if(m_sDbUser.CompareNoCase("") == 0) 
	{
		MessageBox("���ݿ��û�������Ϊ��!","��ʾ",MB_ICONEXCLAMATION);
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
		MessageBox("���������ݿ��������ַ��","��ʾ",MB_ICONEXCLAMATION);
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
		MessageBox("����ʧ�ܣ�","��ʾ",MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBox("���ӳɹ���","��ʾ",MB_ICONEXCLAMATION);
	}
	CoUninitialize();
}

BOOL CCreateDbDlg::CreateDbOnLocalMsde(CString sDbName)
{
	m_aryDb.Add(sDbName);

	m_sDbAddr = CCommonFun::GetLocalIP();
	m_sDbPort;
	CDbInstaller::GetMsdeListenPort("iSecStar",m_sDbPort);
	m_sDbUser = "sa";
	m_sDbPwd = "sa";
	BOOL bLocal = TRUE;
		
	//��װƽ̨���ݿ�
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
					//MessageBox(db.m_sErr,"��ʾ",MB_ICONEXCLAMATION);
					break;
				}
				nSucCount ++;
			}
		}
		
	} while (0);

	theLog.Write("��װ�ɹ�");
	theConfig.SaveConnStr(m_sDbAddr,m_aryDb.GetAt(0),m_sDbUser,m_sDbPwd,m_sDbPort,0);

	CString szMsg;
	szMsg.Format("Data Source=%s,%s;Initial Catalog=%s;User ID=%s;Password=%s",
		m_sDbAddr,m_sDbPort,m_aryDb.GetAt(0),m_sDbUser,m_sDbPwd);

	HMODULE hModule = ::LoadLibrary(_T("WebInf.dll"));
	typedef void (* PFSetDoNetDBStr)(char* pStr,int nSize);
	PFSetDoNetDBStr fSetDoNetDBStr = NULL;
	if(hModule)
	{
		fSetDoNetDBStr = 
			(PFSetDoNetDBStr)::GetProcAddress(hModule,"SetDoNetDBStr");

		if (fSetDoNetDBStr)
			fSetDoNetDBStr(szMsg.GetBuffer(), szMsg.GetLength());

		::FreeLibrary(hModule);
	}

	return TRUE;
}