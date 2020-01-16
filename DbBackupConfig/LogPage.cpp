// LogPage.cpp : 实现文件
#include "stdafx.h"
#include "DbBackupConfig.h"
#include "LogPage.h"

// CLogPage 对话框
IMPLEMENT_DYNAMIC(CLogPage, CPropertyPage)

CLogPage::CLogPage()
	: CPropertyPage(CLogPage::IDD)
{

}

CLogPage::~CLogPage()
{
}

void CLogPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogPage::OnBnClickedButton1)
END_MESSAGE_MAP()

// CLogPage 消息处理程序
void CLogPage::OnBnClickedButton1()
{
	ShellExecute(NULL,"Open",theLog.m_sLogPath,NULL,NULL,SW_SHOW);
}

BOOL CLogPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// 从日志文件中读取日志在界面上显示
	CString sFilePath = theLog.m_sLogPath;
	CFileException fe;
	CFile mSaveFile;
	CString strTemp,strEnd;

	do 
	{
		if (sFilePath.IsEmpty())
			break;

		// 设置默认值
		if (!mSaveFile.Open(sFilePath,CFile::modeRead,&fe))
			break;

		CArchive ar(&mSaveFile,CArchive::load);
		while (ar.ReadString(strTemp))
		{
			strEnd += strTemp;
			strEnd += "\r\n";
		}
		SetDlgItemText(IDC_EDIT1,strEnd);
		ar.Flush();
		ar.Close();
		mSaveFile.Close();
	} while (0);
	return TRUE;
}
