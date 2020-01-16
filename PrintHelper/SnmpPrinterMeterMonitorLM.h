//利盟打印机抄表

#pragma once
#include "snmpinterface.h"

#define Use_Html_Get_Meter	0	//是否使用Html的方式获取抄表信息，0-否，1-是

class CSnmpPrinterMeterMonitorLM : public CSnmpPrinterMeterMonitor
{
public:
	CSnmpPrinterMeterMonitorLM(void);
	~CSnmpPrinterMeterMonitorLM(void);

protected:
	BOOL GetMeterInfo();

#if Use_Html_Get_Meter
protected:
	list<char *>* HtmlToList(CString szUrl);
	BOOL HtmlToList(CString szUrl, CStringArray& szAry);
	BOOL GetMeterInfo_X654de();
	BOOL GetMeterInfo_XM1145();
	BOOL GetMeterInfo_X950();
	BOOL GetMeterInfo_MX811();
	BOOL GetMeterInfo_X860de();
	BOOL GetMeterInfo_MX610de();
	BOOL GetOtherMeterInfoByHtml_X654de();
	BOOL GetOtherMeterInfoByHtml_XM1145();
	BOOL GetOtherMeterInfoByHtml_XM1145_2();
	BOOL GetOtherMeterInfoByHtml_X950();
	BOOL GetOtherMeterInfoByHtml_MX811();
	BOOL GetOtherMeterInfoByHtml_MX811_2();
	BOOL GetOtherMeterInfoByHtml_X860de();
	BOOL GetOtherMeterInfoByHtml_X860de_2();
	BOOL GetOtherMeterInfoByHtml_MX610de();
	BOOL GetOtherMeterInfoByHtml_MX610de_2();

private:
	int m_nPrintedTotal;//已打印面总计数
	int m_nHBTotal;//打印单色面总计数
	int m_nPrintedColorTotal;//已打印彩色总计数
	int m_nA4PrintedHBTotal;//已打印单色A4-普通纸张计数
	int m_nA4PrintedColorTotal;//已打印彩色A4-普通纸张总计数
	int m_nPrintedNet;//扫描用法网络打印计数
	int m_nValue;
	CString m_szPrinterModel;//打印机型号
	CString m_szPrinterSerialNo;//打印机序列号
	CString m_szValue;
	CString m_szTonerLevel;//碳粉水平
	CString m_szConsumeLevel;//消耗品水平
	CString m_szUsbPort;//扫描用法下USB端口信息
	CString m_szAccepted;//扫描下作业的已接收
protected:
	char *m_pBuf;
#endif

protected:
	BOOL GetPrintSidesCountInfo();	//获取打印介质面统计信息
	BOOL GetScanSidesCountInfo();	//获取扫描介质面统计信息
	void InitOID();
	CString m_szGenCountEntryOID;
	CString m_szPaperGeneralCountEntryOID;
	CString m_szPaperSidesCountEntryOID;
	CString m_szPaperSheetsCountEntryOID;
	CString m_szPaperNupCountEntryOID;
	CString m_szPaperJobSizeEntryOID;
	CString m_szScanCountEntryOID;
	CString m_szCurrentSuppliesEntryOID;
	CString m_szSupplyHistoryEntryOID;
	CString m_szSupplyHistogramEntryOID;
};
