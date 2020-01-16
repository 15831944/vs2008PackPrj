
// UploadBkgImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UploadBkgImage.h"
#include "UploadBkgImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUploadBkgImageDlg �Ի���




CUploadBkgImageDlg::CUploadBkgImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadBkgImageDlg::IDD, pParent)
	, m_szFilePath(_T(""))
	, m_nPort(0)
	, m_szPicSize(_T("800*480"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUploadBkgImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UBT_FILEDIR, m_szFilePath);
	DDV_MaxChars(pDX, m_szFilePath, 1024);
	DDX_Control(pDX, IDC_UBT_COMBO, m_CBPicType);
	DDX_Text(pDX, IDC_UBT_PORT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 0, 65535);
	DDX_Control(pDX, IDC_UBT_UPLOAD, m_BtnUpload);
	DDX_Control(pDX, IDC_UBT_IPADDRESS, m_IPCtrl);
	DDX_Text(pDX, IDC_UBT_PICSIZE, m_szPicSize);
}

BEGIN_MESSAGE_MAP(CUploadBkgImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_UBT_OVERVIEW, &CUploadBkgImageDlg::OnBnClickedUbtOverview)
	ON_BN_CLICKED(IDC_UBT_UPLOAD, &CUploadBkgImageDlg::OnBnClickedUbtUpload)
	ON_CBN_SELCHANGE(IDC_UBT_COMBO, &CUploadBkgImageDlg::OnCbnSelchangeUbtCombo)
END_MESSAGE_MAP()


// CUploadBkgImageDlg ��Ϣ�������

BOOL CUploadBkgImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_CBPicType.InsertString(0, _T("ˢ����¼��ҳ����ͼ"));
	m_CBPicType.InsertString(1, _T("ˢ����¼��ҳLOGO"));
	m_CBPicType.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUploadBkgImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUploadBkgImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUploadBkgImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUploadBkgImageDlg::OnBnClickedUbtOverview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);
	dlg.SetDefExt(".jpg");
	if(dlg.DoModal()==IDOK)
	{
		CString FileName=dlg.GetFileName();//��ȡ�ļ������ڴ��ļ��Ի�����ѡ���ļ�
		CString FileExt = dlg.GetFileExt();//�ļ���׺��
		CString FileFloder = dlg.GetFolderPath();
		//��ȡ��·�����ļ������
		//���ļ�����������б�ؼ�
		m_szFilePath = FileFloder+"\\"+FileName;
		UpdateData(FALSE);
	}
}

void CUploadBkgImageDlg::OnBnClickedUbtUpload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	BYTE nField0=0,nField1=0,nField2=0,nField3=0;
	CString szIP;
	m_IPCtrl.GetAddress(nField0,nField1,nField2,nField3);
	szIP.Format("%d.%d.%d.%d",(int)nField0,(int)nField1,(int)nField2,(int)nField3);
	CString URL;
	if(m_CBPicType.GetCurSel() == 0)
	{
		//URL.Format("http://%s:%d/webservice/pctool/bgImage", szIP, m_nPort);
		URL.Format("http://%s:8080/webservice/pctool/bgImage", szIP);
	}
	else
	{
		//URL.Format("http://%s:%d/webservice/pctool/logoImage", szIP, m_nPort);
		URL.Format("http://%s:8080/webservice/pctool/logoImage", szIP);
	}
	
	if(UploadPic(URL, m_szFilePath))
	{
		MessageBox("�ϴ��ɹ�");
	}
	else
	{
		MessageBox("�ϴ�ʧ��");
	}
	UpdateData(FALSE);
}

BOOL CUploadBkgImageDlg::RequestHttp( CString szHttp, CString &szResponse )
{
	//WriteLogEx("CUploadBkgImageDlg::RequestHttp,1,szHttp=[%s].", szHttp);
	//���������о��飬��һ�ξ��������δ֪����
	for(int i = 0; i < 2; ++i)
	{
		string sRequestHttp = szHttp.GetString();	//�������ҳurl
		string sHeaderSend;			//����httpͷ
		string sHeaderReceive;		//����ͷ
		string sMessage="";			//����ҳ������
		bool IsPost = false;		//�Ƿ�Post�ύ

		int iRet = m_pRequest->SendRequest(IsPost, sRequestHttp, sHeaderSend, sHeaderReceive, sMessage);
		if (iRet)
		{
			szResponse = sMessage.c_str();
			// 			WriteLogEx("CDaShiCardTayg::RequestHttp,2, succ. szResponse=[%s].", szResponse);
			return TRUE;
		}
		else
		{
			//WriteLogEx("!!CUploadBkgImageDlg::RequestHttp,3, SendRequest fail ret=%d", iRet);
			continue;
		}
	}
	//WriteLogEx("!!CUploadBkgImageDlg::RequestHttp,4, fail.");
	return FALSE;
}
BOOL CUploadBkgImageDlg::UploadPic(LPCTSTR strURL, LPCTSTR strLocalFileName)
{
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	CString strServer;
	CString strObject;
	INTERNET_PORT wPort = 0;
	DWORD dwFileLength = 0;
	char * pFileBuff = NULL;

	CHttpConnection * pHC = NULL;
	CHttpFile * pHF = NULL;
	CInternetSession cis;

	bResult = AfxParseURL(strURL, dwType, strServer, strObject, wPort);
	if(!bResult)
		return FALSE;
	CFile file;
	try
	{
		if(!file.Open(strLocalFileName, CFile::shareDenyNone | CFile::modeRead))
			return FALSE;
		dwFileLength = file.GetLength();
		if(dwFileLength <= 0)
			return FALSE;
		pFileBuff = new char[dwFileLength];
		memset(pFileBuff, 0, sizeof(char) * dwFileLength);
		file.Read(pFileBuff, dwFileLength);

		const int nTimeOut = 5000;
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //���ӳ�ʱ����
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); //����1��
		pHC = cis.GetHttpConnection(strServer, wPort); //ȡ��һ��Http����

		pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
		if(!pHF->SendRequest(NULL, 0, pFileBuff, dwFileLength))
		{
			delete[]pFileBuff;
			pFileBuff = NULL;
			pHF->Close();
			pHC->Close();
			cis.Close();
			return FALSE;
		}
		DWORD dwStateCode = 0;
		pHF->QueryInfoStatusCode(dwStateCode);

		if(dwStateCode == HTTP_STATUS_OK)
			bResult = TRUE;
	}

	catch(CInternetException * pEx)
	{
		char sz[256] = "";
		pEx->GetErrorMessage(sz, 25);
		CString str;
		str.Format("InternetException occur!\r\n%s", sz);
		AfxMessageBox(str);
		bResult = FALSE;
	}
	catch(CFileException& fe)
	{
		CString str;
		str.Format("FileException occur!\r\n%d", fe.m_lOsError);
		AfxMessageBox(str);
		bResult = FALSE;
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		CString str;
		str.Format("Unknow Exception occur!\r\n%d", dwError);
		AfxMessageBox(str);
		bResult = FALSE;
	}

	delete[]pFileBuff;
	pFileBuff = NULL;
	file.Close();
	pHF->Close();
	pHC->Close();
	cis.Close();
	return bResult;
}

void CUploadBkgImageDlg::OnCbnSelchangeUbtCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_CBPicType.GetCurSel() == 0)
	{
		m_szPicSize = "800*480";
	}
	else if(m_CBPicType.GetCurSel() == 1)
	{
		m_szPicSize = "200*100����";
	}
	UpdateData(FALSE);
}
