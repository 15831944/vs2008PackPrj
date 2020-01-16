#ifndef _LEXMARK_MIB_DEF_7C51C662_16E8_46c7_8150_3AD782FE12F0_H_
#define _LEXMARK_MIB_DEF_7C51C662_16E8_46c7_8150_3AD782FE12F0_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：
	1.lexmark1.mib
	2.lexmark-mps-mib.mib
	3.lexmark-root-mib.mib
	4.lexmark-tc-mib.mib
*/
/************************************************************************/

BEGIN_ENUM_STRUCT(GenCountType)
{
	porCount = 1,
	sleepCount = 2,
	hibernateCount = 3,
	printCalibrateCount = 4,
	powerOnTime = 32,
	powerActiveTime = 33,
	powerIdleTime = 34,
	powerSleepTime = 35,
	powerHibernateTime = 36,
	powerOffTime = 37,
	warmupTotalTime = 38,
	lifetimeBlackCoverage = 64,
	lifetimeCyanCoverage = 65,
	lifetimeYellowCoverage = 66,
	lifetimeMagentaCoverage = 67,
	faxesSent = 96,
	paperJams = 97,
	scannerJams = 98,
	loadPaperPrompts = 99,
	changePaperPrompts = 100,
	coverOpens = 101,
	usbInsertions = 128
}
END_ENUM_STRUCT(GenCountType)


BEGIN_ENUM_STRUCT(PaperGeneralCountType)
{
	totalPicked = 1,
	totalSafe = 2,
	totalMonoSafe = 3,
	totalColorSafe = 4,
	printNHold = 5,
	usbDirect = 6,
	printTotal = 16,
	printMono = 17,
	printColor = 18,
	copyTotal = 32,
	copyMono = 33,
	copyColor = 34,
	faxTotal = 48,
	faxMono = 49,
	faxColor = 50,
	blankTotal = 64,
	blankPrint = 65,
	blankCopy = 66,
	blankFax = 67,
	printerPageCount = 80,
	modularPageCount = 81
}
END_ENUM_STRUCT(PaperGeneralCountType)

BEGIN_ENUM_STRUCT(UnitsTC)
{
	unknown = 1,
	other = 2,

	items = 3,
	sides = 4,
	sheets = 5,

	millimeters = 16,
	centimeters = 17,
	meters = 18,
	inches = 19,
	feet = 20,
	grams = 21,
	ounces = 22,

	nanoseconds = 32,
	microseconds = 33,
	milliseconds = 34,
	seconds = 35,
	minutes = 36,
	hours = 37,
	days = 38,
	weeks = 39,
	months = 40,
	years = 41
}
END_ENUM_STRUCT(UnitsTC)

BEGIN_ENUM_STRUCT(PaperSizeTC)
{
	unknown = 1,
	other = 2,
	universal = 3,
	custom = 4,

	letter = 8,
	legal = 9,
	executive = 10,
	folio = 11,
	statement = 12,
	oficio = 13,
	tabloid = 14,
	businessCard = 15,
	idCard = 16,
	card3x5 = 17,
	card4x6 = 18,
	bookOriginal = 19,
	hagaki = 20,



	envelope7threequarters = 32,
	envelope9 = 33,
	envelope10 = 34,
	envelopeDL = 35,
	envelopeOther = 36,

	isoA0 = 64,
	isoA1 = 65,
	isoA2 = 66,
	isoA3 = 67,
	isoA4 = 68,
	isoA5 = 69,
	isoA6 = 70,

	isoB0 = 72,
	isoB1 = 73,
	isoB2 = 74,
	isoB3 = 75,
	isoB4 = 76,
	isoB5 = 77,
	isoB6 = 78,

	isoC0 = 80,
	isoC1 = 81,
	isoC2 = 82,
	isoC3 = 83,
	isoC4 = 84,
	isoC5 = 85,
	isoC6 = 86,

	isoEnvelopeA0 = 96,
	isoEnvelopeA1 = 97,
	isoEnvelopeA2 = 98,
	isoEnvelopeA3 = 99,
	isoEnvelopeA4 = 100,
	isoEnvelopeA5 = 101,
	isoEnvelopeA6 = 102,

	isoEnvelopeB0 = 104,
	isoEnvelopeB1 = 105,
	isoEnvelopeB2 = 106,
	isoEnvelopeB3 = 107,
	isoEnvelopeB4 = 108,
	isoEnvelopeB5 = 109,
	isoEnvelopeB6 = 110,

	isoEnvelopeC0 = 112,
	isoEnvelopeC1 = 113,
	isoEnvelopeC2 = 114,
	isoEnvelopeC3 = 115,
	isoEnvelopeC4 = 116,
	isoEnvelopeC5 = 117,
	isoEnvelopeC6 = 118,

	jisB0 = 136,
	jisB1 = 137,
	jisB2 = 138,
	jisB3 = 139,
	jisB4 = 140,
	jisB5 = 141,
	jisB6 = 142
}
END_ENUM_STRUCT(PaperSizeTC)

BEGIN_ENUM_STRUCT(PaperTypeTC)
{
	unknown = 1,
	other = 2,
	plain = 3,
	cardstock = 4,
	transparancy = 5,
	recycled = 6,
	labels = 7,
	vinylLabels = 8,
	bond = 9,
	letterhead = 10,
	preprinted = 11,
	colored = 12,
	light = 13,
	heavy = 14,
	roughOrCotton = 15,
	envelope = 16,
	customtype1 = 32,
	customtype2 = 33,
	customtype3 = 34,
	customtype4 = 35,
	customtype5 = 36,
	customtype6 = 37
}
END_ENUM_STRUCT(PaperTypeTC)

BEGIN_ENUM_STRUCT(PaperNupNumber)
{
	off = 1,
	twoUp = 2,
	threeUp = 3,
	fourUp = 4,
	sixUp = 6,
	nineUp = 9,
	twelveUp = 12,
	sixteenUp = 16
}
END_ENUM_STRUCT(PaperNupNumber)

BEGIN_ENUM_STRUCT(ScanCountType)
{
	copyAdf = 1,
	faxAdf = 2,
	scanToEmailAdf = 3,
	scanToNetAdf = 4,
	scanToLocalHostAdf = 5,
	copyFlatbed = 257, 
	faxFlatbed = 258, 
	scanToEmailFlatbed = 259, 
	scanToNetFlatbed = 260, 
	scanToLocalHostFlatbed = 261, 
	copyDuplex = 769, 
	faxDuplex = 770, 
	scanToEmailDuplex = 771, 
	scanToNetDuplex = 772, 
	scanToLocalHostDuplex = 773
}
END_ENUM_STRUCT(ScanCountType)

BEGIN_ENUM_STRUCT(SupplyTypeTC)
{
	unknown = 1,
	other = 2,
	inkCartridge = 3,
	inkBottle = 4,
	inkPrinthead = 5,
	toner = 6,
	photoconductor = 7,
	transferModule = 8,
	fuser = 9,
	wastetonerBox = 10,
	staples = 11,
	holepunchBox = 12
}
END_ENUM_STRUCT(SupplyTypeTC)

BEGIN_ENUM_STRUCT(CartridgeTypeTC)
{
	unknown = 1,
	other = 2,
	invalid = 3,
	shipWith = 4,

	standard = 5,
	highYieldStandard = 6,
	extraHighYieldStandard = 7,

	returnProgram = 21,                       // 16 + 5
	highYieldReturnProgram = 22,              // 16 + 6
	extraHighYieldReturnProgram = 23,         // 16 + 7

	refilledStandard = 37,                    // 32 + 5
	refilledHighYieldStandard = 38,           // 32 + 6
	refilledExtraHighYieldStandard = 39,      // 32 + 7

	refilledReturnProgram = 53,               // 16 + 32 + 5
	refilledHighYieldReturnProgram = 54,      // 16 + 32 + 6
	refilledExtraHighYieldReturnProgram = 55  // 16 + 32 + 7
}
END_ENUM_STRUCT(CartridgeTypeTC)

BEGIN_ENUM_STRUCT(CurrentSupplyCurrentStatus)
{
	other = 1,
	unknown = 2,
	ok = 3,
	low = 4,
	empty = 5,
	invalid = 6
}
END_ENUM_STRUCT(CurrentSupplyCurrentStatus)

BEGIN_ENUM_STRUCT(AdminStatusTC)
{
	unknown = 1,
	other = 3,
	up = 4,
	disabled = 5
}
END_ENUM_STRUCT(AdminStatusTC)

BEGIN_ENUM_STRUCT(StatusTC)
{
	unknown = 1,
	other = 2,
	ok = 3,
	offline = 4,
	warning = 5,
	broken = 6,

	disabledUnknown = 17,    // 16 + 1
	disabledOther = 18,      // 16 + 2
	disabledOk = 19,         // 16 + 3
	disabledOffline = 20,    // 16 + 4
	disabledWarning = 21,    // 16 + 5
	disabledBroken = 22,     // 16 + 6

	unlicensedUnknown = 33,  // 32 + 1
	unlicensedOther = 34,    // 32 + 2
	unlicensedOk = 35,       // 32 + 3
	unlicensedOffline = 36,  // 32 + 4
	unlicensedWarning = 37,  // 32 + 5
	unlicensedBroken = 38,   // 32 + 6

	licensedUnknown = 97,    // 64 + 32 + 1
	licensedOther = 98,      // 64 + 32 + 2
	licensedOk = 99,         // 64 + 32 + 3
	licensedOffline = 100,   // 64 + 32 + 4
	licensedWarning = 101,   // 64 + 32 + 5
	licensedBroken = 102     // 64 + 32 + 6
}
END_ENUM_STRUCT(StatusTC)

typedef OCTET_STRING KeyValueTC;		//0-255


#define GenCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxiWlBcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.1.1.1"
typedef struct tag_GenCountEntry
{
	Integer32 genCountIndex;
	ENUM_STRUCT_TYPE(GenCountType) genCountType;
	ENUM_STRUCT_TYPE(UnitsTC) genCountUnits;
	Counter32 genCountValue;
}GenCountEntry, *PGenCountEntry;
typedef map<int, PGenCountEntry> GenCountEntryMap;
typedef list<PGenCountEntry> GenCountEntryList;

//打印、复印、传真总计数
#define PaperGeneralCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxhWlBcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.2.1.1"
typedef struct tag_PaperGeneralCountEntry
{
	Integer32 paperGeneralCountIndex;
	ENUM_STRUCT_TYPE(PaperGeneralCountType) paperGeneralCountType;
	ENUM_STRUCT_TYPE(UnitsTC) paperGeneralCountUnits;
	Counter32 paperGeneralCountValue;
}PaperGeneralCountEntry, *PPaperGeneralCountEntry;
typedef map<int, PPaperGeneralCountEntry> PaperGeneralCountEntryMap;
typedef list<PPaperGeneralCountEntry> PaperGeneralCountEntryList;


//打印面计数，区分纸张大小和纸张类型
#define PaperSidesCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxhWlNcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.2.2.1"
typedef struct tag_PaperSidesCountEntry
{
	Integer32 paperSidesCountIndex;
	ENUM_STRUCT_TYPE(PaperSizeTC) paperSidesPaperSize;
	ENUM_STRUCT_TYPE(PaperTypeTC) paperSidesPaperType;
	Counter32 paperSidesMonoPicked;
	Counter32 paperSidesColorPicked;
	Counter32 paperSidesMonoSafe;
	Counter32 paperSidesColorSafe;
}PaperSidesCountEntry, *PPaperSidesCountEntry;
typedef map<int, PPaperSidesCountEntry> PaperSidesCountEntryMap;
typedef list<PPaperSidesCountEntry> PaperSidesCountEntryList;


//打印页（一般情况1页=2面）计数，区分纸张大小和纸张类型
#define PaperSheetsCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxhWlJcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.2.3.1"
typedef struct tag_PaperSheetsCountEntry
{
	Integer32 paperSheetsCountIndex;
	ENUM_STRUCT_TYPE(PaperSizeTC) paperSheetsPaperSize;
	ENUM_STRUCT_TYPE(PaperTypeTC) paperSheetsPaperType;
	Counter32 paperSheetsPicked;
	Counter32 paperSheetsSafe;
}PaperSheetsCountEntry, *PPaperSheetsCountEntry;
typedef map<int, PPaperSheetsCountEntry> PaperSheetsCountEntryMap;
typedef list<PPaperSheetsCountEntry> PaperSheetsCountEntryList;


#define PaperNupCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxhWlVcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.2.4.1"
typedef struct tag_PaperNupCountEntry
{
	Integer32 paperNupCountIndex;
	Integer32 paperNupNumber;
	Counter32 paperNupSides;
	Counter32 paperNupLogicalSides;
}PaperNupCountEntry, *PPaperNupCountEntry;
typedef map<int, PPaperNupCountEntry> PaperNupCountEntryMap;
typedef list<PPaperNupCountEntry> PaperNupCountEntryList;


#define PaperJobSizeEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxhWlRcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.2.5.1"
typedef struct tag_PaperJobSizeEntry
{
	Integer32 paperJobSizeIndex;
	Integer32 paperJobSizeMinimum;
	Integer32 paperJobSizeMaximum;
	Counter32 paperJobSizeSideCount;
	Counter32 paperJobSizeJobCount;
}PaperJobSizeEntry, *PPaperJobSizeEntry;
typedef map<int, PPaperJobSizeEntry> PaperJobSizeEntryMap;
typedef list<PPaperJobSizeEntry> PaperJobSizeEntryList;


//扫描计数，区分纸张大小和扫描类型
#define ScanCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxgWlBcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.3.1.1"
typedef struct tag_ScanCountEntry
{
	Integer32 scanCountIndex;
	ENUM_STRUCT_TYPE(ScanCountType) scanCountType;
	ENUM_STRUCT_TYPE(PaperSizeTC) scanCountSize;
	Counter32 scanCountSides;
	Counter32 scanCountSheets;
}ScanCountEntry, *PScanCountEntry;
typedef map<int, PScanCountEntry> ScanCountEntryMap;
typedef list<PScanCountEntry> ScanCountEntryList;


#define CurrentSuppliesEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxnWlBcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.4.1.1"
typedef struct tag_CurrentSuppliesEntry
{
	Integer32 currentSupplyIndex;
	Integer32 currentSupplyInventoryIndex;
	ENUM_STRUCT_TYPE(SupplyTypeTC) currentSupplyType;
	DisplayString currentSupplyColorantValue[OCTET_STRING_SIZE_256];
	SnmpAdminString	currentSupplyDescription[OCTET_STRING_SIZE_256];
	DisplayString currentSupplySerialNumber[OCTET_STRING_SIZE_256];
	DisplayString currentSupplyPartNumber[OCTET_STRING_SIZE_256];
	Integer32 currentSupplyClass;
	ENUM_STRUCT_TYPE(CartridgeTypeTC) currentSupplyCartridgeType;
	DateAndTime	currentSupplyInstallDate[DateAndTime_Size];
	Counter32 currentSupplyPageCountAtInstall;
	Integer32 currentSupplyCurrentStatus;
	ENUM_STRUCT_TYPE(UnitsTC) currentSupplyCapacityUnit;
	Integer32 currentSupplyCapacity;
	Counter32 currentSupplyFirstKnownLevel;
	Counter32 currentSupplyCurrentLevel;
	Counter32 currentSupplyUsage;
	Counter32 currentSupplyCalibrations;
	Counter32 currentSupplyCoverage;
}CurrentSuppliesEntry, *PCurrentSuppliesEntry;
typedef map<int, PCurrentSuppliesEntry> CurrentSuppliesEntryMap;
typedef list<PCurrentSuppliesEntry> CurrentSuppliesEntryList;


#define SupplyHistoryEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxnWlNcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.4.2.1"
typedef struct tag_SupplyHistoryEntry
{
	Integer32 supplyHistoryIndex;
	Integer32 supplyHistoryInventoryIndex;
	ENUM_STRUCT_TYPE(SupplyTypeTC) supplyHistorySupplyType;
	DisplayString supplyHistoryColorantValue[OCTET_STRING_SIZE_256];
	SnmpAdminString supplyHistoryDescription[OCTET_STRING_SIZE_256];
	DisplayString supplyHistorySerialNumber[OCTET_STRING_SIZE_256];
	ENUM_STRUCT_TYPE(CartridgeTypeTC) supplyHistoryCartridgeType;
	DateAndTime supplyHistoryInstallDate[DateAndTime_Size];
	Counter32 supplyHistoryPageCount;
	Integer32 supplyHistoryCapacityUnit;
	Integer32 supplyHistoryCapacity;
	Counter32 supplyHistoryLastLevel;
	Counter32 supplyHistoryUsage;
	Counter32 supplyHistoryCalibrations;
	Counter32 supplyHistoryCoverage;
}SupplyHistoryEntry , *PSupplyHistoryEntry;
typedef map<int, PSupplyHistoryEntry> SupplyHistoryEntryMap;
typedef list<PSupplyHistoryEntry> SupplyHistoryEntryList;


#define SupplyHistogramEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPRAcAR2VLVxxnWlJcAg==")	//".iso.org.dod.internet.private.enterprises.641.6.4.4.3.1"
typedef struct tag_SupplyHistogramEntry
{
	Integer32 supplyHistogramIndex;
	Integer32 supplyHistogramInventoryIndex;
	ENUM_STRUCT_TYPE(SupplyTypeTC) supplyHistogramSupplyType;
	DisplayString supplyHistogramColorantValue[OCTET_STRING_SIZE_256];
	SnmpAdminString supplyHistogramDescription[OCTET_STRING_SIZE_256];
	ENUM_STRUCT_TYPE(UnitsTC) supplyHistogramCapacityUnit;
	Integer32 supplyHistogramCapacity;
	Counter32 supplyHistogramCount;
	ENUM_STRUCT_TYPE(UnitsTC) supplyHistogramCountUnits;
}SupplyHistogramEntry , *PSupplyHistogramEntry;
typedef map<int, PSupplyHistogramEntry> SupplyHistogramEntryMap;
typedef list<PSupplyHistogramEntry> SupplyHistogramEntryList;


#endif	//_LEXMARK_MIB_DEF_7C51C662_16E8_46c7_8150_3AD782FE12F0_H_
