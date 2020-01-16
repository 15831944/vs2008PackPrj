// DBHelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBHelp.h"
#include "DBHelpDlg.h"
#include "SecText.h"
#include "SaturnDBDlg.h"
#include "RegistryEx.h"
#include "oracledbdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CDBHelpApp theApp;
/////////////////////////////////////////////////////////////////////////////
void WriteLogEx(LPCTSTR lpszFormat, ...);
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBHelpDlg dialog

CDBHelpDlg::CDBHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBHelpDlg::IDD, pParent),m_trayIcon(IDR_TRAYICON)
{
	//{{AFX_DATA_INIT(CDBHelpDlg)
	m_nHourSpan = 1;
	m_szPwd2 = _T("");
	m_szUID2 = _T("");
	m_szMsg = _T("");
	m_szDBOwner = _T("");
	m_szDataSource = _T("");
	m_bAutoDep = FALSE;
	m_nTimerSpan = 1;//小时
	m_bCanClose = false;
	m_bRunOnceExit = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDBHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBHelpDlg)
	DDX_Text(pDX, IDC_EDIT_MSG, m_szMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CDBHelpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Set, OnSet)
	ON_BN_CLICKED(IDC_SetSaturn, OnSetSaturn)
	ON_BN_CLICKED(IDC_RightNow, OnRightNow)
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_COMMAND(ID_Exit, OnExit)
	ON_BN_CLICKED(IDC_CHECK1_RunOnce, OnCHECK1RunOnce)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBHelpDlg message handlers

BOOL CDBHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	/*
	CString m_szUserName;
	CString m_szDbName;
	CString m_szServerIP;
	CString m_szPwd;
	CString m_szPort;
	*/
	GetPrintConfig(m_szServerIP,m_szUserName,m_szPwd,m_szDbName,m_szPort);
	/*
	CString	m_szPwd2;
	CString	m_szUID2;
	CString	m_szDBOwner;
	CString	m_szDataSource;
	m_nHourSpan
	*/
	GetOracleConfig(m_szOracleUID,m_szOraclePwd,m_szDBOwner,m_szDataSource,m_nHourSpan);
	//GetOracleConfig(m_szUID2,m_szPwd2,m_szDBOwner,m_szDataSource,m_nHourSpan);
	GetOtherReg();


	CRegistryEx reg;
	reg.SetParam();
	reg.CreateKey("Software\\iSecStar\\Print");
	if(reg.m_hKey)
	{
		CString szMsg;
		reg.Read("RunOnceExit",szMsg);
		m_bRunOnceExit = atoi(szMsg);
		reg.Close();
	}

	if(m_szDBOwner == "" || m_szOracleUID == "" || m_szDataSource == "" || 
		m_szServerIP == "" || m_szUserName == "" || m_szDbName == "")
	{

		m_szMsg = "请正确设置数据库连接参数!";
		goto EXIT;
	}

	SetTimer(11,1,NULL);
	SetTimer(12,24*60*60*1000,NULL);//24小时执行一次

	if(Connect())
	{
		//UpdateSaturnDbInfo();

		if(m_bRunOnceExit)
			SetTimer(1,10*1000,NULL);
		else
			BeginSyncDB();
	}

EXIT:
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_trayIcon.SetIcon(theApp.LoadIcon(IDR_MAINFRAME),"汇文同步工具");

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDBHelpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDBHelpDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDBHelpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CDBHelpDlg::Connect()
{
	m_MSAdo.Release();
	m_szPort = "1433";
	WriteLogEx("print %s,%s,%s,%s,%s！",m_szServerIP,m_szDbName,m_szUserName,m_szPwd,m_szPort);
	if(!m_MSAdo.InitialDB(true,_bstr_t(m_szServerIP),_bstr_t(m_szDbName),_bstr_t(m_szUserName),_bstr_t(m_szPwd),_bstr_t(m_szPort)) )
	{
		m_szMsg = "连接打印数据库失败！";
		WriteLogEx("连接打印数据库失败！");
		UpdateData(false);
		return false;
	}
	else
		WriteLogEx("连接打印数据库成功！");

#if 0
	m_OracleAdo.Release();
	WriteLogEx("oracle %s,%s,%s！",m_szDataSource,m_szOracleUID,m_szOraclePwd);
	if(!m_OracleAdo.InitialDB(false,_bstr_t(""),_bstr_t(m_szDataSource),_bstr_t(m_szOracleUID),_bstr_t(m_szOraclePwd),_bstr_t("")))
	{
		m_szMsg = "连接Oracle数据库失败";
		WriteLogEx("连接Oracle数据库失败");
		UpdateData(false);
		return false;
	}
	else
		WriteLogEx("连接Oracle数据库成功");
#else
	m_OracleAdo.Release();
	WriteLogEx("oracle %s,%s,%s！",m_szDataSource,m_szOracleUID,m_szOraclePwd);
	if(!m_OracleAdo.SetOracleConStr(m_szDataSource,m_szOracleUID,m_szOraclePwd,m_szDBOwner,"1521"))
	{
		m_szMsg = "连接Oracle数据库失败";
		WriteLogEx("连接Oracle数据库失败");
		UpdateData(false);
		return false;
	}
	else
		WriteLogEx("连接Oracle数据库成功");
#endif
	m_szMsg = "状态：正常运行";
	UpdateData(false);
	return true;
}

void GetOracleConfig(CString& sUser,CString& sPwd,CString& sDb,CString& sDataSrc,UINT& nHourSpan)
{
	char  LUserName[200] = {'\0'};
	char  LUPassword[200] = {'\0'};
	char  DBOwner[200] = {'\0'};
	char  DataSource[200] = {'\0'};
	char  TimerSpan[200] = {'\0'};
	
	HKEY  m_hKey = HKEY_LOCAL_MACHINE;
	LONG ReturnValue;
	LPCTSTR pszPath="Software\\iSecStar\\Print";
	LPCTSTR pszKey="Oracle_UID";
	
	ReturnValue = ::RegOpenKeyEx(m_hKey, pszPath, 0L,KEY_QUERY_VALUE, &m_hKey);
    if(ReturnValue == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwSize = 200;
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) LUserName, &dwSize);
		
		dwSize=200;
		pszKey="Oracle_PWD";
		ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
			&dwType, (BYTE *) LUPassword, &dwSize);

		dwSize = 200;
		pszKey="Oracle_DBOwner";
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) DBOwner, &dwSize);

		dwSize = 200;
		pszKey="Oracle_DataSource";
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) DataSource, &dwSize);

		dwSize = 200;
		pszKey="Oracle_TimeSpan";
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) TimerSpan, &dwSize);
		
	}
	RegCloseKey (m_hKey);

	sUser = LUserName;
	sPwd = LUPassword;
	sDb = DBOwner;
	sDataSrc = DataSource;
	nHourSpan = atoi(TimerSpan);
}

void GetOtherReg()
{
	char  TimerSpan[200] = {'\0'};
	char  DBOwner[200] = {'\0'};
	char  DataSource[200] = {'\0'};
	char  AutoDep[200] = {'\0'};

	_bstr_t tstr;//定义转换中间参数变量
	HKEY  m_hKey =HKEY_LOCAL_MACHINE;
	LONG ReturnValue;
	LPCTSTR pszPath="Software\\iSecStar\\Print";
	LPCTSTR pszKey="Oracle_TimeSpan";
	
	ReturnValue = ::RegOpenKeyEx(m_hKey, pszPath, 0L,KEY_QUERY_VALUE, &m_hKey);
    if(ReturnValue == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwSize = 200;
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) TimerSpan, &dwSize);
		
		dwSize = 200;
		pszKey="Oracle_DBOwner";
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) DBOwner, &dwSize);

		dwSize = 200;
		pszKey="Oracle_DataSource";
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) DataSource, &dwSize);

		dwSize = 200;
		pszKey="Oracle_AutoDep";
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) AutoDep, &dwSize);
		
	}
	RegCloseKey (m_hKey);
#if 0
	m_nTimerSpan = atoi(TimerSpan);
	m_szDBOwner = DBOwner;
	m_szDataSource = DataSource;
	m_bAutoDep = atoi(AutoDep);
#endif
}

void GetPrintConfig(CString& sIP,CString& sUser,CString& sPwd,CString& sDbName,CString& sPort)
{
	char ldbinfo[200] = {'\0'};
	char  LUserName[200] = {'\0'};
	char  LUPassword[200] = {'\0'};
	char  LSqlServerIP[200] = {'\0'};
	char  LSqlDbName[200] = {'\0'};
	char  cPort[20] = {'\0'};
	
	HKEY  m_hKey =HKEY_LOCAL_MACHINE;
	LONG ReturnValue;
	LPCTSTR pszPath="Software\\iSecStar\\Print\\database";
	LPCTSTR pszKey="username";
	
	ReturnValue = ::RegOpenKeyEx(m_hKey, pszPath, 0L,KEY_QUERY_VALUE, &m_hKey);
    if(ReturnValue == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwSize = 200;
        ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,&dwType, (BYTE *) LUserName, &dwSize);
		
		dwSize=200;
		pszKey="password";
		ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
			&dwType, (BYTE *) LUPassword, &dwSize);
		
		dwSize=200;
		pszKey="ServIP";
		ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
			&dwType, (BYTE *) LSqlServerIP, &dwSize);

		dwSize=200;
		pszKey="DbName";
		ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
			&dwType, (BYTE *) LSqlDbName, &dwSize);
		
		dwSize=20;
		pszKey="ServPort";
		ReturnValue = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
			&dwType, (BYTE *) cPort, &dwSize);
		
	}
	RegCloseKey (m_hKey);

#ifdef ENABLE_PWD_CRY

	char szUser[MAX_PATH] = {0};
	int nSize = MAX_PATH;
	CPWDCry cry;
	cry.UnCryptPWD((char*)LUserName,szUser,nSize);
	sUser = szUser;

	char szPwd[MAX_PATH] = {0};
	nSize = MAX_PATH;
	cry.UnCryptPWD((char*)LUPassword,szPwd,nSize);
	sPwd = szPwd;
#else
	sUser.Format("%s",LUserName);
	sPwd.Format("%s",LUPassword);
#endif
	
	sIP = LSqlServerIP;
	sDbName = LSqlDbName;
	sPort = cPort;
	if(sPort == "")
		sPort = "1433";

}

void CDBHelpDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		KillTimer(1);

		WriteLogEx("定时器执行，开始同步数据-----OperateDB");

		OperateDB();
		if(m_bRunOnceExit)
		{
			WriteLogEx("通过Windows计划任务执行的，直接执行后退出");
			PostQuitMessage(0);//如果是通过Windows计划任务执行的，则直接执行后退出
		}
	}
	else if(nIDEvent == 11)
	{
		KillTimer(11);

#ifndef _DEBUG
		ShowWindow(SW_HIDE);
#endif
	}
	else if(nIDEvent == 12)
	{
		//add by zhandb 暂时注掉
		//BackupCardInfo();
	}
	CDialog::OnTimer(nIDEvent);
}

bool CDBHelpDlg::OperateDB()
{
	GetPrintConfig(m_szServerIP,m_szUserName,m_szPwd,m_szDbName,m_szPort);
	/*
	CString	m_szPwd2;
	CString	m_szUID2;
	CString	m_szDBOwner;
	CString	m_szDataSource;
	m_nHourSpan
	*/
	GetOracleConfig(m_szOracleUID,m_szOraclePwd,m_szDBOwner,m_szDataSource,m_nHourSpan);
	if(!Connect())
	{
		WriteLogEx("--link to print err");
		BeginSyncDB();
		return false;
	}

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime = time.Format("%Y-%m-%d  %H:%M:%S");
	WriteLogEx("begin sysnc ：[%s]",strTime);
		
	CString szSql;
/*
	szSql.Format("select b.REDR_CERT_ID,a.NAME,a.PASSWORD,a.REDR_FLAG,b.CERT_FLAG,a.DEPT,a.CERT_ID from %s.reader a \
		inner join %s.reader_cert b on a.CERT_ID = b.CERT_ID \
		order by a.CERT_ID,b.OPER_DATE",m_szDBOwner,m_szDBOwner);
		*/

	//---判断身份证有效期
	/*
	szSql.Format("select b.REDR_CERT_ID,a.NAME,a.PASSWORD,a.REDR_FLAG,b.CERT_FLAG,a.DEPT,a.CERT_ID from %s.reader a "
		" inner join %s.reader_cert b on a.CERT_ID = b.CERT_ID and "
		" (a.REDR_DEL_DAY is null or (to_date(to_char(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD') - to_date(a.REDR_DEL_DAY,'YYYY-MM-DD') < 180)) "
		" where (to_date(b.end_date,'YYYY-MM-DD') - to_date(to_char(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD') > 0) and "
		" (to_date(b.end_date,'YYYY-MM-DD') - to_date(to_char(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD') < 180)"
		" order by a.CERT_ID,b.OPER_DATE",m_szOracleUID,m_szOracleUID);
	*/

	szSql.Format("select b.REDR_CERT_ID,a.NAME,a.PASSWORD,a.REDR_FLAG,b.CERT_FLAG,a.DEPT,a.CERT_ID from %s.reader a "
		" inner join %s.reader_cert b on a.CERT_ID = b.CERT_ID and "
		" (a.REDR_DEL_DAY is null or (to_date(to_char(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD') - to_date(a.REDR_DEL_DAY,'YYYY-MM-DD') < 180)) "
		" order by a.CERT_ID,b.OPER_DATE",m_szOracleUID,m_szOracleUID);


	
	//以办证日期为准，可能一个证件号对应好几个条码号，以条码号作为我们打印的上机卡号
	//取出最后一条最新的条码号（新注册的证件），Reader表里的CertID是唯一的，这个字段可能对应到reader_cert 表里的多个证件号

	//szSql.Format("select * from %s.reader",m_szDBOwner);
	CStringArray Rs;
	long lRow,lCol;
	if(!searchrecord2(m_OracleAdo,szSql,&Rs,&lCol,&lRow))
	{
		WriteLogEx("查询orcal表失败:%s",szSql);
		BeginSyncDB();
		return false;
	}
	else
		WriteLogEx("查询orcal表成功   lCol:%d    lRow:%d",lCol,lRow);

	CArray<CString,CString> TiaoMaIDAry;//条码数组
	CString szZhengJianID;//证件号
	CString szCertID,
		szName,
		szPwd,
		szReadFlag,
		szCertFlag,
		szDep,
		szUniqueID;

	for(int i=0;i<lRow;i++)
	{
		szCertID  = Rs.GetAt(i*lCol);//条吗号
		szName  = Rs.GetAt(i*lCol+1);
		szPwd  = Rs.GetAt(i*lCol+2);

		szReadFlag  = Rs.GetAt(i*lCol+3);
		szCertFlag  = Rs.GetAt(i*lCol+4);
		szDep = Rs.GetAt(i*lCol+5);
		szUniqueID  = Rs.GetAt(i*lCol+6);//证件号

		//WriteLogEx2("certid=%s,name=%s,pwd=%s,read=%s,cert=%s,dep=%s",szCertID,szName,szPwd,szReadFlag,szCertFlag,szDep);
		//WriteLogEx("证件号：%s   条码号：%s",szUniqueID,szCertID);
#if 0
		if(szUniqueID == szZhengJianID)
		{
			TiaoMaIDAry.Add(szCertID);
			if(i == lRow -1)
			{
				szZhengJianID = "";
				i--;
				//WriteLogEx("执行到最后一个，为了再执行一次，可能最后一条是需要更新的记录更新不到");
				continue;
			}
		}
		else//不是同一个证件号时
		{
			int nCount = TiaoMaIDAry.GetSize();
			if(nCount > 1)//大于1个，则这个学生的证件号对应多个条码号，此进把所在前面的钱全部同步到最近新加的那个条码号上面来,以减去管理员手动再去同步的情况
			{
				CString szLastTiaoMaID = TiaoMaIDAry.GetAt(nCount-1);

				CString szOldCardIDList = "";
				for(int k=0;k<nCount-1;k++)//最后一个是没有被注销的条码号
				{
					if(k>0)
						szOldCardIDList  += ",";

					szOldCardIDList = szOldCardIDList  + TiaoMaIDAry.GetAt(k) ;
				}
				szOldCardIDList += "";
				szSql.Format("exec ap_TransferOldMoneyToNewCard '%s','%s'",szLastTiaoMaID,szOldCardIDList);

				
			}
			TiaoMaIDAry.RemoveAll();
			TiaoMaIDAry.Add(szCertID);

			//WriteLogEx("不是同一个证件号：当前证件号为：%s    条码号为：%s",szUniqueID,szCertID);
		}
		szZhengJianID = szUniqueID;
#endif
		if(!CheckUserEx(szCertID,szName,szPwd,szReadFlag,szCertFlag,szDep))
		{
			WriteLogEx("CheckUser   执行失败了，返回了");
			break;
		}
		Sleep(10);
	}

	time = CTime::GetCurrentTime();
	strTime = time.Format("%Y-%m-%d  %H:%M:%S");
	WriteLogEx("OperateDB-----------end：[%s]",strTime);

	m_MSAdo.Release();
	m_OracleAdo.Release();

	BeginSyncDB();

	return true;
}

BOOL CDBHelpDlg::DestroyWindow() 
{
	KillTimer(1);
	return CDialog::DestroyWindow();
}


bool CDBHelpDlg::CheckUserEx(CString& szCertID,CString &szName,CString &szPwd,CString &szReadFlag,CString &szCertFlag,CString &szDep)
{
	CSecText sec;
	CString szNewPwd;
	CString szSql;
	CString szShowMsg;
#ifdef ENABLE_PWD_CRY
	CPWDCry cry;
	CString sTmpPwd = szPwd;
	char* pPsw  = cry.CryptPWD(-1,sTmpPwd.GetBuffer());	// 第一个参数又跌一次，打印一定要传-1！ qilu@2012-2-3 16:35
	szNewPwd.Format("%s",pPsw);
	delete pPsw;
	sTmpPwd.ReleaseBuffer();
#endif
	szSql.Format("select id,UserId,State,pwd from  dbo.v_s_account where accountname = '%s' ",szCertID);

	CStringArray Rs;
	long lRow = 0,lCol = 0;
	if(!searchrecord2(m_MSAdo,szSql,&Rs,&lCol,&lRow))
	{
		WriteLogEx("查询打印数据库失败");
		return false;
	}
	
	szSql.Empty();
	//
	if(lRow > 1)
	{
		WriteLogEx("查询打印存在多条数据，异常");
	}
	else if(lRow == 1)
	{
		int nId = atoi(Rs.GetAt(0));
		int nUserId = atoi(Rs.GetAt(1));
		int nState = atoi(Rs.GetAt(2));
		CString sTmp = Rs.GetAt(3);
		CString sDbPwd;
#ifdef ENABLE_PWD_CRY
		char szDbPwd[MAX_PATH] = {0};
		int nSize = MAX_PATH;
		cry.UnCryptPWD((char*)sTmp.GetBuffer(),szDbPwd,nSize);
		sDbPwd.Format("%s",szDbPwd);
		sTmp.ReleaseBuffer();
#endif
		if ( szCertFlag == "3" )//汇文里停借   我们必须是
		{
			//-------这边特殊处理，因为南医大要求为3的状态在汇文里面是新生没开卡，此时也要可以上机用，作为正常卡状态			
			szSql.Format("update dbo.t_s_UserEx set State = 1,name = '%s',password = '%s',updatetime=getdate()  where code = '%s' "
				"update dbo.t_s_accountinfo set State = 1,updatetime=getdate() where UserId = %d",
				szName,szNewPwd,szCertID,nUserId);
			
			szShowMsg += "      汇文里停止借书，但作为正常卡";
			WriteLogEx("user [%s,%s]----汇文里停止借书，但作为正常卡",szName,szCertID);

		}
		else if ( szCertFlag == "2" )//汇文里挂失   我们必须是
		{
			if(nState != 0 || sDbPwd.CompareNoCase(szPwd) != 0)
			{
				szSql.Format("update dbo.t_s_UserEx set State = 0,name = '%s',password = '%s',updatetime=getdate()  where code = '%s' "
				"update dbo.t_s_accountinfo set State = 0 where UserId = %d",
				szName,szNewPwd,szCertID,nUserId);

				szShowMsg += "      汇文里挂失";

				WriteLogEx("user[%s,%s]----汇文里挂失",szName,szCertID);
			}

		}
		else if ( szCertFlag == "1" )//汇文里正常   我们必须是正常的
		{
			if(sDbPwd.CompareNoCase(szPwd) != 0 || nState != 1)
			{
				szSql.Format("update dbo.t_s_UserEx set State = 1,name = '%s',password = '%s',updatetime=getdate()  where code = '%s' "
					"update dbo.t_s_accountinfo set State = 1,updatetime=getdate() where UserId = %d",
					szName,szNewPwd,szCertID,nUserId);

				szShowMsg += "      汇文里正常";

				WriteLogEx("user need update pwd [%s,%s](%s,%s,%d)----汇文里正常",szName,szCertID,sDbPwd,szNewPwd,nState);
			}
		}
		else if ( szCertFlag == "0" || szReadFlag == "0")//汇文里读者表或证件表被注销了，我们也必须注销
		{
			if( nState != 0)
			{
				szSql.Format("update dbo.t_s_UserEx set State = 0,name = '%s',password = '%s'  where code = '%s',updatetime=getdate() "
					"update dbo.t_s_accountinfo set State = 0,updatetime=getdate() where UserId = %d ",
				szName,szNewPwd,szCertID,nUserId);

				szShowMsg += "      汇文里注销";

				WriteLogEx("user[%s,%s]----汇文里注销",szName,szCertID);
			}
		}
		else
		{
			//更新成正常状态
			szSql.Format("update dbo.t_s_UserEx set State = 1,name = '%s',password = '%s',updatetime=getdate()  where code = '%s' "
				"update dbo.t_s_accountinfo set State = 1,updatetime=getdate() where UserId = %d ",
				szName,szNewPwd,szCertID,nUserId);
			WriteLogEx("come normal %s",szName);
		}
			
		if(szSql != "")
		{
			if(Sockcommitsql(m_MSAdo,szSql))
			{
				//WriteLogEx("执行成功了!");
			}
			else
			{
				WriteLogEx("err,SQL=%s",szSql);
			}
		}
			
	}
	else if(lRow == 0)
	{
		if(szReadFlag == "1" && (szCertFlag == "1" || szCertFlag == "3") )//南医大汇文里面特殊处理，因为为3的用户是新生未开卡，此时也要作为正常状态处理
		{
			
			szSql.Format("declare @id int,@depid int,@pid int,"
					" @accountid int,@depName varchar(200),"
					" @pwd varchar(200),@name varchar(1000),"
					" @code varchar(100)"
					" set @id = 0"
					" set @accountid = 0"
					" set @code = '%s'"
					" set @pwd = '%s'"
					" set @name = '%s'"
					" set @depname = '%s'"
					" set @depid = 0"
					" select @pid = id from dbo.t_Department where isDefault = 1"
					" begin tran "
					" select @depid = isnull(id,0) from dbo.t_Department where name = @depname "
					" if @depid = 0 "
					" begin "
					"	insert into t_department(name,pid) values(@depname,@pid) "
					"	if @@error <> 0 goto lb_rollback "
					"	set @depid = SCOPE_IDENTITY() "
					"	insert into t_s_departmentEx(depid) values(@depId) "
					"	if @@error <> 0 goto lb_rollback "
					" end "
					" delete from t_s_userex where code = @code "
					" if @@error <> 0 goto lb_rollback "
					" insert into dbo.t_s_userEx (code,name,password,createdon,updatetime) values (@code,@name,@pwd,getdate(),getdate()) "
					" if @@error <> 0 goto lb_rollback "
					" set @id = SCOPE_IDENTITY() "
					" insert into dbo.t_s_AccountInfo (userId,depid,createdon,updatetime) values (@id,@depid,getdate(),getdate()) "
					" if @@error <> 0 goto lb_rollback "
					" set @accountid = SCOPE_IDENTITY() "
					" insert dbo.t_p_Quota (A3Count, A4Count, PeriodType, MemberType, MemberId) values (0,0,-1,1,@accountid) "
					" if @@error <> 0 goto lb_rollback "
					" lb_commit:"
					" commit tran "
					" goto lb_ok "
					" lb_rollback: rollback tran "
					" lb_ok:",szCertID,szNewPwd,szName,szDep);
			if(Sockcommitsql(m_MSAdo,szSql))
			{
				WriteLogEx("add new user (%s,%s) suc!",szName,szCertID);

				
			}
			else
			{
				WriteLogEx("add new user (%s,%s) err,sql = %s!",szName,szCertID,szSql);
				
				return false;
			}
		}
	}

	return true;
}

bool CDBHelpDlg::CheckUser(CString &szCertID, CString &szName, CString &szPwd, CString &szReadFlag, CString &szCertFlag,CString &szDep)
{
	return 0;
#if 0
	//记事本
	CString sCode = "\t";
	
	CStdioFile sf;
	BOOL bRet = sf.Open("c:\hw.txt",CFile::modeReadWrite | CFile::modeCreate | CFile::typeText);
	if(!bRet)
	{
		AfxMessageBox("打开失败！");
		return true;
	}
	CString sInfo,
		sTmp;
	
	sInfo.Format(_T("%s%s%s%s%s%s%s\n"),szCertID,sCode,
									szName,sCode,
									szPwd,sCode,
									szDep);

		sf.WriteString(sInfo);

	sf.Close();

	return true;
#endif

	CSecText sec;
	CString szNewPwd = sec.secretstr(szPwd);

	CString szSql;

	szSql.Format("select CardID,UserName,UserPsw,UserRealName,CardState,0,b.DepName from cardinfo a inner join DepartMentType b on a.DepID = b.DepID where UserName = '%s' \
				  union \
				  select CardID,UserName,UserPsw,UserRealName,CardState,1,b.DepName from historycardinfo  a inner join DepartMentType b on a.DepID = b.DepID where UserName = '%s' ",
				  szCertID,szCertID);

	CStringArray Rs;
	long lRow,lCol;
	if(!searchrecord2(m_MSAdo,szSql,&Rs,&lCol,&lRow))
	{
		WriteLogEx("联查打印数据库失败");
		return false;
	}


	bool bHistory = false;
	CString szCardID,szUserName,szMSPwd,szRealName,szCardState,szMSDep;
	if(lRow == 1)
	{
		szCardID = Rs.GetAt(0);
		szUserName  = Rs.GetAt(1);
		szMSPwd  = sec.unsecretstr(Rs.GetAt(2));//解密一下
		szRealName  = Rs.GetAt(3);
		szCardState = Rs.GetAt(4);
		bHistory  = atoi(Rs.GetAt(5));
		szMSDep = Rs.GetAt(6);

		if(bHistory)//已在历史表中。必需是已被注销的用户。
		{
			if(szReadFlag == "0" || szCertFlag == "0" )//读者表注销或证件表注销都当作是注销
			{
				return true;
			}
			else
			{
				//汇文数据库中的这个用户已经恢复正常使用了，而我们数据库中还是被注销的
				CString szSql;
				szSql.Format(" exec ap_ResumeOneUser '%s'",szUserName);
				if(Sockcommitsql(m_MSAdo,szSql))
				{
					WriteLogEx("用户[%s]从注销状态恢复正常成功!",szRealName);	
					
					szSql.Format("update CardInfo set UserRealName = '%s' ,UserPsw = '%s'   where UserName = '%s'",
						szName,szNewPwd,szUserName);
					Sockcommitsql(m_MSAdo,szSql);

				}
				else
				{
					WriteLogEx("用户[%s]从注销状态恢复失败,SQL如下：%s!",szRealName,szSql);
					return false;
				}
			}
		}
		else
		{
			bool bUpdateDep = false;
			bool bUpdateName = false;
			szSql = "";
			//在卡表里面
			CString szShowMsg ;
			if(szRealName != szName || szMSPwd != szPwd)//用户名或密码，或部门名称不相等
			{
				szShowMsg = "用户名、密码不同";

				bUpdateName = true;

				WriteLogEx("用户[%s]-----用户名、密码不同",szName);

			}


			if(szMSDep.CompareNoCase(szDep) != 0)
			{
				szShowMsg += "    部门不同";
				bUpdateDep = true;

				WriteLogEx("用户[%s]----部门不同",szName);
			}

			if ( szCertFlag == "3" && szCardState != "3")//汇文里停借   我们必须是
			{
				/*
				if(bUpdateName)
					szSql.Format("update CardInfo set CardState = 3,UserRealName = '%s',UserPsw = '%s'  where UserName = '%s' ",
					szName,szNewPwd,szUserName);
				else
					szSql.Format("update CardInfo set CardState = 3 where UserName = '%s'",szUserName);

				szShowMsg += "      汇文里停借";

				WriteLogEx("用户[%s]----汇文里停借",szName);
				*/

				//-------这边特殊处理，因为南医大要求为3的状态在汇文里面是新生没开卡，此时也要可以上机用，作为正常卡状态

				if(bUpdateName)
					szSql.Format("update CardInfo set CardState = 0,UserRealName = '%s',UserPsw = '%s'  where UserName = '%s' ",
					szName,szNewPwd,szUserName);
				else
					szSql.Format("update CardInfo set CardState = 0 where UserName = '%s'",szUserName);

				szShowMsg += "      汇文里停借，但作为正常卡";

				WriteLogEx("用户[%s]----汇文里停借，但作为正常卡",szName);

			}
			else if ( szCertFlag == "2" && szCardState != "2")//汇文里挂失   我们必须是
			{
				if(bUpdateName)
					szSql.Format("update CardInfo set CardState = 2,UserRealName = '%s',UserPsw = '%s'   where UserName = '%s'",
					szName,szNewPwd,szUserName);
				else
					szSql.Format("update CardInfo set CardState = 2 where UserName = '%s'",szUserName);

				szShowMsg += "      汇文里挂失";

				WriteLogEx("用户[%s]----汇文里挂失",szName);

			}
			else if ( szCertFlag == "1" && szCardState != "0")//汇文里正常   我们必须是正常的
			{
				if(bUpdateName)
					szSql.Format("update CardInfo set CardState = 0,UserRealName = '%s',UserPsw = '%s'    where UserName = '%s'",
					szName,szNewPwd,szUserName);
				else
					szSql.Format("update CardInfo set CardState = 0 where UserName = '%s'",szUserName);

				szShowMsg += "      汇文里正常";

				WriteLogEx("用户[%s]----汇文里正常",szName);

			}
			else if ( szCertFlag == "0" || szReadFlag == "0")//汇文里读者表或证件表被注销了，我们也必须注销
			{
				if(bUpdateName)
					szSql.Format("update CardInfo set UserRealName = '%s' ,UserPsw = '%s'   where UserName = '%s' \
					exec ap_UpdateCardState %d,1 ",szName,szNewPwd,szUserName,
					szCardID);
				else
					szSql.Format("exec ap_UpdateCardState %s,1 ",szCardID);

				szShowMsg += "      汇文里注销";

				WriteLogEx("用户[%s]----汇文里注销",szName);
			}
			if(bUpdateName && szSql == "")
			{
				szSql.Format("update CardInfo set UserRealName = '%s',UserPsw = '%s'   where UserName = '%s' ",
					szName,szNewPwd,szUserName);
			}

			if(szSql != "")
			{
				if(Sockcommitsql(m_MSAdo,szSql))
				{
					//WriteLogEx("执行成功了!");
				}
				else
				{
					WriteLogEx("执行失败了,SQL语句如下：!",szSql);
				}
			}
			//-----------------------------------
			if(bUpdateDep)//需要更新部门,在历史卡表里面不执行更新
			{
				szSql.Format("select DepID from departmenttype where DepName = '%s'",szDep);
				int nDepID = atoi(fieldValue2(m_MSAdo,szSql));
				if(nDepID <= 0)
				{
					szSql.Format("insert into departmenttype (DepName,parentdepid) values ('%s',1)",szDep);//增加部门到根部门下面
					if(Sockcommitsql(m_MSAdo,szSql))
					{
						szSql.Format("select DepID from departmenttype where DepName = '%s'",szDep);
						nDepID = atoi(fieldValue2(m_MSAdo,szSql));
					}
					else
						WriteLogEx("新增加部门失败,SQL如下：%s",szSql);
				}
				if(nDepID > 0)
				{
					szSql.Format("update CardInfo Set DepID = %d where UserName = '%s'",nDepID,szUserName);
					if(!Sockcommitsql(m_MSAdo,szSql))
						WriteLogEx("执行更新部门失败,SQL语句如下：%s",szSql);
				}
			}
		}

	}
	else if(lRow > 1)
	{
		WriteLogEx("查询记录不正常，多于一行[%d]",lRow);//最多只可能有一条记录。
	}
	else if(lRow == 0)
	{
		//if(szReadFlag == "1" &&  szCertFlag == "1"  )
		if(szReadFlag == "1" && (szCertFlag == "1" || szCertFlag == "3") )//南医大汇文里面特殊处理，因为为3的用户是新生未开卡，此时也要作为正常状态处理
		{
			CString szSql;
			szSql.Format("exec ap_AddOneUser '%s','%s','%s',%d,%d,%d",
				szCertID,szNewPwd,szName,2,1,1);

			if(Sockcommitsql(m_MSAdo,szSql))
			{
				WriteLogEx("添加新用户[%s]到卡表成功!",szName);

				
			}
			else
			{
				WriteLogEx("添加新用户[%s]到卡表失败,SQL如下：%s",szName,szSql);
				return false;
			}
		}
	}
	return true;
}

LRESULT CDBHelpDlg::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification(uID, lEvent);
}

void CDBHelpDlg::OnExit()
{
	m_bCanClose = true;
	PostMessage(WM_CLOSE);
}

void CDBHelpDlg::OnClose() 
{
	if(m_bRunOnceExit)///直接关闭退出
	{
		CDialog::OnClose();
		return ;
	}


	if(!m_bCanClose)
	{
		ShowWindow(SW_HIDE);
		return ;
	}

	Release();

	CDialog::OnClose();
}

void CDBHelpDlg::OnSet() 
{
	COracleDbDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		GetOracleConfig(m_szUID2,m_szPwd2,m_szDBOwner,m_szDataSource,m_nHourSpan);
	}
	TestOracleDB();
}

bool CDBHelpDlg::WriteReg(char* szName,LPCTSTR szValue)
{
	CRegistryEx reg;
	reg.SetParam();
	reg.CreateKey("Software\\iSecStar\\Print");
	if(reg.m_hKey)
	{
		reg.Write(CString(szName),CString(szValue));
		reg.Close();
	}
	return true;
}

void CDBHelpDlg::OnSetSaturn() 
{

	CSaturnDBDlg dlg;
	dlg.DoModal();
	
}

void CDBHelpDlg::BeginSyncDB()
{
	WriteLogEx("BeginSyncDB");
#ifdef _DEBUG
	SetTimer(1,12*1000,NULL);
#else
	SetTimer(1,m_nTimerSpan*60*60*1000,NULL);
#endif
}

void CDBHelpDlg::OnRightNow() 
{
	WriteLogEx("OnRightNow--手动执行同步");
	BOOL bRet = OperateDB();
	//BackupCardInfo();
	if(!bRet)
	{
		AfxMessageBox("同步失败");
	}
	else
		AfxMessageBox("同步成功!");
}

void CDBHelpDlg::BackupCardInfo()//CardInfo表一天只备份一次
{
	WriteLogEx("BackupCardInfo--开始备份用户表数据!");

	m_MSAdo.Release();
	m_szPort = "1433";
	if(!m_MSAdo.InitialDB(true,_bstr_t(m_szServerIP),_bstr_t(m_szDbName),_bstr_t(m_szUserName),_bstr_t(m_szPwd),_bstr_t(m_szPort)) )
	{
		WriteLogEx("BackupCardInfo--连接打印数据库失败！");
		return ;
	}

	CString szSql;
	szSql.Format("if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BackUpCardInfo]') and OBJECTPROPERTY(id, N'IsUserTable') = 1) \
					CREATE TABLE [dbo].[BackUpCardInfo] (\
						[ID] [int] IDENTITY (1, 1) NOT NULL ,\
						[BackupTime] [datetime] NULL ,\
						[FilePath] [varchar] (300) COLLATE Chinese_PRC_CI_AS NULL ,\
						[nFlag] [int] NULL \
					) ON [PRIMARY] ");

	if(!Sockcommitsql(m_MSAdo,szSql))
	{
		WriteLogEx("创建备份用户信息表失败!");
		return ;
	}
	CStringArray Rs;
	long lRow,lCol;
	//------------------------备份卡表-----------------------------
	szSql.Format("select * from CardInfo ");
	if(!searchrecord2(m_MSAdo,szSql,&Rs,&lCol,&lRow))
		return ;


	CString szFileName;

	CTime time = CTime::GetCurrentTime();
	CString szTime;
	szTime = time.Format("%Y-%m-%d-%H-%M-%S");
	
	::CreateDirectory(GetDefaultPath() + "\\Backup",NULL);


	szFileName.Format("%s\\backup\\%s.txt",GetDefaultPath(),szTime);

	/*
	char cLongName[300];
	DWORD dwRet =  GetLongPathName(szFileName,cLongName,sizeof(cLongName));
	if(dwRet)
	{
		return cLongName;
	}
	*/

	szSql.Format("insert into BackUpCardInfo(BackupTime,FilePath) values(getdate(),'%s')",szFileName);
	if(!Sockcommitsql(m_MSAdo,szSql))
	{
		WriteLogEx("添加备份用户信息表失败!");
		return ;
	}


	CFile file;
	if(!file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		TRACE("创建文件[%s]失败:%d\n",szFileName,GetLastError());
		CString szMsg;
		szMsg.Format("创建文件[%s]失败:%d\n",szFileName,GetLastError());
	//	AfxMessageBox(szMsg);
		return ;
	}
	for(int i=0;i<lRow;i++)
	{
		CString szLines;
		for(int k =0;k<lCol;k++)
		{
			if(k != 0)
				szLines += ",";

			szLines = szLines + "\"" + Rs.GetAt(i*lCol+k) + "\"";
		}
		file.Write((LPCTSTR)szLines,szLines.GetLength());
		file.Write("\r\n",strlen("\r\n"));
	}
	file.Close();
	//------------------------------------------------------------
	szSql.Format("select ID,BackupTime,FilePath from BackUpCardInfo where ID not in \
		(select Top 30 ID from BackUpCardInfo order by BackupTime desc) ");

	if(!searchrecord2(m_MSAdo,szSql,&Rs,&lCol,&lRow))
		return ;

	if(lRow < 1)
		return ;

	for(int i=0;i<lRow;i++)
	{
		int nID = atoi(Rs.GetAt(i*lCol));
		CString szFilePath = Rs.GetAt(i*lCol + 2);

		szSql.Format("delete from BackUpCardInfo where ID = %d",nID);
		if(!Sockcommitsql(m_MSAdo,szSql))
		{
			WriteLogEx("删除备份用户信息表失败!");
			return ;
		}
		else
		{
			::DeleteFile(szFilePath);
		}
	}
	m_MSAdo.Release();

	WriteLogEx("备份用户信息表成功!");
}

CString CDBHelpDlg::GetDefaultPath()
{
	CString szRet;
	char szTmp[MAX_PATH] = {0};
	int nRet = ::GetModuleFileName(NULL,szTmp,MAX_PATH);
	if(nRet <= 0)
		return szRet;
	char* pDes = strrchr(szTmp,'\\');
	if(pDes)
		*pDes = '\0';

	return szTmp;
}

BOOL CDBHelpDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CDBHelpDlg::UpdateSaturnDbInfo()
{
	CString szSql;
	szSql.Format("delete from systemset where SetName = 'AutoGetOracleData' \
		Insert into systemset(SetName,SetValue) values('AutoGetOracleData','1')");
	Sockcommitsql(m_MSAdo,szSql);
}

void CDBHelpDlg::Release()
{
	CString szSql;
	szSql.Format("delete from systemset where SetName = 'AutoGetOracleData' " );
	Sockcommitsql(m_MSAdo,szSql);
}


void CDBHelpDlg::OnCHECK1RunOnce() 
{
	UpdateData(true);

	CString szMsg;
	szMsg.Format("%d",m_bRunOnceExit);
	WriteReg("RunOnceExit",szMsg);	
}

void CDBHelpDlg::TestOracleDB()
{
	GetPrintConfig(m_szServerIP,m_szUserName,m_szPwd,m_szDbName,m_szPort);
	/*
	CString	m_szPwd2;
	CString	m_szUID2;
	CString	m_szDBOwner;
	CString	m_szDataSource;
	m_nHourSpan
	*/
	GetOracleConfig(m_szOracleUID,m_szOraclePwd,m_szDBOwner,m_szDataSource,m_nHourSpan);

	if(!Connect())
	{
		AfxMessageBox("TestOracleDB--Connect本次连接失败");
		return;
	}
	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime = time.Format("%Y-%m-%d  %H:%M:%S");
	WriteLogEx("TestOracleDB======================同步开始时间为：[%s]",strTime);
		
	CString szSql;

/*
	szSql.Format("select b.REDR_CERT_ID,a.NAME,a.PASSWORD,a.REDR_FLAG,b.CERT_FLAG,a.DEPT,a.CERT_ID from %s.reader a \
		inner join %s.reader_cert b on a.CERT_ID = b.CERT_ID and (a.REDR_DEL_DAY is null or (to_date(to_char(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD') - to_date(a.REDR_DEL_DAY,'YYYY-MM-DD') < 180))  \
		order by a.CERT_ID,b.OPER_DATE",m_szDBOwner,m_szDBOwner);
*/
	szSql.Format("select b.REDR_CERT_ID,a.NAME,a.PASSWORD,a.REDR_FLAG,b.CERT_FLAG,a.DEPT,a.CERT_ID,a.REDR_DEL_DAY from %s.reader a \
		inner join %s.reader_cert b on a.CERT_ID = b.CERT_ID where b.REDR_CERT_ID = 'S055289' \
		order by a.CERT_ID,b.OPER_DATE",m_szOracleUID,m_szOracleUID);
	
	//以办证日期为准，可能一个证件号对应好几个条码号，以条码号作为我们打印的上机卡号
	//取出最后一条最新的条码号（新注册的证件），Reader表里的CertID是唯一的，这个字段可能对应到reader_cert 表里的多个证件号

	//szSql.Format("select * from %s.reader",m_szDBOwner);
	CStringArray Rs;
	long lRow,lCol;
	if(!searchrecord2(m_OracleAdo,szSql,&Rs,&lCol,&lRow))
	{
		WriteLogEx("查询汇数据库表失败:%s",szSql);
		AfxMessageBox("查询汇数据库表失败");
		return;
	}
	else
		WriteLogEx("TestOracleDB---查询汇数据库表成功   lCol:%d    lRow:%d",lCol,lRow);


	if(lRow < 1 )
	{
		AfxMessageBox("没有查询到这个卡!");
		return;
	}



	CArray<CString,CString> TiaoMaIDAry;//条码数组
	CString szZhengJianID;//证件号
 
	CSecText sec;
	CString szNewPwd;
	/*CString szSql;*/
	CString szShowMsg;
	CPWDCry cry;

	CString szCertID,szName,szPwd,szReadFlag,szCertFlag,szDep,szUniqueID,szDelDay;
	for(int i=0;i<lRow;i++)
	{
		szCertID  = Rs.GetAt(i*lCol);//条吗号
		szName  = Rs.GetAt(i*lCol+1);
		szPwd  = Rs.GetAt(i*lCol+2);

		szReadFlag  = Rs.GetAt(i*lCol+3);
		szCertFlag  = Rs.GetAt(i*lCol+4);
		szDep = Rs.GetAt(i*lCol+5);
		szUniqueID  = Rs.GetAt(i*lCol+6);//证件号
		szDelDay  = Rs.GetAt(i*lCol+7);//日期

		AfxMessageBox(szPwd);
#ifdef ENABLE_PWD_CRY
		
		char* pPsw  = cry.CryptPWD(-1,szPwd.GetBuffer());	// 第一个参数又跌一次，打印一定要传-1！ qilu@2012-2-3 16:35
		szNewPwd.Format("%s",pPsw);
		delete pPsw;
		szPwd.ReleaseBuffer();
		AfxMessageBox(szNewPwd);
#endif


		CString szMsg;
		szMsg.Format("证件号：%s 条码号：%sszDelDay:%s,pwd %s,read:%s,cert:%s",szUniqueID,szCertID,szDelDay,szPwd,
			szReadFlag,szCertFlag);
		AfxMessageBox(szMsg);
	}

	time = CTime::GetCurrentTime();
	strTime = time.Format("%Y-%m-%d  %H:%M:%S");
	WriteLogEx("TestOracleDB--------结束时间为：[%s]",strTime);


	szSql.Format("select id,UserId,State,pwd from  dbo.v_s_account where accountname = '%s' ",szCertID);

	/*CStringArray Rs;*/
	Rs.RemoveAll();
	//long lRow = 0,lCol = 0;
	if(!searchrecord2(m_MSAdo,szSql,&Rs,&lCol,&lRow))
	{
		WriteLogEx("查询打印数据库失败");
		return ;
	}
	CString sDbPwd;
	int nState = 0;
	int nUserId = 0;
	int nId = 0;
	if(lRow == 1)
	{
		nId = atoi(Rs.GetAt(0));
		nUserId = atoi(Rs.GetAt(1));
		nState = atoi(Rs.GetAt(2));
		CString sTmp = Rs.GetAt(3);
		
#ifdef ENABLE_PWD_CRY
		char szLocalPwd[MAX_PATH] = {0};
		int nSize = MAX_PATH;
		cry.UnCryptPWD((char*)sTmp.GetBuffer(),szLocalPwd,nSize);
		sDbPwd.Format("%s",szLocalPwd);
		AfxMessageBox(szLocalPwd);
		sTmp.ReleaseBuffer();
#endif
		CString szMsg;
		szMsg.Format("code：%s ,pwd %s,%s,%d",szCertID,sTmp,sDbPwd,nSize);
		AfxMessageBox(szMsg);
	}
	
	//if(sDbPwd.CompareNoCase(szPwd) != 0 || nState != 1)
	{
		szSql.Format("update dbo.t_s_UserEx set State = 1,name = '%s',password = '%s'  where code = '%s',updatetime=getdate() "
			"update dbo.t_s_accountinfo set State = 1,updatetime=getdate() where UserId = %d",
			szName,szNewPwd,szCertID,nUserId);
		Sockcommitsql(m_MSAdo,szSql);
		szShowMsg += "      汇文里正常";
		WriteLogEx("sql = %s",szSql);
		WriteLogEx("user need update pwd [%s,%s](%s,%s,%d)----汇文里正常",szName,szCertID,sDbPwd,szNewPwd,nState);
		AfxMessageBox("hhh");
	}

	m_MSAdo.Release();
	m_OracleAdo.Release();
}
