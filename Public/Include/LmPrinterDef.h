#pragma once

#define WM_PRINT_MANAGESOCK_ONLINE			WM_USER + 800 
//登陆
#define WM_PRINT_CHECK_USER						WM_USER+900 

//打印命令
#define WM_PRINT_REQUEST_JOB					WM_USER+1000 
#define WM_PRINT_NO_JOB							WM_USER+1001 
#define WM_PRINT_JOB_CMPLT						WM_USER+1002 
#define WM_PRINT_JOB_REQ_CMPLT					WM_USER+1003

//复印命令
#define WM_PRINT_COPY_OPERATION_END				WM_USER+1010 
#define WM_PRINT_COPY_OPERATION_START			WM_USER+1011 
#define WM_PRINT_COPY_COUNT						WM_USER+1012
#define WM_PRINT_SCAN_OPERATION_END				WM_USER+1013 
#define WM_PRINT_SCAN_OPERATION_START			WM_USER+1014 
#define WM_PRINT_SCAN_COUNT						WM_USER+1015

#define WM_PRINT_WAIT_OPERATION_END_MSG			WM_USER+1016 
#define WM_PRINT_COPY_SCAN_TIMER_SPAN			WM_USER+1017 
#define WM_PRINT_LOCK_SCREEN					WM_USER+1018 
#define WM_PRINT_IDLE_TIME_OUT_SECOND			WM_USER+1019 

#define WM_PRINT_PARAM							WM_USER+1030

#define WM_PRINT_PROCESS						WM_USER+1040
#define WM_PRINT_ONE_PAPER						WM_USER+1041
#define WM_PRINT_ALL_PAPER						WM_USER+1041

//注意不能有中文字符
typedef struct tag_LM_Param
{
	char	szTitle[100];
	char	szFtpIP[20];
	char	szFtpPort[20];
	char	szFtpUser[20];
	char	szFtpPwd[100];
	char	szFtpDir[100];
	char	szDebugLog[10];
	char	szIdleTimeout[10];
	char	szLogonType[10];
	char	szFaxType[10];	//add by zfq,2012.12.19
	char	szPlatformV3[10];	//add by zxl,2013.11.08,是否是V3平台(显示打印价格)
	char	szPrintCountOid[100];	//add by zxl,2013.11.08,打印计数的Snmp的oid
	char	szScan2Mail[10];
	char	szMailSender[50];
	char	szMailUser[50];
	char	szMailPwd[50];
	char	szMailSmtpSvr[50];
	char	szMailSmtpPort[10];
	char	szMailGzCompress[10];
	char	szMailSSL[10];
	char	szMailTest[50];
	char	szPrintMulIDCard[10];
}LM_PARAM,*PLMPARAM;

