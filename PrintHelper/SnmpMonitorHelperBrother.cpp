#include "StdAfx.h"
#include "SnmpMonitorHelperBrother.h"

#include "BrotherMIB_Def.h"
#include <algorithm>


CSnmpMonitorHelperBrother::CSnmpMonitorHelperBrother(void)
{
	InitOID();
}

void CSnmpMonitorHelperBrother::InitOID()
{
	m_szBrotherModelOID = DecryptOID(BrotherModelOID);
	m_szBrotherMarkerSuppliesMaxCapacityOID_LenovoLJ3650DN = DecryptOID(m_szBrotherMarkerSuppliesMaxCapacityOID_LenovoLJ3650DN);
	m_szBrotherMarkerSuppliesLevelOID_LenovoLJ3650DN = DecryptOID(m_szBrotherMarkerSuppliesLevelOID_LenovoLJ3650DN);

	
}

CSnmpMonitorHelperBrother::~CSnmpMonitorHelperBrother(void)
{
}
BOOL CSnmpMonitorHelperBrother::GetMarkerSupplies()	//重写获取耗材的接口
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
	m_szModel = GetBrotherModel();
	if (m_szModel.Find("Lenovo LJ3650DN") >= 0)
	{
		//自定义一种墨盒耗材
		for (int nIndex = 1;nIndex < 2; nIndex ++)
		{
			if (m_oMarkerSuppliesMap.find(nIndex) == m_oMarkerSuppliesMap.end())
			{
				PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
				memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
				m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
				pEntry->prtMarkerSuppliesIndex = nIndex;
			}

		}

		PrtMarkerSuppliesEntryMap::iterator it;
		for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
		{
			PPrtMarkerSuppliesEntry pEntry = it->second;
			if (pEntry)
			{
				if (it->first == 1)//粉盒
				{
					pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
					pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
					
					pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

					int nIndex = pEntry->prtMarkerSuppliesIndex;
					char cOidStr[128] = {0};
					sprintf(cOidStr, "%s", m_szBrotherMarkerSuppliesMaxCapacityOID_LenovoLJ3650DN);	
					GetRequest(cOidStr, pEntry->prtMarkerSuppliesMaxCapacity);
					memset(cOidStr,0x0,128);
					sprintf(cOidStr, "%s", m_szBrotherMarkerSuppliesLevelOID_LenovoLJ3650DN);	//
					GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
				}
			}
		}

		return TRUE;
	}
	else
	{
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
				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesMarkerIndexOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesMarkerIndex);

				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesColorantIndexOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesColorantIndex
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesColorantIndex);

				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesClassOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesClass
				GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);

				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesTypeOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesType
				GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesType);

				//如果是墨粉，则从prtMarkerColorant中获取颜色信息
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerColorantValueOID, pEntry->prtMarkerSuppliesColorantIndex);	//printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantValue
					GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));
				}
				else
				{
					sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesDescriptionOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesDescription
					GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));
				}

				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesSupplyUnitOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesSupplyUnit
				GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesSupplyUnit);

				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesMaxCapacityOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMaxCapacity
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesMaxCapacity);

				sprintf(cOidStr, "%s.1.%d", m_szPrtMarkerSuppliesLevelOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesLevel
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
			}
		}

		return TRUE;
	}	
}
CString CSnmpMonitorHelperBrother::GetBrotherModel()
{
	CString szModel;
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szBrotherModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpMonitorHelperBrother::GetBrotherModel,获取型号失败");
	}
	szModel.Format("%s",tmp);
	theLog.Write("CSnmpMonitorHelperBrother::GetBrotherModel,szModel=%s",szModel);
	return szModel;

}