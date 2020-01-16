#include "StdAfx.h"
#include "SnmpMonitorHelperLG.h"
#include "RicohMIB_Def.h"

#define Ricoh_Fax_Alert_Modify_20170525	0	//�Ƿ������޸ģ�0-���ã�1-���á�
											//��ʱ��дGetAlertʱ��oid�ĸ�ʽ�����ڷ��������Ĳ�һ����
											//��Ϊ����û�и������oid,���ܵ�ʱ�Ĵ�ӡ���ͺ���֧�ֵģ���ʱ����ԭ�������޸ġ�

CSnmpMonitorHelperLG::CSnmpMonitorHelperLG(void)
{
	InitOID();
}

CSnmpMonitorHelperLG::~CSnmpMonitorHelperLG(void)
{
}

void CSnmpMonitorHelperLG::InitOID()
{
	m_szRicohEngPrtAlertEntryOID = DecryptOID(RicohEngPrtAlertEntryOID);
	m_szRicohEngFAXAlertEntryOID = DecryptOID(RicohEngFAXAlertEntryOID);
	m_szRicohEngCpyAlertEntryOID = DecryptOID(RicohEngCpyAlertEntryOID);
	m_szRicohEngScnAlertEntryOID = DecryptOID(RicohEngScnAlertEntryOID);
	m_szRicohEngCounterEntryOID = DecryptOID(RicohEngCounterEntryOID);
	m_szRicohTonerColorOID = DecryptOID(RicohTonerColorOID);
	m_szRicohTonerDescOID = DecryptOID(RicohTonerDescOID);
	m_szRicohTonerColorTypeOID = DecryptOID(RicohTonerColorTypeOID);
	m_szRicohTonerLevelPercentOID = DecryptOID(RicohTonerLevelPercentOID);

	//ricohEngFAXAlertEntry��ricohEngPrtAlertEntry��ricohEngCpyAlertEntry��ricohEngScnAlertEntry��Ϣ��ȫ
	//������ricohEngFAXAlertEntryΪ��׼����ȡԤ����Ϣ��
	m_szRicohAlertEntryOID = m_szRicohEngFAXAlertEntryOID;
#if Ricoh_Fax_Alert_Modify_20170525
	m_szRicohAlertIndexOID.Format("%s.1", m_szRicohAlertEntryOID);
	m_szRicohAlertSeverityLevelOID.Format("%s.2", m_szRicohAlertEntryOID);
	m_szRicohAlertTrainingLevelOID.Format("%s.3", m_szRicohAlertEntryOID);
	m_szRicohAlertGroupOID.Format("%s.4", m_szRicohAlertEntryOID);
	m_szRicohAlertGroupIndexOID.Format("%s.5", m_szRicohAlertEntryOID);
	m_szRicohAlertLocationOID.Format("%s.6", m_szRicohAlertEntryOID);
	m_szRicohAlertCodeOID.Format("%s.7", m_szRicohAlertEntryOID);
	m_szRicohAlertDescriptionOID.Format("%s.8", m_szRicohAlertEntryOID);
	m_szRicohAlertTimeOID.Format("%s.9", m_szRicohAlertEntryOID);
#else
	m_szRicohAlertIndexOID.Format("%s.1.4", m_szRicohAlertEntryOID);
	m_szRicohAlertSeverityLevelOID.Format("%s.2.4", m_szRicohAlertEntryOID);
	m_szRicohAlertTrainingLevelOID.Format("%s.3.4", m_szRicohAlertEntryOID);
	m_szRicohAlertGroupOID.Format("%s.4.4", m_szRicohAlertEntryOID);
	m_szRicohAlertGroupIndexOID.Format("%s.5.4", m_szRicohAlertEntryOID);
	m_szRicohAlertLocationOID.Format("%s.6.4", m_szRicohAlertEntryOID);
	m_szRicohAlertCodeOID.Format("%s.7.4", m_szRicohAlertEntryOID);
	m_szRicohAlertDescriptionOID.Format("%s.8.4", m_szRicohAlertEntryOID);
	m_szRicohAlertTimeOID.Format("%s.9.4", m_szRicohAlertEntryOID);
#endif
}

//����������дCSnmpMonitorHelper::GetMarkerSupplies
//ֻ��������ī����Ϣ
BOOL CSnmpMonitorHelperLG::GetMarkerSupplies()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

	char *cOidBegin = CStringToChar(m_szRicohTonerColorOID); //RicohTonerColorOID
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oMarkerSuppliesMap.find(nIndex) == m_oMarkerSuppliesMap.end())
			{
				PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
				memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
				m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
				pEntry->prtMarkerSuppliesIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//Ĭ��Ϊī����Ϣ
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//��������ȡ���Ƿۺе�ʣ��ٷֱȣ�����Ĭ�����Ϊ����Ϊ100��
			
			int nIndex = pEntry->prtMarkerSuppliesIndex;
			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);

			sprintf(cOidStr, "%s.%d", m_szRicohTonerLevelPercentOID, nIndex);	//RicohTonerLevelPercentOID
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

//����������дCSnmpMonitorHelper::GetAlert
BOOL CSnmpMonitorHelperLG::GetAlert()
{
	CCriticalSection2::Owner lock(m_cs4AlertMap);

	//��Ϊ��һЩ�ͺŵ�����ӡ����˽�нڵ㣨m_szRicohAlertSeverityLevelOID����û��Ԥ����Ϣ��
	//���У��Ȼ�ȡ����Ĵ�ӡ����׼�ڵ㣨m_szPrtAlertSeverityLevelOID���µĴ�ӡ��Ԥ����Ϣ��
	//Ȼ���ٻ�ȡ���˽�нڵ��µ�Ԥ����Ϣ��

	//1.��ȡ��׼Ԥ����Ϣ
 	CSnmpMonitorHelper::GetAlert();

	//2.��ȡ���˽�е�Ԥ����Ϣ
	PrtAlertEntryMap oAlertMapTmp;	//��ʱԤ��map,���ջ�ϲ���m_oAlertMap��

	char *cOidBegin = CStringToChar(m_szRicohAlertSeverityLevelOID);	//ricohEngFAXAlertSeverityLevel
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oAlertMap.find(nIndex) == m_oAlertMap.end())
			{
				PPrtAlertEntry pEntry = new PrtAlertEntry;
				memset(pEntry, 0x0, sizeof(PrtAlertEntry));
				m_oAlertMap.insert(pair<int,PPrtAlertEntry>(nIndex, pEntry));
				oAlertMapTmp.insert(pair<int,PPrtAlertEntry>(nIndex, pEntry));	//ͬʱ������ʱԤ��map��
				pEntry->prtAlertIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtAlertEntryMap::iterator it;
	for (it=oAlertMapTmp.begin(); it!=oAlertMapTmp.end(); it++)
	{
		PPrtAlertEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtAlertIndex;

			char cOidStr[128] = {0};
#if Ricoh_Fax_Alert_Modify_20170525
			//sprintf(cOidStr, "%s.1.%d", m_szRicohAlertSeverityLevelOID, nIndex);	//ricohEngFAXAlertIndex	//Not-accessible
			//GetRequest(cOidStr, (int&)pEntry->prtAlertSeverityLevel);

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertTrainingLevelOID, nIndex);	//ricohEngFAXAlertTrainingLevel
			GetRequest(cOidStr, (int&)pEntry->prtAlertTrainingLevel);

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertGroupOID, nIndex);			//ricohEngFAXAlertGroup
			GetRequest(cOidStr, (int&)pEntry->prtAlertGroup);

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertGroupIndexOID, nIndex);		//ricohEngFAXAlertGroupIndex
			GetRequest(cOidStr, pEntry->prtAlertGroupIndex);

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertLocationOID, nIndex);			//ricohEngFAXAlertLocation
			GetRequest(cOidStr, pEntry->prtAlertLocation);

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertCodeOID, nIndex);				//ricohEngFAXAlertCode
			GetRequest(cOidStr, (int&)pEntry->prtAlertCode);

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertDescriptionOID, nIndex);		//ricohEngFAXAlertDescription
			GetRequest(cOidStr, pEntry->prtAlertDescription, sizeof(pEntry->prtAlertDescription));

			sprintf(cOidStr, "%s.1.%d", m_szRicohAlertTimeOID, nIndex);				//ricohEngFAXAlertTime
			GetRequest(cOidStr, pEntry->prtAlertTime);
#else
			//sprintf(cOidStr, "%s.%d", m_szRicohAlertSeverityLevelOID, nIndex);	//ricohEngFAXAlertIndex	//Not-accessible
			//GetRequest(cOidStr, (int&)pEntry->prtAlertSeverityLevel);

			sprintf(cOidStr, "%s.%d", m_szRicohAlertTrainingLevelOID, nIndex);	//ricohEngFAXAlertTrainingLevel
			GetRequest(cOidStr, (int&)pEntry->prtAlertTrainingLevel);

			sprintf(cOidStr, "%s.%d", m_szRicohAlertGroupOID, nIndex);			//ricohEngFAXAlertGroup
			GetRequest(cOidStr, (int&)pEntry->prtAlertGroup);

			sprintf(cOidStr, "%s.%d", m_szRicohAlertGroupIndexOID, nIndex);		//ricohEngFAXAlertGroupIndex
			GetRequest(cOidStr, pEntry->prtAlertGroupIndex);

			sprintf(cOidStr, "%s.%d", m_szRicohAlertLocationOID, nIndex);			//ricohEngFAXAlertLocation
			GetRequest(cOidStr, pEntry->prtAlertLocation);

			sprintf(cOidStr, "%s.%d", m_szRicohAlertCodeOID, nIndex);				//ricohEngFAXAlertCode
			GetRequest(cOidStr, (int&)pEntry->prtAlertCode);

			sprintf(cOidStr, "%s.%d", m_szRicohAlertDescriptionOID, nIndex);		//ricohEngFAXAlertDescription
			GetRequest(cOidStr, pEntry->prtAlertDescription, sizeof(pEntry->prtAlertDescription));

			sprintf(cOidStr, "%s.%d", m_szRicohAlertTimeOID, nIndex);				//ricohEngFAXAlertTime
			GetRequest(cOidStr, pEntry->prtAlertTime);
#endif
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelperLG::IsFalutInfo(CString szDetial)
{
	BOOL bYes = FALSE;
	szDetial.Trim();
	if (szDetial.Find("SC") >= 0 || szDetial.Find("����:") >= 0)
	{
		bYes = TRUE;
	}
	return bYes;
}

CString CSnmpMonitorHelperLG::GetFaultCode(CString szDetial)
{
	CString szFaultCode = "";
	szDetial.Trim();

	//��ϵ��������:SC142{40800}
	szDetial.MakeUpper();
	int nPos = szDetial.Find("SC");
	if (nPos >= 0)
	{
		CString szTemp = "";
		for (int i=nPos+2; i<szDetial.GetLength(); i++)
		{
			if (!isdigit(szDetial.GetAt(i)))
			{
				break;
			}
			szTemp.AppendFormat("%c", szDetial.GetAt(i));
		}
		if (szTemp.GetLength() > 0)
		{
			szFaultCode.Format("SC%s", szTemp);
		}
	}

	return szFaultCode;
}
