#ifndef _KonicaMinolta_MIB_DEF_6988E2BA_7E3C_48a1_9AA5_5EF4FFD97063_H_
#define _KonicaMinolta_MIB_DEF_6988E2BA_7E3C_48a1_9AA5_5EF4FFD97063_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：
	1.KM-MIB.pdf
	2.KM-MIB2.pdf
	3.KM-MIB-3.pdf
	4.Job_MIB.pdf
*/
/************************************************************************/

//柯美MIB Version
#define KmJmMibVersionGroupOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQwsCWmdLUhxiWlVcAh9Y")		//".iso.org.dod.internet.private.enterprises.18334.1.1.4.1.1"
typedef struct tag_KmJmMibVersionGroup	
{
	DisplayString kmJmMibVersion[OCTET_STRING_SIZE_8];			//只有0,KmJmMibVersionGroupOID.0
}KmJmMibVersionGroup, *PKmJmMibVersionGroup;
typedef map<int, PKmJmMibVersionGroup> KmJmMibVersionGroupMap;
typedef list<PKmJmMibVersionGroup> KmJmMibVersionGroupList; 

BEGIN_ENUM_STRUCT(KmJmGeneralJobSetType)
{
	mfpAll = 0,
	copy = 1,
	print = 2,
	receiveJob = 3,
	sendJob = 4,
	faxReceive = 5,
	faxSend = 6,
	other = 7
}
END_ENUM_STRUCT(KmJmGeneralJobSetType)

//柯美作业信息概要,KmJmGeneralEntry,本质应该是Group,一般只有一组数据，kmJmGeneralJobSetIndex只有1
#define KmJmGeneralEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQwsCWmdLUhxiWlVcAh9bfVRNAw==")	//".iso.org.dod.internet.private.enterprises.18334.1.1.4.1.2.1.1"
typedef struct tag_KmJmGeneralEntry	
{
	Integer32 kmJmGeneralJobSetIndex;			//只有1
	Integer32 kmJmGeneralNumberOfActiveJobs;	//the number of ActiveJob
	Integer32 kmJmGeneralOldestActiveJobIndex;	//JobID, This value is the oldest kmJmJobIndex of ActiveJob in kmJmJobTable.
	Integer32 kmJmGeneralNewestActiveJobIndex;	//JobID, This value is the last received of	ActiveJob in kmJmJobTable.
	Integer32 kmJmGeneralJobPersistence;		//100,保留作业个数
	ENUM_STRUCT_TYPE(KmJmGeneralJobSetType) kmJmGeneralJobSetType;	
	Integer32 kmJmGeneralJobFinishTimeOfActiveJobs;			//0 or -2，-2: unknown
}KmJmGeneralEntry, *PKmJmGeneralEntry;
typedef map<int, PKmJmGeneralEntry> KmJmGeneralEntryMap;
typedef list<PKmJmGeneralEntry> KmJmGeneralEntryList;


BEGIN_ENUM_STRUCT(KmJmJobFunction)
{
	unknown = 0,
	copy = 1,
	print = 2,
	receiveJob = 3,
	sendJob = 4,
	faxReceive = 5,
	faxSend = 6,
	other = 7,
	multi = 10
}
END_ENUM_STRUCT(KmJmJobFunction)

BEGIN_ENUM_STRUCT(KmJmJobStatus)
{
	unknown = 0,
	other = 1,
	cancelRequest = 2,
	pauseRequest = 3,
	restartRequest = 4,
	pending = 10,
	pendingHeld = 11,
	pendingPaused = 12,
	processing = 20,
	sending = 21,
	receiving = 22,
	printing = 23,
	scanning = 24,
	procesingStopped = 25,
	completed = 30,
	caution = 31,
	aborted = 32,
	canceled = 33,
	sendCompleted = 34	//When data is transmitted from the scanning server, 
						//it becomes it while processing it.
}
END_ENUM_STRUCT(KmJmJobStatus)

BEGIN_ENUM_STRUCT(KmJmJobPriority)
{
	notSupported = 0,		//When priority control is not supported. 
	lowestPriority = 1,		//Lowest Priority (it is not usually used) 
	lowPriority = 2,
	normalPriority = 3,
	highPriority = 4,
	highestPriority = 5
}
END_ENUM_STRUCT(KmJmJobPriority)

//Excluding the Secret Print and the
//Print to HDD, all Print Jobs are
//designed as "normalPrint".
//Excluding the Copy to HDD , all Copy
//Jobs are designed as "Copy".
//When doing scan operation with the
//boxStore, "Copy(40)" is indicated.
//When the data having received with
//the printToHDD, "normalPrint(20)" is
//indicated.
BEGIN_ENUM_STRUCT(KmJmJobType)
{
	other = 0,
	scanSend = 10,
	email = 11,
	iFaxSend = 12,
	scanToServer = 13,
	scanToPC = 14,
	scanToHDD = 15,
	twain = 16,
	faxSend = 17,
	pcFaxSend = 18,
	batchSend = 19,
	normalPrint = 20,
	ippPrint = 21,
	securePrint = 22,
	timerPrint = 23,
	printToHDD = 24,
	proofAndPrint = 25,
	proofAndRestoreSend = 26,	//Proof  and  restore  printing  (Restore sending)
	bindPrint = 27,
	bulletinPrint = 28,
	reportPrint = 29,
	iFaxReceive = 31,
	FaxReceive = 32,
	faxPollingRecieve = 33,		//Polling receiving
	faxForwardingReceive = 35,	//Forward receiving
	faxRelayReceive = 36,	//Relay receiving 
	faxForwardingSend = 37,	//Forwarding 
	faxRelaySend = 38,		//Relay 
	faxRestoreSend = 39,	//Restore sending 
	copy = 40,
	boxPrint = 51,			//Box printing
	boxStore = 60,			//Box Restore
	pollingSend = 71,		
	pollingSendPrint = 72,	
	pollingSendBulletin = 73,	
	pollingSendStore = 74,	
	sipFaxSend = 80,		//SIP-FAX sending
	sipFaxReceive = 81,		//SIP-FAX receiving
	ipFaxSend = 90,			//IP-FAX sending
	ipFaxReceive = 91,		//IP-FAX receiving
	scanToUSB = 100,
	usbPrint = 101,
	scanToWebDAV = 110,
	scanServer = 111		//DistributedScan (Using WSD-RPWS) 
}
END_ENUM_STRUCT(KmJmJobType)

//Papers other than the size written are
//set to "1 (other)".
//Fax receive jobs return "2 (unknown)".
//History jobs return "2 (unknown)".
//When setting "A6Card" from Panel,
//262(A6) is returned.
//When setting "12_1/4x18" from
//Panel, 1(other) is returned.
//Transfer Fax Jobs or Relay Fax Jobs, R
BEGIN_ENUM_STRUCT(KmJmJobPageSize)
{
	other = 1,
	unknown = 2,
	A3 = 259,
	A4 = 260,
	A5 = 261,
	A6 = 262,
	B4 = 516,
	B5 = 517,
	B6 = 518,
	ISO_B5 = 773,
	C5 = 1029,
	Foolscap = 1285,	//220x330
	Size_12_18 = 1540,	//12x18
	Ledger = 1541,		//11x17
	Legal = 1547,		//8_1/2x14
	Foolscap2 = 1548,	//8_1/2x13_1/2
	Foolscap3 = 1549,	//8_1/2x13
	Letter = 1551,		//8_1/2x11
	Foolscap4 = 1553,	//8_1/4x13
	Foolscap5 = 1554,	//8_1/8x13_1/4
	Foolscap6 = 1556,	//8x13
	Executive = 1559,	//7_1/4x10_1/2
	Invoice = 1560,		//5_1/2x8_1/2
	Size_8K = 2051,
	Size_16K = 2052,
	SRA3 = 2323,
	cyoukei3 = 2562,
	choukei4 = 2564,
	kakukei2 = 2578,
	kakukei20 = 2579,	//C4
	kakukei3 = 2580,
	youkei2 = 2593,		//C6
	youkei4 = 2594,
	kakukei1 = 2596,
	youkei3 = 2597,
	DL = 2826,			//designed long size
	Monarch_Envelope = 3073,
	Commercial_10 = 3074,	//Commercial #10
	JapanesePostCard = 3587,
	PostCard = 3602,	//4x6 
	A3Wide = 8451,
	A4Wide = 8452,
	A5Wide = 8453,
	B4Wide = 8708,
	B5Wide = 8709,
	Wide = 9733,		//11x17
	Wide2 = 9743,		//8_1/2x11
	Wide3 = 9752		//5_1/2x8_1/2
}
END_ENUM_STRUCT(KmJmJobPageSize)

BEGIN_ENUM_STRUCT(KmJmJobOutputMode)
{
	other = 0,
	unknown = 1,

	//Send Job
	pageDivision = 11,
	binaryDivision = 12,
	urlNotification = 13,

	//Print/Copy Job
	normalPrint = 21,
	electricSort = 22,
	securePrint = 23,

	//Receive Job
	receivePrint = 31,
	store = 32,
	forwarding = 33
}
END_ENUM_STRUCT(KmJmJobOutputMode)

BEGIN_ENUM_STRUCT(KmJmJobColorMode)
{
	other = 0,
	unknown = 1,
	color = 2,
	grayscale = 3,
	blackWhite = 4
}
END_ENUM_STRUCT(KmJmJobColorMode)

BEGIN_ENUM_STRUCT(KmJmJobFormat)
{
	other = 0,
	unknown = 1,
	raw = 2,
	Tiff = 3,
	Tiff_MH = 4,	//Tiff-MH
	Tiff_MR = 5,	//Tiff-MR
	Tiff_MMR = 6,	//Tiff-MMR
	jpeg = 7,
	Pdf = 8,
	Pcl = 9,
	postScript = 10,
	Gdi = 11,
	linePrinter = 12,
	XPS = 13,
	CompactXPS = 14,
	CompactPDF = 15,
	OutlinePDF = 16
}
END_ENUM_STRUCT(KmJmJobFormat)

BEGIN_ENUM_STRUCT(KmJmJobDuplex)
{
	On = 1,
	Off = 2,
}
END_ENUM_STRUCT(KmJmJobDuplex)

//柯美历史作业信息详情
#define KmJmJobEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQwsCWmdLUhxiWlVcAh9afVRNAw==")	//".iso.org.dod.internet.private.enterprises.18334.1.1.4.1.3.1.1"
typedef struct tag_KmJmJobEntry
{
	Integer32 kmJmJobJobSetIndex;								//1,This value is kmJmGeneralJobSetIndex.
	Integer32 kmJmJobIndex;										//JobID
	ENUM_STRUCT_TYPE(KmJmJobFunction) kmJmJobFunction;	
	ENUM_STRUCT_TYPE(KmJmJobStatus) kmJmJobStatus;
	OCTET_STRING kmJmJobStatusDetails[OCTET_STRING_SIZE_64];	//Active job returns prtAlertLocation.
	DateAndTime kmJmJobReceivedTime[DateAndTime_Size];			//Time
	DateAndTime kmJmJobCompleteTime[DateAndTime_Size];			//Time,ActiveJob is returned "NULL".
	ENUM_STRUCT_TYPE(KmJmJobPriority) kmJmJobPriority;
	OCTET_STRING kmJmJobDivNumber[OCTET_STRING_SIZE_8];			//JobDivNumber,When "Hide Personal Information(MIB)" is turned off,
																//"ActiveJob" and "HistoryJob" are displayed.
	OCTET_STRING kmJmJobOwner[OCTET_STRING_SIZE_64];			//JobOwner,When "Hide Personal Information(MIB)" is turned off,
																//"ActiveJob" and "HistoryJob" are displayed.
	ENUM_STRUCT_TYPE(KmJmJobType) kmJmJobType;
	OCTET_STRING kmJmJobName[OCTET_STRING_SIZE_256];			//Job name,This value is same Job name in IJC Job History.
																//When "Hide Personal Information(MIB)" is turned off, 
																//"ActiveJob" and "HistoryJob" are displayed.
																//When the other cases,NULL is returned.
	Integer32 kmJmJobDocPageNumbers;							//Job Pages,-2(unknown)
	Integer32 kmJmJobDocCopyNumbers;							//Copies,-2(unknown)
	ENUM_STRUCT_TYPE(KmJmJobPageSize) kmJmJobPageSize;
	OCTET_STRING kmJmJobDestination[OCTET_STRING_SIZE_256];		//-----Send Job------
																// (E-mail/iFax) Mail address
																// (File FTP) IP or Host address
																// (File PC) IP or Host address
																// (File HDD)
																// User Box number
																// (Twain) IP or Host address
																// (FAX) FAX number
																// (IP-FAX) IP or Host address
																// (SIP-FAX) FAX number
																// (Scan Server)IP or Host
																// address
																// -----Print Job-----
																// Main Tray
																// Sub Tray
																// Additional Tray
																// Saddle Tray

	OCTET_STRING kmJmJobDriverJobID[OCTET_STRING_SIZE_32];		//Null
	ENUM_STRUCT_TYPE(KmJmJobOutputMode) kmJmJobOutputMode;
	Integer32 kmJmJobDataSize;							
	ENUM_STRUCT_TYPE(KmJmJobColorMode) kmJmJobColorMode;
	ENUM_STRUCT_TYPE(KmJmJobFormat) kmJmJobFormat;
	Integer32 kmJmJobXResolution;							
	Integer32 kmJmJobYResolution;							
	ENUM_STRUCT_TYPE(KmJmJobDuplex) kmJmJobDuplex;
	OCTET_STRING kmJmJobFinishing[OCTET_STRING_SIZE_16];
	Integer32 kmJmJobOutputPages;							
	Integer32 kmJmJobOutputSheets;							
	Integer32 kmJmJobMediaType;							
	Integer32 kmJmJobPaperAttribute;							
}KmJmJobEntry, *PKmJmJobEntry;
typedef map<int, PKmJmJobEntry> KmJmJobEntryMap;
typedef list<PKmJmJobEntry> KmJmJobEntryList;


//抄表节点OID
#define KmA4CopyHBPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1IcYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.1.1"
#define KmA4CopyFullColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1EcYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.2.1"
#define KmA4CopySingleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1AcYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.3.1"
#define KmA4CopyDoubleColorPage			TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1ccYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.4.1"
#define KmA3CopyHBPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1IcYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.1.1"
#define KmA3CopyFullColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1EcYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.2.1"
#define KmA3CopySingleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1AcYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.3.1"
#define KmA3CopyDoubleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1ccYg==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.4.1"
#define KmA4PrintHBPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1IcYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.1.2"
#define KmA4PrintFullColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1EcYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.2.2"
#define KmA4PrintSingleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1AcYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.3.2"
#define KmA4PrintDoubleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdmS1ccYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.5.4.2"
#define KmA3PrintHBPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1IcYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.1.2"
#define KmA3PrintFullColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1EcYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.2.2"
#define KmA3PrintSingleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1AcYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.3.2"
#define KmA3PrintDoubleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0AdAEdkS1ccYQ==")	//".1.3.6.1.4.1.18334.1.1.1.5.7.2.2.1.7.4.2"
#define KmScanTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0EdAEdmS1I=")		//".1.3.6.1.4.1.18334.1.1.1.5.7.2.3.1.5.1"
#define KmScanBigColorPageOID			TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0EdAEdlS1I=")		//".1.3.6.1.4.1.18334.1.1.1.5.7.2.3.1.6.1"
#define KmTotalPageCountOID				TEXT("H1h9Vk0EfUVPRh0AR2JdUAFnWlBcAh9YfVBNBX1GT0MdCUdj")			//".1.3.6.1.4.1.18334.1.1.1.5.7.2.1.8.0"

#endif	//_KonicaMinolta_MIB_DEF_6988E2BA_7E3C_48a1_9AA5_5EF4FFD97063_H_
