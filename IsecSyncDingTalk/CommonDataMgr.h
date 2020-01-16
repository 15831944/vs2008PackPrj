#pragma once

#define GlobalData					CQtMgrGlobalDataConfig::GetInstance()			//获取全局数据实例的宏
#define GlobalWebService			CQtMgrGlobalDataConfig::GetInstance().GetWebService()		//获取全局WebService实例的宏
#define IsConnGlobalWebService		CQtMgrGlobalDataConfig::GetInstance().IsConnWebService()	//判断全局WebService是否连接

class CQtMgrGlobalDataConfig
{
protected:
	CQtMgrGlobalDataConfig();
public:
	static CQtMgrGlobalDataConfig& GetInstance();
	void LoadConfig();
	void DelayLoad();
	CCloudWebServiceHelper& GetWebService();
	BOOL ConnWebService();
	BOOL IsConnWebService();
	void ReleaseWebService();
	BOOL GetEnterpriseInfo();
	BOOL GetEquipInfo();
	int GetEquipId();
	CString GetLocalMaxMac();	//获取本机的物理网卡的最大的MAC
	BOOL IsLocalMac(CString szMac);	//判断一个MAC是否为本机的物理网卡MAC

public:
	CString m_szLocalHdd;
	CString m_szLocalIP;
	CString m_szLocalSubnetMask;
	CString m_szLocalMac;
	CStringArray m_szLocalMacList;
	CString m_szLocalHost;
	CString m_szCPUID;

	CCloudWebServiceHelper m_oWebService;
	Cloud_Print_Equip_Info m_oEquipInfo;		//设备信息
	Cloud_EnterpriseInfo m_oEnterpriseInfo;		//企业信息
};
