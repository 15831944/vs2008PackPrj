//���˴�ӡ������

#pragma once
#include "snmpinterface.h"

#define Use_Html_Get_Meter	0	//�Ƿ�ʹ��Html�ķ�ʽ��ȡ������Ϣ��0-��1-��

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
	int m_nPrintedTotal;//�Ѵ�ӡ���ܼ���
	int m_nHBTotal;//��ӡ��ɫ���ܼ���
	int m_nPrintedColorTotal;//�Ѵ�ӡ��ɫ�ܼ���
	int m_nA4PrintedHBTotal;//�Ѵ�ӡ��ɫA4-��ֽͨ�ż���
	int m_nA4PrintedColorTotal;//�Ѵ�ӡ��ɫA4-��ֽͨ���ܼ���
	int m_nPrintedNet;//ɨ���÷������ӡ����
	int m_nValue;
	CString m_szPrinterModel;//��ӡ���ͺ�
	CString m_szPrinterSerialNo;//��ӡ�����к�
	CString m_szValue;
	CString m_szTonerLevel;//̼��ˮƽ
	CString m_szConsumeLevel;//����Ʒˮƽ
	CString m_szUsbPort;//ɨ���÷���USB�˿���Ϣ
	CString m_szAccepted;//ɨ������ҵ���ѽ���
protected:
	char *m_pBuf;
#endif

protected:
	BOOL GetPrintSidesCountInfo();	//��ȡ��ӡ������ͳ����Ϣ
	BOOL GetScanSidesCountInfo();	//��ȡɨ�������ͳ����Ϣ
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
