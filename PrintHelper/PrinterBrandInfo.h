#pragma once

/*
enum BRAND_TYPE
{
	BT_SL,
	BT_DZ,
	BT_KM,
	BT_LM,
	BT_THIN_CLT,
	BT_LG,		//add by zxl,2013.03.08,理光打印机
	BT_SL2,		//add by zxl,2013.07.01,施乐的新电路板
	BT_JC,		//add by zxl,2013.09.16,京瓷打印机
	BT_ZD,		//add by zxl,2013.09.17,震旦打印机
	BT_XP,		//add by zxl,2013.08.09,夏普打印机
	BT_SX,		//add by zxl,2015.04.27,三星打印机
	BT_HP,		//add by zxl,2015.07.16,惠普打印机
	BT_EPSON,	//add by szy 2016.04.20,爱普生打印机
	BT_CANON,	//add by szy 2016.04.20,佳能打印机
	BT_BROTHER, //add by zfq 2016.05.04,兄弟打印机
	BT_JT,		//add by zfq 2016.05.04,京图打印机
	BT_OKI,		//add by zfq 2016.05.04,OKI打印机
	BT_SEIKO,	//add by zxl 2016.05.18,精工打印机
	BT_PANTUM,		//add by zxl 2016.05.24,奔图
	BT_JOLIMARK,	//add by zxl 2016.05.24,映美
	BT_GPRINTER,	//add by zxl 2016.05.24,佳博
	BT_PANASONIC,	//add by zxl 2016.05.24,松下
	BT_LENOVO,		//add by zxl 2016.05.24,联想
	BT_START,		//add by zxl 2016.05.24,实达
	BT_XPRINTER,	//add by zxl 2016.05.24,芯烨
	BT_SONY,		//add by zxl 2016.05.24,索尼
	BT_DELL			//add by zxl 2016.05.24,戴尔
};
*/

struct PrinterBrandInfo
{
	BRAND_TYPE nBrand;					//品牌内部代码，参考BRAND_TYPE
	int nSnmpPrivateKey;		//snmp的品牌私有识别码
	TCHAR cManufacturer[100];	//品牌名称（英文）
	TCHAR cManufacturer2[200];	//品牌名称（中文）
};

static PrinterBrandInfo gPrinterBrandInfoTable[] = 
{
	{BT_THIN_CLT,	0,		_T("UnKnow"),		_T("未知")},
	{BT_HP,			11,		_T("HP"),			_T("惠普")},
	{BT_SX,			236,	_T("Samsung"),		_T("三星")},
	{BT_SL2,		253,	_T("XEROX"),		_T("施乐")},
	{BT_SL2,		297,	_T("FUJI XEROX"),	_T("富士施乐")},
	{BT_LG,			367,	_T("RICOH"),		_T("理光")},
	{BT_LM,			641,	_T("Lexmark"),		_T("利盟")},
	{BT_DZ,			1129,	_T("TOSHIBA"),		_T("东芝")},
	{BT_EPSON,		1248,	_T("EPSON"),		_T("爱普生")},
	{BT_JC,			1347,	_T("KYOCERA"),		_T("京瓷")},
	{BT_KM,			18334,	_T("KONICA MINOLTA"), _T("柯尼卡美能达")},
	{BT_XP,			2385,	_T("SHARP"),		_T("夏普")},
	{BT_CANON,		1602,	_T("Canon"),		_T("佳能")},
	{BT_BROTHER,	2435,	_T("Brother"),		_T("兄弟")},
	{BT_JT,			44966,	_T("Jingtu"),		_T("京图")},
	{BT_ZD,			111,	_T("AURORA"),		_T("震旦")},
	{BT_OKI,		2001,	_T("Oki"),			_T("冲电气")},
	{BT_SEIKO,		263,	_T("Seiko"),		_T("精工")},
	{BT_PANTUM,		40093,	_T("Pantum"),		_T("奔图")}
};

char* GetManufacturer(int nBrand)
{
	char* pManufacturer = gPrinterBrandInfoTable[0].cManufacturer;
	int nCount = (sizeof(gPrinterBrandInfoTable)/sizeof(gPrinterBrandInfoTable[0]));
	for (int nIndex=1; nIndex<nCount; nIndex++)
	{
		if (gPrinterBrandInfoTable[nIndex].nBrand == nBrand)
		{
			pManufacturer = gPrinterBrandInfoTable[nIndex].cManufacturer;
			break;
		}
	}
	return pManufacturer;
}

char* GetManufacturer2(int nBrand)
{
	char* pManufacturer2 = gPrinterBrandInfoTable[0].cManufacturer2;
	int nCount = (sizeof(gPrinterBrandInfoTable)/sizeof(gPrinterBrandInfoTable[0]));
	for (int nIndex=1; nIndex<nCount; nIndex++)
	{
		if (gPrinterBrandInfoTable[nIndex].nBrand == nBrand)
		{
			pManufacturer2 = gPrinterBrandInfoTable[nIndex].cManufacturer2;
			break;
		}
	}
	return pManufacturer2;
}

BRAND_TYPE GetBrandType(int nSnmpPrivateKey)
{
	BRAND_TYPE nBrand = gPrinterBrandInfoTable[0].nBrand;
	int nCount = (sizeof(gPrinterBrandInfoTable)/sizeof(gPrinterBrandInfoTable[0]));
	for (int nIndex=1; nIndex<nCount; nIndex++)
	{
		if (gPrinterBrandInfoTable[nIndex].nSnmpPrivateKey == nSnmpPrivateKey)
		{
			nBrand = gPrinterBrandInfoTable[nIndex].nBrand;
			break;
		}
	}
	return nBrand;
}
