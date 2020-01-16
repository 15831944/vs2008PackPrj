#ifndef _XEROX_MIB_DEF_8BE86FA5_B805_43A0_AEDD_681C9DAEDF4A_H_
#define _XEROX_MIB_DEF_8BE86FA5_B805_43A0_AEDD_681C9DAEDF4A_H_

#include "Snmp_Def.h"

//施乐抄表计数节点
#define XeroxA4PrintHBPageOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9Ug==")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.7"
#define XeroxA4PrintColorPageOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9V1o=")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.29"
#define XeroxA4CopyHBPageOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MDAkdhVU0B")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.103.20.3"
#define XeroxA4CopyColorPageOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MDAkdhVU0AZg==")			//".1.3.6.1.4.1.253.8.53.13.2.1.6.103.20.25"
#define XeroxScannedImagesStoredOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MDA0dhVU0AYg==")			//".1.3.6.1.4.1.253.8.53.13.2.1.6.102.20.21"
#define XeroxNetworkScanningImagesSentOID	TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MDA0dhVU0DYg==")			//".1.3.6.1.4.1.253.8.53.13.2.1.6.102.20.11"
#define XeroxA4FaxTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9UlI=")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.71"
#define XeroxA4ColorTotalPageOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9VlA=")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.33"
#define XeroxA4HBTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9Vlc=")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.34"
#define XeroxA3ColorTotalPageOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9UVA=")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.43"
#define XeroxA3HBTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MdA1l9UVc=")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.1.20.44"
#define XeroxA3HBTotalPageOIDDocuCentre_S2110 				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRBHQBafVdNA31CT0MDAEdhVU0H")				//".1.3.6.1.4.1.253.8.53.13.2.1.6.101.20.5"

//施乐作业状态
#define XeroxJobStatusProcessing			7		//作业正在处理正在
#define XeroxJobStatusFinished				17		//作业已经结束

//施乐作业类型OID
#define XeroxJobTypeScanToFileOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHYg==")								//".1.3.6.1.4.1.253.8.58.2.1"
#define XeroxJobTypeScanToPrintOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHYQ==")								//".1.3.6.1.4.1.253.8.58.2.2"
#define XeroxJobTypeScanToFaxOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHYA==")								//".1.3.6.1.4.1.253.8.58.2.3"
#define XeroxJobTypeScanToMailListOID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHZw==")								//".1.3.6.1.4.1.253.8.58.2.4"
#define XeroxJobTypeFaxToFileOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHZg==")								//".1.3.6.1.4.1.253.8.58.2.5"
#define XeroxJobTypeFaxToPrintOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHZQ==")								//".1.3.6.1.4.1.253.8.58.2.6"
#define XeroxJobTypeFaxToMailListOID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHZA==")								//".1.3.6.1.4.1.253.8.58.2.7"
#define XeroxJobTypePrintOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHaw==")								//".1.3.6.1.4.1.253.8.58.2.8"
#define XeroxJobTypeFileToFaxOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHag==")								//".1.3.6.1.4.1.253.8.58.2.9"
#define XeroxJobTypeFileToMailListOID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHYlU=")								//".1.3.6.1.4.1.253.8.58.2.10"
#define XeroxJobTypeCopyOID					TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHYlQ=")								//".1.3.6.1.4.1.253.8.58.2.11"
#define XeroxJobTypeFileToFileOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRKHQNHYlc=")								//".1.3.6.1.4.1.253.8.58.2.12"

//施乐当前作业信息
#define XeroxCurentJobListBeginOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRLHQdHYktSHGFaUEICH1F9UVscZ0xPRgsfXWs=")	//".1.3.6.1.4.1.253.8.59.6.1.1.2.101.8.48.48.48.48"		//施乐打印机当前作业列表起始oid
#define XeroxCurentJobTypeOID				TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRLHQdHYktSHGdaUEICH1F9UVscZ0xPRgsfXWs=")	//".1.3.6.1.4.1.253.8.59.6.1.1.4.101.8.48.48.48.48"		//作业类型的OID
#define XeroxCurentJobDocNameIndexOID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRLHQdHYktSHGZaUEICH1F9UVscZ0xPRgsfXWs=")	//".1.3.6.1.4.1.253.8.59.6.1.1.5.101.8.48.48.48.48"		//文档名Index
#define XeroxCurentJobOwnerIndexOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRLHQdHYktSHGVaUEICH1F9UVscZ0xPRgsfXWs=")	//".1.3.6.1.4.1.253.8.59.6.1.1.6.101.8.48.48.48.48"		//作业用户名Index
#define XeroxCurentJobSenderIndexOID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRLHQBZfVRNA31FT0MDAEdrS1cKfUBZXAcJR2dd")	//".1.3.6.1.4.1.253.8.59.10.1.1.1.101.8.48.48.48.48"	//作业发起者Index
#define XeroxCurentJobDocNameOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRDHQRHYktSHGdaUEIC")						//".1.3.6.1.4.1.253.8.51.5.1.1.4.101"					//XeroxCurentJobDocNameIndexOID,Index代表作业ID,=".1.3.6.1.4.1.253.8.51.5.1.1.4.101.Index.3"

//施乐历史作业信息
#define XeroxHistoryJobListBeginOID			TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRLHQdHYktSHGY=")							//".1.3.6.1.4.1.253.8.59.6.1.1.5"						//施乐打印机历史作业列表起始oid
#define XeroxHistoryDocNameFormat1OID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRDHQRHYktSHGdaUFwWVUdiVVU=")				//".1.3.6.1.4.1.253.8.51.5.1.1.4.1.%d.106"				//XeroxHistoryJobListBeginOID,Index代表作业ID,作业完成后作业文档名=".1.3.6.1.4.1.253.8.51.5.1.1.4.1.Index.106"
#define XeroxHistoryDocNameFormat151OID		TEXT("H1h9Vk0EfUVPRh0AR2FQUBxrWlRDHQRHYktSHGdaUEcCH0w3S1A=")				//".1.3.6.1.4.1.253.8.51.5.1.1.4.151.%d.3"				//XeroxHistoryJobListBeginOID,Index代表作业ID,作业完成后作业文档名=".1.3.6.1.4.1.253.8.51.5.1.1.4.151.Index.3"

//施乐替换字符串
#define XeroxPrivateEnterprisesOID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdP")			//".iso.org.dod.internet.private.enterprises."
#define XeroxPrivateEnterprises2OID			TEXT("H1h9Vk0EfUVPRh0ARw==")												//".1.3.6.1.4.1."
#define XeroxPrintStatusOID					TEXT("A1xgS1scZk1PRB0AR2JLVg==")											//"253.8.59.6.1.1.5"
#define XeroxPrintStatus2OID				TEXT("A1xgS1scZk1PRB0AR2JLWg==")											//"253.8.59.6.1.1.9"
#define XeroxPrintedPagesOID				TEXT("A1xgS1scZk1PRB0AR2JLVg==")											//"253.8.59.6.1.1.5"
#define XeroxPrintedPages2OID				TEXT("A1xgS1scZk1PQwUfWH1UTQc=")											//"253.8.59.16.1.1.5"
#define XeroxPrintTimeOID					TEXT("A1xgS1scZk1PRB0AR2JLVg==")											//"253.8.59.6.1.1.5"
#define XeroxPrintTime2OID					TEXT("A1xgS1scZk1PQwMfWH1UTQNj")											//"253.8.59.10.1.1.10"

#define XeroxPrintModel						TEXT("H1h9Vk0EfUVPQB0AR2dWTQd9RU9DHQBffVQ=")								//.1.3.6.1.2.1.43.5.1.1.16.1

//施乐HTML分析的数据类型，begin
BEGIN_ENUM_STRUCT(XeroxMarkerSuppliesType)
{
	Other = 0,		//定影器组件、第2偏压转印辊、转印带
	OPC = 1,		//感光鼓
	WasteToner = 2,	//废粉盒
	Toner = 3		//墨粉筒
}
END_ENUM_STRUCT(XeroxMarkerSuppliesType)

//var stsother = ['良好','不可使用','预备备用品','需要更换','品质寿命到期','需要尽快更换','未安装或安装不良(需要重新安装)'];
BEGIN_ENUM_STRUCT(XeroxMarkerSuppliesStatusOther)
{
	Good = 0,				//良好
	UnAvailable = 1,		//不可使用
	PrepareSpares = 2,		//预备备用品
	NeedReplaced = 3,		//需要更换
	QualityLifeDue = 4,		//品质寿命到期
	NeedReplacedSoon = 5,	//需要尽快更换
	BadInstalled = 6		//未安装或安装不良(需要重新安装)
}
END_ENUM_STRUCT(XeroxMarkerSuppliesStatusOther)

//var stsTnr=['良好','感光鼓/墨粉筒异常','需要尽快更换','需要更换','正在设定','未安装或安装不良(需要重新安装)','故障','良好(预备备用品)'];
BEGIN_ENUM_STRUCT(XeroxMarkerSuppliesStatusToner)
{
	Good = 0,				//良好
	Abnormal = 1,			//感光鼓/墨粉筒异常
	NeedReplacedSoon = 2,	//需要尽快更换
	NeedReplaced = 3,		//需要更换
	Setting = 4,			//正在设定
	BadInstalled = 5,		//未安装或安装不良(需要重新安装)
	Fault = 6,				//故障
	GoodAndPrepareSpares = 7	//良好(预备备用品)
}
END_ENUM_STRUCT(XeroxMarkerSuppliesStatusToner)

//var stsTnrBtl=['良好','预备备用品','需要更换','传感器故障','未安装'];
BEGIN_ENUM_STRUCT(XeroxMarkerSuppliesStatusWasteToner)
{
	Good = 0,				//良好
	PrepareSpares = 1,		//预备备用品
	NeedReplaced = 2,		//需要更换
	SensorFault = 3,		//传感器故障
	UnInstalled = 4			//未安装
}
END_ENUM_STRUCT(XeroxMarkerSuppliesStatusWasteToner)

//var stsDrm=['良好','识别符不匹配','感光鼓/墨粉筒异常','预备备用品','需要更换','未安装或安装不良(需要重新安装)','未安装','品质寿命到期','故障','需要尽快更换'];
BEGIN_ENUM_STRUCT(XeroxMarkerSuppliesStatusOPC)
{
	Good = 0,				//良好
	IdentifierNotMatch = 1,	//识别符不匹配
	Abnormal = 2,			//感光鼓/墨粉筒异常
	PrepareSpares = 3,		//预备备用品
	NeedReplaced = 4,		//需要更换
	BadInstalled = 5,		//未安装或安装不良(需要重新安装)
	UnInstalled = 6,		//未安装
	QualityLifeDue = 7,		//品质寿命到期
	Fault = 8,				//故障
	NeedReplacedSoon = 9	//需要尽快更换
}
END_ENUM_STRUCT(XeroxMarkerSuppliesStatusOPC)

typedef struct tag_XeroxMarkerSupplies
{
	ENUM_STRUCT_TYPE(XeroxMarkerSuppliesType) xeroxMarkerSuppliesType;
	char xeroxMarkerSuppliesName[256];
	int xeroxMarkerSuppliesStatus;	//参考XeroxMarkerSuppliesStatusXXX，具体取决于xeroxMarkerSuppliesType
	int xeroxMarkerSuppliesLevel;
}XeroxMarkerSupplies, *PXeroxMarkerSupplies;
typedef map<int, PXeroxMarkerSupplies> XeroxMarkerSuppliesMap;
typedef list<PXeroxMarkerSupplies> XeroxMarkerSuppliesList;

//施乐HTML分析的数据类型，end

#endif	//_XEROX_MIB_DEF_8BE86FA5_B805_43A0_AEDD_681C9DAEDF4A_H_
