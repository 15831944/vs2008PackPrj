#pragma once
#include "snmpinterface.h"
#include "RFC1213MIB_Def.h"
#include "HostResourcesMIB_Def.h"
#include "PrinterInfoMIB_Def.h"

class _AA_DLL_EXPORT_ CSnmpInfoHelper : public CSnmpOP
{
public:
	CSnmpInfoHelper(void);
	~CSnmpInfoHelper(void);

	BOOL InitConnect(CString szIP, CString szCommunity = "public");
	CString GetPrinterMac();

	//add by zfq,2016-09-07 取远端IP的MAC地址
	CString GetRmtMac(LPCTSTR szRmtIP);

	BOOL IsPrinter();
	CString GetModel();
	CString GetManufacturer();
	CString GetManufacturer2();
	BRAND_TYPE GetBrandType();
	int GetManufacturerSnmpPrivateKey();	//获取设备的厂家私有SNMP的子Key
	CString GetPrinterSerialNumber();		//获取设备的序列号
	BOOL IsColorCapable();			//是否支持彩色打印
	BOOL IsDoublePrintCapable();	//是否支持双面打印
	CString GetSysName();

protected:
	//通过SNMP获取OID信息
	BOOL GetAll();
	BOOL GetSystem();
	BOOL GetInterface();
	BOOL GetHrSystem();
	BOOL GetHrDevice();
	BOOL GetHrPrinter();
	BOOL GetPrtGeneral();
	BOOL GetPrtMarkerColorant();
	BOOL GetPrtMediaPath();

	//清理SNMP获取到的OID信息
	void ClearAll();
	void ClearSystem();
	void ClearInterface();
	void ClearHrSystem();
	void ClearHrDevice();
	void ClearHrPrinter();
	void ClearPrtGeneral();
	void ClearPrtMarkerColorant();
	void ClearPrtMediaPath();

	//其它
	ENUM_STRUCT_TYPE(HrDeviceTypes) GetDeviceTypes(CString szOid);
	CString GetSysDescr();
	void InitOID();

protected:
	//保存通过SNMP获取OID信息的MAP、Group
	PSystemGroup m_pSystemGroup;
	PInterfacesGroup m_pInterfacesGroup;
	PHrSystemGroup m_pHrSystemGroup;
	HrDeviceEntryMap m_oHrDeviceEntryMap;
	HrPrinterEntryMap m_oHrPrinterEntryMap;
	PrtGeneralEntryMap m_oPrtGeneralEntryMap;
	PrtMarkerColorantEntryMap m_oPrtMarkerColorantEntryMap;
	PrtMediaPathEntryMap m_oPrtMediaPathEntryMap;

	//通过SNMP获取OID信息的MAP、Group的线程锁
	CCriticalSection2 m_cs4SystemGroup;
	CCriticalSection2 m_cs4InterfacesGroup;
	CCriticalSection2 m_cs4HrSystemGroup;
	CCriticalSection2 m_cs4HrDeviceEntryMap;
	CCriticalSection2 m_cs4HrPrinterEntryMap;
	CCriticalSection2 m_cs4PrtGeneralEntryMap;
	CCriticalSection2 m_cs4PrtMarkerColorantMap;
	CCriticalSection2 m_cs4PrtMediaPathMap;

	//
	CIniFile m_oIniBrand;

protected:
	//prtMarker
	CString m_szPrtMarkerLifeCountOID;
	CString m_szPrtMarkerLifeCountExOID;

	//prtGeneral
	CString m_szPrtGeneralEntryOID;
	CString m_szPrtGeneralPrinterNameOID;
	CString m_szPrtGeneralSerialNumberOID;

	//prtMarkerColorant
	CString m_szPrtMarkerColorantValueOID;

	//prtMediaPath
	CString m_szPrtMediaPathTypeOID;

	//PrinterModel
	CString m_szStandardModelOID;
	CString m_szCanonModelOID;
	CString m_szLexmarkModelOID;
	CString m_szJingTuModelOID;
	CString m_szKonicaMinoltaModelOID;
	CString m_szAuroraModelOID;
	CString m_szEpsonModelOID;
	CString m_szSamsungModelOID;
	CString m_szPantumModelOID;

	//PrinterSerialNumber
	CString m_szCanonSerialNumberOID;
	CString m_szRicohSerialNumberOID;
	CString m_szOkiSerialNumberOID;
	CString m_szKyoceraSerialNumberOID;
	CString m_szEpsonSerialNumberOID;
	CString m_szEpsonSerialNumber2OID;
	CString m_szPantumSerialNumberOID;

	//system
	CString m_szSystemGroupOID;

	//hrSystem
	CString m_szHrSystemGroupOID;

	//interfaces
	CString m_szInterfacesGroupOID;

	//interfaces.ifTable.ifEntry
	CString m_szIfIndexOID;
	CString m_szIfDescrOID;
	CString m_szIfTypeOID;
	CString m_szIfMtuOID;
	CString m_szIfSpeedOID;
	CString m_szIfPhysAddressOID;
	CString m_szIfAdminStatusOID;
	CString m_szIfOperStatusOID;
	CString m_szIfLastChangeOID;
	CString m_szIfInOctetsOID;
	CString m_szIfInUcastPktsOID;
	CString m_szIfInNUcastPktsOID;
	CString m_szIfInDiscardsOID;
	CString m_szIfInErrorsOID;
	CString m_szIfInUnknownProtosOID;
	CString m_szIfOutOctetsOID;
	CString m_szIfOutUcastPktsOID;
	CString m_szIfOutNUcastPktsOID;
	CString m_szIfOutDiscardsOID;
	CString m_szIfOutErrorsOID;
	CString m_szIfOutQLenOID;
	CString m_szIfSpecificOID;

	//host.hrDevice.hrDeviceTable.hrDeviceEntry
	CString m_szHrDeviceIndexOID;
	CString m_szHrDeviceTypeOID;
	CString m_szHrDeviceDescrOID;
	CString m_szHrDeviceIDOID;
	CString m_szHrDeviceStatusOID;
	CString m_szHrDeviceErrorsOID;
	CString m_szHrDeviceTypePrefixOID;

	//host.hrDevice.hrPrinterTable.hrPrinterEntry
	CString m_szHrPrinterStatusOID;
	CString m_szHrPrinterDetectedErrorStateOID;
};
