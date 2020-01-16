#pragma once

class CDbOperateHelper
{
public:
	CDbOperateHelper(void);
	~CDbOperateHelper(void);

public:
	static BOOL InitDataBase();
	static void ReleaseDataBase();

	BOOL IsNeedDealCloudJob();

	BOOL GetNeedDealCloudJob(CListCloudJobInfo& list);
	BOOL UpdateCloudJob(PCloudJobInfo job);
	BOOL UpdateCloudJobState(PCloudJobInfo job, int nState);
	BOOL GetSystemSetting(CString& sName,CStringArray& ary);
	CString GetTimeStr(double fTime);
	BOOL CheckDbConn();
	BOOL ReConnDb();

protected:
	CAdo* m_pAdo;
	CCriticalSection2 m_cs;
};


class CPrintGlobalConfig
{
protected:
	CPrintGlobalConfig()
	{
		m_sScanDir.Empty();
		m_sScanHttpDir.Empty();
		m_sPrintDir.Empty();
		m_sPrintHttpDir.Empty();
		m_sBackupDir.Empty();

		m_nVersion = 1;
		m_bIsUseFtpStogeCloudDoc = FALSE;
		memset(&m_oFtpInfo, 0x0, sizeof(FtpInfo));
		m_bCloudDocDealAfterIsGz = TRUE;
		m_bCloudDocDealBeforeIsGz = TRUE;
		m_nOfficeDealFormatType = Cloud_Doc_Format_PDF;
		m_nOfficeDealFormatDpi = 300;
		m_nImageDealFormatType = Cloud_Doc_Format_TIF;
		m_nImageDealFormatDpi = 300;
	}
public:
	CString m_sScanDir;
	CString m_sScanHttpDir;
	CString m_sPrintDir;
	CString m_sPrintHttpDir;
	CString m_sBackupDir;
	int m_nVersion;

	FtpInfo m_oFtpInfo;
	BOOL m_bIsUseFtpStogeCloudDoc;
	BOOL m_bCloudDocDealBeforeIsGz;	//���ĵ�����ǰ�Ƿ�ѹ��
	BOOL m_bCloudDocDealAfterIsGz;	//���ĵ�������Ƿ�ѹ��
	int m_nOfficeDealFormatType;	//��office�����ĵ�������ĸ�ʽ����ʱֻ֧�֡�pdf/tif�����֡�
	int m_nOfficeDealFormatDpi;		//��office�����ĵ�������Ϊ��.tif����ʽ��dpi,��.pdf����֧��dpi������
	int m_nImageDealFormatType;		//��ͼƬ�����ĵ�������ĸ�ʽ��֧�֡�jpg/png/bmp/tif/gif����
	int m_nImageDealFormatDpi;		//��ͼƬ�����ĵ�������Ϊ��setvalue5����ʽ��dpi��

	void Load()
	{
		CDbOperateHelper helper;
		if (!helper.CheckDbConn())
		{
			theLog.Write("!!!!CPrintGlobalConfig::Load,CheckDbConn fail,break Load.");
			return;
		}
		CString sName = _T("ScanDir");
		CStringArray ary;
		helper.GetSystemSetting(sName,ary);
		if (ary.GetCount() >= 3)
		{
			m_sScanDir = ary.GetAt(1);
			m_sScanHttpDir = ary.GetAt(2);
		}
		sName = _T("PrintDir");
		ary.RemoveAll();
		helper.GetSystemSetting(sName,ary);
		if (ary.GetCount() >= 3)
		{
			m_sPrintDir = ary.GetAt(1);
			m_sPrintHttpDir = ary.GetAt(2);
		}
		sName = _T("wdbfml");
		ary.RemoveAll();
		helper.GetSystemSetting(sName,ary);
		if (ary.GetCount() >= 3)
		{
			m_sBackupDir = ary.GetAt(1);
		}
		sName = _T("Version");
		ary.RemoveAll();
		helper.GetSystemSetting(sName,ary);
		if (ary.GetCount() >= 2)
		{
			//0����ҵ�棬1��У԰��, 2:У԰�ͳɼ���3���ɼ�
			m_nVersion = atoi(ary.GetAt(1));
		}
		else
		{
			m_nVersion = 1;
		}

		sName = _T("cloud_doc_stoge_ftp_info");
		ary.RemoveAll();
		helper.GetSystemSetting(sName,ary);
		if (ary.GetCount() >= 11)
		{
			strcpy(m_oFtpInfo.szFtpIP, ary.GetAt(1));
			m_oFtpInfo.nFtpPort = atoi(ary.GetAt(2));
			strcpy(m_oFtpInfo.szFtpUser, ary.GetAt(3));
			strcpy(m_oFtpInfo.szFtpPwd, ary.GetAt(4));
			strcpy(m_oFtpInfo.szFtpDir, ary.GetAt(5));
			strcpy(m_oFtpInfo.szFtpDiskDir, ary.GetAt(6));
			m_bIsUseFtpStogeCloudDoc = atoi(ary.GetAt(10));
		}
		else
		{
			m_bIsUseFtpStogeCloudDoc = FALSE;
			memset(&m_oFtpInfo, 0x0, sizeof(FtpInfo));
		}
		theLog.Write("CPrintGlobalConfig::Load,szFtpDiskDir=%s,m_bIsUseFtpStogeCloudDoc=%d"
			, m_oFtpInfo.szFtpDiskDir, m_bIsUseFtpStogeCloudDoc);

		sName = _T("cloud_doc_deal_policy");
		ary.RemoveAll();
		helper.GetSystemSetting(sName,ary);
		if (ary.GetCount() >= 11)
		{
			/*���ĵ�������ԣ�
			setvalue1�����ĵ�����ǰ�Ƿ�ѹ����0-��ѹ����1-ѹ����
			setvalue2�����ĵ�������Ƿ�ѹ����0-��ѹ����1-ѹ����
			setvalue3,��office�����ĵ�������ĸ�ʽ����ʱֻ֧�֡�pdf/tif�����֡�
			setvalue4,��office�����ĵ�������Ϊ��.tif����ʽ��dpi,��.pdf����֧��dpi������
			setvalue5,��ͼƬ�����ĵ�������ĸ�ʽ��֧�֡�jpg/png/bmp/tif/gif����
			setvalue6,��ͼƬ�����ĵ�������Ϊ��setvalue5����ʽ��dpi��*/
			m_bCloudDocDealBeforeIsGz = atoi(ary.GetAt(1));
			m_bCloudDocDealAfterIsGz = atoi(ary.GetAt(2));
			CString szOfficeDealFormatType = ary.GetAt(3);
			szOfficeDealFormatType.Trim(".");
			szOfficeDealFormatType = "." + szOfficeDealFormatType;
			m_nOfficeDealFormatType = (szOfficeDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_PDF) == 0) ? Cloud_Doc_Format_PDF : Cloud_Doc_Format_TIF;
			m_nOfficeDealFormatDpi = atoi(ary.GetAt(4));
			CString szImageDealFormatType = ary.GetAt(5);
			szImageDealFormatType.Trim(".");
			szImageDealFormatType = "." + szImageDealFormatType;
			m_nImageDealFormatType = Cloud_Doc_Format_TIF;	//Ĭ��tif
			if (szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_TIF) == 0
				|| szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_TIFF) == 0)
			{
				//m_nImageDealFormatType = Cloud_Doc_Format_TIF;
			}
			else if (szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_JPG) == 0
				|| szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_JPEG) == 0)
			{
				m_nImageDealFormatType = Cloud_Doc_Format_JPG;
			}
			else if (szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_PNG) == 0)
			{
				m_nImageDealFormatType = Cloud_Doc_Format_PNG;
			}
			else if (szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_BMP) == 0)
			{
				m_nImageDealFormatType = Cloud_Doc_Format_BMP;
			}
			else if (szImageDealFormatType.CompareNoCase(Cloud_Doc_Format_Suffix_GIF) == 0)
			{
				m_nImageDealFormatType = Cloud_Doc_Format_GIF;
			}
			m_nImageDealFormatDpi = atoi(ary.GetAt(6));
		}
		else
		{
			m_bCloudDocDealAfterIsGz = TRUE;
			m_bCloudDocDealBeforeIsGz = TRUE;
			m_nOfficeDealFormatType = Cloud_Doc_Format_PDF;
			m_nOfficeDealFormatDpi = 300;
			m_nImageDealFormatType = Cloud_Doc_Format_TIF;
			m_nImageDealFormatDpi = 300;
		}
		theLog.Write("CPrintGlobalConfig::Load"
			",m_bCloudDocDealBeforeIsGz=%d,m_bCloudDocDealAfterIsGz=%d"
			",m_nOfficeDealFormatType=%d,m_nOfficeDealFormatDpi=%d"
			",m_nImageDealFormatType=%d,m_nImageDealFormatDpi=%d"
			, m_bCloudDocDealBeforeIsGz, m_bCloudDocDealAfterIsGz
			, m_nOfficeDealFormatType, m_nOfficeDealFormatDpi
			, m_nImageDealFormatType, m_nImageDealFormatDpi);
	}

	static CPrintGlobalConfig& GetInstance()
	{
		static CPrintGlobalConfig one;
		return one;
	}
};
