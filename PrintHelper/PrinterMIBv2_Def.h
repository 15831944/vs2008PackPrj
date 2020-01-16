//add by zxl,20160116, (Printer MIB v2) snmp结构体定义
//参考文件
//http://tools.ietf.org/html/rfc1759	//Printer MIB
//http://www.ietf.org/rfc/rfc3807.txt
//http://www.ietf.org/rfc/rfc3806.txt
//http://www.ietf.org/rfc/rfc3805.txt	//Printer MIB v2
//http://www.iana.org/assignments/character-sets/character-sets.xhtml
//http://www.iana.org/assignments/ianacharset-mib/ianacharset-mib
//http://tools.ietf.org/html/rfc1759
//http://tools.ietf.org/html/rfc2579
//http://tools.ietf.org/html/rfc2578#page-21
//http://tools.ietf.org/html/rfc2580
//http://tools.ietf.org/html/rfc3805#page-10

#ifndef _PRINTER_MIB_V2_DEF_650FBD0E_EA63_4BCA_8694_187A22D44C12_H_
#define _PRINTER_MIB_V2_DEF_650FBD0E_EA63_4BCA_8694_187A22D44C12_H_

#include "Snmp_Def.h"

#define RFC_1759
//#define RFC_3805

#ifdef RFC_3805
#undef RFC_3805
#endif

//http://www.iana.org/assignments/ianacharset-mib/ianacharset-mib
//http://www.iana.org/assignments/character-sets/character-sets.xhtml
BEGIN_ENUM_STRUCT(IANACharset)
{
	Other = 1,   // used if the designated
	// character set is not currently
	// registered by IANA
	UnKnown = 2, // used as a default value
	csASCII = 3,
	csISOLatin1 = 4,
	csISOLatin2 = 5,
	csISOLatin3 = 6,
	csISOLatin4 = 7,
	csISOLatinCyrillic = 8,
	csISOLatinArabic = 9,
	csISOLatinGreek = 10,
	csISOLatinHebrew = 11,
	csISOLatin5 = 12,
	csISOLatin6 = 13,
	csISOTextComm = 14,
	csHalfWidthKatakana = 15,
	csJISEncoding = 16,
	csShiftJIS = 17,
	csEUCPkdFmtJapanese = 18,
	csEUCFixWidJapanese = 19,
	csISO4UnitedKingdom = 20,
	csISO11SwedishForNames = 21,
	csISO15Italian = 22,
	csISO17Spanish = 23,
	csISO21German = 24,
	csISO60DanishNorwegian = 25,
	csISO69French = 26,
	csISO10646UTF1 = 27,
	csISO646basic1983 = 28,
	csINVARIANT = 29,
	csISO2IntlRefVersion = 30,
	csNATSSEFI = 31,
	csNATSSEFIADD = 32,
	csNATSDANO = 33,
	csNATSDANOADD = 34,
	csISO10Swedish = 35,
	csKSC56011987 = 36,
	csISO2022KR = 37,
	csEUCKR = 38,
	csISO2022JP = 39,
	csISO2022JP2 = 40,
	csISO13JISC6220jp = 41,
	csISO14JISC6220ro = 42,
	csISO16Portuguese = 43,
	csISO18Greek7Old = 44,
	csISO19LatinGreek = 45,
	csISO25French = 46,
	csISO27LatinGreek1 = 47,
	csISO5427Cyrillic = 48,
	csISO42JISC62261978 = 49,
	csISO47BSViewdata = 50,
	csISO49INIS = 51,
	csISO50INIS8 = 52,
	csISO51INISCyrillic = 53,
	csISO54271981 = 54,
	csISO5428Greek = 55,
	csISO57GB1988 = 56,
	csISO58GB231280 = 57,
	csISO61Norwegian2 = 58,
	csISO70VideotexSupp1 = 59,
	csISO84Portuguese2 = 60,
	csISO85Spanish2 = 61,
	csISO86Hungarian = 62,
	csISO87JISX0208 = 63,
	csISO88Greek7 = 64,
	csISO89ASMO449 = 65,
	csISO90 = 66,
	csISO91JISC62291984a = 67,
	csISO92JISC62991984b = 68,
	csISO93JIS62291984badd = 69,
	csISO94JIS62291984hand = 70,
	csISO95JIS62291984handadd = 71,
	csISO96JISC62291984kana = 72,
	csISO2033 = 73,
	csISO99NAPLPS = 74,
	csISO102T617bit = 75,
	csISO103T618bit = 76,
	csISO111ECMACyrillic = 77,
	csa71 = 78,
	csa72 = 79,
	csISO123CSAZ24341985gr = 80,
	csISO88596E = 81,
	csISO88596I = 82,
	csISO128T101G2 = 83,
	csISO88598E = 84,
	csISO88598I = 85,
	csISO139CSN369103 = 86,
	csISO141JUSIB1002 = 87,
	csISO143IECP271 = 88,
	csISO146Serbian = 89,
	csISO147Macedonian = 90,
	csISO150 = 91,
	csISO151Cuba = 92,
	csISO6937Add = 93,
	csISO153GOST1976874 = 94,
	csISO8859Supp = 95,
	csISO10367Box = 96,
	csISO158Lap = 97,
	csISO159JISX02121990 = 98,
	csISO646Danish = 99,
	csUSDK = 100,
	csDKUS = 101,
	csKSC5636 = 102,
	csUnicode11UTF7 = 103,
	csISO2022CN = 104,
	csISO2022CNEXT = 105,
	csUTF8 = 106,
	csISO885913 = 109,
	csISO885914 = 110,
	csISO885915 = 111,
	csISO885916 = 112,
	csGBK = 113,
	csGB18030 = 114,
	csOSDEBCDICDF0415 = 115,
	csOSDEBCDICDF03IRV = 116,
	csOSDEBCDICDF041 = 117,
	csISO115481 = 118,
	csKZ1048 = 119,
	csUnicode = 1000,
	csUCS4 = 1001,
	csUnicodeASCII = 1002,
	csUnicodeLatin1 = 1003,
	csUnicodeJapanese = 1004,
	csUnicodeIBM1261 = 1005,
	csUnicodeIBM1268 = 1006,
	csUnicodeIBM1276 = 1007,
	csUnicodeIBM1264 = 1008,
	csUnicodeIBM1265 = 1009,
	csUnicode11 = 1010,
	csSCSU = 1011,
	csUTF7 = 1012,
	csUTF16BE = 1013,
	csUTF16LE = 1014,
	csUTF16 = 1015,
	csCESU8 = 1016,
	csUTF32 = 1017,
	csUTF32BE = 1018,
	csUTF32LE = 1019,
	csBOCU1 = 1020,
	csWindows30Latin1 = 2000,
	csWindows31Latin1 = 2001,
	csWindows31Latin2 = 2002,
	csWindows31Latin5 = 2003,
	csHPRoman8 = 2004,
	csAdobeStandardEncoding = 2005,
	csVenturaUS = 2006,
	csVenturaInternational = 2007,
	csDECMCS = 2008,
	csPC850Multilingual = 2009,
	csPCp852 = 2010,
	csPC8CodePage437 = 2011,
	csPC8DanishNorwegian = 2012,
	csPC862LatinHebrew = 2013,
	csPC8Turkish = 2014,
	csIBMSymbols = 2015,
	csIBMThai = 2016,
	csHPLegal = 2017,
	csHPPiFont = 2018,
	csHPMath8 = 2019,
	csHPPSMath = 2020,
	csHPDesktop = 2021,
	csVenturaMath = 2022,
	csMicrosoftPublishing = 2023,
	csWindows31J = 2024,
	csGB2312 = 2025,
	csBig5 = 2026,
	csMacintosh = 2027,
	csIBM037 = 2028,
	csIBM038 = 2029,
	csIBM273 = 2030,
	csIBM274 = 2031,
	csIBM275 = 2032,
	csIBM277 = 2033,
	csIBM278 = 2034,
	csIBM280 = 2035,
	csIBM281 = 2036,
	csIBM284 = 2037,
	csIBM285 = 2038,
	csIBM290 = 2039,
	csIBM297 = 2040,
	csIBM420 = 2041,
	csIBM423 = 2042,
	csIBM424 = 2043,
	csIBM500 = 2044,
	csIBM851 = 2045,
	csIBM855 = 2046,
	csIBM857 = 2047,
	csIBM860 = 2048,
	csIBM861 = 2049,
	csIBM863 = 2050,
	csIBM864 = 2051,
	csIBM865 = 2052,
	csIBM868 = 2053,
	csIBM869 = 2054,
	csIBM870 = 2055,
	csIBM871 = 2056,
	csIBM880 = 2057,
	csIBM891 = 2058,
	csIBM903 = 2059,
	csIBBM904 = 2060,
	csIBM905 = 2061,
	csIBM918 = 2062,
	csIBM1026 = 2063,
	csIBMEBCDICATDE = 2064,
	csEBCDICATDEA = 2065,
	csEBCDICCAFR = 2066,
	csEBCDICDKNO = 2067,
	csEBCDICDKNOA = 2068,
	csEBCDICFISE = 2069,
	csEBCDICFISEA = 2070,
	csEBCDICFR = 2071,
	csEBCDICIT = 2072,
	csEBCDICPT = 2073,
	csEBCDICES = 2074,
	csEBCDICESA = 2075,
	csEBCDICESS = 2076,
	csEBCDICUK = 2077,
	csEBCDICUS = 2078,
	csUnknown8BiT = 2079,
	csMnemonic = 2080,
	csMnem = 2081,
	csVISCII = 2082,
	csVIQR = 2083,
	csKOI8R = 2084,
	csHZGB2312 = 2085,
	csIBM866 = 2086,
	csPC775Baltic = 2087,
	csKOI8U = 2088,
	csIBM00858 = 2089,
	csIBM00924 = 2090,
	csIBM01140 = 2091,
	csIBM01141 = 2092,
	csIBM01142 = 2093,
	csIBM01143 = 2094,
	csIBM01144 = 2095,
	csIBM01145 = 2096,
	csIBM01146 = 2097,
	csIBM01147 = 2098,
	csIBM01148 = 2099,
	csIBM01149 = 2100,
	csBig5HKSCS = 2101,
	csIBM1047 = 2102,
	csPTCP154 = 2103,
	csAmiga1251 = 2104,
	csKOI7switched = 2105,
	csBRF = 2106,
	csTSCII = 2107,
	csCP51932 = 2108,
	cswindows874 = 2109,
	cswindows1250 = 2250,
	cswindows1251 = 2251,
	cswindows1252 = 2252,
	cswindows1253 = 2253,
	cswindows1254 = 2254,
	cswindows1255 = 2255,
	cswindows1256 = 2256,
	cswindows1257 = 2257,
	cswindows1258 = 2258,
	csTIS620 = 2259,
	cs50220 = 2260,
	reserved = 3000
}
END_ENUM_STRUCT(IANACharset)

//计量单位
BEGIN_ENUM_STRUCT(MeasureUnit)
{
	Other = 1,                  //New, not in RFC 1759
	UnKnown = 2,                //New, not in RFC 1759
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4,			//千分一毫米，微米
	Characters = 5,				//字符
	Lines = 6,					//行
	Impressions = 7,			//印模(面，与Sheets页相对)			//New, not in RFC 1759
	Sheets = 8,					//纸张、页			//New, not in RFC 1759
	DotRow = 9,					//
	Hours = 11,					//小时				//New, not in RFC 1759
	ThousandthsOfOunces = 12,	//千分之一盎司
	TenthsOfGrams = 13,			//十分之一克
	HundrethsOfFluidOunces = 14,//百分之一盎司
	TenthsOfMilliliters = 15,	//十分之一毫升
	Feet = 16,					//英尺				//New, not in RFC 1759
	Meters = 17,				//米				//New, not in RFC 1759
	//Values for Finisher MIB
	Items = 18,                  //项				//New, not in RFC 1759	 // e.g., #staples
	Percent = 19                 //百分比			//New, not in RFC 1759
}
END_ENUM_STRUCT(MeasureUnit)

//属性开关
BEGIN_ENUM_STRUCT(PresentOnOff)
{
	Other = 1,		//其它
	On = 3,			//开		
	Off = 4,		//关
	NotPresent = 5	//不提供
}
END_ENUM_STRUCT(PresentOnOff)

//iso.org.dod.internet.mgmt.mib-2.printmib.(02)prtMIBConformance
//.1.3.6.1.2.1.43.2

//iso.org.dod.internet.mgmt.mib-2.printmib.(05)prtGeneral
//.1.3.6.1.2.1.43.5
// Setting this value to 'powerCycleReset',
// 'resetToNVRAM', or 'resetToFactoryDefaults' will result
// in the resetting of the printer.  When read, this object
// will always have the value 'notResetting(3)', and a SET
// of the value 'notResetting' shall have no effect on the
// printer.  Some of the defined values are optional.
// However, every implementation must support at least the
// values 'notResetting' and 'resetToNVRAM'.
BEGIN_ENUM_STRUCT(PrtGeneralResetTC)
{
	NotResetting = 3,
	PowerCycleReset = 4,		// Cold Start
	ResetToNVRAM = 5,			// Warm Start
	ResetToFactoryDefaults = 6	// Reset contents of
								// NVRAM to factory
								// defaults
}
END_ENUM_STRUCT(PrtGeneralResetTC)
// If the object prtConsoleDisable has value
// 'operatorConsoleDisabled' then input is not accepted
// from the operator console. If the object
// prtConsoleDisable has the value 'operatorConsoleEnabled'
// then input is accepted from the operator console. If the
// object prtConsoleDisable has the value
// 'operatorConsoleEnabledLevel1',
// 'operatorConsoleEnabledLevel2' or
// 'operatorConsoleEnabledLevel3' then limited input is
// accepted from the operator console; the limitations are
// product specific, however, the limitations are generally
// less restrictive for operatorConsoleEnabledLevel1 than
// for operatorConsoleEnabledLeve2, which is less
// restrictive than operatorConsoleEnabledLevel3.
// The value of the prtConsoleDisable object is a type-2
// enumeration.
BEGIN_ENUM_STRUCT(PrtConsoleDisableTC)
{
	OperatorConsoleEnabled = 3,
	OperatorConsoleDisabled = 4,
	OperatorConsoleEnabledLevel1 = 5,
	OperatorConsoleEnabledLevel2 = 6,
	OperatorConsoleEnabledLevel3 = 7
}
END_ENUM_STRUCT(PrtConsoleDisableTC)

//printmib.prtGeneral.prtGeneralTable.prtGeneralEntry
#define PrtGeneralEntryOID					TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0Aq")											//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry"
#define PrtGeneralConfigChangesOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz83DhxVWA4QDQJcNBES")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralConfigChanges"
#define PrtGeneralCurrentLocalizationOID	TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz83FABBVAcnKQxRMhgICFJFADwL")	//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralCurrentLocalization"
#define PrtGeneralResetOID					TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz8mBAFWRQ==")					//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralReset"
#define PrtGeneralCurrentOperatorOID		TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz83FABBVAcnKhNXIRUVHUE=")		//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralCurrentOperator"
#define PrtGeneralServicePersonOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz8nBABFWAo2NQZAIBsP")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralServicePerson"
#define PrtInputDefaultIndexOID				TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3gHIxAXdjYSAAdfRSA9AQZK")				//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtInputDefaultIndex"
#define PrtOutputDefaultIndexOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR34cJxUWRhcRBxNGXR0aCwdXKw==")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtOutputDefaultIndex"
#define PrtMarkerDefaultIndexOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3wIIQ4GQBcRBxNGXR0aCwdXKw==")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtMarkerDefaultIndex"
#define PrtMediaPathDefaultIndexOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3wMNwwCYjIACTZWVwgmCRd7PRAECg==")		//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtMediaPathDefaultIndex"
#define PrtConsoleLocalizationOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3IGPRYMXjY4DhFSXQApBBdbPBo=")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtConsoleLocalization"
#define PrtConsoleNumberOfDisplayLinesOID	TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3IGPRYMXjY6FB9RVBscAydbIAQNE0p9AD0AEA==")	//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtConsoleNumberOfDisplayLines"
#define PrtConsoleNumberOfDisplayCharsOID	TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3IGPRYMXjY6FB9RVBscAydbIAQNE0pyATIXEA==")	//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtConsoleNumberOfDisplayChars"
#define PrtConsoleDisableOID				TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3IGPRYMXjYwCAFSUwU2")					//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtConsoleDisable"
#define PrtAuxiliarySheetStartupPageOID		TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3AcKwwPWzIGGCFbVAwnNhdTIQAUAmNQDjY=")	//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtAuxiliarySheetStartupPage"
#define PrtAuxiliarySheetBannerPageOID		TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3AcKwwPWzIGGCFbVAwnJwJcPRETIlJWDA==")	//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtAuxiliarySheetBannerPage"
#define PrtGeneralPrinterNameOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz8kExtdRQwhKwJfNg==")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralPrinterName"
#define PrtGeneralSerialNumberOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3YMPQARUz8nBABaUAUdEA5QNgY=")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtGeneralSerialNumber"
#define PrtAlertCriticalEventsOID			TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3AFNhcXcSEdFRtQUAUWEwZcJwc=")			//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtAlertCriticalEvents"
#define PrtAlertAllEventsOID				TEXT("QRs6CxdfOhZPAkFFLjYLBkAyGE8CQUUuNgsGQDIYNRNRXQx9FRFGFBEPF0FQBRYLF0AqWhEAR3AFNhcXcz8YJARWXx0g")					//"printmib.prtGeneral.prtGeneralTable.prtGeneralEntry.prtAlertAllEvents"
typedef struct tag_PrtGeneralEntry
{
	// Note that not all of the objects in this sequence are in
	// the general printer group. The group to which an
	// object belongs is tagged with a label "General", "Input"
	// "Output", etc. after each entry in the following sequence.
	//
	Counter32 prtGeneralConfigChanges;								//(1) General 
	Integer32 prtGeneralCurrentLocalization;						//(2) General
	ENUM_STRUCT_TYPE(PrtGeneralResetTC) prtGeneralReset;			//(3) General
	OCTET_STRING prtGeneralCurrentOperator[OCTET_STRING_SIZE_128];	//(4) Responsible Party
	OCTET_STRING prtGeneralServicePerson[OCTET_STRING_SIZE_128];	//(5) Responsible Party
	Integer32 prtInputDefaultIndex;									//(6) Input
	Integer32 prtOutputDefaultIndex;								//(7) Output
	Integer32 prtMarkerDefaultIndex;								//(8) Marker
	Integer32 prtMediaPathDefaultIndex;								//(9) Media Path
	Integer32 prtConsoleLocalization;								//(10) Console
	Integer32 prtConsoleNumberOfDisplayLines;						//(11) Console
	Integer32 prtConsoleNumberOfDisplayChars;						//(12) Console
	ENUM_STRUCT_TYPE(PrtConsoleDisableTC) prtConsoleDisable;		//(13) Console,
	ENUM_STRUCT_TYPE(PresentOnOff) prtAuxiliarySheetStartupPage;	//(14) AuxiliarySheet,以后的字段是版本1中是不存在的，使用编号，不能使用名称
	ENUM_STRUCT_TYPE(PresentOnOff) prtAuxiliarySheetBannerPage;		//(15) AuxiliarySheet
	OCTET_STRING prtGeneralPrinterName[OCTET_STRING_SIZE_128];		//(16) General V2
	OCTET_STRING prtGeneralSerialNumber[OCTET_STRING_SIZE_256];		//(17) General V2
	Counter32 prtAlertCriticalEvents;								//(18) Alert V2
	Counter32 prtAlertAllEvents;									//(19) Alert V2
}PrtGeneralEntry, *PPrtGeneralEntry;
typedef map<int, PPrtGeneralEntry> PrtGeneralEntryMap;
typedef list<PPrtGeneralEntry> PrtGeneralEntryList;

//printmib.prtGeneral.prtStorageRefTable.prtStorageRefEntry
typedef struct tag_PrtStorageRefEntry
{
	Integer32 prtStorageRefSeqNumber;
	Integer32 prtStorageRefIndex;    
}PrtStorageRefEntry, *PPrtStorageRefEntry;
typedef map<int, PPrtStorageRefEntry> PrtStorageRefEntryMap;
typedef list<PPrtStorageRefEntry> PrtStorageRefEntryList;

//printmib.prtGeneral.prtDeviceRefTable.prtDeviceRefEntry
typedef struct tag_PrtDeviceRefEntry
{
	Integer32 prtDeviceRefSeqNumber;
	Integer32 prtDeviceRefIndex;    
}PrtDeviceRefEntry, *PPrtDeviceRefEntry;
typedef map<int, PPrtDeviceRefEntry> PrtDeviceRefEntryMap;
typedef list<PPrtDeviceRefEntry> PrtDeviceRefEntryList;
//(05)prtGeneral [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(06)prtCover
//.1.3.6.1.2.1.43.6
//盖板状态
BEGIN_ENUM_STRUCT(PrtCoverStatusTC)
{
	Other = 1,
	CoverOpen = 3,
	CoverClosed = 4,
	InterlockOpen = 5,
	InterlockClosed = 6
}
END_ENUM_STRUCT(PrtCoverStatusTC)

//printmib.prtCover.prtCoverTable.prtCoverEntry
#define PrtCoverEntryOID					TEXT("QRs6CxdfOhZPAkFFKjwTBkB9BBMGcF4fNhc3UzEYBFxDQx0QChVXITEPBkFI")								//"printmib.prtCover.prtCoverTable.prtCoverEntry"
#define PrtCoverIndexOID					TEXT("QRs6CxdfOhZPAkFFKjwTBkB9BBMGcF4fNhc3UzEYBFxDQx0QChVXITEPBkFIRyMXF3E8AgQAel8NNh0=")			//"printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverIndex"
#define PrtCoverDescriptionOID				TEXT("QRs6CxdfOhZPAkFFKjwTBkB9BBMGcF4fNhc3UzEYBFxDQx0QChVXITEPBkFIRyMXF3E8AgQAd1QaMBcKQicdDhw=")	//"printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription"
#define PrtCoverStatusOID					TEXT("QRs6CxdfOhZPAkFFKjwTBkB9BBMGcF4fNhc3UzEYBFxDQx0QChVXITEPBkFIRyMXF3E8AgQAYEUIJxAQ")			//"printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverStatus"
typedef struct tag_PrtCoverEntry
{
	Integer32 prtCoverIndex;									//(1)
	OCTET_STRING prtCoverDescription[OCTET_STRING_SIZE_256];	//(2) PrtLocalizedDescriptionStringTC
	ENUM_STRUCT_TYPE(PrtCoverStatusTC) prtCoverStatus;			//(3)
}PrtCoverEntry, *PPrtCoverEntry;
typedef map<int, PPrtCoverEntry> PrtCoverEntryMap;
typedef list<PPrtCoverEntry> PrtCoverEntryList;

//iso.org.dod.internet.mgmt.mib-2.printmib.(07)prtLocalization
//.1.3.6.1.2.1.43.7
//printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry
#define PrtLocalizationEntryOID				TEXT("QRs6CxdfOhZPAkFFJTwGAl46DgAGWl4HfRURRh8bAhNfWBMyEQpdPSAAEF9URyMXF348FwAeWksIJwwMXBYaFQBK")											//"printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry"
#define PrtLocalizationIndexOID				TEXT("QRs6CxdfOhZPAkFFJTwGAl46DgAGWl4HfRURRh8bAhNfWBMyEQpdPSAAEF9URyMXF348FwAeWksIJwwMXBYaFQBKHxkhES9dMBUNG0lQHToKDXs9EAQK")				//"printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry.prtLocalizationIndex"
#define PrtLocalizationLanguageOID			TEXT("QRs6CxdfOhZPAkFFJTwGAl46DgAGWl4HfRURRh8bAhNfWBMyEQpdPSAAEF9URyMXF348FwAeWksIJwwMXBYaFQBKHxkhES9dMBUNG0lQHToKDX4yGgYHUlYM")			//"printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry.prtLocalizationLanguage"
#define PrtLocalizationCountryOID			TEXT("QRs6CxdfOhZPAkFFJTwGAl46DgAGWl4HfRURRh8bAhNfWBMyEQpdPSAAEF9URyMXF348FwAeWksIJwwMXBYaFQBKHxkhES9dMBUNG0lQHToKDXE8AQ8GQUg=")			//"printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry.prtLocalizationCountry"
#define PrtLocalizationCharacterSetOID		TEXT("QRs6CxdfOhZPAkFFJTwGAl46DgAGWl4HfRURRh8bAhNfWBMyEQpdPSAAEF9URyMXF348FwAeWksIJwwMXBYaFQBKHxkhES9dMBUNG0lQHToKDXE7FRMTUEUMITYGRg==")	//"printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry.prtLocalizationCharacterSet"
typedef struct tag_PrtLocalizationEntry
{
	Integer32 prtLocalizationIndex;								//(1)
	OCTET_STRING prtLocalizationLanguage[OCTET_STRING_SIZE_3];	//(2)
	OCTET_STRING prtLocalizationCountry[OCTET_STRING_SIZE_3];	//(3)
	ENUM_STRUCT_TYPE(IANACharset) prtLocalizationCharacterSet;	//(4)
}PrtLocalizationEntry , *PPrtLocalizationEntry;
typedef map<int, PPrtLocalizationEntry> PrtLocalizationEntryMap;
typedef list<PPrtLocalizationEntry> PrtLocalizationEntryList;


//iso.org.dod.internet.mgmt.mib-2.printmib.(08)prtInput
//.1.3.6.1.2.1.43.8
//输入设备类型
BEGIN_ENUM_STRUCT(PrtInputTypeTC)
{
	Other	= 1,
	UnKnow	= 2,
	SheetFeedAutoRemovableTray = 3,		//可移除自动进纸盒
	SheetFeedAutoNonRemovableTray = 4,	//不可移除自动进纸盒
	SheetFeedManual = 5,	//手动进纸盒
	ContinuousRoll = 6,		//大容量纸盒
	ContinuousFanFold = 7	//折叠式纸盒
}
END_ENUM_STRUCT(PrtInputTypeTC)

//输入长度单位
BEGIN_ENUM_STRUCT(PrtMediaUnitTC)
{
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4				//千分一毫米，微米
}
END_ENUM_STRUCT(PrtMediaUnitTC)

//容量单位
BEGIN_ENUM_STRUCT(PrtCapacityUnitTC)
{
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4,			//千分一毫米，微米
	Sheets = 8,					//纸张、页			//New, not in RFC 1759
	Feet = 16,					//英尺				//New, not in RFC 1759
	Meters = 17					//米				//New, not in RFC 1759
}
END_ENUM_STRUCT(PrtCapacityUnitTC)

//printmib.prtInput.prtInputTable.prtInputEntry
#define	PrtInputEntryOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFI")												//"printmib.prtInput.prtInputTable.prtInputEntry"
#define	PrtInputIndexOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGel8NNh0=")							//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputIndex"
#define	PrtInputTypeOID						TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGZ0gZNg==")							//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputType"
#define	PrtInputDimUnitOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGd1gEBgsKRg==")						//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputDimUnit"
#define	PrtInputMediaDimFeedDirDeclaredOID	TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQnWz4yBBdXdQAhIQZRPxUTF1c=")	//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirDeclared"
#define	PrtInputMediaDimXFeedDirDeclaredOID	TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQnWz4sJxdWVS06FydXMBgAAFZV")	//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirDeclared"
#define	PrtInputMediaDimFeedDirChosenOID	TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQnWz4yBBdXdQAhJgtdIBEP")		//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirChosen"
#define	PrtInputMediaDimXFeedDirChosenOID	TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQnWz4sJxdWVS06FyBaPAcEHA==")	//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirChosen"
#define	PrtInputCapacityUnitOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGcFAZMgYKRiohDxtH")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputCapacityUnit"
#define	PrtInputMaxCapacityOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflAREAQTUzAdFQs=")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMaxCapacity"
#define	PrtInputCurrentLevelOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGcEQbIQANRh8RFxdf")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputCurrentLevel"
#define	PrtInputStatusOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGYEUIJxAQ")							//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputStatus"
#define	PrtInputMediaNameOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQtUz4R")						//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaName"
#define	PrtInputNameOID						TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGfVAENg==")							//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputName"
#define	PrtInputVendorNameOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGZVQHNwoRfDIZBA==")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputVendorName"
#define	PrtInputModelOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGfl4NNgk=")							//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputModel"
#define	PrtInputVersionOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGZVQbIAwMXA==")						//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputVersion"
#define	PrtInputSerialNumberOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGYFQbOgQPfCYZAxdB")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputSerialNumber"
#define	PrtInputDescriptionOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGd1QaMBcKQicdDhw=")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputDescription"
#define	PrtInputSecurityOID					TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGYFQKJhcKRio=")						//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputSecurity"
#define	PrtInputMediaWeightOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQ0VzoTCQY=")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaWeight"
#define	PrtInputMediaTypeOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQ3SyMR")						//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaType"
#define	PrtInputMediaColorOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQgXT8bEw==")					//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaColor"
#define	PrtInputMediaFormPartsOID			TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQlXSEZMRNBRRo=")				//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaFormParts"
#ifdef RFC_3805
#define	PrtInputMediaLoadTimeoutOID			TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGflQNOgQvXTIQNRteVAYmEQ==")			//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaLoadTimeout"
#define	PrtInputNextIndexOID				TEXT("QRs6CxdfOhZPAkFFID0VFkZ9BBMGel8ZJhE3UzEYBFxDQx0aCxNHJzEPBkFIRyMXF3s9BBQGfVQRJywNVjYM")						//"printmib.prtInput.prtInputTable.prtInputEntry.prtInputNextIndex"
#endif
typedef struct tag_PrtInputEntry
{
	Integer32 prtInputIndex;								//(1)
	ENUM_STRUCT_TYPE(PrtInputTypeTC) prtInputType;			//(2)
	ENUM_STRUCT_TYPE(PrtMediaUnitTC) prtInputDimUnit;		//(3)
	Integer32 prtInputMediaDimFeedDirDeclared;				//(4)
	Integer32 prtInputMediaDimXFeedDirDeclared;				//(5)
	Integer32 prtInputMediaDimFeedDirChosen;				//(6)
	Integer32 prtInputMediaDimXFeedDirChosen;				//(7)	
	ENUM_STRUCT_TYPE(PrtCapacityUnitTC) prtInputCapacityUnit;	//(8)
	Integer32 prtInputMaxCapacity;							//(9)
	Integer32 prtInputCurrentLevel;							//(10)
	Integer32 prtInputStatus;	//PrtSubUnitStatusTC		//(11)
	OCTET_STRING prtInputMediaName[OCTET_STRING_SIZE_64];	//(12)
	OCTET_STRING prtInputName[OCTET_STRING_SIZE_64];		//(13)
	OCTET_STRING prtInputVendorName[OCTET_STRING_SIZE_64];	//(14)
	OCTET_STRING prtInputModel[OCTET_STRING_SIZE_64];		//(15)
	OCTET_STRING prtInputVersion[OCTET_STRING_SIZE_64];		//(16)
	OCTET_STRING prtInputSerialNumber[OCTET_STRING_SIZE_64];	//(17)
	OCTET_STRING prtInputDescription[OCTET_STRING_SIZE_256];    //(18) PrtLocalizedDescriptionStringTC,
	ENUM_STRUCT_TYPE(PresentOnOff) prtInputSecurity;		//(19)
	Integer32 prtInputMediaWeight;							//(20)
	OCTET_STRING prtInputMediaType[OCTET_STRING_SIZE_64];	//(21)
	OCTET_STRING prtInputMediaColor[OCTET_STRING_SIZE_64];	//(22)
	Integer32 prtInputMediaFormParts;						//(23)
#ifdef RFC_3805
	Integer32 prtInputMediaLoadTimeout;						//(24) define in rfc3805, not in rfc1759，以后的字段是版本1中是不存在的，使用编号，不能使用名称
	Integer32 prtInputNextIndex;							//(25)	define in rfc3805, not in rfc1759
#endif
}PrtInputEntry, *PPrtInputEntry;
typedef map<int, PPrtInputEntry> PrtInputEntryMap;
typedef list<PPrtInputEntry> PrtInputEntryList;
//(09)prtOutput [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(09)prtOutput
//.1.3.6.1.2.1.43.9
//输出类型
BEGIN_ENUM_STRUCT(PrtOutputTypeTC)
{
	Other	= 1,
	UnKnow	= 2,
	RemovableBin = 3,			//可移除纸盒
	UnRemovableBin = 4,			//不可移除纸盒
	ContinuousRollDevice = 5,	//大容量设备
	MailBox = 6,				//邮箱
	ContinuousFanFold = 7		//折叠式纸盒
}
END_ENUM_STRUCT(PrtOutputTypeTC)

// 输出长度单位
// BEGIN_ENUM_STRUCT(OutputDimUnit)
// {
// 	TenThousandthsOfInches = 3,	//10千分之一英寸
// 	Micrometers = 4				//千分一毫米，微米
// }
// END_ENUM_STRUCT(OutputDimUnit)

// 输出容量单位
// BEGIN_ENUM_STRUCT(OutputCapacityUnit)
// {
// 	TenThousandthsOfInches = 3,	//10千分之一英寸
// 	Micrometers = 4,			//千分一毫米，微米
// 	Sheets = 8,					//纸张、页			//New, not in RFC 1759
// 	Feet = 16,					//英尺				//New, not in RFC 1759
// 	Meters = 17					//米				//New, not in RFC 1759
// }
// END_ENUM_STRUCT(OutputCapacityUnit)

//输出纸盒纸张出纸顺序
BEGIN_ENUM_STRUCT(PrtOutputStackingOrderTC)
{
	UnKnow = 2,
	FirstToLast = 3,
	LastToFirst = 4
}
END_ENUM_STRUCT(PrtOutputStackingOrderTC)

//纸张传送方向
BEGIN_ENUM_STRUCT(PrtOutputPageDeliveryOrientationTC)
{
	FaceUp = 3,		//面朝上
	FaceDown = 4	//面朝下
}
END_ENUM_STRUCT(PrtOutputPageDeliveryOrientationTC)

//printmib.prtOutput.prtOutputTable.prtOutputEntry
#define	PrtOutputEntryOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEc")										//"printmib.prtOutput.prtOutputTable.prtOutputEntry"
#define	PrtOutputIndexOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJywNVjYM")					//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputIndex"
#define	PrtOutputTypeOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzEaQjY=")					//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputType"
#define	PrtOutputCapacityUnitOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJyYCQjIXCAZKZAc6EQ==")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputCapacityUnit"
#define	PrtOutputMaxCapacityOID				TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJygCShAVERNQWB0q")			//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputMaxCapacity"
#define	PrtOutputRemainingCapacityOID		TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzcGXzIdDxtdVioyFQJROgAY")	//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputRemainingCapacity"
#define	PrtOutputStatusOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzYXUycBEg==")				//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputStatus"
#define	PrtOutputNameOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJysCXzY=")					//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputName"
#define	PrtOutputVendorNameOID				TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzMGXDcbEzxSXAw=")			//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputVendorName"
#define	PrtOutputModelOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJygMVjYY")					//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputModel"
#define	PrtOutputVersionOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzMGQCAdDhw=")				//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputVersion"
#define	PrtOutputSerialNumberOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzYGQDoVDTxGXAs2Fw==")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputSerialNumber"
#define	PrtOutputDescriptionOID				TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJyEGQTAGCAJHWAY9")			//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputDescription"
#define	PrtOutputSecurityOID				TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzYGUSYGCAZK")				//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputSecurity"
#define	PrtOutputDimUnitOID					TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJyEKXwYaCAY=")				//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputDimUnit"
#define	PrtOutputMaxDimFeedDirOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJygCShcdDDRWVA0XDBE=")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputMaxDimFeedDir"
#define	PrtOutputMaxDimXFeedDirOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJygCShcdDCp1VAw3IQpA")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputMaxDimXFeedDir"
#define	PrtOutputMinDimFeedDirOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJygKXBcdDDRWVA0XDBE=")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputMinDimFeedDir"
#define	PrtOutputMinDimXFeedDirOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJygKXBcdDCp1VAw3IQpA")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputMinDimXFeedDir"
#define	PrtOutputStackingOrderOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzYXUzAfCBxUfhs3ABE=")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputStackingOrder"
#define	PrtOutputPageDeliveryOrientationOID	TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzUCVTYwBB5aRwwhHCxAOhEPBlJFADwL")	//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputPageDeliveryOrientation"
#define	PrtOutputBurstingOID				TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJycWQCAACBxU")				//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputBursting"
#define	PrtOutputDecollatingOID				TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJyEGUTwYDRNHWAc0")			//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputDecollating"
#define	PrtOutputPageCollatedOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJzUCVTY3Dh5fUB02AQ==")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputPageCollated"
#define	PrtOutputOffsetStackingOID			TEXT("QRs6CxdfOhZPAkFFJiYRE0cnWhEAR34cJxUWRgcVAx5WHxkhESxHJwQUBnZfHSEcTUIhAC4HR0EcJyoFVCARFSFHUAo4DA1V")		//"printmib.prtOutput.prtOutputTable.prtOutputEntry.prtOutputOffsetStacking"
typedef struct tag_PrtOutputEntry
{
	Integer32 prtOutputIndex;									//(1)
	ENUM_STRUCT_TYPE(PrtOutputTypeTC) prtOutputType;			//(2)
	ENUM_STRUCT_TYPE(PrtCapacityUnitTC) prtOutputCapacityUnit;	//(3)
	Integer32 prtOutputMaxCapacity;								//(4)
	Integer32 prtOutputRemainingCapacity;						//(5)
	Integer32 prtOutputStatus;									//(6) PrtSubUnitStatusTC,
	OCTET_STRING prtOutputName[OCTET_STRING_SIZE_64];			//(7)
	OCTET_STRING prtOutputVendorName[OCTET_STRING_SIZE_64];		//(8)
	OCTET_STRING prtOutputModel[OCTET_STRING_SIZE_64];			//(9)
	OCTET_STRING prtOutputVersion[OCTET_STRING_SIZE_64];		//(10)	
	OCTET_STRING prtOutputSerialNumber[OCTET_STRING_SIZE_64];	//(11)
	OCTET_STRING prtOutputDescription[OCTET_STRING_SIZE_64];    //(12) PrtLocalizedDescriptionStringTC,
	ENUM_STRUCT_TYPE(PresentOnOff) prtOutputSecurity;			//(13)
	ENUM_STRUCT_TYPE(PrtMediaUnitTC) prtOutputDimUnit;			//(14)
	Integer32 prtOutputMaxDimFeedDir;							//(15)
	Integer32 prtOutputMaxDimXFeedDir;							//(16)	
	Integer32 prtOutputMinDimFeedDir;							//(17)
	Integer32 prtOutputMinDimXFeedDir;							//(18)
	ENUM_STRUCT_TYPE(PrtOutputStackingOrderTC) prtOutputStackingOrder;						//(19)
	ENUM_STRUCT_TYPE(PrtOutputPageDeliveryOrientationTC) prtOutputPageDeliveryOrientation;	//(20)
	ENUM_STRUCT_TYPE(PresentOnOff) prtOutputBursting;			//(21)
	ENUM_STRUCT_TYPE(PresentOnOff) prtOutputDecollating;		//(22)
	ENUM_STRUCT_TYPE(PresentOnOff) prtOutputPageCollated;		//(23)
	ENUM_STRUCT_TYPE(PresentOnOff) prtOutputOffsetStacking;		//(24)
}PrtOutputEntry, *PPrtOutputEntry;
typedef map<int, PPrtOutputEntry> PrtOutputEntryMap;
typedef list<PPrtOutputEntry> PrtOutputEntryList;
//(09)prtOutput [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(10)prtMarker
//.1.3.6.1.2.1.43.10
//技术类型
BEGIN_ENUM_STRUCT(PrtMarkerMarkTechTC)
{
	Other = 1,		//
	UnKnown = 2,	//
	ElectroPhotoGraphicLED = 3,
	ElectroPhotoGraphicLaser = 4,	//激光打印机
	ElectroPhotoGraphicOther = 5,
	ImpactMovingHeadDotMatrix9pin = 6,
	ImpactMovingHeadDotMatrix24pin = 7,
	ImpactMovingHeadDotMatrixOther = 8,
	ImpactMovingHeadFullyFormed = 9,
	ImpactBand = 10,
	ImpactOther = 11,
	InkJetAqueous = 12,		//喷墨打印机
	InkJetSolid = 13,
	InkJetOther = 14,
	Pen = 15,
	ThermalTransfer = 16,
	ThermalSensitive = 17,
	ThermalDiffusion = 18,
	ThermalOther = 19,
	ElectroErosion = 20,
	ElectroStatic = 21,
	PhotoGraphicMicroFiche = 22,
	PhotoGraphicImageSetter = 23,
	PhotoGraphicOther = 24,
	IonDeposition = 25,
	EBeam = 26,
	TypeSetter = 27
}
END_ENUM_STRUCT(PrtMarkerMarkTechTC)

// The unit that will be used by the printer when
// reporting counter values for this marking sub-unit.  The
// time units of measure are provided for a device like a
// strip recorder that does not or cannot track the
// physical dimensions of the media and does not use
// characters, lines or sheets.
BEGIN_ENUM_STRUCT(PrtMarkerCounterUnitTC)
{
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4,			//千分一毫米，微米
	Characters = 5,				//字符
	Lines = 6,					//行
	Impressions = 7,			//印模(面，与Sheets页相对)			//New, not in RFC 1759
	Sheets = 8,					//纸张、页			//New, not in RFC 1759
	DotRow = 9,					//
	Hours = 11,					//小时				//New, not in RFC 1759
	Feet = 16,					//英尺				//New, not in RFC 1759
	Meters = 17					//米				//New, not in RFC 1759
}
END_ENUM_STRUCT(PrtMarkerCounterUnitTC)

// The unit of measure of distances, as applied to the
// marker's resolution.
BEGIN_ENUM_STRUCT(PrtMarkerAddressabilityUnitTC)
{
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4				//千分一毫米，微米
}
END_ENUM_STRUCT(PrtMarkerAddressabilityUnitTC)

//printmib.prtMarker.prtMarkerTable.prtMarkerEntry
#define	PrtMarkerEntryOID					TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEc")												//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry"
#define	PrtMarkerIndexOID					TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISwNVjYM")							//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerIndex"
#define	PrtMarkerMarkTechOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISgCQDggBBFb")						//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerMarkTech"
#define	PrtMarkerCounterUnitOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISYMRz0ABABmXwAn")					//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerCounterUnit"
#define	PrtMarkerLifeCountOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISkKVDY3DgddRQ==")					//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount"
#define	PrtMarkerPowerOnCount				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMITUMRTYGLhxwXhw9EQ==")				//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerPowerOnCount"
#define	PrtMarkerProcessColorantsOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMITURXTAREgFwXgU8FwJcJwc=")			//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerProcessColorants"
#define	PrtMarkerSpotColorantsOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMITYTXSc3Dh5cQwg9ERA=")				//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerSpotColorants"
#define	PrtMarkerAddressabilityUnitOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISQHViEREgFSUwA/DBdLBhoIBg==")		//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerAddressabilityUnit"
#define	PrtMarkerAddressabilityFeedDirOID	TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISQHViEREgFSUwA/DBdLFREEFndYGw==")	//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerAddressabilityFeedDir"
#define	PrtMarkerAddressabilityXFeedDirOID	TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISQHViEREgFSUwA/DBdLCzIEF1d1ACE=")	//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerAddressabilityXFeedDir"
#define	PrtMarkerNorthMarginOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISsMQCccLBNBVgA9")					//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerNorthMargin"
#define	PrtMarkerSouthMarginOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMITYMRyccLBNBVgA9")					//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerSouthMargin"
#define	PrtMarkerWestMarginOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMITIGQSc5AABUWAc=")					//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerWestMargin"
#define	PrtMarkerEastMarginOID				TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMISACQSc5AABUWAc=")					//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerEastMargin"
#define	PrtMarkerStatusOID					TEXT("QRs6CxdfOhZPAkFFJDIXCFchWhEAR3wIIQ4GQAcVAx5WHxkhES5TIR8EAHZfHSEcTUIhACwTQVoMITYXUycBEg==")						//"printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerStatus"
typedef struct tag_PrtMarkerEntry
{
	Integer32 prtMarkerIndex;									//(1)
	ENUM_STRUCT_TYPE(PrtMarkerMarkTechTC) prtMarkerMarkTech;	//(2)
	ENUM_STRUCT_TYPE(PrtMarkerCounterUnitTC) prtMarkerCounterUnit;					//(3)
	Counter32 prtMarkerLifeCount;								//(4)
	Counter32 prtMarkerPowerOnCount;							//(5)
	Integer32 prtMarkerProcessColorants;						//(6)
	Integer32 prtMarkerSpotColorants;							//(7)
	ENUM_STRUCT_TYPE(PrtMarkerAddressabilityUnitTC) prtMarkerAddressabilityUnit;	//(8)
	Integer32 prtMarkerAddressabilityFeedDir;					//(9)
	Integer32 prtMarkerAddressabilityXFeedDir;					//(10)
	Integer32 prtMarkerNorthMargin;								//(11)
	Integer32 prtMarkerSouthMargin;								//(12)
	Integer32 prtMarkerWestMargin;								//(13)
	Integer32 prtMarkerEastMargin;								//(14)
	Integer32 prtMarkerStatus;									//(15) PrtSubUnitStatusTC
}PrtMarkerEntry, *PPrtMarkerEntry;
typedef map<int, PPrtMarkerEntry> PrtMarkerEntryMap;
typedef list<PPrtMarkerEntry> PrtMarkerEntryList;
//(10)prtMarker [end]


//iso.org.dod.internet.mgmt.mib-2.printmib.(11)prtMarkerSupplies
//.1.3.6.1.2.1.43.11
//
BEGIN_ENUM_STRUCT(PrtMarkerSuppliesClassTC)
{
	Other = 1,
	SupplyThatIsConsumed = 3,	//供应已消费
	ReceptacleThatIsFilled = 4	//容器已填满
}
END_ENUM_STRUCT(PrtMarkerSuppliesClassTC)

//耗材类型
BEGIN_ENUM_STRUCT(PrtMarkerSuppliesTypeTC)
{
	//Values for Printer MIB
	Other = 1,
	UnKnown = 2,
	Toner = 3,
	WasteToner = 4,
	Ink = 5,
	InkCartridge = 6,
	InkRibbon = 7,
	WasteInk = 8,
	OPC = 9,		//有机光导体（Organic Photoconductor）的缩写	//感光鼓单元
	Developer = 10,	//显影单元
	FuserOil = 11,
	SolidWax = 12,
	RibbonWax = 13,
	WasteWax = 14,
	Fuser = 15,				//Not in RFC 1759
	CoronaWire = 16,		//Not in RFC 1759
	FuserOilWick = 17,		//Not in RFC 1759
	CleanerUnit = 18,		//Not in RFC 1759
	FuserCleaningPad = 19,	//Not in RFC 1759
	TransferUnit = 20,		//Not in RFC 1759
	TonerCartridge = 21,	//Not in RFC 1759
	TuserOiler = 22,		//Not in RFC 1759
	//End of values for Printer MIB
	//Values for Finisher MIB
	Water = 23,				//Not in RFC 1759
	WasteWater =24,			//Not in RFC 1759
	GlueWaterAdditive = 25,	//Not in RFC 1759
	WastePaper = 26,		//Not in RFC 1759
	BindingSupply = 27,		//Not in RFC 1759
	BandingSupply = 28,		//Not in RFC 1759
	StitchingWire = 29,		//Not in RFC 1759
	ShrinkWrap = 30,		//Not in RFC 1759
	PaperWrap = 31,			//Not in RFC 1759
	Staples = 32,			//Not in RFC 1759
	Inserts = 33,			//Not in RFC 1759
	Covers = 34				//Not in RFC 1759
	//End of values for Finisher MIB
}
END_ENUM_STRUCT(PrtMarkerSuppliesTypeTC)

BEGIN_ENUM_STRUCT(PrtMarkerSuppliesSupplyUnitTC)
{
	Other = 1,                  //New, not in RFC 1759
	UnKnown = 2,                //New, not in RFC 1759
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4,			//千分一毫米，微米
	Impressions = 7,			//印模(面，与Sheets页相对)			//New, not in RFC 1759
	Sheets = 8,					//纸张、页			//New, not in RFC 1759
	Hours = 11,					//小时				//New, not in RFC 1759
	ThousandthsOfOunces = 12,	//千分之一盎司
	TenthsOfGrams = 13,			//十分之一克
	HundrethsOfFluidOunces = 14,//百分之一盎司
	TenthsOfMilliliters = 15,	//十分之一毫升
	Feet = 16,					//英尺				//New, not in RFC 1759
	Meters = 17,				//米				//New, not in RFC 1759
	//Values for Finisher MIB
	Items = 18,                  //项				//New, not in RFC 1759	 // e.g., #staples
	Percent = 19                 //百分比			//New, not in RFC 1759
}
END_ENUM_STRUCT(PrtMarkerSuppliesSupplyUnitTC)

//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry
#define	PrtMarkerSuppliesEntryOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFL")												//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry"
#define	PrtMarkerSuppliesIndexOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaGgsHVys=")				//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesIndex"
#define	PrtMarkerSuppliesMarkerIndexOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaHgQRWTYGKBxXVBE=")		//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex"
#define	PrtMarkerSuppliesColorantIndexOID	TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaEAoPXSEVDwZ6Xw02HQ==")	//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesColorantIndex"
#define	PrtMarkerSuppliesClassOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaEAkCQSA=")				//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesClass"
#define	PrtMarkerSuppliesTypeOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaBxwTVw==")				//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesType"
#define	PrtMarkerSuppliesDescriptionOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaFwAQUSEdEQZaXgc=")		//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesDescription"
#define	PrtMarkerSuppliesSupplyUnitOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaABATQj8NNBxaRQ==")		//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesSupplyUnit"
#define	PrtMarkerSuppliesMaxCapacityOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaHgQbcTIEABFaRRA=")		//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMaxCapacity"
#define	PrtMarkerSuppliesLevelOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchJxQCQ10ANhZNQiEALBNBWgwhNhZCIxgIF0BlCDEJBhwjBhU/UkMCNhcwRyMEDRtWQiw9ERFLfQQTBn5QGzgAEWEmBBEeWlQaHwAVVz8=")				//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesLevel"
typedef struct tag_PrtMarkerSuppliesEntry
{
	Integer32 prtMarkerSuppliesIndex;							//(1)
	Integer32 prtMarkerSuppliesMarkerIndex;						//(2)
	Integer32 prtMarkerSuppliesColorantIndex;					//(3)
	ENUM_STRUCT_TYPE(PrtMarkerSuppliesClassTC) prtMarkerSuppliesClass;				//(4)
	ENUM_STRUCT_TYPE(PrtMarkerSuppliesTypeTC) prtMarkerSuppliesType;				//(5)
	OCTET_STRING prtMarkerSuppliesDescription[OCTET_STRING_SIZE_256];				//(6) PrtLocalizedDescriptionStringTC
	ENUM_STRUCT_TYPE(PrtMarkerSuppliesSupplyUnitTC) prtMarkerSuppliesSupplyUnit;	//(7)
	Integer32 prtMarkerSuppliesMaxCapacity;						//(8)
	Integer32 prtMarkerSuppliesLevel;							//(9)
}PrtMarkerSuppliesEntry, *PPrtMarkerSuppliesEntry;
typedef map<int, PPrtMarkerSuppliesEntry> PrtMarkerSuppliesEntryMap;
typedef list<PPrtMarkerSuppliesEntry> PrtMarkerSuppliesEntryList;
//(11)prtMarkerSupplies [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(12)prtMarkerColorant
//.1.3.6.1.2.1.43.12
//着色剂角色
BEGIN_ENUM_STRUCT(PrtMarkerColorantRoleTC)
{
	Other = 1,
	Process = 3,	//进行中
	Spot = 4		//污点
}
END_ENUM_STRUCT(PrtMarkerColorantRoleTC)

//printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry
#define	PrtMarkerColorantEntryOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchNw4eXEMIPRFNQiEALBNBWgwhJgxePAYAHEdlCDEJBhwjBhU/UkMCNhcgXT8bExNdRSw9ERFL")											//"printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry"
#define	PrtMarkerColorantIndexOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchNw4eXEMIPRFNQiEALBNBWgwhJgxePAYAHEdlCDEJBhwjBhU/UkMCNhcgXT8bExNdRSw9ERFLfQQTBn5QGzgAEXE8GA4AUl8dGgsHVys=")			//"printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantIndex"
#define	PrtMarkerColorantMarkerIndexOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchNw4eXEMIPRFNQiEALBNBWgwhJgxePAYAHEdlCDEJBhwjBhU/UkMCNhcgXT8bExNdRSw9ERFLfQQTBn5QGzgAEXE8GA4AUl8dHgQRWTYGKBxXVBE=")	//"printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantMarkerIndex"
#define	PrtMarkerColorantRoleOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchNw4eXEMIPRFNQiEALBNBWgwhJgxePAYAHEdlCDEJBhwjBhU/UkMCNhcgXT8bExNdRSw9ERFLfQQTBn5QGzgAEXE8GA4AUl8dAQoPVw==")			//"printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantRole"
#define	PrtMarkerColorantValueOID			TEXT("QRs6CxdfOhZPAkFFJDIXCFchNw4eXEMIPRFNQiEALBNBWgwhJgxePAYAHEdlCDEJBhwjBhU/UkMCNhcgXT8bExNdRSw9ERFLfQQTBn5QGzgAEXE8GA4AUl8dBQQPRzY=")			//"printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantValue"
#define	PrtMarkerColorantTonalityOID		TEXT("QRs6CxdfOhZPAkFFJDIXCFchNw4eXEMIPRFNQiEALBNBWgwhJgxePAYAHEdlCDEJBhwjBhU/UkMCNhcgXT8bExNdRSw9ERFLfQQTBn5QGzgAEXE8GA4AUl8dBwoNUz8dFQs=")		//"printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantTonality"
typedef struct tag_PrtMarkerColorantEntry
{
	Integer32 prtMarkerColorantIndex;							//(1)
	Integer32 prtMarkerColorantMarkerIndex;						//(2)
	ENUM_STRUCT_TYPE(PrtMarkerColorantRoleTC) prtMarkerColorantRole;	//(3)
	OCTET_STRING prtMarkerColorantValue[OCTET_STRING_SIZE_256];	//(4)
	Integer32 prtMarkerColorantTonality;						//(5)
}PrtMarkerColorantEntry, *PPrtMarkerColorantEntry;
typedef map<int, PPrtMarkerColorantEntry> PrtMarkerColorantEntryMap;
typedef list<PPrtMarkerColorantEntry> PrtMarkerColorantEntryList;
//(12)prtMarkerColorant [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(13)prtMediaPath
//.1.3.6.1.2.1.43.13
//媒体处理速度
BEGIN_ENUM_STRUCT(PrtMediaPathMaxSpeedPrintUnitTC)
{
	TenThousandthsOfInchesPerHour = 3,
	MicrometersPerHour = 4,
	CharactersPerHour = 5,
	LinesPerHour = 6,
	ImpressionsPerHour = 7,
	SheetsPerHour = 8,
	DotRowPerHour = 9,
	FeetPerHour = 16,
	MetersPerHour = 17
}
END_ENUM_STRUCT(PrtMediaPathMaxSpeedPrintUnitTC)

// The units of measure of media size for use in
// calculating and relaying dimensional values for all
// media paths in the printer.
BEGIN_ENUM_STRUCT(MediaPathMediaSizeUnit)
{
	TenThousandthsOfInches = 3,	//10千分之一英寸
	Micrometers = 4				//千分一毫米，微米
}
END_ENUM_STRUCT(MediaPathMediaSizeUnit)

//媒体类型
BEGIN_ENUM_STRUCT(PrtMediaPathTypeTC)
{
	Other = 1,
	UnKnown = 2,
	LongEdgeBindingDuplex = 3,		//双面长边
	ShortEdgeBindingDuplex = 4,		//双面短边
	Simplex = 5						//单面
}
END_ENUM_STRUCT(PrtMediaPathTypeTC)

//printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry
#define	PrtMediaPathEntryOID				TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQ")											//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry"
#define	PrtMediaPathIndexOID				TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDSpcNxEZ")					//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathIndex"
#define	PrtMediaPathMaxSpeedPrintUnitOID	TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5TKycRF1ZVOSEMDUYGGggG")	//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMaxSpeedPrintUnit"
#define	PrtMediaPathMediaSizeUnitOID		TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5XNx0AIVpLDAYLCkY=")		//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMediaSizeUnit"
#define	PrtMediaPathMaxSpeedOID				TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5TKycRF1ZV")				//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMaxSpeed"
#define	PrtMediaPathMaxMediaFeedDirOID		TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5TKzkEFlpQLzYAB3Y6Bg==")	//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMaxMediaFeedDir"
#define	PrtMediaPathMaxMediaXFeedDirOID		TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5TKzkEFlpQMRUABlYXHRM=")	//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMaxMediaXFeedDir"
#define	PrtMediaPathMinMediaFeedDirOID		TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5bPTkEFlpQLzYAB3Y6Bg==")	//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMinMediaFeedDir"
#define	PrtMediaPathMinMediaXFeedDirOID		TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDS5bPTkEFlpQMRUABlYXHRM=")	//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathMinMediaXFeedDir"
#define	PrtMediaPathTypeOID					TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDTdLIxE=")					//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathType"
#define	PrtMediaPathDescriptionOID			TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDSdXIBcTG0NFADwL")			//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathDescription"
#define	PrtMediaPathStatusOID				TEXT("QRs6CxdfOhZPAkFFJDYBClMDFRUaHUEbJygGVjoVMRNHWT0yBw9XfQQTBn5UDToEM1MnHCQcR0MQfRURRh4RBRtSYQgnDTBGMgAUAQ==")				//"printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry.prtMediaPathStatus"
typedef struct tag_PrtMediaPathEntry
{
	Integer32 prtMediaPathIndex;								//(1)
	ENUM_STRUCT_TYPE(PrtMediaPathMaxSpeedPrintUnitTC) prtMediaPathMaxSpeedPrintUnit;	//(2)
	ENUM_STRUCT_TYPE(PrtMediaUnitTC) prtMediaPathMediaSizeUnit;	//(3)
	Integer32 prtMediaPathMaxSpeed;								//(4)
	Integer32 prtMediaPathMaxMediaFeedDir;						//(5)
	Integer32 prtMediaPathMaxMediaXFeedDir;						//(6)
	Integer32 prtMediaPathMinMediaFeedDir;						//(7)
	Integer32 prtMediaPathMinMediaXFeedDir;						//(8)
	ENUM_STRUCT_TYPE(PrtMediaPathTypeTC) prtMediaPathType;		//(9)
	OCTET_STRING prtMediaPathDescription[OCTET_STRING_SIZE_256];	//(10) PrtLocalizedDescriptionStringTC,
	Integer32 prtMediaPathStatus;								//(11) PrtSubUnitStatusTC
}PrtMediaPathEntry, *PPrtMediaPathEntry;
typedef map<int, PPrtMediaPathEntry> PrtMediaPathEntryMap;
typedef list<PPrtMediaPathEntry> PrtMediaPathEntryList;
//(13)prtMediaPath [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(14)prtChannel
//.1.3.6.1.2.1.43.14
//通道类型
BEGIN_ENUM_STRUCT(PrtChannelTypeTC)
{
	Other = 1,
	chSerialPort = 3,
	chParallelPort = 4,
	chIEEE1284Port = 5,
	chSCSIPort = 6,
	chAppleTalkPAP = 7,
		// AppleTalk Printer
		// Access Protocol (PAP)
		//
		// prtChannelInformation entry:
		//
		// Printer Name
		//   Keyword:      Name
		//   Syntax:       Name
		//   Status:       Optional
		//   Multiplicity: Single
		//   Description:  The name of the printer
		//     within the AppleTalk naming scope
	chLPDServer = 8,
		// prtChannelInformation entry:
		//
		// Printer queue name
		//   Keyword:      Queue
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description: queue name as
		//     defined in [RFC1179].
	chNetwareRPrinter = 9,
		// Novell, Inc.
		// For each entry of this type, the
		// prtChannelInformation must have a pair of
		// keywords.  For Netware 3.x channels this must
		// be a (PServer, Printer) pair.  For Netware
		// 4.x channels and for IntranetWare channels
		// this must be a (NDSTree, NDSPrinter) pair.
		//
		// prtChannelInformation entries:

		// Print Server Name
		//   Keyword:      PServer
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The Pserver's SAP name
		//
		// Printer Number
		//   Keyword:      Printer
		//   Syntax:       Integer
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The printer number
		//
		// NDSTree
		//   Keyword:      NDSTree
		//   Syntax:       Name
		//   Multiplicity: Single
		//   Description:  The tree's SAP name
		// NDS Printer object
		//   Keyword:      NDSPrinter
		//   Syntax:       Text (Unicode)
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The fully qualified
		//                 name of the Printer
		//
		// In the Netware 3.x environment, the
		// client checks the Bindery object
		// representing the named PServer.  The
		// client then checks for queues which
		// are associated with the numbered
		// printer.  In the 4.x and IntraNetware
		// environment, the client looks up the
		// queues which are associated with the
		// NDS Printer Object in the named Tree.
		// Depending on client access rights to
		// those queues, the client submits jobs
		// to the appropriate queue.
	chNetwarePServer = 10,
		// Novell,Inc.
		// For each entry of this type, the
		// prtChannelInformation must have a pair
		// of keywords.  For Netware 3.x channels
		// this must be a (Server, PServer) pair.
		// For Netware 4.x and IntranetWare
		// channels, this must be a
		// (NDSTree, NDSPServer) pair.
		//
		// prtChannelInformation entries:
		//
		// Server Name
		//   Keyword:      Server
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The SAP name of the
		//   server for which the PServer is defined.
		//
		// PServer
		//   Keyword:      PServer
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The bindery name of
		//                 the PServer
		//
		// NDS Tree
		//   Keyword:      NDSTree
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The NDS Tree name
		//
		// PServer
		//   Keyword:      NDSPServer
		//   Syntax:       Text (Unicode)
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The fully qualified
		//     name of the PServer object in the tree.
		//
		// In the 3.x environment, the client
		// checks the bindery object
		// representing the named PServer on the
		// named Server.  In the 4.x and
		// IntranetWare environment,
		// the client checks the NDS object
		// representing the named PServer in the
		// named Tree.  In either case, the
		// client then checks for all queues
		// associated with the Pserver object.
		// Depending on client access rights
		// to those queues, the client submits
		// jobs to the appropriate queue.
	chPort9100 = 11,
		// DEPRECATED
		// (see chPortTCP - 37; chBidirPortTCP - 38)
	chAppSocket = 12,
		// A bi-directional, LPD-like, protocol using
		// 9101 for control and 9100 for data.
		// Adobe Systems, Inc.
	chFTP = 13,         // [RFC959]
	chTFTP = 14,        // [RFC1350]
	chDLCLLCPort = 15,
	chIBM3270 = 16,     // IBM Coax
	chIBM5250 = 17,     // IBM Twinax
	chFax = 18,
	chIEEE1394 = 19,
	chTransport1 = 20,
		// TCP port 35, for reserved TCP port list see
		// [RFC3232].  This RFC should also be
		// referenced for other channel
		// enumerations utilizing TCP port
		// numbers 0 through 1024.
	chCPAP = 21,      // TCP port 170
		// Digital Equipment Corp.
	chDCERemoteProcCall = 22, // OSF
		// DEPRECATED
	chONCRemoteProcCall = 23, // SUN Microsystems
		// DEPRECATED
	chOLE = 24,         // Microsoft
		// DEPRECATED
	chNamedPipe = 25,
	chPCPrint = 26,   // Banyan
	chServerMessageBlock = 27,
		// File/Print sharing protocol used by
		// various network operating systems
		// from IBM 3Com, Microsoft and others
		//
		// prtChannelInformation entry:
		//
		// Service Name
		//   Keyword:      Name
		//   Syntax:       Name
		//   Status:       Optional
		//   Multiplicity: Single
		//   Description:  The service name of
		//                 the printer
	chDPMF = 28,   // IBM Infoprint
	chDLLAPI = 29, // Microsoft
		// DEPRECATED
	chVxDAPI = 30, // Microsoft
		// DEPRECATED
	chSystemObjectManager = 31, // IBM
	chDECLAT = 32,
		// Digital Equipment Corp.
		//
		// prtChannelInformation entries:
		//
		// Port Name
		//   Keyword:      Port
		//   Syntax:       Name
		//   Status:       Conditionally
		//                 Mandatory
		//                 (see note below)
		//   Multiplicity: Single
		//   Description:  LAT port name
		//
		// Service Name
		//   Keyword:      Service
		//   Syntax:       Name
		//   Status:       Conditionally
		//                 Mandatory
		//   Multiplicity: Single
		//   Description:  LAT service name
		//
		// The LAT channel may be
		// identified by either a port or
		// service, so either a
		// Port or Service entry must be
		// specified, but not both.
	chNPAP = 33,
	chUSB = 34, // Not in RFC 1759
		// Universal Serial Bus
	chIRDA = 35, // Not in RFC 1759
		// Infrared Data Assoc. Prot.
	chPrintXChange = 36,  // Not in RFC 1759
		// PrintXChange Protocol
	chPortTCP = 37,  // Not in RFC 1759
		// A unidirectional "raw" TCP
		// channel that uses an administratively
		// assigned TCP port address.
		//
		// prtChannelInformation entry:
		//
		// Port Number
		//   Keyword:      Port
		//   Syntax:       decimal number
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  TCP port number
	chBidirPortTCP = 38,  // Not in RFC 1759
		// A bi-directional version of chPortTCP
		//
		// prtChannelInformation entries:
		// (See chPortTCP)
	chUNPP = 39,  // Not in RFC 1759
		// Universal Network Printing
		// Protocol(UNPP). A bi-directional,
		// multiport network printing
		// application protocol available on
		// multiple transport protocols.
		// Underscore, Inc.
		// Contact: info@underscore.com
	chAppleTalkADSP = 40,  // Not in RFC 1759
		// AppleTalk Data Stream Protocol.
		// ADSP is part of the AppleTalk
		// suite of protocols.
		// It is a symmetric, connection-
		// oriented protocol that makes
		// possible the establishment
		// and maintenance of full-duplex
		// streams of data bytes between
		// two sockets in an AppleTalk
		// internet.
		// See [APPLEMAC].
	chPortSPX = 41,  // Not in RFC 1759
		// Sequenced Packet Exchange (SPX)
		// socket.
		// Novell, Inc. Similar to TCP, a
		// bi-directional data pipe using
		// Novell SPX as a transport.
		//
		// prtChannelInformation entries:
		//
		// Network Number
		//   Keyword:      Net
		//   Syntax:       HexString
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The network number
		//
		// Node Number
		//   Keyword:      Node
		//   Syntax:       HexString
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The node number
		//
		// Socket Number
		//   Keyword:      Socket
		//   Syntax:       HexString
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The SPX socket number
		//
		// There must be exactly one "Net" and
		// one "Node" and one "Socket" entry.  A
		// HexString is a binary value
		// represented as a string of
		// ASCII characters using hexadecimal
		// notation.
	chPortHTTP = 42,  // Not in RFC 1759
		// Hypertext Transfer Protocol. See [RFC1945]
	// and [RFC2616].
	chNDPS = 43,  // Not in RFC 1759
		// Novell, Inc.
		//
		// prtChannelInformation entry:
		//
		// Printer Agent Name
		//   Keyword:      PA
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Description:  The NDPS Printer
		//                 Agent Name
	chIPP = 44,  // Not in RFC 1759
		// Internet Printing Protocol (IPP),
		// (IPP/1.1 - see [RFC2910] and [RFC2911])
		// also applies to all future versions of IPP.
		//
		// IPP Printer URI
		//   Keyword:      URI
		//   Syntax:       URI (Unicode UTF-8 per
		//                 [RFC2396])
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Default:      not applicable
		//   Description:  URI of this IPP Printer
		//     within Internet naming scope.  Unicode
		//     UTF-8 [RFC3629] string with
		//     hexadecimal escapes for any non-ASCII
		//     characters (per [RFC2396]).
		//   Conformance: An IPP Printer shall list all
		//     IPP URI it supports (one per IPP Channel
		//     entry).  If a URI contains the 'http:'
		//     scheme it must have an explicit port.
		//   See: [RFC3629], [RFC2396], [RFC2910],
		//     [RFC2911].
		//
		// IPP Printer Client Authentication
		//   Keyword:      Auth
		//   Syntax:       Keyword
		//   Status:       Optional
		//   Multiplicity: Single
		//   Default:      'none'
		//   Description:  A client authentication
		//     mechanism supported for this IPP Printer
		//     URI:
		//       'none'
		//         no client authentication mechanism
		//       'requesting-user-name'
		//         authenticated user in 'requesting-
		//         user-name'
		//       'basic'
		//         authenticated user via HTTP Basic
		//         mechanism
		//       'digest'
		//         authenticated user via HTTP Digest
		//         mechanism
		//       'certificate'
		//         authenticated user via certificate
		//         mechanism
		//   Conformance: An IPP Printer should list
		//     all IPP client authentication mechanisms
		//     it supports (one per IPP Channel entry).
		//     See: [RFC2911] and [RFC2910].
		//
		// IPP Printer Security
		//   Keyword:      Security
		//   Syntax:       Keyword
		//   Status:       Optional
		//   Multiplicity: Single
		//   Default:      'none'
		//   Description:  A security mechanism
		//     supported for this IPP Printer URI:
		//     'none'
		//       no security mechanism
		//     'ssl3'
		//       SSL3 secure communications channel
		//       protocol
		//     'tls'
		//       TLS secure communications channel
		//       protocol
		//   Conformance: An IPP Printer should list
		//     all IPP security mechanisms it supports
		//     (one per IPP Channel entry).
		//   See: [RFC2246], [RFC2911].
		//
		// IPP Printer Protocol Version
		//   Keyword:      Version
		//   Syntax:       Keyword
		//   Status:       Optional
		//   Multiplicity: Multiple
		//   Default:      '1.1'
		//   Description:  All of the IPP protocol
		//     versions (major.minor) supported for
		//     this IPP Printer URI:
		//     '1.0'
		//       IPP/1.0 conforming Printer
		//     '1.1'
		//       IPP/1.1 conforming Printer
		//   Conformance:  An IPP Printer should list
		//     all IPP versions it supports (all listed
		//     in each IPP Channel entry).  An IPP
		//     Client should select the highest
		//     numbered version the IPP Client supports
		//     for use in all IPP Requests (for optimum
		//     interworking).
		//   See: [RFC2911].
	chSMTP = 45
		// Print Job submission via Simple Mail
		// Transfer Protocol (SMTP) - see [RFC2821]
		//
		// prtChannelInformation entry:
		//
		//   Keyword:      Mailto
		//   Syntax:       Name
		//   Status:       Mandatory
		//   Multiplicity: Single
		//   Default:      not applicable
		//   Description:  The SMTP URL of the printer.
}
END_ENUM_STRUCT(PrtChannelTypeTC)

//通道状态
BEGIN_ENUM_STRUCT(PrtChannelStateTC)
{
	Other = 1,
	PrintDataAccepted = 3,
	NoDataAccepted = 4
}
END_ENUM_STRUCT(PrtChannelStateTC)

//printmib.prtChannel.prtChannelTable.prtChannelEntry
#define	PrtChannelEntryOID						TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0Aq")												//"printmib.prtChannel.prtChannelTable.prtChannelEntry"
#define	PrtChannelIndexOID						TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz89DxZWSQ==")						//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelIndex"
#define	PrtChannelTypeOID						TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz8gGAJW")							//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelType"
#define	PrtChannelProtocolVersionOID			TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz8kEx1HXgo8CTVXIQcIHV0=")			//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelProtocolVersion"
#define	PrtChannelCurrentJobCntlLangIndexOID	TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz83FABBVAcnLwxQEBoVHn9QBzQsDVY2DA==")	//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelCurrentJobCntlLangIndex"
#define	PrtChannelDefaultPageDescLangIndexOID	TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz8wBBRSRAUnNQJVNjAEAVB9CD0CKlw3ERk=")	//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelDefaultPageDescLangIndex"
#define	PrtChannelStateOID						TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz8nFRNHVA==")						//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelState"
#define	PrtChannelIfIndexOID					TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz89BztdVQwr")						//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelIfIndex"
#define	PrtChannelStatusOID						TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz8nFRNHRBo=")						//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelStatus"
#define	PrtChannelInformationOID				TEXT("QRs6CxdfOhZPAkFFKjsEDVw2GE8CQUUqOwQNXDYYNRNRXQx9FRFGEBwAHF1UBRYLF0AqWhEAR3IBMgsNVz89DxRcQwQyEQpdPQ==")				//"printmib.prtChannel.prtChannelTable.prtChannelEntry.prtChannelInformation"
typedef struct tag_PrtChannelEntry
{
	Integer32 prtChannelIndex;										//(1)
	ENUM_STRUCT_TYPE(PrtChannelTypeTC) prtChannelType;				//(2)
	OCTET_STRING prtChannelProtocolVersion[OCTET_STRING_SIZE_64];	//(3)
	Integer32 prtChannelCurrentJobCntlLangIndex;					//(4)
	Integer32 prtChannelDefaultPageDescLangIndex;					//(5)
	ENUM_STRUCT_TYPE(PrtChannelStateTC) prtChannelState;			//(6)
	Integer32 prtChannelIfIndex;									//(7) InterfaceIndexOrZero,
	Integer32 prtChannelStatus;										//(8) PrtSubUnitStatusTC,
	OCTET_STRING prtChannelInformation[OCTET_STRING_SIZE_256];		//(9) 此处字段是版本1中是不存在的，使用编号，不能使用名称
}PrtChannelEntry, *PPrtChannelEntry;
typedef map<int, PPrtChannelEntry> PrtChannelEntryMap;
typedef list<PPrtChannelEntry> PrtChannelEntryList;
//(14)prtChannel [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(15)prtInterpreter
//.1.3.6.1.2.1.43.15
//解释器语言家族
BEGIN_ENUM_STRUCT(PrtInterpreterLangFamilyTC)
{
	Other = 1,
	UnKnown = 2,          // Not in RFC 1759
	langPCL = 3,          // PCL.  Starting with PCL version 5,
	// HP-GL/2 is included as part of the
	// PCL language.
	// PCL and HP-GL/2 are registered
	// trademarks of Hewlett-Packard
	// Company.
	langHPGL = 4,         // Hewlett-Packard Graphics Language.
	// HP-GL is a registered trademark of
	// Hewlett-Packard Company.
	langPJL = 5,          // Peripheral Job Language.  Appears in
	// the data stream between data intended
	// for a page description language.
	// Hewlett-Packard Co.
	langPS = 6,           // PostScript (tm) Language
		// Postscript - a trademark of Adobe
		// Systems Incorporated which may be
		// registered in certain jurisdictions
	langIPDS = 7,         // Intelligent Printer Data Stream
		// Bi-directional print data stream for
		// documents consisting of data objects
		// (text, image, graphics, bar codes),
		// resources (fonts, overlays) and page,
		// form and finishing instructions.
		// Facilitates system level device
		// control, document tracking and error
		// recovery throughout the print
		// process.
		// IBM Corporation.
	langPPDS = 8,         // IBM Personal Printer Data Stream.
		// Originally called IBM ASCII, the name
		// was changed to PPDS when the Laser
		// Printer was introduced in 1989.
		// Lexmark International, Inc.
	langEscapeP = 9,      // Epson Corp.
	langEpson = 10,
	langDDIF = 11,        // Digital Document Interchange Format
		// Digital Equipment Corp., Maynard MA
	langInterpress = 12,
		// Xerox Corp.
	langISO6429 = 13,     // ISO 6429.  Control functions for
		// Coded Character Sets (has ASCII
		// control characters, plus additional
		// controls for
		// character imaging devices.)
	langLineData = 14,    // line-data:  Lines of data as
		// separate ASCII or EBCDIC records
		// and containing no control functions
		// (no CR, LF, HT, FF, etc.)
		// For use with traditional line
		// printers.  May use CR and/or LF to
		// delimit lines, instead of records.
		// See ISO 10175 Document Printing
		// Application (DPA) [ISO10175].
	langMODCA = 15,       // Mixed Object Document Content
		// Architecture
		// Definitions that allow the
		// composition, interchange, and
		// presentation of final form
		// documents as a collection of data
		// objects (text, image, graphics, bar
		// codes), resources (fonts, overlays)
		// and page, form and finishing
		// instructions.
		// IBM Corporation.
	langREGIS = 16,       // Remote Graphics Instruction Set,
		// Digital Equipment Corp., Maynard MA
	langSCS = 17,         // SNA Character String
		// Bi-directional print data stream for
		// SNA LU-1 mode of communication.
		// IBM
	langSPDL = 18,        // ISO 10180 Standard Page Description
		// Language
		// ISO Standard
	langTEK4014 = 19,     // Tektronix Corp.
	langPDS = 20,
	langIGP = 21,         // Printronix Corp.
	langCodeV = 22,       // Magnum Code-V, Image and printer
		// control language used to control
		// impact/dot-matrix printers.
		// QMS, Inc., Mobile AL
	langDSCDSE = 23,      // DSC-DSE:  Data Stream Compatible and
		// Emulation Bi-directional print data
		// stream for non-SNA (DSC) and SNA LU-3
		// 3270 controller (DSE) communications
		// IBM
	langWPS = 24,         // Windows Printing System, Resource
		// based command/data stream used by
		// Microsoft At Work Peripherals.
		// Developed by the Microsoft
		// Corporation.
	langLN03 = 25,        // Early DEC-PPL3, Digital Equipment
		// Corp.
	langCCITT = 26,
	langQUIC = 27,        // QUIC (Quality Information Code), Page
		// Description Language for laser
		// printers.  Included graphics, printer
		// control capability and emulation of
		// other well-known printer.
		// QMS, Inc.
	langCPAP = 28,        // Common Printer Access Protocol
		// Digital Equipment Corp.
	langDecPPL = 29,      // Digital ANSI-Compliant Printing
		// Protocol
		// (DEC-PPL)
		// Digital Equipment Corp.
	langSimpleText = 30,
		// simple-text:  character coded data,
		// including NUL, CR , LF, HT, and FF
		// control characters.  See ISO 10175
		// Document Printing Application (DPA)
		// [ISO10175].
	langNPAP = 31,        // Network Printer Alliance Protocol
		// (NPAP).  This protocol has been
		// superseded by the IEEE 1284.1 TIPSI
		// Std (ref. LangTIPSI(49)).
	langDOC = 32,         // Document Option Commands, Appears in
		// the data stream between data
		// intended for a page description.
		// QMS, Inc.
	langimPress = 33,     // imPRESS, Page description language
		// originally developed for the
		// ImageServer product line. A binary
		// language providing representations
		// of text, simple graphics, and some
		// large forms (simple
		// bit-map and CCITT group 3/4
		// encoded).The
		// language was intended to be sent over
		// an 8-bit channel and supported early
		// document preparation languages (e.g.,
		// TeX and TROFF).
		// QMS, Inc.
	langPinwriter = 34,
		// 24 wire dot matrix printer  for
		// USA, Europe, and Asia except
		// Japan.
		// More widely used in Germany, and
		// some Asian countries than in US.
		// NEC
	langNPDL = 35,        // Page printer  for Japanese market.
		// NEC
	langNEC201PL = 36,    // Serial printer language used in
		// the Japanese market.
		// NEC
	langAutomatic = 37,
		// Automatic PDL sensing.  Automatic
		// sensing of the interpreter
		// language family by the printer
		// examining the document content.
		// Which actual interpreter language
		// families are sensed depends on
		// the printer implementation.
	langPages = 38,       // Page printer Advanced Graphic
		// Escape Set
		// IBM Japan
	langLIPS = 39,        // LBP Image Processing System
	langTIFF = 40,        // Tagged Image File Format (Aldus)
	langDiagnostic = 41,
		// A hex dump of the input to the
		// interpreter
	langPSPrinter = 42,
		// The PostScript Language used for
		// control (with any PDLs)
		// Adobe Systems Incorporated
	langCaPSL = 43,       // Canon Print Systems Language
	langEXCL = 44,        // Extended Command Language
		// Talaris Systems Inc.
	langLCDS = 45,        // Line Conditioned Data Stream
		// Xerox Corporation
	langXES = 46,         // Xerox Escape Sequences
		// Xerox Corporation
	langPCLXL = 47,       // Not in RFC 1759
		// Printer Control Language. Extended
		// language features for printing, and
		// printer control.
		// Hewlett-Packard Co.
	langART = 48,         // Not in RFC 1759
		// Advanced Rendering Tools (ART).
		// Page Description language
		// originally developed for the Laser
		// Press printers.
		// Technical reference manual: "ART IV
		// Reference Manual", No F33M.
		// Fuji Xerox Co., Ltd.
	langTIPSI = 49,       // Not in RFC 1759
		// Transport Independent Printer
		// System Interface (ref. IEEE Std.
		// 1284.1)
	langPrescribe = 50,   // Not in RFC 1759
		// Page description and printer
		// control language. It can be
		// described with ordinary ASCII
		// Technical reference manual:
	// "PRESCRIBE II Programming Manual"
	langLinePrinter = 51, // Not in RFC 1759
		// A simple-text character stream which
		// supports the control codes LF, VT,
		// FF, and plus Centronics or
		// Dataproducts Vertical Format Unit
		// (VFU) language is commonly used on
		// many older model line and matrix
		// printers.
	langIDP = 52,         // Not in RFC 1759
		// Imaging Device Protocol
		// Apple Computer.
	langXJCL = 53,        // Not in RFC 1759
		// Xerox Job Control Language (JCL).
		// A Job Control language originally
		// developed for the LaserPress printers
		// and is capable of switching PDLs.
		// Technical reference manual:
	// "ART IV Reference Manual", No F33M.
		// Fuji Xerox Co., Ltd.
	langPDF = 54,         // Not in RFC 1759
		// Adobe Portable Document Format
		// Adobe Systems, Inc.
	langRPDL = 55,        // Not in RFC 1759
		// Ricoh Page Description Language for
		// printers.
		// Technical manual "RPDL command
		// reference" No.307029
		// RICOH, Co. LTD
	langIntermecIPL = 56, // Not in RFC 1759
		// Intermec Printer Language for label
		// printers.
		// Technical Manual: "IPL Programmers
		// Reference Manual"
		// Intermec Corporation
	langUBIFingerprint = 57,  // Not in RFC 1759
		// An intelligent basic-like programming
		// language for label printers.
		// Reference Manual: "UBI Fingerprint
		// 7.1", No. 1-960434-00
		// United Barcode Industries
	langUBIDirectProtocol = 58,  // Not in RFC 1759
		// An intelligent control language for
		// label printers.
		// Programmers guide: " UBI Direct
		// Protocol", No. 1-960419-00
		// United Barcode Industries
	langFujitsu = 59,     // Not in RFC 1759
		// Fujitsu Printer Language
		// Reference Manual:
	// "FM Printer Sequence" No. 80HP-0770
		// FUJITSU LIMITED
	langCGM = 60,         // Not in RFC 1759
		// Computer Graphics Metafile
		// MIME type 'image/cgm'
	langJPEG = 61,        // Not in RFC 1759
		// Joint Photographic Experts Group
		// MIME type 'image/jpeg'
	langCALS1 = 62,       // Not in RFC 1759
		// US DOD CALS1 (see MIL-STD-1840)
		// MIME type 'application/cals-1840'
	langCALS2 = 63,       // Not in RFC 1759
		// US DOD CALS2 (see MIL-STD-1840)
		// MIME type 'application/cals-1840'
	langNIRS = 64,        // Not in RFC 1759
		// US DOD NIRS (see MIL-STD-1840)
		// MIME type 'application/cals-1840'
	langC4 = 65           // Not in RFC 1759
		// US DOD C4 (see MIL-STD-1840)
		// MIME type 'application/cals-1840'
}
END_ENUM_STRUCT(PrtInterpreterLangFamilyTC)

//解释器默认方向
BEGIN_ENUM_STRUCT(PrtPrintOrientationTC)
{
	Other = 1,
	Portrait = 3,	//竖向
	Landscape = 4	//横向
}
END_ENUM_STRUCT(PrtPrintOrientationTC)

//Indicates whether or not this interpreter returns
//information back to the host.
BEGIN_ENUM_STRUCT(PrtInterpreterTwoWayTC)
{
	Yes = 3,
	No = 4
}
END_ENUM_STRUCT(PrtInterpreterTwoWayTC)

//printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry
#define	PrtInterpreterEntryOID					TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiEN")													//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry"
#define	PrtInterpreterIndexOID					TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDID0BBko=")						//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterIndex"
#define	PrtInterpreterLangFamilyOID				TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDJTILBHQyGQgeSg==")				//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterLangFamily"
#define	PrtInterpreterLangLevelOID				TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDJTILBH42AgQe")					//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterLangLevel"
#define	PrtInterpreterLangVersionOID			TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDJTILBGQ2BhIbXF8=")				//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterLangVersion"
#define	PrtInterpreterDescriptionOID			TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDLTYWAEA6BBUbXF8=")				//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterDescription"
#define	PrtInterpreterVersionOID				TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDPzYXEFs8Gg==")					//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterVersion"
#define	PrtInterpreterDefaultOrientationOID		TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDLTYDAkc/AC4AWlQHJwQXWzwa")		//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterDefaultOrientation"
#define	PrtInterpreterFeedAddressabilityOID		TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDLzYAB3M3EBMXQEIIMQwPWycN")		//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterFeedAddressability"
#define	PrtInterpreterXFeedAddressabilityOID	TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDMRUABlYSEAUAVkIaMgcKXjoAGA==")	//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterXFeedAddressability"
#define	PrtInterpreterDefaultCharSetInOID		TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDLTYDAkc/ACIaUkM6NhEqXA==")		//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterDefaultCharSetIn"
#define	PrtInterpreterDefaultCharSetOutOID		TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDLTYDAkc/ACIaUkM6NhEsRyc=")		//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterDefaultCharSetOut"
#define	PrtInterpreterTwoWayOID					TEXT("QRs6CxdfOhZPAkFFID0RBkAjBgQGVkNHIxcXez0ABABDQwwnABFmMhYNFx1BGycsDUY2BhEAVkUMISANRiENTwJBRSA9EQZAIwYEBlZDPSQKNFMq")						//"printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry.prtInterpreterTwoWay"
typedef struct tag_PrtInterpreterEntry
{
	Integer32 prtInterpreterIndex;											//(1)
	ENUM_STRUCT_TYPE(PrtInterpreterLangFamilyTC) prtInterpreterLangFamily;	//(2)
	OCTET_STRING prtInterpreterLangLevel[OCTET_STRING_SIZE_32];				//(3)
	OCTET_STRING prtInterpreterLangVersion[OCTET_STRING_SIZE_32];			//(4)
	OCTET_STRING prtInterpreterDescription[OCTET_STRING_SIZE_256];			//(5) PrtLocalizedDescriptionStringTC,
	OCTET_STRING prtInterpreterVersion[OCTET_STRING_SIZE_32];				//(6)
	ENUM_STRUCT_TYPE(PrtPrintOrientationTC) prtInterpreterDefaultOrientation;	//(7)
	Integer32 prtInterpreterFeedAddressability;								//(8)
	Integer32 prtInterpreterXFeedAddressability;							//(9)
	ENUM_STRUCT_TYPE(IANACharset) prtInterpreterDefaultCharSetIn;			//(10)
	ENUM_STRUCT_TYPE(IANACharset) prtInterpreterDefaultCharSetOut;			//(11)
	ENUM_STRUCT_TYPE(PrtInterpreterTwoWayTC) prtInterpreterTwoWay;			//(12)
}PrtInterpreterEntry, *PPrtInterpreterEntry;
typedef map<int, PPrtInterpreterEntry> PrtInterpreterEntryMap;
typedef list<PPrtInterpreterEntry> PrtInterpreterEntryList;
//(15)prtInterpreter [end]

//iso.org.dod.internet.mgmt.mib-2.printmib.(16)prtConsoleDisplayBuffer
//.1.3.6.1.2.1.43.16
//printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry
#define	PrtConsoleDisplayBufferEntryOID		TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ESUbQEEFMhwhRzUSBAAdQRsnJgxcIBsNF3dYGiMJAksRAQcUVkM9MgcPV30EEwZwXgcgCg9XFx0SAl9QEBEQBVQ2BiQcR0MQ")	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry"
#define	PrtConsoleDisplayBufferIndexOID		TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ESUbQEEFMhwhRzUSBAAdQRsnJgxcIBsNF3dYGiMJAksRAQcUVkM9MgcPV30EEwZwXgcgCg9XFx0SAl9QEBEQBVQ2BiQcR0MQfRURRhAbDwFcXQwXDBBCPxUYMEZXDzYXKlw3ERk=")	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferIndex"
#define	PrtConsoleDisplayBufferTextOID		TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ESUbQEEFMhwhRzUSBAAdQRsnJgxcIBsNF3dYGiMJAksRAQcUVkM9MgcPV30EEwZwXgcgCg9XFx0SAl9QEBEQBVQ2BiQcR0MQfRURRhAbDwFcXQwXDBBCPxUYMEZXDzYXN1crAA==")	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferText"
typedef struct tag_PrtConsoleDisplayBufferEntry
{
	Integer32 prtConsoleDisplayBufferIndex;								//(1)
	OCTET_STRING prtConsoleDisplayBufferText[OCTET_STRING_SIZE_256];	//(2) PrtConsoleDescriptionStringTC
}PrtConsoleDisplayBufferEntry, *PPrtConsoleDisplayBufferEntry;
typedef map<int, PPrtConsoleDisplayBufferEntry> PrtConsoleDisplayBufferEntryMap;
typedef list<PPrtConsoleDisplayBufferEntry> PrtConsoleDisplayBufferEntryList;
//(16)prtConsoleDisplayBuffer [end]


//iso.org.dod.internet.mgmt.mib-2.printmib.(17)prtConsoleLights
//.1.3.6.1.2.1.43.17
//
BEGIN_ENUM_STRUCT(PrtConsoleColorTC)
{
	Other = 1,
	UnKnown = 2,
	White = 3,
	Red = 4,
	Green = 5,
	Blue = 6,
	Cyan = 7,
	Magenta = 8,
	Yellow = 9,
	Orange = 10        // Not in RFC 1759
}
END_ENUM_STRUCT(PrtConsoleColorTC)

//printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry
#define	PrtConsoleLightEntryOID				TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ES0bVFkdfRURRhAbDwFcXQwfDARaJyAAEF9URyMXF3E8GhIdX1QlOgILRhYaFQBK")								//"printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry"
#define	PrtConsoleLightIndexOID				TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ES0bVFkdfRURRhAbDwFcXQwfDARaJyAAEF9URyMXF3E8GhIdX1QlOgILRhYaFQBKHxkhESBdPQcOHlZ9ADQNF3s9EAQK")	//"printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry.prtConsoleLightIndex"
#define	PrtConsoleOnTimeOID					TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ES0bVFkdfRURRhAbDwFcXQwfDARaJyAAEF9URyMXF3E8GhIdX1QlOgILRhYaFQBKHxkhESBdPQcOHlZ+BwcMDlc=")		//"printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry.prtConsoleOnTime"
#define	PrtConsoleOffTimeOID				TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ES0bVFkdfRURRhAbDwFcXQwfDARaJyAAEF9URyMXF3E8GhIdX1QlOgILRhYaFQBKHxkhESBdPQcOHlZ+DzUxCl82")		//"printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry.prtConsoleOffTime"
#define	PrtConsoleColorOID					TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ES0bVFkdfRURRhAbDwFcXQwfDARaJyAAEF9URyMXF3E8GhIdX1QlOgILRhYaFQBKHxkhESBdPQcOHlZyBj8KEQ==")		//"printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry.prtConsoleColor"
#define	PrtConsoleDescriptionOID			TEXT("QRs6CxdfOhZPAkFFKjwLEF0/ES0bVFkdfRURRhAbDwFcXQwfDARaJyAAEF9URyMXF3E8GhIdX1QlOgILRhYaFQBKHxkhESBdPQcOHlZ1DCAGEVsjAAgdXQ==")//"printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry.prtConsoleDescription"
typedef struct tag_PrtConsoleLightEntry
{
	Integer32 prtConsoleLightIndex;								//(1)
	Integer32 prtConsoleOnTime;									//(2)
	Integer32 prtConsoleOffTime;								//(3)
	ENUM_STRUCT_TYPE(PrtConsoleColorTC) prtConsoleColor;		//(4)
	OCTET_STRING prtConsoleDescription[OCTET_STRING_SIZE_256];	//(5) PrtConsoleDescriptionStringTC
}PrtConsoleLightEntry, *PPrtConsoleLightEntry;
typedef map<int, PPrtConsoleLightEntry> PrtConsoleLightEntryMap;
typedef list<PPrtConsoleLightEntry> PrtConsoleLightEntryList;
//(17)prtConsoleLights [end]


//iso.org.dod.internet.mgmt.mib-2.printmib.(18)prtAlert
//.1.3.6.1.2.1.43.18
//
// The level of severity of this alert table entry.  The
// printer determines the severity level assigned to each
// entry into the table.
// 预警严重级别
BEGIN_ENUM_STRUCT(PrtAlertSeverityLevelTC)
{
	Other = 1,
	CriticalBinaryChangeEvent = 3,
	WarningUnaryChangeEvent = 4,
	WarningBinaryChangeEvent = 5   // New, not in RFC 1759
}
END_ENUM_STRUCT(PrtAlertSeverityLevelTC)

//See textual convention PrtAlertTrainingLevelTC
// PrtAlertTrainingLevelTC ::= TEXTUAL-CONVENTION
//	 This TC was extracted from prtAlertTrainingLevel in RFC 1759.
// STATUS    current
// DESCRIPTION
// "The level of training required to handle this alert, if
// human intervention is required.  The noInterventionRequired
// value should be used if the event does not require any human
// intervention.  The training level is an enumeration that is
// determined and assigned by the printer manufacturer based on
// the information or training required to handle this alert.
// The printer will break alerts into these different training
// levels.  It is the responsibility of a management application
// in the system to determine how a particular alert is handled
// and how and to whom that alert is routed.  The following are
// the four training levels of alerts:
// 
// Field Service - Alerts that typically require advanced
// training and technical knowledge of the printer and its
// subunits.  An example of a technical person would be a
// manufacturer's Field Service representative, or other
// person formally trained by the manufacturer or similar
// representative.
// Trained - Alerts that require an intermediate or moderate
// knowledge of the printer and its subunits.  A typical
// example of such an alert is replacing a toner cartridge.
// Untrained -     Alerts that can be fixed without prior
// training either because the action to correct the alert
// is obvious or the printer can help the untrained person
// fix the problem.  A typical example of such an alert is
// reloading paper trays or emptying output bins on a low
// end printer.
// Management -    Alerts that have to do with overall operation
// of and configuration of the printer.  Examples of such
// management events are configuration change of subunits."
// 预警处理级别
BEGIN_ENUM_STRUCT(PrtAlertTrainingLevelTC)
{
	Other = 1,
	UnKnown = 2,
	UnTrained = 3,
	Trained = 4,
	FieldService = 5,
	Management = 6,
	NoInterventionRequired = 7  // Not in RFC 1759
}
END_ENUM_STRUCT(PrtAlertTrainingLevelTC)

// The type of sub-unit within the printer model that this
// alert is related. Input, output, and markers are
// examples of printer model groups, i.e., examples of
// types of sub-units. Wherever possible, these
// enumerations match the sub-identifier that identifies
// the relevant table in the printmib.
// 预警组类别
BEGIN_ENUM_STRUCT(PrtAlertGroupTC)
{
	Other = 1,
	// (2) is reserved for conformance information
	// Values for Host Resources MIB
	HostResourcesMIBStorageTable = 3,
	HostResourcesMIBDeviceTable = 4,
	// Values for Printer MIB
	GeneralPrinter = 5,
	Cover = 6,			//盖板
	Localization = 7,
	Input = 8,
	Output = 9,
	Marker = 10,
	MarkerSupplies = 11,
	MarkerColorant = 12,
	MediaPath = 13,
	Channel = 14,
	Interpreter = 15,
	ConsoleDisplayBuffer = 16,
	ConsoleLights = 17,
	Alert = 18,                   // Not in RFC 1759
	// Values (5) to (29) reserved for Printer MIB
	// Values for Finisher MIB
	FinDevice = 30,               // Not in RFC 1759
	FinSupply = 31,               // Not in RFC 1759
	FinSupplyMediaInput = 32,     // Not in RFC 1759
	FinAttribute = 33             // Not in RFC 1759
	// Values (30) to (39) reserved for Finisher MIB
}
END_ENUM_STRUCT(PrtAlertGroupTC)

// See associated textual convention PrtAlertCodeTC
// 预警码
BEGIN_ENUM_STRUCT(PrtAlertCodeTC)
{
	Other = 1,
	// an event that is not represented
	// by one of the alert codes
	// specified below.
	UnKnown = 2,
	// The following generic codes are common to
	// multiple groups.  The NMS may examine the
	// prtAlertGroup object to determine what group
	// to query for further information.
	CoverOpen = 3,
	CoverClosed = 4,
	InterlockOpen = 5,
	InterlockClosed = 6,
	ConfigurationChange = 7,
	Jam = 8,
	SubunitMissing = 9,           // Not in RFC 1759
	// The subunit tray, bin, etc.
	// has been removed.
	SubunitLifeAlmostOver = 10,   // Not in RFC 1759
	SubunitLifeOver = 11,         // Not in RFC 1759
	SubunitAlmostEmpty = 12,      // Not in RFC 1759
	SubunitEmpty = 13,            // Not in RFC 1759
	SubunitAlmostFull = 14,       // Not in RFC 1759
	SubunitFull = 15,             // Not in RFC 1759
	SubunitNearLimit = 16,        // Not in RFC 1759
	SubunitAtLimit = 17,          // Not in RFC 1759
	SubunitOpened = 18,           // Not in RFC 1759
	SubunitClosed = 19,           // Not in RFC 1759
	SubunitTurnedOn = 20,         // Not in RFC 1759
	SubunitTurnedOff = 21,        // Not in RFC 1759
	SubunitOffline = 22,          // Not in RFC 1759
	SubunitPowerSaver = 23,       // Not in RFC 1759
	SubunitWarmingUp = 24,        // Not in RFC 1759
	SubunitAdded = 25,            // Not in RFC 1759
	SubunitRemoved = 26,          // Not in RFC 1759
	SubunitResourceAdded = 27,    // Not in RFC 1759
	SubunitResourceRemoved = 28,  // Not in RFC 1759
	SubunitRecoverableFailure = 29,
	// Not in RFC 1759
	SubunitUnrecoverableFailure = 30,
	// Not in RFC 1759
	SubunitRecoverableStorageError = 31,
	// Not in RFC 1759
	SubunitUnrecoverableStorageError = 32,
	// Not in RFC 1759
	SubunitMotorFailure = 33,     // Not in RFC 1759
	SubunitMemoryExhausted = 34,  // Not in RFC 1759
	SubunitUnderTemperature = 35, // Not in RFC 1759
	SubunitOverTemperature = 36,  // Not in RFC 1759
	SubunitTimingFailure = 37,    // Not in RFC 1759
	SubunitThermistorFailure = 38, // Not in RFC 1759

	// General Printer group
	DoorOpen = 501,    // DEPRECATED
	// Use coverOpened(3)
	DoorClosed = 502,  // DEPRECATED
	// Use coverClosed(4)
	PowerUp = 503,
	PowerDown = 504,
	PrinterNMSReset = 505,        // Not in RFC 1759
	// The printer has been reset by some
	// network management station(NMS)
	// writing into 'prtGeneralReset'.
	PrinterManualReset = 506,     // Not in RFC 1759
	// The printer has been reset manually.
	PrinterReadyToPrint = 507,    // Not in RFC 1759
	// The printer is ready to print. (i.e.,
	// not warming up, not in power save
	// state, not adjusting print quality,
	// etc.).

	// Input Group
	InputMediaTrayMissing = 801,
	InputMediaSizeChange = 802,
	InputMediaWeightChange = 803,
	InputMediaTypeChange = 804,
	InputMediaColorChange = 805,
	InputMediaFormPartsChange = 806,
	InputMediaSupplyLow = 807,
	InputMediaSupplyEmpty = 808,
	InputMediaChangeRequest = 809, // Not in RFC 1759
	// An interpreter has detected that a
	// different medium is need in this input
	// tray subunit.  The prtAlertDescription may
	// be used to convey a human readable
	// description of the medium required to
	// satisfy the request.
	InputManualInputRequest = 810, // Not in RFC 1759
	// An interpreter has detected that manual
	// input is required in this subunit.  The
	// prtAlertDescription may be used to convey
	// a human readable description of the medium
	// required to satisfy the request.
	InputTrayPositionFailure = 811, // Not in RFC 1759
	// The input tray failed to position correctly.
	InputTrayElevationFailure = 812,
	// Not in RFC 1759
	InputCannotFeedSizeSelected = 813,
	// Not in RFC 1759
	// Output Group
	OutputMediaTrayMissing = 901,
	OutputMediaTrayAlmostFull = 902,
	OutputMediaTrayFull = 903,
	OutputMailboxSelectFailure = 904,
	// Not in RFC 1759
	// Marker group
	MarkerFuserUnderTemperature = 1001,
	MarkerFuserOverTemperature = 1002,
	MarkerFuserTimingFailure = 1003,
	// Not in RFC 1759
	MarkerFuserThermistorFailure = 1004,
	// Not in RFC 1759
	MarkerAdjustingPrintQuality = 1005,
	// Not in RFC 1759
	// Marker Supplies group
	MarkerTonerEmpty = 1101,
	MarkerInkEmpty = 1102,
	MarkerPrintRibbonEmpty = 1103,
	MarkerTonerAlmostEmpty = 1104,
	MarkerInkAlmostEmpty = 1105,
	MarkerPrintRibbonAlmostEmpty = 1106,
	MarkerWasteTonerReceptacleAlmostFull = 1107,
	MarkerWasteInkReceptacleAlmostFull = 1108,
	MarkerWasteTonerReceptacleFull = 1109,
	MarkerWasteInkReceptacleFull = 1110,
	MarkerOpcLifeAlmostOver = 1111,
	MarkerOpcLifeOver = 1112,
	MarkerDeveloperAlmostEmpty = 1113,
	MarkerDeveloperEmpty = 1114,
	MarkerTonerCartridgeMissing = 1115,
	// Not in RFC 1759
	// Media Path Device Group
	MediaPathMediaTrayMissing = 1301,
	MediaPathMediaTrayAlmostFull = 1302,
	MediaPathMediaTrayFull = 1303,
	MediaPathCannotDuplexMediaSelected = 1304,
	// Not in RFC 1759
	// Interpreter Group
	InterpreterMemoryIncrease = 1501,
	InterpreterMemoryDecrease = 1502,
	InterpreterCartridgeAdded = 1503,
	InterpreterCartridgeDeleted = 1504,
	InterpreterResourceAdded = 1505,
	InterpreterResourceDeleted = 1506,
	InterpreterResourceUnavailable = 1507,
	InterpreterComplexPageEncountered = 1509,
	// Not in RFC 1759
	// The interpreter has encountered a page
	// that is too complex for the resources that
	// are available.
	// Alert Group
	AlertRemovalOfBinaryChangeEntry = 1801
	// Not in RFC 1759
	// A binary change event entry has been
	// removed from the alert table.  This unary
	// change alert table entry is added to the
	// end of the alert table.
}
END_ENUM_STRUCT(PrtAlertCodeTC)

//printmib.prtAlert.prtAlertTable.prtAlertEntry
#define	PrtAlertEntryOID					TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFI")									//"printmib.prtAlert.prtAlertTable.prtAlertEntry"
#define	PrtAlertIndexOID					TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGel8NNh0=")				//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertIndex"
#define	PrtAlertSeverityLevelOID			TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGYFQfNhcKRio4BARWXQ==")	//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertSeverityLevel"
#define	PrtAlertTrainingLevelOID			TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGZ0MIOgsKXDQ4BARWXQ==")	//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTrainingLevel"
#define	PrtAlertGroupOID					TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGdEMGJhU=")				//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroup"
#define	PrtAlertGroupIndexOID				TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGdEMGJhUqXDcRGQ==")		//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroupIndex"
#define	PrtAlertLocationOID					TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGf14KMhEKXT0=")			//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertLocation"
#define	PrtAlertCodeOID						TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGcF4NNg==")				//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode"
#define	PrtAlertDescriptionOID				TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGd1QaMBcKQicdDhw=")		//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertDescription"
#define	PrtAlertTimeOID						TEXT("QRs6CxdfOhZPAkFFKD8AEUZ9BBMGcl0MIRE3UzEYBFxDQx0SCQZAJzEPBkFIRyMXF3M/ERMGZ1gENg==")				//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTime"
typedef struct tag_PrtAlertEntry
{
	Integer32 prtAlertIndex;								//(1)
	ENUM_STRUCT_TYPE(PrtAlertSeverityLevelTC) prtAlertSeverityLevel;	//(2)
	ENUM_STRUCT_TYPE(PrtAlertTrainingLevelTC) prtAlertTrainingLevel;	//(3)
	ENUM_STRUCT_TYPE(PrtAlertGroupTC) prtAlertGroup;		//(4)
	Integer32 prtAlertGroupIndex;							//(5)
	Integer32 prtAlertLocation;								//(6)
	ENUM_STRUCT_TYPE(PrtAlertCodeTC) prtAlertCode;			//(7)
	OCTET_STRING prtAlertDescription[OCTET_STRING_SIZE_256];//(8)
	TimeTicks prtAlertTime;									//(9)
}PrtAlertEntry, *PPrtAlertEntry;
typedef map<int, PPrtAlertEntry> PrtAlertEntryMap;
typedef list<PPrtAlertEntry> PrtAlertEntryList;

#endif //_PRINTER_MIB_V2_DEF_650FBD0E_EA63_4BCA_8694_187A22D44C12_H_