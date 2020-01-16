#include "StdAfx.h"
#include "SnmpMonitorHelperPT.h"
#include "PantumMIB_Def.h"
#include "PrinterInfoMIB_Def.h"
#include <algorithm>


// add by zmm , 2019.1.10 {
#define ptPrinterCurStateOID    _T(".1.3.6.1.4.1.40093.1.1.3.9") // 打印机当前状态:"0：初始化, 1：休眠, 2：预热, 3：待机, 4：打印中, 5：错误, 6：取消作业中, 7：处理中
#define ptPrintAlertOID         _T(".1.3.6.1.4.1.40093.1.1.3.14.1") // 打印警告: 0：粉量正常, 1：粉量低
#define ptCoverStatusOID        _T(".1.3.6.1.4.1.40093.1.1.3.15.2") // 盖板警告: 0：前后盖都关闭, 1：前盖打开, 2：后盖打开
#define ptFatalErrorOID         _T(".1.3.6.1.4.1.40093.1.1.3.15.1") // 当前打印机发生了fatalerror错误, 0~255
#define ptTonerAlertOID         _T(".1.3.6.1.4.1.40093.1.1.3.15.3") // 粉盒预警: 0：粉盒状态正常, 1：未安装粉盒, 2：粉盒不匹配, 3：粉盒用尽
#define ptPaperBoxAlertOID      _T(".1.3.6.1.4.1.40093.1.1.3.15.4") // 缺纸错误: 0：打印过程中纸盒有纸, 1：自动进纸盒 缺纸/进纸失败, 2：手动进纸盒 缺纸/进纸失败
#define ptPaperAlertOID         _T(".1.3.6.1.4.1.40093.1.1.3.15.5") // 卡纸错误: 0：无卡纸错误, 1：进纸口卡纸, 2：中间卡纸, 3：出口处卡纸, 4：双面单元卡纸
#define ptPaperSourceAlertOID   _T(".1.3.6.1.4.1.40093.1.1.3.15.7") // 纸张来源预警: 0：无纸张来源错误, 1：纸张来源与实际进纸不匹配错误, 
#define ptCurErrorInfoOID       _T(".1.3.6.1.4.1.40093.1.1.3.15.8") // 当前错误详细信息: 0xFFFFFFFF：无错误情况发生, 0xFFFFFFFE：致命错误, 0xFFFFFFFD：开盖错误, 0xFFFFFFB：粉盒状态, 0xFFFFFFF7：缺纸错误, 0xFFFFFFEF：卡纸错误, 0xFFFFFFDF：纸型不匹配, 0xFFFFFFBF：纸张来源不匹配
#define ptJobAlertOID           _T(".1.3.6.1.4.1.40093.1.1.4.11") // 面板取消标志: 0:没有从面板取消作业, 1:从面板取消作业
// }

CSnmpMonitorHelperPT::CSnmpMonitorHelperPT(void)
{
	InitOID();
}

void CSnmpMonitorHelperPT::InitOID()
{
	m_szPantumTonerLevelPercentOID = DecryptOID(PantumTonerLevelPercentOID);
	m_szPantumCoverStatusOID = DecryptOID(PantumCoverStausOID);
	m_szPantumPrinterStatus = DecryptOID(PantumPrinterStatus);
	m_szPantumTrayPaperErrorOID = DecryptOID(PantumTrayPaperErrorOID);
	m_szPantumAlertBaseOID = DecryptOID(PantumAlertBaseOID);
	m_szPantumTonerStatus = DecryptOID(PantumTonerStatus);
	m_szPantumDrumStatusOID = DecryptOID(PantumDrumStatusOID);
	m_szPantumFatalErrorOID = DecryptOID(PantumFatalErrorOID);
}


BOOL CSnmpMonitorHelperPT::CheckPrinterTable()
{
    if (CheckDynamicInfo())
        return TRUE;


    int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;

    bool bNeedCheckBase = false;    
    CString szDesc = "";
    CString szPrinterDesc = "";
    HrPrinterEntryMap::iterator it;
    for (it=m_oPrinterMap.begin(); it!=m_oPrinterMap.end(); it++)
    {
        PHrPrinterEntry pEntry = it->second;
        if (NULL == pEntry)
            continue;

        if (m_oDeviceEntryOfPrinter.hrDeviceStatus == ENUM_STRUCT_VALUE(HrDeviceStatus)::warning 
            || m_oDeviceEntryOfPrinter.hrDeviceStatus == ENUM_STRUCT_VALUE(HrDeviceStatus)::down
            || m_oDeviceEntryOfPrinter.hrDeviceStatus == ENUM_STRUCT_VALUE(HrDeviceStatus)::unknown
            )
        {

            switch (pEntry->hrPrinterDetectedErrorState)
            {
            case 0x08:
                szPrinterDesc = _T("开盖错误");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::CoverOpen;
                break;
            case 0x10:
                szPrinterDesc = _T("粉盒错误");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerEmpty;
                break;
            default:
                bNeedCheckBase = true;
            }
        }
    }

    if (bNeedCheckBase)
        return CSnmpMonitorHelper::CheckPrinterTable();
    else
    {
        if (nAlertCode < 100)
            nAlertCode += 100;	//参考:【PrinterAlert_Def.h】中的【PrinterAlertCode】信安宝扩展多组预警

        m_pCallBack->OnStatus(nAlertCode, CStringToChar(szPrinterDesc));

        return TRUE;
    }
}

CSnmpMonitorHelperPT::~CSnmpMonitorHelperPT(void)
{
}

BOOL CSnmpMonitorHelperPT::GetMarkerSupplies()
{
    CSnmpMonitorHelper::GetMarkerSupplies();

    bool bHasToner = false; // 是否有粉盒
    bool bHasOPC = false; // 是否有感光鼓

    // 查找是否已经读到感光鼓和墨盒
    PrtMarkerSuppliesEntryMap::iterator it;
    for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
    {
        PPrtMarkerSuppliesEntry pEntry = it->second;
        if (pEntry)
        {
            int nIndex = pEntry->prtMarkerSuppliesIndex;

            //如果是墨粉，则从prtMarkerColorant中获取颜色信息
            if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
                || pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
            {
                bHasToner = true;
            }
            else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
            {
                bHasOPC = true;
            }
        }
    }

    if (!bHasOPC)
        getOpcInfo();

    if (!bHasToner)
        getToneInfo();

    return TRUE;
}

void CSnmpMonitorHelperPT::getOpcInfo()
{
    // 1.3.6.1.4.1.40093.1.1.3.13.1, 描述硒鼓寿命
    // 1.3.6.1.4.1.40093.1.1.3.11 硒鼓已打印页数
    int nVal = 0;
    if (!GetRequest(".1.3.6.1.4.1.40093.1.1.3.13.1", nVal))
    {
        theLog.Write(_T("!!!CSnmpMonitorHelperPT::getOpcInfo 没有读到硒鼓寿命"));
        return ;
    }
    const int nOpeLife = nVal;

    if (!GetRequest(".1.3.6.1.4.1.40093.1.1.3.11", nVal))
    {
        theLog.Write(_T("!!!CSnmpMonitorHelperPT::getOpcInfo 没有读到硒鼓已打印页数"));
        return ;
    }
    const int nOpeLevel = nVal;


    const int nIndex = m_oMarkerSuppliesMap.size();
    PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
    memset(pEntry, 0, sizeof(PrtMarkerSuppliesEntry));
    m_oMarkerSuppliesMap[nIndex] = pEntry;

    pEntry->prtMarkerSuppliesIndex = nIndex;
    pEntry->prtMarkerSuppliesLevel = nOpeLevel;
    pEntry->prtMarkerSuppliesMaxCapacity = nOpeLife;
    pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC;
    strncpy((char*)pEntry->prtMarkerSuppliesDescription, CCommonFun::MultiByteToUTF8("硒鼓"), sizeof(pEntry->prtMarkerSuppliesDescription));
}

void CSnmpMonitorHelperPT::getToneInfo()
{
    // 1.3.6.1.4.1.40093.1.1.3.13.2, 碳粉剩余量，单位为%
    int nVal = 0;
    if (!GetRequest(".1.3.6.1.4.1.40093.1.1.3.13.2", nVal))
    {
        theLog.Write(_T("!!!CSnmpMonitorHelperPT::getToneInfo 没有读到碳粉剩余量"));
        return ;
    }
    
    const int nIndex = m_oMarkerSuppliesMap.size();
    PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
    memset(pEntry, 0, sizeof(PrtMarkerSuppliesEntry));
    m_oMarkerSuppliesMap[nIndex] = pEntry;

    pEntry->prtMarkerSuppliesIndex = nIndex;
    pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;
    strncpy((char*)pEntry->prtMarkerSuppliesDescription, CCommonFun::MultiByteToUTF8("碳粉"), sizeof(pEntry->prtMarkerSuppliesDescription));
    pEntry->prtMarkerSuppliesSupplyUnit = ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent;
    pEntry->prtMarkerSuppliesLevel = nVal;
}

BOOL CSnmpMonitorHelperPT::GetAlert()
{
    CString sSeries = (char*)m_oDeviceEntryOfPrinter.hrDeviceDescr;
    sSeries = sSeries.MakeUpper();
    if (sSeries.Find(_T("M6500")) >= 0
        || sSeries.Find(_T("M6200NW")) >= 0
        )
    {// 有些低版本的奔图打印机, 甚至没有企业的私有节点
        return CSnmpMonitorHelper::GetAlert();
    }

    CoverStatus();
    TrayPaperError();
    FatalError();
    TonerStatus();
    DrumStatus();
    return TRUE;
}

BOOL CSnmpMonitorHelperPT::CheckDynamicInfo()
{
    std::string oHexInfo;
    if (!GetRequest(PantumDynamicRequstOID, oHexInfo))
    {
        theLog.Write("CSnmpMonitorHelperPT::GetAlert, 动态查询失败, 改为查询其他节点");
        return FALSE;        
    }

    CString sAlertDesc = "";

    const int nLen = oHexInfo.size();
    if (nLen < 60)
    {
        theLog.Write(_T("!!!!CSnmpMonitorHelperPT::DynamicInfo 长度错误: %d"), nLen);
        return FALSE;
    }

    unsigned char chTmp;
    unsigned char* pszInfo = (unsigned char*)oHexInfo.c_str();
    int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;

    chTmp = pszInfo[0]; // 打印机状态
    switch (chTmp)
    {
    case 0:
        sAlertDesc = _T("初始化");
        break;
    case 1:
        sAlertDesc = _T("休眠");
        break;
    case 2:
        sAlertDesc = _T("预热");
        break;
    case 3:
        sAlertDesc = _T("待机");
        break;
    case 4:
        sAlertDesc = _T("打印中");
        break;
    case 5:
        {
            // 读取 iErrorFlag  ,小端在前
            chTmp = pszInfo[36];
            switch (chTmp)
            {
            case 0xff:
                sAlertDesc = _T("");
                break;
            case 0xfe:
                sAlertDesc.Format(_T("致命错误, %d"), (int)pszInfo[40]);
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;
                break;
            case 0xfd:
                sAlertDesc = _T("开盖错误");
                chTmp = pszInfo[41];
                if (1 == chTmp)
                    sAlertDesc += _T(", 前盖打开");
                else if (2 == chTmp)
                    sAlertDesc += _T(", 后盖打开");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::CoverOpen;
                break;
            case 0xfb:
                sAlertDesc = _T("粉盒错误");

                chTmp = pszInfo[42];
                if (0x01 == chTmp)
                    sAlertDesc += _T(", 未安装粉盒");
                else if (0x02 == chTmp)
                    sAlertDesc += _T(", 粉盒不匹配");
                else if (0x03 == chTmp)
                    sAlertDesc += _T(", 粉盒用尽");

                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerEmpty;
                break;
            case 0xf7:
                sAlertDesc = _T("缺纸错误");
                chTmp = pszInfo[43];
                if (0x01 == chTmp)
                    sAlertDesc += _T(", 自动进纸盒 缺纸");
                else if (0x02 == chTmp)
                    sAlertDesc += _T(", 手动进纸盒 缺纸/进纸失败");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperEmpty;
                break;
            case 0xef:
                sAlertDesc = _T("卡纸错误");
                chTmp = pszInfo[44];
                if (0x01 == chTmp)
                    sAlertDesc += _T(", 自动进纸盒 进纸失败");
                else if (0x02 == chTmp)
                    sAlertDesc += _T(", 进纸口卡纸");
                else if (0x03 == chTmp)
                    sAlertDesc += _T(", 中间卡纸");
                else if (0x04 == chTmp)
                    sAlertDesc += _T(", 出口处卡纸");
                else if (0x05 == chTmp)
                    sAlertDesc += _T(", 双面单元卡纸");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Jam;
                break;
            case 0xdf:
                sAlertDesc = _T("纸型不匹配错误");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;
                break;
            case 0xbf:
                sAlertDesc = _T("纸张来源不匹配错误");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;
                break;
            default:
                sAlertDesc.Format(_T("未知错误, %d"), chTmp);
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::UnKnown;
            }
        }
        break;        
    case 0x86:
        sAlertDesc = _T("取消作业中");
        break;
    default:
        sAlertDesc.Format(_T("未知状态: %d"), chTmp);
        break;
    }

    if (nAlertCode < 100)
        nAlertCode += 100;	//参考:【PrinterAlert_Def.h】中的【PrinterAlertCode】信安宝扩展多组预警
    m_pCallBack->OnStatus(nAlertCode, CStringToChar(sAlertDesc));
    return TRUE;
}

void CSnmpMonitorHelperPT::appendPrtAlert(CString sAlertDesc, ENUM_STRUCT_TYPE(PrtAlertGroupTC) eAlertType, ENUM_STRUCT_TYPE(PrtAlertCodeTC) eAlertCode)
{
    if (!sAlertDesc.IsEmpty())
    {
        PPrtAlertEntry pEntry = new PrtAlertEntry;
        memset(pEntry, 0x0, sizeof(PrtAlertEntry));

        const int nIndex = m_oAlertMap.size();
        m_oAlertMap.insert(pair<int,PPrtAlertEntry>(nIndex, pEntry));
        pEntry->prtAlertIndex = nIndex;

        sprintf((char*)pEntry->prtAlertDescription,"%s",CCommonFun::MultiByteToUTF8(sAlertDesc.GetString()));
        pEntry->prtAlertCode = eAlertCode;

        pEntry->prtAlertGroup = eAlertType;
    }
}

void CSnmpMonitorHelperPT::TonerStatus()
{
	
    ENUM_STRUCT_TYPE(PrtAlertCodeTC) eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other;

	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumTonerStatus);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
	GetRequest(cOidStr, nValue);
	CString szTonerStatus;
	if (nValue == 0)
	{
		szTonerStatus = "";//正常;
	}
	else if (nValue == 1)
	{
		szTonerStatus = _T("未安装粉盒");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerCartridgeMissing;
	}
	else if (nValue == 2)
	{
		szTonerStatus = _T("粉盒不匹配");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerCartridgeMissing;
        
	}
	else if (nValue == 3)
	{
		szTonerStatus = _T("粉盒用尽");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerEmpty;
	}
	else if (nValue == 4)
	{
		szTonerStatus = _T("粉量低");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerAlmostEmpty;
	}

    if (!szTonerStatus.IsEmpty())    
        appendPrtAlert(szTonerStatus,  ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, eAlertCode);
}
void CSnmpMonitorHelperPT::DrumStatus()
{
	//================================3鼓组件状态mpsDrumStatus=====================================================
    ENUM_STRUCT_TYPE(PrtAlertCodeTC) eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other;

	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumDrumStatusOID);
	GetRequest(cOidStr, nValue);
	CString szDrumStatus;
	if (nValue == 0)
	{
		szDrumStatus = "";//正常
	}
	else if (nValue == 1)
	{
		szDrumStatus = "未安装鼓组件";
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeOver;
	}
	else if (nValue == 2)
	{
		szDrumStatus = "鼓组件不匹配";
		eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeOver;
	}
	else if (nValue == 3)
	{
		szDrumStatus = "鼓组件寿命尽";
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeOver;
	}
	else if (nValue == 4)
	{
		szDrumStatus = "鼓组件寿命将尽";
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeAlmostOver;
	}

    if (!szDrumStatus.IsEmpty())
        appendPrtAlert(szDrumStatus,  ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, eAlertCode);
}
void CSnmpMonitorHelperPT::CoverStatus()
{
	//================================4开盖状态mpsCoverStatus=====================================================
	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumCoverStatusOID);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
	GetRequest(cOidStr, nValue);
	CString szCoverStatus;
	if (nValue == 0)
	{
		szCoverStatus = "";//正常
	}
	else if (nValue == 1)
	{
		szCoverStatus = "前盖打开";
	}
	else if (nValue == 2)
	{
		szCoverStatus = "后盖打开";
	}
	else if (nValue == 3)
	{
		CString szPrinterModel = (char*)m_oDeviceEntryOfPrinter.hrDeviceDescr;
		if (szPrinterModel.Find("M7100DN") >= 0 || szPrinterModel.Find("M7200FDW") >= 0)
		{
			szCoverStatus = "请关闭ADF上盖";//ADF开盖
		}
		else
		{
			szCoverStatus = "ADF开盖";
		}
	}
	
    appendPrtAlert(szCoverStatus,  ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert,ENUM_STRUCT_VALUE(PrtAlertCodeTC)::CoverOpen);
}


void CSnmpMonitorHelperPT::TrayPaperError()
{
	//CString szPrinterModel = GetStrByOid(m_szPantumModelOID);
	//if (szPrinterModel.Find("P3500DN") >= 0)
	//{
	//	TrayPaperError_P3500DN();
	//}
	//else
	//{
 //       
	//}

    //================================5纸盒或纸张错误mpsTrayPaperError=====================================================
    char cOidStr[128] = {0};
    int nValue = 0;
    CString szTmp;
    nValue = 0;
    sprintf(cOidStr, "%s", m_szPantumTrayPaperErrorOID);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
    GetRequest(cOidStr, nValue);
    CString szTrayPaperError;
    BYTE byLow = (BYTE)nValue;
    BYTE byHigh = (BYTE)(nValue >> 8);
    CString szStatus;
    CString szTray;
    theLog.Write("SnmpMonitorHelperPT::GetAlert,1,nValue=%x,byLow=%x,byHigh=%x",nValue,byLow,byHigh);
    switch (byLow)
    {
    case 0x00 :
        szStatus = "";//正常
        break;
    case 0x01:
        szStatus = "缺纸";
        break;
    case 0x02:
        szStatus = "进纸失败";
        break;
    case 0x03:
        szStatus = "进纸处卡纸";
        break;
    case 0x04:
        szStatus = "纸张来源不匹配";
        break;
    case 0x05:
        szStatus = "纸张设置不匹配";
        break;
    case 0x06:
        szStatus = "未安装";
        break;
    case 0x07:
        szStatus = "纸盒已打开";
        break;
    case 0x08:
        szStatus = "故障1";
        break;
    case 0x09:
        szStatus = "故障2";
        break;
    case 0x0A:
        szStatus = "故障3";
        break;
    case 0x0B:
        szStatus = "纸型不匹配";
        break;
    case 0x80:
        szStatus = "出纸处卡纸";
        break;
    case 0x81:
        szStatus = "中间卡纸";
        break;
    case 0x82:
        szStatus = "双面单元卡纸";
        break;
    case 0x83:
        szStatus = "自动+手动进纸盒缺纸";
        break;
    case 0x84:
        szStatus = "ADF卡纸";
        break;
    case 0x85:
        szStatus = "自动+手动进纸盒不可用";
        break;
    case 0x86:
        szStatus = "出纸处纸满";
        break;
    case 0x87:
        szStatus = "中间卡纸未排除";
        break;
    case 0x88:
        szStatus = "双面单元卡纸未排除";
        break;
    case 0x89:
        szStatus = "出纸处卡纸未排除";
        break;
    case 0x8A:
        szStatus = "";//自动进纸盒-正常 
        break;
    case 0x8B:
        szStatus = "自动进纸盒-缺纸";
        break;
    case 0x8C:
        szStatus = "自动进纸盒-进纸失败";
        break;
    case 0x8D:
        szStatus = "自动进纸盒-进纸处卡纸";
        break;
    case 0x8E:
        szStatus = "自动进纸盒-纸张来源不匹配";
        break;
    case 0x8F:
        szStatus = "自动进纸盒-纸张设置不匹配";
        break;
    case 0x90:
        szStatus = "自动进纸盒-未安装";
        break;
    case 0x91:
        szStatus = "自动进纸盒-纸盒已打开";
        break;
    case 0x92:
        szStatus = "自动进纸盒-故障1";
        break;
    case 0x93:
        szStatus = "自动进纸盒-故障2";
        break;
    case 0x94:
        szStatus = "自动进纸盒-故障3";
        break;
    case 0x95:
        szStatus = "自动进纸盒-纸型不匹配";
        break;
    case 0x96:
        szStatus = "";//手动进纸盒-正常
        break;
    case 0x97:
        szStatus = "手动进纸盒-缺纸";
        break;
    case 0x98:
        szStatus = "手动进纸盒-进纸失败";
        break;
    case 0x99:
        szStatus = "手动进纸盒-进纸处卡纸";
        break;
    case 0x9A:
        szStatus = "手动进纸盒-纸张来源不匹配";
        break;
    case 0x9B:
        szStatus = "手动进纸盒-手动进纸盒-纸张设置不匹配";
        break;
    case 0x9C:
        szStatus = "手动进纸盒-未安装";
        break;
    case 0x9D:
        szStatus = "手动进纸盒-纸盒已打开";
        break;
    case 0x9E:
        szStatus = "手动进纸盒-故障1";
        break;
    case 0x9F:
        szStatus = "手动进纸盒-故障2";
        break;
    case 0xA0:
        szStatus = "手动进纸盒-故障3";
        break;
    case 0xA1:
        szStatus = "手动进纸盒-纸型不匹配";
        break;
    case 0xA2:
        szStatus = "";//选配纸盒1-正常
        break;
    case 0xA3:
        szStatus = "选配纸盒1-缺纸";
        break;
    case 0xA4:
        szStatus = "选配纸盒1-进纸失败";
        break;
    case 0xA5:
        szStatus = "选配纸盒1-进纸处卡纸";
        break;
    case 0xA6:
        szStatus = "选配纸盒1-纸张来源不匹配";
        break;
    case 0xA7:
        szStatus = "选配纸盒1-纸张设置不匹配";
        break;
    case 0xA8:
        szStatus = "选配纸盒1-未安装";
        break;
    case 0xA9:
        szStatus = "选配纸盒1-纸盒已打开";
        break;
    case 0xAA:
        szStatus = "选配纸盒1-故障1";
        break;
    case 0xAB:
        szStatus = "选配纸盒1-故障2";
        break;
    case 0xAC:
        szStatus = "选配纸盒1-故障3";
        break;
    case 0xAD:
        szStatus = "选配纸盒1-纸型不匹配";
        break;
    case 0xAE:
        szStatus = "";// "选配纸盒2-正常";
        break;
    case 0xAF:
        szStatus = "选配纸盒2-缺纸";
        break;
    case 0xB0:
        szStatus = "选配纸盒2-进纸失败";
        break;
    case 0xB1:
        szStatus = "选配纸盒2-进纸处卡纸";
        break;
    case 0xB2:
        szStatus = "选配纸盒2-纸张来源不匹配";
        break;
    case 0xB3:
        szStatus = "选配纸盒2-纸张设置不匹配";
        break;
    case 0xB4:
        szStatus = "选配纸盒2-未安装";
        break;
    case 0xB5:
        szStatus = "选配纸盒2-纸盒已打开";
        break;
    case 0xB6:
        szStatus = "选配纸盒2-故障1";
        break;
    case 0xB7:
        szStatus = "选配纸盒2-故障2";
        break;
    case 0xB8:
        szStatus = "选配纸盒2-故障3";
        break;
    case 0xB9:
        szStatus = "选配纸盒2-纸型不匹配";
        break;
    case 0xBA:
        szStatus = "";//ADF纸盒-正常
        break;
    case 0xBB:
        szStatus = "ADF纸盒-缺纸";
        break;
    case 0xBC:
        szStatus = "ADF纸盒-进纸失败";
        break;
    case 0xBD:
        szStatus = "ADF纸盒-进纸处卡纸";
        break;
    case 0xBE:
        szStatus = "ADF纸盒-纸张来源不匹配";
        break;
    case 0xBF:
        szStatus = "ADF纸盒-纸张设置不匹配";
        break;
    case 0xC0:
        szStatus = "ADF纸盒-未安装";
        break;
    case 0xC1:
        szStatus = "ADF纸盒-纸盒已打开";
        break;
    case 0xC2:
        szStatus = "ADF纸盒-故障1";
        break;
    case 0xC3:
        szStatus = "ADF纸盒-故障2";
        break;
    case 0xC4:
        szStatus = "ADF纸盒-故障3";
        break;
    case 0xC5:
        szStatus = "ADF纸盒-纸型不匹配";
        break;
    default:
        szStatus = "";
        break;
    }

    //=====================================================
    switch (byHigh)
    {
    case 0x00:
        szTray = "";//不区分纸盒
        break;
    case 0x01:
        szTray = "自动进纸盒";
        break;
    case 0x02:
        szTray = "手动进纸盒";
        break;
    case 0x03:
        szTray = "选配纸盒1";
        break;
    case 0x04:
        szTray = "选配纸盒2";
        break;
    case 0x05:
        szTray = "ADF";
        break;
    default:
        szTray = "";
    }
    
    szTrayPaperError.Format("%s%s",szTray,szStatus); 
    appendPrtAlert(szTrayPaperError, ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other);

}
void CSnmpMonitorHelperPT::FatalError()
{
	//================================6致命错误FatalError=====================================================
	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumFatalErrorOID);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
	GetRequest(cOidStr, nValue);
	CString szFatalError;
	if (nValue != 0)
	{
		szFatalError = "致命错误";
	}
	szFatalError.Format("%s",szFatalError); 
    appendPrtAlert(szFatalError, ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other);
}