#pragma once


// CSetupTipDlg 对话框

class CSetupTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupTipDlg)

public:
	CSetupTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetupTipDlg();

	

// 对话框数据
	enum { IDD = IDD_DLG_TIP };
	CString m_sTip;
	CProgressCtrl m_wndProgress;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
class CTipInfo
{
public:
	int nPos;
	CString sTxt;
};
class CSetupTipMgr
{
public:
	CSetupTipMgr()
	{
		m_dwThreadId = 0;
		m_hThread = 0;
	}
	~CSetupTipMgr()
	{
		if(m_hThread)
		{
			::TerminateThread(m_hThread,0);
	
		}
	}
	DWORD m_dwThreadId;
	HANDLE m_hThread;
	static DWORD WINAPI ThreadFun(void* lp);
	static CSetupTipMgr& GetInstance()
	{
		static CSetupTipMgr one;
		return one;
		
	}
	BOOL Open()
	{
		m_hThread = ::CreateThread(0,0,ThreadFun,0,0,&m_dwThreadId); 
		return m_hThread != NULL;
	}
	void SetTip(int nPos,CString sTxt)
	{
		CTipInfo* p = new CTipInfo();
		p->nPos = nPos;
		p->sTxt = sTxt;
		::PostThreadMessage(m_dwThreadId,WM_USER,(WPARAM)p ,0);
	}
	void Close()
	{
		::PostThreadMessage(m_dwThreadId,WM_CLOSE,0 ,0);
		::Sleep(100);
	}
};