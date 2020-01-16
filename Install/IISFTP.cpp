// IISFTP.cpp: implementation of the CIISFTP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IISFTP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIISFTP::CIISFTP()
{

}

CIISFTP::~CIISFTP()
{

}
//////////////////////////////////////////
//得到Ftp站点的列表
//
//////////////////////////////////////////
void CIISFTP::GetFtpSiteList(CPtrList *pList)
{
	RemoveAllList(*pList);
	IADsContainer* iContainer = NULL;
	IADs* iAds = NULL;
	
	// 首先创建一个服务实例
	if( ADsGetObject(L"IIS://localhost/MSFTPSVC",IID_IADsContainer,(void**)&iContainer) == S_OK ) 
	{
		IEnumVARIANT *pEnum;
		LPUNKNOWN     pUnk;
		VARIANT       var;
		IDispatch    *pDisp;
		BSTR          bstrName;
		BSTR          bstrPath;
		unsigned long    lFetch;
		IADs         *pADs; 
		
		
		iContainer->get__NewEnum(&pUnk);
		
		pUnk->QueryInterface(IID_IEnumVARIANT, (void**) &pEnum);
		pUnk->Release();
		
		// Now Enumerate 
		HRESULT hr = pEnum->Next(1, &var, &lFetch);
		while(hr == S_OK)
		{
			if (lFetch == 1)
			{
				pDisp = V_DISPATCH(&var);
				pDisp->QueryInterface(IID_IADs, (void**)&pADs); 
				pDisp->Release();
				pADs->get_Name(&bstrName);
				CString sName(bstrName);		
				pADs->get_ADsPath(&bstrPath);
				CString sPath(bstrPath);
				
				_variant_t varComment;
				pADs->Get(_bstr_t("ServerComment"),&varComment);	
				CString sComment = m_fun.GetVariantStr(varComment);
				sComment.TrimLeft(" ");
				sComment.TrimRight(" ");
				if( !sComment.IsEmpty()!=0)
				{
					CString sVirPath;
					GetFtpSitePath(sPath,sVirPath);
					VARIANT varBindings;
					pADs->Get(_bstr_t("ServerBindings"),&varBindings);
					CString sBindings = GetServerBinding( &varBindings );
					int nPos = sBindings.Find(":");
					int nPos2 = sBindings.ReverseFind( ':' );
					CString sServerIP = sBindings.Left( nPos );
					CString sServerPort = sBindings.Mid(nPos+1, nPos2 - nPos -1 );
					
					//------------------------------------------
					// Save comment
					P_S_SITEINFO pInfo = new S_SITEINFO;
					ZeroMemory( pInfo, sizeof(S_SITEINFO));
					
					pInfo->nNumber = m_fun.SToN(sName);
					strcpy(pInfo->szPort,sServerPort);
					strcpy(pInfo->szIP,sServerIP);
					strcpy(pInfo->szComment, sComment);
					strcpy(pInfo->szVPath,sVirPath);
					
					pList->AddTail( pInfo );
				}
				
				pADs->Release();
			}
			hr = pEnum->Next(1, &var, &lFetch);
		}			
		
		pEnum->Release();			
	}
	if( iContainer )
	{			
		iContainer->Release();
		iContainer = NULL;
	}
}
///////////////////////////////////////////////////////////
//得到Ftp站点的path
//nNum为ftp站点号
///////////////////////////////////////////////////////////
void CIISFTP::GetFtpSitePath(int nNum,CString &sVPath)
{
	CString sFtp;
	sFtp.Format("IIS://LocalHost/MSFTPSVC/%d",nNum);	
	GetFtpSitePath(sFtp,sVPath);
}

///////////////////////////////////////////////////////////
//得到Ftp站点的path
//sIISPath为Ftp站点的IIS路径如:IIS://LocalHost/FtpSvc/1
////////////////////////////////////////////////////////////
void CIISFTP::GetFtpSitePath(CString sIISPath, CString &sVPath)
{
	IADsContainer* iContainer = NULL;
	IADs* iAds = NULL;

	HRESULT     hr;
	BSTR bsTmp = sIISPath.AllocSysString();
	hr = ADsGetObject(bsTmp,IID_IADsContainer,(void**)&iContainer);
	::SysFreeString(bsTmp);
 	// 首先创建一个服务实例
	if( hr == S_OK ) 
	{
		_bstr_t bsRoot("Root");
		hr = iContainer->GetObject(_bstr_t("IIsFtpVirtualDir"), bsRoot,(IDispatch**)&iAds);
		if( hr == S_OK )
		{
			_variant_t varComment;
			iAds->Get(_bstr_t("Path"),&varComment);
			
			sVPath = varComment.bstrVal;
			
			iAds->Release();
		}

		iContainer->Release();
	}
}

///////////////////////////////////////////////////////////
//创建Ftp站点
//LPCTSTR sComment 名称描述
//LPCTSTR sPath  路径
//int nPort  端口
////////////////////////////////////////////////////////////
BOOL CIISFTP::CreateFtpSite(LPCTSTR sComment, LPCTSTR sPath,int nPort,BOOL bStart)
{
	IADsContainer* iContainer;
	IADs* iAds;

	CString sIISPath;
	sIISPath.Format("IIS://localhost/MSFTPSVC");
 	/* 获得FtpSever */ 
	if(ADsGetObject(_bstr_t(sIISPath),IID_IADsContainer,(void**)&iContainer) != S_OK)
		return FALSE;

	//建立虚拟目录 
	int nIndex = GetUnusedFtpNum();	
	if( nIndex == 0 )
		nIndex = 10;
	if(HRESULT hr =iContainer->Create(_bstr_t("IIsFtpServer"), _bstr_t(m_fun.NToS(nIndex)),(IDispatch**)&iAds)!=S_OK)
	{
		iContainer->Release();
		return FALSE;
	}
	CString sBinding;
	sBinding.Format(":%d:",nPort);
	iAds->Put(_bstr_t("ServerSize"),_variant_t((long)1));
	iAds->Put(_bstr_t("ServerComment"),_variant_t(sComment));
	iAds->Put(_bstr_t("ServerBindings"),_variant_t(sBinding));
	iAds->Put(_bstr_t("ServerAutoStart"),_variant_t((long)1));
	//iContainer->Start();
	//iAds->Put(_bstr_t("ServerState"),_variant_t((long)1));

	iAds->SetInfo();
	BSTR bsIISPath;
	iAds->get_ADsPath(&bsIISPath);


	sIISPath = CString(bsIISPath);
	if( !CreateVirtualFtpDir(sIISPath,sPath,"ROOT") )
	{
		return FALSE;
	}


	if( bStart )
	{
	//	AfxMessageBox("开始启动");
		IADsServiceOperations *pSrvOp = NULL;
		long status = 0;
		
		HRESULT hr = iAds->QueryInterface(IID_IADsServiceOperations,(void**)&pSrvOp);
		
		
		if(pSrvOp)
		{
		//	AfxMessageBox("调用启动接口");
			HRESULT hr = pSrvOp->Start();
			CString szMsg;
			szMsg.Format("%u    Error:%d",hr,GetLastError());
		//	AfxMessageBox(szMsg);
		}
		if(pSrvOp)
			pSrvOp->Release();	
	}
	iAds->Release();
	iContainer->Release();
	return TRUE;
}
///////////////////////////////////////////////////////////
//创建Ftp站点虚拟目录
// LPCTSTR sDiskPath 硬盘路径
//LPCTSTR sVirName 虚拟目录名称
//nSvIndex Ftp站点号
////////////////////////////////////////////////////////////
BOOL CIISFTP::CreateVirtualFtpDir(int nSvIndex,LPCTSTR sDiskPath,LPCTSTR sVirName)
{
	CString sPath;
	sPath.Format("IIS://localhost/MSFTPSVC/%d/Root",nSvIndex);
	return CreateVirtualFtpDir(sPath,sDiskPath,sVirName);
}

///////////////////////////////////////////////////////////
//创建Ftp站点虚拟目录
//LPCTSTR sIISPath Ftp站点的IIS路径如:IIS://LocalHost/w3svc/1
// LPCTSTR sDiskPath 硬盘路径
//LPCTSTR sVirName 虚拟目录名称
////////////////////////////////////////////////////////////
BOOL CIISFTP::CreateVirtualFtpDir(LPCTSTR sIISPath,LPCTSTR sDiskPath,LPCTSTR sVirName)
{
	IADsContainer* iContainer;
	IADs* iAds;

	CString lpszDiskPath = sDiskPath;
	CString lpszVirtualDirName = sVirName;

 	/* 获得FtpSever */ 
	HRESULT hr= ADsGetObject(_bstr_t(sIISPath),IID_IADsContainer,(void**)&iContainer);
	if( hr != S_OK)
		return FALSE;

	//先检测了虚拟目录 是否存在
	if( this->IsVirtualDirExist(sIISPath,sVirName) )
	{
		TRACE(" The same virtual directory is already exist!\n");
		if(iContainer->GetObject(_bstr_t("IIsFtpVirtualDir"), _bstr_t(lpszVirtualDirName),(IDispatch**)&iAds) != S_OK)
		{
			iContainer->Release();
			return FALSE;
		}
		iContainer->Release();
	}
	else
	{
		//建立虚拟目录 
		if(iContainer->Create(_bstr_t("IIsFtpVirtualDir"), _bstr_t(lpszVirtualDirName),(IDispatch**)&iAds)!=S_OK)
		{
			iContainer->Release();
			return FALSE;
		}
		iContainer->Release();
	}

	//设置虚拟目录的属性 
	iAds->Put(_bstr_t("AccessRead"),_variant_t("True"));//注意跟VB中的设置属性比较
	iAds->Put(_bstr_t("AccessWrite"),_variant_t("true"));
	iAds->Put(_bstr_t("DontLog"),_variant_t("TRUE"));
	iAds->Put(_bstr_t("Path"),_variant_t(lpszDiskPath));
	iAds->SetInfo();
	iAds->Release();

	return TRUE;
}

///////////////////////////////////////////////////////////
//检查站点的虚拟目录是否存在
//nSvIndex为FTP站点号
//////////////////////////////////////////////////////////
BOOL CIISFTP::IsVirtualDirExist(LPCTSTR sVirDir,int nSvIndex)
{
	IADsContainer* iContainer;

	CString sW3Path;
	sW3Path.Format("IIS://localhost/MSFTPSVC/%d/Root/%s",nSvIndex,sVirDir);
 	/* 获得FTPSever */ 
	HRESULT hr1 = ADsGetObject(_bstr_t(sW3Path),IID_IADsContainer,(void**)&iContainer);
	if( hr1 == S_OK)
	{	
		iContainer->Release();
		return TRUE;
	}

	return FALSE;
}
///////////////////////////////////////////////////////////
//检查站点的虚拟目录是否存在
//nSvIndex为FTP站点号
//////////////////////////////////////////////////////////
BOOL CIISFTP::IsVirtualDirExist(LPCTSTR sIISPath,LPCTSTR sVirDir)
{
	IADsContainer* iContainer;

	CString sW3Path;
	sW3Path.Format("%s/%s",sIISPath,sVirDir);
 	/* 获得FTPSever */ 
	HRESULT hr1 = ADsGetObject(_bstr_t(sW3Path),IID_IADsContainer,(void**)&iContainer);
	if( hr1 == S_OK)
	{	
		iContainer->Release();
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////
//删除站点
//nIndex为Ftp站点号
//////////////////////////////////////////////////////////
BOOL CIISFTP::DeleteFtpSite(int nIndex)
{	
	IADsContainer* iContainer = NULL;
	IADs* iAds = NULL;
	CString sPath;
	sPath.Format("IIS://localhost/MSFTPSVC");	
	// 首先创建一个服务实例
	HRESULT hr =  ADsGetObject(_bstr_t(sPath),IID_IADsContainer,(void**)&iContainer);
	if(hr == S_OK ) 
	{
		BSTR bstrText = _com_util::ConvertStringToBSTR(m_fun.NToS(nIndex));

		hr = iContainer->Delete(_bstr_t("IIsFtpServer"),bstrText);
		iContainer->Release();
		SysFreeString(bstrText);

	}

	return TRUE;
}

///////////////////////////////////////////////////////////
//得到最小的一个未用的站点号
//////////////////////////////////////////////////////////
int CIISFTP::GetUnusedFtpNum()
{
	int nIndex = 0;
	IADsContainer* iContainer = NULL;
	IADs* iAds = NULL;
	
	// 首先创建一个服务实例
	if( ADsGetObject(L"IIS://localhost/MSFTPSVC",IID_IADsContainer,(void**)&iContainer) == S_OK ) 
	{
		for(int i=1; i<100; i++)
		{
			CString sNumber;
			sNumber.Format("%d", i);
			
			if( iContainer->GetObject(_bstr_t("IIsFtpServer"), _bstr_t( sNumber.operator LPCTSTR() ),(IDispatch**)&iAds) == S_OK )
			{				
				iAds->Release();
			}
			else
			{
				nIndex = i;
				break;
			}
			
		}
		
		iContainer->Release();
	}
	return nIndex;
}
///////////////////////////////////////////////////////////
//得到WebBinding
//nWebNum为web站点号
//////////////////////////////////////////////////////////
CString CIISFTP::GetFtpBinding(int nFtpNum)
{
	CString sBindings;
	IADsContainer* iContainer = NULL;
	IADs* iAds = NULL;
 	
	CString sFtp;
	sFtp.Format("%d",nFtpNum);
	if( ADsGetObject(L"IIS://localhost/MSFTPSVC",IID_IADsContainer,(void**)&iContainer) == S_OK ) 
	{
		if( iContainer->GetObject(_bstr_t("IIsFtpServer"), _bstr_t( sFtp.operator LPCTSTR() ),(IDispatch**)&iAds) == S_OK )
		{
			VARIANT varBindings;
			int nRet;
			nRet = iAds->Get(_bstr_t("ServerBindings"),&varBindings);
			sBindings = GetServerBinding( &varBindings );			
			iAds->Release();
		}

		iContainer->Release();
	}

	return sBindings;
}

BOOL CIISFTP::SetFtpSitePath(int nIndex, CString sPath)
{
	IADsContainer* iContainer;
	IADs* iAds;

	CString sIISPath;
	sIISPath.Format("IIS://localhost/MSFTPSVC/%d",nIndex);
 	/* 获得FtpSever */ 
	if(ADsGetObject(_bstr_t(sIISPath),IID_IADsContainer,(void**)&iContainer) != S_OK)
		return FALSE;

	if( !iContainer->GetObject(_bstr_t("IIsFtpVirtualDir"), _bstr_t("ROOT"),(IDispatch**)&iAds) == S_OK )
	{
		return FALSE;
	}

	

	//设置虚拟目录的属性 	
	_variant_t var;
	var.vt = VT_BOOL;
	var.boolVal = true;
	iAds->Put(_bstr_t("AccessRead"),var);
	iAds->Put(_bstr_t("AccessWrite"),var);
	var.boolVal = false;
	iAds->Put(_bstr_t("DontLog"),var);
	iAds->Put(_bstr_t("Path"),_variant_t(sPath));
	iAds->SetInfo();

	iAds->Release();
	iContainer->Release();

	return TRUE;
}

BOOL CIISFTP::SetFTPProperty(int nIndex, CString szName,BOOL bValue)
{
	CString sBindings;
	IADsContainer* iContainer = NULL;
	IADs* iAds = NULL;
 	
	CString sFtp;
	sFtp.Format("%d",nIndex);
	if( ADsGetObject(L"IIS://localhost/MSFTPSVC",IID_IADsContainer,(void**)&iContainer) == S_OK ) 
	{
		if( iContainer->GetObject(_bstr_t("IIsFtpServer"), _bstr_t( sFtp.operator LPCTSTR() ),(IDispatch**)&iAds) == S_OK )
		{
			VARIANT varBindings;
			int nRet;
			_variant_t var;
			var.vt = VT_BOOL;
			var.boolVal = bValue;
			nRet = iAds->Put(_bstr_t(szName),var);
			iAds->SetInfo();
			iAds->Release();
		}

		iContainer->Release();
	}
	return true;
}
