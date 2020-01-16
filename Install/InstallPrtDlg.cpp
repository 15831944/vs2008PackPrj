// InstallPrtDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "InstallPrtDlg.h"

#include "basesock.h"
// CInstallPrtDlg 对话框

IMPLEMENT_DYNAMIC(CInstallPrtDlg, CDialog)

CInstallPrtDlg::CInstallPrtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallPrtDlg::IDD, pParent)
	, m_sSpeed(_T(""))
	, m_sResolution(_T(""))
	, m_sDesc(_T(""))
	, m_fA3HB(0)
	, m_fA3Color(0)
	, m_fA4HB(0)
	, m_fA4Color(0)
	, m_sColor(_T(""))
{

}

CInstallPrtDlg::~CInstallPrtDlg()
{
}

void CInstallPrtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PRT, m_wndCmbPrinter);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_sSpeed);
	DDX_Text(pDX, IDC_EDIT_RESOLUTION, m_sResolution);
	DDX_Text(pDX, IDC_EDIT_DESC, m_sDesc);
	DDX_Text(pDX, IDC_EDIT_A3HB, m_fA3HB);
	DDV_MinMaxFloat(pDX, m_fA3HB, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_A3COLOR, m_fA3Color);
	DDV_MinMaxFloat(pDX, m_fA3Color, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_A4HB, m_fA4HB);
	DDV_MinMaxFloat(pDX, m_fA4HB, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_A4COLOR, m_fA4Color);
	DDV_MinMaxFloat(pDX, m_fA4Color, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_COLOR, m_sColor);
}


BEGIN_MESSAGE_MAP(CInstallPrtDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInstallPrtDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PRT, &CInstallPrtDlg::OnCbnSelchangeComboPrt)
END_MESSAGE_MAP()


// CInstallPrtDlg 消息处理程序

void CInstallPrtDlg::OnBnClickedOk()
{
	if (!UpdateData())
	{
		return ;
	}
	int nSel = m_wndCmbPrinter.GetCurSel();
	if (nSel < 0)
	{
		ASSERT(0);
	}
	BOOL bDefault = IsDlgButtonChecked(IDC_CHECK_DEFAULT_PRT);
	CString sPrinter;
	m_wndCmbPrinter.GetLBText(nSel,sPrinter);
	HANDLE hPrinter = 0;
	if(OpenPrinter((LPSTR)(LPCTSTR)sPrinter,&hPrinter,0))
	{
		DWORD dwOut = 0;
		DWORD dwSize = 4096;
		BYTE* pBuf = new BYTE[dwSize];
		GetPrinter(hPrinter,2,pBuf,dwSize,&dwOut);
		PRINTER_INFO_2* pInfo = (PRINTER_INFO_2*)pBuf;
		//theLog.Write("printer = %s,%d,%d",pInfo->pPrinterName,pInfo->pDevMode->dmColor,pInfo->pDevMode->dmYResolution);
		m_oAri.dwSpeed = pInfo->AveragePPM;
		m_oAri.dwColor = pInfo->pDevMode ? pInfo->pDevMode->dmColor : 1;
		m_oAri.dwResolution = pInfo->pDevMode ? pInfo->pDevMode->dmYResolution : 0;
		strcpy_s(m_oAri.szName,sizeof(m_oAri.szName),sPrinter);
		strcpy_s(m_oAri.szDesc,sizeof(m_oAri.szDesc),m_sDesc);

		delete pBuf;
		ClosePrinter(hPrinter);
	}
	m_oAri.bDefault = bDefault;
	m_oAri.fA3Color = m_fA3Color;
	m_oAri.fA3HB = m_fA3HB;
	m_oAri.fA4Color = m_fA4Color;
	m_oAri.fA4HB = m_fA4HB;
	
	//add by szy 20150423 begin
    CString szPrinterIP;
    BYTE nField0=199,nField1=99,nField2=99,nField3=1;
    ((CIPAddressCtrl*)GetDlgItem(IDC_PRINTERIP))->GetAddress(nField0,nField1,nField2,nField3);
    szPrinterIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
    strcpy_s((char *)m_oAri.szParam, sizeof(m_oAri.szParam), szPrinterIP);
    if(szPrinterIP.CompareNoCase("0.0.0.0") == 0)
    {
        MessageBox("请输入打印机IP地址！","提示",MB_OK | MB_ICONINFORMATION);
        GetDlgItem(IDC_PRINTERIP)->SetFocus();
        return;
    }
	//add by szy 20150423 end
	
	CFileLog log;
	CString sPath;
	sPath.Format("%s%s\\",CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR);
	
	CCommonFun::CreateDir(sPath);
	sPath += AUTO_PRINTER_DATA_NAME;
	log.Write((BYTE*)&m_oAri,sizeof(m_oAri),sPath);
	HCURSOR waitcursor;
	HCURSOR oldcursor;
	CString szPath = CCommonFun::GetDefaultPath();
	szPath += "res\\wait.ani";
	waitcursor=LoadCursorFromFile(szPath);
	oldcursor=SetCursor(waitcursor);
	UpdateToSvr(m_oAri);
	SetCursor(oldcursor);
	OnOK();
}

BOOL CInstallPrtDlg::UpdateToSvr(CPrinterAtri& oPrint)
{
	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	int nWebPort = 0;

	int nRet = 0;
	CString sLocalIP,
		sHost;
	CString sEquipID;
	EQUIP_INFO equip = {0};
	sLocalIP = CCommonFun::GetLocalIP();
//	sMac = CCommonFun::GetLocalMac();
	sHost = CCommonFun::GetLoaclHost();

	
	CString sIP ,sPort;
	theConfig.LoadPrtStationConfig(sIP,sPort);
	if ((nWebPort = atoi(sPort)) == 0)
	{
		nWebPort = POLICY_TCP_PORT;
	}

	int nSize = sizeof(NET_PACK_HEAD) + max(sizeof(oPrint),sizeof(equip));
	BYTE* pBuf = new BYTE[nSize];
	if (!pBuf)
	{
		return FALSE;
	}
	
	ZeroMemory(pBuf,nSize);
	NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
	pHead->nPackMask = PACK_SMALL;
	pHead->nIndentify = PACKINDENTIFY;
	
	do 
	{
		if(!ServerSocket.Create() && nWebPort == 0)
		{
			ServerSocket.m_bTimeOut = true;
			theLog.Write("服务器套接字创建失败\n");
			//MessageBox("服务器套接字创建失败","提示",MB_OK | MB_ICONINFORMATION);
			break;
		}
		ServerSocket.SetTimeOut(6000);
		if(!ServerSocket.Connect(sIP,nWebPort))//这里只是处理了80端口以后在修改中端口号要从数据报（pBufferSend）中得出
		{
			ServerSocket.m_bTimeOut = true;
			theLog.Write("Connect error");
			break ;
		}
		ServerSocket.KillTimeOut();
		if(ServerSocket.m_bTimeOut)
			break;

		CString sIP;
		UINT nPort;
		ServerSocket.GetSockName(sIP,nPort);
		equip.nEquipType = ISEC_PLT_TOOL;
		strcpy(equip.szIP,sIP);
		strcpy(equip.szHost,sHost);
		theLog.Write("ip=%s",sIP);
		pHead->nMainCmd = WM_CHECK_EQUIP;
		CopyMemory(pHead + 1,&equip,sizeof(equip));
		pHead->nPackBodySize = sizeof(equip);
		if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
		{
			theLog.Write("");
			break;	
		}
		pHead->nMainCmd = WM_PRINT_STATTON_INFO;
		pHead->nSubCmd = REQ_AUTO_ADD_BY_TOOL;
		CopyMemory(pHead + 1,&oPrint,sizeof(oPrint));
		pHead->nPackBodySize = sizeof(oPrint);
		if (ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
		{
			break;
		}
		
		while(1)
		{
			ServerSocket.SetTimeOut(6000);
			int lbuffersize=ServerSocket.Receive(pBuf,nSize);
			ServerSocket.KillTimeOut();
			if(ServerSocket.m_bTimeOut)
				break;
			if(lbuffersize<0)
				break;
		}
		bSuc = TRUE;
	} while (0);
	ServerSocket.Close();
	if (pBuf)
	{
		delete pBuf;
	}
	return bSuc;
}

BOOL CInstallPrtDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	CenterWindow();

	CStringArray ary;
	int nDefault = 0;
	CCommonFun::EnumeratePrinters(ary);
	CString sDefault;
	char szBuf[1024] = {0};
	DWORD dwSize = 1024;
	GetDefaultPrinter(szBuf,&dwSize);
	sDefault = szBuf;
	for (int n = 0; n < ary.GetCount(); n ++)
	{
		if(sDefault.CompareNoCase(ary.GetAt(n)) == 0 )
		{
			nDefault = n;
		}
		m_wndCmbPrinter.AddString(ary.GetAt(n));
	}
	m_wndCmbPrinter.SetCurSel(nDefault);
	OnCbnSelchangeComboPrt();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CInstallPrtDlg::OnCbnSelchangeComboPrt()
{
	int nSel = m_wndCmbPrinter.GetCurSel();
	CString sPrinter;
	m_wndCmbPrinter.GetLBText(nSel,sPrinter);
	HANDLE hPrinter = 0;
	try
	{
		if(OpenPrinter((LPSTR)(LPCTSTR)sPrinter,&hPrinter,0))
		{
			DWORD dwOut = 0;
			GetPrinter(hPrinter,2,0,0,&dwOut);
			DWORD dwSize = dwOut;
			BYTE* pBuf = new BYTE[dwOut];
			if(GetPrinter(hPrinter,2,pBuf,dwSize,&dwOut))
			{
				PRINTER_INFO_2* pInfo = (PRINTER_INFO_2*)pBuf;
				if (pInfo->AveragePPM > 0)
				{
					m_sSpeed.Format("%d",pInfo->AveragePPM);
				}
				if (pInfo->pDevMode && pInfo->pDevMode->dmColor == DMCOLOR_COLOR)
				{
					m_sColor = _T("彩色");
				}
				else
				{
					m_sColor = _T("黑白");
				}
				m_sResolution.Format("%d",pInfo->pDevMode ? pInfo->pDevMode->dmYResolution : 0);
				//add by szy 20150423 begin 选择打印机的同时将打印机ip显示出来，如果没有获取到IP则提示用户手动输入IP
                DWORD dwNeeded=0;
                DWORD dwReturned=0;
                EnumPorts(NULL,2,NULL,0,&dwNeeded,&dwReturned);
                BYTE* pBuf2 = new BYTE[dwNeeded];
                EnumPorts(NULL,2,pBuf2,dwNeeded,&dwNeeded,&dwReturned);
                PORT_INFO_2* p2 = (PORT_INFO_2*) pBuf2;
                CString szPrinterAddrIP="";
                for (DWORD x = 0; x < dwReturned; x++)
                {
                    CString s(p2->pPortName);
                    if (s.CompareNoCase(pInfo->pPortName) == 0)
                    {
                        PORT_TYPE_NET_ATTACHED ;
                        CString sKey;
                        sKey.Format(_T("SYSTEM\\CurrentControlSet\\Control\\Print\\Monitors\\%s\\Ports\\%s"),p2->pDescription,p2->pPortName);

                        theLog.Write("##CPrintChargeByMFP::OnUpdatePrintStation,20,sKey=%s", sKey);
                        HKEY hKey = 0;
                        if(ERROR_SUCCESS ==::RegOpenKeyEx(HKEY_LOCAL_MACHINE,sKey,0,KEY_QUERY_VALUE,&hKey))
                        {
                            DWORD dwType = 0;
                            BYTE szData[50] = {0};
                            DWORD dwLen = 50;
                            if(ERROR_SUCCESS == RegQueryValueEx(hKey,"IPAddress",0,&dwType,szData,&dwLen))
                            {
                                szPrinterAddrIP.Format(_T("%s"),szData);
                                theLog.Write("CPrintChargeByMFP::OnUpdatePrintStation,222,m_sPrinterAddrIP=%s",szPrinterAddrIP);
                            }
                            else
                            {
                                theLog.Write("!!CPrintChargeByMFP::OnUpdatePrintStation,21,RegQueryValueEx fail,err=%d,sKey=%s"
                                    , ::GetLastError(), sKey);
                            }
                            RegCloseKey(hKey);
                        }
                        else
                        {
                            theLog.Write("!!CPrintChargeByMFP::OnUpdatePrintStation,22,RegOpenKeyEx fail,err=%d,sKey=%s"
                                , ::GetLastError(), sKey);
                        }
                        break;
                    }
                    p2++;
                }
                if (!szPrinterAddrIP.IsEmpty())
                {
                    BYTE nField0=0,nField1=0,nField2=0,nField3=0;
                    int Ipoint0,Ipoint1,Ipoint2;
                    Ipoint0=szPrinterAddrIP.Find('.',0);
                    nField0=atoi(szPrinterAddrIP.Mid(0,Ipoint0));
                    Ipoint1=szPrinterAddrIP.Find('.',Ipoint0+1);
                    nField1=atoi(szPrinterAddrIP.Mid(Ipoint0+1,Ipoint1));
                    Ipoint2=szPrinterAddrIP.Find('.',Ipoint1+1);
                    nField2=atoi(szPrinterAddrIP.Mid(Ipoint1+1,Ipoint2));
                    nField3=atoi(szPrinterAddrIP.Mid(Ipoint2+1, szPrinterAddrIP.GetLength()));
                    ((CIPAddressCtrl*)GetDlgItem(IDC_PRINTERIP))->SetAddress(nField0,nField1,nField2,nField3);
                }
                else
                {
                    ((CIPAddressCtrl*)GetDlgItem(IDC_PRINTERIP))->SetAddress(0,0,0,0);
                    //MessageBox("请输入打印机IP地址！","提示",MB_OK | MB_ICONINFORMATION);
                    GetDlgItem(IDC_PRINTERIP)->SetFocus();
                }
			}
			else
			{
				DWORD dw = ::GetLastError();
				theLog.Write("GetPrinter err = %d",dw);
			}
			delete pBuf;
			ClosePrinter(hPrinter);

		}
	}
	catch (...)
	{
		AfxMessageBox("err");
	}
	
	UpdateData(FALSE);
}
