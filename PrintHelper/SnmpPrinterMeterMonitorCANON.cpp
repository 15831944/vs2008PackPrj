#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorCANON.h"
#include "CanonMIB_Def.h"
#include "PrinterInfoMIB_Def.h"

CSnmpPrinterMeterMonitorCANON::CSnmpPrinterMeterMonitorCANON(void)
{
	InitOID();
}

CSnmpPrinterMeterMonitorCANON::~CSnmpPrinterMeterMonitorCANON(void)
{
}

void CSnmpPrinterMeterMonitorCANON::InitOID()
{
	m_szCanonAllTotalPageOID = DecryptOID(CanonAllTotalPageOID);
	m_szCanonA3HBTotalPageOID = DecryptOID(CanonA3HBTotalPageOID);
	m_szCanonA4HBTotalPageOID = DecryptOID(CanonA4HBTotalPageOID);
	m_szCanonA3ColorTotalPageOID = DecryptOID(CanonA3ColorTotalPageOID);
	m_szCanonA4ColorTotalPageOID = DecryptOID(CanonA4ColorTotalPageOID);
	m_szCanonScanTotalPageOID = DecryptOID(CanonScanTotalPageOID);
	m_szCANONPrinterModelOID = DecryptOID(CanonModelOID);
	m_szCanonAllTotalPageOID_LBP6300dn = DecryptOID(CanonTotalPageOID_lbp6300n);
}

BOOL CSnmpPrinterMeterMonitorCANON::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//���ó�ʼ�������������ֹ���ݳ����ۼ������
	ResetPaperMeter();
	CString szModel = GetCANONModel();
	theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,9528,szModel=%s",szModel);
	if (szModel.Find("LBP6300n") >= 0 || szModel.Find("LBP6230dn") >= 0 
		|| szModel.Find("iR1024") >= 0 || szModel.Find("LBP252") >= 0
		|| szModel.Find("iB4100") >= 0)
	{
		return GetMeterInfo_LBP6300n();
	}
	else
	{
		int nValue = 0;

		//��ֽ��ҳ��
	// 	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//��ֽ��ҳ��="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	// 	{
	// 		theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,1.1,��ȡ��ֽ��ҳ��ʧ��");
	// 		return FALSE;	//����ʧ���ˣ�������ڵ�ľͲ�Ҫ���������ˡ�
	// 	}
	// 	else
	// 	{
	// 		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
	// 		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,1.2,��ȡ��ֽ��ҳ���ɹ���nValue=%d", nValue);
	// 	}

		if (!GetRequest(CStringToChar(m_szCanonAllTotalPageOID), nValue))	//����=".1.3.6.1.4.1.1602.1.11.1.4.1.4.101"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡ����ʧ��");
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue : m_oMeterInfo.nAllTotalPage;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡ�����ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA3HBTotalPageOID), nValue))	//A3�ڰ�=".1.3.6.1.4.1.1602.1.11.1.4.1.4.112"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡA3�ڰ�����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA3HBTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡA3�ڰ������ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA4HBTotalPageOID), nValue))	//A4�ڰ�=".1.3.6.1.4.1.1602.1.11.1.4.1.4.113"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡA4�ڰ�����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4HBTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡA4�ڰ������ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA3ColorTotalPageOID), nValue))	//A3��ɫ=".1.3.6.1.4.1.1602.1.11.1.4.1.4.122"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡA3��ɫ����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA3ColorTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡA3��ɫ�����ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA4ColorTotalPageOID), nValue))	//A4��ɫ=".1.3.6.1.4.1.1602.1.11.1.4.1.4.123"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡA4��ɫ����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4ColorTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡA4��ɫ�����ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonScanTotalPageOID), nValue))	//ɨ������=".1.3.6.1.4.1.1602.1.11.1.4.1.4.501"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡɨ������ʧ��");
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡɨ�������ɹ���nValue=%d", nValue);
		}

		int calcTotal = m_oMeterInfo.nA3HBTotalPage + m_oMeterInfo.nA4HBTotalPage + m_oMeterInfo.nA3ColorTotalPage + m_oMeterInfo.nA4ColorTotalPage;
		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,nA3HBTotalPage=%d,nA4HBTotalPage=%d,nA3ColorTotalPage=%d,nA4ColorTotalPage=%d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4ColorTotalPage);
		if (calcTotal <= 0)
		{
			GetMeterInfoByHtml();
			m_oMeterInfo.nA3HBTotalPage = m_oMeterInfo.nA3PrintHBPage + m_oMeterInfo.nA3CopyHBPage;
			m_oMeterInfo.nA4HBTotalPage = m_oMeterInfo.nA4PrintHBPage + m_oMeterInfo.nA4CopyHBPage;
			m_oMeterInfo.nA3ColorTotalPage = m_oMeterInfo.nA3PrintColorPage + m_oMeterInfo.nA3CopyColorPage;
			m_oMeterInfo.nA4ColorTotalPage = m_oMeterInfo.nA4PrintColorPage + m_oMeterInfo.nA4CopyColorPage;
			calcTotal = m_oMeterInfo.nA3HBTotalPage + m_oMeterInfo.nA4HBTotalPage + m_oMeterInfo.nA3ColorTotalPage + m_oMeterInfo.nA4ColorTotalPage;
		}
		if(m_oMeterInfo.nAllTotalPage > calcTotal)
		{
			m_oMeterInfo.nOtherOpHBPage = m_oMeterInfo.nAllTotalPage - calcTotal;
		}

		//A3��ɫ
		//A3�ڰ�
		//A4��ɫ
		//A4�ڰ�
		//������ɫ
		//�����ڰ�

		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		/*
		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.301", nValue))	//��ӡ����
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,��ȡ��ӡ����ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡ��ӡ�����ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.133", nValue))	//��ӡ�ڰ�ҳ��,�ڰ׻����Ǹ�ֵ���ʻ�����Ҫ��ȥ".1.3.6.1.4.1.1602.1.11.1.4.1.4.229"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,4,��ȡ��ӡ�ڰ�ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,5,��ȡ��ӡ�ڰ�ҳ���ɹ���nValue=%d", nValue);

			if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.229", nValue))	
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,6,��ȡ��ӡ�ڰ�ҳ����ʧ�ܣ��ڰ׻���ȡ��������");
			}
			else
			{
				m_oMeterInfo.nA4PrintHBPage -= (nValue>0) ? nValue : 0;
				theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,7,��ȡ��ӡ�ڰ�ҳ���ɹ���nValue=%d", m_oMeterInfo.nA4PrintHBPage);
			}
		}

		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.324", nValue))	//��ӡ��ɫҳ��
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,8,��ȡ��ӡ��ɫҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,9,��ȡ��ӡ��ɫҳ���ɹ���nValue=%d", nValue);
		}

		//===============================================================��ӡ,begin
		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.201", nValue))	//��ӡ��ҳ��
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,10,��ȡ��ӡ��ҳ��ʧ��");
		}
		else
		{
			m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,11,��ȡ��ӡ��ҳ���ɹ���nValue=%d", nValue);
		}

		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.222", nValue))	//��ӡ�ڰ�ҳ��
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,12,��ȡ��ӡ�ڰ�ҳ��ʧ��");
		}
		else
		{
			if(nValue <= m_oMeterInfo.nA4CopyTotalPage)
			{
				m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
				theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,13,��ȡ��ӡ�ڰ�ҳ���ɹ���nValue=%d", nValue);

				if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.401", nValue))	//��ӡ�ڰ׼���
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,14,��ȡ��ӡ�ڰ�ҳ��ʧ��");
				}
				else
				{
					if(0 <= nValue && nValue <= m_oMeterInfo.nA4CopyHBPage)
					{
						m_oMeterInfo.nA4CopyHBPage -= nValue;
						theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,15,��ȡ��ӡ�ڰ׼����ɹ���nA4CopyHBPage=%d", m_oMeterInfo.nA4CopyHBPage);
					}
					else
					{
						theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,16,��ȡ��ӡ�ڰ׼����쳣��nA4CopyHBPage=%d,nValue=%d"
							, m_oMeterInfo.nA4CopyHBPage, nValue);
					}
				}
			}
			else
			{
				m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage;
				theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,17,��ȡ��ӡ�ڰ�ҳ���ɹ�,���Ӧ���Ǹ��ڰ׻���ֻ��������nA4CopyHBPage=%d,nA4CopyTotalPage=%d"
					, m_oMeterInfo.nA4CopyHBPage, m_oMeterInfo.nA4CopyTotalPage);
			}
		}


		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.232", nValue))	//��ӡ��ɫҳ��
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,18,��ȡ��ӡ��ɫҳ��ʧ�ܣ������Ƿ�ڰ׻�");
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,19,��ȡ��ӡ��ɫҳ���ɹ���nValue=%d", nValue);
		}
		//===============================================================��ӡ,end
		*/
		return TRUE;
	}
}

BOOL CSnmpPrinterMeterMonitorCANON::GetMeterInfo_LBP6300n()
{

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szCanonAllTotalPageOID_LBP6300dn), nValue))	//����=".1.3.6.1.2.1.43.10.2.1.4.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo_LBP6300n,2,��ȡ����ʧ��");
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue : m_oMeterInfo.nAllTotalPage;
		m_oMeterInfo.nOtherOpHBPage = (nValue > 0) ? nValue : m_oMeterInfo.nAllTotalPage;
		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,��ȡ�����ɹ���nValue=%d", nValue);
	}


	return TRUE;
}

//ʹ��HTML��������ȡ,Ҫִ������html����
//��һ���ǵ�¼��֤��http://192.168.4.227
//�ڶ��β���ʵ�ʳ������ݣ�http://192.168.4.227/dcounter.cgi
void CSnmpPrinterMeterMonitorCANON::GetMeterInfoByHtml()
{
	theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfoByHtml");
	CString szRequestOne;szRequestOne.Format("http://%s", m_szIP);
	CString szRequestTwo;szRequestTwo.Format("http://%s/dcounter.cgi", m_szIP);
	CHttpClient2 client;
	string strResponse;
	int nRet = SUCCESS;
	nRet = client.HttpGet(szRequestOne, "", strResponse, false);
	if (nRet == SUCCESS)
	{
		nRet = client.HttpGet(szRequestTwo, "", strResponse, false);
		if (nRet == SUCCESS)
		{
			size_t nBeginPos = 0;
			size_t nEndPos = 0;
			//write_value("101", 67911);
			string strBegin("write_value(\"");
			string strMid("\", ");
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
						//strTemp = strResponse.substr(nBeginPos, nEndPos-nBeginPos+strEnd.size());	//strTemp�ĸ�ʽΪ��write_value("101", 67911);
						strTemp = strResponse.substr(nBeginPos+strBegin.size(), nEndPos-nBeginPos-strBegin.size());//strTemp�ĸ�ʽΪ��101", 67911
						int nType = atoi(strTemp.substr(0, strTemp.find(strMid)).c_str());
						int nValue = atoi(strTemp.substr(strTemp.find(strMid)+strMid.size()).c_str());
						//cout << nType << "=" << nValue << endl;
						write_value(nType, nValue);
					}
					nBeginPos = nEndPos;
				}
			}
		}
		else
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfoByHtml,2,fail,nRet=%d,szRequestTwo=%s", nRet, szRequestTwo);
		}
	}
	else
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfoByHtml,1,fail,nRet=%d,szRequestOne=%s", nRet, szRequestOne);
	}
}

/*
����HTML��������:
һ����ɫ��������iR C3200��
101 Total 1 67911 
108 Total (Black 1) 24653 
229 Copy (Full Color + Single Color/Large) 451 
230 Copy (Full Color + Single Color/Small) 2644 
321 Print (Full Color + Single Color /Large) 656 
322 Print (Full Color + Single Color/Small) 39507 

�����ڰ׻�������iR3530��
101 ����1 303 
103 ����(��) 25 
201 ��ӡ(����1) 269 
203 ��ӡ(��) 25 
*/
//write_value��ͨ��html����ʵ�ֵĳ������ݷ��������
//��ʱ���������������ͣ�ֻ�������������ͣ�
//������������󣬿����ʵ����Ÿ������͡�
void CSnmpPrinterMeterMonitorCANON::write_value(int nType, int nValue)
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	if( nType >= 0 && nType <= 99 )
	{
// 		switch(nType)
// 		{
// 			  case   2: "Զ�̸�ӡ(ȫ��ɫ1)" break;
// 			  case   3: "Զ�̸�ӡ(ȫ��ɫ2)" break;
// 			  case   4: "Զ�̸�ӡ(����ɫ1)" break;
// 			  case   5: "Զ�̸�ӡ(����ɫ2)" break;
// 			  case   6: "Զ�̸�ӡ(�ڰ�1)" break;
// 			  case   7: "Զ�̸�ӡ(�ڰ�2)" break;
// 			  case   8: "Զ�̸�ӡ(ȫ��ɫ/��)" break;
// 			  case   9: "Զ�̸�ӡ(ȫ��ɫ/С)" break;
// 			  case  10: "Զ�̸�ӡ(����ɫ/��)" break;
// 			  case  11: "Զ�̸�ӡ(����ɫ/С)" break;
// 			  case  12: "Զ�̸�ӡ(�ڰ�/��)" break;
// 			  case  13: "Զ�̸�ӡ(�ڰ�/С)" break;
// 			  case  14: "Զ�̸�ӡ(ȫ��ɫ+����ɫ/��)" break;
// 			  case  15: "Զ�̸�ӡ(ȫ��ɫ+����ɫ/С)" break;
// 			  case  16: "Զ�̸�ӡ(ȫ��ɫ+����ɫ2)" break;
// 			  case  17: "Զ�̸�ӡ(ȫ��ɫ+����ɫ1)" break;
// 			  case  18: "Զ�̸�ӡ(ȫ��ɫ/��/˫��)" break;
// 			  case  19: "Զ�̸�ӡ(ȫ��ɫ/С/˫��)" break;
// 			  case  20: "Զ�̸�ӡ(����ɫ/��/˫��)" break;
// 			  case  21: "Զ�̸�ӡ(����ɫ/С/˫��)" break;
// 			  case  22: "Զ�̸�ӡ(�ڰ�/��/˫��)" break;
// 			  case  23: "Զ�̸�ӡ(�ڰ�/С/˫��)" break;
// 
// 			default: break;
// 		}
	}
	else if( nType >= 100 && nType <= 199 )
	{
		switch(nType)
		{
			case 101: m_oMeterInfo.nAllTotalPage = nValue; break;	//"����1"
// 			case 102: "����2" break;
 			case 103: m_oMeterInfo.nA3HBTotalPage = nValue; break;	//"����(��)"
 			case 104: m_oMeterInfo.nA4HBTotalPage = nValue; break;	//"����(С)"
// 			case 105: "����(ȫ��ɫ1)" break;
// 			case 106: "����(ȫ��ɫ2)" break;

// 			case 108: "����(�ڰ�1)" break;
// 			case 109: "����(�ڰ�2)" break;
// 			case 110: "����(����ɫ/��)" break;
// 			case 111: "����(����ɫ/С)" break;
 			case 112: m_oMeterInfo.nA3HBTotalPage = nValue; break;	//"����(�ڰ�/��)"
 			case 113: m_oMeterInfo.nA4HBTotalPage = nValue; break;	//"����(�ڰ�/С)"
// 			case 114: "����1(˫��)" break;
// 			case 115: "����2(˫��)" break;
// 			case 116: "��(˫��)" break;
// 			case 117: "С(˫��)" break;
// 			case 118: "����(����ɫ1)" break;
// 			case 119: "����(����ɫ2)" break;
// 			case 120: "����(ȫ��ɫ/��)" break;
// 			case 121: "����(ȫ��ɫ/С)" break;
 			case 122: m_oMeterInfo.nA3ColorTotalPage = nValue; break;	//"����(ȫ��ɫ+����ɫ/��)"
 			case 123: m_oMeterInfo.nA4ColorTotalPage = nValue; break;	//"����(ȫ��ɫ+����ɫ/С)"
// 			case 124: "����(ȫ��ɫ+����ɫ2)" break;
// 			case 125: "����(ȫ��ɫ+����ɫ1)" break;
// 
// 			case 126: "����A1" break;
// 			case 127: "����A2" break;
// 			case 128: "����A(��)" break;
// 			case 129: "����A(С)" break;
// 			case 130: "����A(ȫ��ɫ1)" break;
// 			case 131: "����A(ȫ��ɫ2)" break;
// 			case 132: "����A(�ڰ�1)" break;
// 			case 133: "����A(�ڰ�2)" break;
// 			case 134: "����A(����ɫ/��)" break;
// 			case 135: "����A(����ɫ/С)" break;
// 			case 136: "����A(�ڰ�/��)" break;
// 			case 137: "����A(�ڰ�/С)" break;
// 			case 138: "����A1(˫��)" break;
// 			case 139: "����A2(˫��)" break;
// 			case 140: "��A(˫��)" break;
// 			case 141: "СA(˫��)" break;
// 			case 142: "����A(����ɫ1)" break;
// 			case 143: "����A(����ɫ2)" break;
// 			case 144: "����A(ȫ��ɫ/��)" break;
// 			case 145: "����A(ȫ��ɫ/С)" break;
// 			case 146: "����A(ȫ��ɫ+����ɫ/��)" break;
// 			case 147: "����A(ȫ��ɫ+����ɫ/С)" break;
// 			case 148: "����A(ȫ��ɫ+����ɫ2)" break;
// 			case 149: "����A(ȫ��ɫ+����ɫ1)" break;
// 			case 150: "����B1" break;
// 			case 151: "����B2" break;
// 			case 152: "����B(��)" break;
// 			case 153: "����B(С)" break;
// 			case 154: "����B(ȫ��ɫ1)" break;
// 			case 155: "����B(ȫ��ɫ2)" break;
// 			case 156: "����B(�ڰ�1)" break;
// 			case 157: "����B(�ڰ�2)" break;
// 			case 158: "����B(����ɫ/��)" break;
// 			case 159: "����B(����ɫ/С)" break;
// 			case 160: "����B(�ڰ�/��)" break;
// 			case 161: "����B(�ڰ�/С)" break;
// 			case 162: "����B1(˫��)" break;
// 			case 163: "����B2(˫��)" break;
// 			case 164: "��B(˫��)" break;
// 			case 165: "СB(˫��)" break;
// 			case 166: "����B(����ɫ1)" break;
// 			case 167: "����B(����ɫ2)" break;
// 			case 168: "����B(ȫ��ɫ/��)" break;
// 			case 169: "����B(ȫ��ɫ/С)" break;
// 			case 170: "����B(ȫ��ɫ+����ɫ/��)" break;
// 			case 171: "����B(ȫ��ɫ+����ɫ/С)" break;
// 			case 172: "����B(ȫ��ɫ+����ɫ2)" break;
// 			case 173: "����B(ȫ��ɫ+����ɫ1)" break;

			default: break;
		}
	}
	else if( nType >= 200 && nType <= 299 )
	{
		switch(nType)
		{
 			case 201: m_oMeterInfo.nOtherCopyTotalPage = nValue; break;	//"��ӡ(����1)"
// 			case 202: "��ӡ(����2)" break;
 			case 203: //"��ӡ(��)"
				{
					//һ���Ǻڰ׻�����û�и�ӡСֽ�ʹ�ӡ��������������ӡ����
					m_oMeterInfo.nA3CopyHBPage = nValue;
					if (m_oMeterInfo.nOtherCopyTotalPage > 0)
					{
						m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nOtherCopyTotalPage - m_oMeterInfo.nA3CopyHBPage;
						m_oMeterInfo.nA3PrintHBPage = m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3CopyHBPage;
						m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nOtherCopyTotalPage - m_oMeterInfo.nA3PrintHBPage;
						m_oMeterInfo.nOtherCopyTotalPage = 0;	//���������ֵ����Ϊ����ĸ��������Ѿ������������ֵ
					}
				} 
				break;	
 			case 204: m_oMeterInfo.nA4CopyHBPage = nValue; break;	//"��ӡ(С)"
// 			case 205: "��ӡA(����1)" break;
// 			case 206: "��ӡA(����2)" break;
// 			case 207: "��ӡA(��)" break;
// 			case 208: "��ӡA(С)" break;
// 			case 209: "���ظ�ӡ(����1)" break;
// 			case 210: "���ظ�ӡ(����2)" break;
// 			case 211: "���ظ�ӡ(��)" break;
// 			case 212: "���ظ�ӡ(С)" break;
// 			case 213: "Զ�̸�ӡ(����1)" break;
// 			case 214: "Զ�̸�ӡ(����2)" break;
// 			case 215: "Զ�̸�ӡ(��)" break;
// 			case 216: "Զ�̸�ӡ(С)" break;
// 			case 217: "��ӡ(ȫ��ɫ1)" break;
// 			case 218: "��ӡ(ȫ��ɫ2)" break;
// 			case 219: "��ӡ(����ɫ1)" break;
// 			case 220: "��ӡ(����ɫ2)" break;
// 			case 221: "��ӡ(�ڰ�1)" break;
// 			case 222: "��ӡ(�ڰ�2)" break;
// 			case 223: "��ӡ(ȫ��ɫ/��)" break;
// 			case 224: "��ӡ(ȫ��ɫ/С)" break;
// 			case 225: "��ӡ(����ɫ/��)" break;
// 			case 226: "��ӡ(����ɫ/С)" break;			
			case 227: m_oMeterInfo.nA3CopyHBPage = nValue; break;	//"��ӡ(�ڰ�/��)"
			case 228: m_oMeterInfo.nA4CopyHBPage = nValue; break;	//"��ӡ(�ڰ�/С)"
			case 229: m_oMeterInfo.nA3CopyColorPage = nValue; break;	//"��ӡ(ȫ��ɫ+����ɫ/��)"
			case 230: m_oMeterInfo.nA4CopyColorPage = nValue; break;	//"��ӡ(ȫ��ɫ+����ɫ/С)"
// 			case 231: "��ӡ(ȫ��ɫ+����ɫ/2)" break;
// 
// 			case 232: "��ӡ(ȫ��ɫ+����ɫ/1)" break;
// 			case 233: "��ӡ(ȫ��ɫ/��/˫��)" break;
// 			case 234: "��ӡ(ȫ��ɫ/С/˫��)" break;
// 			case 235: "��ӡ(����ɫ/��/˫��)" break;
// 			case 236: "��ӡ(����ɫ/С/˫��)" break;
// 			case 237: "��ӡ(�ڰ�/��/˫��)" break;
// 			case 238: "��ӡ(�ڰ�/С/˫��)" break;
// 			case 239: "��ӡ(˫ɫ1)" break;
// 			case 240: "��ӡ(˫ɫ2)" break;
// 			case 241: "��ӡ(˫ɫ/��)" break;
// 			case 242: "��ӡ(˫ɫ/С)" break;
// 			case 243: "��ӡ(˫ɫ/��/˫��)" break;
// 			case 244: "��ӡ(˫ɫ/С/˫��)" break;
// 			case 245: "��ӡA(ȫ��ɫ1)" break;
// 			case 246: "��ӡA(ȫ��ɫ2)" break;
// 			case 247: "��ӡA(����ɫ1)" break;
// 			case 248: "��ӡA(����ɫ2)" break;
// 			case 249: "��ӡA(�ڰ�1)" break;
// 			case 250: "��ӡA(�ڰ�2)" break;
// 			case 251: "��ӡA(ȫ��ɫ/��)" break;
// 			case 252: "��ӡA(ȫ��ɫ/С)" break;
// 			case 253: "��ӡA(����ɫ/��)" break;
// 			case 254: "��ӡA(����ɫ/С)" break;
// 			case 255: "��ӡA(�ڰ�/��)" break;
// 			case 256: "��ӡA(�ڰ�/С)" break;
// 			case 257: "��ӡA(ȫ��ɫ+����ɫ/��)" break;
// 			case 258: "��ӡA(ȫ��ɫ+����ɫ/С)" break;
// 			case 259: "��ӡA(ȫ��ɫ+����ɫ2)" break;
// 			case 260: "��ӡA(ȫ��ɫ+����ɫ1)" break;
// 			case 261: "��ӡA(ȫ��ɫ/��/˫��)" break;
// 			case 262: "��ӡA(ȫ��ɫ/С/˫��)" break;
// 			case 263: "��ӡA(����ɫ/��/˫��)" break;
// 			case 264: "��ӡA(����ɫ/С/˫��)" break;
// 			case 265: "��ӡA(�ڰ�/��/˫��)" break;
// 			case 266: "��ӡA(�ڰ�/С/˫��)" break;
// 
// 			case 273: "���ظ�ӡ(ȫ��ɫ1)" break;
// 			case 274: "���ظ�ӡ(ȫ��ɫ2)" break;
// 			case 275: "���ظ�ӡ(����ɫ1)" break;
// 			case 276: "���ظ�ӡ(����ɫ2)" break;
// 			case 277: "���ظ�ӡ(�ڰ�1)" break;
// 			case 278: "���ظ�ӡ(�ڰ�2)" break;
// 			case 279: "���ظ�ӡ(ȫ��ɫ/��)" break;
// 			case 280: "���ظ�ӡ(ȫ��ɫ/С)" break;
// 			case 281: "���ظ�ӡ(����ɫ/��)" break;
// 			case 282: "���ظ�ӡ(����ɫ/С)" break;
// 			case 283: "���ظ�ӡ(�ڰ�/��)" break;
// 			case 284: "���ظ�ӡ(�ڰ�/С)" break;
// 			case 285: "���ظ�ӡ(ȫ��ɫ+����ɫ/��)" break;
// 			case 286: "���ظ�ӡ(ȫ��ɫ+����ɫ/С)" break;
// 			case 287: "���ظ�ӡ(ȫ��ɫ+����ɫ2)" break;
// 			case 288: "���ظ�ӡ(ȫ��ɫ+����ɫ1)" break;
// 			case 289: "���ظ�ӡ(ȫ��ɫ/��/˫��)" break;
// 			case 290: "���ظ�ӡ(ȫ��ɫ/С/˫��)" break;
// 			case 291: "���ظ�ӡ(����ɫ/��/˫��)" break;
// 			case 292: "���ظ�ӡ(����ɫ/С/˫��)" break;
// 			case 293: "���ظ�ӡ(�ڰ�/��/˫��)" break;
// 			case 294: "���ظ�ӡ(�ڰ�/С/˫��)" break;

			default: break;
		}
	}
	else if( nType >= 300 && nType <= 399 )
	{
		switch(nType)
		{
// 			case 301: "��ӡ(����1)" break;
// 			case 302: "��ӡ(����2)" break;
// 			case 303: "��ӡ(��)" break;
// 			case 304: "��ӡ(С)" break;
// 			case 305: "��ӡA(����1)" break;
// 			case 306: "��ӡA(����2)" break;
// 			case 307: "��ӡA(��)" break;
// 			case 308: "��ӡA(С)" break;
// 			case 309: "��ӡ(ȫ��ɫ1)" break;
// 			case 310: "��ӡ(ȫ��ɫ2)" break;
// 			case 311: "��ӡ(����ɫ1)" break;
// 			case 312: "��ӡ(����ɫ2)" break;
// 			case 313: "��ӡ(�ڰ�1)" break;
// 			case 314: "��ӡ(�ڰ�2)" break;
// 			case 315: "��ӡ(ȫ��ɫ/��)" break;
// 			case 316: "��ӡ(ȫ��ɫ/С)" break;
// 			case 317: "��ӡ(����ɫ/��)" break;
// 			case 318: "��ӡ(����ɫ/С)" break;
			case 319: m_oMeterInfo.nA3PrintHBPage = nValue; break;	//"��ӡ(�ڰ�/��)"
			case 320: m_oMeterInfo.nA4PrintHBPage = nValue; break;	//"��ӡ(�ڰ�/С)"
			case 321: m_oMeterInfo.nA3PrintColorPage = nValue; break;	//"��ӡ(ȫ��ɫ+����ɫ/��)"
			case 322: m_oMeterInfo.nA4PrintColorPage = nValue; break;	//"��ӡ(ȫ��ɫ+����ɫ/С)"
// 			case 323: "��ӡ(ȫ��ɫ+����ɫ/2)" break;
// 			case 324: "��ӡ(ȫ��ɫ+����ɫ/1)" break;
// 			case 325: "��ӡ(ȫ��ɫ/��/˫��)" break;
// 			case 326: "��ӡ(ȫ��ɫ/С/˫��)" break;
// 			case 327: "��ӡ(����ɫ/��/˫��)" break;
// 			case 328: "��ӡ(����ɫ/С/˫��)" break;
// 			case 329: "��ӡ(�ڰ�/��/˫��)" break;
// 			case 330: "��ӡ(�ڰ�/С/˫��)" break;
// 			case 331: "��ӡ�����������ӡ(����1)" break;
// 			case 332: "��ӡ�����������ӡ(����2)" break;
// 			case 333: "��ӡ�����������ӡ(��)" break;
// 			case 334: "��ӡ�����������ӡ(С)" break;
// 			case 335: "��ӡ�����������ӡ(ȫ��ɫ1)" break;
// 			case 336: "��ӡ�����������ӡ(ȫ��ɫ2)" break;
// 
// 			case 339: "��ӡ�����������ӡ(�ڰ�1)" break;
// 			case 340: "��ӡ�����������ӡ(�ڰ�2)" break;
// 			case 341: "��ӡ�����������ӡ(ȫ��ɫ/��)" break;
// 			case 342: "��ӡ�����������ӡ(ȫ��ɫ/С)" break;
// 
// 			case 345: "��ӡ�����������ӡ(�ڰ�/��)" break;
// 			case 346: "��ӡ�����������ӡ(�ڰ�/С)" break;
// 
// 			case 351: "��ӡ�����������ӡ(ȫ��ɫ/��/˫��)" break;
// 			case 352: "��ӡ�����������ӡ(ȫ��ɫ/С/˫��)" break;
// 
// 			case 355: "��ӡ�����������ӡ(�ڰ�/��/˫��)" break;
// 			case 356: "��ӡ�����������ӡ(�ڰ�/С/˫��)" break;

			default: break;
		}
	}
// 	else if( nType >= 400 && nType <= 499 )
// 	{
// 		switch(nType)
// 		{
// 			  case 401: "��ӡ+��ӡ(ȫ��ɫ/��)" break;
// 			  case 402: "��ӡ+��ӡ(ȫ��ɫ/С)" break;
// 			  case 403: "��ӡ+��ӡ(�ڰ�/��)" break;
// 			  case 404: "��ӡ+��ӡ(�ڰ�/С)" break;
// 			  case 405: "��ӡ+��ӡ(�ڰ�2)" break;
// 			  case 406: "��ӡ+��ӡ(�ڰ�1)" break;
// 			  case 407: "��ӡ+��ӡ(ȫ��ɫ+����ɫ/��)" break;
// 			  case 408: "��ӡ+��ӡ(ȫ��ɫ+����ɫ/С)" break;
// 			  case 409: "��ӡ+��ӡ(ȫ��ɫ+����ɫ/2)" break;
// 			  case 410: "��ӡ+��ӡ(ȫ��ɫ+����ɫ/1)" break;
// 			  case 411: "��ӡ+��ӡ(��)" break;
// 			  case 412: "��ӡ+��ӡ(С)" break;
// 			  case 413: "��ӡ+��ӡ(2)" break;
// 			  case 414: "��ӡ+��ӡ(1)" break;
// 			  case 415: "��ӡ+��ӡ(����ɫ/��)" break;
// 			  case 416: "��ӡ+��ӡ(����ɫ/С)" break;
// 			  case 417: "��ӡ+��ӡ(ȫ��ɫ/��/˫��)" break;
// 			  case 418: "��ӡ+��ӡ(ȫ��ɫ/С/˫��)" break;
// 			  case 419: "��ӡ+��ӡ(����ɫ/��/˫��)" break;
// 			  case 420: "��ӡ+��ӡ(����ɫ/С/˫��)" break;
// 			  case 421: "��ӡ+��ӡ(�ڰ�/��/˫��)" break;
// 			  case 422: "��ӡ+��ӡ(�ڰ�/С/˫��)" break;
// 
// 			default: break;
// 		}
// 	}
	else if( nType >= 500 && nType <= 599 )
	{
		switch(nType)
		{
			case 501: m_oMeterInfo.nScanTotalPage = nValue; break;	//"ɨ��(����1)"
// 			case 502: "ɨ��(����2)" break;
// 			case 503: "ɨ��(��)" break;
// 			case 504: "ɨ��(С)" break;
// 			case 505: "�ڰ�ɨ��(����1)" break;
// 			case 506: "�ڰ�ɨ��(����2)" break;
			case 507: m_oMeterInfo.nScanBigHBPage = nValue; break;	//"�ڰ�ɨ��(��)"
			case 508: m_oMeterInfo.nScanSmallHBPage = nValue; break;	//"�ڰ�ɨ��(С)"
// 			case 509: "��ɫɨ��(����1)" break;
// 			case 510: "��ɫɨ��(����2)" break;
			case 511: m_oMeterInfo.nScanBigColorPage = nValue; break;	//"��ɫɨ��(��)"
			case 512: m_oMeterInfo.nScanSmallColorPage = nValue; break;	//"��ɫɨ��(С)"
// 			case 513: "��ӡɨ��(��)" break;
// 			case 514: "��ӡɨ��(С)" break;
// 			case 515: "��ӡɨ��(����)" break;

			default: break;
		}
	}
// 	else if( nType >= 600 && nType <= 699 )
// 	{
// 		switch(nType)
// 		{
// 			  case 601: "�ռ����ӡ(����1)" break;
// 			  case 602: "�ռ����ӡ(����2)" break;
// 			  case 603: "�ռ����ӡ(��)" break;
// 			  case 604: "�ռ����ӡ(С)" break;
// 			  case 605: "�ռ����ӡ(ȫ��ɫ1)" break;
// 			  case 606: "�ռ����ӡ(ȫ��ɫ2)" break;
// 			  case 607: "�ռ����ӡ(����ɫ1)" break;
// 			  case 608: "�ռ����ӡ(����ɫ2)" break;
// 			  case 609: "�ռ����ӡ(�ڰ�1)" break;
// 			  case 610: "�ռ����ӡ(�ڰ�2)" break;
// 			  case 611: "�ռ����ӡ(ȫ��ɫ/��)" break;
// 			  case 612: "�ռ����ӡ(ȫ��ɫ/С)" break;
// 			  case 613: "�ռ����ӡ(����ɫ/��)" break;
// 			  case 614: "�ռ����ӡ(����ɫ/С)" break;
// 			  case 615: "�ռ����ӡ(�ڰ�/��)" break;
// 			  case 616: "�ռ����ӡ(�ڰ�/С)" break;
// 			  case 617: "�ռ����ӡ(ȫ��ɫ+����ɫ/��)" break;
// 			  case 618: "�ռ����ӡ(ȫ��ɫ+����ɫ/С)" break;
// 			  case 619: "�ռ����ӡ(ȫ��ɫ+����ɫ2)" break;
// 			  case 620: "�ռ����ӡ(ȫ��ɫ+����ɫ1)" break;
// 			  case 621: "�ռ����ӡ(ȫ��ɫ/��/˫��)" break;
// 			  case 622: "�ռ����ӡ(ȫ��ɫ/С/˫��)" break;
// 			  case 623: "�ռ����ӡ(����ɫ/��/˫��)" break;
// 			  case 624: "�ռ����ӡ(����ɫ/С/˫��)" break;
// 			  case 625: "�ռ����ӡ(�ڰ�/��/˫��)" break;
// 			  case 626: "�ռ����ӡ(�ڰ�/С/˫��)" break;
// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 700 && nType <= 799 )
// 	{
// 		switch(nType)
// 		{
// 			  case 701: "���մ�ӡ(����1)" break;
// 			  case 702: "���մ�ӡ(����2)" break;
// 			  case 703: "���մ�ӡ(��)" break;
// 			  case 704: "���մ�ӡ(С)" break;
// 			  case 705: "���մ�ӡ(ȫ��ɫ1)" break;
// 			  case 706: "���մ�ӡ(ȫ��ɫ2)" break;
// 			  case 707: "���մ�ӡ(�Ҷ�1)" break;
// 			  case 708: "���մ�ӡ(�Ҷ�2)" break;
// 			  case 709: "���մ�ӡ(�ڰ�1)" break;
// 			  case 710: "���մ�ӡ(�ڰ�2)" break;
// 			  case 711: "���մ�ӡ(ȫ��ɫ/��)" break;
// 			  case 712: "���մ�ӡ(ȫ��ɫ/С)" break;
// 			  case 713: "���մ�ӡ(�Ҷ�/��)" break;
// 			  case 714: "���մ�ӡ(�Ҷ�/С)" break;
// 			  case 715: "���մ�ӡ(�ڰ�/��)" break;
// 			  case 716: "���մ�ӡ(�ڰ�/С)" break;
// 			  case 717: "���մ�ӡ(ȫ��ɫ+�Ҷ�/��)" break;
// 			  case 718: "���մ�ӡ(ȫ��ɫ+�Ҷ�/С)" break;
// 			  case 719: "���մ�ӡ(ȫ��ɫ+�Ҷ�2)" break;
// 			  case 720: "���մ�ӡ(ȫ��ɫ+�Ҷ�1)" break;
// 			  case 721: "���մ�ӡ(ȫ��ɫ/��/˫��)" break;
// 			  case 722: "���մ�ӡ(ȫ��ɫ/С/˫��)" break;
// 			  case 723: "���մ�ӡ(�Ҷ�/��/˫��)" break;
// 			  case 724: "���մ�ӡ(�Ҷ�/С/˫��)" break;
// 			  case 725: "���մ�ӡ(�ڰ�/��/˫��)" break;
// 			  case 726: "���մ�ӡ(�ڰ�/С/˫��)" break;

// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 800 && nType <= 899 )
// 	{
// 		switch(nType)
// 		{
// 			  case 801: "�����ӡ(����1)" break;
// 			  case 802: "�����ӡ(����2)" break;
// 			  case 803: "�����ӡ(��)" break;
// 			  case 804: "�����ӡ(С)" break;
// 			  case 805: "�����ӡ(ȫ��ɫ1)" break;
// 			  case 806: "�����ӡ(ȫ��ɫ2)" break;
// 			  case 807: "�����ӡ(�Ҷ�1)" break;
// 			  case 808: "�����ӡ(�Ҷ�2)" break;
// 			  case 809: "�����ӡ(�ڰ�1)" break;
// 			  case 810: "�����ӡ(�ڰ�2)" break;
// 			  case 811: "�����ӡ(ȫ��ɫ/��)" break;
// 			  case 812: "�����ӡ(ȫ��ɫ/С)" break;
// 			  case 813: "�����ӡ(�Ҷ�/��)" break;
// 			  case 814: "�����ӡ(�Ҷ�/С)" break;
// 			  case 815: "�����ӡ(�ڰ�/��)" break;
// 			  case 816: "�����ӡ(�ڰ�/С)" break;
// 			  case 817: "�����ӡ(ȫ��ɫ+�Ҷ�/��)" break;
// 			  case 818: "�����ӡ(ȫ��ɫ+�Ҷ�/С)" break;
// 			  case 819: "�����ӡ(ȫ��ɫ+�Ҷ�2)" break;
// 			  case 820: "�����ӡ(ȫ��ɫ+�Ҷ�1)" break;
// 			  case 821: "�����ӡ(ȫ��ɫ/��/˫��)" break;
// 			  case 822: "�����ӡ(ȫ��ɫ/С/˫��)" break;
// 			  case 823: "�����ӡ(�Ҷ�/��/˫��)" break;
// 			  case 824: "�����ӡ(�Ҷ�/С/˫��)" break;
// 			  case 825: "�����ӡ(�ڰ�/��/˫��)" break;
// 			  case 826: "�����ӡ(�ڰ�/С/˫��)" break;
// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 900 && nType <= 999 )
// 	{
// 		switch(nType)
// 		{
// 			  case 901: "��ӡɨ������1(��ɫ)" break;
// 			  case 902: "��ӡɨ������1(�ڰ�)" break;
// 			  case 903: "��ӡɨ������2(��ɫ)" break;
// 			  case 904: "��ӡɨ������2(�ڰ�)" break;
// 			  case 905: "��ӡɨ������3(��ɫ)" break;
// 			  case 906: "��ӡɨ������3(�ڰ�)" break;
// 			  case 907: "��ӡɨ������4(��ɫ)" break;
// 			  case 908: "��ӡɨ������4(�ڰ�)" break;
// 			  case 909: "���ظ�ӡɨ��(��ɫ)" break;
// 			  case 910: "���ظ�ӡɨ��(�ڰ�)" break;
// 			  case 911: "Զ�̸�ӡɨ��(��ɫ)" break;
// 			  case 912: "Զ�̸�ӡɨ��(�ڰ�)" break;
// 			  case 913: "����ɨ������1(��ɫ)" break;
// 			  case 914: "����ɨ������1(�ڰ�)" break;
// 			  case 915: "����ɨ������2(��ɫ)" break;
// 			  case 916: "����ɨ������2(�ڰ�)" break;
// 			  case 917: "����ɨ������3(��ɫ)" break;
// 			  case 918: "����ɨ������3(�ڰ�)" break;
// 			  case 919: "����ɨ������4(��ɫ)" break;
// 			  case 920: "����ɨ������4(�ڰ�)" break;
// 			  case 921: "����ɨ������5(��ɫ)" break;
// 			  case 922: "����ɨ������5(�ڰ�)" break;
// 
// 			  case 929: "����ɨ������6(��ɫ)" break;
// 			  case 930: "����ɨ������6(�ڰ�)" break;
// 			  case 931: "����ɨ������7(��ɫ)" break;
// 			  case 932: "����ɨ������7(�ڰ�)" break;
// 			  case 933: "����ɨ������8(��ɫ)" break;
// 			  case 934: "����ɨ������8(�ڰ�)" break;
// 			  case 935: "ͨ�÷���ɨ������(��ɫ)" break;
// 			  case 936: "ͨ�÷���ɨ������(�ڰ�)" break;
// 			  case 937: "�ռ���ɨ��(��ɫ)" break;
// 			  case 938: "�ռ���ɨ��(�ڰ�)" break;
// 			  case 939: "Զ��ɨ��(��ɫ)" break;
// 			  case 940: "Զ��ɨ��(�ڰ�)" break;
// 			  case 941: "����ɨ��/����(��ɫ)" break;
// 			  case 942: "����ɨ��/����(�ڰ�)" break;
// 			  case 943: "����ɨ��/����������(��ɫ)" break;
// 			  case 944: "����ɨ��/����������(�ڰ�)" break;
// 			  case 945: "����ɨ��/�����ʼ�(��ɫ)" break;
// 			  case 946: "����ɨ��/�����ʼ�(�ڰ�)" break;
// 			  case 947: "����ɨ��/FTP(��ɫ)" break;
// 			  case 948: "����ɨ��/FTP(�ڰ�)" break;
// 			  case 949: "����ɨ��/SMB(��ɫ)" break;
// 			  case 950: "����ɨ��/SMB(�ڰ�)" break;
// 			  case 951: "����ɨ��/IPX(��ɫ)" break;
// 			  case 952: "����ɨ��/IPX(�ڰ�)" break;
// 			  case 953: "����ɨ��/���ݿ�(��ɫ)" break;
// 			  case 954: "����ɨ��/���ݿ�(�ڰ�)" break;
// 			  case 955: "����ɨ��/���ش�ӡ(��ɫ)" break;
// 			  case 956: "����ɨ��/���ش�ӡ(�ڰ�)" break;
// 			  case 957: "����ɨ��/�ռ���(��ɫ)" break;
// 			  case 958: "����ɨ��/�ռ���(�ڰ�)" break;
// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 1000 && nType <= 1999 )
// 	{
// 		switch(nType)
// 		{
// 			case 1000: "����1" break;
// 			case 1001: "����(С)" break;
// 			case 1002: "����(��)" break;
// 			case 1003: "����(�ڰ�1)" break;
// 			case 1004: "����(�ڰ�/С)" break;
// 			case 1005: "����(�ڰ�/��)" break;
// 			case 1006: "����(ȫ��ɫ1)" break;
// 			case 1007: "����(ȫ��ɫ/С)" break;
// 			case 1008: "����(ȫ��ɫ/��)" break;
// 			case 1009: "����(����ɫ1)" break;
// 			case 1010: "����(����ɫ/С)" break;
// 			case 1011: "����(����ɫ/��)" break;
// 			default: break;
// 		}
// 	  }
// 	else if( nType >= 2000 && nType <= 2999 )
// 	{
// 		switch(nType)
// 		{
// 			case 2000: "ɨ��(����1)" break;
// 			case 2001: "ɨ��(С)" break;
// 			case 2002: "ɨ��(��)" break;
// 			case 2003: "�ڰ�ɨ��(����1)" break;
// 			case 2004: "�ڰ�ɨ��(С)" break;
// 			case 2005: "�ڰ�ɨ��(��)" break;
// 			case 2006: "��ɫɨ��(����1)" break;
// 			case 2007: "��ɫɨ��(С)" break;
// 			case 2008: "��ɫɨ��(��)" break;
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 3000 && nType <= 3999 )
// 	{
// 		switch(nType)
// 		{
// 			case 3000: "��ɫɨ��1" break;
// 			case 3001: "��ɫɨ��2" break;
// 			case 3002: "��ɫɨ��3" break;
// 			case 3003: "��ɫɨ��4" break;
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 4000 && nType <= 4999 )
// 	{
// 		switch(nType)
// 		{
// 			case 4000: "�ڰ�ɨ��1" break;
// 			case 4001: "�ڰ�ɨ��2" break;
// 			case 4002: "�ڰ�ɨ��3" break;
// 			case 4003: "�ڰ�ɨ��4" break;
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 5000 && nType <= 5999 )
// 	{
// 		switch(nType)
// 		{
// 			case 5000: "����1" break;
// 			case 5001: "����2" break;
// 			case 5002: "����3" break;
// 			case 5003: "����4" break;
// 			case 5004: "����5" break;
// 			case 5005: "����6" break;
// 			case 5006: "����7" break;
// 			case 5007: "����8" break;
// 			case 5008: "����9" break;
// 			case 5009: "����10" break;
// 			case 5010: "����11" break;
// 			case 5011: "����12" break;
// 			default: break;
// 		}
// 	}
}
CString	CSnmpPrinterMeterMonitorCANON::GetCANONModel()
{
	CString szModel = "";
	unsigned char tmp[200];
	if (!GetRequest(CStringToChar(m_szCANONPrinterModelOID),tmp,sizeof(tmp)))
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::CSnmpPrinterMeterMonitorCANON,��ȡ�ͺ�ʧ��");
	}
	szModel.Format("%s",tmp);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::CSnmpPrinterMeterMonitorCANON,szModel=%s",szModel);
	return szModel;
	
}
