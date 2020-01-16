// WebInf.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "WebInf.h"
#include "shlwapi.h"
#include "BaseSock.h"
#include "..\EncryptLib\PWDCry.h"
// adsi header
#include <activeds.h>
#include <Iads.h>
#include <Lm.h>
#include <Adshlp.h>
#include "CSmtp.h"	//add by zfq,2012.12.26

// adsi lib
#pragma comment (lib, "Activeds")
#pragma comment(lib, "adsiid")

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//#define INTERLOCK_G_LINSTANCE
//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CWebInfApp

BEGIN_MESSAGE_MAP(CWebInfApp, CWinApp)
END_MESSAGE_MAP()


// CWebInfApp ����

CWebInfApp::CWebInfApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CWebInfApp ����

CWebInfApp theApp;

// CWebInfApp ��ʼ��

BOOL CWebInfApp::InitInstance()
{
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		return FALSE;
	}



#if 0 //�ŵ��ӿ���
	gdiplusToken = 0;
	//��ʼ��gdi++
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

	return TRUE;
}

int CWebInfApp::ExitInstance()
{
	WriteLogEx("CWebInfApp::ExitInstance,1,gdiplusToken=%d", gdiplusToken);
#if 0 //�ŵ��ӿ���
	if(gdiplusToken)
	{
		GdiplusShutdown(gdiplusToken);
		gdiplusToken = 0;
	}
#endif

	WriteLogEx("CWebInfApp::ExitInstance,2");

	WSACleanup();	//add by zfq,2016-03-21

	WriteLogEx("CWebInfApp::ExitInstance,3");
	
	return CWinApp::ExitInstance();
}

void WriteLog(LPCTSTR szMsg,LPCTSTR szFileName)
{

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime = time.Format("%Y-%m-%d  %H:%M:%S ");

	try
	{
		CFile file;
		CString sTemp = strTime;
		//sTemp += "----->>>";
		sTemp += szMsg;
		if(file.Open(szFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
		{
			file.SeekToEnd();
			file.Write(sTemp,sTemp.GetLength());
			file.Write("\r\n",strlen("\r\n"));
			file.Close();
		}
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}
}

//��ȡ��־Ŀ¼
CString& GetLogDir()
{
	static CString g_szLogDir = "";
	if (g_szLogDir.IsEmpty())
	{
		OutputDebugString("##CCommonFun::GetLogDir");

		TCHAR path[MAX_PATH];
		CString sDefaultPath;
		if(GetModuleFileName(0,path,MAX_PATH))
		{
			TCHAR *pdest;
			pdest=_tcsrchr(path,_T('\\'));
			if(pdest) 
				*pdest=0;
			sDefaultPath=path;
			sDefaultPath += "\\xlog";
		}

		if (sDefaultPath.IsEmpty())
		{
			sDefaultPath = ".\\xlog";
		}

		g_szLogDir = sDefaultPath;
	}

	return g_szLogDir;
}
void WriteLogEx(LPCTSTR lpszFormat, ...)
{
	//��־����
// 	if(!CGlobalConfig::GetInstance().m_bEnableDebugLog)
// 	{
// 		return ;
// 	}
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);

	TRACE("%s\n",sTmp);

	CString sSysDir = GetLogDir();

	CFileFind fd;

	if( !fd.FindFile(sSysDir) )
	{
		if( !::CreateDirectory(sSysDir,NULL) )
			return;
	}
	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format("%y%m%d");

	CString sFileName;
	sFileName.Format("%s\\WebInf_%s.Log",sSysDir,sDate);

	WriteLog(sTmp,sFileName);
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

int GetEncoderClsidEx(const CString imagePath, CLSID* pClsid)
{
	WCHAR* format = NULL;
	CString csSuffix = GetFileExt(imagePath);
	if (0 == csSuffix.CompareNoCase("jpg")
		||0 == csSuffix.CompareNoCase("jpeg"))
	{
		format = L"image/jpeg";
	}
	else if (0 == csSuffix.CompareNoCase("tif")
		||0 == csSuffix.CompareNoCase("tiff"))
	{
		format = L"image/tiff";
	}
	else if (0 == csSuffix.CompareNoCase("png"))
	{
		format = L"image/png";
	}
	else if (0 == csSuffix.CompareNoCase("bmp"))
	{
		format = L"image/bmp";
	}
	else if (0 == csSuffix.CompareNoCase("gif"))
	{
		format = L"image/gif";
	}

	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void SetDoNetDBStr(char* pStr,int nSize)
{
	TCHAR pSysPath[MAX_PATH];
	memset(pSysPath,0,MAX_PATH);
	if(FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, pSysPath)))
		return;
	//GetSystemDirectory(pSysPath,MAX_PATH);
	CString szPath = pSysPath;
	CString sDir = szPath + _T("\\iSecStar");

	if(!PathFileExists(sDir))
		CreateDirectory(sDir, NULL);

	szPath = sDir + _T("\\iSec_PrintConfig.Dat");

	::DeleteFile(szPath);

	CFile file;
	if(file.Open(szPath,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		file.Write((LPCTSTR)pStr,nSize);
		file.Close();
	}
}
void GetDoNetDBStr(char* pDBOutStr,DWORD dw)//��ȡDONET �����ַ���
{
    strcpy(pDBOutStr,"");
    //if(dw == 4292967296)
    {
        TCHAR pSysPath[MAX_PATH];
        memset(pSysPath,0,MAX_PATH);
        if(FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, pSysPath))) 
            return;
        //GetSystemDirectory(pSysPath,MAX_PATH);

        CString szPath = pSysPath;
        szPath += _T("\\iSecStar\\iSec_PrintConfig.Dat");

        char cByte[1024] = {0};

        CFile file;
        if(file.Open(szPath,CFile::modeRead|CFile::typeText))
        {
            int nFileLen = file.GetLength();
            file.Read(cByte,nFileLen);
            file.Close();

            strcpy(pDBOutStr,cByte);
            //WriteLogEx("GetDoNetDBStr---Open�ɹ���%s",pDBOutStr);
        }
        else
            WriteLogEx("GetDoNetDBStr---Open fail(%s)��%d",szPath, GetLastError());

    }	
}

void SetDoNetAppDBStr(char* pStr,int nSize)
{
    TCHAR pSysPath[MAX_PATH];
    memset(pSysPath,0,MAX_PATH);
    if(FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, pSysPath))) 
        return;
    //GetSystemDirectory(pSysPath,MAX_PATH);
    CString szPath = pSysPath;
    CString sDir = szPath + _T("\\iSecStar");

    if(!PathFileExists(sDir))
        CreateDirectory(sDir, NULL);

    szPath = sDir + _T("\\iSecPrintApp.Dat");

    ::DeleteFile(szPath);

    CFile file;
    if(file.Open(szPath,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
    {
        file.Write((LPCTSTR)pStr,nSize);
        file.Close();
    }
}

void GetDoNetAppDBStr(char* pDBOutStr,DWORD dw)//��ȡDONET �����ַ���
{
    strcpy(pDBOutStr,"");
    //if(dw == 4292967296)
    {
        TCHAR pSysPath[MAX_PATH];
        memset(pSysPath,0,MAX_PATH);
        if(FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, pSysPath))) 
            return;
        //GetSystemDirectory(pSysPath,MAX_PATH);

        CString szPath = pSysPath;
        szPath += _T("\\iSecStar\\iSecPrintApp.Dat");

        char cByte[1024] = {0};

        CFile file;
        if(file.Open(szPath,CFile::modeRead|CFile::typeText))
        {
            int nFileLen = file.GetLength();
            file.Read(cByte,nFileLen);
            file.Close();

            strcpy(pDBOutStr,cByte);
            //WriteLogEx("GetDoNetDBStr---Open�ɹ���%s",pDBOutStr);
        }
        else
            WriteLogEx("GetDoNetDBStr---Open fail(%s)��%d",szPath, GetLastError());

    }	
}


void EncryptStr(char * strin,char * strout)			//����
{
#if ENABLE_PWD_CRY
	CPWDCry cry;
	char* pwd = cry.CryptPWD(strin);
	strcpy(strout, pwd);
	// delete pwd;	// δ���ԣ�������Ӱ�죬��й©�š� qilu@2012-1-4 9:02
#else 
	CString szStr = "";
	szStr.Format("%s",strin);
	sprintf(strout,"%s",szStr);
#endif
}
/*
void UnEncryptStr(char * strin,char * strout,DWORD dw)		//����
{
	if(dw = 4292967296)
	{
		CString szStr = "";
		szStr.Format("%s",strin);
		sprintf(strout,szStr);
	}
	else
	{
		sprintf(strout,"error!");
	}
}
*/
void StringSplit(const CString sSource, CStringArray *parArray , CString sSplitter,BOOL bTriNUll = FALSE)
{
	int nSplitLen = sSplitter.GetLength();

	CString sTemp(sSource);
	if(sTemp.IsEmpty())
		return ;
	while(sTemp)
	{
		int nSite = sTemp.Find(sSplitter);
		if(nSite == -1)
		{
			parArray->Add(sTemp);
			break;
		}

		CString sNew = sTemp.Left(nSite);
		if(bTriNUll)
		{
			sNew.TrimLeft(" ");
			sNew.TrimLeft("\t");
			sNew.TrimRight(" ");
			sNew.TrimRight("\t");
		}
		parArray->Add(sNew);

		sTemp = (LPCTSTR)sTemp + nSplitLen +nSite;
	}
}




BOOL CheckUserPasswordInDomainEx(CString strAccountName, CString strPassword, CString strDomain /*= ""*/)
{
	CoInitialize(NULL);
	HRESULT hr; 
	if (strDomain.GetLength() == 0)
	{
		HRESULT hr;
		IADsADSystemInfo *pSys = 0;
		hr = CoCreateInstance(CLSID_ADSystemInfo,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IADsADSystemInfo,
			(void**)&pSys);
		BSTR bstr;
		if (SUCCEEDED(hr) && SUCCEEDED(pSys->get_DomainDNSName(&bstr)))
		{
			strDomain = bstr;
			SysFreeString(bstr);
		}
		else
			strDomain = "rootDSE";
		if (pSys)
		{
			pSys->Release();
		}
	}
	CString str, str1, str2; 
	IADs *pADs = NULL; 
	BSTR bstrUrl, bstrName, bstrPass; 
	str="LDAP://"+ strDomain; 
	bstrUrl = str.AllocSysString(); 
	str1 = strDomain+"\\"+strAccountName; 
	bstrName = str1.AllocSysString(); 
	str2 = strPassword; 
	bstrPass = str2.AllocSysString(); 
	hr = ADsOpenObject( bstrUrl, bstrName, bstrPass, ADS_SECURE_AUTHENTICATION, IID_IADs, (void**)&pADs); 
	SysFreeString( bstrUrl );  
	SysFreeString( bstrName );  
	SysFreeString( bstrPass );  
	if ( hr != S_OK || NULL == pADs)  
	{
		WriteLogEx("!!CheckUserPasswordInDomainEx hr = %X",hr);
		return FALSE;
	}
	if (pADs)
	{
		pADs->Release();
		pADs = NULL;
	}

	CoUninitialize();
	return TRUE;
}

//=================================================add by zfq,2012.12.29,ע���û���֤,begin
long g_lCheckUserPwdInstance = 0;
int CheckUserPasswordInDomain(char* lpszAccountName, char* lpszPwd, char* lpszDomain)
{
#define DEF_UseSyncInCheckUserPwd 1
#ifdef DEF_UseSyncInCheckUserPwd
	if(InterlockedIncrement(&g_lCheckUserPwdInstance ) > 1)
	{
		WriteLogEx("!!CheckUserPasswordInDomain,A,1,g_lCheckUserPwdInstance = %d",g_lCheckUserPwdInstance);
		return 0;
	}
#endif //#ifdef DEF_UseSyncInCheckUserPwd

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString szAccountName, szPwd, szDomain;
	BOOL BRet = FALSE;
	if(!lpszAccountName || !lpszPwd || !lpszDomain)
	{
		WriteLogEx("!!CheckUserPasswordInDomain,A,1,lpszAccountName=%p,lpszPwd=%p,lpszDomain=%p"
			, lpszAccountName, lpszPwd, lpszDomain);
		goto ERR1;
	}

	szAccountName = lpszAccountName;
	szPwd = lpszPwd;
	szDomain = lpszDomain;
	if(szAccountName.IsEmpty() || szDomain.IsEmpty())
	{
		WriteLogEx("!!CheckUserPasswordInDomain,A,2,szAccountName=%s,szDomain=%s", szAccountName, szDomain);
		goto ERR1;
	}

	try
	{
		if(!CheckUserPasswordInDomainEx(szAccountName, szPwd, szDomain))
		{
			WriteLogEx("!!CheckUserPasswordInDomain,A,3,szAccountName=%s,szPwd=%s,szDomain=%s"
				, szAccountName, szPwd, szDomain);
			goto ERR1;
		}
	}
	catch (...)
	{
		WriteLogEx("!!CheckUserPasswordInDomain,A,4,catch sth.,szAccountName=%s,szPwd=%s,szDomain=%s"
			, szAccountName, szPwd, szDomain);
		goto ERR1;		
	}

#ifdef DEF_UseSyncInCheckUserPwd
	InterlockedDecrement(&g_lCheckUserPwdInstance);
#endif
	return 1;
ERR1:
#ifdef DEF_UseSyncInCheckUserPwd
	InterlockedDecrement(&g_lCheckUserPwdInstance);
#endif
	return 0;
}

long g_lCheckMailAddrInstance = 0;
int CheckMailAddr(char* pMailAddr, char* pPwd, char* pServAddr, int nServPort)
{
//#define DEF_UseSyncInCheckMailAddr	1
#if DEF_UseSyncInCheckMailAddr
	if(InterlockedIncrement(&g_lCheckMailAddrInstance ) > 1)
	{
		WriteLogEx("!!CheckMailAddr,1.0,g_lCheckMailAddrInstance=%d",g_lCheckMailAddrInstance);
		return 0;
	}
#endif

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSmtp *pSmtp = NULL;
	CString szMailAddr, szPwd, szServAddr;
	BOOL BRet = FALSE;
	if(!pMailAddr || !pPwd || !pServAddr || 0 >= nServPort)
	{
		WriteLogEx("!!CheckMailAddr,2,pMailAddr=%p,pPwd=%p,pServAddr=%p,nServPort=%d"
			, pMailAddr, pPwd, pServAddr, nServPort);
		goto ERR1;
	}

	szMailAddr = pMailAddr;
	szPwd = pPwd;
	szServAddr = pServAddr;
	if(szMailAddr.IsEmpty() || szServAddr.IsEmpty())
	{
		WriteLogEx("!!CheckMailAddr,3,szMailAddr=%s,szServAddr=%s", szMailAddr, szServAddr);
		goto ERR1;
	}

	WriteLogEx("##CheckMailAddr,3.1");

	try
	{
		pSmtp = new CSmtp;
	}
	catch(const ECSmtp&)
	{
		WriteLogEx("!!CheckMailAddr,4.01,new CSmtp,catch sth.");
		goto ERR2;
	}
	catch (...)
	{
		WriteLogEx("!!CheckMailAddr,4.02,new CSmtp,catch sth.");
		goto ERR2;
	}

	WriteLogEx("##CheckMailAddr,4.1");

	try
	{
		if(!pSmtp->ConnectRemoteServer(pServAddr, nServPort, DO_NOT_SET, true, pMailAddr, pPwd))
		{
			WriteLogEx("!!CheckMailAddr,5,ConnectRemoteServer fail,pServAddr=%s,nServPort=%d,usr=%s,pwd=%s"
				, pServAddr, nServPort, pMailAddr, pPwd);
			goto ERR2;
		}
	}
	catch(const ECSmtp&)
	{
		WriteLogEx("!!CheckMailAddr,5.2,ConnectRemoteServer catch sth,pServAddr=%s,nServPort=%d,usr=%s,pwd=%s"
			, pServAddr, nServPort, pMailAddr, pPwd);
		goto ERR2;
	}
	catch(...)
	{
		WriteLogEx("!!CheckMailAddr,6,ConnectRemoteServer catch sth,pServAddr=%s,nServPort=%d,usr=%s,pwd=%s"
			, pServAddr, nServPort, pMailAddr, pPwd);
		goto ERR2;
	}

	WriteLogEx("##CheckMailAddr,6.1");

	try
	{
		delete pSmtp;
		pSmtp = NULL;
	}
	catch(...)
	{
		WriteLogEx("!!CheckMailAddr,7,delete catch sth,pServAddr=%s,nServPort=%d,usr=%s,pwd=%s"
			, pServAddr, nServPort, pMailAddr, pPwd);
		goto ERR1;
	}

	WriteLogEx("##CheckMailAddr,7.1");

#if DEF_UseSyncInCheckMailAddr
	InterlockedDecrement(&g_lCheckMailAddrInstance);
#endif

	return 1;
ERR2:
	if(pSmtp)
	{
		try
		{
			delete pSmtp;
			pSmtp = NULL;
		}
		catch(...)
		{
			WriteLogEx("!!CheckMailAddr,17,delete catch sth,pServAddr=%s,nServPort=%d,usr=%s,pwd=%s"
				, pServAddr, nServPort, pMailAddr, pPwd);
		}

	}
ERR1:
#if DEF_UseSyncInCheckMailAddr
	InterlockedDecrement(&g_lCheckMailAddrInstance);
#endif
	return 0;
}
//=================================================add by zfq,2012.12.29,ע���û���֤,end

int CheckPwd(char* pwd, char* encpwd)
{
#ifdef ENABLE_PWD_CRY
	CPWDCry cry;
	return cry.CheckPWD(-1,pwd,encpwd)==TRUE?1:0;
#else
	return strcmp(pwd, encpwd)==0?1:0;
#endif
}

//$�ָ�
//id$name$Dbpwd$State$domainaddr$Pwd$AuditOk
int CheckUser(char* pszInfo)
{
	WriteLogEx("CheckUser %s",pszInfo);
	int dwErr = ISEC_LOGON_ERR_NOUSER;
	if(!pszInfo )
		return dwErr;
	CStringArray ary;
	int nSize = strlen(pszInfo);
	StringSplit(pszInfo,&ary,"\n");
	
	BOOL bAuditOK = FALSE;
	int nState = ISEC_USER_ENABLE;
	CString sDbPwd ;
	CString sName;
	CString sPwd;
	CString sDomain;
	do 
	{
		if (ary.GetCount() < 7)
		{
			break;
		}
		nState = atoi(ary.GetAt(3));
		if (nState != ISEC_USER_ENABLE)
		{
			dwErr = ISEC_LOGON_ERR_DISABLE;
			break;
		}
		int nUserId = atoi(ary.GetAt(0));
		sName = ary.GetAt(1);
		sDbPwd = ary.GetAt(2);
		sDomain = ary.GetAt(4);
		sPwd = ary.GetAt(5);

		if (!sDomain.IsEmpty())
		{
//			if(!CheckUserPasswordInDomain(sName,sPwd,sDomain))	//delete by zfq,2012.12.26
			if(!CheckUserPasswordInDomainEx(sName,sPwd,sDomain))	//add by zfq,2012.12.26
			{
				WriteLogEx("CheckUserPasswordInDomain err %s,%s,%s",sName,sPwd,sDomain);
				dwErr = ISEC_LOGON_ERR_PWD;
				nUserId = 0;
				break;
			}
		}
		else
		{

#ifdef ENABLE_PWD_CRY
			CPWDCry cry;
			if(!cry.CheckPWD(nUserId,sPwd.GetBuffer(),sDbPwd.GetBuffer()))
			{
				dwErr = ISEC_LOGON_ERR_PWD;
				nUserId = 0;
				break;
			}
#else
			if (sDbPwd.Compare(sPwd) != 0)
			{
				dwErr = ISEC_LOGON_ERR_PWD;
				nUserId = 0;
				break;
			}
#endif
		}

		dwErr = ISEC_LOGON_SUC;

	} while (0);

	return dwErr;
}
//$�ָ�
//id$name$Dbpwd$State$domainaddr$Pwd$AuditOk
int CheckUserCryPwd(char* pszInfo)
{
	WriteLogEx("CheckUserCryPwd %s",pszInfo);
	int dwErr = ISEC_LOGON_ERR_NOUSER;
	if(!pszInfo )
		return dwErr;
	CStringArray ary;
	int nSize = strlen(pszInfo);
	StringSplit(pszInfo,&ary,"\n");
	
	BOOL bAuditOK = FALSE;
	int nState = ISEC_USER_ENABLE;
	CString sDbPwd ;
	CString sName;
	CString sPwd;
	CString sDomain;
	do 
	{
		if (ary.GetCount() < 7)
		{
			break;
		}
		nState = atoi(ary.GetAt(3));
		if (nState != ISEC_USER_ENABLE)
		{
			dwErr = ISEC_LOGON_ERR_DISABLE;
			break;
		}
		int nUserId = atoi(ary.GetAt(0));
		sName = ary.GetAt(1);
		sDbPwd = ary.GetAt(2);
		sDomain = ary.GetAt(4);
		sPwd = ary.GetAt(5);

		//WriteLogEx("sName=%s,sDbPwd=%s,sDomain=%s,sPwd=%s",sName, sDbPwd, sDomain, sPwd);
		//��������н��ܲ���
		CPWDCry cry2;
		char cPwd2[MAX_PATH] = {0};
		int nSize = MAX_PATH;
		if (cry2.UnCryptPWD(sPwd.GetBuffer(),cPwd2,nSize))
		{
			sPwd = cPwd2;
		}
		//WriteLogEx("sName=%s,sDbPwd=%s,sDomain=%s,sPwd=%s",sName, sDbPwd, sDomain, sPwd);

		if (!sDomain.IsEmpty())
		{
//			if(!CheckUserPasswordInDomain(sName,sPwd,sDomain))	//delete by zfq,2012.12.26
			if(!CheckUserPasswordInDomainEx(sName,sPwd,sDomain))	//add by zfq,2012.12.26
			{
				WriteLogEx("CheckUserPasswordInDomain err %s,%s,%s",sName,sPwd,sDomain);
				dwErr = ISEC_LOGON_ERR_PWD;
				nUserId = 0;
				break;
			}
		}
		else
		{

#ifdef ENABLE_PWD_CRY
			CPWDCry cry;
			if(!cry.CheckPWD(nUserId,sPwd.GetBuffer(),sDbPwd.GetBuffer()))
			{
				dwErr = ISEC_LOGON_ERR_PWD;
				nUserId = 0;
				break;
			}
#else
			if (sDbPwd.Compare(sPwd) != 0)
			{
				dwErr = ISEC_LOGON_ERR_PWD;
				nUserId = 0;
				break;
			}
#endif
		}

		dwErr = ISEC_LOGON_SUC;

	} while (0);

	return dwErr;
}
long g_lInstance = 0;
BOOL NotifyFromWeb(int nMask)
{
	if(InterlockedIncrement(&g_lInstance) > 1)
	{
		WriteLogEx("�Ѿ����� NotifyFromWeb = %d",g_lInstance);
		return FALSE;
	}
#if 0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 1;
	}
#endif
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WriteLogEx("NotifyFromWeb %d",nMask);
	//return 1;
	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	CBaseSock	sock2;
	int nWebPort = 0;

	int nRet = 0;
	CString sLocalIP,
		sHost,sIP;
	CString sEquipID;
	EQUIP_INFO equip = {0};
	sLocalIP = "127.0.0.1";
	//	sMac = CCommonFun::GetLocalMac();
	sHost = "IIS-Host";
	sIP = "127.0.0.1";
	nWebPort = POLICY_TCP_PORT;
	
	int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
	BYTE* pBuf = new BYTE[nSize];
	if (!pBuf)
	{
		WriteLogEx("NotifyFromWeb err 000 %d",nMask);
		InterlockedDecrement(&g_lInstance);
		return FALSE;
	}

	ZeroMemory(pBuf,nSize);
	NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
	pHead->nPackMask = PACK_SMALL;
	pHead->nIndentify = PACKINDENTIFY;

	try
	{
		do 
		{			
			//֪ͨpolicyserver
			if(!ServerSocket.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
			{
				WriteLogEx("Connect error");
				break ;
			}

			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP,sIP);
			strcpy(equip.szHost,sHost);
			WriteLogEx("ip=%s",sIP);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 22 err %d",::WSAGetLastError());
				break;	
			}
			pHead->nMainCmd = WM_POLICY_CHANGED;
			pHead->nSubCmd = nMask;
			pHead->nPackBodySize = 0;
			if (ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 33 err %d",::WSAGetLastError());
				break;
			}

			::Sleep(100);

			//--------------------
			//֪ͨdbserver
			nWebPort = DBSVR_TCP_PORT;
			if(!sock2.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
			{
				WriteLogEx("Connect error");
				break ;
			}

			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP,sIP);
			strcpy(equip.szHost,sHost);
			WriteLogEx("ip=%s",sIP);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(sock2.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 44 err %d",::WSAGetLastError());
				break;	
			}
			pHead->nMainCmd = WM_POLICY_CHANGED;
			pHead->nSubCmd = nMask;
			pHead->nPackBodySize = 0;
			if (sock2.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 55 err %d",::WSAGetLastError());
				break;
			}

			::Sleep(2000);
// 			while(1)
// 			{
// 				ServerSocket.SetTimeOut(2000);
// 		//		int lbuffersize=ServerSocket.Receive(pBuf,nSize);
// 				ServerSocket.KillTimeOut();
// 				if(ServerSocket.m_bTimeOut)
// 					break;
// 				if(lbuffersize<0)
// 					break;	 
// 			}
			bSuc = TRUE;
//			ServerSocket.Close();
			WriteLogEx("NotifyFromWeb suc 000 %d",nMask);
		} while (0);
		
	}
	catch (...)
	{
	}
	
	if (pBuf)
	{
		delete pBuf;
	}
	/*WSACleanup( );*/

	InterlockedDecrement(&g_lInstance);
	WriteLogEx("NotifyFromWeb suc 222 %d",g_lInstance);
	return bSuc;
}
//�ͻ�������
BOOL NotifyCltUpdateFromWeb(int nMask)
{
	if(InterlockedIncrement(&g_lInstance) > 1)
	{
		WriteLogEx("�Ѿ����� NotifyFromWeb = %d",g_lInstance);
		return FALSE;
	}
#if 0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 1;
	}
#endif
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WriteLogEx("NotifyCltUpdateFromWeb %d",nMask);
	//return 1;
	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	CBaseSock	sock2;
	int nWebPort = 0;

	int nRet = 0;
	CString sLocalIP,
		sHost,sIP;
	CString sEquipID;
	EQUIP_INFO equip = {0};
	sLocalIP = "127.0.0.1";
	//	sMac = CCommonFun::GetLocalMac();
	sHost = "IIS-Host";
	sIP = "127.0.0.1";
	nWebPort = POLICY_TCP_PORT;

	int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
	BYTE* pBuf = new BYTE[nSize];
	if (!pBuf)
	{
		WriteLogEx("NotifyFromWeb err 000 %d",nMask);
		InterlockedDecrement(&g_lInstance);
		return FALSE;
	}

	ZeroMemory(pBuf,nSize);
	NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
	pHead->nPackMask = PACK_SMALL;
	pHead->nIndentify = PACKINDENTIFY;

	try
	{
		do 
		{			
			//֪ͨpolicyserver
			if(!ServerSocket.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
			{
				WriteLogEx("Connect error");
				break ;
			}

			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP,sIP);
			strcpy(equip.szHost,sHost);
			WriteLogEx("ip=%s",sIP);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 22 err %d",::WSAGetLastError());
				break;	
			}
			pHead->nMainCmd = WM_CLIENT_UPDATE;
			pHead->nSubCmd = nMask;
			pHead->nPackBodySize = 0;
			if (ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 33 err %d",::WSAGetLastError());
				break;
			}

						

			::Sleep(2000);
			// 			while(1)
			// 			{
			// 				ServerSocket.SetTimeOut(2000);
			// 		//		int lbuffersize=ServerSocket.Receive(pBuf,nSize);
			// 				ServerSocket.KillTimeOut();
			// 				if(ServerSocket.m_bTimeOut)
			// 					break;
			// 				if(lbuffersize<0)
			// 					break;	 
			// 			}
			bSuc = TRUE;
			//			ServerSocket.Close();
			WriteLogEx("NotifyFromWeb suc 000 %d",nMask);
		} while (0);

	}
	catch (...)
	{
	}

	if (pBuf)
	{
		delete pBuf;
	}
	/*WSACleanup( );*/

	InterlockedDecrement(&g_lInstance);
	WriteLogEx("NotifyFromWeb suc 222 %d",g_lInstance);
	return bSuc;
}

//�ͻ�������
BOOL NotifyCltUpdateFromWebEx(char *pszIDs)
{
	if(InterlockedIncrement(&g_lInstance) > 1)
	{
		WriteLogEx("!!NotifyCltUpdateFromWebEx,1,�Ѿ����� NotifyFromWeb = %d",g_lInstance);
		return FALSE;
	}
#if 0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 1;
	}
#endif
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString szIDs;
	szIDs.Format("%s", pszIDs);
	WriteLogEx("NotifyCltUpdateFromWebEx,2,szIDs=%s", szIDs);
	int nMsgLen = szIDs.GetLength();
	if(1 >= nMsgLen)
	{
		WriteLogEx("!!NotifyCltUpdateFromWebEx,3,nMsgLen=%d,szIDs=%s", nMsgLen, szIDs);
		InterlockedDecrement(&g_lInstance);
		return FALSE;
	}
	//return 1;
	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	CBaseSock	sock2;
	int nWebPort = 0;

	int nRet = 0;
	CString sLocalIP,
		sHost,sIP;
	CString sEquipID;
	EQUIP_INFO equip = {0};
	sLocalIP = "127.0.0.1";
	//	sMac = CCommonFun::GetLocalMac();
	sHost = "IIS-Host";
	sIP = "127.0.0.1";
	nWebPort = POLICY_TCP_PORT;

	int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip) + nMsgLen + 1;
	BYTE* pBuf = new BYTE[nSize];
	if (!pBuf)
	{
		WriteLogEx("!!NotifyCltUpdateFromWebEx,5, szIDs=%s", szIDs);
		InterlockedDecrement(&g_lInstance);
		return FALSE;
	}

	ZeroMemory(pBuf,nSize);
	NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
	pHead->nPackMask = PACK_SMALL;
	pHead->nIndentify = PACKINDENTIFY;

	try
	{
		do 
		{			
			//֪ͨpolicyserver
			if(!ServerSocket.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
			{
				WriteLogEx("Connect error");
				break ;
			}

			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP,sIP);
			strcpy(equip.szHost,sHost);
			WriteLogEx("ip=%s",sIP);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("!!NotifyCltUpdateFromWebEx,6,Send 22 err %d",::WSAGetLastError());
				break;	
			}


			pHead->nMainCmd = WM_CLIENT_UPDATE;
			pHead->nSubCmd = -1;
			pHead->nPackBodySize = nMsgLen + 1;
			memset(pHead + 1, 0x0, pHead->nPackBodySize);
			memcpy(pHead + 1, szIDs, nMsgLen);
			if (ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("!!NotifyCltUpdateFromWebEx,7,Send 33 err %d,szIDs=%s",::WSAGetLastError(), szIDs);
				break;
			}



			::Sleep(2000);
			// 			while(1)
			// 			{
			// 				ServerSocket.SetTimeOut(2000);
			// 		//		int lbuffersize=ServerSocket.Receive(pBuf,nSize);
			// 				ServerSocket.KillTimeOut();
			// 				if(ServerSocket.m_bTimeOut)
			// 					break;
			// 				if(lbuffersize<0)
			// 					break;	 
			// 			}
			bSuc = TRUE;
			//			ServerSocket.Close();
			WriteLogEx("==NotifyCltUpdateFromWebEx,8,suc szIDs=%s",szIDs);
		} while (0);

	}
	catch (...)
	{
	}

	if (pBuf)
	{
		delete pBuf;
	}
	/*WSACleanup( );*/

	InterlockedDecrement(&g_lInstance);
	WriteLogEx("NotifyFromWeb suc 222 %d",g_lInstance);
	return bSuc;
}

BOOL SubmitPrintFile(int nAccountId,char* szFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//add by zfq,2012.12.18,begin
	if(!szFile)
	{
		WriteLogEx("!!SubmitPrintFile,1,%d,szFile=%p", nAccountId, szFile);
		return FALSE;
	}
	int nStrLen = ::strlen(szFile);
	if(MAX_PATH < nStrLen)
	{
		WriteLogEx("!!SubmitPrintFile,2,nStrLen=%d,szFile=%s", nStrLen, szFile);
		return FALSE;
	}
	//add by zfq,2012.12.18,
	WriteLogEx("SubmitPrintFile,3, %d,%s",nAccountId,szFile);
	
	

	//return 1;
	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	CBaseSock	sock2;
	int nWebPort = 0;

	int nRet = 0;
	CString sLocalIP,
		sHost,sIP;
	CString sEquipID;
	EQUIP_INFO equip = {0};
	sLocalIP = "127.0.0.1";
	//	sMac = CCommonFun::GetLocalMac();
	sHost = "IIS-Host";
	sIP = "127.0.0.1";
	nWebPort = POLICY_TCP_PORT;

	int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip) + sizeof(CWebSubmitJob);
	BYTE* pBuf = new BYTE[nSize];
	if (!pBuf)
	{
		//WriteLogEx("NotifyFromWeb err 000 %d",nMask);
		return FALSE;
	}

	WriteLogEx("SubmitPrintFile,20");

	ZeroMemory(pBuf,nSize);
	NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
	pHead->nPackMask = PACK_SMALL;
	pHead->nIndentify = PACKINDENTIFY;

	CWebSubmitJob job;
	job.nAccount = nAccountId;
	strcpy_s(job.szPath,sizeof(job.szPath),szFile);

	WriteLogEx("SubmitPrintFile,26");

	try
	{
		do 
		{					
			//--------------------
			//֪ͨdbserver
			nWebPort = DBSVR_TCP_PORT;
			if(!sock2.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
			{
				WriteLogEx("Connect error");
				break ;
			}

			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP,sIP);
			strcpy(equip.szHost,sHost);
			WriteLogEx("ip=%s",sIP);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(sock2.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 44 err %d",::WSAGetLastError());
				break;	
			}
			pHead->nMainCmd = WM_WEB_SUBMIT_JOB;
			pHead->nSubCmd = 0;
			CopyMemory(pHead + 1,&job,sizeof(job));
			pHead->nPackBodySize = sizeof(job);
			if (sock2.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("Send 55 err %d",::WSAGetLastError());
				break;
			}

			::Sleep(2000);
			// 			while(1)
			// 			{
			// 				ServerSocket.SetTimeOut(2000);
			// 		//		int lbuffersize=ServerSocket.Receive(pBuf,nSize);
			// 				ServerSocket.KillTimeOut();
			// 				if(ServerSocket.m_bTimeOut)
			// 					break;
			// 				if(lbuffersize<0)
			// 					break;	 
			// 			}
			bSuc = TRUE;
			//			ServerSocket.Close();
		//	WriteLogEx("NotifyFromWeb suc 000 %d",nMask);
		} while (0);

	}
	catch (...)
	{
	}

	WriteLogEx("SubmitPrintFile,80");

	if (pBuf)
	{
		delete pBuf;
	}
	return TRUE;
}

BOOL NotifyPhoneChargeFromWeb(int nMask, char* cIP, char* cPrinterName, char* cPhone, char* cMoney)
{

	if(InterlockedIncrement(&g_lInstance) > 1)
	{
		WriteLogEx("�Ѿ����� NotifyPhoneChargeFromWeb = %d",g_lInstance);
		return FALSE;
	}
#if 0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 1;
	}
#endif
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WriteLogEx("NotifyPhoneChargeFromWeb %d",nMask);
	//return 1;
	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	CBaseSock	sock2;
	int nWebPort = 0;

	int nRet = 0;
	CString sLocalIP,
		sHost,sIP;
	CString sEquipID;
	EQUIP_INFO equip = {0};
	sLocalIP = "127.0.0.1";
	//	sMac = CCommonFun::GetLocalMac();
	sHost = "IIS-Host";
	sIP = "127.0.0.1";
	nWebPort = POLICY_TCP_PORT;

    int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
    BYTE* pBuf = new BYTE[nSize];
    if (!pBuf)
    {
        WriteLogEx("NotifyPhoneChargeFromWeb err 000 %d",nMask);
        InterlockedDecrement(&g_lInstance);
        return FALSE;
    }

    ZeroMemory(pBuf,nSize);
    NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
    pHead->nPackMask = PACK_SMALL;
    pHead->nIndentify = PACKINDENTIFY;

    CString szMsg;
    szMsg.Format("%s;%s;%s;%s", cIP, cPrinterName, cPhone, cMoney);
    int nSize2 = sizeof(NET_PACK_HEAD) + szMsg.GetLength()+1;
    BYTE* pBuf2 = new BYTE[nSize2];
    if (!pBuf2)
    {
        WriteLogEx("NotifyPhoneChargeFromWeb err 33 %d",nMask);
        InterlockedDecrement(&g_lInstance);
        return FALSE;
    }

    ZeroMemory(pBuf2,nSize2);
    NET_PACK_HEAD *pHead2 = (NET_PACK_HEAD*)pBuf2;
    pHead2->nPackMask = PACK_SMALL;
    pHead2->nIndentify = PACKINDENTIFY;

	try
	{
		do 
		{			
			//֪ͨpolicyserver
			if(!ServerSocket.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
			{
				WriteLogEx("NotifyPhoneChargeFromWeb Connect error");
				break ;
			}

			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP,sLocalIP);
			strcpy(equip.szHost,sHost);
			WriteLogEx("sLocalIP=%s",sLocalIP);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("NotifyPhoneChargeFromWeb Send 22 err %d",::WSAGetLastError());
				break;
			}

			pHead2->nMainCmd = WM_PRINT_PHONE_CHARGE;
			CopyMemory(pHead2 + 1,szMsg.GetBuffer(),szMsg.GetLength());
			pHead2->nPackBodySize = szMsg.GetLength() + 1;
			if (ServerSocket.Send(pBuf2,sizeof(NET_PACK_HEAD) + pHead2->nPackBodySize) < 1)
			{
				WriteLogEx("NotifyPhoneChargeFromWeb Send 33 err %d",::WSAGetLastError());
				break;
			}
			::Sleep(2000);
			bSuc = TRUE;
			WriteLogEx("NotifyPhoneChargeFromWeb suc 000 %d",nMask);
		} while (0);
	}
	catch (...)
	{
	}

	if (pBuf)
	{
		delete pBuf;
	}
	if (pBuf2)
	{
		delete pBuf2;
	}
	/*WSACleanup( );*/

	InterlockedDecrement(&g_lInstance);
	WriteLogEx("NotifyPhoneChargeFromWeb suc 222 %d",g_lInstance);
	return bSuc;
}

BOOL WaitForState(DWORD dwDesiredState, SC_HANDLE svc) 
{
	BOOL bResult = FALSE;
	// Loop until driver reaches desired state or error occurs
	while (1)
	{
		SERVICE_STATUS ssa;
		// Get current state of driver
		bResult = ::QueryServiceStatus(svc, &ssa);
		// If we can't query the driver, we're done
		if (!bResult)
		{
			WriteLogEx("!!WaitForState QueryServiceStatus fail err=%d,svc=%X",GetLastError(), svc);
			break;
		}
		// If the driver reaches the desired state
		if (ssa.dwCurrentState == dwDesiredState) 
			break;
		// We're not done, wait the specified period of time
		DWORD dwWaitHint = ssa.dwWaitHint / 10;    // Poll 1/10 of the wait hint
		if (dwWaitHint <  1000) dwWaitHint = 1000;  // At most once a second
		if (dwWaitHint > 10000) dwWaitHint = 10000; // At least every 10 seconds
		::Sleep(dwWaitHint);
	} // while

	return bResult;
}

BOOL StopSMService(CString sServiceName)
{
	WriteLogEx("StopSMService,1,sServiceName=[%s]",sServiceName);
	BOOL bRet = FALSE;
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		SC_HANDLE svc = OpenService(scm,sServiceName,SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			QueryServiceStatus(svc,&ssa);
			if (ssa.dwCurrentState == SERVICE_RUNNING)
			{
				bRet = ControlService(svc, SERVICE_CONTROL_STOP, &ssa);
				if(bRet)
				{
					bRet = WaitForState(SERVICE_STOPPED, svc);
					if (!bRet)
					{
						WriteLogEx("!!StopSMService,6,WaitForState fail,err=[%u]",GetLastError());
					}
				}
				else
				{
					WriteLogEx("!!StopSMService,5,ControlService fail,err=[%u]",GetLastError());
				}
			}
			else
			{
				WriteLogEx("!!StopSMService,4,QueryServiceStatus fail,err=[%u]",GetLastError());
			}
			CloseServiceHandle(svc);
		}                           
		else
		{
			WriteLogEx("!!StopSMService,3,OpenService fail,err=[%u]",GetLastError());
		}
		CloseServiceHandle(scm);
	}
	else
	{
		WriteLogEx("!!StopSMService,2,OpenSCManager fail,err=[%u]",GetLastError());
	}
	return bRet;
}

BOOL StartSMService(CString sServiceName)
{
	WriteLogEx("StartSMService,1,sServiceName=[%s]",sServiceName);
	BOOL bRet = FALSE;
	SC_HANDLE SchSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (SchSCManager != NULL)
	{
		SC_HANDLE  schService;
		schService = OpenService(SchSCManager, sServiceName, SERVICE_ALL_ACCESS);
		if (schService != NULL)
		{
			bRet = StartService(schService, 0, NULL);
			if (bRet)
			{
				bRet = WaitForState(SERVICE_RUNNING, schService);
				if (!bRet)
				{
					WriteLogEx("!!StartSMService,6,WaitForState fail,err=[%u]",GetLastError());
				}
			}
			else
			{
				DWORD dwErr = GetLastError();
				if (dwErr == ERROR_SERVICE_ALREADY_RUNNING)
				{
					bRet = TRUE;
					WriteLogEx("StartSMService,5,StartService fail,err=[ERROR_SERVICE_ALREADY_RUNNING]");
				}
				else
				{
					WriteLogEx("!!StartSMService,4,StartService fail,err=[%u]",dwErr);
				}
			}
			CloseServiceHandle(schService);
		}
		else
		{
			WriteLogEx("!!StartSMService,3,OpenService fail,err=[%u]",GetLastError());
		}
		CloseServiceHandle(SchSCManager);
	}
	else
	{
		WriteLogEx("!!StartSMService,2,OpenSCManager fail,err=[%u]",GetLastError());
	}

	return bRet;
}

BOOL GetServiceStateByServiceName(CString szServiceName, DWORD& dwState)
{
	WriteLogEx("GetServiceStateByServiceName,1,szServiceName=[%s]", szServiceName);
	BOOL bRet = FALSE;
	dwState = 0;
	SERVICE_STATUS ssa;
	SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (scm != NULL)
	{
		SC_HANDLE svc = OpenService(scm,szServiceName,SERVICE_ALL_ACCESS);
		if (svc != NULL)
		{
			bRet = QueryServiceStatus(svc,&ssa);
			if (bRet)
			{
				dwState = ssa.dwCurrentState;
				WriteLogEx("GetServiceStateByServiceName,5,succ, dwState=[%u]", dwState);
			}
			else
			{
				WriteLogEx("!!GetServiceStateByServiceName,4,QueryServiceStatus fail,err=[%u]",GetLastError());
			}
			CloseServiceHandle(svc);
		}                           
		else
		{
			WriteLogEx("!!GetServiceStateByServiceName,3,OpenService fail,err=[%u]",GetLastError());
		}
		CloseServiceHandle(scm);
	}
	else
	{
		WriteLogEx("!!GetServiceStateByServiceName,2,OpenSCManager fail,err=[%u]",GetLastError());
	}
	return bRet;
}

/************************************************************************/
/* 
#define SERVICE_NO_INSTALL                     0x00000001	//δ��װ
#define SERVICE_STOPPED                        0x00000001	//ֹͣ
#define SERVICE_START_PENDING                  0x00000002	//
#define SERVICE_STOP_PENDING                   0x00000003	//
#define SERVICE_RUNNING                        0x00000004	//����
#define SERVICE_CONTINUE_PENDING               0x00000005	//
#define SERVICE_PAUSE_PENDING                  0x00000006	//
#define SERVICE_PAUSED                         0x00000007	//��ͣ

#define iSecASvcHost_Name	TEXT("iSecASvcHost")		//��ҵ�����ķ�����
#define iSecBSvcHost_Name	TEXT("iSecBSvcHost")		//У԰�����ķ�����
#define PrintDbSvcHost_Name	TEXT("PrintDbSvcHost")		//��־������
#define iSecHttpHost_Name	TEXT("iSecHttpHost")		//����������
#define ISecDbHelper_Name	TEXT("ISecDbHelper")		//���ݿⱸ�ݷ�����

#define iSecASvcHost_Index	1			//��ҵ�����ķ�����
#define iSecBSvcHost_Index	2			//У԰�����ķ�����
#define PrintDbSvcHost_Index	3		//��־������
#define iSecHttpHost_Index	4			//����������
#define ISecDbHelper_Index	5			//���ݿⱸ�ݷ�����

                                                                     */
/************************************************************************/

CString GetServiceNameByServiceType(int nServiceType)
{
	CString szServiceName;
	WriteLogEx("GetServiceNameByServiceType,1,nServiceType=[%d]",nServiceType);
	switch (nServiceType)
	{
	case iSecASvcHost_Index:
		szServiceName = iSecASvcHost_Name;
		break;
	case iSecBSvcHost_Index:
		szServiceName = iSecBSvcHost_Name;
		break;
	case PrintDbSvcHost_Index:
		szServiceName = PrintDbSvcHost_Name;
		break;
	case iSecHttpHost_Index:
		szServiceName = iSecHttpHost_Name;
		break;
	case ISecDbHelper_Index:
		szServiceName = ISecDbHelper_Name;
		break;
	default:
		WriteLogEx("!!GetServiceNameByServiceType,2,UnKnow Type,nServiceType=[%d]",nServiceType);
		break;
	}
	return szServiceName;
}

int GetServiceState(int nServiceType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLogEx("GetServiceState,1,nServiceType=[%d]",nServiceType);
	DWORD dwState = 0;
	CString szServiceName = GetServiceNameByServiceType(nServiceType);
	if (szServiceName.GetLength()>0)
	{
		if (GetServiceStateByServiceName(szServiceName, dwState))
		{
			WriteLogEx("GetServiceState,3,GetServiceStateByServiceName succ,szServiceName=[%s],dwState=[%d]"
				, szServiceName, dwState);
		}
		else
		{
			WriteLogEx("!!GetServiceState,2,GetServiceStateByServiceName fail");
		}
	}
	else
	{
		WriteLogEx("!!GetServiceState,2,szServiceName=[%s]",szServiceName);
	}
	return dwState;
}

BOOL StartServiceByType(int nServiceType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLogEx("StartServiceByType,1,nServiceType=[%d]",nServiceType);
	BOOL bRet = FALSE;
	CString szServiceName = GetServiceNameByServiceType(nServiceType);
	if (szServiceName.GetLength()>0)
	{
		bRet = StartSMService(szServiceName);
		if (!bRet)
		{
			WriteLogEx("!!StartServiceByType,3,SartSMService fail,szServiceName=[%s]",szServiceName);
		}
	}
	else
	{
		WriteLogEx("!!StartServiceByType,2,szServiceName=[%s]",szServiceName);
	}

	return bRet;
}

BOOL StopServiceByType(int nServiceType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLogEx("StopServiceByType,1,nServiceType=[%d]",nServiceType);
	BOOL bRet = FALSE;
	CString szServiceName = GetServiceNameByServiceType(nServiceType);
	if (szServiceName.GetLength()>0)
	{
		bRet = StopSMService(szServiceName);
		if (!bRet)
		{
			WriteLogEx("!!StopServiceByType,3,SartSMService fail,szServiceName=[%s]",szServiceName);
		}
	}
	else
	{
        WriteLogEx("!!StopServiceByType,2,szServiceName=[%s]",szServiceName);
    }

    return bRet;
}

BOOL MobileNotifyClt(int nCmd, char* cIP, char* cAccount, char* cPwd, char* cData)
{
#ifdef INTERLOCK_G_LINSTANCE
    if(InterlockedIncrement(&g_lInstance) > 1)
    {
        WriteLogEx("MobileNotifyClt,�Ѿ����� NotifyFromWeb = %d",g_lInstance);
        return FALSE;
    }
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bSuc = FALSE;
    CBaseSock    ServerSocket;
    CBaseSock	sock2;
    int nWebPort = 0;

    int nRet = 0;
    CString sLocalIP,
        sHost,sIP;
    CString sEquipID;
    EQUIP_INFO equip = {0};
    sLocalIP = "127.0.0.1";
    sHost = "IIS-Host";
    sIP = "127.0.0.1";
    nWebPort = POLICY_TCP_PORT;

    int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
    BYTE* pBuf = new BYTE[nSize];
    if (!pBuf)
    {
        WriteLogEx("MobileNotifyClt err 000 %d",nCmd);
        InterlockedDecrement(&g_lInstance);
        return FALSE;
    }

    ZeroMemory(pBuf,nSize);
    NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
    pHead->nPackMask = PACK_SMALL;
    pHead->nIndentify = PACKINDENTIFY;

    CString msg;
    msg.Format("%d;%s;%s;%s;%s", nCmd, cIP, cAccount, cPwd, cData);
    int nSize2 = sizeof(NET_PACK_HEAD) + msg.GetLength() + 1;
    BYTE* pBuf2 = new BYTE[nSize2];
    if (!pBuf2)
    {
        WriteLogEx("MobileNotifyClt err 33 %d",nCmd);
        InterlockedDecrement(&g_lInstance);
        return FALSE;
    }

    ZeroMemory(pBuf2,nSize2);
    NET_PACK_HEAD *pHead2 = (NET_PACK_HEAD*)pBuf2;
    pHead2->nPackMask = PACK_SMALL;
    pHead2->nIndentify = PACKINDENTIFY;

    try
    {
        do 
        {
            //֪ͨpolicyserver
            if(!ServerSocket.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
            {
                WriteLogEx("MobileNotifyClt Connect error");
                break;
            }
            equip.nEquipType = ISEC_PLT_WEB;
            strcpy(equip.szIP,sLocalIP);
            strcpy(equip.szHost,sHost);
            //WriteLogEx("sLocalIP=%s",sLocalIP);
            pHead->nMainCmd = WM_CHECK_EQUIP;
            CopyMemory(pHead + 1,&equip,sizeof(equip));
            pHead->nPackBodySize = sizeof(equip);
            if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
            {
                WriteLogEx("NotifyCltPrintAll Send 22 err %d",::WSAGetLastError());
                break;
            }
            pHead2->nMainCmd = WM_MOBILE_NOTIFY_CLT;
            CopyMemory(pHead2 + 1, msg.GetBuffer(), msg.GetLength());
            pHead2->nPackBodySize = msg.GetLength() + 1;
            if (ServerSocket.Send(pBuf2,sizeof(NET_PACK_HEAD) + pHead2->nPackBodySize) < 1)
            {
                WriteLogEx("MobileNotifyClt Send 33 err %d",::WSAGetLastError());
                break;
            }
            ::Sleep(2000);
            bSuc = TRUE;
            WriteLogEx("MobileNotifyClt suc 000 nCmd=%d, msg=%s", nCmd, msg);
        } 
        while (0);
    }
    catch(...)
    {
    }
    if (pBuf)
    {
        delete pBuf;
    }
    if (pBuf2)
    {
        delete pBuf2;
    }
    WriteLogEx("MobileNotifyClt InterlockedDecrement g_lInstance=%d", g_lInstance);
    InterlockedDecrement(&g_lInstance);
    return TRUE;
#else
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bSuc = FALSE;
    CBaseSock    ServerSocket;
    CBaseSock	sock2;
    int nWebPort = 0;

    int nRet = 0;
    CString sLocalIP,
    sHost,sIP;
    CString sEquipID;
    EQUIP_INFO equip = {0};
    sLocalIP = "127.0.0.1";
    sHost = "IIS-Host";
    sIP = "127.0.0.1";
    nWebPort = POLICY_TCP_PORT;

    int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
    BYTE* pBuf = new BYTE[nSize];
    if (!pBuf)
    {
        WriteLogEx("MobileNotifyClt err 000 %d",nCmd);
        return FALSE;
    }

    ZeroMemory(pBuf,nSize);
    NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
    pHead->nPackMask = PACK_SMALL;
    pHead->nIndentify = PACKINDENTIFY;

    CString msg;
    msg.Format("%d;%s;%s;%s;%s", nCmd, cIP, cAccount, cPwd, cData);
    int nSize2 = sizeof(NET_PACK_HEAD) + msg.GetLength() + 1;
    BYTE* pBuf2 = new BYTE[nSize2];
    if (!pBuf2)
    {
        WriteLogEx("MobileNotifyClt err 33 %d",nCmd);
        return FALSE;
    }

    ZeroMemory(pBuf2,nSize2);
    NET_PACK_HEAD *pHead2 = (NET_PACK_HEAD*)pBuf2;
    pHead2->nPackMask = PACK_SMALL;
    pHead2->nIndentify = PACKINDENTIFY;

    try
    {
        do 
        {
            //WriteLogEx("----------------------MobileNotifyClt 111111111111 cAccount=%s,ProcessId=%d,ThreadId=%d", cAccount, GetCurrentProcessId(), GetCurrentThreadId());
            //Sleep(30000);
            //WriteLogEx("----------------------MobileNotifyClt 222222222222 cAccount=%s,ProcessId=%d,ThreadId=%d", cAccount, GetCurrentProcessId(), GetCurrentThreadId());
            //֪ͨpolicyserver
            if(!ServerSocket.Connect(sIP,nWebPort))//����ֻ�Ǵ�����80�˿��Ժ����޸��ж˿ں�Ҫ�����ݱ���pBufferSend���еó�
            {
                WriteLogEx("MobileNotifyClt Connect error");
                break;
            }
            equip.nEquipType = ISEC_PLT_WEB;
            strcpy(equip.szIP,sLocalIP);
            strcpy(equip.szHost,sHost);
            //WriteLogEx("sLocalIP=%s",sLocalIP);
            pHead->nMainCmd = WM_CHECK_EQUIP;
            CopyMemory(pHead + 1,&equip,sizeof(equip));
            pHead->nPackBodySize = sizeof(equip);
            if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
            {
                WriteLogEx("NotifyCltPrintAll Send 22 err %d",::WSAGetLastError());
                break;
            }
//             pHead2->nMainCmd = WM_MOBILE_NOTIFY_CLT;
            CopyMemory(pHead2 + 1, msg.GetBuffer(), msg.GetLength());
            pHead2->nPackBodySize = msg.GetLength() + 1;
#ifdef DEF_WebNotifyWaitAck
            pHead2->nSubCmd = DEF_WebNotifyWaitAckFlag;
            WriteLogEx("MobileNotifyClt DEF_WebNotifyWaitAck  nSubCmd=%x", pHead2->nSubCmd);
#endif
            if (ServerSocket.Send(pBuf2,sizeof(NET_PACK_HEAD) + pHead2->nPackBodySize) < 1)
            {
                WriteLogEx("MobileNotifyClt Send 33 err %d",::WSAGetLastError());
                break;
            }
#ifndef DEF_WebNotifyWaitAck
            ::Sleep(2000);
#else
            WriteLogEx("MobileNotifyClt wait ack begin");
            if( ServerSocket.RecvUtilCmd(pHead2->nMainCmd, 3000))
            {
                WriteLogEx("MobileNotifyClt wait ack succ");
            }
            WriteLogEx("MobileNotifyClt wait ack end");

#endif
            bSuc = TRUE;
            WriteLogEx("MobileNotifyClt suc 000 nCmd=%d, msg=%s", nCmd, msg);
        } 
        while (0);
    }
    catch(...)
    {
    }
    if (pBuf)
    {
        delete pBuf;
    }
    if (pBuf2)
    {
        delete pBuf2;
    }
    return TRUE;
#endif
};

BOOL WebNotifyServerForLogin(int nPrtId, char* pcAccount, char* pcPwd, char* pcData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	int nServerPort = POLICY_TCP_PORT;	//���ķ������˿�
	CString sLocalIP = "127.0.0.1";
#if 0
	CString	sHost = "IIS-Host";
#else
	CString	sHost;
	sHost.Format("IIS-Host-%d-%s", nPrtId, pcAccount);
#endif
	CString sIP = "127.0.0.1";
	BYTE* pBuf = NULL;
	BYTE* pBuf2 = NULL;

	try
	{
		do 
		{
			//�������ķ�����
			if(!ServerSocket.Connect(sIP,nServerPort))
			{
				WriteLogEx("!!WebNotifyServerForLogin 111 Connect error");
				break;
			}

			//���͵�һ����:�豸��֤
			EQUIP_INFO equip = {0};
			int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
			pBuf = new BYTE[nSize];
			if (!pBuf)
			{
				WriteLogEx("!!WebNotifyServerForLogin err 222");
				break;
			}
			ZeroMemory(pBuf,nSize);
			NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
			pHead->nPackMask = PACK_SMALL;
			pHead->nIndentify = PACKINDENTIFY;
			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP, sLocalIP);
			strcpy(equip.szHost, sHost);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("!!WebNotifyServerForLogin Send 333 err %d",::WSAGetLastError());
				break;
			}

			//���͵ڶ�����:���ݰ�
			CString szMsg;
			szMsg.Format("%d;%s;%s;%s", nPrtId, pcAccount, pcPwd, pcData);
			int nSize2 = sizeof(NET_PACK_HEAD) + szMsg.GetLength() + 1;
			pBuf2 = new BYTE[nSize2];
			if (!pBuf2)
			{
				WriteLogEx("!!WebNotifyServerForLogin err 444");
				break;
			}

			ZeroMemory(pBuf2,nSize2);
			NET_PACK_HEAD *pHead2 = (NET_PACK_HEAD*)pBuf2;
			pHead2->nPackMask = PACK_SMALL;
			pHead2->nIndentify = PACKINDENTIFY;
			pHead2->nMainCmd = WM_WEB_NOTIFY_SERVER_REQ;
			pHead2->nSubCmd = WM_PRINT_PHONE_LOGIN;
			CopyMemory(pHead2 + 1, szMsg.GetBuffer(), szMsg.GetLength());
			pHead2->nPackBodySize = szMsg.GetLength() + 1;
			if (ServerSocket.Send(pBuf2,sizeof(NET_PACK_HEAD) + pHead2->nPackBodySize) < 1)
			{
				WriteLogEx("!!WebNotifyServerForLogin Send 555 err %d",::WSAGetLastError());
				break;
			}

			WriteLogEx("WebNotifyServerForLogin 666 wait ack begin");
			if(ServerSocket.RecvUtilCmd(WM_WEB_NOTIFY_SERVER_ACK, 3000))
			{
				WriteLogEx("WebNotifyServerForLogin 777 wait ack succ");
			}
			WriteLogEx("WebNotifyServerForLogin 888 wait ack end");

			bSuc = TRUE;
			WriteLogEx("WebNotifyServerForLogin suc 999 szMsg=%s", szMsg);
		} 
		while (0);
	}
	catch(...)
	{
		WriteLogEx("!!WebNotifyServerForLogin fail,catch execption,err=%u", GetLastError());
	}
	if (pBuf)
	{
		delete pBuf;
	}
	if (pBuf2)
	{
		delete pBuf2;
	}
	return bSuc;
}

BOOL WebNotifyServerForCharge(char* pcOrderNo, char* pcAccount, char* pcData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	int nServerPort = POLICY_TCP_PORT;	//���ķ������˿�
	CString sLocalIP = "127.0.0.1";
#if 0
	CString	sHost = "IIS-Host";
#else
	CString	sHost;
	sHost.Format("IIS-Host-%s", pcAccount);
#endif
	CString sIP = "127.0.0.1";
	BYTE* pBuf = NULL;
	BYTE* pBuf2 = NULL;

	try
	{
		do 
		{
			//�������ķ�����
			if(!ServerSocket.Connect(sIP,nServerPort))
			{
				WriteLogEx("!!WebNotifyServerForCharge 111 Connect error");
				break;
			}

			//���͵�һ����:�豸��֤
			EQUIP_INFO equip = {0};
			int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
			pBuf = new BYTE[nSize];
			if (!pBuf)
			{
				WriteLogEx("!!WebNotifyServerForCharge err 222");
				break;
			}
			ZeroMemory(pBuf,nSize);
			NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
			pHead->nPackMask = PACK_SMALL;
			pHead->nIndentify = PACKINDENTIFY;
			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP, sLocalIP);
			strcpy(equip.szHost, sHost);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("!!WebNotifyServerForCharge Send 333 err %d",::WSAGetLastError());
				break;
			}

			//���͵ڶ�����:���ݰ�
			CString szMsg;
			szMsg.Format("%s;%s;%s", pcOrderNo, pcAccount, pcData);
			int nSize2 = sizeof(NET_PACK_HEAD) + szMsg.GetLength() + 1;
			pBuf2 = new BYTE[nSize2];
			if (!pBuf2)
			{
				WriteLogEx("!!WebNotifyServerForCharge err 444");
				break;
			}

			ZeroMemory(pBuf2,nSize2);
			NET_PACK_HEAD *pHead2 = (NET_PACK_HEAD*)pBuf2;
			pHead2->nPackMask = PACK_SMALL;
			pHead2->nIndentify = PACKINDENTIFY;
			pHead2->nMainCmd = WM_WEB_NOTIFY_SERVER_REQ;
			pHead2->nSubCmd = WM_PRINT_PHONE_CHARGE;
			CopyMemory(pHead2 + 1, szMsg.GetBuffer(), szMsg.GetLength());
			pHead2->nPackBodySize = szMsg.GetLength() + 1;
			if (ServerSocket.Send(pBuf2,sizeof(NET_PACK_HEAD) + pHead2->nPackBodySize) < 1)
			{
				WriteLogEx("!!WebNotifyServerForCharge Send 555 err %d",::WSAGetLastError());
				break;
			}

			WriteLogEx("WebNotifyServerForCharge 666 wait ack begin");
			if(ServerSocket.RecvUtilCmd(WM_WEB_NOTIFY_SERVER_ACK, 3000))
			{
				WriteLogEx("WebNotifyServerForCharge 777 wait ack succ");
			}
			WriteLogEx("WebNotifyServerForCharge 888 wait ack end");

			bSuc = TRUE;
			WriteLogEx("WebNotifyServerForCharge suc 999 szMsg=%s", szMsg);
		} 
		while (0);
	}
	catch(...)
	{
		WriteLogEx("!!WebNotifyServerForCharge fail,catch execption,err=%u", GetLastError());
	}
	if (pBuf)
	{
		delete pBuf;
	}
	if (pBuf2)
	{
		delete pBuf2;
	}
	return bSuc;
}

BOOL WebNotifyServer(int nCmd, char* pcData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bSuc = FALSE;
	CBaseSock    ServerSocket;
	int nServerPort = POLICY_TCP_PORT;	//���ķ������˿�
	CString sLocalIP = "127.0.0.1";
	CString	sHost = "IIS-Host";
	CString sIP = "127.0.0.1";
	BYTE* pBuf = NULL;
	BYTE* pBuf2 = NULL;

	try
	{
		do 
		{
			//�������ķ�����
			if(!ServerSocket.Connect(sIP,nServerPort))
			{
				WriteLogEx("!!WebNotifyServer 111 Connect error");
				break;
			}

			//���͵�һ����:�豸��֤
			EQUIP_INFO equip = {0};
			int nSize = sizeof(NET_PACK_HEAD) + sizeof(equip);
			pBuf = new BYTE[nSize];
			if (!pBuf)
			{
				WriteLogEx("!!WebNotifyServer err 222");
				break;
			}
			ZeroMemory(pBuf,nSize);
			NET_PACK_HEAD *pHead = (NET_PACK_HEAD*)pBuf;
			pHead->nPackMask = PACK_SMALL;
			pHead->nIndentify = PACKINDENTIFY;
			equip.nEquipType = ISEC_PLT_WEB;
			strcpy(equip.szIP, sLocalIP);
			strcpy(equip.szHost, sHost);
			pHead->nMainCmd = WM_CHECK_EQUIP;
			CopyMemory(pHead + 1,&equip,sizeof(equip));
			pHead->nPackBodySize = sizeof(equip);
			if(ServerSocket.Send(pBuf,sizeof(NET_PACK_HEAD) + pHead->nPackBodySize) < 1)
			{
				WriteLogEx("!!WebNotifyServer Send 333 err %d",::WSAGetLastError());
				break;
			}

			//���͵ڶ�����:���ݰ�
			CString szMsg;
			szMsg.Format("%s", pcData);
			int nSize2 = sizeof(NET_PACK_HEAD) + szMsg.GetLength() + 1;
			pBuf2 = new BYTE[nSize2];
			if (!pBuf2)
			{
				WriteLogEx("!!WebNotifyServer err 444");
				break;
			}

			ZeroMemory(pBuf2,nSize2);
			NET_PACK_HEAD *pHead2 = (NET_PACK_HEAD*)pBuf2;
			pHead2->nPackMask = PACK_SMALL;
			pHead2->nIndentify = PACKINDENTIFY;
			pHead2->nMainCmd = WM_WEB_NOTIFY_SERVER_REQ;
			pHead2->nSubCmd = nCmd;
			CopyMemory(pHead2 + 1, szMsg.GetBuffer(), szMsg.GetLength());
			pHead2->nPackBodySize = szMsg.GetLength() + 1;
			if (ServerSocket.Send(pBuf2,sizeof(NET_PACK_HEAD) + pHead2->nPackBodySize) < 1)
			{
				WriteLogEx("!!WebNotifyServer Send 555 err %d",::WSAGetLastError());
				break;
			}

			WriteLogEx("WebNotifyServer 666 wait ack begin");
			if(ServerSocket.RecvUtilCmd(WM_WEB_NOTIFY_SERVER_ACK, 3000))
			{
				WriteLogEx("WebNotifyServer 777 wait ack succ");
			}
			WriteLogEx("WebNotifyServer 888 wait ack end");

			bSuc = TRUE;
			WriteLogEx("WebNotifyServer suc 999 szMsg=%s", szMsg);
		} 
		while (0);
	}
	catch(...)
	{
		WriteLogEx("!!WebNotifyServer fail,catch execption,err=%u", GetLastError());
	}
	if (pBuf)
	{
		delete pBuf;
	}
	if (pBuf2)
	{
		delete pBuf2;
	}
	return bSuc;
}

/************************************************************************/
/*
���������
	pImageFilePath			//ͼƬ·�����ַ����͡�
	nPageNum				//�����ͼƬҳ�룬��0��ʼ��һҳ��
	pOutData				//�������������������ݣ��Զ��ŷָ�����ͼƬ�ļ��Ƿ���ڡ�����ͼƬ��ҳ���������롿������ǰҳ�ļ�base64���ݡ�������Ϣ����
	nOutDataLen				//ָʾ��������pOutData�������Ա�����ַ������ȡ�
	bCheckOutDataLen		//��ʶ��⵱ǰҳ�ļ�base64���ݵĳ��ȣ�0-��ʾ��ȡ��ǰҳ�ļ�base64���ݣ�1-��ʾ��⵱ǰҳ�ļ�base64���ݳ��ȡ�
	nImageDPI				//���û�ȡͼƬ�ļ���DPI�����鴫��96DPI��150DPI,
							//����ﵽ600DPI,��ͼƬ�ķֱ���Ϊ4767*6822���ļ���������������ռ�ô����ڴ档

���������
	BOOL					//0-��ʾ��������ʧ�ܣ�1-��ʾ�������óɹ���

�ر�˵����
	1.��������1ʱ����bCheckOutDataLenΪ��1��ʱ����pOutData=��ͼƬ�ļ��Ƿ���ڡ���ͼƬ��ҳ������nOutDataLen���ȡ���
	2.��������1ʱ����bCheckOutDataLenΪ��0��ʱ����pOutData=����ǰҳ�ļ�base64����(PNG��ʽ)����
	3.�������ء�0��ʱ����pOutData=��ͼƬ�ļ��Ƿ���ڡ��������롿��������Ϣ����
	4.��bCheckOutDataLenΪ��1��ʱ��pOutData�ĳ���Ӧ�ñ����㹻�ĳ��ȡ�128�����������������Ϣ��
	5.֧��emf,png,jpg,bmp,gifͼƬ��ʽ����emf��ʽ��spl�ļ���
	6.֧���Զ���ѹgz��ʽ���ļ���
	
	*/
/************************************************************************/

BOOL GetImageData(char* pImageFilePath, int nPageNum, char* pOutData, int nOutDataLen, BOOL bCheckOutDataLen, int nImageDPI)
{
	BOOL bRet = FALSE;

	Image* pImage = NULL;
	GUID* pDimensionIDs = NULL;
	BYTE *pData = NULL;
	size_t nSize = 0;
	Status status = Ok;
	CString szOutData = "";

#if 1 //�ŵ��ӿ���
	gdiplusToken = 0;
	//��ʼ��gdi++
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

	do 
	{
		if (!pImageFilePath)
		{
			szOutData.Format("%d,%d,%s", FALSE, 0, "Image File Path Error");
			break;
		}

		if (!pOutData)
		{
			szOutData.Format("%d,%d,%s", FALSE, 0, "Out Data Error");
			break;
		}

		if (nOutDataLen < 128)
		{
			szOutData.Format("%d,%d,%s", FALSE, nOutDataLen, "Out Data Too Samll");
			break;
		}

		if (!PathFileExists(pImageFilePath))
		{
			szOutData.Format("%d,%d,%s", FALSE, 0, "File Not Exists");
			break;
		}

		//�ж��ļ�����
		CString szFileName;
		CString szFileExt;
		if (!GetFileExt3(pImageFilePath, szFileName, szFileExt))
		{
			szOutData.Format("%d,%s,%s", FALSE, pImageFilePath, "Not Support File Format,1");
			break;
		}

		//�ж��Ƿ�Ϊѹ���ļ�,�����ѹ���ļ��������Ҫ��ѹ���ļ�
		if (szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_GZ) == 0)
		{
			//��ѹ���ļ������Ҳ������Ѿ���ѹ���ļ������ļ��ڱ�Ŀ¼��ѹ
			if (!PathFileExists(szFileName))
			{
				if (!UnCompressGzFile(pImageFilePath, szFileName, FALSE))
				{
					szOutData.Format("%d,%s,%s", FALSE, pImageFilePath, "UnCompress GZ File Fail");
					break;
				}
			}

			//��ȡ��ѹ���ļ���׺
			szFileExt = GetFileExt2(szFileName);	//��ѹ���ļ���szFileName���Ƿ�ѹ���ļ���ȫ·��
		}
		else
		{
			szFileName += szFileExt;	//����ѹ���ļ�����׺Ҫƴ�ӻ�ȥ
		}
		
		WriteLogEx("szFileName=%s,szFileExt=%s", szFileName, szFileExt);

		UINT frameCount = 0;

		//�ж��ļ��Ƿ�֧��ͼƬ����
		if (szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_SPL) == 0)
		{
			CString szExtTemp = szFileName.Right(strlen(Cloud_Doc_Format_Suffix_EMF_SPL));
			if (szExtTemp.CompareNoCase(Cloud_Doc_Format_Suffix_EMF_SPL) != 0)
			{
				szOutData.Format("%d,%s,%s", FALSE, pImageFilePath, "Not Support File Format,2");
				break;
			}

			CParseSpl spl;
			if (!spl.InitParse(szFileName, TRUE))
			{
				szOutData.Format("%d,%s,%s", TRUE, szFileName, "Parse EMF File Error");
				break;
			}

			frameCount = spl.GetPageCountPerCopy();
			if (frameCount <= 0)
			{
				szOutData.Format("%d,%d,%s", TRUE, frameCount, "EMF GetPageCountPerCopy Error");
				break;
			}

			if ((nPageNum<0) || (nPageNum>=frameCount))
			{
				szOutData.Format("%d,%d,%s", TRUE, frameCount, "EMF Out of Page Number Range");
				break;
			}

			HENHMETAFILE hEMF = NULL;
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
			PEMF_DATA emf = spl.GetEmfDataByPageIndex(nPageNum);
			if (emf && emf->pData)
			{
				hEMF = SetEnhMetaFileBits(emf->dwSize, emf->pData);
			}
			else
			{
				szOutData.Format("%d,%d,%s", TRUE, nPageNum, "EMF Get Page Data Error");
				break;
			}
#else
			hEMF = spl.GetEmfDataByPageIndex(nPageNum);
#endif

			if (hEMF == NULL)
			{
				szOutData.Format("%d,%u,%s", TRUE, GetLastError(), "SetEnhMetaFileBits Error");
				break;
			}

			Metafile mf(hEMF, TRUE);

			pImage = mf.Clone();
		}
		else if (szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_TIF) == 0
			|| szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_JPG) == 0
			|| szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_PNG) == 0
			|| szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_TIFF) == 0
			|| szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_JPEG) == 0
			|| szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_BMP) == 0
			|| szFileExt.CompareNoCase(Cloud_Doc_Format_Suffix_GIF) == 0
			)
		{
			CStringW wszImageFilePath(szFileName);
			Image imageLocal(wszImageFilePath);

			UINT count = 0;
			count = imageLocal.GetFrameDimensionsCount();
			if (count <= 0)
			{
				szOutData.Format("%d,%d,%s", TRUE, count, "Image GetFrameDimensionsCount Error");
				break;
			}

			pDimensionIDs = (GUID*)malloc(sizeof(GUID)*count);
			status = imageLocal.GetFrameDimensionsList(pDimensionIDs, count);
			if (status != Ok)
			{
				szOutData.Format("%d,%d,%s", TRUE, status, "Image FrameDimensionsList Error");
				break;
			}

			WCHAR strGuid[39];
			StringFromGUID2(pDimensionIDs[0], strGuid, 39);
			frameCount = imageLocal.GetFrameCount(&pDimensionIDs[0]);
			if (frameCount <= 0)
			{
				szOutData.Format("%d,%d,%s", TRUE, frameCount, "Image GetFrameCount Error");
				break;
			}

			if ((nPageNum<0) || (nPageNum>=frameCount))
			{
				szOutData.Format("%d,%d,%s", TRUE, frameCount, "Out of Page Number Range");
				break;
			}

			status = imageLocal.SelectActiveFrame(&FrameDimensionPage, nPageNum);
			if (status != Ok)
			{
				szOutData.Format("%d,%d,%s", TRUE, status, "Image SelectActiveFrame Error");
				break;
			}

			pImage = imageLocal.Clone(); 
		}
		else
		{
			szOutData.Format("%d,%s,%s", FALSE, pImageFilePath, "Not Support File Format,3");
			break;
		}

		if (!pImage)
		{
			szOutData.Format("%d,%u,%s", TRUE, GetLastError(), "Image Clone Error");
			break;
		}

		mi_to_memory(pImage, (void**)&pData, &nSize, true, nImageDPI);
		if (!pData)
		{
			szOutData.Format("%d,%u,%s", TRUE, GetLastError(), "Image To Memory Error");
			break;
		}

		CBase64 base64;
		int nBase64EncodeSize = base64.Base64EncodeSize(nSize);
		szOutData.Format("%d,%u,", TRUE, frameCount);
		WriteLogEx("bCheckOutDataLen=%d,nBase64EncodeSize=%d,nSize=%d", bCheckOutDataLen, nBase64EncodeSize, nSize);
		if (bCheckOutDataLen)
		{
			szOutData.Format("%d,%u,%d", TRUE, frameCount, nBase64EncodeSize + 1);	//Ҫ���һλ��������'\0'
			strcpy(pOutData, szOutData);
		}
		else
		{
			if (nBase64EncodeSize > nOutDataLen)
			{
				szOutData.Format("%d,%u,%s", TRUE, nBase64EncodeSize, "Out Data Buffer Too Small");
				break;
			}
			else
			{
				int nDestLen = base64.EncodeBase64(pData, pOutData, nSize);
				if (nDestLen != nBase64EncodeSize)
				{
					szOutData.Format("%d,%u!=%u,%s", TRUE, nDestLen, nBase64EncodeSize, "Encode Image Data Error");
					break;
				}
			}
		}
		bRet = TRUE;
	} while (FALSE);

	if (pData)
	{
		free(pData);
	}
	if (pImage)
	{
		delete pImage;
	}
	if (pDimensionIDs)
	{
		free(pDimensionIDs);
	}
	if (!bRet)
	{
		strcpy(pOutData, szOutData);
	}


#if 1 //�ŵ��ӿ���
	if(gdiplusToken)
	{
		GdiplusShutdown(gdiplusToken);
		gdiplusToken = 0;
	}
#endif

	return bRet;
}


BOOL UnCompressGzFile(IN CString szFileGzPath, OUT CString& szFileOrgPath, BOOL bDelGzFile)
{
	BOOL bRet = FALSE;
	do 
	{
		CString szSuffix = szFileGzPath.Right(3);
		if (szSuffix.CompareNoCase(".gz") != 0)
		{
			WriteLogEx("!!UnCompressGzFile,1,szSuffix UnKnow=[%s],szFileGzPath=[%s]", szSuffix, szFileGzPath);
			break;
		}

		if (!PathFileExists(szFileGzPath))
		{
			WriteLogEx("!!UnCompressGzFile,2,PathFileExists fail,szFileGzPath=[%s]", szFileGzPath);
			break;
		}

		szFileOrgPath = szFileGzPath;
		szFileOrgPath.MakeLower();
		if (szFileOrgPath.Find(".gz")>0)
		{
			szFileOrgPath = szFileOrgPath.Left(szFileOrgPath.GetLength()-3);
		}

		CFile fileOrg;
		if (!fileOrg.Open(szFileOrgPath, CFile::modeReadWrite|CFile::modeCreate))
		{
			WriteLogEx("!!UnCompressGzFile,3,fileOrg.Open fail,err=%u,szFileOrgPath=[%s]", GetLastError(), szFileOrgPath);
			break;
		}

		gzFile GzFile = gzopen(szFileGzPath, "rb");
		if(GzFile == NULL)
		{
			WriteLogEx("!!UnCompressGzFile,4,gzopen fail,GzFile=%p szFileGzPath=[%s]", GzFile, szFileGzPath);
			break;
		}

		int nRead = 0;
		int nBufSize = 1024 * 1024 * 10;
		BYTE *pBuf = new BYTE[nBufSize];
		BOOL bSucc = TRUE;
		while(!gzeof(GzFile))
		{
			memset(pBuf, 0x0, nBufSize);
			nRead = gzread(GzFile, pBuf, nBufSize);
			if (nRead>0)
			{
				fileOrg.Write(pBuf, nRead);
			}
			else if (nRead<0)
			{
				WriteLogEx("!!CompressGzFile,5,gzread fail,nRead=%d", nRead);
				bSucc = FALSE;
				break;
			}
			else
			{
				break;
			}
		}
		fileOrg.Close();
		gzclose(GzFile);
		delete[] pBuf;

		if (bDelGzFile)
		{
			if (!DeleteFile(szFileGzPath))
			{
				WriteLogEx("!!UnCompressGzFile,6,DeleteFile fail,szFileGzPath=[%s]", szFileGzPath);
			}
		}
		bRet = bSucc;
	} while (FALSE);

	return bRet;
}

BOOL CompressGzFile(IN CString szFileOrgPath, OUT CString& szFileGzPath, BOOL bDelOrgzFile)
{
	BOOL bRet = FALSE;
	do 
	{
		if (!PathFileExists(szFileOrgPath))
		{
			WriteLogEx("!!CompressGzFile,1,PathFileExists fail,szFileOrg=[%s]", szFileOrgPath);
			break;
		}

		szFileGzPath = szFileOrgPath + ".gz";

		CFile fileOrg;
		if (!fileOrg.Open(szFileOrgPath, CFile::modeRead))
		{
			WriteLogEx("!!CompressGzFile,2,szFileGzPath.Open fail,err=%u,szFileOrgPath=[%s]", GetLastError(), szFileOrgPath);
			break;
		}

		gzFile GzFile = gzopen(szFileGzPath, "wb");
		if(GzFile == NULL)
		{
			WriteLogEx("!!CompressGzFile,3,gzopen fail,GzFile=%p szFileGzPath=[%s]", GzFile, szFileGzPath);
			break;
		}

		int nRead = 0;
		int nWrite = 0;
		int nBufSize = 1024 * 1024 * 10;
		BYTE *pBuf = new BYTE[nBufSize];
		BOOL bSucc = TRUE;
		do 
		{
			memset(pBuf, 0x0, nBufSize);
			nRead = fileOrg.Read(pBuf, nBufSize);
			if (nRead>0)
			{
				nWrite = gzwrite(GzFile, pBuf, nRead);
				if(nWrite != nRead)
				{
					WriteLogEx("!!CompressGzFile,4,gzwrite fail,nWrite=%d,nRead=%d", nWrite, nRead);
					bSucc = FALSE;
					break;
				}
			}
			else
			{
				break;
			}		
		} while (nRead>0);
		fileOrg.Close();
		gzclose(GzFile);
		delete[] pBuf;

		if (bDelOrgzFile)
		{
			if (!DeleteFile(szFileOrgPath))
			{
				WriteLogEx("!!CompressGzFile,5,DeleteFile fail,szFileGzPath=[%s]", szFileGzPath);
			}
		}
		bRet = bSucc;
	} while (FALSE);

	return bRet;
}

BOOL FileToBase64(IN CString szFilePath, OUT CString& szBase64)
{
	BOOL bRet = FALSE;
	char* pBase64 = NULL;
	unsigned char* pFile = NULL;
	do 
	{
		CFile file;
		if (file.Open(szFilePath, CFile::modeRead))
		{
			int nFileLen = (int)file.GetLength();	//���֧��2G�ļ�
			if (nFileLen <= 0)
			{
				WriteLogEx("!!FileToBase64, nFileLen=%d, szFilePath=%s", nFileLen, szFilePath);
				break;
			}
			pFile = new unsigned char[nFileLen+1];
			memset(pFile, 0x0, nFileLen+1);
			int nRead = file.Read(pFile, nFileLen);
			if (nRead != nFileLen)
			{
				WriteLogEx("!!FileToBase64, nFileLen=%d, nRead=%d", nFileLen, nRead);
				break;
			}

			CBase64 base64;
			int nEncodeSize = base64.Base64EncodeSize(nFileLen);
			pBase64 = new char[nEncodeSize+1];
			memset(pBase64, 0x0, nEncodeSize+1);
			base64.EncodeBase64(pFile, pBase64, nFileLen);
			szBase64 = pBase64;

			file.Close();
			bRet = TRUE;
		}
		else
		{
			WriteLogEx("!!FileToBase64,Open file err=%u, szFilePath=%s"
				, GetLastError(), szFilePath);
		}	
	} while (FALSE);

	if (pBase64)
	{
		delete[] pBase64;
	}
	if (pFile)
	{
		delete[] pFile;
	}
	return bRet;
}

BOOL Base64ToFile(IN CString& szBase64, IN CString szFilePath)
{
	BOOL bRet = FALSE;
	char* pBase64 = NULL;
	unsigned char* pFile = NULL;
	do 
	{
		int nLen = szBase64.GetLength();
		if (nLen < 4)
		{
			WriteLogEx("!!Base64ToFile, nLen=%d, szBase64=%s", nLen, szBase64);
			break;
		}

		CBase64 base64;
		pBase64 = new char[nLen+1];
		memset(pBase64, 0x0, nLen+1);
		strcpy_s(pBase64, nLen+1, szBase64.GetString());
		int nDecodeSize = base64.Base64DecodeSize(pBase64);
		pFile = new unsigned char[nDecodeSize+1];
		memset(pFile, 0x0, nDecodeSize+1);
		base64.DecodeBase64(pBase64, pFile, nLen);
		CFile file;
		if (file.Open(szFilePath, CFile::modeCreate|CFile::modeWrite))
		{
			file.Write(pFile, nDecodeSize);
			file.Close();
			bRet = TRUE;
		}
		else
		{
			WriteLogEx("!!FileToBase64,Open file err=%u, szFilePath=%s"
				, GetLastError(), szFilePath);
			break;
		}	
	} while (FALSE);

	if (pBase64)
	{
		delete[] pBase64;
	}
	if (pFile)
	{
		delete[] pFile;
	}
	return bRet;
}

//�õ��ļ�����չ����
//��abc.txt����txt
CString GetFileExt(CString sFile)
{
	CString sTmp;
	int nPos = sFile.ReverseFind('.');
	sTmp = sFile.Right(sFile.GetLength()-nPos-1);
	return sTmp;
}

//�õ��ļ�����չ����
//��"abc.txt"����".txt"
CString GetFileExt2(CString sFile)
{
	CString sTmp;
	int nPos = sFile.ReverseFind('.');
	sTmp = sFile.Right(sFile.GetLength()-nPos);
	return sTmp;
}

//�õ��ļ�����չ����,��"abc.txt"����".txt"
BOOL GetFileExt3(CString szFile, CString &szFileName, CString &szExtName)
{
	int nPos = szFile.ReverseFind('.');
	if(0 >= nPos)
	{
		return FALSE;
	}

	int nLen = szFile.GetLength();
	szFileName = szFile.Left(nPos);
	szExtName = szFile.Right(nLen - nPos);
	return TRUE;
}

