#include "StdAfx.h"
#include "SnmpMonitorHelperSL.h"
#include "XeroxMIB_Def.h"
#include <algorithm>

CSnmpMonitorHelperSL::CSnmpMonitorHelperSL(void)
{
}

CSnmpMonitorHelperSL::~CSnmpMonitorHelperSL(void)
{
}

//����������дCSnmpMonitorHelper::GetMarkerSupplies
//ֻ���������ī����Ϣ
BOOL CSnmpMonitorHelperSL::GetMarkerSupplies()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
	CSnmpMonitorHelper::GetMarkerSupplies();
	if (m_oMarkerSuppliesMap.size() <= 0)
	{
		GetMarkerSuppliesByHtml();
	}

	return TRUE;
}

void CSnmpMonitorHelperSL::GetMarkerSuppliesByHtml()
{
	theLog.Write("CSnmpMonitorHelperSL::GetMarkerSuppliesByHtml");
	//��ȡʩ�ֳ�����Ϣ
	CString szRequest; szRequest.Format("http://%s/stsply.htm", m_szIP);
	CHttpClient2 client;
	string strResponse;
	int nRet = client.HttpGet(szRequest, "", strResponse, false);
	if (nRet == SUCCESS)
	{
		size_t nBeginPos = 0;
		size_t nEndPos = 0;
		//info=info.concat([['ī��Ͳ',[['��ɫī��(C)',0,32],['���ɫī��(M)',0,43],['��ɫī��(Y)',5,50],['��ɫī��(K)',0,92]],3]]);
		//info=info.concat([['�Ϸۺ�',0,2]]);
		//info=info.concat([['�й��',[['��ɫ�й��',0,69],['���ɫ�й��',0,69],['��ɫ�й��',0,69],['��ɫ�й��',0,34]],1]]);
		//info=info.concat([['��Ӱ�����',0,0]]);
		//info=info.concat([['��2ƫѹתӡ��',0,0]]);
		//info=info.concat([['תӡ��',0,0]]);
		string strBegin("info=info.concat(");
		string strEnd(");");
		string strTemp;
		while(nBeginPos != string::npos)
		{
			nBeginPos = strResponse.find(strBegin, nBeginPos);
			if (nBeginPos != string::npos)
			{
				nEndPos = strResponse.find(strEnd, nBeginPos);
				if (nEndPos != string::npos)
				{
					//strTemp = strResponse.substr(nBeginPos, nEndPos-nBeginPos+strEnd.size());//strTemp�ĸ�ʽΪ��info=info.concat([['תӡ��',0,0]]);
					//cout << strTemp << endl;
					strTemp = strResponse.substr(nBeginPos+strBegin.size(), nEndPos-nBeginPos-strBegin.size());//strTemp�ĸ�ʽΪ��[['תӡ��',0,0]]
					//cout << strTemp << endl;
					Parse(strTemp);
				}
				nBeginPos = nEndPos;
			}
		}
	}
	else
	{
		theLog.Write("!!CSnmpMonitorHelperSL::GetMarkerSuppliesByHtml,fail,szRequest=%s", szRequest);
	}
}

//[['ī��Ͳ',[['��ɫī��(C)',0,32],['���ɫī��(M)',0,43],['��ɫī��(Y)',5,50],['��ɫī��(K)',0,92]],3]]
//[['�Ϸۺ�',0,2]]
//[['�й��',[['��ɫ�й��',0,69],['���ɫ�й��',0,69],['��ɫ�й��',0,69],['��ɫ�й��',0,34]],1]]
//[['��Ӱ�����',0,0]]
//[['��2ƫѹתӡ��',0,0]]
//[['תӡ��',0,0]]
void CSnmpMonitorHelperSL::Parse(string& strData)
{
	ENUM_STRUCT_TYPE(XeroxMarkerSuppliesType) xeroxMarkerSuppliesType = 
		(ENUM_STRUCT_TYPE(XeroxMarkerSuppliesType))(strData[strData.size()-3] - '0');
#if 0
	return ParseEx(strData, xeroxMarkerSuppliesType);
#else
	if (xeroxMarkerSuppliesType == ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::Toner)
	{
		ParseEx(strData,  xeroxMarkerSuppliesType);
	}
	else if (xeroxMarkerSuppliesType == ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::OPC)
	{
		ParseEx(strData, xeroxMarkerSuppliesType);
	}
	else
	{
		//����������ʱ������
	}
#endif
}

//[['ī��Ͳ',[['��ɫī��(C)',0,32],['���ɫī��(M)',0,43],['��ɫī��(Y)',5,50],['��ɫī��(K)',0,92]],3]]
void CSnmpMonitorHelperSL::ParseEx(string& strData, int nMarkerSuppliesType)
{
	size_t currentPos = 0;
	size_t len = strData.size();
	bool bItemBegin = false;
	bool bReadFirstDigit = false;
	string strMarkerSuppliesName = "";
	int nMarkerSuppliesStatus = 0;
	int nMarkerSuppliesLevel = 0;
	while (currentPos < len)
	{
		char c = strData[currentPos];
		if (c == '\'')
		{
			size_t ItemBeginPos = ++currentPos;
			while(currentPos < len)
			{
				c = strData[currentPos++];
				if (c == '\'')
				{
					break;
				}
			}
			strMarkerSuppliesName = strData.substr(ItemBeginPos, currentPos-ItemBeginPos-1);
		}
		else if (c == ',')
		{
			size_t ItemBeginPos = ++currentPos;
			if (isdigit(strData[ItemBeginPos]))
			{
				while(currentPos < len)
				{
					c = strData[currentPos];
					if (c == ',' || c == ']')
					{
						break;
					}
					currentPos++;
				}
				string strTemp = strData.substr(ItemBeginPos, currentPos-ItemBeginPos);
				if (!bReadFirstDigit)
				{
					bReadFirstDigit = true;
					nMarkerSuppliesStatus = atoi(strTemp.c_str());
				}
				else
				{
					bReadFirstDigit = false;
					nMarkerSuppliesLevel = atoi(strTemp.c_str());

					//����һ����¼
					AddOneMarkerSupplies(strMarkerSuppliesName, nMarkerSuppliesStatus, nMarkerSuppliesLevel, nMarkerSuppliesType);
				}
			}
		}
		else
		{
			currentPos++;
		}
	}
}

void CSnmpMonitorHelperSL::AddOneMarkerSupplies(string& strMarkerSuppliesName, int nMarkerSuppliesStatus, int nMarkerSuppliesLevel, int nMarkerSuppliesType)
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

	ConvertColorDesc(strMarkerSuppliesName);
	//theLog.Write("CSnmpMonitorHelperSL::AddOneMarkerSupplies,strMarkerSuppliesName=%s,nMarkerSuppliesStatus=%d,nMarkerSuppliesLevel=%d,nMarkerSuppliesType=%d"
	//	, strMarkerSuppliesName.c_str(), nMarkerSuppliesStatus, nMarkerSuppliesLevel, nMarkerSuppliesType);
	//����һ��PrtMarkerSuppliesEntry���ݣ��������ϲ㴦���߼�һ�¡�
	PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
	memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
	int nIndex = m_oMarkerSuppliesMap.size()+1;
	m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
	pEntry->prtMarkerSuppliesIndex = nIndex;
	pEntry->prtMarkerSuppliesClass = ENUM_STRUCT_VALUE(PrtMarkerSuppliesClassTC)::Other;
	pEntry->prtMarkerSuppliesType = (ENUM_STRUCT_TYPE(PrtMarkerSuppliesTypeTC))ConvertMarkerSuppliesType(nMarkerSuppliesType);
	strcpy_s((char*)pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription), strMarkerSuppliesName.c_str());
	pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;	//�ٷֱȱ�ʶ����
	pEntry->prtMarkerSuppliesMaxCapacity = 100;	//���Ϊ100%
	pEntry->prtMarkerSuppliesLevel = ConvertMarkerSuppliesLevel(nMarkerSuppliesStatus, nMarkerSuppliesLevel, nMarkerSuppliesType);
}

void CSnmpMonitorHelperSL::ConvertColorDesc(string& strColorDesc)
{
	string strTemp = strColorDesc;
	std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);
	if (strTemp.find("��ɫ") != string::npos || strTemp.find("cyan") != string::npos)
	{
		strColorDesc = "cyan";
	}
	else if (strTemp.find("���ɫ") != string::npos || strTemp.find("magenta") != string::npos)
	{
		strColorDesc = "magenta";
	}
	else if (strTemp.find("��ɫ") != string::npos || strTemp.find("yellow") != string::npos)
	{
		strColorDesc = "yellow";
	}
	else if (strTemp.find("��ɫ") != string::npos || strTemp.find("black") != string::npos)
	{
		strColorDesc = "black";
	}
}

int CSnmpMonitorHelperSL::ConvertMarkerSuppliesType(int nType)
{
	ENUM_STRUCT_TYPE(XeroxMarkerSuppliesType) nMarkerSuppliesType = (ENUM_STRUCT_TYPE(XeroxMarkerSuppliesType))nType;
	switch (nMarkerSuppliesType)
	{
	case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::Other:
		return ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Other;
		break;
	case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::OPC:
		return ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC;
		break;
	case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::WasteToner:
		return ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::WasteToner;
		break;
	case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::Toner:
		return ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;
		break;
	default:
		return ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Other;
		break;
	}
}

int CSnmpMonitorHelperSL::ConvertMarkerSuppliesLevel(int nMarkerSuppliesStatus, int nMarkerSuppliesLevel, int nMarkerSuppliesType)
{
	switch (nMarkerSuppliesType)
	{
	//nMarkerSuppliesLevel�ڷۺг���������ʣ������ʾ�������ģ����Զ��δ���һ�£�������ʵ��ʣ����
	case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesType)::Toner:
		{
			switch (nMarkerSuppliesStatus)
			{
			case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesStatusToner)::Abnormal:
			case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesStatusToner)::BadInstalled:
			case ENUM_STRUCT_VALUE(XeroxMarkerSuppliesStatusToner)::Fault:
				nMarkerSuppliesLevel = 0;
				break;
			}
		}
		break;
	default:
		break;
	}
	return nMarkerSuppliesLevel;
}