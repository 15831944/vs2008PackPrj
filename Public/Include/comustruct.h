#ifndef COMU_STRUCT_
#define COMU_STRUCT_
#pragma pack (push,1)
/*
typedef struct {	
	//MARSFH_DRVGETDIRECTDRAWINFO MarsFH_DrvGetDirectDrawinfo;
	MARSFH_DRVBITBLT MarsFH_DrvBitBlt;	
	MARSFH_DRVLINETO MarsFH_DrvLineTo;
	MARSFH_DRVSTRETCHBLT MarsFH_DrvStretchBlt;	
	MARSFH_DRVTEXTOUT MarsFH_DrvTextOut;	
	MARSFH_DRVSTROKEPATH MarsFH_DrvStrokePath;	
	MARSFH_DRVPAINT MarsFH_DrvPaint;	
	MARSFH_DRVCOPYBITS MarsFH_DrvCopyBits;		
	MARSFH_DRVSAVESCREENBITS MarsFH_DrvSaveScreenBits;
	MARSFH_DRVSTROKEANDFILLPATH MarsFH_DrvStrokeAndFillPath;
	MARSFH_DRVFILLPATH MarsFH_DrvFillPath;
	MARSFH_DRVDRAWESCAPE MarsFH_DrvDrawEscape;
	MARSFH_DDBLT MarsFH_DdBlt;
	MARSFH_DDLOCK MarsFH_DdLock;
	DELETESHAREDFILE DeleteSharedFile;
	CREATENOTIFYEVENT CreateNotifyEvent;
	CLOSENOTIFYEVENT  CloseNotifyEvent;
	MARSFH_DRVENABLESURFACE  MarsFH_DrvEnableSurface;
	MARSFH_DRVDISABLESURFACE  MarsFH_DrvDisableSurface;
	MARSFH_DRVASSERTMODE   MarsFH_DrvAssertMode;
	MARSFH_DDFLIP MarsFH_DDFlip;
	MARSFH_DDUPDATEOVERLAP MarsFH_DDUpOverlap;
	MARSFH_STARTCAPTURE   MarsFH_StartCapture;
	MARSFH_GETCAPTURERECT MarsFH_GetCapturerect;
	PVIDEOFUNC                MarsFS_VideoFunc;
} COMMUNICATION_BLOCK, *PCOMMUNICATION_BLOCK;
*/
typedef struct {
	DWORD MarsFH_DrvBitBlt;	
 	DWORD MarsFH_DrvLineTo;
    DWORD MarsFH_DrvStretchBlt;	
	DWORD MarsFH_DrvTextOut;	
	DWORD MarsFH_DrvStrokePath;	
//	MARSFH_DRVPAINT MarsFH_DrvPaint;	
	DWORD MarsFH_DrvCopyBits;		
//	MARSFH_DRVSAVESCREENBITS MarsFH_DrvSaveScreenBits;
 	DWORD MarsFH_DrvStrokeAndFillPath;
//	MARSFH_DRVFILLPATH MarsFH_DrvFillPath;
//	MARSFH_DRVDRAWESCAPE MarsFH_DrvDrawEscape;
//	MARSFH_DDBLT MarsFH_DdBlt;
//	MARSFH_DDLOCK MarsFH_DdLock;
//	DELETESHAREDFILE DeleteSharedFile;
//	CREATENOTIFYEVENT CreateNotifyEvent;
//	CLOSENOTIFYEVENT  CloseNotifyEvent;
	//MARSFH_DRVENABLESURFACE  MarsFH_DrvEnableSurface;
	//MARSFH_DRVDISABLESURFACE  MarsFH_DrvDisableSurface;
	DWORD   MarsFH_DrvEnableSurface;
	DWORD  MarsFH_DrvDisableSurface;
} COMMUNICATION_BLOCK, *PCOMMUNICATION_BLOCK;


typedef struct {
	__int64 MarsFH_DrvBitBlt;	
	__int64  MarsFH_DrvLineTo;
	__int64  MarsFH_DrvStretchBlt;	
	__int64  MarsFH_DrvTextOut;	
	__int64  MarsFH_DrvStrokePath;	
	//	MARSFH_DRVPAINT MarsFH_DrvPaint;	
	__int64  MarsFH_DrvCopyBits;		
	//	MARSFH_DRVSAVESCREENBITS MarsFH_DrvSaveScreenBits;
	__int64  MarsFH_DrvStrokeAndFillPath;
	//	MARSFH_DRVFILLPATH MarsFH_DrvFillPath;
	//	MARSFH_DRVDRAWESCAPE MarsFH_DrvDrawEscape;
	//	MARSFH_DDBLT MarsFH_DdBlt;
	//	MARSFH_DDLOCK MarsFH_DdLock;
	//	DELETESHAREDFILE DeleteSharedFile;
	//	CREATENOTIFYEVENT CreateNotifyEvent;
	//	CLOSENOTIFYEVENT  CloseNotifyEvent;
	//MARSFH_DRVENABLESURFACE  MarsFH_DrvEnableSurface;
	//MARSFH_DRVDISABLESURFACE  MarsFH_DrvDisableSurface;
	__int64    MarsFH_DrvEnableSurface;
	__int64   MarsFH_DrvDisableSurface;
} COMMUNICATION_BLOCK_X64, *PCOMMUNICATION_BLOCK_X64;

typedef struct 
{		
  DWORD   MarsFH_DrvEnableSurface;
  DWORD  MarsFH_DrvDisableSurface;
} COMMUNICATION_IMGSYS_BLOCK, *PCOMMUNICATION_IMGSYS_BLOCK;

#pragma pack (pop)
#endif 