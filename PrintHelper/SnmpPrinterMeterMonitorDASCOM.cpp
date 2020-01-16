#include "StdAfx.h"
#include "SnmpPrinterMeterMonitorDASCOM.h"
#include "SimpleSocket.h"
#include "Dascom_Def.h"

CSnmpPrinterMeterMonitorDASCOM::CSnmpPrinterMeterMonitorDASCOM(void)
{
}

CSnmpPrinterMeterMonitorDASCOM::~CSnmpPrinterMeterMonitorDASCOM(void)
{
}

BOOL CSnmpPrinterMeterMonitorDASCOM::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitorDASCOM::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	CString szPrinterIP = GetPrtIP();
	int nPort = 9100;
	CSimpleSocket m_oSocket;
	if (m_oSocket.Connect(szPrinterIP, nPort))
	{
		int nValue = 0;
		unsigned char ucRespondData[8];

		//请求打印量命令数据
		unsigned char ucRequestDascomPrintUsage[3] = {0x1C, 0x0E, 0x43};
		m_oSocket.SendData((const char*)ucRequestDascomPrintUsage, sizeof(ucRequestDascomPrintUsage));
		memset(ucRespondData, 0x0, sizeof(ucRespondData));
		if (m_oSocket.RecvData((char*)ucRespondData, sizeof(ucRequestDascomPrintUsage)))
		{
			//将返回结果的后4个字节转换为int型，实际是有8个字节的，
			//但是因为打印系统使用的是int类型，所以目前暂时只用后4个字节。
			//m_oSocket.DumpData((char*)ucRespondData, 8);
			nValue = (ucRespondData[4]<<24) + (ucRespondData[5]<<16) + (ucRespondData[6]<<8) + ucRespondData[7];
			m_oMeterInfo.nPrintDistance = nValue;
			m_oMeterInfo.nAllTotalDistance = nValue;
		}
		else
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorDASCOM::GetMeterInfo,2,DascomPrinterRequestUsage fail");
		}

		//请求打印页数命令数据
		unsigned char ucRequestDascomPrintPages[3] = {0x1C, 0x0E, 0x50};
		m_oSocket.SendData((const char*)ucRequestDascomPrintPages, sizeof(ucRequestDascomPrintPages));
		memset(ucRespondData, 0x0, sizeof(ucRespondData));
		if (m_oSocket.RecvData((char*)ucRespondData, sizeof(ucRequestDascomPrintPages)))
		{
			//m_oSocket.DumpData((char*)bRespondData, 8);
			nValue = (ucRespondData[4]<<24) + (ucRespondData[5]<<16) + (ucRespondData[6]<<8) + ucRespondData[7];
			m_oMeterInfo.nAllTotalPage = nValue;
		}
		else
		{
			theLog.Write("!!CSnmpPrinterMeterMonitorDASCOM::GetMeterInfo,3,DascomPrinterRequestPages fail");
		}

		m_oSocket.Close();

		return TRUE;
	}

	return FALSE;
}

