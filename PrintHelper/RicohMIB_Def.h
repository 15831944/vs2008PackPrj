#ifndef _RICOH_MIB_DEF_0E931DCE_E930_4EB4_B626_C5F7B6BA82AE_H_
#define _RICOH_MIB_DEF_0E931DCE_E930_4EB4_B626_C5F7B6BA82AE_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：[iSecPrint\Doc\Snmp\理光]
	1.理光回复_请教snmp.tif
	2.理光回复_请教snmp.eml
	3.Private MIB information ver.4.09.zip
		PrivateMIBSpecificationV4_080-1.pdf
		PrivateMIBSpecificationV4_080-2.pdf
		PrivateMIBSpecificationV4_080-3.pdf
		PrivateMIBSpecificationV4_080-4.pdf
		PrivateMIBSpecificationV4_080-5.pdf
*/
/************************************************************************/

BEGIN_ENUM_STRUCT(RicohTonerColorType)
{
	Other = 1,
	UnKnown = 2,
	BlackTonerMono = 3,
	RedTonerMono = 4,
	CyanToner4Color = 10,
	MagentaToner4Color = 11,
	YellowToner4Color = 12,
	BlackToner4Color = 13	
}
END_ENUM_STRUCT(RicohTonerColorType)


BEGIN_ENUM_STRUCT(ricohEngCounterType)
{
	Machine_Total = 10,				//Counter: Machine Total
	Copy_Total = 200,				//Counter:Copy:Total
	Copy_Black_White = 201,			//Counter:Copy:Black & White
	Copy_SingleOrTwo_Color = 202,	//Counter:Copy:Single/Two-color
	Copy_Full_Color = 203,			//Counter:Copy:Full Color
	Fax_Total = 300,				//Counter:Fax Total
	Fax_Black_White = 301,			//Counter:FAX:Black & White
	Fax_SingleOrTwo_Color = 302,	//Counter:Fax:Single/Two-color
	Fax_Full_Color = 303,			//Counter:Fax:Full Color
	Print_Total = 400,				//Counter:Print:Total
	Print_Black_White = 401,		//Counter:Print:Black & White
	Print_SingleOrTwo_Color = 402,	//Counter:Print:Single/Two-col.
	Print_Full_Color = 403,			//Counter:Print:Full Color
	Machine_Total2 = 800,			//Counter: Machine Total
	Transmission_Total = 870,		//Counter:Transmission:Total
	Transmission_Black_White = 871,	//Counter:Transmission:B&W
	Transmission_FAX = 872,			//Counter:Transmission:FAX
	Transmission_Color_Scan = 873,	//Counter:Transmission:Color Scan
	Transmission_Black_White_Scan = 874			//Counter:Transmission:B&W Scan
}
END_ENUM_STRUCT(ricohEngCounterType)


BEGIN_ENUM_STRUCT(ricohEngTonerType)
{
	Black_toner1 = 3, 
	Black_toner2 = 13, 
	Cyan_toner = 10, 
	Magenta_toner = 11, 
	Yellow_toner = 12
}
END_ENUM_STRUCT(ricohEngTonerType)

BEGIN_ENUM_STRUCT(ricohEngCounterUnit)
{
	sheets = 8,
	times = 50,
	percent = 51,
	minutes = 64
}
END_ENUM_STRUCT(ricohEngCounterUnit)

#define RicohEngCounterEntryOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAghHZktS")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.19.5.1"
typedef struct tag_ricohEngCounterEntry
{
	Integer32 ricohEngCounterIndex;								//(1)
	ENUM_STRUCT_TYPE(ricohEngCounterType) ricohEngCounterType;	//(2)
	Integer32 ricohEngCounterIncrement;							//(3)
	ENUM_STRUCT_TYPE(ricohEngCounterUnit) ricohEngCounterUnit;	//(4)
	OCTET_STRING ricohEngCounterName[OCTET_STRING_SIZE_256];	//(5)
	OCTET_STRING ricohEngCounterDescr[OCTET_STRING_SIZE_256];	//(6)	//可能是utf-8编码
	Integer32 ricohEngCounterRangeMax;							//(7)
	Integer32 ricohEngCounterRangeMin;							//(8)
	Counter32 ricohEngCounterValue;								//(9)
}ricohEngCounterEntry , *PricohEngCounterEntry;
typedef list<PricohEngCounterEntry> RicohEngCounterEntryList;
typedef map<int, PricohEngCounterEntry> RicohEngCounterEntryMap;

#define RicohEngPrtAlertEntryOID	TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQBHZ0tRHGI=")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.21.4.2.1"
typedef struct tag_ricohEngPrtAlertEntry
{
	Integer32 ricohEngPrtAlertIndex;		//(1)	3.2.1.2.21.4.2.1.1.1.* ricohEngPrtAlertIndex	//Not-accessible
	Integer ricohEngPrtAlertSeverityLevel;	//(2)	3.2.1.2.21.4.2.1.2.1.* ricohEngPrtAlertSeverityLevel
	Integer ricohEngPrtAlertTrainingLevel;	//(3)	3.2.1.2.21.4.2.1.3.1.* ricohEngPrtAlertTrainingLevel
	Integer ricohEngPrtAlertGroup;			//(4)	3.2.1.2.21.4.2.1.4.1.* ricohEngPrtAlertGroup
	Integer32 ricohEngPrtAlertGroupIndex;	//(5)	3.2.1.2.21.4.2.1.5.1.* ricohEngPrtAlertGroupIndex
	Integer32 ricohEngPrtAlertLocation;		//(6)	3.2.1.2.21.4.2.1.6.1.* ricohEngPrtAlertLocation
	Integer ricohEngPrtAlertCode;			//(7)	3.2.1.2.21.4.2.1.7.1.* ricohEngPrtAlertCode
	OCTET_STRING ricohEngPrtAlertDescription[OCTET_STRING_SIZE_256];	//(8)	3.2.1.2.21.4.2.1.8.1.* ricohEngPrtAlertDescription
	TimeTicks ricohEngPrtAlertTime;			//(9)	3.2.1.2.21.4.2.1.9.1.* ricohEngPrtAlertTime
}ricohEngPrtAlertEntry, *PricohEngPrtAlertEntry;
typedef list<PricohEngPrtAlertEntry> RicohEngPrtAlertEntryList;
typedef map<int, PricohEngPrtAlertEntry> RicohEngPrtAlertEntryMap;

#define RicohEngFAXAlertEntryOID	TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQNHZ0tRHGI=")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.22.4.2.1"
typedef struct tag_ricohEngFAXAlertEntry
{
	Integer32 ricohEngFAXAlertIndex;		//(1)	3.2.1.2.22.4.2.1.1.4.* ricohEngFAXAlertIndex	//Not-accessible
	Integer ricohEngFAXAlertSeverityLevel;	//(2)	3.2.1.2.22.4.2.1.2.4.* ricohEngFAXAlertSeverityLevel
	Integer ricohEngFAXAlertTrainingLevel;	//(3)	3.2.1.2.22.4.2.1.3.4.* ricohEngFAXAlertTrainingLevel
	Integer ricohEngFAXAlertGroup;			//(4)	3.2.1.2.22.4.2.1.4.4.* ricohEngFAXAlertGroup
	Integer32 ricohEngFAXAlertGroupIndex;	//(5)	3.2.1.2.22.4.2.1.5.4.* ricohEngFAXAlertGroupIndex
	Integer32 ricohEngFAXAlertLocation;		//(6)	3.2.1.2.22.4.2.1.6.4.* ricohEngFAXAlertLocation
	Integer ricohEngFAXAlertCode;			//(7)	3.2.1.2.22.4.2.1.7.4.* ricohEngFAXAlertCode
	OCTET_STRING ricohEngFAXAlertDescription[OCTET_STRING_SIZE_256];	//(8)	3.2.1.2.22.4.2.1.8.4.* ricohEngFAXAlertDescription
	TimeTicks ricohEngFAXAlertTime;			//(9)	3.2.1.2.22.4.2.1.9.4.* ricohEngFAXAlertTime
}ricohEngFAXAlertEntry, *PricohEngFAXAlertEntry;
typedef list<PricohEngFAXAlertEntry> RicohEngFAXAlertEntryList;
typedef map<int, PricohEngFAXAlertEntry> RicohEngFAXAlertEntryMap;

#define RicohEngCpyAlertEntryOID	TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQJHZ0tRHGI=")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.23.4.2.1"
typedef struct tag_ricohEngCpyAlertEntry
{
	Integer32 ricohEngCpyAlertIndex;		//(1)	3.2.1.2.23.4.2.1.1.3.* ricohEngCpyAlertIndex	//Not-accessible
	Integer ricohEngCpyAlertSeverityLevel;	//(2)	3.2.1.2.23.4.2.1.2.3.* ricohEngCpyAlertSeverityLevel
	Integer ricohEngCpyAlertTrainingLevel;	//(3)	3.2.1.2.23.4.2.1.3.3.* ricohEngCpyAlertTrainingLevel
	Integer ricohEngCpyAlertGroup;			//(4)	3.2.1.2.23.4.2.1.4.3.* ricohEngCpyAlertGroup
	Integer32 ricohEngCpyAlertGroupIndex;	//(5)	3.2.1.2.23.4.2.1.5.3.* ricohEngCpyAlertGroupIndex
	Integer32 ricohEngCpyAlertLocation;		//(6)	3.2.1.2.23.4.2.1.6.3.* ricohEngCpyAlertLocation
	Integer ricohEngCpyAlertCode;			//(7)	3.2.1.2.23.4.2.1.7.3.* ricohEngCpyAlertCode
	OCTET_STRING ricohEngCpyAlertDescription[OCTET_STRING_SIZE_256];	//(8)	3.2.1.2.23.4.2.1.8.3.* ricohEngCpyAlertDescription
	TimeTicks ricohEngCpyAlertTime;			//(9)	3.2.1.2.23.4.2.1.9.3.* ricohEngCpyAlertTime
}ricohEngCpyAlertEntry, *PricohEngCpyAlertEntry;
typedef list<PricohEngCpyAlertEntry> RicohEngCpyAlertEntryList;
typedef map<int, PricohEngCpyAlertEntry> RicohEngCpyAlertEntryMap;

#define RicohEngScnAlertEntryOID	TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAgdHYFVNAH1F")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.16.30.2.1"
typedef struct tag_ricohEngScnAlertEntry
{
	Integer32 ricohEngScnAlertIndex;		//(1)	3.2.1.2.16.30.2.1.1.* ricohEngScnAlertIndex	//Not-accessible
	Integer ricohEngScnAlertSeverityLevel;	//(2)	3.2.1.2.16.30.2.1.2.* ricohEngScnAlertSeverityLevel
	Integer ricohEngScnAlertTrainingLevel;	//(3)	3.2.1.2.16.30.2.1.3.* ricohEngScnAlertTrainingLevel
	Integer ricohEngScnAlertGroup;			//(4)	3.2.1.2.16.30.2.1.4.* ricohEngScnAlertGroup
	Integer32 ricohEngScnAlertGroupIndex;	//(5)	3.2.1.2.16.30.2.1.5.* ricohEngScnAlertGroupIndex
	Integer32 ricohEngScnAlertLocation;		//(6)	3.2.1.2.16.30.2.1.6.* ricohEngScnAlertLocation
	Integer ricohEngScnAlertCode;			//(7)	3.2.1.2.16.30.2.1.7.* ricohEngScnAlertCode
	OCTET_STRING ricohEngScnAlertDescription[OCTET_STRING_SIZE_256];	//(8)	3.2.1.2.16.30.2.1.8.* ricohEngScnAlertDescription
	TimeTicks ricohEngScnAlertTime;			//(9)	3.2.1.2.16.30.2.1.9.* ricohEngScnAlertTime
}ricohEngScnAlertEntry, *PricohEngScnAlertEntry;
typedef list<PricohEngScnAlertEntry> RicohEngScnAlertEntryList;
typedef map<int, PricohEngScnAlertEntry> RicohEngScnAlertEntryMap;


#define RicohTonerColorOID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQVHYktSHGE=")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.24.1.1.2"		//粉盒颜色描述，如【Black Toner】【Cyan Toner】【Magenta Toner】【Yellow Toner】
#define RicohTonerDescOID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQVHYktSHGA=")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.24.1.1.3"		//粉盒颜色描述，如【黑色碳粉】UTF-8编码
#define RicohTonerColorTypeOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQVHYktSHGc=")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.24.1.1.4"		//粉盒颜色类型，参考RicohTonerColorType
#define RicohTonerLevelPercentOID	TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlNcAQVHYktSHGY=")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.2.24.1.1.5"		//粉盒剩余百分比，如100、68、0等等

#define RicohJobInfo2OID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31G")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.2"
#define RicohJobInfo3OID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31H")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.3"
#define RicohJobInfoDocNameOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31A")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.4"		//文件名(MultiByte)
#define RicohJobInfo5OID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31B")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.5"		//用户识别号码
#define RicohJobInfoPageOID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31C")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.6"		//Value=proc=1,prt=1				//proc=处理的页数，prt=打印的页数
#define RicohJobInfoIDOID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31D")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.7"		//Value=pid=1,submit=(NONE),recv=1	//pid=打印作业ID
#define RicohJobInfoCreateTimeOID	TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31M")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.8"		//Value=submit=1603120139Z			//创建时间2016-03-12 01:39:00
#define RicohJobInfoUserNameOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31N")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.9"		//Value=username=,hostloginname=Administrator	//用户名称
#define RicohJobInfoStateStrOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31FUQ==")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.10"	//作业状态(utf8)
#define RicohJobInfoStateOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31FUA==")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.11"	//作业状态：0-无作业，1-打印完成，6-处理中，7-错误，8-已取消
#define RicohJobInfo12OID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31FUw==")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.12"
#define RicohJobInfo13OID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVRNA31FUg==")	//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.1.1.13"

#define RicohJobCurrentCountOID		TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVdNAg==")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.2.0"		//当前打印机作业记录总数
#define RicohJobMaxCountOID			TEXT("HwAgCk1dIRNPFlxVRzoLF1chGgQGHUEbOhMCRjZaBBxHVBsjFwpBNgdPQQUGR2BLURxiWlJcAR9YfVZNAg==")		//".iso.org.dod.internet.private.enterprises.367.3.2.1.3.2.1.3.0"		//当前打印机可记录的作业记录总数



#endif	//_RICOH_MIB_DEF_0E931DCE_E930_4EB4_B626_C5F7B6BA82AE_H_
