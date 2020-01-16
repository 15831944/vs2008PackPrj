#ifndef _DASCOM_MIB_DEF_AF7B6177_4730_4446_9628_7E6E73F6F2B0__H_
#define _DASCOM_MIB_DEF_AF7B6177_4730_4446_9628_7E6E73F6F2B0__H_

/************************************************************************/
/* DASCOM 得实（小票）打印机，获取打印机状态和抄表信息的网络接口说明
1.添加指令实现网口返回，具体如下：
A.1B 0D 42:返回相关状态共3Bytes
具体如下：
Byte1: 
	Bit7: 1：设备忙 0：非忙
	Bit6: 1：切刀出错 0：非错
	Bit5：1：缺纸 0：有纸
	Bit4：Reserved
	Bit3：Reserved
	Bit2：Reserved
	Bit1：1：卡纸出错 0：正常
	Bit0：Reserved

Byte2: 
	Bit7: 1：标签校验出错 0：正常
	Bit6: Reserved
	Bit5：Reserved
	Bit4：Reserved
	Bit3：1：缓冲非空 0：空
	Bit2：Reserved
	Bit1：Reserved
	Bit0：Reserved

Byte3: 
	Bit7:Reserved
	Bit6: 1：打印头过热 0：正常
	Bit5：Reserved
	Bit4：Reserved
	Bit3：打印模式 1：热敏 0：热转印
	Bit2：纸张类型 1：标签纸 0：连续纸
	Bit1：1：打印头抬起
	Bit0：1：碳带出错

B.1C 0E 43:返回打印量 8Bytes

C.1C 0E 50:返回打印页数 8Bytes
*/
/************************************************************************/

//请求设备信息命令数据
//unsigned char ucRequestDascomDeviceInfo[3] = {0x1B, 0x0D, 0x42};

//请求打印量命令数据
//unsigned char ucRequestDascomPrintUsage[3] = {0x1C, 0x0E, 0x43};

//请求打印页数命令数据
//unsigned char ucRequestDascomPrintPages[3] = {0x1C, 0x0E, 0x50};


#pragma pack(push, 1)

struct DascomDeviceInfo
{
	unsigned char ucDeviceStatus;
	unsigned char ucDeviceStatus2;
	union
	{
		unsigned char ucDeviceStatus3;
		unsigned char ucDeviceInfo;
	};
};

#if 0
union DascomPrintUsage
{
	unsigned __int64 ulData8;
	struct
	{
		unsigned int unHighData4;
		unsigned int unLowData4;
	};
};

typedef DascomPrintUsage DascomPrintPages;
#endif

#pragma pack(pop)

//DascomDeviceInfo.ucDeviceStatus
#define DascomDeviceStatus_Busy					0x80L	//1<<7	设备忙
#define DascomDeviceStatus_CutterError			0x40L	//1<<6	切刀出错
#define DascomDeviceStatus_NoPaper				0x20L	//1<<5	缺纸
#define DascomDeviceStatus_Jam					0x02L	//1<<1	卡纸

//DascomDeviceInfo.ucDeviceStatus2
#define DascomDeviceStatus2_LabelCheckError		0x80L	//1<<7	标签校验出错
#define DascomDeviceStatus2_BufferNotEmpty		0x08L	//1<<3	缓冲非空

//DascomDeviceInfo.ucDeviceStatus3
#define DascomDeviceStatus3_PrintHeadOverheat	0x40L	//1<<6	打印头过热
#define DascomDeviceStatus3_PrintHeadRaise		0x02L	//1<<1	打印头抬起
#define DascomDeviceStatus3_TTRError			0x01L	//1<<0	碳带出错(TTR Thermal Transfer Ribbons)

//DascomDeviceInfo.ucDeviceInfo
//纸张类型 1：标签纸 0：连续纸
#define DascomDeviceInfo_PaperType_Continuous		0x081L	//1<<3	标签纸
#define DascomDeviceInfo_PaperType_Label			0x00L	//0		连续纸

//打印模式 1：热敏 0：热转印
#define DascomDeviceInfo_PrintMode_Thermosensitive	0x10L	//1<<4	热敏
#define DascomDeviceInfo_PrintMode_ThermalTransfer	0x00L	//0		热转印

#endif	//_DASCOM_MIB_DEF_AF7B6177_4730_4446_9628_7E6E73F6F2B0__H_
