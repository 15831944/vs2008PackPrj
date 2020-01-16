#include "StdAfx.h"
#include "SnmpMonitorHelper_SEIKO.h"
#include "SeikoMIB_Def.h"

CSnmpMonitorHelper_SEIKO::CSnmpMonitorHelper_SEIKO(void)
{
	InitOID();
}

CSnmpMonitorHelper_SEIKO::~CSnmpMonitorHelper_SEIKO(void)
{
}

void CSnmpMonitorHelper_SEIKO::InitOID()
{
	m_szPrtInputNameOID = DecryptOID(SeikoPrtInputNameOID);
	m_szPrtInputCurrentLevelOID = DecryptOID(SeikoPrtInputCurrentLevelOID);
	m_szPrtInputMediaTypeOID = DecryptOID(SeikoPrtInputMediaTypeOID);
}
