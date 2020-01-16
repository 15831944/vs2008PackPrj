/************************************************************************/
/*		 本文件是企业打印配置文件，控制一些编译开关                       */
/************************************************************************/

#ifndef _GLOBALPRINTCONFIG_31C1BAE5_5795_4E2F_A617_D4DC4055DCE9_H_ 
#define _GLOBALPRINTCONFIG_31C1BAE5_5795_4E2F_A617_D4DC4055DCE9_H_

//打印机品牌标识定义开始，参考BRAND_TYPE
#define DEF_BT_SL			0
#define DEF_BT_DZ			1
#define DEF_BT_KM			2
#define DEF_BT_LM			3
#define DEF_BT_THIN_CLT		4
#define DEF_BT_LG			5
#define DEF_BT_SL2			6
#define DEF_BT_JC			7
#define DEF_BT_ZD			8
#define DEF_BT_XP			9
#define DEF_BT_SX			10
#define DEF_BT_HP			11
#define DEF_BT_EPSON		12
#define DEF_BT_CANON		13
#define DEF_BT_BROTHER		14
#define DEF_BT_OKI			15
#define DEF_BT_SEIKO		16
#define DEF_BT_PANTUM		17
#define DEF_BT_JOLIMARK		18
#define DEF_BT_GPRINTER		19
#define DEF_BT_PANASONIC	20
#define DEF_BT_LENOVO		21
#define DEF_BT_START		22
#define DEF_BT_XPRINTER		23
#define DEF_BT_SONY			24
#define DEF_BT_DELL			25
//打印机品牌标识定义结束

#define Enable_OEM_ONE_PRINTER_BRAND	1			//是否启用OEM一个打印机品牌，0-禁用，1-启用
#define OEM_PRINTER_BRAND				DEF_BT_HP	//OEM打印机品牌标识

#endif	//_GLOBALPRINTCONFIG_31C1BAE5_5795_4E2F_A617_D4DC4055DCE9_H_
