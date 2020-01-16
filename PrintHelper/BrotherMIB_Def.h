#ifndef _BROTHER_MIB_DEF_02CEC903_48AC_4A56_B824_8AE1B254AA04_H_
#define _BROTHER_MIB_DEF_02CEC903_48AC_4A56_B824_8AE1B254AA04_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：[iSecPrint\Doc\Snmp\兄弟]
	1.Brother HL-L8250CDN series.log
	2.Brother MFC-8540DN.log
	3.snmp记录.txt
*/
/************************************************************************/

BEGIN_ENUM_STRUCT(BrotherCounterType)
{
	PrintTotalPages = 1,
	FaxTotalPages = 2,
	CopyTotalPages = 3,
	Type4 = 4,
	Type7 = 7,
	Type10 = 10,
	PrintDuplxTotalPages = 11,
	CopyDuplxTotalPages = 12,
	FaxDuplxTotalPages = 13,
	Type12 = 14,
	Type24 = 24,
	Type25 = 25,
	Type26 = 26,
	Type45 = 45,
	Type80 = 80,
	Type81 = 81,
	Type90 = 90,
	OtherPrintTotalPages = 91,
	OtherPrintDuplxTotalPages = 94,
	Type99 = 99,
	Type100 = 100,
	Type101 = 101,
	Type102 = 102,
	Type702 = 702,
	Type703 = 703,
	BlackWhiteTotalPages = 704,
	ColorTotalPages = 705,
	Type706 = 706,
	Type707 = 707,
	Type708 = 708,
	Type709 = 709,
	Type710 = 710,
	Type711 = 711,
	Type712 = 712,
	Type713 = 713,
	TotalPages = 898,
	Type899 = 899,
	Type900 = 900,
	Type901 = 901,
	Type902 = 902,
	Type903 = 903,
	Type908 = 908,
	Type909 = 909,
	Type913 = 913,
	Type920 = 920,
	Type921 = 921,
	Type922 = 922,
	Type923 = 923,
	Type924 = 924,
	Type925 = 925,
	Type926 = 926,
	Type927 = 927,
	Type932 = 932,
	Type933 = 933,
	Type939 = 939,
	Type940 = 940,
	Type941 = 941,
	Type942 = 942,
	Type998 = 998,
	Type999 = 999
}
END_ENUM_STRUCT(BrotherCounterType)

#define BrotherCounterInfo1OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdAEdi")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.1.1"
typedef struct tag_BrotherCounterInfo1Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo1Entry, *PBrotherCounterInfo1Entry;
typedef map<int, PBrotherCounterInfo1Entry> BrotherCounterInfo1EntryMap;
typedef list<PBrotherCounterInfo1Entry> BrotherCounterInfo1EntryList;

//单面出纸统计
#define BrotherCounterInfo2OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdA0di")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.2.1"
typedef struct tag_BrotherCounterInfo2Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo2Entry, *PBrotherCounterInfo2Entry;
typedef map<int, PBrotherCounterInfo2Entry> BrotherCounterInfo2EntryMap;
typedef list<PBrotherCounterInfo2Entry> BrotherCounterInfo2EntryList;

//总出纸统计，根据色彩分类
#define BrotherCounterInfo3OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdAkdi")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.3.1"
typedef struct tag_BrotherCounterInfo3Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo3Entry, *PBrotherCounterInfo3Entry;
typedef map<int, PBrotherCounterInfo3Entry> BrotherCounterInfo3EntryMap;
typedef list<PBrotherCounterInfo3Entry> BrotherCounterInfo3EntryList;

//双面出纸统计
#define BrotherCounterInfo4OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdBUdi")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.4.1"
typedef struct tag_BrotherCounterInfo4Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo4Entry, *PBrotherCounterInfo4Entry;
typedef map<int, PBrotherCounterInfo4Entry> BrotherCounterInfo4EntryMap;
typedef list<PBrotherCounterInfo4Entry> BrotherCounterInfo4EntryList;

//总出纸统计,根据打印、复印、扫描、传真的功能和单双面分类，
//好像当只有打印功能的情况下没有有效的数据。
#define BrotherCounterInfo21OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdA1h9VA==") //".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.21.1"
typedef struct tag_BrotherCounterInfo21Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo21Entry, *PBrotherCounterInfo21Entry;
typedef map<int, PBrotherCounterInfo21Entry> BrotherCounterInfo21EntryMap;
typedef list<PBrotherCounterInfo21Entry> BrotherCounterInfo21EntryList;

//未知
#define BrotherCounterInfo22OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdA1t9VA==")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.22.1"
typedef struct tag_BrotherCounterInfo22Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo22Entry, *PBrotherCounterInfo22Entry;
typedef map<int, PBrotherCounterInfo22Entry> BrotherCounterInfo22EntryMap;
typedef list<PBrotherCounterInfo22Entry> BrotherCounterInfo22EntryList;

//未知
#define BrotherCounterInfo31OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdAlh9VA==")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.31.1"
typedef struct tag_BrotherCounterInfo31Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo31Entry, *PBrotherCounterInfo31Entry;
typedef map<int, PBrotherCounterInfo31Entry> BrotherCounterInfo31EntryMap;
typedef list<PBrotherCounterInfo31Entry> BrotherCounterInfo31EntryList;

//未知
#define BrotherCounterInfo41OID TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQAcCXH1XTQF9TU9GHQNHYktWHGZaVEAdBVh9VA==")	//".iso.org.dod.internet.private.enterprises.2435.2.3.9.4.2.1.5.5.52.41.1"
typedef struct tag_BrotherCounterInfo41Entry	
{
	Integer32 Index;
	ENUM_STRUCT_TYPE(BrotherCounterType) Type;	
	Integer32 PageNumbers;			
}BrotherCounterInfo41Entry, *PBrotherCounterInfo41Entry;
typedef map<int, PBrotherCounterInfo41Entry> BrotherCounterInfo41EntryMap;
typedef list<PBrotherCounterInfo41Entry> BrotherCounterInfo41EntryList;

#define BrotherModelOID							TEXT("H1h9Vk0EfUVPQB0AR2FQTQF9Rk9DHQJHYg==")//获取兄弟型号 
#define BrotherA4HBpPagesOID_LenovoLJ3650DN		TEXT("H1h9Vk0EfUVPRh0AR2FRUAd9Rk9BHQhHZ0tRHGJaVFwGH1xhS1IcYlpSXAI=")	//".1.3.6.1.4.1.2435.2.3.9.4.2.1.5.5.52.1.1.3.1"
#define BrotherTotalHBPagesOID_LenovoLJ3650DN	TEXT("H1h9Vk0EfUVPRh0AR2FRUAd9Rk9BHQhHZ0tRHGJaVFwGH1xhS1ADfUVPQR0A")	//".1.3.6.1.4.1.2435.2.3.9.4.2.1.5.5.52.31.1.3.1"
#define BrotherMarkerSuppliesMaxCapacityOID_LenovoLJ3650DN TEXT("H1h9Vk0EfUVPQB0AR2dWTQNiWlBcAh9RfVRNAQ==");//.1.3.6.1.2.1.43.11.1.1.8.1.3
#define BrotherMarkerSuppliesLevelOID_LenovoLJ3650DN TEXT("H1h9Vk0EfUVPQB0AR2dWTQNiWlBcAh9QfVRNAQ==");//.1.3.6.1.2.1.43.11.1.1.9.1.3

#endif	//_BROTHER_MIB_DEF_02CEC903_48AC_4A56_B824_8AE1B254AA04_H_
