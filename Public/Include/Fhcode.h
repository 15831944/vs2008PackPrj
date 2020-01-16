/*++

Module Name:

    fhcode.h

Abstract:
	function hook 's in NT's driver
	control code headfile (IOCTL)

Environment:

    Kernel mode

Revision History:

    20-07-99 : created

--*/
// follow Headfile include by Windows NT's DDK
#include "..\public\include\Devioctl.h"
#ifndef _MARS_FHCODE_H_
#define _MARS_FHCODE_H_
//
// the base control code
//

#define DRV_APIFUNC_1 1
#define DRV_APIFUNC_2 2
#define DRV_APIFUNC_3 3
#define DRV_APIFUNC_4 4
#define DRV_APIFUNC_5 5
#define DRV_APIFUNC_6 6
#define DRV_APIFUNC_7 7
#define DRV_APIFUNC_8 8
#define DRV_APIFUNC_9 9
#define DRV_APIFUNC_10 10
#define DRV_APIFUNC_11 11
#define DRV_APIFUNC_12 12
#define DRV_APIFUNC_13 13
#define DRV_APIFUNC_14 14
#define DRV_APIFUNC_15 15
#define DRV_APIFUNC_16 16
#define DRV_APIFUNC_30 30
#define DRV_APIFUNC_31 31

//
// the VxD control code
//

#define DRV_FUNCHOOK_INSTALL			DRV_APIFUNC_1
#define DRV_FUNCHOOK_UNINSTALL			DRV_APIFUNC_2
#define DRV_FUNCHOOK_SETPARAMETER		DRV_APIFUNC_3
#define DRV_FUNCHOOK_GETUPDATERECT		DRV_APIFUNC_4
#define DRV_FUNCHOOK_DISABLERECT		DRV_APIFUNC_5
#define DRV_FUNCHOOK_ADDRECT			DRV_APIFUNC_6
#define DRV_FUNCHOOK_GETOLDFUNCTION		DRV_APIFUNC_7
#define DRV_FUNCHOOK_GETNEWFUNCTION		DRV_APIFUNC_8




//opqit_add_begin 02/09/24
#define DRV_FUNCHOOK_REFERENCE_EVENT  DRV_APIFUNC_9
#define DRV_FUNCHOOK_DEREFERENCE_EVENT DRV_APIFUNC_10
//opqit_add_end 02/09/24
//modified by jh 03/05/28
#define DRV_FUNCHOOK_DEREFERENCE_PROCID DRV_APIFUNC_11
#define DRV_GETINFO                     DRV_APIFUNC_12
//#define DRV_GETSPECIALUPEVENT           DRV_APIFUNC_13
//#define DRV_CLEARSPECIALUPEVENT         DRV_APIFUNC_14
#define DRV_GETIMGERECT                    DRV_APIFUNC_15

#define DRV_GETCOMUADDR                     DRV_APIFUNC_16

#define DRV_FUNCHOOK_GETUPDATERECTARRAY	    DRV_APIFUNC_30

#define DRV_FUNCHOOK_GET_RECTARRAY_IMGDATA	DRV_APIFUNC_31
//
// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
//

#define FILE_DEVICE_FUNCHOOK  0x00008300

//
// Macro definition for defining IOCTL and FSCTL function control codes.  Note
// that function codes 0-2047 are reserved for Microsoft Corporation, and
// 2048-4095 are reserved for customers.
//

#define FUNCHOOK_IOCTL_INDEX  0x830

//
// The NT device driver IOCTLs
//

//opqit_add_begin 02/09/24
#define IOCTL_FUNCHOOK_REFERENCE_EVENT        CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                              FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_REFERENCE_EVENT,  \
                                               METHOD_NEITHER,       \
                                               FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_DEREFERENCE_EVENT      CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                              FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_DEREFERENCE_EVENT,  \
                                               METHOD_NEITHER,       \
                                               FILE_ANY_ACCESS)
//opqit_add_end 02/09/24

#define IOCTL_FUNCHOOK_INSTALL          CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                              FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_INSTALL,  \
                                               METHOD_BUFFERED,       \
                                               FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_UNINSTALL        CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_UNINSTALL,  \
                                               METHOD_BUFFERED,       \
                                               FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_SETPARAMETER     CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_SETPARAMETER,  \
                                               METHOD_BUFFERED,       \
                                               FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GETUPDATERECT    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_GETUPDATERECT,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)


#define IOCTL_FUNCHOOK_DISABLERECT    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_DISABLERECT,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_ADDRECT		    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_ADDRECT,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GETOLDFUNCTION    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_GETOLDFUNCTION,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GETNEWFUNCTION    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_GETNEWFUNCTION,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GETPROCID   CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_DEREFERENCE_PROCID,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GETIMAGEINFO    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_GETINFO,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GETIMGERECT    CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_GETIMGERECT,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)
/*
//#define IOCTL_FUNCHOOK_GETSPECIALUPDATEEVENT   CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                               FUNCHOOK_IOCTL_INDEX|DRV_GETSPECIALUPEVENT,  \
                                               METHOD_BUFFERED,       \
                                              FILE_ANY_ACCESS)

//#define IOCTL_FUNCHOOK_CLEARSPECIALUPDATEEVENT   CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                                 FUNCHOOK_IOCTL_INDEX|DRV_CLEARSPECIALUPEVENT,  \
                                                 METHOD_BUFFERED,       \
                                                 FILE_ANY_ACCESS)
*/
#define IOCTL_FUNCHOOK_GETCOMUADDR   CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                                 FUNCHOOK_IOCTL_INDEX|DRV_GETCOMUADDR,  \
                                                 METHOD_BUFFERED,       \
                                                 FILE_ANY_ACCESS)
#define IOCTL_FUNCHOOK_GETUPDATERECTARRAY      CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                                 FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_GETUPDATERECTARRAY,  \
                                                 METHOD_BUFFERED,       \
                                                 FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GET_RECTARRAY_IMGDATA      CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
                                                 FUNCHOOK_IOCTL_INDEX|DRV_FUNCHOOK_GET_RECTARRAY_IMGDATA,  \
                                                 METHOD_NEITHER,       \
                                                 FILE_ANY_ACCESS)



#define IOCTL_FUNCHOOK_GETCOMUADDR_X64   CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
	FUNCHOOK_IOCTL_INDEX|64,  \
	METHOD_BUFFERED,       \
	FILE_ANY_ACCESS)
#define IOCTL_FUNCHOOK_GET_RECTARRAY_IMGDATA_X64      CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
	FUNCHOOK_IOCTL_INDEX|65,  \
	METHOD_NEITHER,       \
	FILE_ANY_ACCESS)

#define IOCTL_FUNCHOOK_GET_INTERFACE_API     CTL_CODE(FILE_DEVICE_FUNCHOOK,  \
	FUNCHOOK_IOCTL_INDEX|70,  \
	METHOD_BUFFERED,       \
	FILE_ANY_ACCESS)

#pragma pack (push,1)
typedef struct _CHANGERECTARRAY
{
   int    iRectNum;
   RECTL  pWindRectArray[200];   
}CHANGERECTARRAY,*pCHANGERECTARRAY;

typedef struct _CHANGERECTDATAANDARRAY
{
	int      iRectNum;
	RECTL *  pWindRectArray; 
	int      iImgBufferDataSize;      
    BYTE  *  pBuffer;
}CHANGERECTDATAANDARRAY,*pCHANGERECTDATAANDARRAY;

typedef struct _CHANGERECTDATAANDARRAY_X64
{
	int      iRectNum;
	union
	{
		__int64 ptrA;
		RECTL *  pWindRectArray; 
	};

	int      iImgBufferDataSize;    
	union
	{
		__int64 ptrB;
		BYTE  *  pBuffer;
	};
}CHANGERECTDATAANDARRAY_X64,*pCHANGERECTDATAANDARRAY_X64;

typedef BOOL (WINAPI *PFUpdateRect)(RECTL*,BOOL);
typedef BOOL (WINAPI *PFSetGDISurface)(void*);
typedef BOOL (WINAPI *PFCopySurface)(RECTL*);

typedef struct _DISPLAY_INTERFACE_API 
{
	PFSetGDISurface						pfSetGDISurface;
	PFUpdateRect						pfnUpdateRect;
}DISPLAY_INTERFACE_API ,*PDISPLAY_INTERFACE_API;

typedef struct _DISPLAY_CALLBACK_API 
{
	PFCopySurface						pfCopySurface;

}DISPLAY_CALLBACK_API,*PDISPLAY_CALLBACK_API;

#pragma pack (pop)
#endif
