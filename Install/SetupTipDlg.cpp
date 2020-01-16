// SetupTipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "SetupTipDlg.h"


// CSetupTipDlg 对话框

IMPLEMENT_DYNAMIC(CSetupTipDlg, CDialog)

CSetupTipDlg::CSetupTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupTipDlg::IDD, pParent)
{
	m_sTip = _T("准备安装....");
}

CSetupTipDlg::~CSetupTipDlg()
{
}

void CSetupTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIP, m_sTip);
	DDX_Control(pDX,IDC_PROGRESS1,m_wndProgress);
}


BEGIN_MESSAGE_MAP(CSetupTipDlg, CDialog)
END_MESSAGE_MAP()

DWORD WINAPI CSetupTipMgr::ThreadFun(void* lp)
{
	CSetupTipDlg* pDlg = new CSetupTipDlg();
	ASSERT(pDlg);
	pDlg->Create(CSetupTipDlg::IDD);
	pDlg->ShowWindow(SW_SHOW);

	pDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	pDlg->m_wndProgress.SetRange(0,100);
	pDlg->m_sTip = "开始安装....";
	MSG msg = {0};
	BOOL bRet = FALSE;
	while (bRet = ::GetMessage(&msg,NULL,0,0))
	{
		if (!bRet)
		{
			break;
		}
		if (WM_QUIT == msg.message || WM_CLOSE == msg.message)
		{
			break;
		}
		else if (WM_USER == msg.message)
		{
			CTipInfo* p = (CTipInfo*)msg.wParam;
			if (p)
			{
				pDlg->m_wndProgress.SetPos(p->nPos);
				pDlg->m_sTip = p->sTxt;
				pDlg->UpdateData(FALSE);
				delete p;
			}
		}
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	if (pDlg)
	{
		pDlg->DestroyWindow();
		delete pDlg;
	}
	return 0;
}
// CSetupTipDlg 消息处理程序
