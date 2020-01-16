#ifndef _CHENGDU_JDHUR
#define _CHENGDU_JDHUR

typedef struct _JDHR_CARD_INFO
{
	char usercardid[52];
	char userName[52];
	char userCert[52];
	char userLeaveMoney[52];
	char userUsePc[52];
	int  IuseType;
}JDHR_CARD_INFO,*PJDHR_CARD_INFO;

#define CANT_LOG_ON_NOMONEY		1
#define SUC_LOG_ON_PC           2
#define SUC_LOG_OFF_PC          3
#define MOREONEDAY_FAILED_OG_OFF_PC          4
#endif