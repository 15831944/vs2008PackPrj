#pragma once
#define CARD_REGISTY_KEY				_T("SOFTWARE\\iSecStar\\CardConfig\\")
#define PROJECT_REGISTY_KEY				_T("SOFTWARE\\iSecStar")

#define  DEF_NJLiDaShePin		0
#define  DEF_MHSle4442			1
#define  DEF_EmIDCard			2
#define  DEF_RD800Card			3
#define  DEF_BARCODE			4

#define  DEF_JINLONGONECARD     5
#define  DEF_CHENGDU_JDHUARUI   6
#define  DEF_KEYCOM_ONECARD     7
#define  DEF_ZH_ONECARD			8  //智慧一卡通
#define  DEF_XIN_SNAQIANG_CARD	9//
#define  DEF_JSDCARD			10//金仕达卡,add by zfq
#define	 DEF_RD600Card			11 //add by zfq
#define  DEF_MinHuaOHanCard		12 //明华澳汉读卡器(M1卡),add by zfq
#define  DEF_JingLongOneCard2	13 //第二个金龙一卡通
#define  DEF_BJDKYWOneCard      14  //add by xph北京迪克远望一卡通
#define  DEF_YinDaOneCard		15	//add by xph银达一卡通
#define  DEF_YiDong24GCard		16	//add by xph移动2.4G手机卡
#define  DEF_BaoShiOneCard		17	//add by xph宝石一卡通
#define  DEF_JsdCpuOneCardV311	18	//add by xph金士达CPU卡
#define  DEF_ZYZHOneCard		19	//add by xph正元智慧一卡通
#define  DEF_XinKPOneCard		20  //add by xph新开普一卡通
#define  DEF_CHQYLOneCard		21  //add by xph重庆英黎一卡通
#define  DEF_TWOneCard			22  //add by xph拓维信息一卡通
#define  DEF_BeiJingSdFz		23  //add by xph北京师大附中
#define  DEF_GUOFANGKDONECARD	24  //add by xph国防科大
#define  DEF_XZXNANINFOPROU     25  //add by xph新中新一卡通（南京信息工程大学）


#define  NJLiDaShePin		0
#define  MHSle4442			1
#define  EmIDCard			2
#define  RD800Card			3
#define  BARCODE			4

#define  JINLONGONECARD     5
#define  CHENGDU_JDHUARUI   6
#define  KEYCOM_ONECARD     7
#define  ZH_ONECARD			8  //智慧一卡通
#define  XIN_SNAQIANG_CARD	9//
#define  JSDCARD			10//金仕达卡,add by zfq
#define	 RD600Card			11 //add by zfq
#define  DEF_MinHuaOHanCard	12 //add by zfq,明华澳汉读卡器(M1卡)
#define  DEF_JingLongOneCard2	13 //金龙一卡通2
#define  BJDKYW_ONECARD     14  //add by xph北京迪克远望一卡通
#define  YINDA_ONECARD		15  //ADD BY XPH
#define  DEF_JingLongOneCard3	16 //金龙一卡通3(移动2.4G手机卡)
#define  BAOSHIONECARD	    17 //宝石一卡通
#define  JSDCPUONECARD		18 //金士达CPU卡
#define  ZYZHONECARD		19 //正元智慧一卡通
#define  XINKPONECARD		20 //新开普一卡通
#define  CHQYLONECARD		21 //重庆英黎一卡通
#define  TUOWEIONECARD		22 //拓维信息一卡通
#define  BEIJINGONECARD		23 //北京师大附中使用的一卡通
#define  GUOFANGKDONECARD	24 //国防科大使用的一卡通
#define  XZXNANINFOPROU		25 //新中新一卡通（南京信息工程大学使用）
#define	 SYX_JF				26	//从苏亚星机房中获取用户信息
#define  XINKPONECARD2		27  //新开普一卡通, 内蒙古工业大学,(要先在XINKPONECARD中配置好webservices和终端ID,然后选择XINKPONECARD2)
#define  XINKPONECARD4		28  //新开普一卡通, 南京铁道学院,(暂时弃用,使用内蒙古工业大学的XINKPONECARD2)
#define  GUANGXIRFIDP200U	29	//广西职业技术学院,深圳海恒+金盘
#define  BAOSHIONECARD3	    30	//宝石一卡通V3.0, 南京晓庄学院
#define  XINSANQIANGCARD	31	//鑫三强一卡通公司	南京邮电大学
#define	 SANDONGWENFAN		32	//山东潍坊(图书馆)
#define	 TIANHAINETWORK		33	//河北邯郸(海天网络一卡通)
#define	 SHIMINKA			34	//张家港图书馆(市民卡)(图创sip2接口)
#define	 GuiZhouCJDX		35	//贵州财经大学,(扣费与查询走HTTP接口,读卡走明华奥汉读卡器)
#define	 AsMFCard			36	//爱索一卡通，南京体育学院，在企业版利盟打印机中使用
#define  LiDaCard			37	//理达一卡能，南京航天航空大学
#define  SHDLXY				38	//上海电力学院
#define  XINKPONECARD5		39  //新开普一卡通和金盘数据库
#define  DaShiCard			40  //达实一卡通(香港中文大学，深圳分校，利盟打印机，企业打印版, 网络读卡器)
#define  IlasCard			41  //Ilas邯郸市图书馆，软键盘输入
#define  IDR210				42	//IDR210台式居民身份证读卡器
#define  FoShanBook			43	//佛山市图书馆，使用webservice验证，打印系统扣费
#define  BJDKYW_ONECARD3	44	//北京迪科远望一卡通3.0
#define  SZTReader			45	//深圳通读卡器
#define  DEF_TuoWeiKunMingXueYuan	46	//拓维昆明学院一卡通
#define  SHDL_BAOSHICARD	47	//上海电力-宝石一卡通，模拟键盘输入，验证LDAP, 走webservice扣费。
#define  DEF_TuChuangGuiLinLvYou 48  //桂林旅游学校，明华澳汉读卡，图创http扣费
#define  DEF_SanJiuZhiHuiJiNanDaXue 49  //三九智慧-济南大学
#define  DEF_YingFuSenShaoXingTuShuGuan 50 //英富森-绍兴图书馆
#define  DEF_JinShiDaNanJingDaXue	51 //金仕达-南京大学
#define  DEF_HaiHengHaMiLib			52 //海恒-哈密图书馆,IC卡，背面条码号，条码号也写入了IC卡的扇区中
#define  DEF_SZTWanDaShunTong       53  //深圳通-万达顺通
#define  DaShiCard2			54  //达实一卡通(香港中文大学，深圳分校，利盟打印机，企业打印版, COM串口读卡器)
#define  DeCardReader		55  //德卡读卡器
#define  DEF_ZhengYuanZhiHuiWuXiGongZhi	56	//正元智慧-无锡工业职业技术学院
#define  DEF_JinShiDaShanDaXueYuan	57	//金仕达-上海杉达学院
#define  DEF_TaiShang_Beijing11     58  //泰尚-北京11中学
#define	 DEF_XinKaiPuShenFenZhengDuKaQi 59 //新开普-身份证读卡器
#define  DEF_YingChuangBeiJingWangFuZhongXue   60//盈创-北京王府中学
#define  DEF_HD900IDReader      61  //HD-900身份证阅读器
#define  DEF_SuZhouXingHai      62  //苏州星海中学，MIFARE卡
#define  DEF_XingLongJi			63//辽大图书馆新龙基电子读卡器
#define	 DEF_YingDaHuiZhi     64//银达汇智-华北理工大学
#define	 DEF_TianHaiWangLuoGaoBeiDianLib     65 //天海网络-高碑店图书馆
#define  DEF_XinZhongXinIdCardReader		66  //新中新二代身份证读卡器
#define  DEF_ShuWeiNeiMengGuCaiJing			67	//树维-内蒙古财经
#define  DEF_TongFangRuiAnTFRF2061			68	//同方锐安-TFRF2061-北京飞机维修
#define  DEF_XiaoJingLingNingXiaShiFan		69	//兰大小精灵-宁夏师范
#define  DEF_ZhenZhiNeng					70	//真智能
#define  DEF_DaShiCardTianAnYunGu			71	//达实网络一卡通-天安云谷


