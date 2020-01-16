
// Install.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Install.h"
#include "InstallDlg.h"
#include "WebSheet.h"
#include "CreateDbDlg.h"
#include "setipdlg.h"
#include "SetDbDlg.h" 
#include "InstallPrtDlg.h"
#include "EncryptDlg.h"
#include <Tlhelp32.h>
#include <atlsecurity.h>
#include "NtDriverController.h"
#include "ConfigPrtDlg.h"
#include "CeleritySettingDlg.h"
#include "ExportDataDlg.h"
#include "setuptipdlg.h"
#include "Registry2.h"	//add by zfq,2013.01.06
#include "DlgCfgPrtCenterSvr.h"	//add by zfq,2013.02.16
#include "ConfigEmbedWeb.h"//add by szy 20150421
#include "DlgCreateAppDB.h"
#include "EmbedWebSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL AddObjectAccess(CString strFileName, const CSid &rSid, ACCESS_MASK accessmask,SE_OBJECT_TYPE eType /*= SE_OBJECT_TYPE*/);
BOOL AddRegAccess(CString szRegPath);


// CInstallApp

BEGIN_MESSAGE_MAP(CInstallApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

BOOL IsWow64()
{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")),"IsWow64Process");

	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		//AfxMessageBox(_T("aa"));
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
			//AfxMessageBox(_T("errr"));
		}
	}
	return bIsWow64;
}
// CInstallApp ����

CInstallApp::CInstallApp()
{
	m_nExitCode = 0;
	m_bQueit = 0;
}


// Ψһ��һ�� CInstallApp ����

CInstallApp theApp;
CFileLog theLog;
CConfigSettingHelper theConfig;
CIniFile g_oSetupConfig;
CIniFile g_oIniFileConfig;
CIniFile g_oIniFileSet;
CIniFile g_oIniFileCurretnConfig;	//config_X.ini
CIniFile g_oIniFileClientSetup;
// CInstallApp ��ʼ��


BOOL CInstallApp::InitInstance()
{
	CFileLog::EnableLog(TRUE);
	theLog.SetLogName("install");

	
// 	CString sPath;
// 	sPath.Format(_T("%s"),"MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\XabVPrinter");
// 
// 	AddObjectAccess(sPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_REGISTRY_KEY);


	m_sDefaultPath = CCommonFun::GetDefaultPath();
	//------------------
	//��װ����·��
	CString sKey = PRODUCT_REGISTY_KEY;
	CRegistryEx reg2;
	reg2.SetRootKey(HKEY_LOCAL_MACHINE);
	if(!reg2.Open(sKey))
	{
		reg2.SetRootKey(HKEY_LOCAL_MACHINE);
		if (!reg2.CreateKey(PRODUCT_REGISTY_KEY))
			theLog.Write("CreateKey %s faile ",PRODUCT_REGISTY_KEY);
	}
	reg2.Read("SourcePath",m_sInstallSourcePath);

	//add by zfq,2013.01.14,begin
	CString szWebInstallPath;
#if 0
	szWebInstallPath.Format("%sWebStation", m_sDefaultPath);
	int nIdx = m_sDefaultPath.Find("bin\\WebStation");
	if(0 < nIdx)
	{
		reg2.Write("WebInstallPath", szWebInstallPath);
	}
#else
	CString szDefWebPath;
	CString szDefPath;
	CString szNotUse = "\\bin\\";
	int nIdx = m_sDefaultPath.Find(szNotUse);
	if(0 < nIdx)
	{//˵����web��װʱ��·��������install.exe��ȡ�İ�װ·��
		int nUseLen = m_sDefaultPath.GetLength() - szNotUse.GetLength();
		szDefWebPath = m_sDefaultPath.Left(nUseLen);
		reg2.Write("WebInstallPath", szDefWebPath);
	}
	else
	{//˵���������̰�װʱ��ȡ��·��
		if(0 < m_sDefaultPath.Find("Print5.0"))
		{
			szDefWebPath.Format("%sWebStation", m_sDefaultPath);
			reg2.Write("WebInstallPath", szDefWebPath);
		}
	}	
#endif
	//add by zfq,2013.01.14,end

	reg2.Close();
	m_sInstallSourcePath.TrimRight("\\");

	// ����1����װ��ɺ�ע�����SourcePath��ֵΪ�գ�ԭ��δ֪����װ������û���⡣
	// ����2���޸�ʱ�ļ����ܺͰ�װʱ·����ͬ����ȡע���ʽҲ����ȡ�� qilu@2011-12-19 18:32
	if (m_sInstallSourcePath.IsEmpty())
	{
		m_sInstallSourcePath = m_sDefaultPath;
		m_sInstallSourcePath.TrimRight("\\");
	}

	theLog.Write("m_sInstallSourcePath = %s",m_sInstallSourcePath);
	
	//��ʼ�������ļ�·��

	g_oSetupConfig.m_sPath.Format(_T("%s\\setup_config.ini"),m_sInstallSourcePath);
	m_bQueit = g_oSetupConfig.GetVal("Option","quiet",0);
	theLog.Write("queit=%d",m_bQueit);

	g_oIniFileConfig.m_sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
	g_oIniFileSet.m_sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_SETTINGS_FILE);
	g_oIniFileCurretnConfig.m_sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_FILE);
	g_oIniFileClientSetup.m_sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_ClientSetup_FILE);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	::CoInitialize(NULL);
	if (!AfxSocketInit())
	{
		return FALSE;
	}
	//��������
	if(ParseParam())
	{
		return FALSE;
	}
#if 0
	{
		CExportDataDlg a;
		a.DoModal();
		CConfigPrtDlg c;
		c.DoModal();
		CInstallPrtDlg dlg;
		dlg.DoModal();
	}
#endif

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
#ifdef _DEBUG
	CInstallDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
#endif
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

CString LoadS(UINT nID)
{
	CString s;
	s.LoadString(nID);
	return s;
}

void RemoveUsers( ATL::CDacl &pDacl)
{
	for(UINT i = 0; i < pDacl.GetAceCount(); i++)
	{
		ATL::CSid pSid;
		ACCESS_MASK pMask = 0;
		BYTE pType = 0, pFlags = 0;
		/* Get the ith ACL */

		const_cast<ATL::CDacl &>(pDacl).GetAclEntry(i, &pSid, &pMask, &pType, &pFlags);

		CString str(pSid.AccountName());

		if (str.CompareNoCase("Users") == 0 )
		{
			pDacl.RemoveAce(i);
		}
	}
}
BOOL AddRegAccess(CString szRegPath)
{
	DWORD dwRet;

	// ��������ַ�����ֵ�޸�Ϊ��Ҫ����Ȩ�޲�����ע����ע���ÿһ����Ȩ���ǲ�һ���ģ�������Ҫ�ܾ����ָ����ĳһ��
	char pReg[100] = {0};
	sprintf_s(pReg, 100, "MACHINE\\%s", szRegPath);
	theLog.Write("AddRegAccess,1,pReg=[%s]", pReg);

	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pOldDacl = NULL;
	PACL pNewDacl = NULL;
	EXPLICIT_ACCESS ea;
	HKEY hKey = NULL;

	// ��ȡSAM������DACL
	dwRet = GetNamedSecurityInfo(pReg, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION,
		NULL, NULL, &pOldDacl, NULL, &pSD);
	if (dwRet != ERROR_SUCCESS)
	{
		theLog.Write("!!AddRegAccess,2,GetNamedSecurityInfo Error: %d", dwRet);
		goto FreeAndExit;
	}

	// ����һ��ACE������Everyone��ȫ���ƶ��󣬲������Ӷ���̳д�Ȩ��
	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	BuildExplicitAccessWithName(&ea, "Everyone", KEY_ALL_ACCESS, SET_ACCESS,
		SUB_CONTAINERS_AND_OBJECTS_INHERIT);

	// ���µ�ACE����DACL
	dwRet = SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl);
	if (dwRet != ERROR_SUCCESS)
	{
		theLog.Write("!!AddRegAccess,3,SetEntriesInAcl Error: %d", dwRet);
		goto FreeAndExit;
	}

	// ����SAM������DACL
	dwRet = SetNamedSecurityInfo(pReg, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION,
		NULL, NULL, pNewDacl, NULL);
	if (dwRet != ERROR_SUCCESS)
	{
		theLog.Write("!!AddRegAccess,4,SetNamedSecurityInfo Error: %d", dwRet);
		goto FreeAndExit;
	}

	theLog.Write("!!AddRegAccess,5,SetNamedSecurityInfo Successfully.");

	return TRUE;

FreeAndExit:
	if (hKey) RegCloseKey(hKey);
	if (pNewDacl) LocalFree(pNewDacl);
	// ��ԭSAM������DACL
	if (pOldDacl) SetNamedSecurityInfo(pReg, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION,
		NULL, NULL, pOldDacl, NULL);
	if (pSD) LocalFree(pSD);
	return FALSE;
}

BOOL AddObjectAccess(CString strFileName, const CSid &rSid, ACCESS_MASK accessmask,SE_OBJECT_TYPE eType /*= SE_OBJECT_TYPE*/)
{
	ATL::CSecurityDesc OutSecDesc;
	ATL::AtlGetSecurityDescriptor(strFileName, eType, &OutSecDesc);
	OutSecDesc.MakeAbsolute();
	bool pbPresent = 0;

	ATL::CDacl pDacl;
	if (!OutSecDesc.GetDacl(&pDacl, &pbPresent))
		return FALSE;

	RemoveUsers(pDacl);

	if (!pDacl.AddAllowedAce(rSid, accessmask,  CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE))
	{
		//�Ѿ������е�ɾ���ˣ�
		return FALSE;
	}

	OutSecDesc.SetControl(SE_DACL_AUTO_INHERITED | SE_DACL_PROTECTED, SE_DACL_AUTO_INHERITED);
	ATL::AtlSetDacl(strFileName, eType, pDacl);
	ATL::CSacl pSacl;
	/* We've already set the Dacl. Now set the SACL. */
	OutSecDesc.GetSacl(&pSacl, &pbPresent);
	if(pbPresent)
	{
		ATL::AtlSetSacl(strFileName, eType, pSacl);
	}
	return TRUE;
}
void Install(CString sPath,CString sParam,DWORD dwWait /* = TRUE*/,DWORD dwShowCmd)
{
	int nLen = sPath.GetLength() + sParam.GetLength() + 10;
	char* pszPath = new char[nLen];
	ASSERT(pszPath);
	memset(pszPath,0,nLen);

	sprintf(pszPath,"\"%s\" %s",sPath,sParam);
	STARTUPINFO si = { sizeof si, 0, "" };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = dwShowCmd;

	theLog.Write("Install pathandParam = %s",pszPath);
	PROCESS_INFORMATION pi;
	if(CreateProcess(0, 
		pszPath,// command line
		0, 0,  // security attributes
		FALSE, // handle inheritance
		0,     // creation flags
		0,     // environment block
		0,     // current directory
		&si, &pi ))
	{
		theLog.Write("createprocess pid = %d",pi.dwProcessId);
		//	if(bWait)
		{
			::WaitForSingleObject(pi.hProcess,dwWait);
		}
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
	else
	{
		CString s;
		s.Format("Install ʧ�� err = %d \n",::GetLastError());
		theLog.Write(s);
	}
	if (pszPath)
	{
		delete pszPath;
		pszPath = NULL;
	}
}
int FindTerminateProcess(CString strProcessName)
{
	DWORD dwRet = 0;

	//����
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hSnapshot != INVALID_HANDLE_VALUE)
	{
		strProcessName.MakeUpper();
		PROCESSENTRY32 ProcessEntry;  
		ZeroMemory(&ProcessEntry,sizeof(PROCESSENTRY32));
		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		BOOL bFlag = Process32First(hSnapshot,&ProcessEntry);
		while(bFlag)
		{
			CString strExePath
				= _T(ProcessEntry.szExeFile);
			strExePath.MakeUpper();
			if(strExePath.Find(strProcessName) != -1)
			{
				dwRet = ProcessEntry.th32ProcessID;
				break;
			}
			ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
			bFlag = Process32Next(hSnapshot,&ProcessEntry);
		}
		CloseHandle(hSnapshot);
	}
	if (dwRet == 0)
		return 0;

	//�ս�
	//����Ȩ��
	CCommonFun::ModifyPrivilege(SE_DEBUG_NAME,TRUE);
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwRet);
	if (hProcess == NULL)
		return 0;
	dwRet = TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	return dwRet ;
}

void CInstallApp::SetErrCode(int nCode,CString sErr)
{
	m_nExitCode = nCode;
	theLog.Write("ErrCode = %d,%s",nCode,sErr);
}

//add by zfq,2013.01.06,��ǰ�û��Ƿ������˻���¼�����
BOOL IsLogonByDomainUser()
{
	CRegistry2 oReg(0, TRUE);
	CString szRegPath = "Volatile Environment";
	if(!oReg.Open(szRegPath))
	{
		theLog.Write("!!IsLogonByDomainUser,1,Open FAIL,err=%d,szRegPath=%s", GetLastError(), szRegPath);
		return FALSE;
	}

	CString szValue;
	if(!oReg.Read("USERDNSDOMAIN", szValue))
	{
		theLog.Write("!!IsLogonByDomainUser,2,read FAIL,err=%d,szRegPath=%s\\USERDNSDOMAIN", GetLastError(), szRegPath);
		return FALSE;
	}

	if(szValue.IsEmpty())
	{
		return FALSE;
	}

	return TRUE;
}

int CInstallApp::ParseParam(void)
{
//	theLog.Write("##CInstallApp::ParseParam,1,__argc=%d", __argc);
	for(int n = 0; n < __argc; n ++)
	{
		CString s;
		s.Format("CInstallApp��������:%s",__argv[n]);
		theLog.Write(s);
	}

	if(__argc < 2)
		return FALSE;

	theLog.Write("##CInstallApp::ParseParam,2,__argv[1]=%s", __argv[1]);

	CDrvInstallHelper helper;
	//��װ�����ӡ��
	if(stricmp(__argv[1],"-InstallPrinter") == 0 || stricmp(__argv[1],"/InstallPrinter") == 0)
	{
		theLog.Write("CInstallApp::ParseParam,InstallPrinter,1");

		bool bRet = helper.CallFun("InstallPrinter");
		
		theLog.Write("CInstallApp::ParseParam,InstallPrinter,2,bRet=%d", bRet);
		return TRUE;
	}
	else if(stricmp(__argv[1],"-UnInstallPrinter") == 0 || stricmp(__argv[1],"/UnInstallPrinter") == 0)
	{
		helper.CallFun("UnInstallPrinter");
		return TRUE;
	}
	else if(stricmp(__argv[1],"-ConfigPrinter") == 0 || stricmp(__argv[1],"/ConfigPrinter") == 0)
	{
		helper.CallFun("ConfigPrinter");
		return TRUE;
	}
	else if(stricmp(__argv[1],"-ExportSYX") == 0 || stricmp(__argv[1],"/ExportSYX") == 0)
	{
		CExportDataDlg dlg;
		dlg.DoModal();
		return TRUE;
	}
	else if(stricmp(__argv[1],"-demover") == 0 || stricmp(__argv[1],"/demover") == 0)
	{
		theLog.Write("demover --- begin %d",::GetTickCount());

		CSetupTipMgr::GetInstance().Open();
//		CString sDispName = _T("�Ű�֮��iSecStar��ӡ����ϵͳ V5.0");	//del by zfq,2013.04.24
		CString sDispName = _T("��ӡ����ϵͳ");	//add by zfq,2013.04.24
		CString sUninstall = CCommonFun::GetUninstallString(sDispName);
		if (sUninstall)
		{
			CSetupTipMgr::GetInstance().SetTip(0,"��ʼж��֮ǰ�汾....");
			CCommonFun::UninstallSoftware(sDispName);
		}
//		sDispName = _T("�Ű�֮��iSecStar��ӡ����ϵͳ V5.0��web��ѯ��");	//del by zfq,2013.04.24
		sDispName = _T("��ӡ����ϵͳ ��web��ѯ��");	//add by zfq,2013.04.24
		sUninstall = CCommonFun::GetUninstallString(sDispName);
		if (sUninstall)
		{
			CSetupTipMgr::GetInstance().SetTip(0,"��ʼж��֮ǰ�汾....");
			CCommonFun::UninstallSoftware(sDispName);
		}

		CSetupTipMgr::GetInstance().SetTip(0,"��ʼ��װ���ݿ����....");
		//�Ȱ�װmsde���ݿ�
		CString sPath;
		CString sparam;
		sPath.Format(_T("%smsde\\setup.exe"),m_sDefaultPath);
		sparam.Format(_T(" /S"));
		Install(sPath,sparam,INFINITE,SW_HIDE);

		CSetupTipMgr::GetInstance().SetTip(20,"��ʼ��װvc���а�....");
		//��װvc���а�
		sPath.Format(_T("%svcredist_x86\\vcredist_x86.exe"),m_sDefaultPath);
		Install(sPath," /q",INFINITE,SW_HIDE);

		CSetupTipMgr::GetInstance().SetTip(80,"��ʼ��װ�Ű�֮�Ǵ�ӡ����ϵͳ....");
		//��װ��ӡ������
		TCHAR szDir[MAX_PATH]={0};
		GetSystemDirectory(szDir,MAX_PATH);
		strcat(szDir,"\\msiexec.exe");
		sPath.Format(_T("%ssetup.msi"),m_sDefaultPath);
		Install(szDir,"/i \"" + sPath + "\" /q",INFINITE,SW_SHOW);

		//��װ��ӡweb������
		sPath.Format(_T("%ssetup-web.msi"),m_sDefaultPath);
		Install(szDir,"/i \"" + sPath + "\" /q",INFINITE,SW_SHOW);
		
		CSetupTipMgr::GetInstance().SetTip(100,"��װ��ɣ�");
		CSetupTipMgr::GetInstance().Close();
		theLog.Write("demover --- over %d",::GetTickCount());
		return TRUE;
	}
	else if (stricmp(__argv[1],"-Commit_Intranet") == 0 || stricmp(__argv[1],"/Commit_Intranet") == 0)
	{
		theLog.Write("ѧ������װ��ϣ�ɾ����װѡ��");
		DeleteInstallKey();
		DeleteInstallKey2();
		DeleteInstallKey3();
		return TRUE;
	}	
	else if(stricmp(__argv[1],_T("-RUNASADMIN")) == 0 || stricmp(__argv[1],_T("/RUNASADMIN")) == 0)
	{
		if(__argc < 3)
		{
			theLog.Write(_T("��������ȷ"));
			return 0;
		}
		CString sDefaultPath = CCommonFun::GetDefaultPath();
		sDefaultPath.TrimRight("\\");
		int nFind = -1;
		if((nFind = sDefaultPath.ReverseFind('\\')) > 0)
		{
			sDefaultPath = sDefaultPath.Left(nFind);
		}
		CString s;
		s.Format(_T("%s\\%s"),sDefaultPath,__argv[2]);
		theLog.Write(_T("set runasadmin = %s"),s);
		CRegistryEx reg(TRUE,FALSE);
		if(reg.CreateKey(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers")))
		{
			reg.Write(s,_T("RUNASADMIN"));
			theLog.Write(_T("set suc"));
		}
		CRegistryEx reg2(FALSE,FALSE);
		if(reg2.CreateKey(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers")))
		{
			reg2.Write(s,_T("RUNASADMIN"));
			theLog.Write(_T("set suc user"));
		}
		return TRUE;
	}
	//��װ��ӡ�ͻ���
	else if(stricmp(__argv[1],"-InstallClient") == 0 || stricmp(__argv[1],"/InstallClient") == 0)
	{
		//���÷���ǽ
		theLog.Write("���÷���ǽ");
		//
		//x64
		if(IsWow64() && CCommonFun::IsWinVistaOrLater())
		{
			CString sPath,sParam;
			sPath.Format("%sx64\\Install.exe",m_sDefaultPath);
			sParam.Format("-RunasAdmin iCltPrintHost.exe");
			Install(sPath,sParam,1,SW_HIDE);

// 			sParam.Format("-RunasAdmin iSafeClient.exe");
// 			Install(sPath,sParam,1,SW_HIDE);

			sParam.Format("-RunasAdmin install.exe");
			Install(sPath,sParam,1,SW_HIDE);

			sParam.Format("-InstallPrinter");
			Install(sPath,sParam,INFINITE,SW_HIDE);
		}
		else
		{
			helper.CallFun("InstallPrinter");
		}


		CWinFireWallOp firewall;
		if (firewall.IsWinFireWallEnabled())
		{
			if (firewall.IsExceptModeEnabled())
			{
				firewall.EnableExceptMode();
			}
		}
		CString sExPath;
		sExPath.Format("%siCltPrintHost.exe",m_sDefaultPath);
		firewall.ExceptProgram(sExPath);

		/*
		if(g_oIniFileConfig.GetVal("ClientConfig", "SvrIP", "").IsEmpty())
		{
			CSetIPDlg dlg(TYPE_PRINTER);
			if(dlg.DoModal() == IDOK)
				theConfig.SaveClientConfig(dlg.m_sIP);
		}
		*/
		
		// ��װ����
		Install(m_sDefaultPath+"iCltPrintHost.exe","-install",1,SW_HIDE);

		//����ע���Ȩ��
		AddRegAccess(PRODUCT_CUSTOM_REGISTY_KEY);
		AddRegAccess(PRODUCT_CUSTOM_REGISTY_KEY_AMD64);

		//����Ȩ��
		CString sTmpPath;
		sTmpPath.Format(_T("%s%s"),m_sDefaultPath,SYS_CONFIG_DIR);
		AddObjectAccess(sTmpPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		//add by zfq,2013.06.24,begin,�����ǵĳ���Ŀ¼���ó�everyoneȨ��
		sTmpPath.Format(_T("%s"),m_sDefaultPath);
		AddObjectAccess(sTmpPath, ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		//==ȡwindowsĿ¼ �� system32Ŀ¼ 
		char bysSysPath[255]; 
		memset(bysSysPath, 0x0, sizeof(bysSysPath) / sizeof(bysSysPath[0]));
		GetSystemDirectory(bysSysPath, MAX_PATH); 
		sTmpPath.Format(_T("%s\\spool"), bysSysPath);
		AddObjectAccess(sTmpPath, ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);
		//add by zfq,2013.06.24,end
		return TRUE;
	}
	//��װ������
	else if(stricmp(__argv[1],"-unInstallClient") == 0 || stricmp(__argv[1],"/unInstallClient") == 0)
	{
		theLog.Write("##CInstallApp::ParseParam,20.1");

#if 0
		if(IsLogonByDomainUser())	//add by zfq,2013.01.06,�Ӵ��ж�,�������û���¼��������ɾ��
		{
			theLog.Write("##=====CInstallApp::ParseParam,20.11,IsLogonByDomainUser=TRUE");
			return TRUE;
		}
#endif
		//����������ʱ,��iSecUpdate.exe�����������İ�װ��,���ʱ��Ҫ�Ƚ���iSecUpdate.exe,�Ա���������.
		FindTerminateProcess("iSecUpdate.exe");

		//ж�ط���
		Install(m_sDefaultPath+"iCltPrintHost.exe","-uninstall",INFINITE,SW_HIDE);

		theLog.Write("##CInstallApp::ParseParam,20.2");

		FindTerminateProcess("iCltPrintHost.exe");

		theLog.Write("##CInstallApp::ParseParam,20.3");

		FindTerminateProcess("iSafeClient.exe");
		FindTerminateProcess("iSafeClient_ENU.exe");	//add by zfq,2013.10.22

		theLog.Write("##CInstallApp::ParseParam,20.4");
		//ж�ش�ӡ����
		theLog.Write("begin uninstall printer");
		//
		//x64
		if(IsWow64() && CCommonFun::IsWinVistaOrLater())
		{
			theLog.Write("##CInstallApp::ParseParam,21.1");
			CString sPath,sParam;
			sPath.Format("%sx64\\Install.exe",m_sDefaultPath);
			
			sParam.Format("-UnInstallPrinter");
			Install(sPath,sParam,INFINITE,SW_HIDE);

		}
		else
		{
			theLog.Write("##CInstallApp::ParseParam,22.1");

			helper.CallFun("UnInstallPrinter");
		}
		
		theLog.Write("##CInstallApp::ParseParam,23");

		// ɾ��PrintJob�ļ���
		CString sPath;
		CCommonFun::DeleteDir(CCommonFun::GetDefaultPath()+"PrintJob");

		theLog.Write("##CInstallApp::ParseParam,24");

			sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR);
			theLog.Write("del %s",sPath);
			CCommonFun::DeleteDir(sPath);
			sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),PRT_CLIENT_JOB_DIR);
			theLog.Write("del %s",sPath);
			CCommonFun::DeleteDir(sPath);
		
		theLog.Write("##CInstallApp::ParseParam,28");

#if 0	//delete by zfq,2013.01.06,��װ�ͻ��ˣ��ټ��������û�ж��ʱ����������䱨��
		if(PathFileExists(CCommonFun::GetCommonDesktopDir()+"\\��ӡϵͳ��ѯ.lnk"))
			CFile::Remove(CCommonFun::GetCommonDesktopDir()+"\\��ӡϵͳ��ѯ.lnk");
#else	//add by zfq,2013.01.06
		CString szPrintLink = CCommonFun::GetCommonDesktopDir();
		if(szPrintLink.IsEmpty())
		{
			theLog.Write("!!CInstallApp::ParseParam,28.1,szPrintLink is empty");
		}
		else
		{
#if 0
			szPrintLink += "\\��ӡϵͳ��ѯ.lnk";
#else
			szPrintLink += "\\��ӡϵͳ��ѯ.url";
#endif
			try
			{
				if(PathFileExists(szPrintLink))
				{
					theLog.Write("##CInstallApp::ParseParam,28.2,szPrintLink=%s", szPrintLink);
					CFile::Remove(szPrintLink);
					theLog.Write("##CInstallApp::ParseParam,28.3");
				}
			}
			catch(...)
			{
				theLog.Write("##CInstallApp::ParseParam,28.4,catch sth.szPrintLink=%s", szPrintLink);
			}
		}
#endif

		theLog.Write("##CInstallApp::ParseParam,29");

		return TRUE;
	}
	//��װ������
	else if(stricmp(__argv[1],"-ConfigSvrDb") == 0 || stricmp(__argv[1],"/ConfigSvrDb") == 0)
	{
		CSetDbDlg dlg;
		dlg.DoModal();
	}
	//��װ������
	else if(stricmp(__argv[1],"-checkcry") == 0 || stricmp(__argv[1],"/checkcry") == 0)
	{
		EncryptCheck();
		return TRUE;
	}
	//��װ������
	else if(stricmp(__argv[1],"-InstallSvr") == 0 || stricmp(__argv[1],"/InstallSvr") == 0)
	{
		if(CCommonFun::IsWinVistaOrLater()&&IsWow64())
		{
			CString sPath,sParam;		
			sPath.Format("%sx64\\Install.exe",m_sDefaultPath);
			sParam.Format("-RunasAdmin iSecSvcMgr.exe");
			theLog.Write("64λ��װ path:%s param:%s",sPath,sParam);
			Install(sPath,sParam,1,SW_HIDE);
		}
		InstallSvr(PRINT_SVC_EXE);

		// ��ӳ���˵���ݷ�ʽ
#if 0	//del by zfq,2013.04.26
		CString proDir=CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetPrintProductName();
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		CString proDir = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,50,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,51,prodir=%s",  proDir);
		}
#endif

		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\���÷��������ݿ�����.lnk", CCommonFun::GetDefaultPath()+"res\\svrdb.ico", 0, "/ConfigSvrDb");
#if 0
		CString run;
		run.Format("\"%sinstsrv.exe\" iSecUpdaterHttp \"%ssrvany.exe\""
			, CCommonFun::GetDefaultPath(), CCommonFun::GetDefaultPath());
		CCommonFun::CreateProcessAndWait(run.GetBuffer());

		CRegistryEx pReg;
		pReg.CreateKey("SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterHttp\\Parameters");
		pReg.SetRootKey(HKEY_LOCAL_MACHINE);
		if(pReg.Open("SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterHttp\\Parameters"))
		{
			pReg.Write("Application",CCommonFun::GetDefaultPath()+"iSecHttpHost.exe");
			pReg.Close();
		}

		run.Format("\"%sPatchUpdateSvc.exe\" /config", CCommonFun::GetDefaultPath());
		CCommonFun::CreateProcessAndWait(run.GetBuffer());
		run.Format("\"%ssc.exe\" start iSecUpdaterHttp", CCommonFun::GetDefaultPath());
		CCommonFun::CreateProcessAndWait(run.GetBuffer());
#endif
		return TRUE;
	}
	//��װ������
	else if(stricmp(__argv[1],"-InstallSvr_print_edu") == 0 || stricmp(__argv[1],"/InstallSvr_print_edu") == 0)
	{
		InstallSvr(PRINT_EDU_SVC_EXE);

		// ��ӳ���˵���ݷ�ʽ
#if 0	//del by zfq,2013.04.26
		CString proDir=CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetEduPrintProductName();
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\���÷��������ݿ�����.lnk", CCommonFun::GetDefaultPath()+"res\\svrdb.ico", 0, "/ConfigSvrDb");
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,200,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			CString szDefPath = CCommonFun::GetDefaultPath();
			CString proDir = szProgramDir + szProductName;
			theLog.Write("##CInstallApp::ParseParam,201,prodir=%s",  proDir);
			if(!CCommonFun::CreateShellLink(szDefPath + "Install.exe", 
				proDir+"\\���÷��������ݿ�����.lnk", szDefPath + "res\\svrdb.ico", 0, "/ConfigSvrDb"))
			{
				theLog.Write("!!CInstallApp::ParseParam,202,CreateShellLink fail,szDefPath=%s,proDir=%s", szDefPath, proDir);
			}

		}
#endif

#if 0
		CString run;
		run.Format("\"%sinstsrv.exe\" iSecUpdaterHttp \"%ssrvany.exe\""
			, CCommonFun::GetDefaultPath(), CCommonFun::GetDefaultPath());
		CCommonFun::CreateProcessAndWait(run.GetBuffer());

		CRegistryEx pReg;
		pReg.CreateKey("SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterHttp\\Parameters");
		pReg.SetRootKey(HKEY_LOCAL_MACHINE);
		if(pReg.Open("SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterHttp\\Parameters"))
		{
			pReg.Write("Application",CCommonFun::GetDefaultPath()+"iSecHttpHost.exe");
			pReg.Close();
		}

		run.Format("\"%sPatchUpdateSvc.exe\" /config", CCommonFun::GetDefaultPath());
		CCommonFun::CreateProcessAndWait(run.GetBuffer());
		run.Format("\"%ssc.exe\" start iSecUpdaterHttp", CCommonFun::GetDefaultPath());
		CCommonFun::CreateProcessAndWait(run.GetBuffer());
#endif
		return TRUE;
	}
	else if(stricmp(__argv[1],"-UnInstallSvr") == 0 || stricmp(__argv[1],"/UnInstallSvr") == 0 ||
		stricmp(__argv[1],"-UnInstallSvr_edu") == 0 || stricmp(__argv[1],"/UnInstallSvr_edu") == 0)
	{
		UninstallSvr();

		try
		{
			if(stricmp(__argv[1],"-UnInstallSvr_edu") == 0 || stricmp(__argv[1],"/UnInstallSvr_edu") == 0)
			{
#if 0	//del by zfq,2013.04.26
				CString proDir=CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetEduPrintProductName();
#else	//add by zfq,2013.04.26
				CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
				CString szProductName = "";
				CString proDir = "";
				if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
				{
					theLog.Write("!!CInstallApp::ParseParam,300,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
				}
				else
				{
					proDir = szProgramDir + szProductName;
					theLog.Write("##CInstallApp::ParseParam,301,prodir=%s",  proDir);
				}
#endif
				if(PathFileExists(proDir+"\\��Ӵ�ӡ����.lnk"))
					CFile::Remove(proDir+"\\��Ӵ�ӡ����.lnk");
				if(PathFileExists(proDir+"\\���ô�ӡվ�����������.lnk"))
					CFile::Remove(proDir+"\\���ô�ӡվ�����������.lnk");
				if(PathFileExists(proDir+"\\���÷��������ݿ�����.lnk"))
					CFile::Remove(proDir+"\\���÷��������ݿ�����.lnk");
			}
			else
			{
#if 0
				CString proDir=CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetEduPrintProductName();
#else	//add by zfq,2013.04.26
				CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
				CString szProductName = "";
				CString proDir = "";
				if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
				{
					theLog.Write("!!CInstallApp::ParseParam,330,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
				}
				else
				{
					proDir = szProgramDir + szProductName;
					theLog.Write("##CInstallApp::ParseParam,331,prodir=%s",  proDir);
				}
#endif

				if(PathFileExists(proDir+"\\��Ӵ�ӡ����.lnk"))
					CFile::Remove(proDir+"\\��Ӵ�ӡ����.lnk");
				if(PathFileExists(proDir+"\\���ô�ӡվ�����������.lnk"))
					CFile::Remove(proDir+"\\���ô�ӡվ�����������.lnk");
				if(PathFileExists(proDir+"\\���÷��������ݿ�����.lnk"))
					CFile::Remove(proDir+"\\���÷��������ݿ�����.lnk");

				CString run;
				run.Format("\"%ssc.exe\"  stop iSecUpdaterHttp"
					, CCommonFun::GetDefaultPath());
				CCommonFun::CreateProcessAndWait(run.GetBuffer());
				run.Format("\"%sinstsrv.exe\"  iSecUpdaterHttp remove"
					, CCommonFun::GetDefaultPath());
				CCommonFun::CreateProcessAndWait(run.GetBuffer());
			}
		}
		catch (...)
		{
		}

		return TRUE;
	}
	//��װ��ӡ���ݿ�
	else if(stricmp(__argv[1],"-InstallDb") == 0 || 
		stricmp(__argv[1],"/InstallDb") == 0 ||
		stricmp(__argv[1],"/InstallDb_print_edu") == 0 ||
		stricmp(__argv[1],"/InstallDb_print_edu") == 0)
	{
		if(m_bQueit)
		{
			CCreateDbDlg dlg;
			dlg.CreateDbOnLocalMsde(PRINT_DB_NAME);
		}
		else
		{
			CCreateDbDlg dlg;
			dlg.m_aryDb.Add(PRINT_DB_NAME);
			dlg.DoModal();
			if (!dlg.m_bInstalledSuc)
			{
				//SetErrCode(1,"���ݿⰲװʧ��");
			}
			else
			{
				if (::MessageBox(0,"�Ƿ���������ͬ���򵼣�","��ʾ",MB_ICONQUESTION |MB_YESNO) == IDYES)
				{
					CString sPath;
					sPath.Format(_T("%siSecSync.exe"),m_sDefaultPath);
					Install(sPath,"",INFINITE,SW_SHOW);
				}
			}
		}

		return TRUE;
	}
	//��װweb
	else if(stricmp(__argv[1],"-Installweb_LM") == 0 || stricmp(__argv[1],"/Installweb_LM") == 0)
	{
		CString sParentDir;
		//����Ȩ��TempImages
		CString sTmp = m_sDefaultPath;
		int n =sTmp.ReverseFind('\\');
		theLog.Write("dir = %s",sTmp);
		sTmp = sTmp.Left(n);
		theLog.Write("dir = %s",sTmp);
		n = sTmp.ReverseFind('\\');
		sParentDir = sTmp = sTmp.Left(n);
		sTmp += "\\TempImages";
		CCommonFun::CreateDir(sTmp);
		theLog.Write("dir = %s",sTmp);
		AddObjectAccess(sTmp,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		CIISWeb is;
		char szUserName[100] = {0};
		//������װ��iis
		if(is.GetAnonymousUserName(szUserName))
		{
			theLog.Write("GetAnonymousUserName %s",szUserName);

			theLog.Write("CInstallApp::ParseParam,Installweb_LM,3,m_bQueit=%d", m_bQueit);

			if(m_bQueit)
			{
				CWebSheet sheet(_T("��ӡWeb������������"));
				sheet.page2.FinishWebSiteSet(TRUE);
				//sheet.WriteTheURL(TRUE);
			}
			else
			{
				CWebSheet sheet(_T("��ӡWeb������������"));
				sheet.DoModal();
			}

			theLog.Write("CInstallApp::ParseParam,Installweb_LM,4");
		}
		else
		{
			CWebSheet sheet(0,_T("��ӡWeb������������"));
			if(m_bQueit)
			{

				sheet.WriteTheURL(TRUE);
			}
			else
			{
				sheet.DoModal();
				sheet.WriteTheURL();
			}

			CString run;
			run.Format("\"%sinstsrv.exe\" WebServer \"%ssrvany.exe\""
				, CCommonFun::GetDefaultPath(), CCommonFun::GetDefaultPath());
			CCommonFun::CreateProcessAndWait(run.GetBuffer());

			CRegistryEx pReg;
			pReg.CreateKey("SYSTEM\\CurrentControlSet\\Services\\WebServer\\Parameters");
			pReg.SetRootKey(HKEY_LOCAL_MACHINE);
			if(pReg.Open("SYSTEM\\CurrentControlSet\\Services\\WebServer\\Parameters"))
			{
				pReg.Write("Application",CCommonFun::GetDefaultPath()+"WebServer.exe");
				CString sparm;
				sparm.Format(_T("/port:80 /path:\"%s\" /vpath:\"/XabPrint\" "),sParentDir);
				pReg.Write("AppParameters",sparm);
				pReg.Close();
			}

			is.CreateAdminLnk();
			::Sleep(100);
			CCommonFun::StartSvc("WebServer");
		}

		CString sIP = g_oIniFileClientSetup.GetVal("ClientConfig","SvrIP",CCommonFun::GetLocalIP());
		int nPort = g_oIniFileClientSetup.GetVal("ClientConfig","urlport",80);
		CCommonFun::CreateDir(CCommonFun::GetDefaultPath()+"Patches");		
		g_oIniFileClientSetup.SetVal("UpdaterClt", "LocalAddr", CCommonFun::GetDefaultPath()+"Patches");
		g_oIniFileClientSetup.SetVal("UpdaterClt", "SvrAddr", sIP);
		g_oIniFileClientSetup.SetVal("UpdaterClt", "SvrPort", 8080);

		SetCurrentDirectory(CCommonFun::GetDefaultPath()+"Client_Setup");

		theLog.Write("CInstallApp::ParseParam,Installweb_LM,6");

		ShellExecute(NULL, "open", "\""+CCommonFun::GetDefaultPath()+"Client_Setup\\����һ����װ��.bat\"", NULL, NULL, SW_HIDE);

		theLog.Write("CInstallApp::ParseParam,Installweb_LM,7");

		//add by zfq,2013.05.31,begin
		CString szJpnClntPath;
		CString szJpnSetupPath;
		szJpnClntPath.Format("%sClient_Setup_JPN", CCommonFun::GetDefaultPath());
		szJpnSetupPath.Format("%s\\Setup.exe", szJpnClntPath);
		if(!::PathFileExists(szJpnClntPath))
		{//������			
			theLog.Write("!#CInstallApp::ParseParam,200,Not Exists,szJpnClntPath=%s", szJpnClntPath);
		}
		else if(!::PathFileExists(szJpnSetupPath))
		{//�ļ�������
			theLog.Write("!#CInstallApp::ParseParam,201,Not Exists,szJpnSetupPath=%s", szJpnSetupPath);
		}
		else
		{
			CString szJapIniPath;
			szJapIniPath.Format(_T("%sClient_Setup_JPN\\config.ini"), CCommonFun::GetDefaultPath());
			if(!::CopyFile(g_oIniFileClientSetup.m_sPath, szJapIniPath, FALSE))	//�����İ�Ŀͻ��������ļ����������İ�Ŀͻ��������ļ�
			{
				theLog.Write("!!CInstallApp::ParseParam,203,CopyFile fail,err=%d,ChsIni=[%s],szJapIniPath=[%s]"
					, GetLastError(), g_oIniFileClientSetup.m_sPath, szJapIniPath);
			}

			::Sleep(5000);
			SetCurrentDirectory(szJpnClntPath);
			ShellExecute(NULL, "open", "\""+CCommonFun::GetDefaultPath()+"Client_Setup_JPN\\����һ����װ��.bat\"", NULL, NULL, SW_HIDE);
		}
		//add by zfq,2013.05.31,end
		return TRUE;
	}
	
	//��װǶ��ʽwebվ��
    else if(stricmp(__argv[1],"-InstallEmbedWeb") == 0 || stricmp(__argv[1],"/InstallEmbedWeb") == 0)
    {
        if (__argc < 3)
        {
            theLog.Write("��������������");
            return 0;
        }
        CRegistryEx pReg;
        pReg.SetRootKey(HKEY_LOCAL_MACHINE);
        if(pReg.Open("SOFTWARE\\Microsoft\\ASP.NET\\4.0.30319.0"))
        {
            pReg.Close();
        }
        else
        {
            theLog.Write("!!InstallEmbedWeb, Open reg failed, SOFTWARE\\Microsoft\\ASP.NET\\4.0.30319.0");
            MessageBox(0, "��������谲װ.NET Framework 4.0, ���Ȱ�װ", "��ʾ", MB_OK);
            return FALSE;
        }
        {
            CString sPath;
            CString sparam;
            sPath.Format(_T("%s.NETFramework4.0\\Microsoft.NETFramework4.0.exe"),m_sDefaultPath);
            sparam.Format(_T(""));
            Install(sPath,sparam,INFINITE,SW_SHOW);

            char temp[_MAX_PATH];
            TCHAR windowsRoot[ MAX_PATH ];
            if( !::GetWindowsDirectory( windowsRoot, sizeof(windowsRoot)))
            {
                return FALSE;
            }
            else
            {
                sPath.Format("%s",windowsRoot);
                theLog.Write("GetSystemDirectory,sPath=%s", sPath);
                sPath.Format("%s\\Microsoft.NET\\Framework\\v4*", windowsRoot);
                ShellExecute(NULL, "open", "\""+CCommonFun::GetDefaultPath()+"RegIIS.bat\"", sPath, NULL, SW_HIDE);
            }
        }
        
        //ΪWEB������־�ļ��еķ���Ȩ��
        CString szPath;
        CString szDefaultPath = CCommonFun::GetDefaultPath();
        theLog.Write("/InstallEmbedWeb,1,szDefaultPath=%s", szDefaultPath);
        int n = szDefaultPath.ReverseFind('\\');
        CString szWebstaionPath = szDefaultPath.Left(n);
        n = szWebstaionPath.ReverseFind('\\');
        szWebstaionPath = szWebstaionPath.Left(n);
        szPath.Format("%s\\Log", szWebstaionPath);

        theLog.Write("�������� %d", __argc);
        CString webName(__argv[2]);
        theLog.Write("webName=%s", webName);

        DWORD dwMask = GENERIC_ALL|STANDARD_RIGHTS_ALL;
        TCHAR *pcPath = new TCHAR[100];
        pcPath = szPath.GetBuffer();
        CString szuser = "Everyone";
        TCHAR *pcuser = new TCHAR[20];pcuser = szuser.GetBuffer();
        if(!CCommonFun::AddRights(pcPath,pcuser,dwMask))
        {
            theLog.Write("!!/InstallEmbedWeb,2,AddAccessRights,szPath = %s", szPath);
        }
        szPath.ReleaseBuffer(); delete [] pcPath; pcPath = 0;
        szuser.ReleaseBuffer(); delete [] pcuser; pcuser = 0;

        szPath.Format("%s\\doc", szWebstaionPath);
        pcPath = new TCHAR[100];
        pcPath = szPath.GetBuffer();
        szuser = "Everyone";
        pcuser = new TCHAR[20];
        pcuser = szuser.GetBuffer();
        if(!CCommonFun::AddRights(pcPath,pcuser,dwMask))
        {
            theLog.Write("!!/InstallEmbedWeb,3,AddAccessRights,szPath = %s", szPath);
        }
        szPath.ReleaseBuffer(); delete [] pcPath; pcPath = 0;
        szuser.ReleaseBuffer(); delete [] pcuser; pcuser = 0;
#if 1
        CString szSharePath;
        szSharePath.Format("%s\\doc", szWebstaionPath);
        theLog.Write("/InstallEmbedWeb,szSharePath=%s", szSharePath);
        if (webName.CompareNoCase("KEMEIPrint") == 0 && PathFileExists(szSharePath))
        {
            //�����ڴ�֮ǰ����CheckNetShare�ģ�������صĶ���true�����Բ��ã�ֱ����ӹ���
            if(CCommonFun::AddNetShare("doc","",szSharePath))
            {
                theLog.Write("/InstallEmbedWeb, AddNetShare, success, szSharePath=%s", szSharePath);
            }
            else
            {
                theLog.Write("!!/InstallEmbedWeb, AddNetShare, failed, szSharePath=%s", szSharePath);
            }
            //����û�
            system("net user scan xab123456 /add");//����������߳�ȥִ�У����Բ�֪����ʱ��ִ�����
			::Sleep(500);
            system("net localgroup administrators scan /add");
			::Sleep(500);
        }
        else
        {
            theLog.Write("!!/InstallEmbedWeb, AddNetShare, file is not exists, szSharePath= %s", szSharePath);
        }
#endif

        CIISWeb is;
        char szUserName[100] = {0};

        //������װ��iis
        if(is.GetAnonymousUserName(szUserName))
        {
            theLog.Write("GetAnonymousUserName = %s, m_bQueit = %d",szUserName, m_bQueit);
            if(m_bQueit)
            {
                CEmbedWebSheet sheet(_T("��ӡ��Web����������"));
                sheet.page2.FinishWebSiteSet(TRUE);
                //sheet.WriteTheURL(TRUE);
            }
            else
            {
                CEmbedWebSheet sheet(_T("��ӡ��Web����������"));
                sheet.page2.SetWebVirtaulName(webName);
                sheet.DoModal();
            }
        }
        else
        {
            CEmbedWebSheet sheet(0,_T("��ӡ��Web����������"));
            if(m_bQueit)
            {

                sheet.WriteTheURL(TRUE);
            }
            else
            {
                sheet.DoModal();
                sheet.WriteTheURL();
            }

            CString run;
            run.Format("\"%sinstsrv.exe\" WebServer \"%ssrvany.exe\""
                , CCommonFun::GetDefaultPath(), CCommonFun::GetDefaultPath());
            CCommonFun::CreateProcessAndWait(run.GetBuffer());

            CString sParentDir;
            CString sTmp = m_sDefaultPath;
            int n =sTmp.ReverseFind('\\');
            theLog.Write("dir = %s",sTmp);
            sTmp = sTmp.Left(n);
            theLog.Write("dir = %s",sTmp);
            n = sTmp.ReverseFind('\\');
            sParentDir = sTmp = sTmp.Left(n);

            CRegistryEx pReg;
            pReg.CreateKey("SYSTEM\\CurrentControlSet\\Services\\WebServer\\Parameters");
            pReg.SetRootKey(HKEY_LOCAL_MACHINE);
            if(pReg.Open("SYSTEM\\CurrentControlSet\\Services\\WebServer\\Parameters"))
            {
                pReg.Write("Application",CCommonFun::GetDefaultPath()+"WebServer.exe");
                CString sparm;

                sparm.Format(_T("/port:80 /path:\"%s\" /vpath:\"/%s\" "),sParentDir, webName);
                pReg.Write("AppParameters",sparm);
                pReg.Close();
            }
            else
            {
                theLog.Write("!!InstallEmbedWeb, pReg.Open failed");
            }
            if(webName.CompareNoCase("TOSHIBAPrint") == 0)
                is.CreateEmbededWebLnk("", "80", webName);
            ::Sleep(100);
            CCommonFun::StartSvc("WebServer");
        }
        return TRUE;
    }
	//��װweb
	else if(stricmp(__argv[1],"-Installweb") == 0 || stricmp(__argv[1],"/Installweb") == 0)
	{
		CString sParentDir;
		//����Ȩ��TempImages
		CString sTmp = m_sDefaultPath;
		int n =sTmp.ReverseFind('\\');
		theLog.Write("dir = %s",sTmp);
		sTmp = sTmp.Left(n);
		theLog.Write("dir = %s",sTmp);
		n = sTmp.ReverseFind('\\');
		sParentDir = sTmp = sTmp.Left(n);
		sTmp += "\\TempImages";
		CCommonFun::CreateDir(sTmp);
		theLog.Write("dir = %s",sTmp);
		AddObjectAccess(sTmp,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		CIISWeb is;
		char szUserName[100] = {0};
		//������װ��iis
		if(is.GetAnonymousUserName(szUserName))
		{
			theLog.Write("GetAnonymousUserName %s",szUserName);
			if(m_bQueit)
			{
				CWebSheet sheet(_T("��ӡWeb������������"));
				sheet.page2.FinishWebSiteSet(TRUE);
				//sheet.WriteTheURL(TRUE);
			}
			else
			{
				CWebSheet sheet(_T("��ӡWeb������������"));
				sheet.DoModal();
			}
			
		}
		else
		{
			CWebSheet sheet(0,_T("��ӡWeb������������"));
			if(m_bQueit)
			{
				
				sheet.WriteTheURL(TRUE);
			}
			else
			{
				sheet.DoModal();
				sheet.WriteTheURL();
			}
			
			CString run;
			run.Format("\"%sinstsrv.exe\" WebServer \"%ssrvany.exe\""
				, CCommonFun::GetDefaultPath(), CCommonFun::GetDefaultPath());
			CCommonFun::CreateProcessAndWait(run.GetBuffer());

			CRegistryEx pReg;
			pReg.CreateKey("SYSTEM\\CurrentControlSet\\Services\\WebServer\\Parameters");
			pReg.SetRootKey(HKEY_LOCAL_MACHINE);
			if(pReg.Open("SYSTEM\\CurrentControlSet\\Services\\WebServer\\Parameters"))
			{
				pReg.Write("Application",CCommonFun::GetDefaultPath()+"WebServer.exe");
				CString sparm;
#if 1 //del by zfq,2015-04-25 �ϴ����� ��ʱ����XabPrintĿ¼
				sparm.Format(_T("/port:80 /path:\"%s\" /vpath:\"/XabPrint\" "),sParentDir);
#else
				sparm.Format(_T("/port:80 /path:\"%s\" /vpath:\"\" "), sParentDir);
#endif
				pReg.Write("AppParameters",sparm);
				pReg.Close();
			}
			
			is.CreateAdminLnk();
			::Sleep(100);
			CCommonFun::StartSvc("WebServer");
		}
		

		CString sIP = CCommonFun::GetLocalIP();
		g_oIniFileClientSetup.SetVal("ClientConfig","SvrIP",sIP);
		CCommonFun::CreateDir(CCommonFun::GetDefaultPath()+"Patches");		
		g_oIniFileClientSetup.SetVal("UpdaterClt", "LocalAddr", CCommonFun::GetDefaultPath()+"Patches");
		g_oIniFileClientSetup.SetVal("UpdaterClt", "SvrAddr", sIP);
		g_oIniFileClientSetup.SetVal("UpdaterClt", "SvrPort", 8080);

		SetCurrentDirectory(CCommonFun::GetDefaultPath()+"Client_Setup");
		ShellExecute(NULL, "open", "\""+CCommonFun::GetDefaultPath()+"Client_Setup\\����һ����װ��.bat\"", NULL, NULL, SW_HIDE);
		return TRUE;
	}
	//��װweb
	else if(stricmp(__argv[1],"-ConfigwebDb") == 0 || stricmp(__argv[1],"/ConfigwebDb") == 0)
	{
		CWebSheet sheet(0,_T("��ӡWeb������������"));
		sheet.DoModal();
		return TRUE;
	}
	//��װ��ӡվ��
	else if(stricmp(__argv[1],"-ConfigPrt") == 0 || stricmp(__argv[1],"/ConfigPrt") == 0)
	{
		DWORD dwPid = 0;
		if (__argc == 3)
		{
			dwPid = atoi(__argv[2]);
		}
		else if (__argc == 4)
		{
			dwPid = atoi(__argv[2]);
			theConfig.SetCurrentConfig(__argv[3]);
		}

		//��Ӵ�ӡ��
		CStringArray ary;
		int nIndex = 0;
		CCommonFun::EnumeratePrinters(ary);
		if (ary.GetCount() > 0)
		{
			CInstallPrtDlg dlg;
			if(dlg.DoModal()==IDOK && dwPid > 0)
			{
				theLog.Write(_T(" prttation = %d"),dwPid);

#if 0	//modify by zxl20130618
				FindTerminateProcess("printStation.exe");

				::Sleep(20);
				CString sExPath;
				sExPath.Format("%sPrintStation.exe",m_sDefaultPath);
#else
				CString sExPath;
				if (theConfig.IsPrintStationCloud())
				{
					FindTerminateProcess("printStationCloud.exe");

					::Sleep(20);
					sExPath.Format("%sPrintStationCloud.exe",m_sDefaultPath);
				}
				else
				{
					FindTerminateProcess("printStation.exe");

					::Sleep(20);
					sExPath.Format("%sPrintStation.exe",m_sDefaultPath);
				}
#endif
				Install(sExPath,"",1,SW_SHOW);
			}
		}
		else
		{
			theLog.Write("no printer");
			MessageBox(0,"�û���û�д�ӡ�����밲װ��ӡ����","��ʾ",MB_OK | MB_ICONINFORMATION);
		}
		
		return TRUE;
	}
	//��װ��ӡվ��
	else if(stricmp(__argv[1],"-ConfigPrtCnnt") == 0 || stricmp(__argv[1],"/ConfigPrtCnnt") == 0)
	{
		if (__argc == 3)
		{
			theConfig.SetCurrentConfig(__argv[2]);
		}

		CConfigPrtDlg config;
		config.DoModal();
	}
	else if(stricmp(__argv[1],"-ConfigEmbedWeb") == 0 || stricmp(__argv[1],"/ConfigEmbedWeb") == 0)
	{
		if (__argc == 3)
		{
			theConfig.SetCurrentConfig(__argv[2]);
		}

        ConfigEmbedWeb config;
        config.DoModal();
	}
	//��װ��ӡ�����ն�
	else if(stricmp(__argv[1],"-UnInstallPrtMon") == 0 || 
		stricmp(__argv[1],"/UnInstallPrtMon") == 0 )
	{
		FindTerminateProcess("LmPrinterSvcHost.exe");
		//ж�ط���
		Install(m_sDefaultPath+"LmPrinterSvcHost.exe","-uninstall",INFINITE,SW_HIDE);
	

		// ɾ��PrintJob�ļ���
		CString sPath;
		sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR);
		theLog.Write("del %s",sPath);
		CCommonFun::DeleteDir(sPath);
		sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),PRT_STATION_JOB_DIR);
		theLog.Write("del %s",sPath);
		CCommonFun::DeleteDir(sPath);

		if(PathFileExists(CCommonFun::GetCommonDesktopDir()+"\\��ӡ���ù���.lnk"))
			CFile::Remove(CCommonFun::GetCommonDesktopDir()+"\\��ӡ���ù���.lnk");

	}
	//��װ��ӡ�����ն�
	else if(stricmp(__argv[1],"-InstallPrtMon") == 0 || 
		stricmp(__argv[1],"/InstallPrtMon") == 0 )
	{
		//���÷���ǽ
		theLog.Write("���÷���ǽ");

		CWinFireWallOp firewall;
		if (firewall.IsWinFireWallEnabled())
		{
			if (firewall.IsExceptModeEnabled())
			{
				firewall.EnableExceptMode();
			}
		}
		CString sExPath;
		sExPath.Format("%sLmPrinterSvcHost.exe",m_sDefaultPath);
		firewall.ExceptProgram(sExPath);

		//����ip��ַ
		CConfigPrtDlg config;
		config.DoModal();

		//��Ӵ�ӡ��
		CStringArray ary;
		int nIndex = 0;
		CCommonFun::EnumeratePrinters(ary);
		if (ary.GetCount() > 0)
		{
			CInstallPrtDlg dlg;
			if(dlg.DoModal()==IDOK )
			{
			}
		}
	
		// ��ӳ���˵���ݷ�ʽ
		//���ڷ������ʹ�ӡվ�㶼�ᰲװinstall.exe,û�а취�����Ƿ񴴽���ݷ�ʽ������ʹ�ô��봴��
		//add by zhandb 2012.3.5
#if 0
		CString proDir = CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetPrintProductName();
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		CString proDir = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,420,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,421,prodir=%s",  proDir);
		}
#endif

		theLog.Write("prodir = %s",proDir);
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\��Ӵ�ӡ����.lnk", CCommonFun::GetDefaultPath()+"res\\prtwizard.ico", 0, "/Configprt");

		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"lmConfig.exe", 
			CCommonFun::GetCommonDesktopDir()+"\\��ӡ���ù���.lnk", 
			CCommonFun::GetDefaultPath()+"res\\prtwizard.ico",
			0,
			0);
		//��װ����ת������
		Install(m_sDefaultPath+"LmPrinterSvcHost.exe","-install",1,SW_HIDE);

		return TRUE;
	}
	//=================================================================add by zfq,2013.02.16,������ҵ��ӡ���й���˵İ�װж��,begin
	//ж�ش�ӡ���й����
	else if(stricmp(__argv[1], "-UnInstallPrtCenterSvr") == 0 || 
		stricmp(__argv[1],"/UnInstallPrtCenterSvr") == 0 )
	{//ж�ط���
		theLog.Write("##CInstallApp::ParseParam,160,ж�ش�ӡ���й����");

		Install(m_sDefaultPath + "iPrintCenterSvrHost.exe", "-uninstall",INFINITE,SW_HIDE);

		theLog.Write("##CInstallApp::ParseParam,161,ж�ش�ӡ���й����");

		FindTerminateProcess("iPrintCenterSvrHost.exe");

		theLog.Write("##CInstallApp::ParseParam,162");

		//x64
		if(IsWow64() && CCommonFun::IsWinVistaOrLater())
		{
			theLog.Write("##CInstallApp::ParseParam,162.1");
			CString sPath,sParam;
			sPath.Format("%sx64\\Install.exe",m_sDefaultPath);

			sParam.Format("-UnInstallPrinter");
			Install(sPath,sParam,INFINITE,SW_HIDE);

		}
		else
		{
			theLog.Write("##CInstallApp::ParseParam,163.1");

			helper.CallFun("UnInstallPrinter");
		}

		theLog.Write("##CInstallApp::ParseParam,164");

		// ɾ��PrintJob�ļ���
		CString sPath;
		CCommonFun::DeleteDir(CCommonFun::GetDefaultPath()+"PrintJob");

		theLog.Write("##CInstallApp::ParseParam,165");

		sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR);
		theLog.Write("##CInstallApp::ParseParam,166,del %s",sPath);
		CCommonFun::DeleteDir(sPath);
		sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),PRT_CLIENT_JOB_DIR);
		theLog.Write("##CInstallApp::ParseParam,167,del %s",sPath);
		CCommonFun::DeleteDir(sPath);

		theLog.Write("##CInstallApp::ParseParam,28");

	}
	//��װ��ӡ���й����
	else if(stricmp(__argv[1],"-InstallPrtCenterSvr") == 0 || 
		stricmp(__argv[1],"/InstallPrtCenterSvr") == 0 )
	{
		//���÷���ǽ
		theLog.Write("##CInstallApp::ParseParam,170,��װ��ӡ���й����");

		CWinFireWallOp firewall;
		if (firewall.IsWinFireWallEnabled())
		{
			if (firewall.IsExceptModeEnabled())
			{
				firewall.EnableExceptMode();
			}
		}

		theLog.Write("##CInstallApp::ParseParam,171,��װ��ӡ���й����");

		CString sExPath;
		sExPath.Format("%siPrintCenterSvrHost.exe",m_sDefaultPath);
		firewall.ExceptProgram(sExPath);

		theLog.Write("##CInstallApp::ParseParam,172,��װ��ӡ���й����");

		//����ip��ַ
		CDlgCfgPrtCenterSvr dlg;
		dlg.DoModal();

		theLog.Write("##CInstallApp::ParseParam,173,��װ��ӡ���й����");


		// ��ӳ���˵���ݷ�ʽ
		//���ڷ������ʹ�ӡվ�㶼�ᰲװinstall.exe,û�а취�����Ƿ񴴽���ݷ�ʽ������ʹ�ô��봴��
		//add by zhandb 2012.3.5
#if 0
		CString proDir = CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetPrintProductName();
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		CString proDir = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,174,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,175,prodir=%s",  proDir);
		}
#endif

#if 0
		theLog.Write("prodir = %s",proDir);
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\��Ӵ�ӡ����.lnk", CCommonFun::GetDefaultPath()+"res\\prtwizard.ico", 0, "/Configprt");

		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"lmConfig.exe", 
			CCommonFun::GetCommonDesktopDir()+"\\��ӡ���ù���.lnk", 
			CCommonFun::GetDefaultPath()+"res\\prtwizard.ico",
			0,
			0);
#endif

		theLog.Write("##CInstallApp::ParseParam,174,��װ��ӡ���й����");

		//��װ���й����
		Install(m_sDefaultPath + "iPrintCenterSvrHost.exe","-install", 1, SW_HIDE);

		theLog.Write("##CInstallApp::ParseParam,175,��װ��ӡ���й����");

		//bool bRet = helper.CallFun("InstallPrinter");  //��װ�����ӡ����

		//theLog.Write("##CInstallApp::ParseParam,176,��װ��ӡ���й����,bRet=%d", bRet);

		//����Ȩ��
		CString sTmpPath;
		sTmpPath.Format(_T("%s%s"), m_sDefaultPath, SYS_CONFIG_DIR);
		AddObjectAccess(sTmpPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		//add by zfq,2013.06.24,begin,�����ǵĳ���Ŀ¼���ó�everyoneȨ��
		sTmpPath.Format(_T("%s"), m_sDefaultPath);
		AddObjectAccess(sTmpPath, ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);

		//==ȡwindowsĿ¼ �� system32Ŀ¼ 
		char bysSysPath[255]; 
		memset(bysSysPath, 0x0, sizeof(bysSysPath) / sizeof(bysSysPath[0]));
		GetSystemDirectory(bysSysPath, MAX_PATH); 
		sTmpPath.Format(_T("%s\\spool"), bysSysPath);
		AddObjectAccess(sTmpPath, ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);
		//add by zfq,2013.06.24,end

		return TRUE;
	}
	//=================================================================add by zfq,2013.02.16,������ҵ��ӡ���й���˵İ�װж��,end
	//��װ��ӡ�����ն�
	else if(stricmp(__argv[1],"-InstallLMPrt") == 0 || 
		stricmp(__argv[1],"/InstallLMPrt") == 0 )
	{
		//���÷���ǽ
		theLog.Write("���÷���ǽ");

		CWinFireWallOp firewall;
		if (firewall.IsWinFireWallEnabled())
		{
			if (firewall.IsExceptModeEnabled())
			{
				firewall.EnableExceptMode();
			}
		}

		CString sExPath;
		sExPath.Format("%sLmPrinterSvcHost.exe",m_sDefaultPath);
		firewall.ExceptProgram(sExPath);
		
#if 0 //delete by zfq,2012.12.27
		//��װ����ת������
		Install(m_sDefaultPath+"LmConfig.exe","-config",INFINITE,SW_SHOW);
#else	//add by zfq,2012.12.27
		CString sMsg;
		sMsg.Format(_T("�Ƿ����˴�ӡ����\n"));
		int nId = ::MessageBox(0,sMsg,"��ʾ",MB_ICONQUESTION |MB_YESNO);
		if (IDYES == nId)
		{//���˴�ӡ������
			Install(m_sDefaultPath+"LmConfig.exe","-config",INFINITE,SW_SHOW);
		}
		else
		{//������ӡ������
			//add by szy 20150424 begin �����˻���ʱ�����û�ѡ���Ƿ�װ�������ݿ�
#if 0
			CDlgPrtChoose choosedlg;
			choosedlg.DoModal();
			BOOL bCboxDB = choosedlg.m_bCboxDB; 
			BOOL bCboxWeb = choosedlg.m_bCboxWeb; 
			if(bCboxDB)
			{
				CDlgCreateAppDB dlg;
				dlg.m_aryDb.Add(_T("iSec_PrinterApp"));
				dlg.DoModal();
				if (!dlg.m_bInstalledSuc)
				{
					//SetErrCode(1,"���ݿⰲװʧ��");
				}
			}
#endif
			//add by szy 20150424 end
			CConfigPrtDlg config;
			config.DoModal();
		}
#endif
			
		//��Ӵ�ӡ��
		CStringArray ary;
		int nIndex = 0;
		CCommonFun::EnumeratePrinters(ary);
		if (ary.GetCount() > 0)
		{
			CInstallPrtDlg dlg;
			if(dlg.DoModal()==IDOK )
			{
			}
		}

		// ��ӳ���˵���ݷ�ʽ
		//���ڷ������ʹ�ӡվ�㶼�ᰲװinstall.exe,û�а취�����Ƿ񴴽���ݷ�ʽ������ʹ�ô��봴��
		//add by zhandb 2012.3.5
#if 0
		CString proDir = CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetPrintProductName();
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		CString proDir = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,620,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,621,prodir=%s",  proDir);
		}
#endif

		theLog.Write("prodir = %s",proDir);
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\��Ӵ�ӡ����.lnk", CCommonFun::GetDefaultPath()+"res\\prtwizard.ico", 0, "/Configprt");


		//��װ����ת������
		Install(m_sDefaultPath+"LmPrinterSvcHost.exe","-install",1,SW_HIDE);

		return TRUE;
	}
	//��װ��ӡվ��
	else if(stricmp(__argv[1],"-InstallPrt") == 0 || 
		stricmp(__argv[1],"/InstallPrt") == 0 ||
		stricmp(__argv[1],"-InstallPrt_print_edu") == 0 ||
		stricmp(__argv[1],"/InstallPrt_print_edu") == 0)
	{
		//���÷���ǽ
		theLog.Write("���÷���ǽ");

		CWinFireWallOp firewall;
		if (firewall.IsWinFireWallEnabled())
		{
			if (firewall.IsExceptModeEnabled())
			{
				firewall.EnableExceptMode();
			}
		}
		CString sExPath;
#if 0
		sExPath.Format("%sPrintStation.exe",m_sDefaultPath);
#else
		if (theConfig.IsPrintStationCloud())
		{
			sExPath.Format("%sPrintStationCloud.exe",m_sDefaultPath);
		}
		else
		{
			sExPath.Format("%sPrintStation.exe",m_sDefaultPath);
		}
#endif
		firewall.ExceptProgram(sExPath);
		if (m_bQueit)
		{
			theConfig.SavePrtStationConfig(CCommonFun::GetLocalIP());
		}
		else
		{
			CConfigPrtDlg config;
			config.DoModal();
		}
		

		//���һ��ͨ����
		CString sKey;
		sKey.Format(_T("%s\\cardConfig"),PROJECT_REGISTY_KEY);
		DWORD dw = SHDeleteKey(HKEY_LOCAL_MACHINE,sKey);
		theLog.Write("SHDeleteKey %d,err = %d",dw,::GetLastError());

		if(!m_bQueit)
		{
			//��Ӵ�ӡ��
			CStringArray ary;
			int nIndex = 0;
			CCommonFun::EnumeratePrinters(ary);
			if (ary.GetCount() > 0)
			{
				CInstallPrtDlg dlg;
				dlg.DoModal();
			}
			else
			{
				theLog.Write("no printer");
			}
		}
		
		// ��ӳ���˵���ݷ�ʽ
		//���ڷ������ʹ�ӡվ�㶼�ᰲװinstall.exe,û�а취�����Ƿ񴴽���ݷ�ʽ������ʹ�ô��봴��
		//add by zhandb 2012.3.5
#if 0 //del by zfq,2013.04.26
		CString proDir=CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetEduPrintProductName();
		if(stricmp(__argv[1],"-InstallPrt") == 0 || stricmp(__argv[1],"/InstallPrt") == 0)
			proDir = CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetPrintProductName();

		theLog.Write("__argv[1]=%s,prodir = %s", __argv[1]�� proDir);
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\��Ӵ�ӡ����.lnk", CCommonFun::GetDefaultPath()+"res\\prtwizard.ico", 0, "/Configprt");
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\���ô�ӡվ�����������.lnk", CCommonFun::GetDefaultPath()+"res\\prtsvr.ico", 0, "/ConfigprtCnnt");
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,700,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			CString proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,701,prodir=%s",  proDir);

			if(!CCommonFun::CreateShellLink(szDefPath + "Install.exe", 
				proDir+"\\��Ӵ�ӡ����.lnk", szDefPath + "res\\prtwizard.ico", 0, "/Configprt"))
			{
				theLog.Write("!!CInstallApp::ParseParam,702,CreateShellLink fail,szDefPath=%s,proDir=%s", szDefPath, proDir);
			}
			if(!CCommonFun::CreateShellLink(szDefPath + "Install.exe", 
				proDir+"\\���ô�ӡվ�����������.lnk", szDefPath + "res\\prtsvr.ico", 0, "/ConfigprtCnnt"))
			{
				theLog.Write("!!CInstallApp::ParseParam,703,CreateShellLink fail,szDefPath=%s,proDir=%s", szDefPath, proDir);
			}

		}
#endif
		return TRUE;
	}
	else if(stricmp(__argv[1],"-UnInstallweb") == 0 || stricmp(__argv[1],"/UnInstallweb") == 0)
	{
		
		CString sVirtualPath = m_sDefaultPath;
		sVirtualPath.TrimRight("\\");
		int nFind = sVirtualPath.ReverseFind('\\');
		if(nFind>=0)
		{
			sVirtualPath = sVirtualPath.Left(nFind);
		}
		theLog.Write("virdir = %s",sVirtualPath);
		CIISWeb web;
		CPtrList list;
		
		web.GetWebSiteList(&list);
		POSITION pos = list.GetHeadPosition();
		while(pos)
		{
			S_SITEINFO* pInfo = (S_SITEINFO*)list.GetNext(pos);
			theLog.Write("web nNumber:%d   szPort:%s  szIP:%s  szComment:%s   szVPath:%s\n",
				pInfo->nNumber,pInfo->szPort,pInfo->szIP,pInfo->szComment,pInfo->szVPath);
			CString sPath;
			web.GetVirtualWebDirPath(pInfo->nNumber,VIRTUALNAME,sPath);
			theLog.Write("%s %s",VIRTUALNAME,sPath);
			if (web.IsVirtualDirExist(VIRTUALNAME,pInfo->nNumber))
			{
				theLog.Write("%s exist",VIRTUALNAME);
				web.DeleteVirtualDir(VIRTUALNAME,pInfo->nNumber);
			}

			//ɾ������ӵ�վ��
			CString szComment(pInfo->szComment);
			if (szComment.CompareNoCase(PRINT_WEB_SITE) == 0)
			{
				theLog.Write("%s exist",PRINT_WEB_SITE);
				web.DeleteWebSite(pInfo->nNumber);
			}
		}
		web.RemoveAllList(list);
		web.RemoveAdminLnk();
		theLog.Write("UnInstallweb over");
		return TRUE;
	}
	//��װǶ��ʽ���ݿ� add by szy 20150511
	else if(stricmp(__argv[1], "-InstallLM_EmbedDB") == 0 ||
	    stricmp(__argv[1], "/InstallLM_EmbedDB") == 0)
	{
        CDlgCreateAppDB dlg;
        dlg.m_aryDb.Add(_T("iSec_PrinterApp"));
        dlg.DoModal();
        if (!dlg.m_bInstalledSuc)
        {
            //SetErrCode(1,"���ݿⰲװʧ��");
            return FALSE;
        }
        return TRUE;
	}
    else if(stricmp(__argv[1],"-UnInstallEmbedWeb") == 0 || stricmp(__argv[1],"/UnInstallEmbedWeb") == 0)
	{
        CString sVirtualPath = m_sDefaultPath;
        sVirtualPath.TrimRight("\\");
        int nFind = sVirtualPath.ReverseFind('\\');
        if(nFind>=0)
        {
            sVirtualPath = sVirtualPath.Left(nFind);
        }
        theLog.Write("virdir = %s",sVirtualPath);
        CIISWeb web;
        CPtrList list;

        CString webName(__argv[2]);
        theLog.Write("webName=%s", webName);

        web.GetWebSiteList(&list);
        POSITION pos = list.GetHeadPosition();
        while(pos)
        {
            S_SITEINFO* pInfo = (S_SITEINFO*)list.GetNext(pos);
            theLog.Write("web nNumber:%d   szPort:%s  szIP:%s  szComment:%s   szVPath:%s\n",
                pInfo->nNumber,pInfo->szPort,pInfo->szIP,pInfo->szComment,pInfo->szVPath);
            CString sPath;
            web.GetVirtualWebDirPath(pInfo->nNumber,webName,sPath);
            theLog.Write("%s %s",webName,sPath);
            if (web.IsVirtualDirExist(webName,pInfo->nNumber))
            {
                theLog.Write("%s exist",webName);
                web.DeleteVirtualDir(webName,pInfo->nNumber);
            }

            //ɾ������ӵ�վ��
            CString szComment(pInfo->szComment);
            if (szComment.CompareNoCase(PRINT_WEB_SITE) == 0)
            {
                theLog.Write("%s exist",PRINT_WEB_SITE);
                web.DeleteWebSite(pInfo->nNumber);
            }
        }
        web.RemoveAllList(list);
        web.RemoveEmbededWebLnk(webName);
        theLog.Write("UnInstallEmbedWeb end");
	}
	else if(stricmp(__argv[1],"-InstallPrintIdCard") == 0 || stricmp(__argv[1],"/InstallPrintIdCard") == 0)
	{
		typedef  BOOL (*PInstallDemo)(void);
		PInstallDemo InstallDemo = NULL;
		HMODULE hModule = NULL;
		do 
		{
			CString szDefPath = CCommonFun::GetDefaultPath();

			theLog.Write("##InstallPrintIdCard,4,��װ��ӡ���֤��szDefPath=%s", szDefPath);

			Install(m_sDefaultPath + "PrintHost.exe", "-install", 1, SW_HIDE);

			hModule = LoadLibraryA("eCC.dll");
			if (!hModule) 
			{
				theLog.Write("!!InstallPrintIdCard,1,fail err=%u", GetLastError());
				break;
			}

			InstallDemo = (PInstallDemo)GetProcAddress(hModule ,"InstallA");
			if (!InstallDemo)
			{
				theLog.Write("!!InstallPrintIdCard,2,fail err=%u", GetLastError());
				break;
			}

			if (!InstallDemo())
			{
				theLog.Write("!!InstallPrintIdCard,3,fail");
				break;
			}


		} while (FALSE);

		if (hModule)
		{
			FreeLibrary(hModule);
			hModule = NULL;
		}		
		theLog.Write("##InstallPrintIdCard,5,��װ��ӡ���֤��end");
		return TRUE;
	}
	else if(stricmp(__argv[1],"-UnInstallPrintIdCard") == 0 || stricmp(__argv[1],"/UnInstallPrintIdCard") == 0)
	{
		CString szDefPath = CCommonFun::GetDefaultPath();

		theLog.Write("##InstallPrintIdCard,4,ж�ش�ӡ���֤��szDefPath=%s", szDefPath);

		Install(m_sDefaultPath + "PrintHost.exe", "-UnInstall", INFINITE, SW_HIDE);

		FindTerminateProcess("PrintHost.exe");

		theLog.Write("##InstallPrintIdCard,4,ж�ش�ӡ���֤��end");
		return TRUE;
	}
	else if(stricmp(__argv[1],"-InstallSvrC") == 0 || stricmp(__argv[1],"/InstallSvrC") == 0)
	{
		if(CCommonFun::IsWinVistaOrLater()&&IsWow64())
		{
			CString sPath,sParam;		
			sPath.Format("%sx64\\Install.exe",m_sDefaultPath);
			sParam.Format("-RunasAdmin iSecSvcMgr.exe");
			theLog.Write("64λ��װ path:%s param:%s",sPath,sParam);
			Install(sPath,sParam,1,SW_HIDE);
		}
		InstallSvr(INTRANET_SVC_EXE);

		// ��ӳ���˵���ݷ�ʽ
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		CString proDir = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,50,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,51,prodir=%s",  proDir);
		}

		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\���÷��������ݿ�����.lnk", CCommonFun::GetDefaultPath()+"res\\svrdb.ico", 0, "/ConfigSvrDb");

		DealDbFunctionTree();

		//ע�᱾��Ϊ��ʾ�汾
		typedef  BOOL (*PInstallDemo)(void);
		PInstallDemo InstallDemo = NULL;
		HMODULE hModule = NULL;
		do 
		{
			CString szDefPath = CCommonFun::GetDefaultPath();

			theLog.Write("##InstallSvrC,1,��װ��ӡC�汾��szDefPath=%s", szDefPath);

			hModule = LoadLibraryA("eCC.dll");
			if (!hModule) 
			{
				theLog.Write("!!InstallSvrC,2,fail err=%u", GetLastError());
				break;
			}

			InstallDemo = (PInstallDemo)GetProcAddress(hModule ,"InstallC");
			if (!InstallDemo)
			{
				theLog.Write("!!InstallSvrC,3,fail err=%u", GetLastError());
				break;
			}

			if (!InstallDemo())
			{
				theLog.Write("!!InstallSvrC,4,fail");
				break;
			}

		} while (FALSE);

		if (hModule)
		{
			FreeLibrary(hModule);
			hModule = NULL;
		}		

		CCommonFun::StopSvc("iSecCSvcHost");
		Sleep(5000);
		CCommonFun::StartSvc("iSecCSvcHost");
		theLog.Write("##InstallPrintIdCard,5,��װ��ӡC�汾��end");
		return TRUE;
	}
	else if(stricmp(__argv[1],"-UnInstallSvrC") == 0 || stricmp(__argv[1],"/UnInstallSvrC") == 0)
	{
		UninstallSvr();

		try
		{
			CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
			CString szProductName = "";
			CString proDir = "";
			if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
			{
				theLog.Write("!!CInstallApp::ParseParam,330,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
			}
			else
			{
				proDir = szProgramDir + szProductName;
				theLog.Write("##CInstallApp::ParseParam,331,prodir=%s",  proDir);
			}

			if(PathFileExists(proDir+"\\��Ӵ�ӡ����.lnk"))
				CFile::Remove(proDir+"\\��Ӵ�ӡ����.lnk");
			if(PathFileExists(proDir+"\\���ô�ӡվ�����������.lnk"))
				CFile::Remove(proDir+"\\���ô�ӡվ�����������.lnk");
			if(PathFileExists(proDir+"\\���÷��������ݿ�����.lnk"))
				CFile::Remove(proDir+"\\���÷��������ݿ�����.lnk");

			CString run;
			run.Format("\"%ssc.exe\"  stop iSecUpdaterHttp"
				, CCommonFun::GetDefaultPath());
			CCommonFun::CreateProcessAndWait(run.GetBuffer());
			run.Format("\"%sinstsrv.exe\"  iSecUpdaterHttp remove"
				, CCommonFun::GetDefaultPath());
			CCommonFun::CreateProcessAndWait(run.GetBuffer());
		}
		catch (...)
		{
		}

		return TRUE;
	}
	else if(stricmp(__argv[1],"-InstallLMPrtC") == 0 || 
		stricmp(__argv[1],"/InstallLMPrtC") == 0 )
	{
		//���÷���ǽ
		theLog.Write("���÷���ǽ");

		CWinFireWallOp firewall;
		if (firewall.IsWinFireWallEnabled())
		{
			if (firewall.IsExceptModeEnabled())
			{
				firewall.EnableExceptMode();
			}
		}

		CString sExPath;
		sExPath.Format("%sLmPrinterSvcHost.exe",m_sDefaultPath);
		firewall.ExceptProgram(sExPath);

		//��װ����ת������
		Install(m_sDefaultPath+"LmConfig.exe","-config",INFINITE,SW_SHOW);

		//��Ӵ�ӡ��
		CStringArray ary;
		int nIndex = 0;
		CCommonFun::EnumeratePrinters(ary);
		if (ary.GetCount() > 0)
		{
			CInstallPrtDlg dlg;
			if(dlg.DoModal()==IDOK )
			{
			}
		}

		// ��ӳ���˵���ݷ�ʽ
		//���ڷ������ʹ�ӡվ�㶼�ᰲװinstall.exe,û�а취�����Ƿ񴴽���ݷ�ʽ������ʹ�ô��봴��
		//add by zhandb 2012.3.5
#if 0
		CString proDir = CCommonFun::GetProgramDir()+"\\"+CProductInfo::GetPrintProductName();
#else	//add by zfq,2013.04.26
		CString szProgramDir = CCommonFun::GetProgramDir()+"\\";
		CString szProductName = "";
		CString proDir = "";
		if(!CProductInfo::GetProductDirNameInProgramDir(szProgramDir, szProductName))
		{
			theLog.Write("!!CInstallApp::ParseParam,620,GetProductDirNameInProgramDir fail,szProgramDir=%s", szProgramDir);
		}
		else
		{
			proDir = szProgramDir + szProductName;
			CString szDefPath = CCommonFun::GetDefaultPath();
			theLog.Write("##CInstallApp::ParseParam,621,prodir=%s",  proDir);
		}
#endif

		theLog.Write("prodir = %s",proDir);
		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"Install.exe", 
			proDir+"\\��Ӵ�ӡ����.lnk", CCommonFun::GetDefaultPath()+"res\\prtwizard.ico", 0, "/Configprt");


		//��װ����ת������
		Install(m_sDefaultPath+"LmPrinterSvcHost.exe","-install",1,SW_HIDE);

		return TRUE;
	}
	else if(stricmp(__argv[1],"-InstallCloudPrintRouter") == 0 || 
		stricmp(__argv[1],"/InstallCloudPrintRouter") == 0 )
	{
		theLog.Write("��װ�ƴ�ӡ����,begin");

		CCommonFun::StopSMService("CloudPrintRouterHost");
		CCommonFun::TerminalProcess("CloudPrintRouterHost.exe");
		CCommonFun::TerminalProcess("CloudPrintMgr.exe");

		//��װ��װ�ƴ�ӡ���ط���
		Install(m_sDefaultPath+"CloudPrintRouterHost.exe","-install",INFINITE,SW_HIDE);

		CString szTitle = g_oIniFileConfig.GetVal("Version", "Title", "�ƴ�ӡ����");
		CString szLinkPath;
		szLinkPath.Format("%s\\%s.lnk", CCommonFun::GetCommonDesktopDir(), szTitle);

		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"CloudPrintMgr.exe", 
			szLinkPath, 
			CCommonFun::GetDefaultPath()+"res\\CloudPrintMgr.ico",
			0,
			0);

		theLog.Write("��װ�ƴ�ӡ����,end");

		return TRUE;
	}	///
	else if(stricmp(__argv[1],"-UnInstallCloudPrintRouter") == 0 || 
		stricmp(__argv[1],"/UnInstallCloudPrintRouter") == 0 )
	{
		theLog.Write("ж���ƴ�ӡ����,begin");

		CCommonFun::StopSMService("CloudPrintRouterHost");
		CCommonFun::TerminalProcess("CloudPrintRouterHost.exe");
		CCommonFun::TerminalProcess("CloudPrintMgr.exe");

		//��װ��װ�ƴ�ӡ���ط���
		Install(m_sDefaultPath+"CloudPrintRouterHost.exe","-uninstall",INFINITE,SW_HIDE);

		CString szTitle = g_oIniFileConfig.GetVal("Version", "Title", "�ƴ�ӡ����");
		CString szLinkPath;
		szLinkPath.Format("%s\\%s.lnk", CCommonFun::GetCommonDesktopDir(), szTitle);

		if (PathFileExists(szLinkPath))
		{
			DeleteFile(szLinkPath);
		}

		theLog.Write("ж���ƴ�ӡ����,end");

		return TRUE;
	}
	else if(stricmp(__argv[1],"-InstallCloudPrintDocConverter") == 0 || 
		stricmp(__argv[1],"/InstallCloudPrintDocConverter") == 0 )
	{
		theLog.Write("��װ�ƴ�ӡ�ĵ�ת����,begin");

		CCommonFun::TerminalProcess("CloudPrintDocConverter.exe");

		CRegistryEx reg(TRUE,FALSE);
		if(reg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
		{
			CString szRunExe;
			szRunExe.Format("\"%sCloudPrintDocConverter.exe\" /auto", m_sDefaultPath);
			if (reg.Write("CloudPrintDocConverter", szRunExe))
			{
				theLog.Write("set CloudPrintDocConverter suc szRunExe = %s", szRunExe);
			}
		}
		reg.Close();

		CCommonFun::CreateShellLink(CCommonFun::GetDefaultPath()+"CloudPrintDocConverter.exe", 
			CCommonFun::GetCommonDesktopDir()+"\\�ƴ�ӡ�ĵ�ת����.lnk", 
			CCommonFun::GetDefaultPath()+"res\\CloudPrintDocConverter.ico",
			0,
			0);

		theLog.Write("��װ�ƴ�ӡ�ĵ�ת����,end");

		return TRUE;
	}	///
	else if(stricmp(__argv[1],"-UnInstallCloudPrintDocConverter") == 0 || 
		stricmp(__argv[1],"/UnInstallCloudPrintDocConverter") == 0 )
	{
		theLog.Write("ж���ƴ�ӡ�ĵ�ת����,begin");

		CCommonFun::TerminalProcess("CloudPrintDocConverter.exe");

		CRegistryEx reg(TRUE,FALSE);
		if(reg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
		{
			if (reg.DeleteValue("CloudPrintDocConverter"))
			{
				theLog.Write("delete CloudPrintDocConverter suc");
			}
		}
		reg.Close();

		CString szlnk = CCommonFun::GetCommonDesktopDir()+"\\�ƴ�ӡ�ĵ�ת����.lnk";
		if (PathFileExists(szlnk))
		{
			DeleteFile(szlnk);
		}

		theLog.Write("ж���ƴ�ӡ�ĵ�ת����,end");

		return TRUE;
	}	
	return FALSE;
}
int CInstallApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	::CoUninitialize();
	CWinAppEx::ExitInstance();
	theLog.Write("ExitInstance %d",m_nExitCode);
	return m_nExitCode;
}
PVOID m_pOldValue = NULL;
void DisableFileRedirect()
{
	typedef BOOL (WINAPI* PFUN)(PVOID *OldValue);
	PFUN pFun = (PFUN)GetProcAddress(GetModuleHandle("kernel32"),"Wow64DisableWow64FsRedirection");
	if(NULL != pFun)
	{
		pFun(&m_pOldValue);
	}
}

void EnableFileRedirect()
{
	typedef BOOL (WINAPI* PFUN)(PVOID OldValue);
	PFUN pFun = (PFUN)GetProcAddress(GetModuleHandle("kernel32"),"Wow64RevertWow64FsRedirection");
	if(NULL != pFun)
	{
		pFun(m_pOldValue);
	}
}

BOOL Enable32BitAppOnWin64(BOOL bEnable)
{
	char chPath[MAX_PATH];
	GetWindowsDirectory(chPath,MAX_PATH);

	CString sPath;
	sPath.Format("%c:\\inetpub\\adminscripts\\adsutil.vbs",chPath[0]);
	CFileFind fd;

	if( !fd.FindFile(sPath) )
	{
		MessageBox(NULL,"û���ҵ�"+sPath+"�ļ�������32λ���л���ʧ�ܡ�","��ʾ",MB_OK);
		return FALSE;
	}
	CString sTmp;
	if( bEnable )
	{
		sTmp.Format("%s set W3SVC/AppPools/Enable32BitAppOnWin64 \"true\"",sPath);
	}
	else
	{
		sTmp.Format("%s set W3SVC/AppPools/Enable32BitAppOnWin64 \"false\"",sPath);
	}

	TCHAR szDir[MAX_PATH*2+16]={0};

	strcat(szDir,"CSCRIPT.EXE ");
	strcat(szDir,sTmp);

	STARTUPINFO StartupInfo= {0};
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION ProcessInfo;
	StartupInfo.cb=sizeof(STARTUPINFO); 

	if (!CreateProcess(NULL, (LPTSTR)szDir, NULL,NULL,FALSE,0,NULL,NULL, &StartupInfo, &ProcessInfo))
		return FALSE;

	WaitForSingleObject(ProcessInfo.hProcess ,3000);
	CloseHandle (ProcessInfo.hThread);
	CloseHandle (ProcessInfo.hProcess); 

	return TRUE;
}

BOOL MyExecuteCmdComd()
{
	if(!theApp.m_bQueit)
		MessageBox(0,"����ע��IIS����������Ҫ������ʱ�䣬�����ĵȴ�....","��ʾ",MB_ICONINFORMATION | MB_OK);

	DisableFileRedirect();

	char chPath[MAX_PATH];
	GetWindowsDirectory(chPath,MAX_PATH);

	EnableFileRedirect();

	CString szPath;
	szPath.Format("%s\\Microsoft.NET\\framework\\v2.0.50727\\aspnet_regiis.exe -i",chPath);

	TCHAR szDir[MAX_PATH*2+16]={0};
	strcat(szDir,szPath);

	STARTUPINFO StartupInfo= {0};

	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION ProcessInfo;
	StartupInfo.cb=sizeof(STARTUPINFO); 
	if (!CreateProcess(NULL, (LPTSTR)szDir, NULL,NULL,FALSE,0,NULL,NULL, &StartupInfo, &ProcessInfo))
	{
		return FALSE;
	}
	if(WAIT_TIMEOUT == WaitForSingleObject(ProcessInfo.hProcess ,1000*100))
	{
		AfxMessageBox("ע��IIS��������ʱ!");
		return FALSE;
	}
	CloseHandle (ProcessInfo.hThread);
	CloseHandle (ProcessInfo.hProcess); 

	return TRUE;
}

BOOL MyStartAspnet_StateService()
{
	BOOL bResult = FALSE;

	CRegistryEx pReg;
	pReg.SetRootKey(HKEY_LOCAL_MACHINE);
	if(pReg.Open("SYSTEM\\CurrentControlSet\\Services\\aspnet_state"))
	{
		pReg.Write("Start",(DWORD)2);
		pReg.Close();
	}
	else
	{
		return bResult;
	}

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM != NULL)
	{
		SC_HANDLE hService = OpenService(hSCM, "aspnet_state", SERVICE_START);
		if (hService != NULL)
		{
			if(StartService(hService,0,NULL))
			{
				bResult = TRUE;
			}
			else
			{
				if (ERROR_SERVICE_ALREADY_RUNNING == GetLastError())
				{
					bResult = TRUE;
				}
			}
			CloseServiceHandle(hService);
		}
		CloseServiceHandle(hSCM);
	}
	return bResult;
}

void CInstallApp::DelEncrypFile()
{
	//ɾ�������ļ�
	CStringArray sAry;
	sAry.Add("winacl.dll");
	sAry.Add("winemm.dll");
	sAry.Add("winfc8.dll");
	sAry.Add("winmc8.dll");

	//add by zxl,20151023,����5���Ǿ������ļ�
	sAry.Add("eCC.dll");
	sAry.Add("version.dat");
	sAry.Add("data.dat");
	sAry.Add("data2.dat");
	sAry.Add("data3.dat");


	CString sDes;
	for( int i=0;i<sAry.GetSize();i++)
	{
		sDes = m_sDefaultPath + sAry.GetAt(i);
		BOOL b = ::DeleteFile(sDes);
		theLog.Write("del secfile %d,%s",b,sDes);
	}
}
BOOL CInstallApp::CopyEnCryptFiles()
{
	CStringArray sAry;
	sAry.Add("winacl.dll");
	sAry.Add("winemm.dll");
	sAry.Add("winfc8.dll");
	sAry.Add("winmc8.dll");

	CString sDes,sSour;
	for( int i=0;i<sAry.GetSize();i++)
	{
		sSour = m_sInstallSourcePath + "\\" + sAry.GetAt(i);	
		sDes = m_sDefaultPath + sAry.GetAt(i);

		theLog.Write(sSour);
		theLog.Write(sDes);

		if(sSour.CompareNoCase(sDes) == 0) // �޸�ʱ����ִ����
			break;
		//ȥ��ֻ������
		CFileStatus status;
		if(CFile::GetStatus(sDes, status))
		{
			status.m_attribute &= ~0x01;
			try
			{
				CFile::SetStatus(sDes, status);
			}
			catch(...)
			{
				//CFileException �ܾ����ʣ�
			}
		}
		if( !CopyFile(sSour,sDes,FALSE) )
		{
			if( IDRETRY == MessageBox(NULL,"���������ļ�ʧ�ܣ�","��ʾ",MB_ICONERROR|MB_RETRYCANCEL))
			{
				i+=-1;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

//add by zxl,20151023,���������Ǿ������ļ�
BOOL CInstallApp::CopyEnCryptFilesC()
{
	CStringArray sAry;
	sAry.Add("eCC.dll");		//PrintIdCard�������ɵ�
	sAry.Add("version.dat");	//�汾��Ϣ

	CString sDes,sSour;
	for( int i=0;i<sAry.GetSize();i++)
	{
		sSour = m_sInstallSourcePath + "\\" + sAry.GetAt(i);	
		sDes = m_sDefaultPath + sAry.GetAt(i);

		theLog.Write(sSour);
		theLog.Write(sDes);

		if(sSour.CompareNoCase(sDes) == 0) // �޸�ʱ����ִ����
			break;
		//ȥ��ֻ������
		CFileStatus status;
		if(CFile::GetStatus(sDes, status))
		{
			status.m_attribute &= ~0x01;
			try
			{
				CFile::SetStatus(sDes, status);
			}
			catch(...)
			{
				//CFileException �ܾ����ʣ�
			}
		}
		if( !CopyFile(sSour,sDes,FALSE) )
		{
			if( IDRETRY == MessageBox(NULL,"���������ļ�ʧ�ܣ�","��ʾ",MB_ICONERROR|MB_RETRYCANCEL))
			{
				i+=-1;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

typedef int (* LP_SetupA)(HWND hwnd,LPLONG pInput1,LPSTR pInput2); 
typedef int (* LP_SetupB)(HWND hwnd,LPLONG pInput1,LPSTR pInput2); 
typedef int (* LP_SetupB)(HWND hwnd,LPLONG pInput1,LPSTR pInput2); 
BOOL CInstallApp::EncryptCheck()
{     
	BOOL bOk = FALSE;
	HMODULE hmd = LoadLibrary(TEXT("xinstall.dll"));
	if(hmd == NULL)
		return FALSE;

	//�жϰ汾
	LP_SetupB pSetupB = NULL;
	pSetupB = (LP_SetupB)GetProcAddress((HMODULE)hmd,TEXT("setupb"));
	if(pSetupB == NULL)
	{
		FreeModule(hmd);
		return FALSE;
	}

	LONG nVer,nValue;
	CString sSetupExe;

	sSetupExe.Format("%ssetup.exe",m_sDefaultPath);		
	nVer = pSetupB(NULL,&nValue,(LPSTR)(LPCTSTR)sSetupExe);

	CString sTmp;
	sTmp.Format("file=%s version=%d",sSetupExe,nVer);
	theLog.Write(sTmp);

	if(nVer == 101)//demo version		
	{
		FreeModule(hmd);
		//m_nExitCode = 0;
		return	TRUE;//demo version need nod to check serial number
	}

	//��װ���ܹ�����
	IntallDogDrv();

	if(nVer != 1)
	{
		MBHW(NULL, IDS_WRONG_VERSION);
		FreeModule(hmd);
		return FALSE;
	}

	//�ж����к�
	LP_SetupA pSetupA = NULL;
	pSetupA = (LP_SetupA)GetProcAddress((HMODULE)hmd,TEXT("setupa"));
	if(pSetupA == NULL)
	{
		FreeModule(hmd);
		return FALSE;
	}

	int nTry = 3;	
	CString sSerial;
	while(nTry > 0)
	{
		CEncryptDlg dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			sSerial = dlg.m_sSerial;
			long nTemp,nRet;
			nRet = pSetupA(NULL,&nTemp,(LPSTR)(LPCTSTR)sSerial);

			sTmp.Format("Serial=%s version=%d",sSerial,nRet);
			theLog.Write(sTmp);
			if(nRet == 101)
			{
				bOk = TRUE;
				break;
			}
			else
			{
				MBHW(NULL, IDS_WRONG_SERIAL);
			}			
		}
		else if (nResponse == IDCANCEL)
		{
			break;
		}	
		nTry--;	
	}

	FreeModule(hmd);

	if(!bOk)
	{
		MBHW(NULL, IDS_WRONG_SERIAL);
	}
	else
	{
		//�����к�д��ע���
// 		CRegistry2 general(2,FALSE);
// 		CString sKey = PROJECT_REGISTY_KEY;	 
// 		if( general.CreateKey(sKey) )
// 		{		
// 			// Database
// 			general.Write("SerialNumber",sSerial); 
// 		}
	}

	return bOk;
}

CString LoadS(int nText)
{
	CString s;
	s.LoadString(nText);
	return s;
}
//��װ���ܹ�����
void CInstallApp::IntallDogDrv()
{
	theLog.Write("install dog begin");
	CString sPath;
	CString sparam;
#if 0	//del by zfq,2013.07.08
	sPath.Format(_T("%ssetup.exe"),m_sDefaultPath);
#else	//add by zfq,2013.07.08
	if(IsWow64())
	{
		sPath.Format(_T("%sx64\\setup.exe"),m_sDefaultPath);
		theLog.Write("CInstallApp::IntallDogDrv,2,wow64");
	}
	else
	{
		sPath.Format(_T("%ssetup.exe"),m_sDefaultPath);
		theLog.Write("CInstallApp::IntallDogDrv,3,x86");
	}
#endif
	sparam.Format(_T(" /i/u/s"));
	Install(sPath,sparam,INFINITE,SW_HIDE);
	theLog.Write("install dog suc");
}

#if 0 //del by zfq,2013.06.24,�������
void CInstallApp::InstallSvr(CString sSvcName)
{
#if 1
	if (!CopyEnCryptFiles())
	{
		theLog.Write("CopyEnCryptFiles �汾����ȷ");
		m_nExitCode = 1;
		return ;
	}
	
	if (!EncryptCheck())
	{
		theLog.Write("EncryptCheck �汾����ȷ");
		m_nExitCode = 1;
		return ;
	}

	//���÷���ǽ
	theLog.Write("���÷���ǽ");

	CWinFireWallOp firewall;
	if (firewall.IsWinFireWallEnabled())
	{
		if (firewall.IsExceptModeEnabled())
		{
			firewall.EnableExceptMode();
		}
	}
	CString sExPath;
	sExPath.Format("%s%s",m_sDefaultPath,sSvcName);
	firewall.ExceptProgram(sExPath);
	sExPath.Format("%sDbSvcHost.exe",m_sDefaultPath);
	firewall.ExceptProgram(sExPath);
#endif



	if(m_bQueit)
	{

	}
	else
	{
		CSetDbDlg dlg;
		dlg.DoModal();
	}
	

	CString sUpdatePath = "";
	UINT nUpdateport = 0;

	//��װ����
	Install(m_sDefaultPath+sSvcName,"-install",1,SW_HIDE);

	//��װ����ת������
	Install(m_sDefaultPath+"DbSvcHost.exe","-install",1,SW_HIDE);

	//�����ļ�Ȩ��
	CString sPath;
	sPath.Format(_T("%s%s"),m_sDefaultPath,DB_SERVER_JOB_DIR);
	CreateDirectory(sPath,0);
	AddObjectAccess(sPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);
}
#else //add by zfq,2013.06.24,�������
void CInstallApp::InstallSvr(CString sSvcName)
{
#if 1
	if (sSvcName.CompareNoCase(INTRANET_SVC_EXE) == 0)
	{
		//�������ܲ�ͬ
		if (!CopyEnCryptFilesC())
		{
			theLog.Write("CopyEnCryptFiles �汾����ȷ");
			m_nExitCode = 1;
			return ;
		}
	}
	else
	{
		if (!CopyEnCryptFiles())
		{
			theLog.Write("CopyEnCryptFiles �汾����ȷ");
			m_nExitCode = 1;
			return ;
		}

		if (!EncryptCheck())
		{
			theLog.Write("EncryptCheck �汾����ȷ");
			m_nExitCode = 1;
			return ;
		}
	}

	//���÷���ǽ
	theLog.Write("���÷���ǽ");

	CWinFireWallOp firewall;
	if (firewall.IsWinFireWallEnabled())
	{
		if (firewall.IsExceptModeEnabled())
		{
			firewall.EnableExceptMode();
		}
	}
	CString sExPath;
	sExPath.Format("%s%s",m_sDefaultPath,sSvcName);
	firewall.ExceptProgram(sExPath);
	sExPath.Format("%sDbSvcHost.exe",m_sDefaultPath);
	firewall.ExceptProgram(sExPath);
#endif

	//add by zfq,2013.06.24,start
	CString sUpdatePath = "c:\\ServerUpdate";	
	UINT nUpdateport = 8080;	
	BOOL bNeedInstallUpdate = FALSE;	
	if(0 == sSvcName.CompareNoCase(PRINT_SVC_EXE)
		|| 0 == sSvcName.CompareNoCase(PRINT_EDU_SVC_EXE)
		|| 0 == sSvcName.CompareNoCase(INTRANET_SVC_EXE))
	{
		bNeedInstallUpdate = TRUE;
	}
	//add by zfq,2013.06.24,end

	if(m_bQueit)
	{//��Ĭ��װ
		if(bNeedInstallUpdate)//add by zfq,2013.06.24
		{
			theConfig.SaveUpdaterServerConfig(sUpdatePath,nUpdateport);
			if(!CCommonFun::CreateDir(sUpdatePath))
			{
				theLog.Write("!!CInstallApp::InstallSvr,20,CreateDir fail,err=%d,sUpdatePath=%s"
					, GetLastError(), sUpdatePath);
			}
			CString sPath,sParam;
			sPath.Format(_T("%sPatchUpdateSvc.exe"),m_sDefaultPath);
			sParam.Format(_T(" /Install"));
			Install(sPath, sParam, 1, SW_HIDE);
		}
	}
	else
	{
		CSetDbDlg dlg;
		dlg.DoModal();

		if(bNeedInstallUpdate)//add by zfq,2013.06.24
		{
			CSetIPDlg dlg2(TYPE_INTRANET_SERVER);
			if (dlg2.DoModal() == IDOK)	
			{
				sUpdatePath = dlg2.m_sUpdataPath;
				nUpdateport = dlg2.m_nUpdataPort;
				theConfig.SaveUpdaterServerConfig(sUpdatePath,nUpdateport);
				CCommonFun::CreateDir(sUpdatePath);

				CString sPath,sParam;
				sPath.Format(_T("%sPatchUpdateSvc.exe"),m_sDefaultPath);
				sParam.Format(_T(" /Install"));
				Install(sPath,sParam,1, SW_HIDE);
			}
		}
	}

#if 0
	CString sUpdatePath = "";
	UINT nUpdateport = 0;
#else
	sUpdatePath = "";
	nUpdateport = 0;
#endif


	//��װ����
	Install(m_sDefaultPath+sSvcName,"-install",1,SW_HIDE);

	//��װ����ת������
	Install(m_sDefaultPath+"DbSvcHost.exe","-install",1,SW_HIDE);

	//�����ļ�Ȩ��
	CString sPath;
	sPath.Format(_T("%s%s"),m_sDefaultPath,DB_SERVER_JOB_DIR);
	CreateDirectory(sPath,0);
	AddObjectAccess(sPath,ATL::Sids::Users(), GENERIC_ALL|STANDARD_RIGHTS_ALL,SE_FILE_OBJECT);
}
#endif

void CInstallApp::UninstallSvr()
{
	//ж�ط���
	FindTerminateProcess(PRINT_SVC_EXE);
	Install(m_sDefaultPath+PRINT_SVC_EXE,"-UnInstall",INFINITE,SW_HIDE);
	//ж�ط���
	FindTerminateProcess(PRINT_EDU_SVC_EXE);
	Install(m_sDefaultPath+PRINT_EDU_SVC_EXE,"-UnInstall",INFINITE,SW_HIDE);
	//ж�ط���
	FindTerminateProcess(INTRANET_SVC_EXE);
	Install(m_sDefaultPath+INTRANET_SVC_EXE,"-UnInstall",INFINITE,SW_HIDE);
	
	// �رշ���
	CCommonFun::StopSvc("iSecUpdaterHttp");
	CCommonFun::StopSvc("iSecUpdaterSvr");
//	CCommonFun::StopSvc("PatchUpdateSvc");	//add by zfq,2013.06.24,����Ҫ
	DeleteServerKey();

	// ɾ��������
	CString sUpHttpSvc;
	sUpHttpSvc.Format(" \"iSecUpdaterHttp\" remove");
	ShellExecute(NULL,NULL,m_sDefaultPath + "instsrv.exe",sUpHttpSvc,NULL,SW_HIDE);
	sUpHttpSvc.Format(" \"iSecUpdaterSvr\" remove");
	ShellExecute(NULL,NULL,m_sDefaultPath + "instsrv.exe",sUpHttpSvc,NULL,SW_HIDE);
#if 0 //����Ҫ
	sUpHttpSvc.Format(" \"PatchUpdateSvc\" remove");	//add by zfq,2013.06.24
	ShellExecute(NULL,NULL,m_sDefaultPath + "instsrv.exe",sUpHttpSvc,NULL,SW_HIDE);	//add by zfq,2013.06.24
#endif

	//ж�ط���
	FindTerminateProcess("DbSvcHost.exe");
	Install(m_sDefaultPath+"DbSvcHost.exe","-UnInstall",1,SW_HIDE);

	FindTerminateProcess("iSecSvcMgr.exe");

#if 0	//modify by zxl20130618
	FindTerminateProcess("printStation.exe");
#else
	if (theConfig.IsPrintStationCloud())
	{
		FindTerminateProcess("printStationCloud.exe");
	}
	else
	{
		FindTerminateProcess("printStation.exe");
	}
#endif

	FindTerminateProcess("manager.exe");
	FindTerminateProcess("PrintJobAudit.exe");
	
	//ж�����ݿ�
// 	if(::MessageBox(0,"�Ƿ�ɾ�����ݿ⣿","��ʾ",MB_YESNO | MB_ICONINFORMATION) == IDYES)
// 	{
// 
// 	}
	
	//ɾ��Ŀ¼
	CString sPath;
	sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),DB_SERVER_SCREEN_DIR);
	theLog.Write("del %s",sPath);
	CCommonFun::DeleteDir(sPath);

	sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),DB_SERVER_JOB_DIR);
	theLog.Write("del %s",sPath);
	CCommonFun::DeleteDir(sPath);

	sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),PRT_STATION_JOB_DIR);
	theLog.Write("del %s",sPath);
	CCommonFun::DeleteDir(sPath);

	sPath.Format(_T("%s%s"),CCommonFun::GetDefaultPath(),SYS_CONFIG_DIR);
	theLog.Write("del %s",sPath);
	CCommonFun::DeleteDir(sPath);

	//ɾ�������ļ�
	DelEncrypFile();

}

void CInstallApp::DeleteInstallKey()
{
	HKEY   m_key,m_key2;
	char   m_SubKey[255]="Software\\Microsoft\\Installer\\Products";

	DWORD   m_index=0,count=0;
	char   m_name[200],m_displayname[200],m_uninstall[200],uninstall[200];
	DWORD   m_namelen=200,m_displaynamelen=200,   m_uninstalllen=200;
	DWORD   m_attr=REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;	
//	CString sDisplayName = "�Ű�֮��iSecStar�ͻ���";	//del by zfq,2013.04.24
	CString sDisplayName = "iSecStar�ͻ���";	//add by zfq,2013.04.24
	//sDisplayName = EEGetProductNameVer();
	if   (RegOpenKeyEx(HKEY_CURRENT_USER,m_SubKey,0,KEY_ALL_ACCESS,&m_key)==ERROR_SUCCESS)
	{
		while   (RegEnumKeyEx(m_key,m_index,m_name,&m_namelen,0,NULL,NULL,0)!=ERROR_NO_MORE_ITEMS)
		{
			m_index++;
			if   (strcmp(m_name,"")!=NULL)
			{   
				strcpy(uninstall,m_SubKey);
				strcat(uninstall,"\\");
				strcat(uninstall,m_name);    
				m_displaynamelen=200;   
				memset(m_displayname,0,200);
				m_uninstalllen=200;
				memset(m_uninstall,0,200);


				if   (RegOpenKeyEx(HKEY_CURRENT_USER,uninstall,0,KEY_ALL_ACCESS,&m_key2)==ERROR_SUCCESS)
				{
					RegQueryValueEx(m_key2,"ProductName",0,&m_attr,LPBYTE(m_displayname),&m_displaynamelen);


					if (strcmp(m_displayname,"")!=NULL)
					{
						count++;
						CString sName = m_displayname;
						if(sName.CompareNoCase(sDisplayName)==0 || sName.Find(sDisplayName) >= 0) 
						{
							CString sSubKey;
							sSubKey = uninstall;
							int nRet = RegDeleteKey(HKEY_CURRENT_USER,sSubKey+"\\SourceList\\Media");

							nRet = RegDeleteKey(HKEY_CURRENT_USER,sSubKey+"\\SourceList\\Net");
							nRet = RegDeleteKey(HKEY_CURRENT_USER,sSubKey+"\\SourceList");
							nRet = RegDeleteKey(HKEY_CURRENT_USER,uninstall);

							theLog.Write("ɾ�� dispname = %s,name = %s",sDisplayName,sName);
						}
					}					
					m_displaynamelen=200;
					memset(m_displayname,0,200);
					m_uninstalllen=200;
					memset(m_uninstall,0,200);
				}

			}
			m_namelen=200;
			memset(m_name,0,200);
		}
		RegCloseKey(m_key);
	}   
}

void CInstallApp::CreateServerKey(LPCTSTR pszData, LPCTSTR pszData2)
{
	HKEY hKey = NULL;
	DWORD dw;
	do 
	{
		LONG ReturnValue = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterSvr\\Parameters", 0L, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw);
		if(ReturnValue != ERROR_SUCCESS)
			break;

		ReturnValue = RegSetValueEx (hKey, "Application", 0L, REG_SZ,(CONST BYTE*) pszData, strlen(pszData) + 1);
		if(ReturnValue != ERROR_SUCCESS)
			break;

		RegCloseKey(hKey);
		ReturnValue = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterHttp\\Parameters", 0L, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw);
		if(ReturnValue != ERROR_SUCCESS)
			break;

		ReturnValue = RegSetValueEx (hKey, "Application", 0L, REG_SZ,(CONST BYTE*) pszData2, strlen(pszData2) + 1);
		if(ReturnValue != ERROR_SUCCESS)
			break;
	} while (0);
	if (hKey)
		RegCloseKey(hKey);
}

void CInstallApp::DeleteServerKey()
{
	HKEY hKey = NULL;
	DWORD dw;

	LONG ReturnValue = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterSvr\\Parameters", 0L, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw);
	if(ReturnValue == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey,"Application");
		RegCloseKey(hKey);
	}
	
	ReturnValue = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\iSecUpdaterHttp\\Parameters", 0L, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw);
	if(ReturnValue == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey,"Application");
		RegCloseKey(hKey);
	}
}

void CInstallApp::DeleteInstallKey2()
{
	HKEY   m_key,m_key2;
	char   m_SubKey[255]="Installer\\Products";

	DWORD   m_index=0,count=0;
	char   m_name[200],m_displayname[200],m_uninstall[200],uninstall[200];
	DWORD   m_namelen=200,m_displaynamelen=200,   m_uninstalllen=200;
	DWORD   m_attr=REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;	
//	CString sDisplayName = "�Ű�֮��iSecStar�ͻ���";	//del by zfq,2013.04.24
	CString sDisplayName = "iSecStar�ͻ���";	//add by zfq,2013.04.24,�ĳ����Ե�����
	//sDisplayName = EEGetProductNameVer();
	if   (RegOpenKeyEx(HKEY_CLASSES_ROOT,m_SubKey,0,KEY_ALL_ACCESS,&m_key)==ERROR_SUCCESS)
	{
		while   (RegEnumKeyEx(m_key,m_index,m_name,&m_namelen,0,NULL,NULL,0)!=ERROR_NO_MORE_ITEMS)
		{
			m_index++;
			if   (strcmp(m_name,"")!=NULL)
			{   
				strcpy(uninstall,m_SubKey);
				strcat(uninstall,"\\");
				strcat(uninstall,m_name);    
				m_displaynamelen=200;   
				memset(m_displayname,0,200);
				m_uninstalllen=200;
				memset(m_uninstall,0,200);


				if   (RegOpenKeyEx(HKEY_CLASSES_ROOT,uninstall,0,KEY_ALL_ACCESS,&m_key2)==ERROR_SUCCESS)
				{
					RegQueryValueEx(m_key2,"ProductName",0,&m_attr,LPBYTE(m_displayname),&m_displaynamelen);


					if (strcmp(m_displayname,"")!=NULL)
					{
						count++;
						CString sName = m_displayname;
						if(sName.CompareNoCase(sDisplayName)==0 || sName.Find(sDisplayName) >= 0) 
						{
							CString sSubKey;
							sSubKey = uninstall;
							int nRet = RegDeleteKey(HKEY_CLASSES_ROOT,sSubKey+"\\SourceList\\Media");

							nRet = RegDeleteKey(HKEY_CLASSES_ROOT,sSubKey+"\\SourceList\\Net");
							nRet = RegDeleteKey(HKEY_CLASSES_ROOT,sSubKey+"\\SourceList");
							nRet = RegDeleteKey(HKEY_CLASSES_ROOT,uninstall);

							theLog.Write("ɾ��2 %s,%s",sDisplayName,sName);
						}
					}					
					m_displaynamelen=200;
					memset(m_displayname,0,200);
					m_uninstalllen=200;
					memset(m_uninstall,0,200);
				}

			}
			m_namelen=200;
			memset(m_name,0,200);
		}
		RegCloseKey(m_key);
	}   
}

void CInstallApp::DeleteInstallKey3()
{
	HKEY   m_key,m_key2;
	char   m_SubKey[255]="Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

	DWORD   m_index=0,count=0;
	char   m_name[200],m_displayname[200],m_uninstall[200],uninstall[200];
	DWORD   m_namelen=200,m_displaynamelen=200,   m_uninstalllen=200;
	DWORD   m_attr=REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;	

//	CString sDisplayName = "�Ű�֮��iSecStar�ͻ���";	//del by zfq,2013.04.24
	CString sDisplayName = "iSecStar�ͻ���";	//add by zfq,2013.04.24
	//sDisplayName = EEGetProductNameVer();
	if   (RegOpenKeyEx(HKEY_LOCAL_MACHINE,m_SubKey,0,KEY_ALL_ACCESS,&m_key)==ERROR_SUCCESS)
	{
		while   (RegEnumKeyEx(m_key,m_index,m_name,&m_namelen,0,NULL,NULL,0)!=ERROR_NO_MORE_ITEMS)
		{
			m_index++;
			if   (strcmp(m_name,"")!=NULL)
			{   
				strcpy(uninstall,m_SubKey);
				strcat(uninstall,"\\");
				strcat(uninstall,m_name);    
				m_displaynamelen=200;   
				memset(m_displayname,0,200);
				m_uninstalllen=200;
				memset(m_uninstall,0,200);


				if   (RegOpenKeyEx(HKEY_LOCAL_MACHINE,uninstall,0,KEY_ALL_ACCESS,&m_key2)==ERROR_SUCCESS)
				{
					RegQueryValueEx(m_key2,"DisplayName",0,&m_attr,LPBYTE(m_displayname),&m_displaynamelen);

					if (strcmp(m_displayname,"")!=NULL)
					{
						count++;
						CString sName = m_displayname;
						if(sName.CompareNoCase(sDisplayName)==0 || sName.Find(sDisplayName) >= 0) 
						{
							CString sSubKey;
							sSubKey = uninstall;
							int nRet = RegDeleteKey(HKEY_LOCAL_MACHINE,sSubKey+"\\SourceList\\Media");

							nRet = RegDeleteKey(HKEY_LOCAL_MACHINE,uninstall);

							theLog.Write("ɾ��3 %s,%s",sDisplayName,sName);
						}
					}					
					m_displaynamelen=200;
					memset(m_displayname,0,200);
					m_uninstalllen=200;
					memset(m_uninstall,0,200);
				}

			}
			m_namelen=200;
			memset(m_name,0,200);
		}
		RegCloseKey(m_key);
	}   
}

void CInstallApp::DealDbFunctionTree()
{
	theLog.Write("DealDbFunctionTree,1,begin");

	CString szIP = g_oIniFileConfig.GetVal("DBConfig","DbSvr",_T(""));
	CString szDbName = g_oIniFileConfig.GetVal("DBConfig","DbName",_T(""));
	CString szUserName = g_oIniFileConfig.GetVal("DBConfig","DbUser",_T(""));
	CString szPwd = g_oIniFileConfig.GetVal("DBConfig","DbPwd",_T(""));
	CString szPort = g_oIniFileConfig.GetVal("DBConfig","DbPort",_T("1433"));
	int nDbType = g_oIniFileConfig.GetVal("DBConfig","DbType",SQLSERVER);

#ifdef ENABLE_PWD_CRY
	char cPwd[MAX_PATH] = {0};
	int nSize = MAX_PATH;
	CPWDCry cry;
	cry.UnCryptPWD(szPwd.GetBuffer(),cPwd,nSize);
	szPwd = cPwd;
#endif

	CoInitialize(0);

	CAdo ado;
	ado.SetConnStr(szIP,szDbName,szUserName,szPwd,szPort);
	if (!ado.Connect())
	{
		theLog.Write("!!DealDbFunctionTree,2,Connect fail");
	}
	else
	{
		CString szSql;
		//		delete from t_s_FunctionTree where Code='10030000' and Name='�����û�����';
		szSql.Format(
		"delete from t_s_FunctionTree where Code='10010600' and Name='���������'; \
		delete from t_s_FunctionTree where Code='10010700' and Name='˫����ʾ��������'; \
		delete from t_s_FunctionTree where Code='10020200' and Name='��ӡ�������';\
		delete from t_s_FunctionTree where Code='10020300' and Name='��ӡ���������';\
		delete from t_s_FunctionTree where Code='10030300' and Name='��ӡ����';\
		delete from t_s_FunctionTree where Code='10050203' and Name='USB��ӡ�����ѯ';\
		delete from t_s_FunctionTree where Code='10050204' and Name='�Ѵ�ӡ�������ѯ';\
		delete from t_s_FunctionTree where Code='10050205' and Name='δ��ӡ�������ѯ';\
		delete from t_s_FunctionTree where Code='10051000' and Name='һ��ͨת�˼�¼';\
		delete from t_s_FunctionTree where Code='10051200' and Name='��ӡ����';\
		delete from t_s_FunctionTree where Code='10051300' and Name='�ɼ���ӡ';\
		delete from t_s_FunctionTree where Code='10051301' and Name='�Ѵ�ӡ�ɼ�����ѯ';\
		delete from t_s_FunctionTree where Code='10051302' and Name='�ɼ�����ѯ';\
		delete from t_s_FunctionTree where Code='10051400' and Name='֤����ӡ';\
		delete from t_s_FunctionTree where Code='10051401' and Name='֤����ӡ��ѯ';\
		delete from t_s_FunctionTree where Code='10051500' and Name='�����ӡ';\
		delete from t_s_FunctionTree where Code='10051501' and Name='�鿴����';\
		delete from t_s_FunctionTree where Code='10051502' and Name='��ӡ��¼';\
		delete from t_s_FunctionTree where Code='10060408' and Name='USB��ӡ�����ѯ';\
		delete from t_s_FunctionTree where Code='10060409' and Name='�Ѵ�ӡ�������ѯ';\
		delete from t_s_FunctionTree where Code='10060410' and Name='δ��ӡ�������ѯ';\
		delete from t_s_FunctionTree where Code='10060910' and Name='һ��ͨ��������־';\
		delete from t_s_FunctionTree where Code='10061000' and Name='Ƿ���˻���־';\
		delete from t_s_FunctionTree where Code='10061200' and Name='��ˮ�˲�ѯ';\
		delete from t_s_FunctionTree where Code='10070000' and Name='��ӡ��Ŀ����';\
		delete from t_s_FunctionTree where Code='10070100' and Name='��ӡ��Ŀ����';\
		delete from t_s_FunctionTree where Code='10080800' and Name='һ��ͨת�˱���';\
		delete from t_s_FunctionTree where Code='10080900' and Name='һ��ͨת��ͼ��';\
		delete from t_s_FunctionTree where Code='10081000' and Name='��������';\
		delete from t_s_FunctionTree where Code='10110000' and Name='�ɼ���ӡ';\
		delete from t_s_FunctionTree where Code='10110010' and Name='�ɼ�ģ��ѡ��';\
		delete from t_s_FunctionTree where Code='10110020' and Name='�ɼ���ӡ��ѯ';\
		delete from t_s_FunctionTree where Code='10110030' and Name='���ʲ�������';\
		delete from t_s_FunctionTree where Code='10120000' and Name='֤����ӡ';\
		delete from t_s_FunctionTree where Code='10120100' and Name='֤����ӡ��ѯ';\
		delete from t_s_FunctionTree where Code='10120200' and Name='���ʲ�������';\
		delete from t_s_FunctionTree where Code='10120300' and Name='֤��ģ������';\
		delete from t_s_FunctionTree where Code='10130000' and Name='�����ӡ';\
		delete from t_s_FunctionTree where Code='10130100' and Name='���ʲ�������';\
		delete from t_s_FunctionTree where Code='10130200' and Name='���淢������';\
		delete from t_s_FunctionTree where Code='10130300' and Name='�����ӡ��¼';\
		delete from t_s_FunctionTree where Code='10140000' and Name='��ӡί��';\
		delete from t_s_FunctionTree where Code='10150000' and Name='��ӡί��';\
		delete from t_s_FunctionTree where Code='10160000' and Name='���ɷѹ���';\
		delete from t_s_FunctionTree where Code='10160100' and Name='���ɷ�����';\
		delete from t_s_FunctionTree where Code='10160200' and Name='���ɷ�';\
		delete from t_s_FunctionTree where Code='10160300' and Name='���ɷ���־';");
		if (!ado.ExecuteSQL(szSql))
		{
			theLog.Write("!!DealDbFunctionTree,3,ExecuteSql fail");
		}
		ado.ReleaseConn();		
	}

	CoUninitialize();
}


//===========================
BOOL CDrvInstallHelper::Load()
{
	if (m_hModule)
	{
		return TRUE;
	}

	CString szDefPath = CCommonFun::GetDefaultPath();
	CString szDll;
	szDll.Format("%sDrvInstall.dll", szDefPath);

//	m_hModule = ::LoadLibrary(_T("DrvInstall.dll"));
	m_hModule = ::LoadLibrary(szDll);
	if (m_hModule)
	{
		theLog.Write("CDrvInstallHelper::Load,1,LoadLibrary DrvInstall.dll OK,szDll=%s"
			, szDll);

		return TRUE;
	}

	theLog.Write("!!CDrvInstallHelper::Load,2.0,LoadLibrary DrvInstall.dll fail,err=%d,szDll=%s"
		, GetLastError(), szDll);
	return FALSE;
}

bool CDrvInstallHelper::CallFun(TCHAR* szFun)
{
	typedef bool ( *XAB_INSTALL)(char *);
	XAB_INSTALL  pInstall=NULL;
	if(Load())
	{
		pInstall = (XAB_INSTALL)GetProcAddress (m_hModule, szFun);
		if (pInstall)
		{
			bool bRet = pInstall(m_szPath);
			if(!bRet)
			{
				theLog.Write("!!CDrvInstallHelper::CallFun,1,pInstall %s fail,err=%d", m_szPath, GetLastError());
			}
			else
			{
				theLog.Write("CDrvInstallHelper::CallFun,2,pInstall %s OK", m_szPath);
			}
			return bRet;
		}
		else
		{
			theLog.Write("!!CDrvInstallHelper::CallFun,3,GetProcAddress %s fail,err=%d,DefPath=%s"
				, szFun, GetLastError(), CCommonFun::GetDefaultPath());
			return false;
		}
	}

	theLog.Write("!!CDrvInstallHelper::CallFun,4,Load DrvInstall.dll fail,err=%d", GetLastError());
	return false;
}


//===========================

CString CProductInfo::GetEduPrintProductName()
{
	CString sVal = g_oIniFileSet.GetVal("General","product","");
	if(sVal.IsEmpty())
	{
		sVal = _T("�Ű�֮��У԰��ӡ��ӡ����ϵͳV5.0");
	}

	return sVal;
}

CString CProductInfo::GetPrintProductName()
{
	CString sVal = g_oIniFileSet.GetVal("General","product","");
	if(sVal.IsEmpty())
	{
		sVal = _T("�Ű�֮�Ǵ�ӡ����ϵͳV5.0");
	}

	return sVal;
}

//add by zfq,�ӳ���Ŀ¼�еĵ���ƷĿ¼,�����С���ӡ��ӡ����ϵͳ����Ŀ¼
BOOL CProductInfo::GetProductDirNameInProgramDir(CString &szProgramDir, CString &szProductDir)
{
	if(szProgramDir.IsEmpty())
	{
		theLog.Write("!!CProductInfo::GetProductDirNameInProgramDir,1,szProgramDir=%s", szProgramDir);
		return FALSE;
	}
	if(!::PathFileExists(szProgramDir))
	{
		theLog.Write("!!CProductInfo::GetProductDirNameInProgramDir,2,szProgramDir=%s,NOT Exists", szProgramDir);
		return FALSE;
	}

	//������Ŀ¼���ҵ����в�Ʒ��Ŀ¼
	CString szDestName = "��ӡ��ӡ����ϵͳ";
	CString szDestName2 = "�ƴ�ӡ����ƽ̨";
	CString szDestName3 = "��ʱ�������ն˹���ϵͳ";	//OEM
	CString szDestName4 = "�δ�Ϊ";	//OEM
	CString szDestName5 = "���Ǻ��";	//OEM
	CString szDestName6 = "��Ԫ�ǻ�";	//OEM
	CString szDestName7 = "����";		//OEM
	CFileFind	finder;				
	CString		sFullPath = _T("");					
	CString		strWildcard;

	BOOL BGet = FALSE;
	CString sDir = szProgramDir;
	sDir.TrimRight(_T("\\"));

	strWildcard = sDir + _T("\\*.*");
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		if (!finder.IsDirectory())
		{//����Ŀ¼
			continue;
		}
	
		CString sName = finder.GetFileName();
		CString sPath = finder.GetFilePath();

		if((-1 == sName.Find(szDestName)) && (-1 == sName.Find(szDestName2)) 
			&& (-1 == sName.Find(szDestName3)) && (-1 == sName.Find(szDestName4))
			&& (-1 == sName.Find(szDestName5)) && (-1 == sName.Find(szDestName6))
			&& (-1 == sName.Find(szDestName7))
			)
		{//����Ҫ�ҵ�Ŀ¼
			continue;
		}
	
		BGet = TRUE;
		szProductDir = sName;
		break;
	}
	finder.Close();	

	return BGet;
}
