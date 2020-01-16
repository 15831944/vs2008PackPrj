#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorSX.h"
#include "SamsungMIB_Def.h"

CSnmpPrinterMeterMonitorSX::CSnmpPrinterMeterMonitorSX(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorSX::~CSnmpPrinterMeterMonitorSX(void)
{
}

void CSnmpPrinterMeterMonitorSX::InitOID()
{
	m_szScmHrDevCountEntryOID = DecryptOID(ScmHrDevCountEntryOID);
}

//ֽ�Ų�����ȷָʾΪֽ��ʱ��Ĭ�ϼ���ΪA4
//��ӡ����ӡ�й��ڼ�����A3=2*A4
BOOL CSnmpPrinterMeterMonitorSX::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorSX::GetMeterInfo,��ȡ��ҳ��ʧ��");
		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
		//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,��ȡ��ҳ���ɹ���nValue=%d", nValue);
	}

	//1.����������ҵ����ӡ����ӡ��ɨ�衢����ȵȣ��ļ�������
	ScmHrDevCountEntryMap oScmHrDevCount;
	char *cOidBegin = CStringToChar(m_szScmHrDevCountEntryOID);	//ScmHrDevCountEntryOID
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
			if (oScmHrDevCount.find(nIndex) == oScmHrDevCount.end())
			{
				PScmHrDevCountEntry pEntry = new ScmHrDevCountEntry;
				memset(pEntry, 0x0, sizeof(ScmHrDevCountEntry));
				oScmHrDevCount.insert(pair<int,PScmHrDevCountEntry>(nIndex, pEntry));
				pEntry->scmHrDevCountIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	int nHrDevCount = oScmHrDevCount.size();
	theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,nHrDevCount=%d", nHrDevCount);

	//2.������ҵ������������ȡ�������ҵ������Ϣ������ҵ���͡�ֽ�͡�������Ԫ����˫�桢ɫ�ʡ�������
	ScmHrDevCountEntryMap::iterator it;
	for (it=oScmHrDevCount.begin(); it!=oScmHrDevCount.end(); it++)
	{
		PScmHrDevCountEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->scmHrDevCountIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.2.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountJobType=2
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountJobType);

			sprintf(cOidStr, "%s.3.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountMediaSize=3
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountMediaSize);

			sprintf(cOidStr, "%s.4.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountUnit=4
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountUnit);

			sprintf(cOidStr, "%s.5.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountDuplex=5
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountDuplex);

			sprintf(cOidStr, "%s.6.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountColor=6
			GetRequest(cOidStr, (int&)pEntry->scmHrDevCountColor);

			sprintf(cOidStr, "%s.7.1.%d", m_szScmHrDevCountEntryOID, nIndex);	//scmHrDevCountValue=7
			GetRequest(cOidStr, pEntry->scmHrDevCountValue);
		}
	}
	int nA4HBReport = 0;
	int nA4ColorReport = 0;
	int nA3HBReport = 0;
	int nA3ColorReport = 0;
	//3.ͳ�ƴ�ӡʵ�ʳ�ֽ������Ϣ��
	for (it=oScmHrDevCount.begin(); it!=oScmHrDevCount.end(); it++)
	{
		PScmHrDevCountEntry pEntry = it->second;
		if (pEntry)
		{
			if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::print)
			{
				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4PrintHBPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3PrintHBPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4PrintHBPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherPrintHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4PrintColorPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3PrintColorPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4PrintColorPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherPrintColorPage += pEntry->scmHrDevCountValue;
					}
				}
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::copy)
			{
				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4CopyHBPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3CopyHBPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4CopyHBPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherCopyHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4CopyColorPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3CopyColorPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4CopyColorPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherCopyColorPage += pEntry->scmHrDevCountValue;
					}
				}			
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::faxIn
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::faxOut)
			{
				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4FaxHBPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3FaxHBPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4FaxHBPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherFaxHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						m_oMeterInfo.nA4FaxColorPage += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						m_oMeterInfo.nA3FaxColorPage += pEntry->scmHrDevCountValue;
						m_oMeterInfo.nA4FaxColorPage += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherFaxColorPage += pEntry->scmHrDevCountValue;
					}
				}	
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalSend/*scan*/)
			{
				m_oMeterInfo.nScanTotalPage += pEntry->scmHrDevCountValue;
			}
			else if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::report)	//�����ӡ����
			{

				if (pEntry->scmHrDevCountColor == ENUM_STRUCT_VALUE(ScmHrDevCountColorTC)::monoColor)
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						nA4HBReport += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						nA3HBReport += pEntry->scmHrDevCountValue;
						nA4HBReport += pEntry->scmHrDevCountValue * 2;
					}
					else
					{
						m_oMeterInfo.nOtherPrintHBPage += pEntry->scmHrDevCountValue;
					}
				}
				else
				{
					if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a4)
					{
						nA4ColorReport += pEntry->scmHrDevCountValue;
					}
					else if (pEntry->scmHrDevCountMediaSize == ENUM_STRUCT_VALUE(ScmHrDevCountMediaSizeTC)::a3)
					{
						nA3ColorReport += pEntry->scmHrDevCountValue;
						nA4ColorReport += pEntry->scmHrDevCountValue * 2; 
					}
					else
					{
						m_oMeterInfo.nOtherPrintColorPage += pEntry->scmHrDevCountValue;
					}
				}
			}
			else /*if (pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalSend
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalSend
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::digitalRecieve
				|| pEntry->scmHrDevCountJobType == ENUM_STRUCT_VALUE(ScmHrDevCountJobTypeTC)::localStorage)*/
			{
				/*
				theLog.Write("##CSnmpPrinterMeterMonitorSX::GetMeterInfo,"
					"scmHrDevCountIndex=%d,scmHrDevCountJobType=%d,scmHrDevCountColor=%d,"
					"scmHrDevCountMediaSize=%d,scmHrDevCountDuplex=%d,scmHrDevCountValue=%d,scmHrDevCountUnit=%d"
					, pEntry->scmHrDevCountIndex, pEntry->scmHrDevCountJobType, pEntry->scmHrDevCountColor
					, pEntry->scmHrDevCountMediaSize, pEntry->scmHrDevCountDuplex, pEntry->scmHrDevCountValue
					, pEntry->scmHrDevCountUnit);
				*/
			}

			delete pEntry;	//ɾ���ڴ�
		}
	}

	oScmHrDevCount.clear();
	
	m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage + nA3HBReport;
	m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage + nA3ColorReport;
	m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage + nA4HBReport;
	m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage + nA4ColorReport;
	m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nOtherPrintHBPage + m_oMeterInfo.nOtherCopyHBPage;
	m_oMeterInfo.nOtherOpColorPage = m_oMeterInfo.nOtherPrintColorPage + m_oMeterInfo.nOtherCopyColorPage;
	//��������Ʒ�ƵĴ�ӡ����Ҫ�����������Ϊ��Щ��ӡ����������Ѿ�����ϸ�ˣ�û�б�Ҫ���μ�������ֵ�ļ�����
// 	int other = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3ColorTotalPage- m_oMeterInfo.nA4HBTotalPage - m_oMeterInfo.nA4ColorTotalPage - m_oMeterInfo.nOtherOpHBPage - m_oMeterInfo.nOtherOpColorPage;
// 	if(other > 0)
// 	{
// 		m_oMeterInfo.nOtherOpHBPage += other;
// 	}

	//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
	//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

	//theLog.Write("CSnmpPrinterMeterMonitorSX::GetMeterInfo,100,end");
	return (nHrDevCount>0) ? TRUE : FALSE;
}
