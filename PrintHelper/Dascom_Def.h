#ifndef _DASCOM_MIB_DEF_AF7B6177_4730_4446_9628_7E6E73F6F2B0__H_
#define _DASCOM_MIB_DEF_AF7B6177_4730_4446_9628_7E6E73F6F2B0__H_

/************************************************************************/
/* DASCOM ��ʵ��СƱ����ӡ������ȡ��ӡ��״̬�ͳ�����Ϣ������ӿ�˵��
1.���ָ��ʵ�����ڷ��أ��������£�
A.1B 0D 42:�������״̬��3Bytes
�������£�
Byte1: 
	Bit7: 1���豸æ 0����æ
	Bit6: 1���е����� 0���Ǵ�
	Bit5��1��ȱֽ 0����ֽ
	Bit4��Reserved
	Bit3��Reserved
	Bit2��Reserved
	Bit1��1����ֽ���� 0������
	Bit0��Reserved

Byte2: 
	Bit7: 1����ǩУ����� 0������
	Bit6: Reserved
	Bit5��Reserved
	Bit4��Reserved
	Bit3��1������ǿ� 0����
	Bit2��Reserved
	Bit1��Reserved
	Bit0��Reserved

Byte3: 
	Bit7:Reserved
	Bit6: 1����ӡͷ���� 0������
	Bit5��Reserved
	Bit4��Reserved
	Bit3����ӡģʽ 1������ 0����תӡ
	Bit2��ֽ������ 1����ǩֽ 0������ֽ
	Bit1��1����ӡͷ̧��
	Bit0��1��̼������

B.1C 0E 43:���ش�ӡ�� 8Bytes

C.1C 0E 50:���ش�ӡҳ�� 8Bytes
*/
/************************************************************************/

//�����豸��Ϣ��������
//unsigned char ucRequestDascomDeviceInfo[3] = {0x1B, 0x0D, 0x42};

//�����ӡ����������
//unsigned char ucRequestDascomPrintUsage[3] = {0x1C, 0x0E, 0x43};

//�����ӡҳ����������
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
#define DascomDeviceStatus_Busy					0x80L	//1<<7	�豸æ
#define DascomDeviceStatus_CutterError			0x40L	//1<<6	�е�����
#define DascomDeviceStatus_NoPaper				0x20L	//1<<5	ȱֽ
#define DascomDeviceStatus_Jam					0x02L	//1<<1	��ֽ

//DascomDeviceInfo.ucDeviceStatus2
#define DascomDeviceStatus2_LabelCheckError		0x80L	//1<<7	��ǩУ�����
#define DascomDeviceStatus2_BufferNotEmpty		0x08L	//1<<3	����ǿ�

//DascomDeviceInfo.ucDeviceStatus3
#define DascomDeviceStatus3_PrintHeadOverheat	0x40L	//1<<6	��ӡͷ����
#define DascomDeviceStatus3_PrintHeadRaise		0x02L	//1<<1	��ӡͷ̧��
#define DascomDeviceStatus3_TTRError			0x01L	//1<<0	̼������(TTR Thermal Transfer Ribbons)

//DascomDeviceInfo.ucDeviceInfo
//ֽ������ 1����ǩֽ 0������ֽ
#define DascomDeviceInfo_PaperType_Continuous		0x081L	//1<<3	��ǩֽ
#define DascomDeviceInfo_PaperType_Label			0x00L	//0		����ֽ

//��ӡģʽ 1������ 0����תӡ
#define DascomDeviceInfo_PrintMode_Thermosensitive	0x10L	//1<<4	����
#define DascomDeviceInfo_PrintMode_ThermalTransfer	0x00L	//0		��תӡ

#endif	//_DASCOM_MIB_DEF_AF7B6177_4730_4446_9628_7E6E73F6F2B0__H_
