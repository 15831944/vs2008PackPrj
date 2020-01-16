#include "StdAfx.h"
#include "SnmpMonitorHelperDASCOM.h"
#include "SimpleSocket.h"
#include "Dascom_Def.h"

CSnmpMonitorHelperDASCOM::CSnmpMonitorHelperDASCOM(void)
{
}

CSnmpMonitorHelperDASCOM::~CSnmpMonitorHelperDASCOM(void)
{
}

BOOL CSnmpMonitorHelperDASCOM::GetAlert()
{
	CString szPrinterIP = GetPrtIP();
	int nPort = 9100;
	CSimpleSocket m_oSocket;
	if (m_oSocket.Connect(szPrinterIP, nPort))
	{
		//请求设备信息命令数据
		unsigned char ucRequestDascomDeviceInfo[3] = {0x1B, 0x0D, 0x42};
		m_oSocket.SendData((const char*)ucRequestDascomDeviceInfo, sizeof(ucRequestDascomDeviceInfo));
		DascomDeviceInfo oDascomDeviceInfo;
		memset(&oDascomDeviceInfo, 0x0, sizeof(DascomDeviceInfo));
		if (m_oSocket.RecvData((char*)ucRequestDascomDeviceInfo, sizeof(DascomDeviceInfo)))
		{
			//m_oSocket.DumpData((char*)&oDascomDeviceInfo, sizeof(DascomDeviceInfo));

			//模拟SNMP的GetAlert操作，添加一个模拟预警信息
			if ((oDascomDeviceInfo.ucDeviceStatus & DascomDeviceStatus_Busy) == DascomDeviceStatus_Busy)
			{
				AddAlert(1, ENUM_STRUCT_VALUE(PrinterAlertType)::Other, ENUM_STRUCT_VALUE(PrinterAlertCode)::Other, "设备忙");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus & DascomDeviceStatus_CutterError) == DascomDeviceStatus_CutterError)
			{
				AddAlert(2, ENUM_STRUCT_VALUE(PrinterAlertType)::PrinterFault, ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, "切刀出错");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus & DascomDeviceStatus_NoPaper) == DascomDeviceStatus_NoPaper)
			{
				AddAlert(3, ENUM_STRUCT_VALUE(PrinterAlertType)::Tary, ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperEmpty, "NoPaper");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus & DascomDeviceStatus_Jam) == DascomDeviceStatus_Jam)
			{
				AddAlert(4, ENUM_STRUCT_VALUE(PrinterAlertType)::Input, ENUM_STRUCT_VALUE(PrinterAlertCode)::Jam, "Jam");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus2 & DascomDeviceStatus2_LabelCheckError) == DascomDeviceStatus2_LabelCheckError)
			{
				AddAlert(5, ENUM_STRUCT_VALUE(PrinterAlertType)::PrinterFault, ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, "标签校验出错");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus2 & DascomDeviceStatus2_BufferNotEmpty) == DascomDeviceStatus2_BufferNotEmpty)
			{
				AddAlert(6, ENUM_STRUCT_VALUE(PrinterAlertType)::Other, ENUM_STRUCT_VALUE(PrinterAlertCode)::Other, "缓冲非空");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus3 & DascomDeviceStatus3_PrintHeadOverheat) == DascomDeviceStatus3_PrintHeadOverheat)
			{
				AddAlert(7, ENUM_STRUCT_VALUE(PrinterAlertType)::PrinterFault, ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, "打印头过热");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus3 & DascomDeviceStatus3_PrintHeadRaise) == DascomDeviceStatus3_PrintHeadRaise)
			{
				AddAlert(8, ENUM_STRUCT_VALUE(PrinterAlertType)::PrinterFault, ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, "打印头抬起");
			}
			if ((oDascomDeviceInfo.ucDeviceStatus3 & DascomDeviceStatus3_TTRError) == DascomDeviceStatus3_TTRError)
			{
				AddAlert(9, ENUM_STRUCT_VALUE(PrinterAlertType)::PrinterFault, ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, "碳带出错");
			}
		}
		else
		{
			theLog.Write("!!CSnmpMonitorHelperDASCOM::GetAlert,2,DascomPrinterRequestUsage fail");
		}

		m_oSocket.Close();

		return TRUE;
	}

	return FALSE;
}

void CSnmpMonitorHelperDASCOM::AddAlert(int nIndex, int nAlertGroup, int nAlertCode, CString prtAlertDescription)
{
	PPrtAlertEntry pEntry = new PrtAlertEntry;
	memset(pEntry, 0x0, sizeof(PrtAlertEntry));
	m_oAlertMap.insert(pair<int,PPrtAlertEntry>(nIndex, pEntry));
	//pEntry->prtAlertIndex = nIndex;
	//pEntry->prtAlertSeverityLevel = ;
	//pEntry->prtAlertTrainingLevel = ;
	pEntry->prtAlertGroup = (ENUM_STRUCT_TYPE(PrtAlertGroupTC))nAlertGroup;
	//pEntry->prtAlertGroupIndex = ;
	//pEntry->prtAlertLocation = ;
	pEntry->prtAlertCode = (ENUM_STRUCT_TYPE(PrtAlertCodeTC))nAlertCode;
	strcpy_s((char*)pEntry->prtAlertDescription, sizeof(pEntry->prtAlertDescription), prtAlertDescription.GetString());
	//pEntry->prtAlertTime = 0;
}
