#include "StdAfx.h"
#include "SnmpMonitorHelper_JT.h"
#include "JingTuMIB_Def.h"

CSnmpMonitorHelper_JT::CSnmpMonitorHelper_JT(void)
{
	InitOID();
}

CSnmpMonitorHelper_JT::~CSnmpMonitorHelper_JT(void)
{
}

void CSnmpMonitorHelper_JT::InitOID()
{
	m_szPrtInputNameOID = DecryptOID(JingTuPrtInputNameOID);
	m_szPrtInputCurrentLevelOID = DecryptOID(JingTuPrtInputCurrentLevelOID);
	m_szPrtInputMediaTypeOID = DecryptOID(JingTuPrtInputMediaTypeOID);
}

BOOL CSnmpMonitorHelper_JT::GetCover()
{
	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverIndex
	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription
	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverStatus

	//CCriticalSection2::Owner lock(m_cs4CoverMap);

	char *cOidBegin = CStringToChar(m_szPrtCoverDescriptionOID);	//"printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription"
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
			if (m_oCoverMap.find(nIndex) == m_oCoverMap.end())
			{
				PPrtCoverEntry pEntry = new PrtCoverEntry;
				memset(pEntry, 0x0, sizeof(PrtCoverEntry));
				m_oCoverMap.insert(pair<int,PPrtCoverEntry>(nIndex, pEntry));
				pEntry->prtCoverIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtCoverEntryMap::iterator it;
	for (it=m_oCoverMap.begin(); it!=m_oCoverMap.end(); it++)
	{
		PPrtCoverEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtCoverIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.%d", m_szPrtCoverDescriptionOID, nIndex);	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription
			GetRequest(cOidStr, pEntry->prtCoverDescription, sizeof(pEntry->prtCoverDescription));

			sprintf(cOidStr, "%s.%d", m_szPrtCoverStatusOID, nIndex);	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverStatus
			GetRequest(cOidStr, (int&)pEntry->prtCoverStatus);
		}
	}

	return TRUE;
}


BOOL CSnmpMonitorHelper_JT::GetInput()
{
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputIndex 
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputType
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDimUnit
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirDeclared
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirDeclared
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirChosen
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirChosen
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCapacityUnit
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMaxCapacity
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCurrentLevel
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputStatus
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaName
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputName
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVendorName
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputModel
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVersion
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSerialNumber
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDescription
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSecurity
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaWeight
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaType
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaColor
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaFormParts
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaLoadTimeout
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputNextIndex

	//CCriticalSection2::Owner lock(m_cs4InputMap);

	char *cOidBegin = CStringToChar(m_szPrtInputTypeOID);	//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputType"
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
			if (m_oInputMap.find(nIndex) == m_oInputMap.end())
			{
				PPrtInputEntry pEntry = new PrtInputEntry;
				memset(pEntry, 0x0, sizeof(PrtInputEntry));
				m_oInputMap.insert(pair<int,PPrtInputEntry>(nIndex, pEntry));
				pEntry->prtInputIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtInputEntryMap::iterator it;
	for (it=m_oInputMap.begin(); it!=m_oInputMap.end(); it++)
	{
		PPrtInputEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtInputIndex;

			char cOidStr[128] = {0};

			sprintf(cOidStr, "%s.%d", m_szPrtInputTypeOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputType
			GetRequest(cOidStr, (int&)pEntry->prtInputType);

			sprintf(cOidStr, "%s.%d", m_szPrtInputDimUnitOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDimUnit
			GetRequest(cOidStr, (int&)pEntry->prtInputDimUnit);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaDimFeedDirDeclaredOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirDeclared
			GetRequest(cOidStr, (int&)pEntry->prtInputMediaDimFeedDirDeclared);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaDimXFeedDirDeclaredOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirDeclared
			GetRequest(cOidStr, pEntry->prtInputMediaDimXFeedDirDeclared);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaDimFeedDirChosenOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirChosen
			GetRequest(cOidStr, pEntry->prtInputMediaDimFeedDirChosen);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaDimXFeedDirChosenOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirChosen
			GetRequest(cOidStr, pEntry->prtInputMediaDimXFeedDirChosen);

			sprintf(cOidStr, "%s.%d", m_szPrtInputCapacityUnitOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCapacityUnit
			GetRequest(cOidStr, (int&)pEntry->prtInputCapacityUnit);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMaxCapacityOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMaxCapacity
			//GetRequest(cOidStr, pEntry->prtInputMaxCapacity);
			pEntry->prtInputMaxCapacity = 100;	//模拟百分比量，最大为100

			int nValue = 0;
			sprintf(cOidStr, "%s.%d", m_szPrtInputCurrentLevelOID, nIndex);	//.1.3.6.1.4.1.44966.2.2.2.2.2.2.1.4
			GetRequest(cOidStr, nValue);
			pEntry->prtInputCurrentLevel = GetPaperLevel(nValue);	//获取百分比量
			//theLog.Write("CSnmpMonitorHelper_JT::GetInput,10,prtInputCurrentLevel=%d", pEntry->prtInputCurrentLevel);

			sprintf(cOidStr, "%s.%d", m_szPrtInputStatusOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputStatus
			GetRequest(cOidStr, pEntry->prtInputStatus);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaNameOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaName
			GetRequest(cOidStr, pEntry->prtInputMediaName, sizeof(pEntry->prtInputMediaName));

			sprintf(cOidStr, "%s.%d", m_szPrtInputNameOID, nIndex);	//.1.3.6.1.4.1.44966.2.2.2.2.2.2.1.1
			//GetRequest(cOidStr, pEntry->prtInputName, sizeof(pEntry->prtInputName));
			sprintf((char *)pEntry->prtInputName, "%d", nIndex);
			//theLog.Write("CSnmpMonitorHelper_JT::GetInput,11,prtInputName=%s", pEntry->prtInputName);

			sprintf(cOidStr, "%s.%d", m_szPrtInputVendorNameOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVendorName
			GetRequest(cOidStr, pEntry->prtInputVendorName, sizeof(pEntry->prtInputVendorName));

			sprintf(cOidStr, "%s.%d", m_szPrtInputModelOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputModel
			GetRequest(cOidStr, pEntry->prtInputModel, sizeof(pEntry->prtInputModel));

			sprintf(cOidStr, "%s.%d", m_szPrtInputVersionOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVersion
			GetRequest(cOidStr, pEntry->prtInputVersion, sizeof(pEntry->prtInputVersion));

			sprintf(cOidStr, "%s.%d", m_szPrtInputSerialNumberOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSerialNumber
			GetRequest(cOidStr, pEntry->prtInputSerialNumber, sizeof(pEntry->prtInputSerialNumber));

			sprintf(cOidStr, "%s.%d", m_szPrtInputDescriptionOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDescription
			GetRequest(cOidStr, pEntry->prtInputDescription, sizeof(pEntry->prtInputDescription));

			sprintf(cOidStr, "%s.%d", m_szPrtInputSecurityOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSecurity
			GetRequest(cOidStr, (int&)pEntry->prtInputSecurity);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaWeightOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaWeight
			GetRequest(cOidStr, pEntry->prtInputMediaWeight);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaTypeOID, nIndex);	//.1.3.6.1.4.1.44966.2.2.2.2.2.2.1.9
			nValue = 0;
			GetRequest(cOidStr, nValue);
			sprintf((char*)pEntry->prtInputMediaType, "%d", nValue);
			//theLog.Write("CSnmpMonitorHelper_JT::GetInput,12,prtInputMediaType=%s", pEntry->prtInputMediaType);

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaColorOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaColor
			GetRequest(cOidStr, pEntry->prtInputMediaColor, sizeof(pEntry->prtInputMediaColor));

			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaFormPartsOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaFormParts
			GetRequest(cOidStr, pEntry->prtInputMediaFormParts);

#ifdef RFC_3805
			sprintf(cOidStr, "%s.%d", m_szPrtInputMediaLoadTimeoutOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaLoadTimeout
			GetRequest(cOidStr, pEntry->prtInputMediaLoadTimeout);

			sprintf(cOidStr, "%s.%d", m_szPrtInputNextIndexOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputNextIndex
			GetRequest(cOidStr, pEntry->prtInputNextIndex);
#endif
		}
	}

	return TRUE;
}

void CSnmpMonitorHelper_JT::CheckInput()
{
	//CCriticalSection2::Owner lock(m_cs4InputMap);

	PrtInputEntryMap::iterator it;
	for (it=m_oInputMap.begin(); it!=m_oInputMap.end(); it++)
	{
		PPrtInputEntry pEntry = it->second;
		if (pEntry)
		{
			//theLog.Write("====CSnmpMonitorHelper::CheckInput,m_eType=%d,prtInputType=%d,prtInputMaxCapacity=%d,prtInputCurrentLevel=%d, prtInputName=%s,m_pCallBack=%p"
			//	, m_eType, pEntry->prtInputType, pEntry->prtInputMaxCapacity, pEntry->prtInputCurrentLevel, pEntry->prtInputName, m_pCallBack);
			
			if (pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray
				&& pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray
				&& pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::ContinuousRoll)
			{
				continue;
			}

			int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::UnKnown;
			CString szDesc = "";
			if (pEntry->prtInputCurrentLevel == 5)
			{
				nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperEmpty;
				szDesc.Format("纸盒(%s)，纸型(%s)，无纸！", pEntry->prtInputName, GetPageSize(atoi((char*)pEntry->prtInputMediaType)));
			}
			else if (pEntry->prtInputCurrentLevel == 4)
			{
				nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperLow ;
				szDesc.Format("纸盒(%s)，纸型(%s)，少纸，请准备备用纸！", pEntry->prtInputName, GetPaperTypeByPrtInputEntry(pEntry));
			}

			if (szDesc.GetLength()>0 && m_pCallBack)
			{
				if (nAlertCode < 100)
				{
					nAlertCode += 100;	//参考:【PrinterAlert_Def.h】中的【PrinterAlertCode】信安宝扩展多组预警
				}
				m_pCallBack->OnStatus(nAlertCode, CStringToChar(szDesc));
			}
		}
	}
}

BOOL CSnmpMonitorHelper_JT::GetMarkerSupplies()
{
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesIndex
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesColorantIndex
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesClass
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesType
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesDescription
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesSupplyUnit
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMaxCapacity
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesLevel

	//CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

	char *cOidBegin = CStringToChar(m_szPrtMarkerSuppliesMarkerIndexOID);	//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex"
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
			int nIndex = pEntry->prtMarkerSuppliesIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesMarkerIndexOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesMarkerIndex);

			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesColorantIndexOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesColorantIndex
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesColorantIndex);

			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesClassOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesClass
			GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);

			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesTypeOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesType
			GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesType);

			//如果是墨粉，则从prtMarkerColorant中获取颜色信息
			if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon)
			{
				sprintf(cOidStr, "%s.%d", m_szPrtMarkerColorantValueOID, pEntry->prtMarkerSuppliesColorantIndex);	//printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantValue
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));
			}
			else
			{
				sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesDescriptionOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesDescription
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));
			}

			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesSupplyUnitOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesSupplyUnit
			GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesSupplyUnit);

			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesMaxCapacityOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMaxCapacity
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesMaxCapacity);

			sprintf(cOidStr, "%s.%d", m_szPrtMarkerSuppliesLevelOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesLevel
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelper_JT::GetAlert()
{
	//CCriticalSection2::Owner lock(m_cs4AlertMap);

	char *cOidBegin = CStringToChar(m_szPrtAlertSeverityLevelOID);	//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertSeverityLevel"
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
				pEntry->prtAlertIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtAlertEntryMap::iterator it;
	for (it=m_oAlertMap.begin(); it!=m_oAlertMap.end(); it++)
	{
		PPrtAlertEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtAlertIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.%d", m_szPrtAlertSeverityLevelOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertSeverityLevel
			GetRequest(cOidStr, (int&)pEntry->prtAlertSeverityLevel);

			sprintf(cOidStr, "%s.%d", m_szPrtAlertTrainingLevelOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTrainingLevel
			GetRequest(cOidStr, (int&)pEntry->prtAlertTrainingLevel);

			sprintf(cOidStr, "%s.%d", m_szPrtAlertGroupOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroup
			GetRequest(cOidStr, (int&)pEntry->prtAlertGroup);

			sprintf(cOidStr, "%s.%d", m_szPrtAlertGroupIndexOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroupIndex
			GetRequest(cOidStr, pEntry->prtAlertGroupIndex);

			sprintf(cOidStr, "%s.%d", m_szPrtAlertLocationOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertLocation
			GetRequest(cOidStr, pEntry->prtAlertLocation);

			sprintf(cOidStr, "%s.%d", m_szPrtAlertCodeOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
			GetRequest(cOidStr, (int&)pEntry->prtAlertCode);

			sprintf(cOidStr, "%s.%d", m_szPrtAlertDescriptionOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertDescription
			GetRequest(cOidStr, pEntry->prtAlertDescription, sizeof(pEntry->prtAlertDescription));

			sprintf(cOidStr, "%s.%d", m_szPrtAlertTimeOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTime
			GetRequest(cOidStr, pEntry->prtAlertTime);
		}
	}

	return TRUE;
}

CString CSnmpMonitorHelper_JT::GetPageSize(int code)
{
	CString pz = "";
	switch(code)
	{
	case 1:
		pz = "other";
		break;
	case 2:
		pz = "unknown";
		break;
	case 256:
		pz = "A0";
		break;
	case 257:
		pz = "A1";
		break;
	case 258:
		pz = "A2";
		break;
	case 259:
		pz = "A3";
		break;
	case 260:
		pz = "A4";
		break;
	case 261:
		pz = "A5";
		break;
	case 512:
		pz = "B0";
		break;
	case 513:
		pz = "B1";
		break;
	case 514:
		pz = "B2";
		break;
	case 515:
		pz = "B3";
		break;
	case 516:
		pz = "B4";
		break;
	case 517:
		pz = "B5";
		break;
	case 768:
		pz = "kentikuA";
		break;
	case 769:
		pz = "kentikuB";
		break;
	case 770:
		pz = "kentikuC";
		break;
	case 771:
		pz = "kentikuD";
		break;
	case 772:
		pz = "kentikuE";
		break;
	case 773:
		pz = "kentikuF";
		break;
	case 774:
		pz = "kentikuG";
		break;
	case 1024:
		pz = "ansiA";
		break;
	case 1025:
		pz = "ansiB";
		break;
	case 1026:
		pz = "ansiC";
		break;
	case 1027:
		pz = "ansiD";
		break;
	case 1028:
		pz = "ansiE";
		break;
	case 1280:
		pz = "30inchX42inch";
		break;
	case 1281:
		pz = "15inchX21inch";
		break;
	case 1536:
		pz = "mapbB1";
		break;
	case 1537:
		pz = "mapbB2";
		break;
	case 1792:
		pz = "dinB1";
		break;
	case 1793:
		pz = "dinB2";
		break;
	case 1794:
		pz = "dinB3";
		break;
	}
	return pz;
}

int CSnmpMonitorHelper_JT::GetPaperLevel(int code)
{
	int nPaperLevel = -1;
	if(code == 3)
	{
		nPaperLevel = 100;
	}
	else if(code == 4)
	{
		nPaperLevel = 25;
	}
	else if(code == 5)
	{
		nPaperLevel = 0;
	}
	return nPaperLevel;
}
