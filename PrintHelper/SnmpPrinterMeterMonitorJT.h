#pragma once
#include "snmpinterface.h"

//京图宽幅打印机 基于JT1500

class CSnmpPrinterMeterMonitorJT : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorJT(void);
	virtual ~CSnmpPrinterMeterMonitorJT(void);

protected:
	BOOL GetMeterInfo();

protected:
	void InitOID();
	CString m_szJingTuAllTotalDistanceOID;
	CString m_szJingTuAllTotalPageOID;
	CString m_szJingTuPrintDistanceOID;
	CString m_szJingTuPrintAreaOID;
	CString m_szJingTuCopyDistanceOID;
	CString m_szJingTuCopyAreaOID;
	CString m_szJingTuPaper1LevelPercentOID;
	CString m_szJingTuPaper2LevelPercentOID;
	CString m_szJingTuPaper3LevelPercentOID;
	CString m_szJingTuPaper4LevelPercentOID;
	CString m_szJingTuTonerBlackLevelPercentOID;
};
