#ifndef _KYOCERA_MIB_DEF_AA638EB3_5BEE_4D0E_B435_15BA8FF4DF31_H_
#define _KYOCERA_MIB_DEF_AA638EB3_5BEE_4D0E_B435_15BA8FF4DF31_H_

#include "Snmp_Def.h"

//京瓷抄表计数节点
#define KyoceraA4PrintTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVRNA31FT0M=")		//".1.3.6.1.4.1.1347.42.3.1.1.1.1.1"
#define KyoceraA4PrintHBPageOID					TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVdNA31FT0MdAA==")	//".1.3.6.1.4.1.1347.42.3.1.2.1.1.1.1"
#define KyoceraA4PrintSingleColorPageOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVdNA31FT0MdAw==")	//".1.3.6.1.4.1.1347.42.3.1.2.1.1.1.2"
#define KyoceraA4PrintFullColorPageOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVdNA31FT0MdAg==")	//".1.3.6.1.4.1.1347.42.3.1.2.1.1.1.3"
#define KyoceraA4CopyTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVRNA31FT0A=")		//".1.3.6.1.4.1.1347.42.3.1.1.1.1.2"
#define KyoceraA4CopyHBPageOID					TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVdNA31FT0AdAA==")	//".1.3.6.1.4.1.1347.42.3.1.2.1.1.2.1"
#define KyoceraA4CopySingleColorPageOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVdNA31FT0AdAw==")	//".1.3.6.1.4.1.1347.42.3.1.2.1.1.2.2"
#define KyoceraA4CopyFullColorPageOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVdNA31FT0AdAg==")	//".1.3.6.1.4.1.1347.42.3.1.2.1.1.2.3"
#define KyoceraScanTotalPageOID					TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVFNA31FT0M=")		//".1.3.6.1.4.1.1347.42.3.1.4.1.1.1"
#define KyoceraScanTotalPage2OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFdcAgFHYktSHGZaUg==")		//".1.3.6.1.4.1.1347.46.10.1.1.5.3"
#define KyoceraTotalPageInfo6OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9YfVRNA31CT0M=")		//".1.3.6.1.4.1.1347.42.2.1.1.1.6.1"
#define KyoceraTotalPageInfo7OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9YfVRNA31DT0M=")		//".1.3.6.1.4.1.1347.42.2.1.1.1.7.1"
#define KyoceraTotalPageInfo8OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9YfVRNA31MT0M=")		//".1.3.6.1.4.1.1347.42.2.1.1.1.8.1"
#define KyoceraTotalPageInfo9OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9YfVRNA31NT0M=")		//".1.3.6.1.4.1.1347.42.2.1.1.1.9.1"

//京瓷当前作业信息节点
#define KyoceraActiveJobInfoJobCountOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAh9YfVRNAH1F")	//".1.3.6.1.4.1.1347.47.1.1.1.2.1"		//当前激活打印作业个数

#define KyoceraActiveJobInfoJobIDOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNAA==")	//".1.3.6.1.4.1.1347.47.2.1.1.2"		//作业ID
#define KyoceraActiveJobInfo3OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNAQ==")	//".1.3.6.1.4.1.1347.47.2.1.1.3"		//（整形数字）
#define KyoceraActiveJobInfoJobTypeOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNBg==")	//".1.3.6.1.4.1.1347.47.2.1.1.4"		//作业类型，1-打印，2-复印
#define KyoceraActiveJobInfoJobNameOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNBw==")	//".1.3.6.1.4.1.1347.47.2.1.1.5"		//作业名称(多字节存储)
#define KyoceraActiveJobInfoJobStateOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNBA==")	//".1.3.6.1.4.1.1347.47.2.1.1.6"		//作业状态，1-处理中，2-打印中，4-暂停，5-等待中
#define KyoceraActiveJobInfoJobBeginTimeOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNBQ==")	//".1.3.6.1.4.1.1347.47.2.1.1.7"		//作业开始时间，1970-01-01与当前时间的秒数之差
#define KyoceraActiveJobInfo8OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNCg==")	//".1.3.6.1.4.1.1347.47.2.1.1.8"		//（字符串）
#define KyoceraActiveJobInfoPrintedCountOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNCw==")	//".1.3.6.1.4.1.1347.47.2.1.1.9"		//已经出纸页数
#define KyoceraActiveJobInfoPrintedCopyOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2M=")	//".1.3.6.1.4.1.1347.47.2.1.1.10"		//已经出纸份数
#define KyoceraActiveJobInfoTotalCopyOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2I=")	//".1.3.6.1.4.1.1347.47.2.1.1.11"		//总份数
#define KyoceraActiveJobInfoUserNameOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2E=")	//".1.3.6.1.4.1.1347.47.2.1.1.12"		//用户名
#define KyoceraActiveJobInfo13OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2A=")	//".1.3.6.1.4.1.1347.47.2.1.1.13"		//(不存在此节点)
#define KyoceraActiveJobInfoFromOID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2c=")	//".1.3.6.1.4.1.1347.47.2.1.1.14"		//连接至
#define KyoceraActiveJobInfoOrgDocPageCountOID	TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2Y=")	//".1.3.6.1.4.1.1347.47.2.1.1.15"		//原稿页数
#define KyoceraActiveJobInfo16OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2U=")	//".1.3.6.1.4.1.1347.47.2.1.1.16"		//（整形数字）
#define KyoceraActiveJobInfoColorTypeOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAR9YfVRNA2Q=")	//".1.3.6.1.4.1.1347.47.2.1.1.17"		//输出色彩，0-黑白，1-全色，6-自动色彩（彩色／灰度），7-自动色彩，8-混合

//京瓷历史作业信息节点
#define KyoceraHistoryJobInfoJobCountOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcAh9YfVRNBH1F")	//".1.3.6.1.4.1.1347.47.1.1.1.6.1"		//历史打印作业总日志个数

#define KyoceraHistoryJobInfoJobIDOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNAA==")	//".1.3.6.1.4.1.1347.47.6.1.1.2"		//作业ID
#define KyoceraHistoryJobInfoJobTypeOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNAQ==")	//".1.3.6.1.4.1.1347.47.6.1.1.3"		//作业类型，1-打印，2-复印
#define KyoceraHistoryJobInfoJobNameOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNBg==")	//".1.3.6.1.4.1.1347.47.6.1.1.4"		//作业名称(多字节存储)
#define KyoceraHistoryJobInfoJobStateOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNBw==")	//".1.3.6.1.4.1.1347.47.6.1.1.5"		//作业完成状态，0-完成，1-取消，2-错误
#define KyoceraHistoryJobInfoBeginTimeOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNBA==")	//".1.3.6.1.4.1.1347.47.6.1.1.6"		//作业开始时间，1970-01-01与当前时间的秒数之差
#define KyoceraHistoryJobInfoEndTimeOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNBQ==")	//".1.3.6.1.4.1.1347.47.6.1.1.7"		//作业结束时间，1970-01-01与当前时间的秒数之差
#define KyoceraHistoryJobInfo8OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNCg==")	//".1.3.6.1.4.1.1347.47.6.1.1.8"		//（字符串）
#define KyoceraHistoryJobInfoPrintedCountOID	TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNCw==")	//".1.3.6.1.4.1.1347.47.6.1.1.9"		//已经出纸页数
#define KyoceraHistoryJobInfoPrintedCopyOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2M=")	//".1.3.6.1.4.1.1347.47.6.1.1.10"		//已经出纸份数
#define KyoceraHistoryJobInfoTotalCopyOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2I=")	//".1.3.6.1.4.1.1347.47.6.1.1.11"		//总份数
#define KyoceraHistoryJobInfoUserNameOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2E=")	//".1.3.6.1.4.1.1347.47.6.1.1.12"		//用户名
#define KyoceraHistoryJobInfo13OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2A=")	//".1.3.6.1.4.1.1347.47.6.1.1.13"		//（字符串）
#define KyoceraHistoryJobInfo14OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2c=")	//".1.3.6.1.4.1.1347.47.6.1.1.14"		//（字符串）
#define KyoceraHistoryJobInfoOrgDocPageCountOID	TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2Y=")	//".1.3.6.1.4.1.1347.47.6.1.1.15"		//原稿页数
#define KyoceraHistoryJobInfo16OID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2U=")	//".1.3.6.1.4.1.1347.47.6.1.1.16"		//（整形数字）
#define KyoceraHistoryJobInfoColorTypeOID		TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFZcBR9YfVRNA2Q=")	//".1.3.6.1.4.1.1347.47.6.1.1.17"		//输出色彩，0-黑白，6-自动色彩（彩色／灰度），1-全色，7-自动色彩，8-混合

#define KyoceraModelOID							TEXT("H1h9Vk0EfUVPQB0AR2FQTQF9Rk9DHQJHYg==")	//".1.3.6.1.2.1.25.3.2.1.3.1"				//型号OID
#define ECOSYS_P5021cdn_A4PrintHBOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9bfVRNA31HT0MdAA==")	//".1.3.6.1.4.1.1347.42.2.2.1.1.3.1.1"
#define ECOSYS_P5021cdn_A4PrintColorOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9bfVRNA31HT0MdAw==")	//".1.3.6.1.4.1.1347.42.2.2.1.1.3.1.2"//型号OID
#define ECOSYS_P5021cdn_A4PrintTotalOID			TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFJcAgFHYktSHGJGT0MdAA==")	//".1.3.6.1.4.1.1347.43.10.1.1.12.1.1"//型号OID
#define FS_1060DN_A4PrintHBOID					TEXT("H1h9Vk0EfUVPQB0AR2dWTQNjWlNcAh9dfVRNAw==")	//".1.3.6.1.2.1.43.10.2.1.4.1.1"//型号OID

#define FS_1060DN_A4PrintHBOID					TEXT("H1h9Vk0EfUVPQB0AR2dWTQNjWlNcAh9dfVRNAw==")	//".1.3.6.1.2.1.43.10.2.1.4.1.1"//型号OID
#define TASKalfa6002iA4PrintHBOID				TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAB9YfVRNA31FT0M=")	//".1.3.6.1.4.1.1347.42.3.1.1.1.1.1"//型号OID
#define TASKalfa6002iA4TotalOID				    TEXT("H1h9Vk0EfUVPRh0AR2JWVwV9QFNcAR9cfVRNA31HT0M=")	//".1.3.6.1.4.1.1347.42.2.5.1.1.3.1"//型号OID


#endif	//_KYOCERA_MIB_DEF_AA638EB3_5BEE_4D0E_B435_15BA8FF4DF31_H_
