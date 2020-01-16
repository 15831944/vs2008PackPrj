#ifndef _PRINTPROCESSDEF_7827BD1F_2D30_4055_8A19_1FF3B780BE31_H_ 
#define _PRINTPROCESSDEF_7827BD1F_2D30_4055_8A19_1FF3B780BE31_H_

//打印处理的回调接口和结构体定义

typedef struct tag_PrintProcessCallBackData 
{
	int nPrintedPage;
	BOOL bPrintComplete;
	BOOL bError;
	void *pData;
}PrintProcessCallBackData, *PPrintProcessCallBackData;

typedef void (WINAPI *FPrintProcessCallBack)(PrintProcessCallBackData* pCallBackData);

#endif //_PRINTPROCESSDEF_7827BD1F_2D30_4055_8A19_1FF3B780BE31_H_
