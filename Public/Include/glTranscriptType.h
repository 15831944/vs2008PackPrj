#ifndef __glTranscriptType_h__2014_11_19__
#define __glTranscriptType_h__2014_11_19__

//此头文件定义所有成绩打印、公告打印、单证打印的 类型

typedef struct tag_ST_TranscriptHead
{
	DWORD	dwSchool;			//学校
	DWORD	dwTransType;		//类型
	DWORD	dwPrtID;			//打印站点ID
	DWORD	dwLoadLen;			//后续数据长度，不包括本结构
	DWORD	dwMsg1Len;			//后面第一个Msg的长度
	DWORD	dwReserv2;	//保留2
	DWORD	dwReserv3;	//保留3
} ST_TranscriptHead;

#define DEF_School_HeiFeiXueYuan		1	//合肥学院
#define DEF_School_KeiMeiQuery			2	//柯美OEM相关确认
#define DEF_School_AHCJDX				3	//安徽财经大学
#define DEF_School_DBSFDX				4	//东北师范大学
#define DEF_School_DualScreen			5	//双屏信息
#define DEF_School_MobileAuthPayment	6	//移动设备(微信/QQ/...)登录/支付信息
#define DEF_School_BillPayment			7	//代缴费功能
#define DEF_School_Xab					8	//成绩打印内建类型
#define DEF_School_SJXY					9   //三江学院
#define DEF_School_DWJMDX				10  //对外经贸大学


//====================	成绩打印	================
#define DEF_TransType_ExamCard			1	//准考证
#define DEF_TransType_Transcript		2	//成绩打印
#define DEF_TransType_AtSchool			3	//在校证明打印,包括其它一些证件，如休学申请，免修申请等
#define DEF_TransType_KaoShi			4	//免缓重修申请
#define DEF_TransType_XueJi				5	//学籍异动申请
#define DEF_TransType_Custom			6	//自定义模板


//============	DEF_School_KeiMeiQuery	===============
#define DEF_TransType_KeiMeiReq			1	//打印站点向服务器确认当前是否开启柯美特色功能
#define DEF_TransType_KeiMeiReqPrtLog	2	//打印站点向服务器请求打印作业的历史记录


//==============================================================
#define DEF_TransType_DualScreenUrl		1	//打印站点向服务器确认当前双屏信息


//============  DEF_School_ThridAuthPayment	======================
#define DEF_TransType_QRCodeInfo			1	//打印站点向服务器请求,第三方(微信/QQ/...)登录/支付的二维码信息

//============= DEF_School_BillPaymet	======================
#define DEF_TransType_BillPaymentInfo		1	//代缴费信息信息
#define DEF_TransType_BillPaymentCharge		2	//代缴费支付信息
#define DEF_TransType_BillPaymentChargeLog	3	//代缴费支付信息,日志信息，可能的支付成功，日志没有删除

#endif //#ifndef __glTranscriptType_h__2014_11_19__

