#include "StdAfx.h"
#include "SnmpMonitorHelperPT.h"
#include "PantumMIB_Def.h"
#include "PrinterInfoMIB_Def.h"
#include <algorithm>


// add by zmm , 2019.1.10 {
#define ptPrinterCurStateOID    _T(".1.3.6.1.4.1.40093.1.1.3.9") // ��ӡ����ǰ״̬:"0����ʼ��, 1������, 2��Ԥ��, 3������, 4����ӡ��, 5������, 6��ȡ����ҵ��, 7��������
#define ptPrintAlertOID         _T(".1.3.6.1.4.1.40093.1.1.3.14.1") // ��ӡ����: 0����������, 1��������
#define ptCoverStatusOID        _T(".1.3.6.1.4.1.40093.1.1.3.15.2") // �ǰ徯��: 0��ǰ��Ƕ��ر�, 1��ǰ�Ǵ�, 2����Ǵ�
#define ptFatalErrorOID         _T(".1.3.6.1.4.1.40093.1.1.3.15.1") // ��ǰ��ӡ��������fatalerror����, 0~255
#define ptTonerAlertOID         _T(".1.3.6.1.4.1.40093.1.1.3.15.3") // �ۺ�Ԥ��: 0���ۺ�״̬����, 1��δ��װ�ۺ�, 2���ۺв�ƥ��, 3���ۺ��þ�
#define ptPaperBoxAlertOID      _T(".1.3.6.1.4.1.40093.1.1.3.15.4") // ȱֽ����: 0����ӡ������ֽ����ֽ, 1���Զ���ֽ�� ȱֽ/��ֽʧ��, 2���ֶ���ֽ�� ȱֽ/��ֽʧ��
#define ptPaperAlertOID         _T(".1.3.6.1.4.1.40093.1.1.3.15.5") // ��ֽ����: 0���޿�ֽ����, 1����ֽ�ڿ�ֽ, 2���м俨ֽ, 3�����ڴ���ֽ, 4��˫�浥Ԫ��ֽ
#define ptPaperSourceAlertOID   _T(".1.3.6.1.4.1.40093.1.1.3.15.7") // ֽ����ԴԤ��: 0����ֽ����Դ����, 1��ֽ����Դ��ʵ�ʽ�ֽ��ƥ�����, 
#define ptCurErrorInfoOID       _T(".1.3.6.1.4.1.40093.1.1.3.15.8") // ��ǰ������ϸ��Ϣ: 0xFFFFFFFF���޴����������, 0xFFFFFFFE����������, 0xFFFFFFFD�����Ǵ���, 0xFFFFFFB���ۺ�״̬, 0xFFFFFFF7��ȱֽ����, 0xFFFFFFEF����ֽ����, 0xFFFFFFDF��ֽ�Ͳ�ƥ��, 0xFFFFFFBF��ֽ����Դ��ƥ��
#define ptJobAlertOID           _T(".1.3.6.1.4.1.40093.1.1.4.11") // ���ȡ����־: 0:û�д����ȡ����ҵ, 1:�����ȡ����ҵ
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
                szPrinterDesc = _T("���Ǵ���");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::CoverOpen;
                break;
            case 0x10:
                szPrinterDesc = _T("�ۺд���");
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
            nAlertCode += 100;	//�ο�:��PrinterAlert_Def.h���еġ�PrinterAlertCode���Ű�����չ����Ԥ��

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

    bool bHasToner = false; // �Ƿ��зۺ�
    bool bHasOPC = false; // �Ƿ��ий��

    // �����Ƿ��Ѿ������й�ĺ�ī��
    PrtMarkerSuppliesEntryMap::iterator it;
    for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
    {
        PPrtMarkerSuppliesEntry pEntry = it->second;
        if (pEntry)
        {
            int nIndex = pEntry->prtMarkerSuppliesIndex;

            //�����ī�ۣ����prtMarkerColorant�л�ȡ��ɫ��Ϣ
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
    // 1.3.6.1.4.1.40093.1.1.3.13.1, ������������
    // 1.3.6.1.4.1.40093.1.1.3.11 �����Ѵ�ӡҳ��
    int nVal = 0;
    if (!GetRequest(".1.3.6.1.4.1.40093.1.1.3.13.1", nVal))
    {
        theLog.Write(_T("!!!CSnmpMonitorHelperPT::getOpcInfo û�ж�����������"));
        return ;
    }
    const int nOpeLife = nVal;

    if (!GetRequest(".1.3.6.1.4.1.40093.1.1.3.11", nVal))
    {
        theLog.Write(_T("!!!CSnmpMonitorHelperPT::getOpcInfo û�ж��������Ѵ�ӡҳ��"));
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
    strncpy((char*)pEntry->prtMarkerSuppliesDescription, CCommonFun::MultiByteToUTF8("����"), sizeof(pEntry->prtMarkerSuppliesDescription));
}

void CSnmpMonitorHelperPT::getToneInfo()
{
    // 1.3.6.1.4.1.40093.1.1.3.13.2, ̼��ʣ��������λΪ%
    int nVal = 0;
    if (!GetRequest(".1.3.6.1.4.1.40093.1.1.3.13.2", nVal))
    {
        theLog.Write(_T("!!!CSnmpMonitorHelperPT::getToneInfo û�ж���̼��ʣ����"));
        return ;
    }
    
    const int nIndex = m_oMarkerSuppliesMap.size();
    PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
    memset(pEntry, 0, sizeof(PrtMarkerSuppliesEntry));
    m_oMarkerSuppliesMap[nIndex] = pEntry;

    pEntry->prtMarkerSuppliesIndex = nIndex;
    pEntry->prtMarkerSuppliesType = ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner;
    strncpy((char*)pEntry->prtMarkerSuppliesDescription, CCommonFun::MultiByteToUTF8("̼��"), sizeof(pEntry->prtMarkerSuppliesDescription));
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
    {// ��Щ�Ͱ汾�ı�ͼ��ӡ��, ����û����ҵ��˽�нڵ�
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
        theLog.Write("CSnmpMonitorHelperPT::GetAlert, ��̬��ѯʧ��, ��Ϊ��ѯ�����ڵ�");
        return FALSE;        
    }

    CString sAlertDesc = "";

    const int nLen = oHexInfo.size();
    if (nLen < 60)
    {
        theLog.Write(_T("!!!!CSnmpMonitorHelperPT::DynamicInfo ���ȴ���: %d"), nLen);
        return FALSE;
    }

    unsigned char chTmp;
    unsigned char* pszInfo = (unsigned char*)oHexInfo.c_str();
    int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;

    chTmp = pszInfo[0]; // ��ӡ��״̬
    switch (chTmp)
    {
    case 0:
        sAlertDesc = _T("��ʼ��");
        break;
    case 1:
        sAlertDesc = _T("����");
        break;
    case 2:
        sAlertDesc = _T("Ԥ��");
        break;
    case 3:
        sAlertDesc = _T("����");
        break;
    case 4:
        sAlertDesc = _T("��ӡ��");
        break;
    case 5:
        {
            // ��ȡ iErrorFlag  ,С����ǰ
            chTmp = pszInfo[36];
            switch (chTmp)
            {
            case 0xff:
                sAlertDesc = _T("");
                break;
            case 0xfe:
                sAlertDesc.Format(_T("��������, %d"), (int)pszInfo[40]);
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;
                break;
            case 0xfd:
                sAlertDesc = _T("���Ǵ���");
                chTmp = pszInfo[41];
                if (1 == chTmp)
                    sAlertDesc += _T(", ǰ�Ǵ�");
                else if (2 == chTmp)
                    sAlertDesc += _T(", ��Ǵ�");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::CoverOpen;
                break;
            case 0xfb:
                sAlertDesc = _T("�ۺд���");

                chTmp = pszInfo[42];
                if (0x01 == chTmp)
                    sAlertDesc += _T(", δ��װ�ۺ�");
                else if (0x02 == chTmp)
                    sAlertDesc += _T(", �ۺв�ƥ��");
                else if (0x03 == chTmp)
                    sAlertDesc += _T(", �ۺ��þ�");

                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerEmpty;
                break;
            case 0xf7:
                sAlertDesc = _T("ȱֽ����");
                chTmp = pszInfo[43];
                if (0x01 == chTmp)
                    sAlertDesc += _T(", �Զ���ֽ�� ȱֽ");
                else if (0x02 == chTmp)
                    sAlertDesc += _T(", �ֶ���ֽ�� ȱֽ/��ֽʧ��");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperEmpty;
                break;
            case 0xef:
                sAlertDesc = _T("��ֽ����");
                chTmp = pszInfo[44];
                if (0x01 == chTmp)
                    sAlertDesc += _T(", �Զ���ֽ�� ��ֽʧ��");
                else if (0x02 == chTmp)
                    sAlertDesc += _T(", ��ֽ�ڿ�ֽ");
                else if (0x03 == chTmp)
                    sAlertDesc += _T(", �м俨ֽ");
                else if (0x04 == chTmp)
                    sAlertDesc += _T(", ���ڴ���ֽ");
                else if (0x05 == chTmp)
                    sAlertDesc += _T(", ˫�浥Ԫ��ֽ");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Jam;
                break;
            case 0xdf:
                sAlertDesc = _T("ֽ�Ͳ�ƥ�����");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;
                break;
            case 0xbf:
                sAlertDesc = _T("ֽ����Դ��ƥ�����");
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::Other;
                break;
            default:
                sAlertDesc.Format(_T("δ֪����, %d"), chTmp);
                nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::UnKnown;
            }
        }
        break;        
    case 0x86:
        sAlertDesc = _T("ȡ����ҵ��");
        break;
    default:
        sAlertDesc.Format(_T("δ֪״̬: %d"), chTmp);
        break;
    }

    if (nAlertCode < 100)
        nAlertCode += 100;	//�ο�:��PrinterAlert_Def.h���еġ�PrinterAlertCode���Ű�����չ����Ԥ��
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
		szTonerStatus = "";//����;
	}
	else if (nValue == 1)
	{
		szTonerStatus = _T("δ��װ�ۺ�");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerCartridgeMissing;
	}
	else if (nValue == 2)
	{
		szTonerStatus = _T("�ۺв�ƥ��");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerCartridgeMissing;
        
	}
	else if (nValue == 3)
	{
		szTonerStatus = _T("�ۺ��þ�");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerEmpty;
	}
	else if (nValue == 4)
	{
		szTonerStatus = _T("������");
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerTonerAlmostEmpty;
	}

    if (!szTonerStatus.IsEmpty())    
        appendPrtAlert(szTonerStatus,  ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, eAlertCode);
}
void CSnmpMonitorHelperPT::DrumStatus()
{
	//================================3�����״̬mpsDrumStatus=====================================================
    ENUM_STRUCT_TYPE(PrtAlertCodeTC) eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other;

	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumDrumStatusOID);
	GetRequest(cOidStr, nValue);
	CString szDrumStatus;
	if (nValue == 0)
	{
		szDrumStatus = "";//����
	}
	else if (nValue == 1)
	{
		szDrumStatus = "δ��װ�����";
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeOver;
	}
	else if (nValue == 2)
	{
		szDrumStatus = "�������ƥ��";
		eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeOver;
	}
	else if (nValue == 3)
	{
		szDrumStatus = "�����������";
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeOver;
	}
	else if (nValue == 4)
	{
		szDrumStatus = "�������������";
        eAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MarkerOpcLifeAlmostOver;
	}

    if (!szDrumStatus.IsEmpty())
        appendPrtAlert(szDrumStatus,  ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, eAlertCode);
}
void CSnmpMonitorHelperPT::CoverStatus()
{
	//================================4����״̬mpsCoverStatus=====================================================
	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumCoverStatusOID);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
	GetRequest(cOidStr, nValue);
	CString szCoverStatus;
	if (nValue == 0)
	{
		szCoverStatus = "";//����
	}
	else if (nValue == 1)
	{
		szCoverStatus = "ǰ�Ǵ�";
	}
	else if (nValue == 2)
	{
		szCoverStatus = "��Ǵ�";
	}
	else if (nValue == 3)
	{
		CString szPrinterModel = (char*)m_oDeviceEntryOfPrinter.hrDeviceDescr;
		if (szPrinterModel.Find("M7100DN") >= 0 || szPrinterModel.Find("M7200FDW") >= 0)
		{
			szCoverStatus = "��ر�ADF�ϸ�";//ADF����
		}
		else
		{
			szCoverStatus = "ADF����";
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

    //================================5ֽ�л�ֽ�Ŵ���mpsTrayPaperError=====================================================
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
        szStatus = "";//����
        break;
    case 0x01:
        szStatus = "ȱֽ";
        break;
    case 0x02:
        szStatus = "��ֽʧ��";
        break;
    case 0x03:
        szStatus = "��ֽ����ֽ";
        break;
    case 0x04:
        szStatus = "ֽ����Դ��ƥ��";
        break;
    case 0x05:
        szStatus = "ֽ�����ò�ƥ��";
        break;
    case 0x06:
        szStatus = "δ��װ";
        break;
    case 0x07:
        szStatus = "ֽ���Ѵ�";
        break;
    case 0x08:
        szStatus = "����1";
        break;
    case 0x09:
        szStatus = "����2";
        break;
    case 0x0A:
        szStatus = "����3";
        break;
    case 0x0B:
        szStatus = "ֽ�Ͳ�ƥ��";
        break;
    case 0x80:
        szStatus = "��ֽ����ֽ";
        break;
    case 0x81:
        szStatus = "�м俨ֽ";
        break;
    case 0x82:
        szStatus = "˫�浥Ԫ��ֽ";
        break;
    case 0x83:
        szStatus = "�Զ�+�ֶ���ֽ��ȱֽ";
        break;
    case 0x84:
        szStatus = "ADF��ֽ";
        break;
    case 0x85:
        szStatus = "�Զ�+�ֶ���ֽ�в�����";
        break;
    case 0x86:
        szStatus = "��ֽ��ֽ��";
        break;
    case 0x87:
        szStatus = "�м俨ֽδ�ų�";
        break;
    case 0x88:
        szStatus = "˫�浥Ԫ��ֽδ�ų�";
        break;
    case 0x89:
        szStatus = "��ֽ����ֽδ�ų�";
        break;
    case 0x8A:
        szStatus = "";//�Զ���ֽ��-���� 
        break;
    case 0x8B:
        szStatus = "�Զ���ֽ��-ȱֽ";
        break;
    case 0x8C:
        szStatus = "�Զ���ֽ��-��ֽʧ��";
        break;
    case 0x8D:
        szStatus = "�Զ���ֽ��-��ֽ����ֽ";
        break;
    case 0x8E:
        szStatus = "�Զ���ֽ��-ֽ����Դ��ƥ��";
        break;
    case 0x8F:
        szStatus = "�Զ���ֽ��-ֽ�����ò�ƥ��";
        break;
    case 0x90:
        szStatus = "�Զ���ֽ��-δ��װ";
        break;
    case 0x91:
        szStatus = "�Զ���ֽ��-ֽ���Ѵ�";
        break;
    case 0x92:
        szStatus = "�Զ���ֽ��-����1";
        break;
    case 0x93:
        szStatus = "�Զ���ֽ��-����2";
        break;
    case 0x94:
        szStatus = "�Զ���ֽ��-����3";
        break;
    case 0x95:
        szStatus = "�Զ���ֽ��-ֽ�Ͳ�ƥ��";
        break;
    case 0x96:
        szStatus = "";//�ֶ���ֽ��-����
        break;
    case 0x97:
        szStatus = "�ֶ���ֽ��-ȱֽ";
        break;
    case 0x98:
        szStatus = "�ֶ���ֽ��-��ֽʧ��";
        break;
    case 0x99:
        szStatus = "�ֶ���ֽ��-��ֽ����ֽ";
        break;
    case 0x9A:
        szStatus = "�ֶ���ֽ��-ֽ����Դ��ƥ��";
        break;
    case 0x9B:
        szStatus = "�ֶ���ֽ��-�ֶ���ֽ��-ֽ�����ò�ƥ��";
        break;
    case 0x9C:
        szStatus = "�ֶ���ֽ��-δ��װ";
        break;
    case 0x9D:
        szStatus = "�ֶ���ֽ��-ֽ���Ѵ�";
        break;
    case 0x9E:
        szStatus = "�ֶ���ֽ��-����1";
        break;
    case 0x9F:
        szStatus = "�ֶ���ֽ��-����2";
        break;
    case 0xA0:
        szStatus = "�ֶ���ֽ��-����3";
        break;
    case 0xA1:
        szStatus = "�ֶ���ֽ��-ֽ�Ͳ�ƥ��";
        break;
    case 0xA2:
        szStatus = "";//ѡ��ֽ��1-����
        break;
    case 0xA3:
        szStatus = "ѡ��ֽ��1-ȱֽ";
        break;
    case 0xA4:
        szStatus = "ѡ��ֽ��1-��ֽʧ��";
        break;
    case 0xA5:
        szStatus = "ѡ��ֽ��1-��ֽ����ֽ";
        break;
    case 0xA6:
        szStatus = "ѡ��ֽ��1-ֽ����Դ��ƥ��";
        break;
    case 0xA7:
        szStatus = "ѡ��ֽ��1-ֽ�����ò�ƥ��";
        break;
    case 0xA8:
        szStatus = "ѡ��ֽ��1-δ��װ";
        break;
    case 0xA9:
        szStatus = "ѡ��ֽ��1-ֽ���Ѵ�";
        break;
    case 0xAA:
        szStatus = "ѡ��ֽ��1-����1";
        break;
    case 0xAB:
        szStatus = "ѡ��ֽ��1-����2";
        break;
    case 0xAC:
        szStatus = "ѡ��ֽ��1-����3";
        break;
    case 0xAD:
        szStatus = "ѡ��ֽ��1-ֽ�Ͳ�ƥ��";
        break;
    case 0xAE:
        szStatus = "";// "ѡ��ֽ��2-����";
        break;
    case 0xAF:
        szStatus = "ѡ��ֽ��2-ȱֽ";
        break;
    case 0xB0:
        szStatus = "ѡ��ֽ��2-��ֽʧ��";
        break;
    case 0xB1:
        szStatus = "ѡ��ֽ��2-��ֽ����ֽ";
        break;
    case 0xB2:
        szStatus = "ѡ��ֽ��2-ֽ����Դ��ƥ��";
        break;
    case 0xB3:
        szStatus = "ѡ��ֽ��2-ֽ�����ò�ƥ��";
        break;
    case 0xB4:
        szStatus = "ѡ��ֽ��2-δ��װ";
        break;
    case 0xB5:
        szStatus = "ѡ��ֽ��2-ֽ���Ѵ�";
        break;
    case 0xB6:
        szStatus = "ѡ��ֽ��2-����1";
        break;
    case 0xB7:
        szStatus = "ѡ��ֽ��2-����2";
        break;
    case 0xB8:
        szStatus = "ѡ��ֽ��2-����3";
        break;
    case 0xB9:
        szStatus = "ѡ��ֽ��2-ֽ�Ͳ�ƥ��";
        break;
    case 0xBA:
        szStatus = "";//ADFֽ��-����
        break;
    case 0xBB:
        szStatus = "ADFֽ��-ȱֽ";
        break;
    case 0xBC:
        szStatus = "ADFֽ��-��ֽʧ��";
        break;
    case 0xBD:
        szStatus = "ADFֽ��-��ֽ����ֽ";
        break;
    case 0xBE:
        szStatus = "ADFֽ��-ֽ����Դ��ƥ��";
        break;
    case 0xBF:
        szStatus = "ADFֽ��-ֽ�����ò�ƥ��";
        break;
    case 0xC0:
        szStatus = "ADFֽ��-δ��װ";
        break;
    case 0xC1:
        szStatus = "ADFֽ��-ֽ���Ѵ�";
        break;
    case 0xC2:
        szStatus = "ADFֽ��-����1";
        break;
    case 0xC3:
        szStatus = "ADFֽ��-����2";
        break;
    case 0xC4:
        szStatus = "ADFֽ��-����3";
        break;
    case 0xC5:
        szStatus = "ADFֽ��-ֽ�Ͳ�ƥ��";
        break;
    default:
        szStatus = "";
        break;
    }

    //=====================================================
    switch (byHigh)
    {
    case 0x00:
        szTray = "";//������ֽ��
        break;
    case 0x01:
        szTray = "�Զ���ֽ��";
        break;
    case 0x02:
        szTray = "�ֶ���ֽ��";
        break;
    case 0x03:
        szTray = "ѡ��ֽ��1";
        break;
    case 0x04:
        szTray = "ѡ��ֽ��2";
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
	//================================6��������FatalError=====================================================
	char cOidStr[128] = {0};
	int nValue = 0;
	sprintf(cOidStr, "%s", m_szPantumFatalErrorOID);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
	GetRequest(cOidStr, nValue);
	CString szFatalError;
	if (nValue != 0)
	{
		szFatalError = "��������";
	}
	szFatalError.Format("%s",szFatalError); 
    appendPrtAlert(szFatalError, ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Alert, ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other);
}