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

	//重置初始化抄表计数，防止数据出现累加情况。
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

		//出纸总页数
	// 	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//出纸总页数="printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1"
	// 	{
	// 		theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,1.1,获取出纸总页数失败");
	// 		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	// 	}
	// 	else
	// 	{
	// 		m_oMeterInfo.nAllTotalPage = (nValue>0) ? nValue : 0;
	// 		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,1.2,获取出纸总页数成功，nValue=%d", nValue);
	// 	}

		if (!GetRequest(CStringToChar(m_szCanonAllTotalPageOID), nValue))	//总数=".1.3.6.1.4.1.1602.1.11.1.4.1.4.101"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取总数失败");
		}
		else
		{
			m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue : m_oMeterInfo.nAllTotalPage;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA3HBTotalPageOID), nValue))	//A3黑白=".1.3.6.1.4.1.1602.1.11.1.4.1.4.112"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取A3黑白总数失败");
		}
		else
		{
			m_oMeterInfo.nA3HBTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取A3黑白总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA4HBTotalPageOID), nValue))	//A4黑白=".1.3.6.1.4.1.1602.1.11.1.4.1.4.113"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取A4黑白总数失败");
		}
		else
		{
			m_oMeterInfo.nA4HBTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取A4黑白总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA3ColorTotalPageOID), nValue))	//A3彩色=".1.3.6.1.4.1.1602.1.11.1.4.1.4.122"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取A3彩色总数失败");
		}
		else
		{
			m_oMeterInfo.nA3ColorTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取A3彩色总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonA4ColorTotalPageOID), nValue))	//A4彩色=".1.3.6.1.4.1.1602.1.11.1.4.1.4.123"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取A4彩色总数失败");
		}
		else
		{
			m_oMeterInfo.nA4ColorTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取A4彩色总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(CStringToChar(m_szCanonScanTotalPageOID), nValue))	//扫描总数=".1.3.6.1.4.1.1602.1.11.1.4.1.4.501"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取扫描总数失败");
		}
		else
		{
			m_oMeterInfo.nScanTotalPage = (nValue > 0) ? nValue : 0;
			//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取扫描总数成功，nValue=%d", nValue);
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

		//A3彩色
		//A3黑白
		//A4彩色
		//A4黑白
		//其它彩色
		//其它黑白

		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo, nA3HBTotalPage = %d, nA3ColorTotalPage = %d, nA4HBTotalPage = %d, nA4ColorTotalPage = %d, nOtherOpHBPage = %d, nOtherOpColorPage = %d"
		//	, m_oMeterInfo.nA3HBTotalPage, m_oMeterInfo.nA3ColorTotalPage, m_oMeterInfo.nA4HBTotalPage, m_oMeterInfo.nA4ColorTotalPage, m_oMeterInfo.nOtherOpHBPage, m_oMeterInfo.nOtherOpColorPage);

		/*
		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.301", nValue))	//打印总数
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,2,获取打印总数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintTotalPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取打印总数成功，nValue=%d", nValue);
		}

		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.133", nValue))	//打印黑白页数,黑白机就是该值；彩机还需要减去".1.3.6.1.4.1.1602.1.11.1.4.1.4.229"
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,4,获取打印黑白页数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintHBPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,5,获取打印黑白页数成功，nValue=%d", nValue);

			if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.229", nValue))	
			{
				theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,6,获取打印黑白页减数失败，黑白机获取不到正常");
			}
			else
			{
				m_oMeterInfo.nA4PrintHBPage -= (nValue>0) ? nValue : 0;
				theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,7,获取打印黑白页数成功，nValue=%d", m_oMeterInfo.nA4PrintHBPage);
			}
		}

		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.324", nValue))	//打印彩色页数
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,8,获取打印彩色页数失败");
		}
		else
		{
			m_oMeterInfo.nA4PrintColorPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,9,获取打印彩色页数成功，nValue=%d", nValue);
		}

		//===============================================================复印,begin
		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.201", nValue))	//复印总页数
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,10,获取复印总页数失败");
		}
		else
		{
			m_oMeterInfo.nA4CopyTotalPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,11,获取复印总页数成功，nValue=%d", nValue);
		}

		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.222", nValue))	//复印黑白页数
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,12,获取复印黑白页数失败");
		}
		else
		{
			if(nValue <= m_oMeterInfo.nA4CopyTotalPage)
			{
				m_oMeterInfo.nA4CopyHBPage = (nValue>0) ? nValue : 0;
				theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,13,获取复印黑白页数成功，nValue=%d", nValue);

				if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.401", nValue))	//复印黑白减数
				{
					theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,14,获取复印黑白页数失败");
				}
				else
				{
					if(0 <= nValue && nValue <= m_oMeterInfo.nA4CopyHBPage)
					{
						m_oMeterInfo.nA4CopyHBPage -= nValue;
						theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,15,获取复印黑白减数成功，nA4CopyHBPage=%d", m_oMeterInfo.nA4CopyHBPage);
					}
					else
					{
						theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,16,获取复印黑白减数异常，nA4CopyHBPage=%d,nValue=%d"
							, m_oMeterInfo.nA4CopyHBPage, nValue);
					}
				}
			}
			else
			{
				m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nA4CopyTotalPage;
				theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,17,获取复印黑白页数成功,这个应该是个黑白机，只用总数，nA4CopyHBPage=%d,nA4CopyTotalPage=%d"
					, m_oMeterInfo.nA4CopyHBPage, m_oMeterInfo.nA4CopyTotalPage);
			}
		}


		if (!GetRequest(".1.3.6.1.4.1.1602.1.11.1.4.1.4.232", nValue))	//复印彩色页数
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo,18,获取复印彩色页数失败，考虑是否黑白机");
		}
		else
		{
			m_oMeterInfo.nA4CopyColorPage = (nValue>0) ? nValue : 0;
			theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,19,获取复印彩色页数成功，nValue=%d", nValue);
		}
		//===============================================================复印,end
		*/
		return TRUE;
	}
}

BOOL CSnmpPrinterMeterMonitorCANON::GetMeterInfo_LBP6300n()
{

	int nValue = 0;

	if (!GetRequest(CStringToChar(m_szCanonAllTotalPageOID_LBP6300dn), nValue))	//总数=".1.3.6.1.2.1.43.10.2.1.4.1.1"
	{
		theLog.Write("!!CSnmpPrinterMeterMonitorCANON::GetMeterInfo_LBP6300n,2,获取总数失败");
	}
	else
	{
		m_oMeterInfo.nAllTotalPage = (nValue > 0) ? nValue : m_oMeterInfo.nAllTotalPage;
		m_oMeterInfo.nOtherOpHBPage = (nValue > 0) ? nValue : m_oMeterInfo.nAllTotalPage;
		//theLog.Write("CSnmpPrinterMeterMonitorCANON::GetMeterInfo,3,获取总数成功，nValue=%d", nValue);
	}


	return TRUE;
}

//使用HTML分析来获取,要执行两次html请求，
//第一次是登录认证：http://192.168.4.227
//第二次才是实际抄表内容：http://192.168.4.227/dcounter.cgi
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
						//strTemp = strResponse.substr(nBeginPos, nEndPos-nBeginPos+strEnd.size());	//strTemp的格式为：write_value("101", 67911);
						strTemp = strResponse.substr(nBeginPos+strBegin.size(), nEndPos-nBeginPos-strBegin.size());//strTemp的格式为：101", 67911
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
两种HTML抄表类型:
一，彩色机器：（iR C3200）
101 Total 1 67911 
108 Total (Black 1) 24653 
229 Copy (Full Color + Single Color/Large) 451 
230 Copy (Full Color + Single Color/Small) 2644 
321 Print (Full Color + Single Color /Large) 656 
322 Print (Full Color + Single Color/Small) 39507 

二，黑白机器：（iR3530）
101 总数1 303 
103 总数(大) 25 
201 复印(总数1) 269 
203 复印(大) 25 
*/
//write_value是通过html内容实现的抄表数据分类分析，
//暂时忽略了其它的类型，只用了少数的类型，
//如果后面有需求，可以适当开放更多类型。
void CSnmpPrinterMeterMonitorCANON::write_value(int nType, int nValue)
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	if( nType >= 0 && nType <= 99 )
	{
// 		switch(nType)
// 		{
// 			  case   2: "远程复印(全彩色1)" break;
// 			  case   3: "远程复印(全彩色2)" break;
// 			  case   4: "远程复印(单彩色1)" break;
// 			  case   5: "远程复印(单彩色2)" break;
// 			  case   6: "远程复印(黑白1)" break;
// 			  case   7: "远程复印(黑白2)" break;
// 			  case   8: "远程复印(全彩色/大)" break;
// 			  case   9: "远程复印(全彩色/小)" break;
// 			  case  10: "远程复印(单彩色/大)" break;
// 			  case  11: "远程复印(单彩色/小)" break;
// 			  case  12: "远程复印(黑白/大)" break;
// 			  case  13: "远程复印(黑白/小)" break;
// 			  case  14: "远程复印(全彩色+单彩色/大)" break;
// 			  case  15: "远程复印(全彩色+单彩色/小)" break;
// 			  case  16: "远程复印(全彩色+单彩色2)" break;
// 			  case  17: "远程复印(全彩色+单彩色1)" break;
// 			  case  18: "远程复印(全彩色/大/双面)" break;
// 			  case  19: "远程复印(全彩色/小/双面)" break;
// 			  case  20: "远程复印(单彩色/大/双面)" break;
// 			  case  21: "远程复印(单彩色/小/双面)" break;
// 			  case  22: "远程复印(黑白/大/双面)" break;
// 			  case  23: "远程复印(黑白/小/双面)" break;
// 
// 			default: break;
// 		}
	}
	else if( nType >= 100 && nType <= 199 )
	{
		switch(nType)
		{
			case 101: m_oMeterInfo.nAllTotalPage = nValue; break;	//"总数1"
// 			case 102: "总数2" break;
 			case 103: m_oMeterInfo.nA3HBTotalPage = nValue; break;	//"总数(大)"
 			case 104: m_oMeterInfo.nA4HBTotalPage = nValue; break;	//"总数(小)"
// 			case 105: "总数(全彩色1)" break;
// 			case 106: "总数(全彩色2)" break;

// 			case 108: "总数(黑白1)" break;
// 			case 109: "总数(黑白2)" break;
// 			case 110: "总数(单彩色/大)" break;
// 			case 111: "总数(单彩色/小)" break;
 			case 112: m_oMeterInfo.nA3HBTotalPage = nValue; break;	//"总数(黑白/大)"
 			case 113: m_oMeterInfo.nA4HBTotalPage = nValue; break;	//"总数(黑白/小)"
// 			case 114: "总数1(双面)" break;
// 			case 115: "总数2(双面)" break;
// 			case 116: "大(双面)" break;
// 			case 117: "小(双面)" break;
// 			case 118: "总数(单彩色1)" break;
// 			case 119: "总数(单彩色2)" break;
// 			case 120: "总数(全彩色/大)" break;
// 			case 121: "总数(全彩色/小)" break;
 			case 122: m_oMeterInfo.nA3ColorTotalPage = nValue; break;	//"总数(全彩色+单彩色/大)"
 			case 123: m_oMeterInfo.nA4ColorTotalPage = nValue; break;	//"总数(全彩色+单彩色/小)"
// 			case 124: "总数(全彩色+单彩色2)" break;
// 			case 125: "总数(全彩色+单彩色1)" break;
// 
// 			case 126: "总数A1" break;
// 			case 127: "总数A2" break;
// 			case 128: "总数A(大)" break;
// 			case 129: "总数A(小)" break;
// 			case 130: "总数A(全彩色1)" break;
// 			case 131: "总数A(全彩色2)" break;
// 			case 132: "总数A(黑白1)" break;
// 			case 133: "总数A(黑白2)" break;
// 			case 134: "总数A(单彩色/大)" break;
// 			case 135: "总数A(单彩色/小)" break;
// 			case 136: "总数A(黑白/大)" break;
// 			case 137: "总数A(黑白/小)" break;
// 			case 138: "总数A1(双面)" break;
// 			case 139: "总数A2(双面)" break;
// 			case 140: "大A(双面)" break;
// 			case 141: "小A(双面)" break;
// 			case 142: "总数A(单彩色1)" break;
// 			case 143: "总数A(单彩色2)" break;
// 			case 144: "总数A(全彩色/大)" break;
// 			case 145: "总数A(全彩色/小)" break;
// 			case 146: "总数A(全彩色+单彩色/大)" break;
// 			case 147: "总数A(全彩色+单彩色/小)" break;
// 			case 148: "总数A(全彩色+单彩色2)" break;
// 			case 149: "总数A(全彩色+单彩色1)" break;
// 			case 150: "总数B1" break;
// 			case 151: "总数B2" break;
// 			case 152: "总数B(大)" break;
// 			case 153: "总数B(小)" break;
// 			case 154: "总数B(全彩色1)" break;
// 			case 155: "总数B(全彩色2)" break;
// 			case 156: "总数B(黑白1)" break;
// 			case 157: "总数B(黑白2)" break;
// 			case 158: "总数B(单彩色/大)" break;
// 			case 159: "总数B(单彩色/小)" break;
// 			case 160: "总数B(黑白/大)" break;
// 			case 161: "总数B(黑白/小)" break;
// 			case 162: "总数B1(双面)" break;
// 			case 163: "总数B2(双面)" break;
// 			case 164: "大B(双面)" break;
// 			case 165: "小B(双面)" break;
// 			case 166: "总数B(单彩色1)" break;
// 			case 167: "总数B(单彩色2)" break;
// 			case 168: "总数B(全彩色/大)" break;
// 			case 169: "总数B(全彩色/小)" break;
// 			case 170: "总数B(全彩色+单彩色/大)" break;
// 			case 171: "总数B(全彩色+单彩色/小)" break;
// 			case 172: "总数B(全彩色+单彩色2)" break;
// 			case 173: "总数B(全彩色+单彩色1)" break;

			default: break;
		}
	}
	else if( nType >= 200 && nType <= 299 )
	{
		switch(nType)
		{
 			case 201: m_oMeterInfo.nOtherCopyTotalPage = nValue; break;	//"复印(总数1)"
// 			case 202: "复印(总数2)" break;
 			case 203: //"复印(大)"
				{
					//一般是黑白机器，没有复印小纸和打印抄表，这里计算出打印抄表
					m_oMeterInfo.nA3CopyHBPage = nValue;
					if (m_oMeterInfo.nOtherCopyTotalPage > 0)
					{
						m_oMeterInfo.nA4CopyHBPage = m_oMeterInfo.nOtherCopyTotalPage - m_oMeterInfo.nA3CopyHBPage;
						m_oMeterInfo.nA3PrintHBPage = m_oMeterInfo.nA3HBTotalPage - m_oMeterInfo.nA3CopyHBPage;
						m_oMeterInfo.nA4PrintHBPage = m_oMeterInfo.nAllTotalPage - m_oMeterInfo.nOtherCopyTotalPage - m_oMeterInfo.nA3PrintHBPage;
						m_oMeterInfo.nOtherCopyTotalPage = 0;	//重置这个数值，因为上面的各个分量已经包含了这个数值
					}
				} 
				break;	
 			case 204: m_oMeterInfo.nA4CopyHBPage = nValue; break;	//"复印(小)"
// 			case 205: "复印A(总数1)" break;
// 			case 206: "复印A(总数2)" break;
// 			case 207: "复印A(大)" break;
// 			case 208: "复印A(小)" break;
// 			case 209: "本地复印(总数1)" break;
// 			case 210: "本地复印(总数2)" break;
// 			case 211: "本地复印(大)" break;
// 			case 212: "本地复印(小)" break;
// 			case 213: "远程复印(总数1)" break;
// 			case 214: "远程复印(总数2)" break;
// 			case 215: "远程复印(大)" break;
// 			case 216: "远程复印(小)" break;
// 			case 217: "复印(全彩色1)" break;
// 			case 218: "复印(全彩色2)" break;
// 			case 219: "复印(单彩色1)" break;
// 			case 220: "复印(单彩色2)" break;
// 			case 221: "复印(黑白1)" break;
// 			case 222: "复印(黑白2)" break;
// 			case 223: "复印(全彩色/大)" break;
// 			case 224: "复印(全彩色/小)" break;
// 			case 225: "复印(单彩色/大)" break;
// 			case 226: "复印(单彩色/小)" break;			
			case 227: m_oMeterInfo.nA3CopyHBPage = nValue; break;	//"复印(黑白/大)"
			case 228: m_oMeterInfo.nA4CopyHBPage = nValue; break;	//"复印(黑白/小)"
			case 229: m_oMeterInfo.nA3CopyColorPage = nValue; break;	//"复印(全彩色+单彩色/大)"
			case 230: m_oMeterInfo.nA4CopyColorPage = nValue; break;	//"复印(全彩色+单彩色/小)"
// 			case 231: "复印(全彩色+单彩色/2)" break;
// 
// 			case 232: "复印(全彩色+单彩色/1)" break;
// 			case 233: "复印(全彩色/大/双面)" break;
// 			case 234: "复印(全彩色/小/双面)" break;
// 			case 235: "复印(单彩色/大/双面)" break;
// 			case 236: "复印(单彩色/小/双面)" break;
// 			case 237: "复印(黑白/大/双面)" break;
// 			case 238: "复印(黑白/小/双面)" break;
// 			case 239: "复印(双色1)" break;
// 			case 240: "复印(双色2)" break;
// 			case 241: "复印(双色/大)" break;
// 			case 242: "复印(双色/小)" break;
// 			case 243: "复印(双色/大/双面)" break;
// 			case 244: "复印(双色/小/双面)" break;
// 			case 245: "复印A(全彩色1)" break;
// 			case 246: "复印A(全彩色2)" break;
// 			case 247: "复印A(单彩色1)" break;
// 			case 248: "复印A(单彩色2)" break;
// 			case 249: "复印A(黑白1)" break;
// 			case 250: "复印A(黑白2)" break;
// 			case 251: "复印A(全彩色/大)" break;
// 			case 252: "复印A(全彩色/小)" break;
// 			case 253: "复印A(单彩色/大)" break;
// 			case 254: "复印A(单彩色/小)" break;
// 			case 255: "复印A(黑白/大)" break;
// 			case 256: "复印A(黑白/小)" break;
// 			case 257: "复印A(全彩色+单彩色/大)" break;
// 			case 258: "复印A(全彩色+单彩色/小)" break;
// 			case 259: "复印A(全彩色+单彩色2)" break;
// 			case 260: "复印A(全彩色+单彩色1)" break;
// 			case 261: "复印A(全彩色/大/双面)" break;
// 			case 262: "复印A(全彩色/小/双面)" break;
// 			case 263: "复印A(单彩色/大/双面)" break;
// 			case 264: "复印A(单彩色/小/双面)" break;
// 			case 265: "复印A(黑白/大/双面)" break;
// 			case 266: "复印A(黑白/小/双面)" break;
// 
// 			case 273: "本地复印(全彩色1)" break;
// 			case 274: "本地复印(全彩色2)" break;
// 			case 275: "本地复印(单彩色1)" break;
// 			case 276: "本地复印(单彩色2)" break;
// 			case 277: "本地复印(黑白1)" break;
// 			case 278: "本地复印(黑白2)" break;
// 			case 279: "本地复印(全彩色/大)" break;
// 			case 280: "本地复印(全彩色/小)" break;
// 			case 281: "本地复印(单彩色/大)" break;
// 			case 282: "本地复印(单彩色/小)" break;
// 			case 283: "本地复印(黑白/大)" break;
// 			case 284: "本地复印(黑白/小)" break;
// 			case 285: "本地复印(全彩色+单彩色/大)" break;
// 			case 286: "本地复印(全彩色+单彩色/小)" break;
// 			case 287: "本地复印(全彩色+单彩色2)" break;
// 			case 288: "本地复印(全彩色+单彩色1)" break;
// 			case 289: "本地复印(全彩色/大/双面)" break;
// 			case 290: "本地复印(全彩色/小/双面)" break;
// 			case 291: "本地复印(单彩色/大/双面)" break;
// 			case 292: "本地复印(单彩色/小/双面)" break;
// 			case 293: "本地复印(黑白/大/双面)" break;
// 			case 294: "本地复印(黑白/小/双面)" break;

			default: break;
		}
	}
	else if( nType >= 300 && nType <= 399 )
	{
		switch(nType)
		{
// 			case 301: "打印(总数1)" break;
// 			case 302: "打印(总数2)" break;
// 			case 303: "打印(大)" break;
// 			case 304: "打印(小)" break;
// 			case 305: "打印A(总数1)" break;
// 			case 306: "打印A(总数2)" break;
// 			case 307: "打印A(大)" break;
// 			case 308: "打印A(小)" break;
// 			case 309: "打印(全彩色1)" break;
// 			case 310: "打印(全彩色2)" break;
// 			case 311: "打印(单彩色1)" break;
// 			case 312: "打印(单彩色2)" break;
// 			case 313: "打印(黑白1)" break;
// 			case 314: "打印(黑白2)" break;
// 			case 315: "打印(全彩色/大)" break;
// 			case 316: "打印(全彩色/小)" break;
// 			case 317: "打印(单彩色/大)" break;
// 			case 318: "打印(单彩色/小)" break;
			case 319: m_oMeterInfo.nA3PrintHBPage = nValue; break;	//"打印(黑白/大)"
			case 320: m_oMeterInfo.nA4PrintHBPage = nValue; break;	//"打印(黑白/小)"
			case 321: m_oMeterInfo.nA3PrintColorPage = nValue; break;	//"打印(全彩色+单彩色/大)"
			case 322: m_oMeterInfo.nA4PrintColorPage = nValue; break;	//"打印(全彩色+单彩色/小)"
// 			case 323: "打印(全彩色+单彩色/2)" break;
// 			case 324: "打印(全彩色+单彩色/1)" break;
// 			case 325: "打印(全彩色/大/双面)" break;
// 			case 326: "打印(全彩色/小/双面)" break;
// 			case 327: "打印(单彩色/大/双面)" break;
// 			case 328: "打印(单彩色/小/双面)" break;
// 			case 329: "打印(黑白/大/双面)" break;
// 			case 330: "打印(黑白/小/双面)" break;
// 			case 331: "打印机驱动程序打印(总数1)" break;
// 			case 332: "打印机驱动程序打印(总数2)" break;
// 			case 333: "打印机驱动程序打印(大)" break;
// 			case 334: "打印机驱动程序打印(小)" break;
// 			case 335: "打印机驱动程序打印(全彩色1)" break;
// 			case 336: "打印机驱动程序打印(全彩色2)" break;
// 
// 			case 339: "打印机驱动程序打印(黑白1)" break;
// 			case 340: "打印机驱动程序打印(黑白2)" break;
// 			case 341: "打印机驱动程序打印(全彩色/大)" break;
// 			case 342: "打印机驱动程序打印(全彩色/小)" break;
// 
// 			case 345: "打印机驱动程序打印(黑白/大)" break;
// 			case 346: "打印机驱动程序打印(黑白/小)" break;
// 
// 			case 351: "打印机驱动程序打印(全彩色/大/双面)" break;
// 			case 352: "打印机驱动程序打印(全彩色/小/双面)" break;
// 
// 			case 355: "打印机驱动程序打印(黑白/大/双面)" break;
// 			case 356: "打印机驱动程序打印(黑白/小/双面)" break;

			default: break;
		}
	}
// 	else if( nType >= 400 && nType <= 499 )
// 	{
// 		switch(nType)
// 		{
// 			  case 401: "复印+打印(全彩色/大)" break;
// 			  case 402: "复印+打印(全彩色/小)" break;
// 			  case 403: "复印+打印(黑白/大)" break;
// 			  case 404: "复印+打印(黑白/小)" break;
// 			  case 405: "复印+打印(黑白2)" break;
// 			  case 406: "复印+打印(黑白1)" break;
// 			  case 407: "复印+打印(全彩色+单彩色/大)" break;
// 			  case 408: "复印+打印(全彩色+单彩色/小)" break;
// 			  case 409: "复印+打印(全彩色+单彩色/2)" break;
// 			  case 410: "复印+打印(全彩色+单彩色/1)" break;
// 			  case 411: "复印+打印(大)" break;
// 			  case 412: "复印+打印(小)" break;
// 			  case 413: "复印+打印(2)" break;
// 			  case 414: "复印+打印(1)" break;
// 			  case 415: "复印+打印(单彩色/大)" break;
// 			  case 416: "复印+打印(单彩色/小)" break;
// 			  case 417: "复印+打印(全彩色/大/双面)" break;
// 			  case 418: "复印+打印(全彩色/小/双面)" break;
// 			  case 419: "复印+打印(单彩色/大/双面)" break;
// 			  case 420: "复印+打印(单彩色/小/双面)" break;
// 			  case 421: "复印+打印(黑白/大/双面)" break;
// 			  case 422: "复印+打印(黑白/小/双面)" break;
// 
// 			default: break;
// 		}
// 	}
	else if( nType >= 500 && nType <= 599 )
	{
		switch(nType)
		{
			case 501: m_oMeterInfo.nScanTotalPage = nValue; break;	//"扫描(总数1)"
// 			case 502: "扫描(总数2)" break;
// 			case 503: "扫描(大)" break;
// 			case 504: "扫描(小)" break;
// 			case 505: "黑白扫描(总数1)" break;
// 			case 506: "黑白扫描(总数2)" break;
			case 507: m_oMeterInfo.nScanBigHBPage = nValue; break;	//"黑白扫描(大)"
			case 508: m_oMeterInfo.nScanSmallHBPage = nValue; break;	//"黑白扫描(小)"
// 			case 509: "彩色扫描(总数1)" break;
// 			case 510: "彩色扫描(总数2)" break;
			case 511: m_oMeterInfo.nScanBigColorPage = nValue; break;	//"彩色扫描(大)"
			case 512: m_oMeterInfo.nScanSmallColorPage = nValue; break;	//"彩色扫描(小)"
// 			case 513: "复印扫描(大)" break;
// 			case 514: "复印扫描(小)" break;
// 			case 515: "复印扫描(总数)" break;

			default: break;
		}
	}
// 	else if( nType >= 600 && nType <= 699 )
// 	{
// 		switch(nType)
// 		{
// 			  case 601: "收件箱打印(总数1)" break;
// 			  case 602: "收件箱打印(总数2)" break;
// 			  case 603: "收件箱打印(大)" break;
// 			  case 604: "收件箱打印(小)" break;
// 			  case 605: "收件箱打印(全彩色1)" break;
// 			  case 606: "收件箱打印(全彩色2)" break;
// 			  case 607: "收件箱打印(单彩色1)" break;
// 			  case 608: "收件箱打印(单彩色2)" break;
// 			  case 609: "收件箱打印(黑白1)" break;
// 			  case 610: "收件箱打印(黑白2)" break;
// 			  case 611: "收件箱打印(全彩色/大)" break;
// 			  case 612: "收件箱打印(全彩色/小)" break;
// 			  case 613: "收件箱打印(单彩色/大)" break;
// 			  case 614: "收件箱打印(单彩色/小)" break;
// 			  case 615: "收件箱打印(黑白/大)" break;
// 			  case 616: "收件箱打印(黑白/小)" break;
// 			  case 617: "收件箱打印(全彩色+单彩色/大)" break;
// 			  case 618: "收件箱打印(全彩色+单彩色/小)" break;
// 			  case 619: "收件箱打印(全彩色+单彩色2)" break;
// 			  case 620: "收件箱打印(全彩色+单彩色1)" break;
// 			  case 621: "收件箱打印(全彩色/大/双面)" break;
// 			  case 622: "收件箱打印(全彩色/小/双面)" break;
// 			  case 623: "收件箱打印(单彩色/大/双面)" break;
// 			  case 624: "收件箱打印(单彩色/小/双面)" break;
// 			  case 625: "收件箱打印(黑白/大/双面)" break;
// 			  case 626: "收件箱打印(黑白/小/双面)" break;
// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 700 && nType <= 799 )
// 	{
// 		switch(nType)
// 		{
// 			  case 701: "接收打印(总数1)" break;
// 			  case 702: "接收打印(总数2)" break;
// 			  case 703: "接收打印(大)" break;
// 			  case 704: "接收打印(小)" break;
// 			  case 705: "接收打印(全彩色1)" break;
// 			  case 706: "接收打印(全彩色2)" break;
// 			  case 707: "接收打印(灰度1)" break;
// 			  case 708: "接收打印(灰度2)" break;
// 			  case 709: "接收打印(黑白1)" break;
// 			  case 710: "接收打印(黑白2)" break;
// 			  case 711: "接收打印(全彩色/大)" break;
// 			  case 712: "接收打印(全彩色/小)" break;
// 			  case 713: "接收打印(灰度/大)" break;
// 			  case 714: "接收打印(灰度/小)" break;
// 			  case 715: "接收打印(黑白/大)" break;
// 			  case 716: "接收打印(黑白/小)" break;
// 			  case 717: "接收打印(全彩色+灰度/大)" break;
// 			  case 718: "接收打印(全彩色+灰度/小)" break;
// 			  case 719: "接收打印(全彩色+灰度2)" break;
// 			  case 720: "接收打印(全彩色+灰度1)" break;
// 			  case 721: "接收打印(全彩色/大/双面)" break;
// 			  case 722: "接收打印(全彩色/小/双面)" break;
// 			  case 723: "接收打印(灰度/大/双面)" break;
// 			  case 724: "接收打印(灰度/小/双面)" break;
// 			  case 725: "接收打印(黑白/大/双面)" break;
// 			  case 726: "接收打印(黑白/小/双面)" break;

// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 800 && nType <= 899 )
// 	{
// 		switch(nType)
// 		{
// 			  case 801: "报告打印(总数1)" break;
// 			  case 802: "报告打印(总数2)" break;
// 			  case 803: "报告打印(大)" break;
// 			  case 804: "报告打印(小)" break;
// 			  case 805: "报告打印(全彩色1)" break;
// 			  case 806: "报告打印(全彩色2)" break;
// 			  case 807: "报告打印(灰度1)" break;
// 			  case 808: "报告打印(灰度2)" break;
// 			  case 809: "报告打印(黑白1)" break;
// 			  case 810: "报告打印(黑白2)" break;
// 			  case 811: "报告打印(全彩色/大)" break;
// 			  case 812: "报告打印(全彩色/小)" break;
// 			  case 813: "报告打印(灰度/大)" break;
// 			  case 814: "报告打印(灰度/小)" break;
// 			  case 815: "报告打印(黑白/大)" break;
// 			  case 816: "报告打印(黑白/小)" break;
// 			  case 817: "报告打印(全彩色+灰度/大)" break;
// 			  case 818: "报告打印(全彩色+灰度/小)" break;
// 			  case 819: "报告打印(全彩色+灰度2)" break;
// 			  case 820: "报告打印(全彩色+灰度1)" break;
// 			  case 821: "报告打印(全彩色/大/双面)" break;
// 			  case 822: "报告打印(全彩色/小/双面)" break;
// 			  case 823: "报告打印(灰度/大/双面)" break;
// 			  case 824: "报告打印(灰度/小/双面)" break;
// 			  case 825: "报告打印(黑白/大/双面)" break;
// 			  case 826: "报告打印(黑白/小/双面)" break;
// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 900 && nType <= 999 )
// 	{
// 		switch(nType)
// 		{
// 			  case 901: "复印扫描总数1(彩色)" break;
// 			  case 902: "复印扫描总数1(黑白)" break;
// 			  case 903: "复印扫描总数2(彩色)" break;
// 			  case 904: "复印扫描总数2(黑白)" break;
// 			  case 905: "复印扫描总数3(彩色)" break;
// 			  case 906: "复印扫描总数3(黑白)" break;
// 			  case 907: "复印扫描总数4(彩色)" break;
// 			  case 908: "复印扫描总数4(黑白)" break;
// 			  case 909: "本地复印扫描(彩色)" break;
// 			  case 910: "本地复印扫描(黑白)" break;
// 			  case 911: "远程复印扫描(彩色)" break;
// 			  case 912: "远程复印扫描(黑白)" break;
// 			  case 913: "发送扫描总数1(彩色)" break;
// 			  case 914: "发送扫描总数1(黑白)" break;
// 			  case 915: "发送扫描总数2(彩色)" break;
// 			  case 916: "发送扫描总数2(黑白)" break;
// 			  case 917: "发送扫描总数3(彩色)" break;
// 			  case 918: "发送扫描总数3(黑白)" break;
// 			  case 919: "发送扫描总数4(彩色)" break;
// 			  case 920: "发送扫描总数4(黑白)" break;
// 			  case 921: "发送扫描总数5(彩色)" break;
// 			  case 922: "发送扫描总数5(黑白)" break;
// 
// 			  case 929: "发送扫描总数6(彩色)" break;
// 			  case 930: "发送扫描总数6(黑白)" break;
// 			  case 931: "发送扫描总数7(彩色)" break;
// 			  case 932: "发送扫描总数7(黑白)" break;
// 			  case 933: "发送扫描总数8(彩色)" break;
// 			  case 934: "发送扫描总数8(黑白)" break;
// 			  case 935: "通用发送扫描总数(彩色)" break;
// 			  case 936: "通用发送扫描总数(黑白)" break;
// 			  case 937: "收件箱扫描(彩色)" break;
// 			  case 938: "收件箱扫描(黑白)" break;
// 			  case 939: "远程扫描(彩色)" break;
// 			  case 940: "远程扫描(黑白)" break;
// 			  case 941: "发送扫描/传真(彩色)" break;
// 			  case 942: "发送扫描/传真(黑白)" break;
// 			  case 943: "发送扫描/互联网传真(彩色)" break;
// 			  case 944: "发送扫描/互联网传真(黑白)" break;
// 			  case 945: "发送扫描/电子邮件(彩色)" break;
// 			  case 946: "发送扫描/电子邮件(黑白)" break;
// 			  case 947: "发送扫描/FTP(彩色)" break;
// 			  case 948: "发送扫描/FTP(黑白)" break;
// 			  case 949: "发送扫描/SMB(彩色)" break;
// 			  case 950: "发送扫描/SMB(黑白)" break;
// 			  case 951: "发送扫描/IPX(彩色)" break;
// 			  case 952: "发送扫描/IPX(黑白)" break;
// 			  case 953: "发送扫描/数据库(彩色)" break;
// 			  case 954: "发送扫描/数据库(黑白)" break;
// 			  case 955: "发送扫描/本地打印(彩色)" break;
// 			  case 956: "发送扫描/本地打印(黑白)" break;
// 			  case 957: "发送扫描/收件箱(彩色)" break;
// 			  case 958: "发送扫描/收件箱(黑白)" break;
// 
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 1000 && nType <= 1999 )
// 	{
// 		switch(nType)
// 		{
// 			case 1000: "总数1" break;
// 			case 1001: "总数(小)" break;
// 			case 1002: "总数(大)" break;
// 			case 1003: "总数(黑白1)" break;
// 			case 1004: "总数(黑白/小)" break;
// 			case 1005: "总数(黑白/大)" break;
// 			case 1006: "总数(全彩色1)" break;
// 			case 1007: "总数(全彩色/小)" break;
// 			case 1008: "总数(全彩色/大)" break;
// 			case 1009: "总数(单彩色1)" break;
// 			case 1010: "总数(单彩色/小)" break;
// 			case 1011: "总数(单彩色/大)" break;
// 			default: break;
// 		}
// 	  }
// 	else if( nType >= 2000 && nType <= 2999 )
// 	{
// 		switch(nType)
// 		{
// 			case 2000: "扫描(总数1)" break;
// 			case 2001: "扫描(小)" break;
// 			case 2002: "扫描(大)" break;
// 			case 2003: "黑白扫描(总数1)" break;
// 			case 2004: "黑白扫描(小)" break;
// 			case 2005: "黑白扫描(大)" break;
// 			case 2006: "彩色扫描(总数1)" break;
// 			case 2007: "彩色扫描(小)" break;
// 			case 2008: "彩色扫描(大)" break;
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 3000 && nType <= 3999 )
// 	{
// 		switch(nType)
// 		{
// 			case 3000: "彩色扫描1" break;
// 			case 3001: "彩色扫描2" break;
// 			case 3002: "彩色扫描3" break;
// 			case 3003: "彩色扫描4" break;
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 4000 && nType <= 4999 )
// 	{
// 		switch(nType)
// 		{
// 			case 4000: "黑白扫描1" break;
// 			case 4001: "黑白扫描2" break;
// 			case 4002: "黑白扫描3" break;
// 			case 4003: "黑白扫描4" break;
// 			default: break;
// 		}
// 	}
// 	else if( nType >= 5000 && nType <= 5999 )
// 	{
// 		switch(nType)
// 		{
// 			case 5000: "任意1" break;
// 			case 5001: "任意2" break;
// 			case 5002: "任意3" break;
// 			case 5003: "任意4" break;
// 			case 5004: "任意5" break;
// 			case 5005: "任意6" break;
// 			case 5006: "任意7" break;
// 			case 5007: "任意8" break;
// 			case 5008: "任意9" break;
// 			case 5009: "任意10" break;
// 			case 5010: "任意11" break;
// 			case 5011: "任意12" break;
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
		theLog.Write("!!CSnmpPrinterMeterMonitorHP::CSnmpPrinterMeterMonitorCANON,获取型号失败");
	}
	szModel.Format("%s",tmp);
	//theLog.Write("CSnmpPrinterMeterMonitorHP::CSnmpPrinterMeterMonitorCANON,szModel=%s",szModel);
	return szModel;
	
}
