// EmbedWebPageTwo.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "BaseSock.h"
#include "EmbedWebSheet.h"
#include "EmbedWebPageTwo.h"


// CEmbedWebPageTwo 对话框

IMPLEMENT_DYNAMIC(CEmbedWebPageTwo, CPropertyPage)

CEmbedWebPageTwo::CEmbedWebPageTwo()
	: CPropertyPage(CEmbedWebPageTwo::IDD)
{
    m_szWebPort = _T("80");
    m_bInitSuc = false;
    m_szVirtualPath = "";
    m_szNewWebName = PRINT_WEB_SITE;
    m_szNewWebIp = CCommonFun::GetLocalIP();
    m_szNewWebPort = _T("8021");
    
    ::CoInitialize(NULL);
    m_szVirtualPath = CCommonFun::GetDefaultPath();
    m_szVirtualPath.TrimRight("\\");
    int nFind = m_szVirtualPath.ReverseFind('\\');
    if(nFind>=0)
    {
        m_szVirtualPath = m_szVirtualPath.Left(nFind);
    }
}

CEmbedWebPageTwo::~CEmbedWebPageTwo()
{
    m_Web.RemoveAllList(m_WebList);
    ::CoUninitialize();
}

void CEmbedWebPageTwo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ZHAN, m_ctWeb);
    DDX_Text(pDX, IDC_EDIT_WPORT, m_szWebPort);
    DDX_Text(pDX, IDC_EDIT_WEB_NAME, m_szNewWebName);
    DDX_Text(pDX, IDC_EDIT_WEB_IP, m_szNewWebIp);
    DDX_Text(pDX, IDC_EDIT_WEB_PORT, m_szNewWebPort);
}


BEGIN_MESSAGE_MAP(CEmbedWebPageTwo, CPropertyPage)
    ON_CBN_SELCHANGE(IDC_COMBO_ZHAN, OnSelchangeComboZhan)
    ON_BN_CLICKED(IDC_TEST, OnTest)
    ON_BN_CLICKED(IDC_CHECK_NEW_WEB, &CEmbedWebPageTwo::OnBnClickedCheckNewWeb)
END_MESSAGE_MAP()


// CEmbedWebPageTwo 消息处理程序
BOOL CEmbedWebPageTwo::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();
    CPropertySheet *pMyWizard = (CPropertySheet *)GetParent();
    pMyWizard->SetWindowText("Web站点设置");

    theLog.EnableLog(TRUE);	//add by zfq,2012.12.11
    // TODO: Add extra initialization here
    if(!InitWebSiteInfo())
    {
        theLog.Write("!!CEmbedWebPageTwo::OnInitDialog,1,InitWebSiteInfo fail,err=%d", GetLastError());
        MessageBox("初始化Web站点失败！","提示",MB_OK | MB_ICONINFORMATION);
    }

    ((CButton *)GetDlgItem(IDC_CHECK_NEW_WEB))->SetCheck(FALSE);
    EnableAddNewWebSite(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEmbedWebPageTwo::OnSetActive() 
{
    // TODO: Add your specialized code here and/or call the base class
    CPropertySheet *pMyWizard = (CPropertySheet *)GetParent();

    DWORD dwMask = 0;
    if (pMyWizard->GetPageCount() > 2)
    {
        dwMask = PSWIZB_NEXT;

    }
    else
    {
        dwMask = PSWIZB_FINISH;
    }
    dwMask |= PSWIZB_BACK;

    pMyWizard->SetWizardButtons(dwMask);

    return CPropertyPage::OnSetActive();
}

LRESULT CEmbedWebPageTwo::OnWizardNext() 
{
    // TODO: Add your specialized code here and/or call the base class
    if(!FinishWebSiteSet())
    {
        theLog.Write("!!CEmbedWebPageTwo::OnWizardNext,1,FinishWebSiteSet fail,err=%d", GetLastError());
        return 1;
    }

    theLog.Write("##CEmbedWebPageTwo::OnWizardNext,FinishWebSiteSet OK");
    return CPropertyPage::OnWizardNext();
}

BOOL CEmbedWebPageTwo::OnWizardFinish() 
{
    // TODO: Add your specialized code here and/or call the base class
    if(!FinishWebSiteSet())
    {
        theLog.Write("!!CEmbedWebPageTwo::OnWizardFinish,1,FinishWebSiteSet fail,err=%d", GetLastError());
        return FALSE;
    }

    return CPropertyPage::OnWizardFinish();
}

BOOL CEmbedWebPageTwo::InitWebSiteInfo()
{
    try
    {
        m_Web.GetWebSiteList(&m_WebList);
        POSITION pos = NULL;
        bool bFirst = true;
        CString str = "";
        pos = m_WebList.GetHeadPosition();
        if(!pos)
        {
            theLog.Write("!!CEmbedWebPageTwo::InitWebSiteInfo,1,pos=%p", pos);
        }

        while(pos)
        {
            S_SITEINFO* pInfo = (S_SITEINFO*)m_WebList.GetNext(pos);
            str.Format("nNumber:%d   szPort:%s  szIP:%s  szComment:%s   szVPath:%s\n",
                pInfo->nNumber,pInfo->szPort,pInfo->szIP,pInfo->szComment,pInfo->szVPath);

            CString sTemp = pInfo->szComment;
            if(bFirst)
            {
                bFirst = false;
                m_szWebPort.Format("%s",pInfo->szPort);
            }
            int nCount = m_ctWeb.GetCount();
            m_ctWeb.AddString(sTemp);
            m_ctWeb.SetItemData(nCount,(DWORD)pInfo);
        }
        int nCount = m_ctWeb.GetCount();

        if(nCount>0)
        {
            m_ctWeb.SetCurSel(0);
            UpdateData(false);

        }

        theLog.Write("##CEmbedWebPageTwo::InitWebSiteInfo,2,nCount=%d,bFirst=%d", nCount, bFirst);

        if(bFirst)
        {
            m_bInitSuc = false;
            m_ctWeb.EnableWindow(false);
        }
        else
            m_bInitSuc = true;
    }
    catch(...)
    {
        theLog.Write("!!CEmbedWebPageTwo::InitWebSiteInfo,11,catch sth.err=%d", GetLastError());
        return FALSE;
    }
    return TRUE;
}

BOOL CEmbedWebPageTwo::FinishWebSiteSet(BOOL bQueit)
{
    S_SITEINFO* pWebInfo = NULL;
    Enable32BitAppOnWin64(true);
    MyExecuteCmdComd();

    if(bQueit)
    {
        m_Web.GetWebSiteList(&m_WebList);
        POSITION pos = NULL;

        CString str = "";
        pos = m_WebList.GetHeadPosition();
        if(!pos)
        {
            theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,2,pos=%p", pos);
        }
        while(pos)
        {
            S_SITEINFO* pInfo = (S_SITEINFO*)m_WebList.GetNext(pos);
            str.Format("nNumber:%d   szPort:%s  szIP:%s  szComment:%s   szVPath:%s\n",
                pInfo->nNumber,pInfo->szPort,pInfo->szIP,pInfo->szComment,pInfo->szVPath);
            CString sTemp = pInfo->szComment;
            if(pInfo)
            {
                m_bInitSuc = true;
                pWebInfo = pInfo;
                break;
            }
        }
        if (!pWebInfo)
        {
            theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,2,pWebInfo=%p", pWebInfo);
            return FALSE;
        }
        int nNum = pWebInfo->nNumber;
        StartWebServer(nNum);
        BOOL bRet = m_Web.CreateVirtualWebDir(nNum,m_szVirtualPath, m_szVirtaulName);
        if(!bRet)
        {
            theLog.Write("创建虚拟目录失败!","提示");
            theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,3,CreateVirtualWebDir fail,m_szVirtualPath=%s,nNum=%d,err=%d"
                , m_szVirtualPath, nNum, GetLastError());
            return FALSE;
        }
        else
        {
            StartWebServer(nNum);
            theLog.Write("设置Web站点成功！","提示");
        }

        CEmbedWebSheet *pSheet = (CEmbedWebSheet *)GetParent();
        if (pSheet)
        {
            pSheet->m_bSuc = TRUE;
            pSheet->WriteTheURL();
        }
        if(m_szVirtaulName.CompareNoCase("TOSHIBAPrint") == 0)
            m_Web.CreateEmbededWebLnk("", "80", m_szVirtaulName);
        //m_Web.SetWebSiteDefaultDoc(nNum,"Default.aspx");

        if(!MyStartAspnet_StateService())
        {
            theLog.Write("设置asp_state失败！","提示");
            theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,5,MyStartAspnet_StateService fail,m_szVirtualPath=%s,nNum=%d,err=%d"
                , m_szVirtualPath, nNum, GetLastError());
            return FALSE;
        }
    }
    else
    {
        if(!m_bInitSuc)
        {
            MessageBox("请确认是否正确安装Web服务器!","提示",MB_OK | MB_ICONINFORMATION);

            theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,11,m_bInitSuc=%d,m_szVirtualPath=%s,err=%d"
                , m_bInitSuc, m_szVirtualPath, GetLastError());
            return FALSE;
        }
        UpdateData();

        //添加新的打印web站点，可以配置端口
        if (IsCheck(IDC_CHECK_NEW_WEB))
        {
            if(m_szNewWebName == "")
            {
                MessageBox("Web站点名字不能为空!","提示",MB_OK | MB_ICONINFORMATION);
                return FALSE;
            }

            if(m_szNewWebIp == "")
            {
                MessageBox("Web站点的IP不能为空!","提示",MB_OK | MB_ICONINFORMATION);
                return FALSE;
            }

            if(m_szNewWebPort == "")
            {
                MessageBox("Web站点端口不能为空!","提示",MB_OK | MB_ICONINFORMATION);
                return FALSE;
            }
            g_oIniFileClientSetup.SetVal("WebConfig","CreateNewWeb",1);
            g_oIniFileClientSetup.SetVal("ClientConfig","SvrIP",m_szNewWebIp);
            g_oIniFileClientSetup.SetVal("ClientConfig","urlport",m_szNewWebPort);

            //创建一个新的打印web
            BOOL bRet = m_Web.CreateWebSite(m_szNewWebName,m_szVirtaulName,m_szVirtualPath,atoi(m_szNewWebPort),FALSE);
            if(!bRet)
            {
                MessageBox("创建Web站点失败!","提示",MB_OK | MB_ICONINFORMATION);
                theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,13,CreateVirtualWebDir fail,m_szVirtualPath=%s,m_szNewWebPort=%s,err=%d"
                    , m_szVirtualPath, m_szNewWebPort, GetLastError());
                return FALSE;
            }
            else
            {
                MessageBox("创建Web站点成功！","提示",MB_OK | MB_ICONINFORMATION);
            }
            CEmbedWebSheet *pSheet = (CEmbedWebSheet *)GetParent();
            if (pSheet)
            {
                pSheet->m_bSuc = TRUE;
                pSheet->WriteTheURL(FALSE, m_szNewWebIp, m_szNewWebPort);
            }

            //m_Web.CreateAdminLnk(m_szNewWebIp, m_szNewWebPort);
        }
        //使用默认的打印点，无法配置端口，只能使用默认的配置
        else
        {
            g_oIniFileClientSetup.SetVal("WebConfig","CreateNewWeb",0);
            g_oIniFileClientSetup.SetVal("ClientConfig","SvrIP",CCommonFun::GetLocalIP());
            g_oIniFileClientSetup.SetVal("ClientConfig","urlport",80);

            int nIndex = m_ctWeb.GetCurSel();
            if(nIndex == CB_ERR)
            {
                theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,12,nIndex=%d", nIndex);
                return FALSE;
            }

            pWebInfo = (S_SITEINFO*)m_ctWeb.GetItemData(nIndex);
            int nNum = pWebInfo->nNumber;
            StartWebServer(nNum);
            BOOL bRet = m_Web.CreateVirtualWebDir(nNum,m_szVirtualPath,m_szVirtaulName);
            if(!bRet)
            {
                MessageBox("创建虚拟目录失败!","提示",MB_OK | MB_ICONINFORMATION);
                theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,13,CreateVirtualWebDir fail,m_szVirtualPath=%s,nNum=%d,err=%d"
                    , m_szVirtualPath, nNum, GetLastError());

                return FALSE;
            }
            else
            {
                StartWebServer(nNum);
                MessageBox("设置Web站点成功！","提示",MB_OK | MB_ICONINFORMATION);
            }
            CEmbedWebSheet *pSheet = (CEmbedWebSheet *)GetParent();
            if (pSheet)
            {
                pSheet->m_bSuc = TRUE;
                pSheet->WriteTheURL();
            }
            if(m_szVirtaulName.CompareNoCase("TOSHIBAPrint") == 0)
                m_Web.CreateEmbededWebLnk("", "80", m_szVirtaulName);
            //m_Web.SetWebSiteDefaultDoc(nNum,"Default.aspx");

        }

        if(!MyStartAspnet_StateService())
        {
            MessageBox("设置asp_state失败！","提示",MB_OK | MB_ICONINFORMATION);

            theLog.Write("!!CEmbedWebPageTwo::FinishWebSiteSet,14,MyStartAspnet_StateService fail,m_szVirtualPath=%s,err=%d"
                , m_szVirtualPath, GetLastError());

            return FALSE;
        }

    }


    return TRUE;
}

void CEmbedWebPageTwo::OnSelchangeComboZhan() 
{
    // TODO: Add your control notification handler code here
    int nIndex = m_ctWeb.GetCurSel();
    if(nIndex == CB_ERR)
        return ;

    ShowWebPara(nIndex);
}

void CEmbedWebPageTwo::ShowWebPara(int index)
{
    S_SITEINFO* pInfo = (S_SITEINFO*)m_ctWeb.GetItemData(index);

    if(pInfo)
    {
        m_szWebPort.Format("%s",pInfo->szPort);
        UpdateData(false);
    }
}

void CEmbedWebPageTwo::OnTest() 
{
    // TODO: Add your control notification handler code here
    OnTestIIS();
}

void CEmbedWebPageTwo::OnTestIIS()
{
    UpdateData();
    int inum=0;
    int iretsize;
    char pBufferSend[MAX_PATH];
    CBaseSock    ServerSocket;
    int nWebPort = 0;
    nWebPort = atoi(m_szWebPort);

#if 1 //delete by zfq,2012.12.11
    if(!ServerSocket.Create() && nWebPort == 0)
#else
    if(!ServerSocket.Create() || nWebPort == 0)
#endif
    {
        ServerSocket.m_bTimeOut = true;
        TRACE("服务器套接字创建失败\n");
        theLog.Write("!!CEmbedWebPageTwo::OnTestIIS,1,err=%d,nWebPort=%d", GetLastError(), nWebPort);
        MessageBox("服务器套接字创建失败","提示",MB_OK | MB_ICONINFORMATION);
        goto EXIT;
    }
    ServerSocket.SetTimeOut(6000);
    if(!ServerSocket.Connect("127.0.0.1",nWebPort))//这里只是处理了80端口以后在修改中端口号要从数据报（pBufferSend）中得出
    {
        ServerSocket.m_bTimeOut = true;
        theLog.Write("!!CEmbedWebPageTwo::OnTestIIS,2,err=%d,nWebPort=%d", GetLastError(), nWebPort);
        MessageBox("Connect error");
        goto EXIT; ;
    }
    ServerSocket.KillTimeOut();

    if(ServerSocket.m_bTimeOut)
    {
        theLog.Write("!!CEmbedWebPageTwo::OnTestIIS,3,err=%d,nWebPort=%d", GetLastError(), nWebPort);
        goto EXIT;
    }
    MakeHttpCon(pBufferSend,MAX_PATH);
    iretsize= ServerSocket.Send(pBufferSend,strlen(pBufferSend));

    while(1)
    {
        ZeroMemory(pBufferSend,MAX_PATH);
        ServerSocket.SetTimeOut(6000);
        int lbuffersize=ServerSocket.Receive(pBufferSend,MAX_PATH);
        ServerSocket.KillTimeOut();
        if(ServerSocket.m_bTimeOut)
        {
            theLog.Write("!!CEmbedWebPageTwo::OnTestIIS,4,err=%d,nWebPort=%d", GetLastError(), nWebPort);
            goto EXIT;
        }


        TRACE(pBufferSend);
        if(lbuffersize==0) 
        {
            int error = GetLastError();
            TRACE("从SERVER接收信息结束");
            break;
        }
        if(lbuffersize<0)
            break;	 
    }
    ServerSocket.Close();
EXIT:
    if(ServerSocket.m_bTimeOut)
        MessageBox("Web服务器工作不正常，安装Web服务器可能不成功!","提示",MB_OK | MB_ICONINFORMATION);
    else
        MessageBox("Web服务器工作正常!","提示",MB_OK | MB_ICONINFORMATION);

}

void CEmbedWebPageTwo::MakeHttpCon(char *cinfo, int imax)
{
    ZeroMemory(cinfo,imax);
    strcpy(cinfo, "GET http://127.0.0.1 HTTP/1.0\r\n"); 
    strcat(cinfo,"Referer: http://127.0.0.1\r\n");
    strcat(cinfo,"Accept-Language: zh-cn\r\n");
    strcat(cinfo,"Accept-Encoding: gzip, deflate\r\n");
    strcat(cinfo,"User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n");
    strcat(cinfo,"Host: 127.0.0.1\r\n");
    strcat(cinfo,"Proxy-Connection: Keep-Aliv\r\n");
    strcat(cinfo,"Pragma: no-cache\r\n\r\n");
}


void CEmbedWebPageTwo::StartWebServer(int nNum)
{
    CString szDefaultPath = CCommonFun::GetDefaultPath();
    szDefaultPath.Format(_T("%sreg.vbs"),CCommonFun::GetDefaultPath());
    char cShortPath[300];
    GetShortPathName(szDefaultPath,cShortPath,300);

    CString szPath;
    szPath.Format("CSCRIPT.EXE %s -c localhost -a %d",cShortPath,nNum);

    STARTUPINFO StartupInfo= {0};
    StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
    StartupInfo.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION ProcessInfo;
    StartupInfo.cb=sizeof(STARTUPINFO);	

    if (!CreateProcess(NULL, szPath.GetBuffer(szPath.GetLength()), NULL,NULL,FALSE,0,NULL,NULL, &StartupInfo, &ProcessInfo))
    {
        theLog.Write("create web start process error");
        theLog.Write("!!CEmbedWebPageTwo::StartWebServer,4,CreateProcess fail,err=%d,szPath=%s", GetLastError(), szPath);
        return ;
    }
    szPath.ReleaseBuffer();
    WaitForSingleObject(ProcessInfo.hProcess ,5000);
    CloseHandle (ProcessInfo.hThread);
    CloseHandle (ProcessInfo.hProcess);	
}

BOOL CEmbedWebPageTwo::IsCheck(int uID)
{
    CButton * pButton;
    pButton=(CButton *)GetDlgItem(uID);
    int state=pButton->GetCheck();
    if(state)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CEmbedWebPageTwo::OnBnClickedCheckNewWeb()
{
    BOOL bEnable = IsCheck(IDC_CHECK_NEW_WEB);
    EnableAddNewWebSite(bEnable);
}

void CEmbedWebPageTwo::EnableAddNewWebSite(BOOL bEanble)
{
    GetDlgItem(IDC_STATIC_NEW)->EnableWindow(bEanble);
    GetDlgItem(IDC_STATIC_NAME)->EnableWindow(bEanble);
    GetDlgItem(IDC_STATIC_IP)->EnableWindow(bEanble);
    GetDlgItem(IDC_STATIC_PORT)->EnableWindow(bEanble);
    GetDlgItem(IDC_EDIT_WEB_NAME)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_WEB_IP)->EnableWindow(bEanble);
    GetDlgItem(IDC_EDIT_WEB_PORT)->EnableWindow(bEanble);
}

