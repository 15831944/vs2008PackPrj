#include "StdAfx.h"
#include "SnmpMonitorHelperHP.h"
#include "HewlettPackardMIB_Def.h"
#include <algorithm>
#define DEF_HTTP	"http://"
#define DEF_HTTPS	"https://"
#define DEF_USEAGEPAGE_CP3505	"/hp/device/this.LCDispatcher?nav=hp.DeviceStatus"
#define DEF_SUPPLIESPAGE_M525	"/hp/device/InternalPages/Index?id=SuppliesStatus"//耗材页
#define DEF_SUPPLIESPAGE_E77822	"/hp/device/DeviceStatus/Index"//耗材页
#define DEF_SUPPLIESPAGE_E72530	"/hp/device/DeviceStatus/Index"//耗材页
#define DEF_SUPPLIESPAGE_E72535	"/hp/device/DeviceStatus/Index"//耗材页


CSnmpMonitorHelperHP::CSnmpMonitorHelperHP(void)
{
	InitOID();
}

void CSnmpMonitorHelperHP::InitOID()
{
	
	m_szHpPrinterModelOID = DecryptOID(HpPrinterModelOID);
	m_szHPTonerLevelPercentOID_4345 = DecryptOID(HPTonerLevelPercentOID_4345); 
}

CSnmpMonitorHelperHP::~CSnmpMonitorHelperHP(void)
{
}
BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_M4345()
{
	int nIndex = 1;//GetOidEndNumber(cOidCurrent);
	if (m_oMarkerSuppliesMap.find(nIndex) == m_oMarkerSuppliesMap.end())
	{
		PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
		memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
		m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
		pEntry->prtMarkerSuppliesIndex = nIndex;
	}

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

			int nIndex = pEntry->prtMarkerSuppliesIndex;
			char cOidStr[128] = {0};
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			sprintf(cOidStr, "%s", m_szHPTonerLevelPercentOID_4345);	//RicohTonerLevelPercentOID
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_M1216()
{
	int nIndex = 1;//GetOidEndNumber(cOidCurrent);
	if (m_oMarkerSuppliesMap.find(nIndex) == m_oMarkerSuppliesMap.end())
	{
		PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
		memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
		m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
		pEntry->prtMarkerSuppliesIndex = nIndex;
	}

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

			int nIndex = pEntry->prtMarkerSuppliesIndex;
			char cOidStr[128] = {0};
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			sprintf(cOidStr, "%s", m_szHPTonerLevelPercentOID_4345);	//RicohTonerLevelPercentOID
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_M4650()
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

	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

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
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon)
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
BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_M525()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_SUPPLIESPAGE_M525);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMarkerSupplies_M525,0,begin,szUrl=%s",szUrl);
	CString szContent = GetMeterByUrl(szUrl);
	for (int nIndex = 1;nIndex <2 ;nIndex++)
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
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

			int nIndex = pEntry->prtMarkerSuppliesIndex;
			if (nIndex == 1)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Black Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(GetBlackInfo_M525(szContent).GetString());
			}
			
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

CString CSnmpMonitorHelperHP::GetBlackInfo_M525(CString& szContent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetBlackInfo_M525,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "Black Cartridge";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetBlackInfo_M525,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		szFlag = "<p class=\"data percentage\">";
		int nIndex1 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Right(szTmp.GetLength() - nIndex1 - szFlag.GetLength());
			szFlag = "</p>";
			int nIndex2 = szTmp.Find(szFlag);
			if (nIndex2 >= 0)
			{
				szTmp.Left(nIndex2);
			}
		}
	}
	szTmp.Replace("&nbsp","");
	szTmp.Replace(" ","");
	szTmp.Replace(";","");
	szTmp.Replace("%","");
	szTmp.Replace("*","");
	return szTmp;
}
BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_CP3505()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTP,szIp,DEF_USEAGEPAGE_CP3505);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMarkerSupplies_CP3505,0,begin,szUrl=%s",szUrl);
	CString szContent = GetMeterByUrl(szUrl);
	for (int nIndex = 1;nIndex <5 ;nIndex++)
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
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

			int nIndex = pEntry->prtMarkerSuppliesIndex;
			if (nIndex == 1)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Black Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(GetBlackInfo_CP3505(szContent).GetString());
			}
			else if (nIndex == 2)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Cyan Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(GetCyanInfo_CP3505(szContent).GetString());
			}
			else if (nIndex == 3)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Magenta Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(GetMagentaInfo_CP3505(szContent).GetString());
			}
			else if (nIndex == 4)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Yellow Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(GetYellowInfo_CP3505(szContent).GetString());
			}
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}
CString CSnmpMonitorHelperHP::GetBlackInfo_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetBlackInfo_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "Black Cartridge";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetBlackInfo_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		szFlag = "</span>";
		int nIndex1 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
		}
	}
	szTmp.Replace("&nbsp","");
	szTmp.Replace(";","");
	szTmp.Replace("%","");
	return szTmp;
}
CString CSnmpMonitorHelperHP::GetCyanInfo_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetCyanInfo_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "Cyan Cartridge";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetCyanInfo_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		szFlag = "</span>";
		int nIndex1 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
		}
	}
	szTmp.Replace("&nbsp","");
	szTmp.Replace(";","");
	szTmp.Replace("%","");
	return szTmp;
}
CString CSnmpMonitorHelperHP::GetMagentaInfo_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetMagentaInfo_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "Magenta Cartridge";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMagentaInfo_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		szFlag = "</span>";
		int nIndex1 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
		}
	}
	szTmp.Replace("&nbsp","");
	szTmp.Replace(";","");
	szTmp.Replace("%","");
	return szTmp;
}
CString CSnmpMonitorHelperHP::GetYellowInfo_CP3505(CString& szContent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetYellowInfo_CP3505,0");
	CString szTmp = "";
	CString szFlag;
	if (szContent.GetLength()<= 0)
	{
		return "";
	}
	szFlag = "Yellow Cartridge";
	int nIndex = szContent.Find(szFlag);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetYellowInfo_CP3505,1,nIndex=%d",nIndex);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex - szFlag.GetLength());
		szFlag = "</span>";
		int nIndex1 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
		}
	}
	szTmp.Replace("&nbsp","");
	szTmp.Replace(";","");
	szTmp.Replace("%","");
	return szTmp;
}
#if 0
BOOL CSnmpMonitorHelperHP::GetMarkerSupplies()	//重写获取耗材的接口
{
CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

// 	char *cOidBegin = CStringToChar(m_szRicohTonerColorOID); //RicohTonerColorOID
// 	char *cOidCurrent = cOidBegin;
// 	char pszValue[128] = {0};
// 	char pszOidNext[128] = {0};

	m_szHpPrintModel = GetHPModel();
	if(m_szHpPrintModel.Find("4345") >= 0)
	{
		return GetMarkerSupplies_M4345();		
	}
	else if(m_szHpPrintModel.Find("1216") >= 0)
	{
		return GetMarkerSupplies_M1216();
	}
	else if (m_szHpPrintModel.Find("4650") >= 0)
	{
		return GetMarkerSupplies_M4650();
	}
	else if (m_szHpPrintModel.Find("CP3505") >= 0)
	{
		return GetMarkerSupplies_CP3505();
	}
// 	else if (m_szHpPrintModel.Find("E77822") >= 0)
// 	{
// 		return GetMarkerSupplies_E77822();
// 	}
	else if (m_szHpPrintModel.Find("E72530") >= 0)
	{
		return GetMarkerSupplies_E72530();
	}
	else if (m_szHpPrintModel.Find("E72535") >= 0)//E72535跟E72530抄表页一样
	{
		return GetMarkerSupplies_E72530();
	}
	else
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

		CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

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
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon)
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
				int aa = pEntry->prtMarkerSuppliesMaxCapacity;
				int bb = pEntry->prtMarkerSuppliesLevel;
				theLog.Write("====aa=%d,bb=%d",aa,bb);

			}
		}

		return TRUE;
	}

	int nIndex = 1;//GetOidEndNumber(cOidCurrent);
	if (m_oMarkerSuppliesMap.find(nIndex) == m_oMarkerSuppliesMap.end())
	{
		PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
		memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
		m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
		pEntry->prtMarkerSuppliesIndex = nIndex;
	}

#if 0
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
#endif

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。
			
			int nIndex = pEntry->prtMarkerSuppliesIndex;
			char cOidStr[128] = {0};
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperHP::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			sprintf(cOidStr, "%s", m_szHPTonerLevelPercentOID_4345);	//RicohTonerLevelPercentOID
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
			//theLog.Write("====CSnmpMonitorHelperHP::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}
#endif


CString CSnmpMonitorHelperHP::GetHPModel()
{
	CString szModel = GetStrByOid(m_szHpPrinterModelOID);
	if (szModel.GetLength() <= 0)
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetHPModel,获取型号失败");
	}
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetHPModel,szModel=%s",szModel);
	return szModel;

}
int CSnmpMonitorHelperHP::DoubleToInt(double fValue)
{
	return ((int)(fValue+0.5));
}

CString CSnmpMonitorHelperHP::GetMeterByUrl(CString szUrl)
{
	CString szinfo="";
	CInternetSession *pInetSession = NULL;
	try
	{
		pInetSession = new CInternetSession; 
		CHttpFile *pHttpFile = NULL ;
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(szUrl) ;   //打开一个URL
		if (!pHttpFile)
		{
			theLog.Write("!!CSnmpMonitorHelperHP::HtmlToList,1,fail");
			return szinfo;
		}

		//theLog.Write("CSnmpPrinterMeterMonitorHP::HtmlToList,2,pHttpFile=0x%x", pHttpFile);
		//pHttpFile 确实是否要删除
		CString str ;

		while(pHttpFile->ReadString(str))   
		{
			szinfo += CCommonFun::UTF8ToMultiByte((const unsigned char*)str.GetString());
		}
		delete pHttpFile;
		pHttpFile = NULL;
		delete pInetSession;
		pInetSession  = NULL;
	}
	catch(...)
	{
		theLog.Write("!!CSnmpMonitorHelperHP catch exception.ERROR=%d", GetLastError());
		delete pInetSession;
		pInetSession  = NULL;
	}
	return szinfo;
}
CString CSnmpMonitorHelperHP::GetHttpsMeterByUrl(CString szUrl)
{
	DWORD dwFlags;  
	DWORD dwStatus = 0;  
	DWORD dwStatusLen = sizeof(dwStatus);  
	CString strLine;  
	DWORD m_dwServiceType;
	CString m_strServer;
	CString m_strObject_URI;
	INTERNET_PORT  m_nServerPort = 0;
	CString m_strHttpVersion;
	CString m_strHtml;

	AfxParseURL(szUrl, m_dwServiceType, m_strServer, m_strObject_URI, m_nServerPort);  
	CInternetSession *m_pSess = new CInternetSession; 
	CHttpFile *m_pHttpFile = NULL ;
	CHttpConnection *m_pHttpConn= NULL;

	try {  
		m_pHttpConn = m_pSess->GetHttpConnection(m_strServer, INTERNET_FLAG_SECURE, m_nServerPort,  
			NULL, NULL);  
		if(m_pHttpConn)
		{  
			m_pHttpFile = (CHttpFile*)m_pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, m_strObject_URI, NULL, 1,  
				NULL, m_strHttpVersion,  
				INTERNET_FLAG_SECURE |  
				INTERNET_FLAG_EXISTING_CONNECT |  
				INTERNET_FLAG_RELOAD |  
				INTERNET_FLAG_NO_CACHE_WRITE |  
				INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  
				INTERNET_FLAG_IGNORE_CERT_CN_INVALID  
				);  
			//get web server option  
			BOOL BRet = m_pHttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
			if(!BRet)
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorHP::GetHttpsMeterByUrl,11,QueryOption fail,err=%d", GetLastError());
			}
			//dwFlags |= SECURITY_FLAG_IGNORE_WRONG_USAGE;//SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
//			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;//SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
			dwFlags |= SECURITY_IGNORE_ERROR_MASK;	//SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			//set web server option  
			BRet = m_pHttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
			if(!BRet)
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorHP::SetOption,11,QueryOption fail,err=%d", GetLastError());
			}
			m_pHttpFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
			m_pHttpFile->AddRequestHeaders(_T("Accept: */*"));
			if(m_pHttpFile->SendRequest()) 
			{
				//get response status if success, return 200  
				int nRet = m_pHttpFile->QueryInfo(HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE,  
					&dwStatus, &dwStatusLen, 0); 
				//theLog.Write("CSnmpPrinterMeterMonitorHP::SetOption,13,QueryOption nRet=%d",nRet);
				while(m_pHttpFile->ReadString(strLine))
				{  
					m_strHtml += strLine + char(13) + char(10);  
				}
			} 
			else 
			{  
				theLog.Write("!!CSnmpPrinterMeterMonitorHP::SetOption,13,send request failed");
	
			}  
		} 
		else 
		{  
			theLog.Write("CSnmpPrinterMeterMonitorHP::SetOption,13,send request failed");
		}  
	} 
	catch(CInternetException *e) 
	{  
		e->ReportError();  
	}  
	m_pSess->Close(); 
	delete m_pSess;
	m_pSess = NULL;
	m_pHttpFile->Close();
	delete m_pHttpFile;
	m_pHttpFile = NULL;
	return m_strHtml;
}
BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_E77822()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_SUPPLIESPAGE_E77822);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMarkerSupplies_E77822,0,begin,szUrl=%s",szUrl);
	CString szContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMarkerSupplies_E77822,1");
	for (int nIndex = 1;nIndex <5 ;nIndex++)
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
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

			int nIndex = pEntry->prtMarkerSuppliesIndex;
			CString szBlackPercent;
			CString szCyanPercent;
			CString szMagentPercent;
			CString szYellowPercent;
			GetTonerInfo_E77822(szContent,szBlackPercent,szCyanPercent,szMagentPercent,szYellowPercent);
			if (nIndex == 1)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Black Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(szBlackPercent.GetString());
			}
			else if (nIndex == 2)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Cyan Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(szCyanPercent.GetString());
			}
			else if (nIndex == 3)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Magenta Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(szMagentPercent.GetString());
			}
			else if (nIndex == 4)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Yellow Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(szYellowPercent.GetString());
			}
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

void CSnmpMonitorHelperHP::GetTonerInfo_E77822(CString& szContent,CString &szBlackPercent,CString &szCyanPercent,CString &szMagentPercent,CString &szYellowPercent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetTonerInfo_E77822,1");
	CString szFlag = "<span id=\"SupplyPLR0\" class=\"plr\">";
	CString szTmp = "";
	int nIndex = szContent.Find(szFlag);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex -szFlag.GetLength());
		int nIndex1 = szTmp.Find("</span>");
		int nIndex2 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
			szTmp.Replace(" ","");
			szTmp.Replace("*","");
			szTmp.Replace("%","");
			szBlackPercent = szTmp;
		}
	}
	szFlag = "<span id=\"SupplyPLR1\" class=\"plr\">";
	szTmp = "";
	nIndex = szContent.Find(szFlag);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex -szFlag.GetLength());
		int nIndex1 = szTmp.Find("</span>");
		int nIndex2 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
			szTmp.Replace(" ","");
			szTmp.Replace("*","");
			szTmp.Replace("%","");
			szCyanPercent = szTmp;
		}
	}
	szFlag = "<span id=\"SupplyPLR2\" class=\"plr\">";
	szTmp = "";
	nIndex = szContent.Find(szFlag);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex -szFlag.GetLength());
		int nIndex1 = szTmp.Find("</span>");
		int nIndex2 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
			szTmp.Replace(" ","");
			szTmp.Replace("*","");
			szTmp.Replace("%","");
			szMagentPercent = szTmp;
		}
	}
	szFlag = "<span id=\"SupplyPLR3\" class=\"plr\">";
	szTmp = "";
	nIndex = szContent.Find(szFlag);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex -szFlag.GetLength());
		int nIndex1 = szTmp.Find("</span>");
		int nIndex2 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
			szTmp.Replace(" ","");
			szTmp.Replace("*","");
			szTmp.Replace("%","");
			szYellowPercent = szTmp;
		}
	}
}
//======
BOOL CSnmpMonitorHelperHP::GetMarkerSupplies_E72530()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
	CString szIp = GetPrtIP();
	CString szUrl;
	szUrl.Format("%s%s%s",DEF_HTTPS,szIp,DEF_SUPPLIESPAGE_E72530);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMarkerSupplies_E77822,0,begin,szUrl=%s",szUrl);
	CString szContent = GetHttpsMeterByUrl(szUrl);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::GetMarkerSupplies_E77822,1");
	for (int nIndex = 1;nIndex <2 ;nIndex++)
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
			pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::SupplyThatIsConsumed;
			pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;	//默认为墨盒信息
			pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
			pEntry->prtMarkerSuppliesMaxCapacity = 100;	//本函数获取的是粉盒的剩余百分比，所以默认最大为容量为100。

			int nIndex = pEntry->prtMarkerSuppliesIndex;
			CString szBlackPercent;

			GetTonerInfo_E72530(szContent,szBlackPercent);
			if (nIndex == 1)
			{
				sprintf((char*)pEntry->prtMarkerSuppliesDescription,"%s",CCommonFun::MultiByteToUTF8("Black Cartridge"));
				pEntry->prtMarkerSuppliesLevel =  atoi(szBlackPercent.GetString());
			}
			
			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorOID, nIndex);	//RicohTonerColorOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerDescOID, nIndex);	//RicohTonerDescOID
			//GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription);

			//sprintf(cOidStr, "%s.%d", m_szRicohTonerColorTypeOID, nIndex);	//RicohTonerColorTypeOID
			//GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,0,oid=%s",m_szPantumTonerLevelPercentOID);
			//theLog.Write("====CSnmpMonitorHelperPT::GetMarkerSupplies,test=%d",pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

void CSnmpMonitorHelperHP::GetTonerInfo_E72530(CString& szContent,CString &szBlackPercent)
{
	//theLog.Write("CSnmpMonitorHelperHP::GetTonerInfo_E77822,1");
	CString szFlag = "<span id=\"SupplyGauge0\" style=\"width:100%;\">";
	CString szTmp = "";
	int nIndex = szContent.Find(szFlag);
	if (nIndex >= 0)
	{
		szTmp = szContent.Right(szContent.GetLength() - nIndex -szFlag.GetLength());
		int nIndex1 = szTmp.Find("</span>");
		int nIndex2 = szTmp.Find(szFlag);
		if (nIndex1 >= 0)
		{
			szTmp = szTmp.Left(nIndex1);
			szTmp.Replace(" ","");
			szTmp.Replace("*","");
			szTmp.Replace("%","");
			szBlackPercent = szTmp;
		}
	}
	
}