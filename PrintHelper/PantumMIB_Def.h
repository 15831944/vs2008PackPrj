#ifndef _PANTUM_MIB_DEF_F904_4328_A21A_93B9FB5FC41F_H_
#define _PANTUM_MIB_DEF_F904_4328_A21A_93B9FB5FC41F_H_

#include "Snmp_Def.h"

//奔图打印机抄表节点
#define PantumTotalPageOID					TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcBx9Y")				//".1.3.6.1.4.1.40093.6.4.1"	
#define PantumA4PrintTotalPageOID			TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcBx9Y")				//".1.3.6.1.4.1.40093.6.4.1"
#define PantumA4HBTotalPageOID				TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcBx9Y")				//".1.3.6.1.4.1.40093.6.4.1"

#define PantumTonerLevelPercentOID				TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAB9Y")		//".1.3.6.1.4.1.40093.6.3.1"
#define PantumCoverStausOID						TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAR9d")		//".1.3.6.1.4.1.40093.6.2.4"
#define PantumPrinterStatus						TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAR9Y")			//.1.3.6.1.4.1.40093.6.2.1
#define PantumTonerStatus						TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAR9b")		//.1.3.6.1.4.1.40093.6.2.2
#define PantumTrayPaperErrorOID					TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAR9c")		//.1.3.6.1.4.1.40093.6.2.5
#define PantumFatalErrorOID						TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAR9f")		//.1.3.6.1.4.1.40093.6.2.6
#define PantumAlertBaseOID						TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAQ==")		//.1.3.6.1.4.1.40093.6.2
#define PantumDrumStatusOID						TEXT("H1h9Vk0EfUVPRh0AR2dVUwtgWldcAR9a")		//.1.3.6.1.4.1.40093.6.2.3

// add by zmm, 2019.1.10 {
#define PantumDynamicRequstOID      _T(".1.3.6.1.4.1.40093.1.1.3.16.2") // 奔图动态查询, 用来查询当前打印机的所有状态
#define PantumStaticRequstOID      _T(".1.3.6.1.4.1.40093.1.1.3.16.1") // 奔图静态查询, 用来查询当前打印机的所有印量信息
// }

#endif	//_XEROX_MIB_DEF_8BE86FA5_B805_43A0_AEDD_681C9DAEDF4A_H_
