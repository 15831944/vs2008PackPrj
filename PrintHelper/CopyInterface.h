#pragma once

#include "PrinterBrand.h"

enum PAPER_TYPE
{
	A3_PAPER,
		A4_PAPER
};

enum PAPER_COLOR
{
	NULL_PAPER,
	WB_PAPER,
	COLOR_PAPER
};

enum COPY_INFO_FROM
{
	CIF_UnKnow,
	CIF_HardWare,
	CFI_Snmp
};

enum COPY_OP_TYPE
{
	COPY_START,
	COPY_STOP
};

class ICopyCmdInterface
{
public:
	virtual void OnInitSuc(){ASSERT(0);}
	virtual void OnCheckStateSuc(){ASSERT(0);}
	virtual void OnLockSuc(BOOL bLocked){ASSERT(0);}
	virtual void OnPowerSuc(BOOL bPowered){ASSERT(0);}
	
	virtual void OnPaperType(PAPER_TYPE){ASSERT(0);}
	virtual void OnPaperColor(PAPER_COLOR){ASSERT(0);}
	virtual void OnBeginOneCopy(){ASSERT(0);}
	virtual void OnOneCopyCmplt(){ASSERT(0);}
	virtual void OnOneCopyCmplt(PAPER_TYPE,PAPER_COLOR){ASSERT(0);}
	virtual void OnErr(int){ASSERT(0);}
};


class ICopyChargeInterface
{
public:
	virtual void OnOneCopyCmplt(void*){ASSERT(0);}
	virtual void OnCopyPriorChargeCmplt(void*){ASSERT(0);} //add by szy 20141211
	virtual void OnAllCopyCmplt(void*){ASSERT(0);}
	virtual void OnErr(int){ASSERT(0);}
	virtual void OnIdle(int ){ASSERT(0);}
	virtual void OnTimeout(){ASSERT(0);}
};