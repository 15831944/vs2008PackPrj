#pragma once

#define GlobalData					CQtMgrGlobalDataConfig::GetInstance()			//��ȡȫ������ʵ���ĺ�
#define GlobalWebService			CQtMgrGlobalDataConfig::GetInstance().GetWebService()		//��ȡȫ��WebServiceʵ���ĺ�
#define IsConnGlobalWebService		CQtMgrGlobalDataConfig::GetInstance().IsConnWebService()	//�ж�ȫ��WebService�Ƿ�����

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
	CString GetLocalMaxMac();	//��ȡ��������������������MAC
	BOOL IsLocalMac(CString szMac);	//�ж�һ��MAC�Ƿ�Ϊ��������������MAC

public:
	CString m_szLocalHdd;
	CString m_szLocalIP;
	CString m_szLocalSubnetMask;
	CString m_szLocalMac;
	CStringArray m_szLocalMacList;
	CString m_szLocalHost;
	CString m_szCPUID;

	CCloudWebServiceHelper m_oWebService;
	Cloud_Print_Equip_Info m_oEquipInfo;		//�豸��Ϣ
	Cloud_EnterpriseInfo m_oEnterpriseInfo;		//��ҵ��Ϣ
};
