#ifndef _HOSTRESOURCES_MIB_DEF_B109E139_5CD2_4691_AF35_9FC39022E86D_H_
#define _HOSTRESOURCES_MIB_DEF_B109E139_5CD2_4691_AF35_9FC39022E86D_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：[iSecPrint\Doc\Snmp\Snmp通用MIB]
	1.HOST-RESOURCES-MIB
	2.HOST-RESOURCES-TYPES
*/
/************************************************************************/

/*
host     OBJECT IDENTIFIER ::= { mib-2 25 }

hrSystem        OBJECT IDENTIFIER ::= { host 1 }
hrStorage       OBJECT IDENTIFIER ::= { host 2 }
hrDevice        OBJECT IDENTIFIER ::= { host 3 }
hrSWRun         OBJECT IDENTIFIER ::= { host 4 }
hrSWRunPerf     OBJECT IDENTIFIER ::= { host 5 }
hrSWInstalled   OBJECT IDENTIFIER ::= { host 6 }
hrMIBAdminInfo  OBJECT IDENTIFIER ::= { host 7 }
*/

#define HrSystemGroupOID				TEXT("WQYgEU1aIScYAUdUBA==")	//"host.hrSystem"
//hrSystem        OBJECT IDENTIFIER ::= { host 1 } 
//".1.3.6.1.2.1.25.1"	//.iso.org.dod.internet.mgmt.mib-2.host.hrSystem
typedef struct tag_HrSystemGroup
{
	TimeTicks hrSystemUptime;						//::= { hrSystem 1 }	//0,只有一个数值
	DateAndTime hrSystemDate[DateAndTime_Size];		//::= { hrSystem 2 }	//0,只有一个数值
	Integer32 hrSystemInitialLoadDevice;			//::= { hrSystem 3 }	//0,只有一个数值
	InternationalDisplayString hrSystemInitialLoadParameters[OCTET_STRING_SIZE_128];		//::= { hrSystem 4 }	//0,只有一个数值
	Gauge32 hrSystemNumUsers;						//::= { hrSystem 5 }	//0,只有一个数值
	Gauge32 hrSystemProcesses;						//::= { hrSystem 6 }	//0,只有一个数值
	Integer32 hrSystemMaxProcesses;					//::= { hrSystem 7 }	//0,只有一个数值
}HrSystemGroup, *PHrSystemGroup;


//HrDeviceTypes是【.1.3.6.1.2.1.25.3.1.x】中的最后一位【x】
BEGIN_ENUM_STRUCT(HrDeviceTypes)
{
	hrDeviceOther = 1,
	hrDeviceUnknown = 2,
	hrDeviceProcessor = 3,
	hrDeviceNetwork = 4,
	hrDevicePrinter = 5,
	hrDeviceDiskStorage = 6,
	hrDeviceVideo = 10,
	hrDeviceAudio = 11,
	hrDeviceCoprocessor = 12,
	hrDeviceKeyboard = 13,
	hrDeviceModem = 14,
	hrDeviceParallelPort = 15,
	hrDevicePointing = 16,
	hrDeviceSerialPort = 17,
	hrDeviceTape = 18,
	hrDeviceClock = 19,
	hrDeviceVolatileMemory = 20,
	hrDeviceNonVolatileMemory = 21
}
END_ENUM_STRUCT(HrDeviceTypes)

BEGIN_ENUM_STRUCT(HrDeviceStatus)
{
	unknown = 1,
	running = 2,
	warning = 3,
	testing = 4,
	down = 5
}
END_ENUM_STRUCT(HrDeviceStatus)

BEGIN_ENUM_STRUCT(HrPrinterStatus)
{
	other = 1,
	unknown = 2,
	idle = 3,
	printing = 4,
	warmup = 5
}
END_ENUM_STRUCT(HrPrinterStatus)


//hrDevice        OBJECT IDENTIFIER ::= { host 3 }
//".1.3.6.1.2.1.25.3"=".iso.org.dod.internet.mgmt.mib-2.host.hrDevice"
#define HrDeviceGroupOID				TEXT("WQYgEU1aITAEBFpSDA==")	//"host.hrDevice"
typedef struct tag_HrDeviceGroup
{
	ENUM_STRUCT_TYPE(HrDeviceTypes) hrDeviceTypes;	//::= { hrDevice 1 }
	DateAndTime hrSystemDate[DateAndTime_Size];		//::= { hrSystem 2 }	//0,只有一个数值
	Integer32 hrSystemInitialLoadDevice;			//::= { hrSystem 3 }	//0,只有一个数值
	InternationalDisplayString hrSystemInitialLoadParameters[InternationalDisplayString_Size];		//::= { hrSystem 4 }	//0,只有一个数值
	Gauge32 hrSystemNumUsers;						//::= { hrSystem 5 }	//0,只有一个数值
	Gauge32 hrSystemProcesses;						//::= { hrSystem 6 }	//0,只有一个数值
	Integer32 hrSystemMaxProcesses;						//::= { hrSystem 7 }	//0,只有一个数值
}HrDeviceGroup, *PHrDeviceGroup;
typedef map<int, PHrDeviceGroup> HrDeviceGroupMap;
typedef list<PHrDeviceGroup> HrDeviceGroupList;

//hrDeviceTable ::= { hrDevice 2 }
//".1.3.6.1.2.1.25.3.2.1"	//.iso.org.dod.internet.mgmt.mib-2.host.hrDevice.hrDeviceTable.hrDeviceEntry
#define HrDeviceEntryOID				TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ0=")						//"host.hrDevice.hrDeviceTable.hrDeviceEntry"
#define HrDeviceIndexOID				TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ1PGkF1DCUMAFcaGgUXSw==")	//"host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceIndex"
#define HrDeviceTypeOID					TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ1PGkF1DCUMAFcHDREX")		//"host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceType"
#define HrDeviceDescrOID				TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ1PGkF1DCUMAFcXERIRQQ==")	//"host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceDescr"
#define HrDeviceIDOID					TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ1PGkF1DCUMAFcaMA==")		//"host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceID"
#define HrDeviceStatusOID				TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ1PGkF1DCUMAFcAAAAGRkI=")	//"host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceStatus"
#define HrDeviceErrorsOID				TEXT("WQYgEU1aITAEBFpSDH0NEXY2AggRVmUIMQkGHDsGJRdFWAo2IA1GIQ1PGkF1DCUMAFcWBhMdQUI=")	//"host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceErrors"
#define HrDeviceTypePrefixOID			TEXT("H1h9Vk0EfUVPQB0AR2FQTQF9RQ==")	//".1.3.6.1.2.1.25.3.1" //HrDeviceTypes是【.1.3.6.1.2.1.25.3.1.x】中的最后一位【x】
typedef struct tag_HrDeviceEntry
{
	Integer32 hrDeviceIndex;								//::= { hrDeviceEntry 1 }
	ENUM_STRUCT_TYPE(HrDeviceTypes) hrDeviceType;			//::= { hrDeviceEntry 2 }
	DisplayString hrDeviceDescr[OCTET_STRING_SIZE_64];		//::= { hrDeviceEntry 3 }
	OBJECT_IDENTIFIER hrDeviceID[OBJECT_IDENTIFIER_SIZE];	//::= { hrDeviceEntry 4 }
	ENUM_STRUCT_TYPE(HrDeviceStatus) hrDeviceStatus;		//::= { hrDeviceEntry 5 }
	Counter32 hrDeviceErrors;								//::= { hrDeviceEntry 6 }
}HrDeviceEntry, *PHrDeviceEntry;
typedef map<int, PHrDeviceEntry> HrDeviceEntryMap;
typedef list<PHrDeviceEntry> HrDeviceEntryList;


//hrProcessorTable ::= { hrDevice 3 }
//".1.3.6.1.2.1.25.3.3.1"	//.iso.org.dod.internet.mgmt.mib-2.host.hrDevice.hrProcessorTable.hrProcessorEntry
#define HrProcessorEntryOID				TEXT("WQYgEU1aITAEBFpSDH0NEWIhGwIXQEIGITECUD8RTxpBYRs8BgZBIBsTN11FGyo=")							//"host.hrDevice.hrProcessorTable.hrProcessorEntry"
#define HrProcessorFrwIDOID				TEXT("WQYgEU1aITAEBFpSDH0NEWIhGwIXQEIGITECUD8RTxpBYRs8BgZBIBsTN11FGypLC0ADBg4RVkIaPBclQCQ9JQ==")	//"host.hrDevice.hrProcessorTable.hrProcessorEntry.hrProcessorFrwID"
#define hrProcessorLoadOID				TEXT("WQYgEU1aITAEBFpSDH0NEWIhGwIXQEIGITECUD8RTxpBYRs8BgZBIBsTN11FGypLC0ADBg4RVkIaPBcvXTIQ")		//"host.hrDevice.hrProcessorTable.hrProcessorEntry.hrProcessorLoad"
typedef struct tag_HrProcessorEntry
{
	OBJECT_IDENTIFIER hrProcessorFrwID[OBJECT_IDENTIFIER_SIZE];	//::= { hrProcessorEntry 1 }
	Integer32 hrProcessorLoad;									//::= { hrProcessorEntry 2 }
}HrProcessorEntry, *PHrProcessorEntry;
typedef map<int, PHrProcessorEntry> HrProcessorEntryMap;
typedef list<PHrProcessorEntry> HrProcessorEntryList;


//hrPrinterTable ::= { hrDevice 5 }
//".1.3.6.1.2.1.25.3.5.1"	//.iso.org.dod.internet.mgmt.mib-2.host.hrDevice.hrPrinterTable.hrPrinterEntry
#define HrPrinterEntryOID				TEXT("WQYgEU1aITAEBFpSDH0NEWIhHQ8GVkM9MgcPV30cEyJBWAcnABF3PQATCw==")										//"host.hrDevice.hrPrinterTable.hrPrinterEntry"
#define HrPrinterStatusOID				TEXT("WQYgEU1aITAEBFpSDH0NEWIhHQ8GVkM9MgcPV30cEyJBWAcnABF3PQATCx1ZGwMXClwnERMhR1AdJhY=")					//"host.hrDevice.hrPrinterTable.hrPrinterEntry.hrPrinterStatus"
#define HrPrinterDetectedErrorStateOID	TEXT("WQYgEU1aITAEBFpSDH0NEWIhHQ8GVkM9MgcPV30cEyJBWAcnABF3PQATCx1ZGwMXClwnERM2VkUMMBEGVhYGEx1BYh0yEQY=")	//"host.hrDevice.hrPrinterTable.hrPrinterEntry.hrPrinterDetectedErrorState"
typedef struct tag_HrPrinterEntry
{
	ENUM_STRUCT_TYPE(HrPrinterStatus) hrPrinterStatus;			//::= { hrPrinterEntry 1 }
	OBJECT_BITS hrPrinterDetectedErrorState;					//::= { hrPrinterEntry 2 }
}HrPrinterEntry, *PHrPrinterEntry;
typedef map<int, PHrPrinterEntry> HrPrinterEntryMap;
typedef list<PHrPrinterEntry> HrPrinterEntryList;

#endif	//_HOSTRESOURCES_MIB_DEF_B109E139_5CD2_4691_AF35_9FC39022E86D_H_
