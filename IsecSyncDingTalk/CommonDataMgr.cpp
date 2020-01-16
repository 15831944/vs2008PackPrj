#include "stdafx.h"
#include "CommonDataMgr.h"

CQtMgrGlobalDataConfig::CQtMgrGlobalDataConfig()
{
	LoadConfig();
}

CQtMgrGlobalDataConfig& CQtMgrGlobalDataConfig::GetInstance()
{
	static CQtMgrGlobalDataConfig one ;
	return one;
}

CString CQtMgrGlobalDataConfig::GetLocalMaxMac()
{
	CString szLocalMaxMac = ERROR_MAC_ADDRESS;

	m_szLocalMacList.RemoveAll();
	CCommonFun::GetLocalMacList(m_szLocalMacList);
	for (int i=0; i<m_szLocalMacList.GetCount(); i++)
	{
		CString szLocalMac = m_szLocalMacList.GetAt(i);
		szLocalMac.MakeUpper();
		if (szLocalMaxMac.Compare(szLocalMac) < 0)
		{
			szLocalMaxMac = szLocalMac;
		}
	}
	//获取MAC失败，则使用默认IP的MAC
	if (szLocalMaxMac.CompareNoCase(ERROR_MAC_ADDRESS) == 0)
	{
		szLocalMaxMac = CCommonFun::GetLocalMac(CCommonFun::GetLocalIP());
	}
	return szLocalMaxMac;
}

BOOL CQtMgrGlobalDataConfig::IsLocalMac(CString szMac)
{
	if (m_szLocalMacList.GetCount() <= 0)
	{
		CCommonFun::GetLocalMacList(m_szLocalMacList);
	}

	for (int i=0; i<m_szLocalMacList.GetCount(); i++)
	{
		CString szLocalMac = m_szLocalMacList.GetAt(i);
		if (szMac.CompareNoCase(szLocalMac) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CQtMgrGlobalDataConfig::LoadConfig()
{
	DelayLoad();	//延迟加载

	CDiskSerial disk;
	m_szLocalHdd = disk.GetDiskSerialID();
	m_szLocalIP = CCommonFun::GetLocalIP();
	m_szLocalSubnetMask = CCommonFun::GetSubnetMask(m_szLocalIP);
	m_szLocalMac = GetLocalMaxMac();
	m_szLocalHost = CCommonFun::GetLoaclHost();
	m_szCPUID = CCommonFun::GetCPUID();
	theLog.Write("CQtMgrGlobalDataConfig::LoadConfig, m_szLocalHdd=[%s],m_szLocalIP=[%s],m_szLocalSubnetMask=[%s],m_szLocalMac=[%s],m_szLocalHost=[%s],m_szCPUID=[%s]"
		, m_szLocalHdd, m_szLocalIP, m_szLocalSubnetMask, m_szLocalMac, m_szLocalHost, m_szCPUID);
}

void CQtMgrGlobalDataConfig::DelayLoad()
{
	//延迟加载,缓解当windows刚刚开机并且还未初始化好网络设备时，LoadConfig()中出现获取IP和MAC错误。
	//支持的时间范围是0-1800秒，默认是60*2秒
	int nDelayLoadSecond = theIniFile.GetVal("Setting", "DelayLoadSecond", 60*2);
	if (nDelayLoadSecond > 1800)
	{
		nDelayLoadSecond = 1800;
	}
	if (nDelayLoadSecond < 0)
	{
		nDelayLoadSecond = 0;
	}
	theLog.Write("CQtMgrGlobalDataConfig::DelayLoad,nDelayLoadSecond=%d", nDelayLoadSecond);
	int nDelayLoadMilliSecond = nDelayLoadSecond * 1000;
	while (GetTickCount() < nDelayLoadMilliSecond)
	{
		//theLog.Write("CQtMgrGlobalDataConfig::DelayLoad,GetTickCount()=%u,nDelayLoadMilliSecond=%d", GetTickCount(), nDelayLoadMilliSecond);
		Sleep(500);
	}
}

CCloudWebServiceHelper& CQtMgrGlobalDataConfig::GetWebService()
{
	if (!m_oWebService.IsInited())
	{
		ConnWebService();
	}
	return m_oWebService;
}

BOOL CQtMgrGlobalDataConfig::ConnWebService()
{
	if (m_oWebService.IsInited())
	{
		theLog.Write("CQtMgrGlobalDataConfig::ConnWebService,1.0");
		return TRUE;
	}
	CString szEntKey = theIniFile.GetVal("NetWork", "EntKey", /*XabEntKey*/"");
	CString szWebserviceKey = theIniFile.GetVal("NetWork", "WebserviceKey", WebServiceKey);
	CString szServerIP = theIniFile.GetVal("NetWork", "ServiceIP", WebServiceDomain);
	int nServerPort = theIniFile.GetVal("NetWork", "ServicePort", WebServicePort);
	CString szServiceUrl = theIniFile.GetVal("NetWork", "ServiceUrl", "");
	if ((szServiceUrl.GetLength() <= 0) && (szServerIP.GetLength() <= 0))
	{
		szServiceUrl.Format(WebServiceURL);
	}
	else if ((szServiceUrl.GetLength() <= 0) && (szServerIP.GetLength() > 0))
	{
		szServiceUrl.Format(WebServiceURLFormat, szServerIP, nServerPort);
	}
	int nServiceTimeout = theIniFile.GetVal("NetWork", "ServiceTimeout", WebServiceTimeout);
	BOOL bIEProxy = theIniFile.GetVal("NetWork", "IEProxy", FALSE);
	CString szIEProxyAddr = theIniFile.GetVal("NetWork", "IEProxyAddr", "");
	int nIEProxyPort = theIniFile.GetVal("NetWork", "IEProxyPort", 808);

// 	m_oWebService.SetEntKey(szEntKey);
	m_oWebService.ConfigWebService(szWebserviceKey, szServiceUrl, nServiceTimeout, bIEProxy, szIEProxyAddr, nIEProxyPort);
	return m_oWebService.ConnWebService();
}

BOOL CQtMgrGlobalDataConfig::IsConnWebService()
{
	return m_oWebService.IsInited();
}

void CQtMgrGlobalDataConfig::ReleaseWebService()
{
	m_oWebService.ReleaseWebService();
}
BOOL CQtMgrGlobalDataConfig::GetEnterpriseInfo()
{
	CString szEntKey = theIniFile.GetVal("Enterprise", "EntKey", /*XabEntKey*/"");
	GetWebService().SetEntKey(szEntKey);
	if (!GetWebService().GetEnterpriseInfo(m_oEnterpriseInfo))
	{
		theLog.Write("!!CQtMgrGlobalDataConfig::GetEnterpriseInfo,GetEnterpriseInfo fail");
		return FALSE;
	}

	theLog.Write("CQtMgrGlobalDataConfig::GetEnterpriseInfo succ, ent_id=%d, ent_key=%s, ent_name=%s, comment=%s"
		", level=%d, audit=%d, parent_id=%d, date_type=%d, organization_code=%s, business_license=%s"
		", is_default=%d, leftmoney=%d, state=%d, begin_time=%s, end_time=%s, approval_time=%s, create_time=%s"
		, m_oEnterpriseInfo.ent_id(), m_oEnterpriseInfo.ent_key().c_str()
		, m_oEnterpriseInfo.ent_name().c_str(), m_oEnterpriseInfo.comment().c_str()
		, m_oEnterpriseInfo.level(), m_oEnterpriseInfo.audit()
		, m_oEnterpriseInfo.parent_id(), m_oEnterpriseInfo.date_type()
		, m_oEnterpriseInfo.organization_code().c_str(), m_oEnterpriseInfo.business_license().c_str()
		, m_oEnterpriseInfo.is_default(), m_oEnterpriseInfo.leftmoney(), m_oEnterpriseInfo.state()
		, CCommonFun::DoubleToTimeString(m_oEnterpriseInfo.begin_time())
		, CCommonFun::DoubleToTimeString(m_oEnterpriseInfo.end_time())
		, CCommonFun::DoubleToTimeString(m_oEnterpriseInfo.approval_time())
		, CCommonFun::DoubleToTimeString(m_oEnterpriseInfo.create_time()));
	return TRUE;
}

BOOL CQtMgrGlobalDataConfig::GetEquipInfo()
{
	BOOL bSucc = FALSE;
	CString szEntKey = theIniFile.GetVal("Enterprise", "EntKey", /*XabEntKey*/"");
	CString szVersion = theIniFile.GetVal("Version", "Version", "");
	m_oEquipInfo.set_entkey(szEntKey);
	m_oEquipInfo.set_ip(CQtMgrGlobalDataConfig::GetInstance().m_szLocalIP);
	m_oEquipInfo.set_mac(CQtMgrGlobalDataConfig::GetInstance().m_szLocalMac);
	m_oEquipInfo.set_host(CQtMgrGlobalDataConfig::GetInstance().m_szLocalHost);
	m_oEquipInfo.set_hdd(CQtMgrGlobalDataConfig::GetInstance().m_szLocalHdd);
	m_oEquipInfo.set_version(szVersion);
	m_oEquipInfo.set_equiptype(Cloud_Print_Equip_Type_DataSync);
	m_oEquipInfo.set_runsystem(Cloud_Print_OS_Type_Windows);	//运行的操作系统
	m_oEquipInfo.set_equipcode(CQtMgrGlobalDataConfig::GetInstance().m_szCPUID);
	m_oEquipInfo.set_equipname(CQtMgrGlobalDataConfig::GetInstance().m_szLocalHost);

	m_oWebService.SetEntKey(szEntKey);

	if (GlobalWebService.RegisterEquip(m_oEquipInfo))
	{
		theLog.Write("CQtMgrGlobalDataConfig::GetEquipInfo,succ,EquipId=%d,Status=%d"
			, m_oEquipInfo.equipid(), m_oEquipInfo.status());
		bSucc = TRUE;
		if (m_oEquipInfo.status() == Cloud_Print_Equip_Status_Enable)
		{
			
		}
		else
		{
			theLog.Write("!!CQtMgrGlobalDataConfig::GetEquipInfo,1,设备已经禁用,将退出进程，请联系管理员！");
			Sleep(1000*60);
			exit(0);
		}
	}
	else
	{
		theLog.Write("!!CQtMgrGlobalDataConfig::GetEquipInfo,fail");
	}
	return bSucc;
}

int CQtMgrGlobalDataConfig::GetEquipId()
{
	int nEquipId = m_oEquipInfo.has_equipid() ? m_oEquipInfo.equipid() : 0;
	if (nEquipId <= 0)
	{
		if (GetEquipInfo())
		{
			nEquipId = m_oEquipInfo.equipid();
		}
	}
	return nEquipId;
}
