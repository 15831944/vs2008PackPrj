#ifndef _SAMSUNG_MIB_DEF_E1DB442C_274B_465E_9544_65D83172227D_H_
#define _SAMSUNG_MIB_DEF_E1DB442C_274B_465E_9544_65D83172227D_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：[iSecPrint\Doc\Snmp\三星]
	1.SAMSUNG-CLONING-MIB
	2.SAMSUNG-COMMON-MIB
	3.SAMSUNG-DIAGNOSTICS-MIB
	4.SAMSUNG-GENERAL-TC
	5.SAMSUNG-HOST-RESOURCES-EXT-MIB
	6.SAMSUNG-HOST-RESOURCES-EXT-TC
	7.SAMSUNG-PRINTER-EXT-MIB
	8.SAMSUNG-PRINTER-EXT-TC
*/
/************************************************************************/

BEGIN_ENUM_STRUCT(ScmHrDevCountJobTypeTC)
{
	print = 1,
	copy = 2,
	faxIn = 3,
	faxOut = 4,
	scan = 5,
	report = 6,
	digitalSend = 11,
	digitalRecieve = 12,
	localStorage = 21
}
END_ENUM_STRUCT(ScmHrDevCountJobTypeTC)

BEGIN_ENUM_STRUCT(ScmHrDevCountMediaSizeTC)
{
	small_size = 1,
	large_size = 2,
	letter = 3,
	legal = 4,
	a4 = 5,
	executive = 6,
	jisB5 = 7,
	isoB5 = 8,
	com10 = 9,
	monarch = 10,
	dl = 11,
	c5 = 12,
	postA6 = 13,
	c6 = 14,
	folio = 15,
	a5 = 16,
	statement = 17,
	a6 = 18,
	ledger = 19,
	a3 = 20,
	jisB4 = 21,
	jpost = 22,
	jpostd = 23,
	custom = 24,
	letterP = 25,
	a4P = 26,
	jisB5P = 27,
	a5P = 28,
	executiveP = 29,
	statementP = 30,
	a3Over = 31,
	b5Envelope = 32
}
END_ENUM_STRUCT(ScmHrDevCountMediaSizeTC)

BEGIN_ENUM_STRUCT(ScmHrDevCountUnitTC)
{
	other = 1,
	unknown = 2,
	tenThousandthsOfInches = 3,
	micrometers = 4,
	impressions = 7,
	sheets = 8,
	hours = 11,
	thousandthsOfOunces = 12,
	tenthsOfGrams = 13,
	hundrethsOfFluidOunces = 14,
	tenthsOfMilliliters = 15,
	feet = 16,
	meters = 17,
	items = 18,
	percent = 19
}
END_ENUM_STRUCT(ScmHrDevCountUnitTC)

BEGIN_ENUM_STRUCT(ScmHrDevCountDuplexTC)
{
	simplex = 1,
	duplex = 2,
	duplexSingle = 3
}
END_ENUM_STRUCT(ScmHrDevCountDuplexTC)

BEGIN_ENUM_STRUCT(ScmHrDevCountColorTC)
{
	fullColor = 1,
	singleColor = 2,
	monoColor = 3
}
END_ENUM_STRUCT(ScmHrDevCountColorTC)

#define ScmHrDevCountEntryOID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAAHR2JUTQd9RVBcBgJHYlRNAH1F")	//".iso.org.dod.internet.private.enterprises.236.11.5.11.53.11.2.1"
typedef struct tag_ScmHrDevCountEntry
{
	Integer32 scmHrDevCountIndex;
	ENUM_STRUCT_TYPE(ScmHrDevCountJobTypeTC) scmHrDevCountJobType;
	ENUM_STRUCT_TYPE(ScmHrDevCountMediaSizeTC) scmHrDevCountMediaSize;
	ENUM_STRUCT_TYPE(ScmHrDevCountUnitTC) scmHrDevCountUnit;
	ENUM_STRUCT_TYPE(ScmHrDevCountDuplexTC) scmHrDevCountDuplex;
	ENUM_STRUCT_TYPE(ScmHrDevCountColorTC) scmHrDevCountColor;
	Counter32 scmHrDevCountValue;
}ScmHrDevCountEntry , *PScmHrDevCountEntry;
typedef map<int, PScmHrDevCountEntry> ScmHrDevCountEntryMap;
typedef list<PScmHrDevCountEntry> ScmHrDevCountEntryList;


#endif	//_SAMSUNG_MIB_DEF_E1DB442C_274B_465E_9544_65D83172227D_H_
