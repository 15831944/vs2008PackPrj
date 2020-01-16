#ifndef PRINTERBRAND_H
#define PRINTERBRAND_H

/// 打印机品牌类型, 从CopyInterfa.h 中copy 过来
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
    BT_DELL,			//add by zxl 2016.05.24,戴尔
    BT_PRINTRONIX,		//add by lzj 2017-06-28,普印力
    BT_AVISION,			//add by lzj 2017-07-24,虹光扫描仪
    BT_TSC,			//add by lzj 2018-06-25,TSC条码机
    BT_DASCOM		//add by zxl,2018.11.21,得实（小票打印机）
};

#endif
