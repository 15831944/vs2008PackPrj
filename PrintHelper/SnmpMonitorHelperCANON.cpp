#include "StdAfx.h"
#include "SnmpMonitorHelperCANON.h"
#include "CanonMIB_Def.h"

CSnmpMonitorHelperCANON::CSnmpMonitorHelperCANON(void)
{
	InitOID();
}

CSnmpMonitorHelperCANON::~CSnmpMonitorHelperCANON(void)
{
}

void CSnmpMonitorHelperCANON::InitOID()
{
	m_szCanonFaultInfoEntryOID = DecryptOID(CanonFaultInfoEntryOID);
	m_szCanonFaultInfoDescOID.Format("%s.5.1", m_szCanonFaultInfoEntryOID);
}

void CSnmpMonitorHelperCANON::CheckFaultCode()
{
	char *cOidBegin = CStringToChar(m_szCanonFaultInfoDescOID);	//".iso.org.dod.internet.private.enterprises.1602.1.5.5.3.1.1.5.1"
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			CString szFault(pszValue);
			szFault.Trim();
			if (szFault.GetLength()>0 && m_pCallBack)
			{
				//过滤一些非故障信息
				if (szFault.Find("paper is out") >= 0)
				{
					continue;
				}
				m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, CStringToChar(szFault));
				break;
			}
		}
		else
		{
			break;
		}
	}
}
