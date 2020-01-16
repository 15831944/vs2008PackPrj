// ExportDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "ExportDataDlg.h"

#include "../Public/src/SecText.h"
// CExportDataDlg 对话框

IMPLEMENT_DYNAMIC(CExportDataDlg, CDialog)

CExportDataDlg::CExportDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportDataDlg::IDD, pParent)
{
	m_szDbName = "compmang2";
	m_szIP = CCommonFun::GetLocalIP();
	m_szPort = _T("1433");
	m_szUserName = _T("sa");
	m_szUserPwd = _T("");
}

CExportDataDlg::~CExportDataDlg()
{
}

void CExportDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDataDlg)
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_szDbName);
	DDX_Text(pDX, IDC_EDIT_IP, m_szIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_szPort);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_szUserName);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_szUserPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportDataDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CNNT, &CExportDataDlg::OnBnClickedBtnCnnt)
	ON_BN_CLICKED(IDC_BUTTON2, &CExportDataDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CExportDataDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CExportDataDlg 消息处理程序

void CExportDataDlg::OnBnClickedBtnCnnt()
{
	if(!UpdateData(TRUE))
		return ;
	if(m_szIP == "")
	{
		MessageBox("数据库IP地址不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if(m_szUserName == "")
	{
		MessageBox("数据库用户名不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if(m_szPort == "")
	{
		MessageBox("连接端口不能为空!","提示",MB_OK | MB_ICONINFORMATION);
		return ;
	}

	if(TestConnect())
		MessageBox("连接数据库成功！","提示",MB_OK | MB_ICONINFORMATION);
	else
		MessageBox("连接数据库失败！","提示",MB_OK | MB_ICONINFORMATION);
}
BOOL CExportDataDlg::TestConnect()
{
	if(!UpdateData(TRUE))
		return FALSE;

	BOOL bRet = TRUE;

	CoInitialize(0);
	CAdo ado;


	ado.SetConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd,m_szPort);
	bRet = ado.Connect();


	CoUninitialize();

	return bRet;
}

void CExportDataDlg::OnBnClickedButton2()
{
	if(!UpdateData(TRUE))
		return ;
	BOOL bRet = TRUE;
	CoInitialize(0);
	CAdo ado;
	ado.SetConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd,m_szPort);
	bRet = ado.Connect();

	if (bRet)
	{
		CString sSQL;
		int nType = 0;
		CStringArray ary;
		long lRow = 0;
		long lCol = 0;
		sSQL.Format(_T("select username,userpsw,userrealname,depname from cardinfo a left join "
			" departmenttype b on  a.depid = b.depid "));
		
		if(ado.GetRecordset(sSQL,ary,lRow,lCol) && lRow > 0)
		{
			//
			CString sCode = "";
			CString sFileName;
			CString		sType = "文件 (*.txt)|*.txt|Execel (*.xls)|*.xls||";

			CString		sFilePath;
			CFileDialog dlg(FALSE,"",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,sType,NULL);
			if(dlg.DoModal()==IDOK)
			{
				TRY
				{
					int nType = 0;
					sFileName=dlg.GetFileName();
					CString sTmp = dlg.GetFileExt();
					if(sTmp.CompareNoCase("txt") == 0)
					{
						nType = 0;
					}
					else
					{
						nType = 1;
					}
				}
				CATCH(CException,e)
				{
					return;
				}
				END_CATCH
			}

			//记事本
			if(nType == 0)
			{
				sCode = "\t";
			}
			//excel
			else if(nType == 1)
			{
				sCode.Format("%c",9);
			}
			CStdioFile sf;
			BOOL bRet = sf.Open(sFileName,CFile::modeReadWrite | CFile::modeCreate | CFile::typeText);
			if(!bRet)
			{
				AfxMessageBox(sFileName + "打开失败！");
				return;
			}
			

			for(int nIndex = 0; nIndex < lRow; nIndex ++)
			{
				CString sInfo,sTmp;
				for(int nSubIndex = 0; nSubIndex < lCol; nSubIndex ++)
				{
					sTmp = ary.GetAt(nIndex * lCol + nSubIndex);
					if (nSubIndex == 1)
					{
						CSecText sec;
						sTmp = sec.unsecretstr(sTmp);
					}
					sInfo += sTmp;
					sInfo += sCode;
				}
				sInfo.TrimRight(sCode);
				sInfo += "\n";
				sf.WriteString(sInfo);
			}
			sf.Close();
			MessageBox("数据导出成功！","提示",MB_OK | MB_ICONINFORMATION);
		}
	}

	CoUninitialize();
}

void CExportDataDlg::OnBnClickedBtnSave()
{
	if(!UpdateData(TRUE))
		return ;

	theConfig.SaveSyxConnStr(m_szIP,m_szDbName,m_szUserName,m_szUserPwd,m_szPort);
}
