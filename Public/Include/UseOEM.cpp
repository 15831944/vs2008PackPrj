// UseOEM.cpp: implementation of the CUseOEM class.
//
//////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "UseOEM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUseOEM::CUseOEM()
{
	m_bInitOK = false;
	m_hDll = NULL;
	m_pfnGetCfgValue = NULL;

	//加载DLL
	CString szMsg;
	CString szDllPath = _T("");
	char pszModuleName[MAX_PATH];
	GetModuleFileName(AfxGetApp()->m_hInstance, pszModuleName, MAX_PATH);
	szDllPath = pszModuleName;
	szDllPath = szDllPath.Left(szDllPath.ReverseFind('\\') + 1);
	szDllPath += _T("SystemCompile.dll");

	m_hDll = LoadLibrary(szDllPath);
	if(!m_hDll)
	{
		szMsg.Format("无法加载资源处理dll:%s. errno=%d", szDllPath, GetLastError());
		//AfxMessageBox(szMsg);
		return;
	}

	m_pfnGetCfgValue = (PGetCfgValue)GetProcAddress(m_hDll, "GetCfgValue");
	if(!m_pfnGetCfgValue)
	{
		//AfxMessageBox("无法得到函数GetCfgValue的地址");
		FreeLibrary(m_hDll);
		return;
	}

	m_bInitOK = true;
}

CUseOEM::~CUseOEM()
{
/*//师傅说用完不释放
	if(m_hDll)
		FreeLibrary(m_hDll);
*/
}

int CUseOEM::GetCfgValue(LPCTSTR szCfgName,char* pCfgValueBuf, int nCfgValueBufLen)
{
	if(!m_bInitOK || !m_pfnGetCfgValue)
		return -2;
	if(!pCfgValueBuf)
		return -3;

	return m_pfnGetCfgValue(szCfgName, pCfgValueBuf, nCfgValueBufLen);
}
bool CUseOEM::CheckNumAndChar(CString &szCheck)
{
	for(int i=0;i<szCheck.GetLength();i++)
	{
		char cTemp = szCheck.GetAt(i);
		if( (cTemp <= '9' && cTemp >= '0') || 
			(cTemp <= 'z' && cTemp >= 'a') ||
			(cTemp <= 'Z' && cTemp >= 'A') )
		{
			TRACE("");
		}
		else
		{
			return false;
		}
	}
	return true;
}
int CUseOEM::GetVersion()
{
	if(!m_bInitOK || !m_pfnGetCfgValue)
		return -1;

	char szBuf[1024];
	int nRet = GetCfgValue(DEF_KEY2_GLOBAL_CorpFullName, szBuf, 1024);
	if(0 >= nRet)
		return nRet;
	CString szValue;
	szValue.Format("%s", szBuf);
	if(0 == szValue.Compare(_T("南京苏亚星资讯科技开发有限公司")))
		return DEF_VERSION_SYX;
	else if(0 == szValue.Compare(_T("武汉噢易科技有限公司")))
		return DEF_VERSION_OYI;
	else
		return -100;
}

CString CUseOEM::GetShortCorpName()
{
	if(!m_bInitOK || !m_pfnGetCfgValue)
		return "";

	char szBuf[1024];
	int nRet = GetCfgValue(DEF_KEY2_GLOBAL_CorpShortName, szBuf, sizeof(szBuf));
	if(0 >= nRet)
		return "";

	CString szRet = szBuf;
	return szRet;
}

CString CUseOEM::GetFullCorpName()
{
	if(!m_bInitOK || !m_pfnGetCfgValue)
		return "";

	char szBuf[1024];
	int nRet = GetCfgValue(DEF_KEY2_GLOBAL_CorpFullName, szBuf, sizeof(szBuf));
	if(0 >= nRet)
		return "";

	CString szRet = szBuf;
	return szRet;
}

CString CUseOEM::GetAppTitle()
{
	if(!m_bInitOK || !m_pfnGetCfgValue)
		return "";

	char szBuf[1024];
	int nRet = GetCfgValue(DEF_KEY2_GLOBAL_AppTitle, szBuf, sizeof(szBuf));
	if(0 >= nRet)
		return "";

	CString szRet = szBuf;
	return szRet;
}

CString CUseOEM::GetHttp()
{
	if(!m_bInitOK || !m_pfnGetCfgValue)
		return "";

	char szBuf[1024];
	int nRet = GetCfgValue(DEF_KEY2_GLOBAL_Http, szBuf, sizeof(szBuf));
	if(0 >= nRet)
		return "";

	CString szRet = szBuf;
	return szRet;
}