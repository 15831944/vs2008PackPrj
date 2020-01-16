#include "stdafx.h"
#include "SnmpInterface.h"
#include "comm.h"

CSnmpOP::CSnmpOP()
{
	m_bIsActiveIP = FALSE;
	m_bIsSupportSnmp = FALSE;
	m_pCallBack = NULL;
	m_lpMgrSession = NULL;
	m_szIP.Empty();
	m_szCommunity = "public";
}

CSnmpOP::~CSnmpOP()
{
	ReleaseSnmp();
}

BOOL CSnmpOP::InitSnmp()
{
	theLog.Write("CSnmpOP::InitSnmp,1,m_szIP=%s, m_szCommunity=%s, m_lpMgrSession=%p", m_szIP, m_szCommunity, m_lpMgrSession);
	if (m_lpMgrSession)
	{
		return TRUE;
	}

	if (!CheckIPActive())
	{
		theLog.Write("!!CSnmpOP::InitSnmp,2,CheckIPActive fail,m_szIP=%s", m_szIP);
		//return FALSE;
	}

	m_lpMgrSession = SnmpMgrOpen(CStringToChar(m_szIP), CStringToChar(m_szCommunity), 1000, 3);
	if(m_lpMgrSession == NULL)
	{
		theLog.Write("!!CSnmpOP::InitSnmp,3,SnmpMgrOpen err=%d", ::GetLastError());
		return FALSE;
	}
	else
	{
		if (CheckSupportSnmp())
		{
			theLog.Write("CSnmpOP::InitSnmp,4,succ,m_lpMgrSession=%p", m_lpMgrSession);
			return TRUE;
		}
		else
		{
			theLog.Write("!!CSnmpOP::InitSnmp,5,fail,m_szIP=%s,m_bIsActiveIP=%d,m_bIsSupportSnmp=%d"
				, m_szIP, m_bIsActiveIP, m_bIsSupportSnmp);
			return FALSE;
		}
	}
}

void CSnmpOP::SetConnect(CString szIP, CString szCommunity /*= "public"*/)
{
	m_szIP = szIP;
	m_szCommunity = szCommunity;
	theLog.Write("CSnmpOP::SetConnect,m_szIP=%s, m_szCommunity=%s", m_szIP, m_szCommunity);
}

void CSnmpOP::SetCallBack(ISCPStatusInterface* pCallBack)
{
	m_pCallBack = pCallBack;
	theLog.Write("CSnmpOP::SetCallBack,m_pCallBack=%p", m_pCallBack);
}

CString CSnmpOP::GetPrtIP()
{
	return m_szIP;
}

void CSnmpOP::ReleaseSnmp()
{
	m_bIsActiveIP = FALSE;
	m_bIsSupportSnmp = FALSE;
	m_pCallBack = NULL;
	m_szIP.Empty();
	m_szCommunity = "public";
	if(m_lpMgrSession)
	{
		SnmpMgrClose(m_lpMgrSession);
		m_lpMgrSession = NULL;
	}
}
void CSnmpOP::Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )  
{  
	int  i;  
	char szTmp[3];  

	for( i = 0; i < nSrcLen; i++ )  
	{  
		sprintf( szTmp, "%02X", (unsigned char) sSrc[i] );  
		memcpy( &sDest[i * 2], szTmp, 2 );  
	}  
	return ;  
} 
char * CSnmpOP::SNMP_AnyToHexStr(AsnObjectSyntax *sAny, UINT& uLen)
{
	if (!sAny)
	{
		return NULL;
	}

	DWORD dwValue = 0;
	//UINT uLen = 0;
	BYTE *puData = 0;
	char *pString = NULL;

	//theLog.Write("CSnmpOP::SNMP_AnyToHexStr,type=%d",sAny->asnType);
	switch ( sAny->asnType )
	{
	case ASN_INTEGER:   
		pString = (char *) SnmpUtilMemAlloc(33);
		if(pString)
			ltoa(sAny->asnValue.number, pString, 10 );
		uLen = strlen(pString);
		break;
	case ASN_RFC1155_COUNTER:
		dwValue = sAny->asnValue.counter;
		pString = (char *) SnmpUtilMemAlloc(33);
		if(pString)
			ultoa(dwValue, pString, 10);
		uLen = strlen(pString);
		break;
	case ASN_RFC1155_GAUGE:
		dwValue = sAny->asnValue.gauge;
		pString = (char *) SnmpUtilMemAlloc(33);
		if (pString)
			ultoa(dwValue, pString, 10);
		uLen = strlen(pString);
		break;
	case ASN_RFC1155_TIMETICKS:
		dwValue = sAny->asnValue.ticks;
		pString = (char *) SnmpUtilMemAlloc(33);
		if (pString)
			ultoa(dwValue, pString, 10);
		uLen = strlen(pString);
		break;
	case ASN_OCTETSTRING:   /* Same as ASN_RFC1213_DISPSTRING */
		uLen = sAny->asnValue.string.length*8+1;
		puData = sAny->asnValue.string.stream;
		pString = (char *) SnmpUtilMemAlloc(uLen + 1);
		if(pString)
		{
			if (sAny->asnValue.arbitrary.length)
			{
				char test[200] = {0};
				Hex2Str((const char *)puData,test,sizeof(puData));
				//theLog.Write("CSnmpOP::SNMP_AnyToHexStr,1,test=%s",test);
				memcpy(pString, test, uLen-1);
				//theLog.Write("CSnmpOP::SNMP_AnyToHexStr,2,pString=%s",pString);
				//strncpy(pString, (const char*)puData, uLen -1);
				pString[uLen] = '\0';
			}
		}
		break;
	case ASN_SEQUENCE:      /* Same as ASN_SEQUENCEOF */
		uLen = sAny->asnValue.sequence.length;
		puData = sAny->asnValue.sequence.stream;
		theLog.Write("CSnmpOP::SNMP_AnyToStr,6==");
		if(pString)
		{
			if (sAny->asnValue.arbitrary.length)
			{
				memcpy(pString, puData, uLen-1);
				//strncpy(pString, (const char*)puData, uLen -1);
				pString[uLen] = '\0';
			}
		}
		break;
	case ASN_RFC1155_IPADDRESS:
		theLog.Write("CSnmpOP::SNMP_AnyToStr,7==");
		if (sAny->asnValue.address.length )
		{
			UINT i;
			char szBuf[17];

			uLen = sAny->asnValue.address.length;
			puData = sAny->asnValue.address.stream;

			pString = (char *) SnmpUtilMemAlloc( uLen * 4 );
			if(pString)
			{
				pString[0] = '\0';

				for (i = 0; i < uLen; i++)
				{
					lstrcat( pString, itoa( puData[i], szBuf, 10 ) );    
					if( i < uLen-1 )
						lstrcat( pString, "." );
				}
				uLen = strlen(pString) + 1;
			}
		}
		else
			pString = NULL;
		break;
	case ASN_RFC1155_OPAQUE:
		theLog.Write("CSnmpOP::SNMP_AnyToStr,8==");
		if( sAny->asnValue.arbitrary.length )
		{
			uLen = sAny->asnValue.arbitrary.length;
			puData = sAny->asnValue.arbitrary.stream;
			pString = (char *) SnmpUtilMemAlloc(uLen + 1);
			if (pString)
			{
				if (sAny->asnValue.arbitrary.length)
				{
					memcpy(pString, puData, uLen-1);
					//strncpy(pString, (const char*)puData, uLen -1);
					pString[uLen] = '\0';
				}
			}
		}
		else
			pString = NULL;
		break;
	case ASN_OBJECTIDENTIFIER:
		if( sAny->asnValue.object.idLength )
		{
			pString = (char *) SnmpUtilMemAlloc( sAny->asnValue.object.idLength * 5 );
			if(pString)
			{
				UINT i;
				char szBuf[17];
				for( i = 0; i < sAny->asnValue.object.idLength; i++ )
				{
					lstrcat( pString, "." );
					lstrcat( pString, itoa( sAny->asnValue.object.ids[i], szBuf, 10 ) );    
				}
				uLen = strlen(pString) + 1;
			}
		}
		else
			pString = NULL;
		break;
	default:             /* Unrecognised data type */
		return( FALSE );
	}
	return(pString);
}

char * CSnmpOP::SNMP_AnyToStr(AsnObjectSyntax *sAny, UINT& uLen)
{
	if (!sAny)
	{
		return NULL;
	}

	DWORD dwValue = 0;
	//UINT uLen = 0;
	BYTE *puData = 0;
	char *pString = NULL;

	switch ( sAny->asnType )
	{
	case ASN_INTEGER:    
		pString = (char *) SnmpUtilMemAlloc(33);
		if(pString)
			ltoa(sAny->asnValue.number, pString, 10 );
		uLen = strlen(pString);
		break;
	case ASN_RFC1155_COUNTER:
		dwValue = sAny->asnValue.counter;
		pString = (char *) SnmpUtilMemAlloc(33);
		if(pString)
			ultoa(dwValue, pString, 10);
		uLen = strlen(pString);
		break;
	case ASN_RFC1155_GAUGE:
		dwValue = sAny->asnValue.gauge;
		pString = (char *) SnmpUtilMemAlloc(33);
		if (pString)
			ultoa(dwValue, pString, 10);
		uLen = strlen(pString);
		break;
	case ASN_RFC1155_TIMETICKS:
		dwValue = sAny->asnValue.ticks;
		pString = (char *) SnmpUtilMemAlloc(33);
		if (pString)
			ultoa(dwValue, pString, 10);
		uLen = strlen(pString);
		break;
	case ASN_OCTETSTRING:   /* Same as ASN_RFC1213_DISPSTRING */
		uLen = sAny->asnValue.string.length+1;
		puData = sAny->asnValue.string.stream;
		pString = (char *) SnmpUtilMemAlloc(uLen + 1);
		if(pString)
		{
			if (sAny->asnValue.arbitrary.length)
			{
				memcpy(pString, puData, uLen-1);
				//strncpy(pString, (const char*)puData, uLen -1);
				pString[uLen] = '\0';
			}
		}
		break;
	case ASN_SEQUENCE:      /* Same as ASN_SEQUENCEOF */
		uLen = sAny->asnValue.sequence.length;
		puData = sAny->asnValue.sequence.stream;
		if(pString)
		{
			if (sAny->asnValue.arbitrary.length)
			{
				memcpy(pString, puData, uLen-1);
				//strncpy(pString, (const char*)puData, uLen -1);
				pString[uLen] = '\0';
			}
		}
		break;
	case ASN_RFC1155_IPADDRESS:
		if (sAny->asnValue.address.length )
		{
			UINT i;
			char szBuf[17];

			uLen = sAny->asnValue.address.length;
			puData = sAny->asnValue.address.stream;

			pString = (char *) SnmpUtilMemAlloc( uLen * 4 );
			if(pString)
			{
				pString[0] = '\0';

				for (i = 0; i < uLen; i++)
				{
					lstrcat( pString, itoa( puData[i], szBuf, 10 ) );    
					if( i < uLen-1 )
						lstrcat( pString, "." );
				}
				uLen = strlen(pString) + 1;
			}
		}
		else
			pString = NULL;
		break;
	case ASN_RFC1155_OPAQUE:
		if( sAny->asnValue.arbitrary.length )
		{
			uLen = sAny->asnValue.arbitrary.length;
			puData = sAny->asnValue.arbitrary.stream;
			pString = (char *) SnmpUtilMemAlloc(uLen + 1);
			if (pString)
			{
				if (sAny->asnValue.arbitrary.length)
				{
					memcpy(pString, puData, uLen-1);
					//strncpy(pString, (const char*)puData, uLen -1);
					pString[uLen] = '\0';
				}
			}
		}
		else
			pString = NULL;
		break;
	case ASN_OBJECTIDENTIFIER:
		if( sAny->asnValue.object.idLength )
		{
			pString = (char *) SnmpUtilMemAlloc( sAny->asnValue.object.idLength * 15 );
			if(pString)
			{
				UINT i;
				char szBuf[17];
				for( i = 0; i < sAny->asnValue.object.idLength; i++ )
				{
					lstrcat( pString, "." );
					lstrcat( pString, itoa( sAny->asnValue.object.ids[i], szBuf, 10 ) );    
				}
				uLen = strlen(pString) + 1;
			}
		}
		else
			pString = NULL;
		break;
	default:             /* Unrecognised data type */
		return( FALSE );
	}
	return(pString);
}

BOOL CSnmpOP::GetRequest(char* pszOid, int& nVal)
{
	if (!m_bIsActiveIP || !pszOid)
	{
		//theLog.Write("!!CSnmpOP::GetRequest,1,u i,m_bIsActiveIP=%d,pszOid=%x", m_bIsActiveIP, pszOid);
		return FALSE;
	}

	BOOL bRet = FALSE;
	char *asciiStr = NULL;
	nVal = 0;	//默认返回0

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetRequest,2,u i,SnmpMgrStrToOid fail,err=%u,pszOid=%s"
			, GetLastError(), pszOid);
		return FALSE;
	}

	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	

	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		// initiates the GET request
		if(!SnmpMgrRequest(m_lpMgrSession,SNMP_PDU_GET,&snmpVarList,&errorStatus,&errorIndex))
		{		
			theLog.Write("!!CSnmpOP::GetRequest,7,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			nVal = atoi(asciiStr);
			bRet = TRUE;
		}
	} while (0);

	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	

	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}

	return bRet;
}

BOOL CSnmpOP::GetRequest(char* pszOid, unsigned int& nVal)
{
	if (!m_bIsActiveIP || !pszOid)
	{
		//theLog.Write("!!CSnmpOP::GetRequest,1,c ui,m_bIsActiveIP=%d,pszOid=%x", m_bIsActiveIP, pszOid);
		return FALSE;
	}

	BOOL bRet = FALSE;
	char *asciiStr = NULL;
	nVal = 0;	//默认返回0

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetRequest,2,c ui,SnmpMgrStrToOid fail,err=%u,pszOid=%s"
			, GetLastError(), pszOid);
		return FALSE;
	}


	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	

	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		// initiates the GET request
		if(!SnmpMgrRequest(m_lpMgrSession,SNMP_PDU_GET,&snmpVarList,&errorStatus,&errorIndex))
		{	
			theLog.Write("!!CSnmpOP::GetRequest,8,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			nVal = (unsigned int)_atoi64(asciiStr);
			bRet = TRUE;
		}


	} while (0);

	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	

	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}

	return bRet;
}

BOOL CSnmpOP::GetRequest(char* pszOid, unsigned char* pszStr,int nStrBufferLen)
{
	if (!m_bIsActiveIP || !pszStr)
	{
		//theLog.Write("!!CSnmpOP::GetRequest,1,c,uc,m_bIsActiveIP=%d,pszStr=%x", m_bIsActiveIP, pszStr);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetRequestStr,1,begin,szIP=%s,pszOid=%s", m_szDestIP, pszOid);
	BOOL bRet = FALSE;
	char *asciiStr = NULL;

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetRequestStr,2,c uc,end，err=%u,%s",GetLastError(),pszOid);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetRequestStr,3,end");
	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	
	//theLog.Write("CSnmpOP::GetRequestStr,4,end");
	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		if(!m_lpMgrSession)
		{
			theLog.Write("!!CSnmpOP::GetRequestStr,5,c uc,m_lpMgrSession=%p", m_lpMgrSession);
			SnmpUtilOidFree(&asnOid);	//add by zfq,2016-01-28
			SnmpUtilVarBindListFree(&snmpVarList);	//add by zfq,2016-01-28
			return FALSE;
		}
		if(!SnmpMgrRequest(m_lpMgrSession, SNMP_PDU_GET, &snmpVarList, &errorStatus, &errorIndex))
		{			
			theLog.Write("!!CSnmpOP::GetRequest,9,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			if (uLen > nStrBufferLen)
			{
				uLen = nStrBufferLen - 1;	//超出buffer长度，字符串截断，并且传出实际得到的长度
			}
			memcpy(pszStr, asciiStr, uLen);
			//strncpy((char*)pszStr, asciiStr, uLen);
			bRet = TRUE;
		}
	} while (0);
	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	
	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}
	return bRet;
}
BOOL CSnmpOP::GetHexRequest(char* pszOid, unsigned char* pszStr,int nStrBufferLen)
{
	if (!m_bIsActiveIP || !pszStr)
	{
		//theLog.Write("!!CSnmpOP::GetHexRequest,1,c,uc,m_bIsActiveIP=%d,pszStr=%x", m_bIsActiveIP, pszStr);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetHexRequest,1,begin,szIP=%s,pszOid=%s", m_szDestIP, pszOid);
	BOOL bRet = FALSE;
	char *asciiStr = NULL;

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetHexRequest,2,c uc,end，err=%u,%s",GetLastError(),pszOid);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetHexRequest,3,end");
	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	
	//theLog.Write("CSnmpOP::GetHexRequest,4,end");
	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		if(!m_lpMgrSession)
		{
			theLog.Write("!!CSnmpOP::GetHexRequest,5,c uc,m_lpMgrSession=%p", m_lpMgrSession);
			SnmpUtilOidFree(&asnOid);	
			SnmpUtilVarBindListFree(&snmpVarList);
			return FALSE;
		}
		if(!SnmpMgrRequest(m_lpMgrSession, SNMP_PDU_GET, &snmpVarList, &errorStatus, &errorIndex))
		{			
			theLog.Write("!!CSnmpOP::GetHexRequest,9,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToHexStr(&snmpVarList.list[0].value, uLen);
		//theLog.Write("CSnmpOP::GetHexRequest,1,asciiStr=%s,uLen=%d",asciiStr,uLen);
		if (asciiStr)
		{
			if (uLen > nStrBufferLen)
			{
				uLen = nStrBufferLen - 1;	//超出buffer长度，字符串截断，并且传出实际得到的长度
			}
			memcpy(pszStr, asciiStr, uLen);
			//strncpy((char*)pszStr, asciiStr, uLen);
			bRet = TRUE;
		}
	} while (0);
	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	
	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}
	return bRet;
}

BOOL CSnmpOP::GetRequest(char* pszOid, std::string& sOutStr)
{
    sOutStr = "";

    if (!m_bIsActiveIP)
    {
        //theLog.Write("!!CSnmpOP::GetRequest,1,c,uc,m_bIsActiveIP=%d,pszStr=%x", m_bIsActiveIP, pszStr);
        return FALSE;
    }

    //theLog.Write("CSnmpOP::GetRequestStr,1,begin,szIP=%s,pszOid=%s", m_szDestIP, pszOid);
    BOOL bRet = FALSE;
    char *asciiStr = NULL;

    AsnObjectIdentifier asnOid;
    if(!SnmpMgrStrToOid(pszOid, &asnOid))
    {
        theLog.Write("!!CSnmpOP::GetRequestStr,2,c uc,end，err=%u,%s",GetLastError(),pszOid);
        return FALSE;
    }

    //theLog.Write("CSnmpOP::GetRequestStr,3,end");
    AsnInteger	errorStatus=0;	// Error type that is returned if encountered
    AsnInteger	errorIndex=0;		// Works with variable above

    SnmpVarBindList snmpVarList;
    snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
    snmpVarList.len = 1;	
    //theLog.Write("CSnmpOP::GetRequestStr,4,end");
    // Assigning OID to variable bindings list
    SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
    snmpVarList.list[0].value.asnType = ASN_NULL;

    do 
    {
        if(!m_lpMgrSession)
        {
            theLog.Write("!!CSnmpOP::GetRequestStr,5,c uc,m_lpMgrSession=%p", m_lpMgrSession);
            SnmpUtilOidFree(&asnOid);	//add by zfq,2016-01-28
            SnmpUtilVarBindListFree(&snmpVarList);	//add by zfq,2016-01-28
            return FALSE;
        }
        if(!SnmpMgrRequest(m_lpMgrSession, SNMP_PDU_GET, &snmpVarList, &errorStatus, &errorIndex))
        {			
            theLog.Write("!!CSnmpOP::GetRequest,9,end,err=%u", GetLastError());
            break;
        }
        if(errorStatus > 0)
        {
            break;
        }
        UINT uLen = 0;
        asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
        if (asciiStr)
        {
            sOutStr.assign(asciiStr, uLen);
            bRet = TRUE;
        }
    } while (0);
    SnmpUtilOidFree(&asnOid);
    SnmpUtilVarBindListFree(&snmpVarList);	
    if(asciiStr)
    {
        SnmpUtilMemFree(asciiStr);
    }
    return bRet;
}

BOOL CSnmpOP::GetNextRequest(char* pszOid, int& nVal)
{
	if (!m_bIsActiveIP || !pszOid)
	{
		//theLog.Write("!!CSnmpOP::GetNextRequest,1 c i,m_bIsActiveIP=%d,pszOid=%x", m_bIsActiveIP, pszOid);
		return FALSE;
	}

	BOOL bRet = FALSE;
	char *asciiStr = NULL;
	nVal = 0;	//默认返回0

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetNextRequest,2 c i,SnmpMgrStrToOid fail,err=%u,pszOid=%s"
			, GetLastError(), pszOid);
		return FALSE;
	}


	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	

	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		// initiates the GET request
		if(!SnmpMgrRequest(m_lpMgrSession,SNMP_PDU_GETNEXT,&snmpVarList,&errorStatus,&errorIndex))
		{			
			theLog.Write("!!CSnmpOP::GetNextRequest,9,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			nVal = atoi(asciiStr);
			bRet = TRUE;
		}
	} while (0);

	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	

	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}

	return bRet;
}

BOOL CSnmpOP::GetRequestStr(char* pszOid, unsigned char* pszStr, int& nStrBufferLen)
{
	if (!m_bIsActiveIP || !pszStr)
	{
		//theLog.Write("!!CSnmpOP::GetRequestStr,1,m_bIsActiveIP=%d,pszStr=%x", m_bIsActiveIP, pszStr);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetRequestStr,1,begin,szIP=%s,pszOid=%s", m_szDestIP, pszOid);
	BOOL bRet = FALSE;
	char *asciiStr = NULL;

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetRequestStr,2,c c,end，err=%u,%s",GetLastError(),pszOid);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetRequestStr,3,end");
	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	
	//theLog.Write("CSnmpOP::GetRequestStr,4,end");
	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		if(!m_lpMgrSession)
		{
			theLog.Write("!!CSnmpOP::GetRequestStr 5 m_lpMgrSession=%p", m_lpMgrSession);
			return FALSE;
		}
		if(!SnmpMgrRequest(m_lpMgrSession, SNMP_PDU_GET, &snmpVarList, &errorStatus, &errorIndex))
		{	
			theLog.Write("!!CSnmpOP::GetRequestStr,9,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			if (uLen > nStrBufferLen)
			{
				uLen = nStrBufferLen - 1;	//超出buffer长度，字符串截断，并且传出实际得到的长度
			}
			nStrBufferLen = uLen;
			memcpy(pszStr, asciiStr, uLen);
			//strncpy((char*)pszStr, asciiStr, uLen);
			bRet = TRUE;
		}
	} while (0);
	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	
	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}
	return bRet;
}

BOOL CSnmpOP::GetNextRequestStr(char* pszOid, unsigned char* pszStr, int& nStrBufferLen)
{
	if (!m_bIsActiveIP || !pszStr)
	{
		//theLog.Write("!!CSnmpOP::GetNextRequestStr,1,m_bIsActiveIP=%d,pszStr=%x", m_bIsActiveIP, pszStr);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetNextRequestStr,1,begin,szIP=%s,pszOid=%s", m_szDestIP, pszOid);
	BOOL bRet = FALSE;
	char *asciiStr = NULL;

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetNextRequestStr,2,end，err=%u,%s",GetLastError(),pszOid);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetRequestStr,3,end");
	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	
	//theLog.Write("CSnmpOP::GetRequestStr,4,end");
	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		if(!m_lpMgrSession)
		{
			theLog.Write("!!CSnmpOP::GetNextRequestStr 5 m_lpMgrSession=%p", m_lpMgrSession);
			return FALSE;
		}
		if(!SnmpMgrRequest(m_lpMgrSession, SNMP_PDU_GETNEXT, &snmpVarList, &errorStatus, &errorIndex))
		{			
			theLog.Write("!!CSnmpOP::GetNextRequestStr,9,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			if (uLen > nStrBufferLen)
			{
				uLen = nStrBufferLen - 1;	//超出buffer长度，字符串截断，并且传出实际得到的长度
			}
			nStrBufferLen = uLen;
			memcpy(pszStr, asciiStr, uLen);
			//strncpy((char*)pszStr, asciiStr, uLen);
			bRet = TRUE;
		}
	} while (0);
	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	
	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}
	return bRet;
}

BOOL CSnmpOP::GetNextRequestStrEx(char* pszOid,char* pszStr,int nStrBufferLen,char* pszOidNext,int nOidNextBufferLen)
{
	if (!m_bIsActiveIP || !pszStr || !pszOidNext)
	{
		//theLog.Write("!!CSnmpOP::GetNextRequestStrEx,1,m_bIsActiveIP=%d,pszStr=%x,pszOidNext=%x", m_bIsActiveIP, pszStr, pszOidNext);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetNextRequestStr,1,begin,szIP=%s,pszOid=%s", m_szDestIP, pszOid);
	BOOL bRet = FALSE;
	char *asciiStr = NULL;
	char *szOidNext = NULL;

	AsnObjectIdentifier asnOid;
	if(!SnmpMgrStrToOid(pszOid, &asnOid))
	{
		theLog.Write("!!CSnmpOP::GetNextRequestStrEx,2,end，err=%u,%s",GetLastError(),pszOid);
		return FALSE;
	}

	//theLog.Write("CSnmpOP::GetRequestStr,3,end");
	AsnInteger	errorStatus=0;	// Error type that is returned if encountered
	AsnInteger	errorIndex=0;		// Works with variable above

	SnmpVarBindList snmpVarList;
	snmpVarList.list = (SnmpVarBind *)SnmpUtilMemAlloc(0x18u); 
	snmpVarList.len = 1;	
	//theLog.Write("CSnmpOP::GetRequestStr,4,end");
	// Assigning OID to variable bindings list
	SnmpUtilOidCpy(&snmpVarList.list[0].name,&asnOid);
	snmpVarList.list[0].value.asnType = ASN_NULL;

	do 
	{
		if(!m_lpMgrSession)
		{
			theLog.Write("!!CSnmpOP::GetNextRequestStrEx 5 m_lpMgrSession=%p", m_lpMgrSession);
			return FALSE;
		}
		if(!SnmpMgrRequest(m_lpMgrSession, SNMP_PDU_GETNEXT, &snmpVarList, &errorStatus, &errorIndex))
		{			
			theLog.Write("!!CSnmpOP::GetNextRequestStrEx,9,end,err=%u", GetLastError());
			break;
		}
		if(errorStatus > 0)
		{
			break;
		}
		UINT uLen = 0;
		asciiStr = SNMP_AnyToStr(&snmpVarList.list[0].value, uLen);
		if (asciiStr)
		{
			if (uLen > nStrBufferLen)
			{
				uLen = nStrBufferLen - 1;	//超出buffer长度，字符串截断，并且传出实际得到的长度
			}
			nStrBufferLen = uLen;
			memcpy(pszStr, asciiStr, uLen);
			//strncpy((char*)pszStr, asciiStr, uLen);
		}

		SnmpMgrOidToStr(&snmpVarList.list[0].name,&szOidNext);
		if (szOidNext)
		{
			int nOidNextLen = strlen(szOidNext);
			if (nOidNextLen > (nOidNextBufferLen-1))
			{
				nOidNextLen = nOidNextBufferLen-1;
			}
			nOidNextBufferLen = nOidNextLen;
			memcpy(pszOidNext, szOidNext, nOidNextLen);
			//strncpy(pszOidNext, szOidNext, nOidNextLen);
			bRet = TRUE;
		}
	} while (0);
	SnmpUtilOidFree(&asnOid);
	SnmpUtilVarBindListFree(&snmpVarList);	
	if(asciiStr)
	{
		SnmpUtilMemFree(asciiStr);
	}
	if (szOidNext)
	{
		SnmpUtilMemFree(szOidNext);
	}
	return bRet;
}

//获取OID最后一位数字
//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription.1.1
//result = 1 
int CSnmpOP::GetOidEndNumber(char* pszOid)
{
	if (!pszOid)
	{
		return 0;
	}

	int nOidLen = strlen(pszOid);
	int nDotFirstPostionReverse = nOidLen - 1;
	while(nDotFirstPostionReverse>0)
	{
		if (pszOid[nDotFirstPostionReverse] == '.')
		{
			break;
		}
		nDotFirstPostionReverse--;
	}

	int nRet = atoi(pszOid+nDotFirstPostionReverse+1);
	return nRet;
}

BOOL CSnmpOP::OidBeginWithStr(char* pszOid, char* pszStr)
{
	if (!pszOid || !pszStr)
	{
		return FALSE;
	}

	int nOidLen = strlen(pszOid);
	int nStrLen = strlen(pszStr);
	if (nStrLen > nOidLen)
	{
		return FALSE;
	}

	BOOL bFail = FALSE;
	for (int i=0; i<nStrLen; i++)
	{
		if (pszOid[i] != pszStr[i])
		{
			bFail = TRUE;
			break;
		}
	}
	return !bFail;
}

void CSnmpOP::TimeTicksToDateTimeStr(TimeTicks timeticks, char* pszDateTimeStr)
{
	//时间格式：2016-01-13 10:28:45
	if (!pszDateTimeStr && strlen(pszDateTimeStr) < 20)
	{
		return;
	}

	sprintf(pszDateTimeStr
		, "%04d-%02d-%02d %02d:%02d:%02d"
		, timeticks&0xFF
		, timeticks&0xF00
		, timeticks&0xF000
		, timeticks&0xF0000
		, timeticks&0xF00000
		, timeticks&0xF000000
		);
}

CString CSnmpOP::GetStrByOid(CString szOid)
{
	unsigned char ucStr[256] = {0};
	memset(ucStr, 0x0, 256);
	GetRequest(CStringToChar(szOid), ucStr, 256);
	CString szStr(ucStr);
	return szStr;
}

BOOL CSnmpOP::CheckIPActive()
{
	BOOL bRet = FALSE;

	HANDLE hIcmpFile = NULL;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[] = "Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	do 
	{
		ipaddr = inet_addr(m_szIP.GetString());
		if (ipaddr == INADDR_NONE) 
		{
			theLog.Write("!!CSnmpOP::IsActiveIP,1,usage: %s IP address", m_szIP);
			break;
		}

		hIcmpFile = IcmpCreateFile();
		if (hIcmpFile == INVALID_HANDLE_VALUE) 
		{
			theLog.Write("!!CSnmpOP::IsActiveIP,2,IcmpCreateFile fail. error: %ld", GetLastError() );
			break;
		}    

		ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
		ReplyBuffer = (VOID*) malloc(ReplySize);
		if (ReplyBuffer == NULL) 
		{
			theLog.Write("!!CSnmpOP::IsActiveIP,3,Unable to allocate memory.");
			break;
		}    

		dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
		if (dwRetVal != 0) 
		{
			PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
			struct in_addr ReplyAddr;
			ReplyAddr.S_un.S_addr = pEchoReply->Address;
			if (pEchoReply->Status == IP_SUCCESS)
			{
				bRet = TRUE;
			}
			else
			{
				theLog.Write("CSnmpOP::IsActiveIP,4,Received from %s, dwRetVal=%ld, Status=%ld, time=%ldms"
					, inet_ntoa( ReplyAddr ) 
					, dwRetVal
					, pEchoReply->Status
					, pEchoReply->RoundTripTime);
			}
		}
		else 
		{
			theLog.Write("!!CSnmpOP::IsActiveIP,5,IcmpSendEcho failed. error: %ld, m_szIP=%s,dwRetVal=%d", GetLastError(), m_szIP, dwRetVal);
			break;
		}
	} while (FALSE);

	if (hIcmpFile)
	{
		IcmpCloseHandle(hIcmpFile);
		hIcmpFile = NULL;
	}

	if (ReplyBuffer)
	{
		free(ReplyBuffer);
		ReplyBuffer = NULL;
	}

	m_bIsActiveIP = bRet;

	return m_bIsActiveIP;
}


/************************************************************************/
/* 
//system       OBJECT IDENTIFIER ::= { mib-2 1 }
#define SystemGroupOID TEXT(".1.3.6.1.2.1.1")	//.iso.org.dod.internet.mgmt.mib-2.system
OCTET_STRING sysDescr[OCTET_STRING_SIZE_256];			//::= { system 1 }	//0,只有一个数值
*/
/************************************************************************/
BOOL CSnmpOP::CheckSupportSnmp()
{
	OCTET_STRING* sysDescr = new OCTET_STRING[OCTET_STRING_SIZE_256];
	memset(sysDescr, 0x0, OCTET_STRING_SIZE_256);

	char* cOidStr = new char[OBJECT_IDENTIFIER_SIZE];
	memset(cOidStr, 0x0, OBJECT_IDENTIFIER_SIZE);
	sprintf(cOidStr, "%s.1.0", DecryptOID(SystemGroupOID));	//".1.3.6.1.2.1.1.1.0"
	if (GetRequest(cOidStr, sysDescr, OCTET_STRING_SIZE_256))
	{
		m_bIsSupportSnmp = TRUE;
	}
	else
	{
		theLog.Write("!!CSnmpOP::CheckSupportSnmp,failed.");
		m_bIsSupportSnmp = FALSE;
	}

	delete[] sysDescr;
	delete[] cOidStr;

	return m_bIsSupportSnmp;
}

CString CSnmpOP::EncryptOID(IN CString szOrgString)
{
	return EncryptString(szOrgString);
}

CString CSnmpOP::DecryptOID(IN CString szEncryptString)
{
	return DecryptString(szEncryptString);
}

char* CSnmpOP::CStringToChar(CString& szString)
{
	return (char*)szString.GetString();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////	CSnmpJobMonitor  ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
CSnmpJobMonitor::CSnmpJobMonitor()
{
	m_oJobMap.RemoveAll();
	m_nLastPrintCount = 0;
	m_nLastPrintCount_Color = 0;
	m_nLastPrintCount_WB = 0;
	m_nLastCopyCount = 0;
	m_nLastCopyCount_Color = 0;
	m_nLastCopyCount_WB = 0;
	m_nLastScanCount = 0;
	m_nLastScanCount_Color = 0;
	m_nLastScanCount_WB = 0;
	m_nLastFaxCount = 0;
	m_nLastFaxCount_Color = 0;
	m_nLastFaxCount_WB = 0;
	m_nA3PageCount = 0;
	m_nA4PageCount = 0;
}

CSnmpJobMonitor::~CSnmpJobMonitor()
{
	Release();
}

void CSnmpJobMonitor::Release()
{
	//清空m_oJobMap
	POSITION pos = m_oJobMap.GetStartPosition();
	while(pos)
	{
		int nJobId = 0;
		PSCP_JobInfo pVal = NULL;
		m_oJobMap.GetNextAssoc(pos,nJobId,pVal);
		if (pVal)
		{
			m_oJobMap.RemoveKey(nJobId);
			delete pVal;
		}
	}

	//清空m_oJobList
	while (m_oJobList.GetCount() > 0)
	{
		PSCP_JobInfo pJob = m_oJobList.RemoveTail();
		if (pJob)
		{
			delete pJob;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////	CSnmpPrinterMeterMonitor  ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
CSnmpPrinterMeterMonitor::CSnmpPrinterMeterMonitor()
{
	InitOID();
	InitMeter();
}

CSnmpPrinterMeterMonitor::~CSnmpPrinterMeterMonitor()
{
	Release();
}

void CSnmpPrinterMeterMonitor::CheckMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitor::CheckMeterInfo,1,begin");
	do 
	{
		if(!CheckIPActive())
		{
			theLog.Write("!!CSnmpPrinterMeterMonitor::CheckMeterInfo,2,CheckIPActive fail,m_szIP=%s", m_szIP);
			break;
		}

		if (!m_lpMgrSession)
		{
			theLog.Write("!!CSnmpPrinterMeterMonitor::CheckMeterInfo,3,m_lpMgrSession=%p", m_lpMgrSession);
			break;
		}

		//modify by zxl,20160703
		//现在抄表不进行回调操作，因为，现在为了处理打印异常退费的问题，
		//需要上层代码随时能获取到最新的抄表信息，所以此处调用会更频繁，
		//实际的抄表入库操作放到上层代码中调取此处抄表信息来处理。
#if 0	
		BOOL bGetMeterInfo = FALSE;
		PrinterMeterInfo oMeterInfo;

		{
			CCriticalSection2::Owner lock(m_cs4MeterInfo);
			bGetMeterInfo = GetMeterInfo();
			if (bGetMeterInfo)
			{
				memcpy(&oMeterInfo, &m_oMeterInfo, sizeof(PrinterMeterInfo));
			}
		}

		if (!bGetMeterInfo)
		{
			theLog.Write("!!CSnmpPrinterMeterMonitor::CheckMeterInfo,4,GetMeterInfo fail");
			break;
		}
		else
		{
			if (m_pCallBack)
			{
				m_pCallBack->OnMeterInfo(&oMeterInfo);
			}
			else
			{
				theLog.Write("!!CSnmpPrinterMeterMonitor::CheckMeterInfo,5,m_pCallBack=%p", m_pCallBack);
			}
		}
#else
		if (GetMeterInfo() && IsMeterValid(&m_oMeterInfo))
		{
			CCriticalSection2::Owner lock(m_cs4MeterInfo);
			memcpy(&m_oLastMeterInfo, &m_oMeterInfo, sizeof(PrinterMeterInfo));
			if (!m_bFirstSucc)
			{
				//第一次成功的抄表信息上传到云上，一般是打印网关重启的时候
				m_bFirstSucc = TRUE;
				if (m_pCallBack)
				{
					m_pCallBack->OnMeterInfo(&m_oLastMeterInfo);
				}
				else
				{
					theLog.Write("!!CSnmpPrinterMeterMonitor::CheckMeterInfo,4,m_pCallBack=%p", m_pCallBack);
				}
			}
		}
		else
		{
			theLog.Write("!!CSnmpPrinterMeterMonitor::CheckMeterInfo,5,本次抄表信息失败。");
		}
#endif

	} while (FALSE);
	//theLog.Write("CSnmpPrinterMeterMonitor::CheckMeterInfo,6,end");
}

BOOL CSnmpPrinterMeterMonitor::GetLastMeterInfo(PrinterMeterInfo* pInfo)
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	BOOL bRet = FALSE;
	if (pInfo)
	{
		if (!IsMeterValid(&m_oLastMeterInfo))
		{
			//数据为全空,抄表失败
			theLog.Write("!!CSnmpPrinterMeterMonitor::GetLastMeterInfo,数据为全空,最新抄表失败");
		}
		else
		{
			bRet = TRUE;
			memcpy(pInfo, &m_oLastMeterInfo, sizeof(PrinterMeterInfo));
		}
	}
	return bRet;
}

void CSnmpPrinterMeterMonitor::SetMeterInfo(PrtInputEntryMap& map)
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	PrtInputEntryMap::iterator it;
	for (it=map.begin(); it!=map.end(); it++)
	{
		PPrtInputEntry pEntry = it->second;
		if (pEntry)
		{
			CString szInputName(pEntry->prtInputName);
			szInputName.Trim();
			if (szInputName.GetLength()<=0)
			{
				szInputName = pEntry->prtInputDescription;
				szInputName.Trim();
			}
			szInputName.MakeLower();

			char cDesc[50] = {0};
			if ((pEntry->prtInputMaxCapacity > 0) && (pEntry->prtInputCurrentLevel >= 0))
			{
				double fRatio = 1.0 * pEntry->prtInputCurrentLevel / pEntry->prtInputMaxCapacity;
				int nRatio = (int)(fRatio * 100);
				sprintf(cDesc, "%d%%", nRatio);
			}
			else if (pEntry->prtInputCurrentLevel == 0)
			{
				sprintf(cDesc, "%d%%", pEntry->prtInputCurrentLevel);
			}
			else if ((pEntry->prtInputCurrentLevel == -1) || (pEntry->prtInputCurrentLevel == -2))
			{
				sprintf(cDesc, "%s", "未知");
			}
			else if (pEntry->prtInputCurrentLevel == -3)
			{
				sprintf(cDesc, "%s", "不为空");
			}

			if (strlen(cDesc) <= 0)
			{
				theLog.Write("##CSnmpPrinterMeterMonitor::SetMeterInfo,szInputName=[%s],prtInputType=%d,"
					"prtInputCapacityUnit=%d,prtInputMaxCapacity=%d,prtInputCurrentLevel=%d"
				, szInputName, pEntry->prtInputType, pEntry->prtInputCapacityUnit
				, pEntry->prtInputMaxCapacity, pEntry->prtInputCurrentLevel);
				continue;
			}

			if (m_eType == BT_LG || m_eType == BT_LM)
			{
				if (pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray)
				{
					if (szInputName.Find("1") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper1LevelPercent, cDesc);
					}
					else if (szInputName.Find("2") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper2LevelPercent, cDesc);
					}
					else if (szInputName.Find("3") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper3LevelPercent, cDesc);

					}
					else if (szInputName.Find("4") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper4LevelPercent, cDesc);
					}
				}
				else if (pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray)
				{
					strcpy(m_oMeterInfo.cPaperBypassLevelPercent, cDesc);
				}
			}
			else if (m_eType == BT_XP)
			{
				if (pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray
					|| pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray)
				{
					if (szInputName.Find("1") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper1LevelPercent, cDesc);
					}
					else if (szInputName.Find("2") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper2LevelPercent, cDesc);
					}
					else if (szInputName.Find("3") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper3LevelPercent, cDesc);

					}
					else if (szInputName.Find("4") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper4LevelPercent, cDesc);
					}
				}
				else if (pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::Other)
				{
					strcpy(m_oMeterInfo.cPaperBypassLevelPercent, cDesc);
				}			
			}
			else
			{
				if (pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray
					|| pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray)
				{
					if (szInputName.Find("1") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper1LevelPercent, cDesc);
					}
					else if (szInputName.Find("2") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper2LevelPercent, cDesc);
					}
					else if (szInputName.Find("3") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper3LevelPercent, cDesc);

					}
					else if (szInputName.Find("4") >= 0)
					{
						strcpy(m_oMeterInfo.cPaper4LevelPercent, cDesc);
					}
				}
				else if (pEntry->prtInputType == ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedManual)
				{
					strcpy(m_oMeterInfo.cPaperBypassLevelPercent, cDesc);
				}			
			}
		}
	}
}

void CSnmpPrinterMeterMonitor::SetMeterInfo(PrtMarkerSuppliesEntryMap& map)
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=map.begin(); it!=map.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			CString szDesc(CCommonFun::UTF8ToMultiByte(pEntry->prtMarkerSuppliesDescription));
			szDesc.MakeLower();
			char cDesc[50] = {0};
			if ((pEntry->prtMarkerSuppliesMaxCapacity > 0) && (pEntry->prtMarkerSuppliesLevel >= 0))
			{
				double fRatio = 1.0 * pEntry->prtMarkerSuppliesLevel / pEntry->prtMarkerSuppliesMaxCapacity;
				int nRatio = (int)(fRatio * 100);
				sprintf(cDesc, "%d%%", nRatio);
			}
			else if (pEntry->prtMarkerSuppliesLevel == 0)
			{
				sprintf(cDesc, "%d%%", pEntry->prtMarkerSuppliesLevel);
			}
			else if ((pEntry->prtMarkerSuppliesLevel == -1) || (pEntry->prtMarkerSuppliesLevel == -2))
			{
				sprintf(cDesc, "%s", "未知");
			}
			else if (pEntry->prtMarkerSuppliesLevel == -3)
			{
				sprintf(cDesc, "%s", "不为空");
			}
			else if (pEntry->prtMarkerSuppliesSupplyUnit == ENUM_STRUCT_VALUE(PrtMarkerSuppliesSupplyUnitTC)::Percent)
			{
				if (pEntry->prtMarkerSuppliesLevel >= 0)
				{
					sprintf(cDesc, "%d%%", pEntry->prtMarkerSuppliesLevel);
				}
			}

			if (strlen(cDesc) <= 0)
			{
				theLog.Write("##CSnmpPrinterMeterMonitor::SetMeterInfo,szDesc=[%s],prtMarkerSuppliesType=%d,"
					"prtMarkerSuppliesSupplyUnit=%d,prtMarkerSuppliesMaxCapacity=%d,prtMarkerSuppliesLevel=%d"
					, szDesc, pEntry->prtMarkerSuppliesType, pEntry->prtMarkerSuppliesSupplyUnit
					, pEntry->prtMarkerSuppliesMaxCapacity, pEntry->prtMarkerSuppliesLevel);
				continue;
			}

			if (szDesc.Find("cyan") >= 0 || szDesc.Find("青色") >= 0)
			{
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					strcpy(m_oMeterInfo.cTonerCyanLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
				{
					strcpy(m_oMeterInfo.cOPCCyanLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
				{
					strcpy(m_oMeterInfo.cDeveloperCyanLevelPercent, cDesc);
				}
			}
			else if (szDesc.Find("magenta") >= 0 || szDesc.Find("品红色") >= 0)
			{
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					strcpy(m_oMeterInfo.cTonerMagentaLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
				{
					strcpy(m_oMeterInfo.cOPCMagentaLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
				{
					strcpy(m_oMeterInfo.cDeveloperMagentaLevelPercent, cDesc);
				}
			}
			else if (szDesc.Find("yellow") >= 0 || szDesc.Find("黄色") >= 0)
			{
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					strcpy(m_oMeterInfo.cTonerYellowLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
				{
					strcpy(m_oMeterInfo.cOPCYellowLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
				{
					strcpy(m_oMeterInfo.cDeveloperYellowLevelPercent, cDesc);
				}
			}
			else //if (szDesc.Find("black") >= 0)	//这个分支中，可能出现汉字，如利盟机器的[黑色碳粉]、[成像部件]、[维护工具包]等等
			{
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					strcpy(m_oMeterInfo.cTonerBlackLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
				{
					strcpy(m_oMeterInfo.cOPCBlackLevelPercent, cDesc);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
				{
					strcpy(m_oMeterInfo.cDeveloperBlackLevelPercent, cDesc);
				}
			}
		}
	}
}

void CSnmpPrinterMeterMonitor::InitMeter()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	m_eType = BT_THIN_CLT;
	m_bFirstSucc = FALSE;

	memset(&m_oMeterInfo, 0x0, sizeof(PrinterMeterInfo));
	strcpy(m_oMeterInfo.cPaper1LevelPercent, "-");
	strcpy(m_oMeterInfo.cPaper2LevelPercent, "-");
	strcpy(m_oMeterInfo.cPaper3LevelPercent, "-");
	strcpy(m_oMeterInfo.cPaper4LevelPercent, "-");
	strcpy(m_oMeterInfo.cPaperBypassLevelPercent, "-");

	strcpy(m_oMeterInfo.cTonerCyanLevelPercent, "-");
	strcpy(m_oMeterInfo.cTonerMagentaLevelPercent, "-");
	strcpy(m_oMeterInfo.cTonerYellowLevelPercent, "-");
	strcpy(m_oMeterInfo.cTonerBlackLevelPercent, "-");

	strcpy(m_oMeterInfo.cOPCCyanLevelPercent, "-");
	strcpy(m_oMeterInfo.cOPCMagentaLevelPercent, "-");
	strcpy(m_oMeterInfo.cOPCYellowLevelPercent, "-");
	strcpy(m_oMeterInfo.cOPCBlackLevelPercent, "-");

	strcpy(m_oMeterInfo.cDeveloperCyanLevelPercent, "-");
	strcpy(m_oMeterInfo.cDeveloperMagentaLevelPercent, "-");
	strcpy(m_oMeterInfo.cDeveloperYellowLevelPercent, "-");
	strcpy(m_oMeterInfo.cDeveloperBlackLevelPercent, "-");

	memset(&m_oLastMeterInfo, 0x0, sizeof(PrinterMeterInfo));
	strcpy(m_oLastMeterInfo.cPaper1LevelPercent, "-");
	strcpy(m_oLastMeterInfo.cPaper2LevelPercent, "-");
	strcpy(m_oLastMeterInfo.cPaper3LevelPercent, "-");
	strcpy(m_oLastMeterInfo.cPaper4LevelPercent, "-");
	strcpy(m_oLastMeterInfo.cPaperBypassLevelPercent, "-");

	strcpy(m_oLastMeterInfo.cTonerCyanLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cTonerMagentaLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cTonerYellowLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cTonerBlackLevelPercent, "-");

	strcpy(m_oLastMeterInfo.cOPCCyanLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cOPCMagentaLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cOPCYellowLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cOPCBlackLevelPercent, "-");

	strcpy(m_oLastMeterInfo.cDeveloperCyanLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cDeveloperMagentaLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cDeveloperYellowLevelPercent, "-");
	strcpy(m_oLastMeterInfo.cDeveloperBlackLevelPercent, "-");
}

void CSnmpPrinterMeterMonitor::ResetPaperMeter()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//总数，从打印机抄上来的总数
	m_oMeterInfo.nAllTotalPage = 0;			//总页数
	m_oMeterInfo.nAllTotalDistance = 0;		//总长度
	m_oMeterInfo.nA3HBTotalPage = 0;		//A3黑白总页数
	m_oMeterInfo.nA3ColorTotalPage = 0;		//A3彩色总页数
	m_oMeterInfo.nA4HBTotalPage = 0;		//A4黑白总页数
	m_oMeterInfo.nA4ColorTotalPage = 0;		//A4彩色总页数

	//打印计数
	m_oMeterInfo.nA4PrintTotalPage = 0;		//A4打印总计数
	m_oMeterInfo.nA4PrintColorPage = 0;		//A4打印彩色总计数
	m_oMeterInfo.nA4PrintHBPage = 0;	    //A4打印黑白总计数
	m_oMeterInfo.nA3PrintTotalPage = 0;		//A3打印总计数
	m_oMeterInfo.nA3PrintColorPage = 0;		//A3打印彩色总计数
	m_oMeterInfo.nA3PrintHBPage = 0;	    //A3打印黑白总计数
	m_oMeterInfo.nOtherPrintTotalPage = 0;	//其它纸型打印总计数
	m_oMeterInfo.nOtherPrintColorPage = 0;	//其它纸型打印彩色总计数
	m_oMeterInfo.nOtherPrintHBPage = 0;		//其它纸型打印黑白总计数

	m_oMeterInfo.nPrintDistance = 0;		//打印长度
	m_oMeterInfo.nPrintArea = 0;			//打印面积

	//复印计数
	m_oMeterInfo.nA4CopyTotalPage = 0;		//A4复印总计数
	m_oMeterInfo.nA4CopyColorPage = 0;		//A4复印彩色总计数
	m_oMeterInfo.nA4CopyHBPage = 0;			//A4复印黑白总计数
	m_oMeterInfo.nA3CopyTotalPage = 0;		//A3复印总计数
	m_oMeterInfo.nA3CopyColorPage = 0;		//A3复印彩色总计数
	m_oMeterInfo.nA3CopyHBPage = 0;			//A3复印黑白总计数
	m_oMeterInfo.nOtherCopyTotalPage = 0;	//其它纸型复印总计数
	m_oMeterInfo.nOtherCopyColorPage = 0;	//其它纸型复印彩色总计数
	m_oMeterInfo.nOtherCopyHBPage = 0;		//其它纸型复印黑白总计数

	m_oMeterInfo.nCopyDistance = 0;			//复印长度
	m_oMeterInfo.nCopyArea = 0;				//复印面积

	//扫描计数
	m_oMeterInfo.nScanTotalPage = 0;		//扫描总计数
	m_oMeterInfo.nScanBigColorPage = 0;		//扫描大幅彩色总计数
	m_oMeterInfo.nScanBigHBPage = 0;		//扫描大幅黑白总计数
	m_oMeterInfo.nScanSmallColorPage = 0;	//扫描小幅彩色总计数
	m_oMeterInfo.nScanSmallHBPage = 0;		//扫描小幅黑白总计数

	//传真计数
	m_oMeterInfo.nA4FaxTotalPage = 0;		//A4传真总计数
	m_oMeterInfo.nA4FaxColorPage = 0;		//A4传真彩色总计数
	m_oMeterInfo.nA4FaxHBPage = 0;			//A4传真黑白总计数
	m_oMeterInfo.nA3FaxTotalPage = 0;		//A3传真总计数
	m_oMeterInfo.nA3FaxColorPage = 0;		//A3传真彩色总计数
	m_oMeterInfo.nA3FaxHBPage = 0;			//A3传真黑白总计数
	m_oMeterInfo.nOtherFaxTotalPage = 0;	//其它纸型传真总计数
	m_oMeterInfo.nOtherFaxColorPage = 0;	//其它纸型传真彩色总计数
	m_oMeterInfo.nOtherFaxHBPage = 0;		//其它纸型传真黑白总计数

	//其它操作计数
	m_oMeterInfo.nOtherOpTotalPage = 0;		//其它操作总计数
	m_oMeterInfo.nOtherOpColorPage = 0;		//其它操作彩色总计数
	m_oMeterInfo.nOtherOpHBPage = 0;	    //其它操作黑白总计数
}

BOOL CSnmpPrinterMeterMonitor::IsMeterValid(PrinterMeterInfo* pInfo)
{
	if (!pInfo)
	{
		theLog.Write("!!CSnmpPrinterMeterMonitor::IsMeterValid,pInfo=%p", pInfo);
		return FALSE;
	}

	if (
		pInfo->nAllTotalPage <= 0				//总页数
		&& pInfo->nAllTotalDistance <= 0		//总长度
		&& pInfo->nA3HBTotalPage <= 0			//A3黑白总页数
		&& pInfo->nA3ColorTotalPage <= 0		//A3彩色总页数
		&& pInfo->nA4HBTotalPage <= 0			//A4黑白总页数
		&& pInfo->nA4ColorTotalPage <= 0		//A4彩色总页数

		//打印计数
		&& pInfo->nA4PrintTotalPage <= 0		//A4打印总计数
		&& pInfo->nA4PrintColorPage <= 0		//A4打印彩色总计数
		&& pInfo->nA4PrintHBPage <= 0			//A4打印黑白总计数
		&& pInfo->nA3PrintTotalPage <= 0		//A3打印总计数
		&& pInfo->nA3PrintColorPage <= 0		//A3打印彩色总计数
		&& pInfo->nA3PrintHBPage <= 0			//A3打印黑白总计数
		&& pInfo->nOtherPrintTotalPage <= 0		//其它纸型打印总计数
		&& pInfo->nOtherPrintColorPage <= 0		//其它纸型打印彩色总计数
		&& pInfo->nOtherPrintHBPage <= 0		//其它纸型打印黑白总计数

		&& pInfo->nPrintDistance <= 0			//打印长度
		&& pInfo->nPrintArea <= 0				//打印面积

		//复印计数
		&& pInfo->nA4CopyTotalPage <= 0			//A4复印总计数
		&& pInfo->nA4CopyColorPage <= 0			//A4复印彩色总计数
		&& pInfo->nA4CopyHBPage <= 0			//A4复印黑白总计数
		&& pInfo->nA3CopyTotalPage <= 0			//A3复印总计数
		&& pInfo->nA3CopyColorPage <= 0			//A3复印彩色总计数
		&& pInfo->nA3CopyHBPage <= 0			//A3复印黑白总计数
		&& pInfo->nOtherCopyTotalPage <= 0		//其它纸型复印总计数
		&& pInfo->nOtherCopyColorPage <= 0		//其它纸型复印彩色总计数
		&& pInfo->nOtherCopyHBPage <= 0			//其它纸型复印黑白总计数

		&& pInfo->nCopyDistance <= 0			//复印长度
		&& pInfo->nCopyArea <= 0				//复印面积


		//扫描计数
		&& pInfo->nScanTotalPage <= 0			//扫描总计数
		&& pInfo->nScanBigColorPage <= 0		//扫描大幅彩色总计数
		&& pInfo->nScanBigHBPage <= 0			//扫描大幅黑白总计数
		&& pInfo->nScanSmallColorPage <= 0		//扫描小幅彩色总计数
		&& pInfo->nScanSmallHBPage <= 0			//扫描小幅黑白总计数

		//传真计数
		&& pInfo->nA4FaxTotalPage <= 0			//A4传真总计数
		&& pInfo->nA4FaxColorPage <= 0			//A4传真彩色总计数
		&& pInfo->nA4FaxHBPage <= 0				//A4传真黑白总计数
		&& pInfo->nA3FaxTotalPage <= 0			//A3传真总计数
		&& pInfo->nA3FaxColorPage <= 0			//A3传真彩色总计数
		&& pInfo->nA3FaxHBPage <= 0				//A3传真黑白总计数
		&& pInfo->nOtherFaxTotalPage <= 0		//其它纸型传真总计数
		&& pInfo->nOtherFaxColorPage <= 0		//其它纸型传真彩色总计数
		&& pInfo->nOtherFaxHBPage <= 0			//其它纸型传真黑白总计数

		//其它操作计数
		&& pInfo->nOtherOpTotalPage <= 0		//其它操作总计数
		&& pInfo->nOtherOpColorPage <= 0		//其它操作彩色总计数
		&& pInfo->nOtherOpHBPage <= 0			//其它操作黑白总计数
		)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void CSnmpPrinterMeterMonitor::Release()
{
	CCriticalSection2::Owner lock(m_cs4MeterInfo);
	m_eType = BT_THIN_CLT;
	m_bFirstSucc = FALSE;
	memset(&m_oMeterInfo, 0x0, sizeof(PrinterMeterInfo));
}

void CSnmpPrinterMeterMonitor::SetBrandType(BRAND_TYPE eType)
{
	m_eType = eType;
	theLog.Write("CSnmpPrinterMeterMonitor::SetBrandType,m_eType=%d", m_eType);
}

void CSnmpPrinterMeterMonitor::InitOID()
{
	m_szPrtMarkerLifeCountOID.Format("%s.1.1", DecryptOID(PrtMarkerLifeCountOID));
}

//本函数只能在CheckMeterInfo只内部调用
BOOL CSnmpPrinterMeterMonitor::GetMeterInfo()
{
	//theLog.Write("CSnmpPrinterMeterMonitor::GetMeterInfo,1,begin");
	CCriticalSection2::Owner lock(m_cs4MeterInfo);

	//重置初始化抄表计数，防止数据出现累加情况。
	ResetPaperMeter();

	int nValue = 0;
	//出纸总页数
	if (!GetRequest(CStringToChar(m_szPrtMarkerLifeCountOID), nValue))	//出纸总页数=printmib.prtMarker.prtMarkerTable.prtMarkerEntry.prtMarkerLifeCount.1.1
	{
		theLog.Write("!!CSnmpPrinterMeterMonitor::GetMeterInfo,2,获取出纸总页数失败");
		return FALSE;	//总数失败了，则下面节点的就不要继续抄表了。
	}
	else
	{
		//m_oMeterInfo.nOtherPrintTotalPage = nValue;
		m_oMeterInfo.nAllTotalPage = nValue;
		//theLog.Write("CSnmpPrinterMeterMonitor::GetMeterInfo,3,获取出纸总页数成功，nValue=%d", nValue);
	}

	//theLog.Write("CSnmpPrinterMeterMonitor::GetMeterInfo,100,end");
	return TRUE;
}


/************************************************************************/
/*     打印机状态监视类                                                 */
/************************************************************************/
//
CSnmpMonitorHelper::CSnmpMonitorHelper()
{
	Init();
}

CSnmpMonitorHelper::~CSnmpMonitorHelper()
{
	Release();
}

void CSnmpMonitorHelper::SetBrandType(BRAND_TYPE eType)
{
	m_eType = eType;
	theLog.Write("CSnmpMonitorHelper::SetBrandType,m_eType=%d", m_eType);
}

CString CSnmpMonitorHelper::GetPaperTypeByPrtInputEntry(PPrtInputEntry pEntry)
{
	int nWidthTenthsOfMillimeter = 0;	//十分之一毫米
	int nHeightTenthsOfMillimeter = 0;	//十分之一毫米
	if (pEntry)
	{
		if (pEntry->prtInputDimUnit == 	ENUM_STRUCT_VALUE(PrtMediaUnitTC)::Micrometers)
		{
			//297000*210000
			nWidthTenthsOfMillimeter = MulDiv(pEntry->prtInputMediaDimFeedDirDeclared, 1, 100);
			nHeightTenthsOfMillimeter = MulDiv(pEntry->prtInputMediaDimXFeedDirDeclared, 1, 100);
		}
		else if (pEntry->prtInputDimUnit == ENUM_STRUCT_VALUE(PrtMediaUnitTC)::TenThousandthsOfInches)
		{
			//1英寸=2.54厘米
			//116929*82677
			nWidthTenthsOfMillimeter = MulDiv(pEntry->prtInputMediaDimFeedDirDeclared*2.54, 1, 100);
			nHeightTenthsOfMillimeter = MulDiv(pEntry->prtInputMediaDimXFeedDirDeclared*2.54, 1, 100);
		}	
	}

	return CCommonFun::GetPagerSize3(nWidthTenthsOfMillimeter, nHeightTenthsOfMillimeter);
}

BOOL CSnmpMonitorHelper::IsFalutInfo(CString szDetial)
{
	BOOL bYes = FALSE;
	szDetial.Trim();
	if (m_eType == BT_CANON)
	{//佳能
		//A service call error has occurred.(0407)
		if (szDetial.Find("A service call error has occurred.") >= 0)
		{
			bYes = TRUE;
		}
	}
	else if (m_eType == BT_KM || m_eType == BT_ZD)
	{//柯美,震旦和柯美是一样的
		//Service Call C5351
		//Service Call C2021 (Fatal Error)
		if (szDetial.Find("Service Call") >= 0)
		{
			bYes = TRUE;
		}
	}
	else if (m_eType == BT_XP)
	{//夏普
		//Service Error F2-73
		//Service Error TA
		if (szDetial.Find("Service Error") >= 0)
		{
			bYes = TRUE;
		}
	}
#if 0	//三星里面的所有预警都是S2-3319这类格式
	else if (m_eType == BT_SX)
	{//三星
		//S2-3319 The Machine is currently running on auto color registration mode.
		if (TRUE)
		{
			bYes = TRUE;
		}
	}
#endif
	return bYes;
}

CString CSnmpMonitorHelper::GetFaultCode(CString szDetial)
{
	CString szFaultCode = "";
	szDetial.Trim();
	if (m_eType == BT_CANON)
	{//佳能
		//A service call error has occurred.(0407)
		if (szDetial.Find("A service call error has occurred.") >= 0)
		{
			int nLeft = szDetial.Find("(");
			int nRight = szDetial.Find(")");
			szFaultCode = szDetial.Mid(nLeft+1, nRight-nLeft-1);
		}
	}
	else if (m_eType == BT_KM || m_eType == BT_ZD)
	{//柯美,震旦和柯美是一样的
		//Service Call C5351
		//Service Call C2021 (Fatal Error)
		if (szDetial.Find("Service Call") >= 0)
		{
			CStringArray ary;
			CCommonFun::StringSplit(szDetial, &ary, " ", TRUE);
			if (ary.GetCount() >= 3)
			{
				szFaultCode = ary.GetAt(2);
			}
		}
	}
	else if (m_eType == BT_XP)
	{//夏普
		//Service Error F2-73
		//Service Error TA
		if (szDetial.Find("Service Error") >= 0)
		{
			CStringArray ary;
			CCommonFun::StringSplit(szDetial, &ary, " ", TRUE);
			if (ary.GetCount() >= 3)
			{
				szFaultCode = ary.GetAt(2);
			}
		}
	}
#if 0	//三星里面的所有预警都是S2-3319这类格式
	else if (m_eType == BT_SX)
	{//三星
		//S2-3319 The Machine is currently running on auto color registration mode.
		if (TRUE)
		{
			CStringArray ary;
			CCommonFun::StringSplit(szDetial, &ary, " ", TRUE);
			if (ary.GetCount() >= 1)
			{
				szFaultCode = ary.GetAt(0);
			}
		}
	}
#endif
	return szFaultCode;
}

void CSnmpMonitorHelper::CheckFaultCode()
{
	//此接口要让子类实现
}

void CSnmpMonitorHelper::Init()
{
	Release();
	InitOID();
}

void CSnmpMonitorHelper::Release()
{
	m_eType = BT_THIN_CLT;
	m_pPrinterMeterMonitor = NULL;
	ClearAll();
}

void CSnmpMonitorHelper::SetPrinterMeterMonitor(CSnmpPrinterMeterMonitor* pMeter)
{
	m_pPrinterMeterMonitor = pMeter;
	theLog.Write("CSnmpMonitorHelper::SetPrinterMeterMonitor, m_pPrinterMeterMonitor=%p", m_pPrinterMeterMonitor);
}

void CSnmpMonitorHelper::SetPaperConfig(CString sConfig, int nMin)
{
	m_szPaperConfig = sConfig;
	m_nPaperMinRatio = nMin;
	theLog.Write("CSnmpMonitorHelper::SetPaperConfig, m_szPaperConfig=%s,m_nPaperMinRatio=%d"
		, m_szPaperConfig, m_nPaperMinRatio);
}

void CSnmpMonitorHelper::SetInkConfig(CString sConfig, int nMin)
{
	m_szInkConfig = sConfig;
	m_nInkMinRatio = nMin;
	theLog.Write("CSnmpMonitorHelper::SetInkConfig, m_szInkConfig=%s,m_nInkMinRatio=%d"
		, m_szInkConfig, m_nInkMinRatio);
}

void CSnmpMonitorHelper::CheckMonitor()
{
	if (CheckIPActive())
	{
		ClearAll();
		GetAll();
		CheckAll();
	}
}

//通过SNMP获取OID信息
BOOL CSnmpMonitorHelper::GetAll()
{
    GetPrinterTable();
	GetGeneral();
	GetStorageRef();
	GetDeviceRef();
	GetCover();
	GetLocalization();
	GetInput();
	GetOutput();
	GetMarker();
	GetMarkerSupplies();
	GetMarkerColorant();
	GetMediaPath();
	GetChannel();
	GetInterpreter();
	GetConsoleDisplayBuffer();
	GetConsoleLight();
	GetAlert();
	return TRUE;
}

BOOL CSnmpMonitorHelper::GetGeneral()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetStorageRef()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetDeviceRef()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetCover()
{
	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverIndex
	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription
	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverStatus

	CCriticalSection2::Owner lock(m_cs4CoverMap);

	char *cOidBegin = CStringToChar(m_szPrtCoverDescriptionOID);	//"printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription"
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oCoverMap.find(nIndex) == m_oCoverMap.end())
			{
				PPrtCoverEntry pEntry = new PrtCoverEntry;
				memset(pEntry, 0x0, sizeof(PrtCoverEntry));
				m_oCoverMap.insert(pair<int,PPrtCoverEntry>(nIndex, pEntry));
				pEntry->prtCoverIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtCoverEntryMap::iterator it;
	for (it=m_oCoverMap.begin(); it!=m_oCoverMap.end(); it++)
	{
		PPrtCoverEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtCoverIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.1.%d", m_szPrtCoverDescriptionOID, nIndex);	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverDescription
			GetRequest(cOidStr, pEntry->prtCoverDescription, sizeof(pEntry->prtCoverDescription));

			sprintf(cOidStr, "%s.1.%d", m_szPrtCoverStatusOID, nIndex);	//printmib.prtCover.prtCoverTable.prtCoverEntry.prtCoverStatus
			GetRequest(cOidStr, (int&)pEntry->prtCoverStatus);
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelper::GetLocalization()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetInput()
{
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputIndex 
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputType
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDimUnit
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirDeclared
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirDeclared
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirChosen
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirChosen
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCapacityUnit
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMaxCapacity
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCurrentLevel
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputStatus
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaName
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputName
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVendorName
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputModel
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVersion
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSerialNumber
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDescription
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSecurity
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaWeight
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaType
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaColor
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaFormParts
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaLoadTimeout
	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputNextIndex

	CCriticalSection2::Owner lock(m_cs4InputMap);
;
	char *cOidBegin = CStringToChar(m_szPrtInputIndexOID);
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oInputMap.find(nIndex) == m_oInputMap.end())
			{
				PPrtInputEntry pEntry = new PrtInputEntry;
				memset(pEntry, 0x0, sizeof(PrtInputEntry));
				m_oInputMap.insert(pair<int,PPrtInputEntry>(nIndex, pEntry));
				pEntry->prtInputIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtInputEntryMap::iterator it;
	for (it=m_oInputMap.begin(); it!=m_oInputMap.end(); it++)
	{
		PPrtInputEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtInputIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.1.%d", m_szPrtInputTypeOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputType
			GetRequest(cOidStr, (int&)pEntry->prtInputType);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputDimUnitOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDimUnit
			GetRequest(cOidStr, (int&)pEntry->prtInputDimUnit);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaDimFeedDirDeclaredOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirDeclared
			GetRequest(cOidStr, (int&)pEntry->prtInputMediaDimFeedDirDeclared);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaDimXFeedDirDeclaredOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirDeclared
			GetRequest(cOidStr, pEntry->prtInputMediaDimXFeedDirDeclared);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaDimFeedDirChosenOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimFeedDirChosen
			GetRequest(cOidStr, pEntry->prtInputMediaDimFeedDirChosen);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaDimXFeedDirChosenOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaDimXFeedDirChosen
			GetRequest(cOidStr, pEntry->prtInputMediaDimXFeedDirChosen);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputCapacityUnitOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCapacityUnit
			GetRequest(cOidStr, (int&)pEntry->prtInputCapacityUnit);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMaxCapacityOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMaxCapacity
			GetRequest(cOidStr, pEntry->prtInputMaxCapacity);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputCurrentLevelOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputCurrentLevel
			GetRequest(cOidStr, pEntry->prtInputCurrentLevel);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputStatusOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputStatus
			GetRequest(cOidStr, pEntry->prtInputStatus);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaNameOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaName
			GetRequest(cOidStr, pEntry->prtInputMediaName, sizeof(pEntry->prtInputMediaName));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputNameOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputName
			GetRequest(cOidStr, pEntry->prtInputName, sizeof(pEntry->prtInputName));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputVendorNameOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVendorName
			GetRequest(cOidStr, pEntry->prtInputVendorName, sizeof(pEntry->prtInputVendorName));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputModelOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputModel
			GetRequest(cOidStr, pEntry->prtInputModel, sizeof(pEntry->prtInputModel));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputVersionOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputVersion
			GetRequest(cOidStr, pEntry->prtInputVersion, sizeof(pEntry->prtInputVersion));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputSerialNumberOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSerialNumber
			GetRequest(cOidStr, pEntry->prtInputSerialNumber, sizeof(pEntry->prtInputSerialNumber));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputDescriptionOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputDescription
			GetRequest(cOidStr, pEntry->prtInputDescription, sizeof(pEntry->prtInputDescription));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputSecurityOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputSecurity
			GetRequest(cOidStr, (int&)pEntry->prtInputSecurity);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaWeightOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaWeight
			GetRequest(cOidStr, pEntry->prtInputMediaWeight);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaTypeOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaType
			GetRequest(cOidStr, pEntry->prtInputMediaType, sizeof(pEntry->prtInputMediaType));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaColorOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaColor
			GetRequest(cOidStr, pEntry->prtInputMediaColor, sizeof(pEntry->prtInputMediaColor));

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaFormPartsOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaFormParts
			GetRequest(cOidStr, pEntry->prtInputMediaFormParts);

#ifdef RFC_3805
			sprintf(cOidStr, "%s.1.%d", m_szPrtInputMediaLoadTimeoutOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputMediaLoadTimeout
			GetRequest(cOidStr, pEntry->prtInputMediaLoadTimeout);

			sprintf(cOidStr, "%s.1.%d", m_szPrtInputNextIndexOID, nIndex);	//printmib.prtInput.prtInputTable.prtInputEntry.prtInputNextIndex
			GetRequest(cOidStr, pEntry->prtInputNextIndex);
#endif
		}
	}


	return TRUE;
}

BOOL CSnmpMonitorHelper::GetOutput()
{
	return FALSE;

}

BOOL CSnmpMonitorHelper::GetMarker()
{
	return FALSE;

}


BOOL CSnmpMonitorHelper::GetMarkerSupplies()
{
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesIndex
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesColorantIndex
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesClass
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesType
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesDescription
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesSupplyUnit
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMaxCapacity
	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesLevel

	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

    bool bIsStdLeafNodeOfTable = true;

	char *cOidBegin = CStringToChar(m_szPrtMarkerSuppliesMarkerIndexOID);	//"printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex"
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
            // 如果考虑效率, 则只用判断一次即可
            bIsStdLeafNodeOfTable = Xab::isStdLeafNodeOfTable(cOidBegin, pszOidNext);

			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oMarkerSuppliesMap.find(nIndex) == m_oMarkerSuppliesMap.end())
			{
				PPrtMarkerSuppliesEntry pEntry = new PrtMarkerSuppliesEntry;
				memset(pEntry, 0x0, sizeof(PrtMarkerSuppliesEntry));
				m_oMarkerSuppliesMap.insert(pair<int,PPrtMarkerSuppliesEntry>(nIndex, pEntry));
				pEntry->prtMarkerSuppliesIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

    CString sFormat;
    if (bIsStdLeafNodeOfTable)
        sFormat = _T("%s.%d");
    else
        sFormat = _T("%s.1.%d");



	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtMarkerSuppliesIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesMarkerIndexOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMarkerIndex
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesMarkerIndex);

			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesColorantIndexOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesColorantIndex
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesColorantIndex);

			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesClassOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesClass
			GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesClass);

			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesTypeOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesType
			GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesType);

			//如果是墨粉，则从prtMarkerColorant中获取颜色信息
			if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
				|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
			{
				sprintf(cOidStr, sFormat, m_szPrtMarkerColorantValueOID, pEntry->prtMarkerSuppliesColorantIndex);	//printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry.prtMarkerColorantValue
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));
			}
			else
			{
				sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesDescriptionOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesDescription
				GetRequest(cOidStr, pEntry->prtMarkerSuppliesDescription, sizeof(pEntry->prtMarkerSuppliesDescription));
			}

			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesSupplyUnitOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesSupplyUnit
			GetRequest(cOidStr, (int&)pEntry->prtMarkerSuppliesSupplyUnit);

			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesMaxCapacityOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesMaxCapacity
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesMaxCapacity);

			sprintf(cOidStr, sFormat, m_szPrtMarkerSuppliesLevelOID, nIndex);	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry.prtMarkerSuppliesLevel
			GetRequest(cOidStr, pEntry->prtMarkerSuppliesLevel);
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelper::GetMarkerColorant()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetMediaPath()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetChannel()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetInterpreter()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetConsoleDisplayBuffer()
{
	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferIndex"
	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferText"

	CCriticalSection2::Owner lock(m_cs4ConsoleDisplayBufferMap);

	char *cOidBegin = CStringToChar(m_szPrtConsoleDisplayBufferTextOID);	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferText"
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oConsoleDisplayBufferMap.find(nIndex) == m_oConsoleDisplayBufferMap.end())
			{
				PPrtConsoleDisplayBufferEntry pEntry = new PrtConsoleDisplayBufferEntry;
				memset(pEntry, 0x0, sizeof(PrtConsoleDisplayBufferEntry));
				m_oConsoleDisplayBufferMap.insert(pair<int,PPrtConsoleDisplayBufferEntry>(nIndex, pEntry));
				pEntry->prtConsoleDisplayBufferIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtConsoleDisplayBufferEntryMap::iterator it;
	for (it=m_oConsoleDisplayBufferMap.begin(); it!=m_oConsoleDisplayBufferMap.end(); it++)
	{
		PPrtConsoleDisplayBufferEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtConsoleDisplayBufferIndex;

			char cOidStr[128] = {0};
			//sprintf(cOidStr, "%s.1.%d", m_szPrtConsoleDisplayBufferIndexOID, nIndex);	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferIndex"
			//GetRequest(cOidStr, pEntry->prtConsoleDisplayBufferIndex);

			sprintf(cOidStr, "%s.1.%d", m_szPrtConsoleDisplayBufferTextOID, nIndex);	//"printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry.prtConsoleDisplayBufferText"
			GetRequest(cOidStr, pEntry->prtConsoleDisplayBufferText, sizeof(pEntry->prtConsoleDisplayBufferText));
		}
	}

	return TRUE;
}

BOOL CSnmpMonitorHelper::GetConsoleLight()
{
	return FALSE;
}

BOOL CSnmpMonitorHelper::GetAlert()
{
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertIndex
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertSeverityLevel
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTrainingLevel
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroup
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroupIndex
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertLocation
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertDescription
	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTime

	CCriticalSection2::Owner lock(m_cs4AlertMap);

	char *cOidBegin = CStringToChar(m_szPrtAlertSeverityLevelOID);	//"printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertSeverityLevel"
	char *cOidCurrent = cOidBegin;
	char pszValue[128] = {0};
	char pszOidNext[128] = {0};
	while (TRUE) 
	{
		if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
			&& OidBeginWithStr(pszOidNext, cOidBegin))
		{
			cOidCurrent = pszOidNext;
			int nIndex = GetOidEndNumber(cOidCurrent);
			if (m_oAlertMap.find(nIndex) == m_oAlertMap.end())
			{
				PPrtAlertEntry pEntry = new PrtAlertEntry;
				memset(pEntry, 0x0, sizeof(PrtAlertEntry));
				m_oAlertMap.insert(pair<int,PPrtAlertEntry>(nIndex, pEntry));
				pEntry->prtAlertIndex = nIndex;
			}
		}
		else
		{
			break;
		}
	}

	PrtAlertEntryMap::iterator it;
	for (it=m_oAlertMap.begin(); it!=m_oAlertMap.end(); it++)
	{
		PPrtAlertEntry pEntry = it->second;
		if (pEntry)
		{
			int nIndex = pEntry->prtAlertIndex;

			char cOidStr[128] = {0};
			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertSeverityLevelOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertSeverityLevel
			GetRequest(cOidStr, (int&)pEntry->prtAlertSeverityLevel);

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertTrainingLevelOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTrainingLevel
			GetRequest(cOidStr, (int&)pEntry->prtAlertTrainingLevel);

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertGroupOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroup
			GetRequest(cOidStr, (int&)pEntry->prtAlertGroup);

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertGroupIndexOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertGroupIndex
			GetRequest(cOidStr, pEntry->prtAlertGroupIndex);

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertLocationOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertLocation
			GetRequest(cOidStr, pEntry->prtAlertLocation);

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertCodeOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertCode
			GetRequest(cOidStr, (int&)pEntry->prtAlertCode);

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertDescriptionOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertDescription
			GetRequest(cOidStr, pEntry->prtAlertDescription, sizeof(pEntry->prtAlertDescription));

			sprintf(cOidStr, "%s.1.%d", m_szPrtAlertTimeOID, nIndex);	//printmib.prtAlert.prtAlertTable.prtAlertEntry.prtAlertTime
			GetRequest(cOidStr, pEntry->prtAlertTime);
		}
	}

	return TRUE;
}


static ENUM_STRUCT_TYPE(HrDeviceTypes) GetDeviceTypes(CString szOid)
{
    ENUM_STRUCT_TYPE(HrDeviceTypes) type = ENUM_STRUCT_VALUE(HrDeviceTypes)::hrDeviceOther;

    //HrDeviceTypes是【.1.3.6.1.2.1.25.3.1.x】中的最后一位【x】
    CStringArray ary;
    CCommonFun::StringSplit(szOid, &ary, ".", TRUE);
    if (ary.GetCount() == 11)
    {
        type = (ENUM_STRUCT_TYPE(HrDeviceTypes))atoi(ary.GetAt(10));
    }

    return type;
}

BOOL CSnmpMonitorHelper::GetPrinterTable()
{
    CCriticalSection2::Owner lock(m_cs4PrinterMap);

    // 先获得设备信息
    memset(&m_oDeviceEntryOfPrinter, 0, sizeof(HrDeviceEntry));
    m_oDeviceEntryOfPrinter.hrDeviceType = ENUM_STRUCT_VALUE(HrDeviceTypes)::hrDeviceOther;


    char *cOidBegin = CStringToChar(m_szHrDeviceEntryOID);
    char *cOidCurrent = cOidBegin;
    char pszValue[128] = {0};
    char pszOidNext[128] = {0};
    char cOidStr[128] = {0};
    std::string sVal;
    while (TRUE) 
    {
        if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
            && OidBeginWithStr(pszOidNext, cOidBegin))
        {
            cOidCurrent = pszOidNext;
            const int nIndex = GetOidEndNumber(cOidCurrent);


            sprintf(cOidStr, "%s.2.%d", m_szHrDeviceEntryOID, nIndex);	//host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceTypeOID
            if (GetRequest(cOidStr, sVal))
            {// 如果存在这个字段, 则判断是否是打印机
                const ENUM_STRUCT_TYPE(HrDeviceTypes) hrDeviceType = GetDeviceTypes(sVal.c_str());
                theLog.Write("index = %d, device type = %d", nIndex, hrDeviceType);
                if (ENUM_STRUCT_VALUE(HrDeviceTypes)::hrDevicePrinter != hrDeviceType)
                    continue;
                
            }

            m_oDeviceEntryOfPrinter.hrDeviceType = ENUM_STRUCT_VALUE(HrDeviceTypes)::hrDevicePrinter;


            sprintf(cOidStr, "%s.3.%d", m_szHrDeviceEntryOID, nIndex);	//host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceDescr
            GetRequest(cOidStr, sVal);
            strncpy((char*)m_oDeviceEntryOfPrinter.hrDeviceDescr, sVal.c_str(), sizeof(m_oDeviceEntryOfPrinter.hrDeviceID));

            sprintf(cOidStr, "%s.4.%d", m_szHrDeviceEntryOID, nIndex);		//host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceID
            GetRequest(cOidStr, sVal);
            strncpy((char*)m_oDeviceEntryOfPrinter.hrDeviceID, sVal.c_str(), sizeof(m_oDeviceEntryOfPrinter.hrDeviceID));

            sprintf(cOidStr, "%s.5.%d", m_szHrDeviceEntryOID, nIndex);	//host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceStatus
            GetRequest(cOidStr, (int&)m_oDeviceEntryOfPrinter.hrDeviceStatus);

            sprintf(cOidStr, "%s.6.%d", m_szHrDeviceEntryOID, nIndex);	//host.hrDevice.hrDeviceTable.hrDeviceEntry.hrDeviceErrors
            GetRequest(cOidStr, (int&)m_oDeviceEntryOfPrinter.hrDeviceErrors);

            break;
        }
        else
        {
            break;
        }
    }

    if (m_oDeviceEntryOfPrinter.hrDeviceType == ENUM_STRUCT_VALUE(HrDeviceTypes)::hrDeviceOther)
        return FALSE;


    cOidBegin = CStringToChar(m_szHrPrinterEntryOID);	//"host.hrDevice.hrPrinterTable.hrPrinterEntry.hrPrinterStatus"
    cOidCurrent = cOidBegin;
    while (TRUE) 
    {
        if (GetNextRequestStrEx(cOidCurrent, pszValue, sizeof(pszValue), pszOidNext, sizeof(pszOidNext))
            && OidBeginWithStr(pszOidNext, cOidBegin))
        {
            cOidCurrent = pszOidNext;
            int nIndex = GetOidEndNumber(cOidCurrent);
            if (m_oPrinterMap.find(nIndex) == m_oPrinterMap.end())
            {
                PHrPrinterEntry pEntry = new HrPrinterEntry;
                memset(pEntry, 0x0, sizeof(HrPrinterEntry));
                m_oPrinterMap.insert(pair<int,PHrPrinterEntry>(nIndex, pEntry));
                pEntry->hrPrinterStatus = (ENUM_STRUCT_TYPE(HrPrinterStatus))nIndex;
            }
        }
        else
        {
            break;
        }
    }

    HrPrinterEntryMap::iterator it;
    for (it=m_oPrinterMap.begin(); it!=m_oPrinterMap.end(); it++)
    {
        PHrPrinterEntry pEntry = it->second;
        if (pEntry)
        {
            int nIndex = pEntry->hrPrinterStatus;

            char cOidStr[128] = {0};
            sprintf(cOidStr, "%s.1.%d", m_szHrPrinterEntryOID, nIndex);	//host.hrDevice.hrPrinterTable.hrPrinterEntry.hrPrinterStatus
            GetRequest(cOidStr, (int&)pEntry->hrPrinterStatus);

            sprintf(cOidStr, "%s.2.%d", m_szHrPrinterEntryOID, nIndex);	//host.hrDevice.hrPrinterTable.hrPrinterEntry.hrPrinterDetectedErrorState
            GetRequest(cOidStr, (unsigned char*)cOidStr, 2);
            pEntry->hrPrinterDetectedErrorState = (BYTE)cOidStr[0];
        }
    }

    return TRUE;
}

BOOL CSnmpMonitorHelper::CheckPrinterTable()
{
    CCriticalSection2::Owner lock(m_cs4PrinterMap);

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
            case 0x00:
                szPrinterDesc = _T("少纸预警");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperLow, CStringToChar(szPrinterDesc));
                break;
            case 0x01:
                szPrinterDesc = _T("缺纸错误");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperEmpty, CStringToChar(szPrinterDesc));
                break;
            case 0x02:
                szPrinterDesc = _T("碳粉低");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerLow, CStringToChar(szPrinterDesc));
                break;
            case 0x03:
                szPrinterDesc = _T("碳粉用尽");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerEmpty, CStringToChar(szPrinterDesc));
                break;
            case 0x04:
                szPrinterDesc = _T("盖门打开");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::CoverOpen, CStringToChar(szPrinterDesc));
                break;
            case 0x05:
                szPrinterDesc = _T("卡纸错误");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::Jam, CStringToChar(szPrinterDesc));
                break;
            case 0x06:
                szPrinterDesc = _T("离线错误");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::Other, CStringToChar(szPrinterDesc));
                break;
            case 0x07:
                szPrinterDesc = _T("服务请求预警");
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::Other, CStringToChar(szPrinterDesc));
                break;
            default:
                szPrinterDesc.Format(_T("未知错误, 错误码: %d"), pEntry->hrPrinterDetectedErrorState);
                m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::Other, CStringToChar(szPrinterDesc));
                break;
            }
        }
    }

    return TRUE;
    
}
BOOL CSnmpMonitorHelper::ClearPrinterTable()
{
    CCriticalSection2::Owner lock(m_cs4PrinterMap);

    HrPrinterEntryMap::iterator it;
    for (it=m_oPrinterMap.begin(); it!=m_oPrinterMap.end(); it++)
    {
        PHrPrinterEntry pEntry = it->second;
        if (pEntry)
        {
            delete pEntry;
            pEntry = NULL;
        }
    }
    m_oPrinterMap.clear();

    return TRUE;
}

//检查SNMP获取到的OID信息
void CSnmpMonitorHelper::CheckAll()
{
    CheckPrinterTable();
	CheckGeneral();
	CheckStorageRef();
	CheckDeviceRef();
	CheckCover();
	CheckLocalization();
	CheckInput();
	CheckOutput();
	CheckMarker();
	CheckMarkerSupplies();
	CheckMarkerColorant();
	CheckMediaPath();
	CheckChannel();
	CheckInterpreter();
	CheckConsoleDisplayBuffer();
	CheckConsoleLight();
	CheckAlert();

	//扩展功能检测
	CheckMeter();	//抄表
	CheckAlertEx();	//预警扩展
	CheckFaultCode();	//单独的故障代码检测，从打印机的私有节点获取
}

void CSnmpMonitorHelper::CheckGeneral()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckStorageRef()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckDeviceRef()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckCover()
{
	CCriticalSection2::Owner lock(m_cs4CoverMap);

	CString szDesc = "";
	CString szCoverDesc = "";
	CString szInterLockDesc = "";
	CString szOtherDesc = "";
	PrtCoverEntryMap::iterator it;
	for (it=m_oCoverMap.begin(); it!=m_oCoverMap.end(); it++)
	{
		PPrtCoverEntry pEntry = it->second;
		if (pEntry)
		{
			int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::UnKnown;
			if (pEntry->prtCoverStatus == ENUM_STRUCT_VALUE(PrtCoverStatusTC)::CoverOpen)
			{
				szCoverDesc.AppendFormat("%s,", CCommonFun::UTF8ToMultiByte(pEntry->prtCoverDescription));
			}
			else if (pEntry->prtCoverStatus == ENUM_STRUCT_VALUE(PrtCoverStatusTC)::InterlockOpen)
			{
				szInterLockDesc.AppendFormat("%s,", CCommonFun::UTF8ToMultiByte(pEntry->prtCoverDescription));
			}
			else if (pEntry->prtCoverStatus == ENUM_STRUCT_VALUE(PrtCoverStatusTC)::Other)
			{
				szOtherDesc.AppendFormat("%s,", CCommonFun::UTF8ToMultiByte(pEntry->prtCoverDescription));
			}
		}
	}
	szCoverDesc.TrimRight(",");
	szInterLockDesc.TrimRight(",");
	szOtherDesc.TrimRight(",");
	if (szCoverDesc.GetLength()>0 && m_pCallBack)
	{
		szDesc.Format("盖板(%s)被打开！", szCoverDesc);
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::MultipleGroupsCoverOpen, CStringToChar(szDesc));
	}
	if (szInterLockDesc.GetLength()>0 && m_pCallBack)
	{
		szDesc.Format("连锁装置(%s)被打开！", szInterLockDesc);
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::MultipleGroupsInterlockOpen, CStringToChar(szDesc));
	}
	if (szOtherDesc.GetLength()>0 && m_pCallBack)
	{
		szDesc.Format("盖板(%s)状态异常！", szOtherDesc);
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::MultipleGroupsOther, CStringToChar(szDesc));
	}
}

void CSnmpMonitorHelper::CheckLocalization()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckInput()
{
	CCriticalSection2::Owner lock(m_cs4InputMap);

	PrtInputEntryMap::iterator it;
	for (it=m_oInputMap.begin(); it!=m_oInputMap.end(); it++)
	{
		PPrtInputEntry pEntry = it->second;
		if (pEntry)
		{
			//theLog.Write("====CSnmpMonitorHelper::CheckInput,m_eType=%d,prtInputType=%d,prtInputMaxCapacity=%d,prtInputCurrentLevel=%d, prtInputName=%s,m_pCallBack=%p"
			//	, m_eType, pEntry->prtInputType, pEntry->prtInputMaxCapacity, pEntry->prtInputCurrentLevel, pEntry->prtInputName, m_pCallBack);
			/*
			if (m_eType == BT_LM || m_eType == BT_LG)
			{
				if (pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray)
				{
					continue;
				}
			}
			else if (m_eType == BT_XP || m_eType == BT_SX)
			{
				if (pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray
					&& pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray)
				{
					continue;
				}
			}
			else
			{
				if (pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray)
				{
					continue;
				}
			}
			*/
			if (pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoNonRemovableTray
				&& pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::SheetFeedAutoRemovableTray
				&& pEntry->prtInputType != ENUM_STRUCT_VALUE(PrtInputTypeTC)::ContinuousRoll)
			{
				continue;
			}

			if (pEntry->prtInputMaxCapacity <= 0 || pEntry->prtInputCurrentLevel < 0)
			{
				continue;
			}

			int nDiff = 0;
			CString szInputName(pEntry->prtInputName);
			if (szInputName.Find("1") >= 0)
			{
				nDiff = 10;
			}
			else if (szInputName.Find("2") >= 0)
			{
				nDiff = 8;
			}
			else if (szInputName.Find("3") >= 0)
			{
				nDiff = 6;
			}
			else if (szInputName.Find("4") >= 0)
			{
				nDiff = 4;
			}
			else
			{
#if 0
				CString szTemp = szInputName;
				szTemp.MakeUpper();
				if (szTemp.Find("MP TRAY") >= 0)
				{
					continue;
				}
				else
				{
					nDiff = 0;
				}
#else
				continue;
#endif
			}

			int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::UnKnown;
			CString szDesc = "";
			double fRatio = 1.0 * pEntry->prtInputCurrentLevel / pEntry->prtInputMaxCapacity;
			int nRatio = (int)(fRatio * 100);
			if (nRatio == 0)
			{
				nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperEmpty - nDiff;
				szDesc.Format("纸盒(%s)，纸型(%s)，无纸！", szInputName, GetPaperTypeByPrtInputEntry(pEntry));
			}
			else if (nRatio <= m_nPaperMinRatio)
			{
				nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TaryPaperLow - nDiff;
				szDesc.Format("纸盒(%s)，纸型(%s)，少纸(剩余%d%%)，请准备备用纸！"
					, szInputName, GetPaperTypeByPrtInputEntry(pEntry), nRatio);
			}

			//theLog.Write("========nRatio=%d, szDesc=%s", nRatio, szDesc);
			if (szDesc.GetLength()>0 && m_pCallBack)
			{
				if (nAlertCode < 100)
				{
					nAlertCode += 100;	//参考:【PrinterAlert_Def.h】中的【PrinterAlertCode】信安宝扩展多组预警
				}
				m_pCallBack->OnStatus(nAlertCode, CStringToChar(szDesc));
			}
		}
	}
}

void CSnmpMonitorHelper::CheckOutput()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckMarker()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckMarkerSupplies()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			//theLog.Write("CSnmpMonitorHelper::CheckMarkerSupplies,1,prtMarkerSuppliesType=%d,prtMarkerSuppliesMaxCapacity=%d,prtMarkerSuppliesLevel=%d"
			//	, pEntry->prtMarkerSuppliesType, pEntry->prtMarkerSuppliesMaxCapacity, pEntry->prtMarkerSuppliesLevel);
			if (pEntry->prtMarkerSuppliesType != ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner
				&& pEntry->prtMarkerSuppliesType != ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
				&& pEntry->prtMarkerSuppliesType != ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
				&& pEntry->prtMarkerSuppliesType != ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
				&& pEntry->prtMarkerSuppliesType != ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC
				&& pEntry->prtMarkerSuppliesType != ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
			{
				#pragma message("!!CSnmpMonitorHelper::CheckMarkerSupplies,暂时只处理Toner、Ink、OPC、Developer，其它耗材不处理。")
				continue;
			}

			if (pEntry->prtMarkerSuppliesMaxCapacity <= 0 || pEntry->prtMarkerSuppliesLevel < 0)
			{
				continue;
			}

			int nDiff = 0;
			CString szMarkerSuppliesDescription = CCommonFun::UTF8ToMultiByte(pEntry->prtMarkerSuppliesDescription);
			szMarkerSuppliesDescription.MakeLower();
			if (szMarkerSuppliesDescription.Find("cyan") >= 0 || szMarkerSuppliesDescription.Find("青色") >= 0)
			{
				nDiff = 8;
			}
			else if (szMarkerSuppliesDescription.Find("magenta") >= 0 || szMarkerSuppliesDescription.Find("品红色") >= 0)
			{
				nDiff = 6;
			}
			else if (szMarkerSuppliesDescription.Find("yellow") >= 0 || szMarkerSuppliesDescription.Find("黄色") >= 0)
			{
				nDiff = 4;
			}
			else //if (szMarkerSuppliesDescription.Find("black") >= 0)	//这个分支中，可能出现汉字，如利盟机器的[黑色碳粉]、[成像部件]、[维护工具包]等等
			{
				nDiff = 2;
			}

			int nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::UnKnown;
			CString szDesc = "";
			double fRatio = 1.0 * pEntry->prtMarkerSuppliesLevel / pEntry->prtMarkerSuppliesMaxCapacity;
			int nRatio = (int)(fRatio * 100);
			if (nRatio == 0)
			{
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerEmpty - nDiff;
					szDesc.Format("粉盒(%s)，无粉！", szMarkerSuppliesDescription);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::InkEmpty - nDiff;
					szDesc.Format("墨盒(%s)，无墨！", szMarkerSuppliesDescription);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::OPCEmpty - nDiff;
					szDesc.Format("感光鼓单元(%s)，使用寿命耗尽！", szMarkerSuppliesDescription);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::DeveloperEmpty - nDiff;
					szDesc.Format("显影单元(%s)，使用寿命耗尽！", szMarkerSuppliesDescription);
				}
			} 
			else if (nRatio <= m_nInkMinRatio)
			{
				if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Toner)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::TonerLow - nDiff;
					szDesc.Format("粉盒(%s)，少粉(余量%d%%)，请准备备用粉盒！", szMarkerSuppliesDescription, nRatio);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Ink
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkCartridge
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::InkRibbon
					|| pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::TonerCartridge)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::InkLow - nDiff;
					szDesc.Format("墨盒(%s)，少墨(余量%d%%)，请准备备用墨盒！", szMarkerSuppliesDescription, nRatio);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::OPC)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::OPCLow - nDiff;
					szDesc.Format("感光鼓单元(%s)，使用寿命低(余量%d%%)，请准备备用感光鼓！", szMarkerSuppliesDescription, nRatio);
				}
				else if (pEntry->prtMarkerSuppliesType == ENUM_STRUCT_VALUE(PrtMarkerSuppliesTypeTC)::Developer)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::DeveloperLow - nDiff;
					szDesc.Format("显影单元(%s)，使用寿命低(余量%d%%)，请准备备用显影！", szMarkerSuppliesDescription, nRatio);
				}
			}

			if (szDesc.GetLength()>0 && m_pCallBack)
			{
				if (nAlertCode < 100)
				{
					nAlertCode += 100;	//参考:【PrinterAlert_Def.h】中的【PrinterAlertCode】信安宝扩展多组预警
				}
				m_pCallBack->OnStatus(nAlertCode, CStringToChar(szDesc));
			}
		}
	}
}

void CSnmpMonitorHelper::CheckMarkerColorant()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckMediaPath()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckChannel()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckInterpreter()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckConsoleDisplayBuffer()
{
	CCriticalSection2::Owner lock(m_cs4ConsoleDisplayBufferMap);

	CString szPrtConsoleDisplayBufferText;
	PrtConsoleDisplayBufferEntryMap::iterator it;
	for (it=m_oConsoleDisplayBufferMap.begin(); it!=m_oConsoleDisplayBufferMap.end(); it++)
	{
		PPrtConsoleDisplayBufferEntry pEntry = it->second;
		if (pEntry)
		{
			//theLog.Write("CSnmpMonitorHelper::CheckConsoleDisplayBuffer,1,prtConsoleDisplayBufferIndex=%d,prtConsoleDisplayBufferText=%s"
			//	, pEntry->prtConsoleDisplayBufferIndex, pEntry->prtConsoleDisplayBufferText);

			//多条控制面板信息合并成一条，并且多条之间换行处理。
			szPrtConsoleDisplayBufferText.AppendFormat("%s\r\n", CCommonFun::UTF8ToMultiByte(pEntry->prtConsoleDisplayBufferText));
		}
	}

	szPrtConsoleDisplayBufferText.TrimRight("\r\n");
	szPrtConsoleDisplayBufferText.Trim();
	if (szPrtConsoleDisplayBufferText.GetLength()>0 && m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::ConsoleDisplayBufferText, CStringToChar(szPrtConsoleDisplayBufferText));
	}
}

void CSnmpMonitorHelper::CheckConsoleLight()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::CheckAlert()
{
	CCriticalSection2::Owner lock(m_cs4AlertMap);

	PrtAlertEntryMap::iterator it;
	for (it=m_oAlertMap.begin(); it!=m_oAlertMap.end(); it++)
	{
		PPrtAlertEntry pEntry = it->second;
		if (pEntry)
		{
#pragma message("!!CSnmpMonitorHelper::CheckAlert,打印机SNMP的预警处理此处的逻辑判断可能会导致预警缺失，目前暂时这么处理，后续再测试验证和优化")
			//纸盒、墨盒、粉盒、感光鼓、显影单元在CheckInput和CheckMarkerSupplies已经处理了一些预警信息，
			//此处处理上述没有处理过的预警信息。
			CString szAlertDescription = CCommonFun::UTF8ToMultiByte(pEntry->prtAlertDescription);
			//theLog.Write("CSnmpMonitorHelper::CheckAlert,prtAlertSeverityLevel=%d,prtAlertTrainingLevel=%d,prtAlertGroup=%d,"
			//	"prtAlertGroupIndex=%d,prtAlertLocation=%d,prtAlertCode=%d,prtAlertDescription=%s,prtAlertTime=%u"
			//	, pEntry->prtAlertSeverityLevel, pEntry->prtAlertTrainingLevel, pEntry->prtAlertGroup
			//	, pEntry->prtAlertGroupIndex, pEntry->prtAlertLocation, pEntry->prtAlertCode, szAlertDescription, pEntry->prtAlertTime);

			//此处一般在CheckCover()接口之后被调用，如果盖板预警已经存在，下面的盖板预警不会重复添加
			if (m_eType == BT_HP || m_eType == BT_CANON)
			{
				//惠普、佳能打印机在CheckCover()中无法获取,此处需要处理盖板打开的预警.
			}
			else
			{
				if (pEntry->prtAlertGroup == ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Cover)
				{
					continue;
				}
			}

			//此处忽略Other和UnKnown两种预警，放到CheckAlertEx()中过滤处理。
			if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other
				|| pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::UnKnown)
			{
				continue;
			}
			
			CString szDesc = "";
			if (pEntry->prtAlertGroup == ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Cover)
			{
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::CoverOpen
					|| pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::InterlockOpen
					|| pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::DoorOpen)
				{
					szDesc.Format("盖板信息：盖板(%s)被打开！", szAlertDescription);
				}
				else
				{
					szDesc.Format("盖板信息：警告类别(%d),警告码(%d),警告详情(%s)！"
						, pEntry->prtAlertGroup, pEntry->prtAlertCode, szAlertDescription);
				}
			}
			else if (pEntry->prtAlertGroup == ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Input)
			{
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::InputMediaSupplyLow
					|| pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::InputMediaSupplyEmpty
					|| pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::SubunitAlmostEmpty
					|| pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::SubunitEmpty)
				{
					//在CheckInput()中已经处理了
					continue;
				}
				else if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::InputMediaTrayMissing)
				{
					szDesc.Format("纸盒信息：纸盒(%s)抽出！", szAlertDescription);
				}
				else if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Jam)
				{
					szDesc.Format("警告：打印机卡纸,警告详情(%s)！", szAlertDescription);
				}
				else
				{
					szDesc.Format("警告信息：警告类别(%d),警告码(%d),警告详情(%s)！"
						, pEntry->prtAlertGroup, pEntry->prtAlertCode, szAlertDescription);
				}
			}
			else if (pEntry->prtAlertGroup == ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Output)
			{
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Jam)
				{
					szDesc.Format("警告：打印机卡纸,警告详情(%s)！", szAlertDescription);
				}
				else
				{
					szDesc.Format("警告信息：警告类别(%d),警告码(%d),警告详情(%s)！"
						, pEntry->prtAlertGroup, pEntry->prtAlertCode, szAlertDescription);
				}
			}
			else if (pEntry->prtAlertGroup == ENUM_STRUCT_VALUE(PrtAlertGroupTC)::MediaPath)
			{
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Jam)
				{
					szDesc.Format("警告：打印机卡纸,警告详情(%s)！", szAlertDescription);
				}
				else if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::SubunitMissing)
				{
					szDesc.Format("纸盒信息：纸盒(%s)抽出！", szAlertDescription);
				}
				else if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::MediaPathMediaTrayMissing)
				{
					szDesc.Format("纸盒信息：纸盒(%s)抽出！", szAlertDescription);
				}
				else
				{
					szDesc.Format("警告信息：警告类别(%d),警告码(%d),警告详情(%s)！"
						, pEntry->prtAlertGroup, pEntry->prtAlertCode, szAlertDescription);
				}
			}
			else if (pEntry->prtAlertGroup == ENUM_STRUCT_VALUE(PrtAlertGroupTC)::GeneralPrinter)
			{
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Jam)
				{
					szDesc.Format("警告：打印机卡纸,警告详情(%s)！", szAlertDescription);
				}
				else
				{
					szDesc.Format("警告信息：警告类别(%d),警告码(%d),警告详情(%s)！"
						, pEntry->prtAlertGroup, pEntry->prtAlertCode, szAlertDescription);
				}
			}
			else
			{
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::CoverOpen)
				{
					//代码走到此处，一般为预警信息不是盖板类(ENUM_STRUCT_VALUE(PrtAlertGroupTC)::Cover)，
					//但是是某个装置被打开了，手动将预警码设置为InterlockOpen，避免遗漏重要预警，
					//而又不与盖板打开预警(ENUM_STRUCT_VALUE(PrtAlertCodeTC)::CoverOpen)冲突。
					pEntry->prtAlertCode = ENUM_STRUCT_VALUE(PrtAlertCodeTC)::InterlockOpen;
				}

                if (!szAlertDescription.IsEmpty())
				    szDesc.Format("警告信息：警告类别(%d),警告码(%d),警告详情(%s)！", pEntry->prtAlertGroup, pEntry->prtAlertCode, szAlertDescription);
			}

			if (szDesc.GetLength()>0 && m_pCallBack)
			{
				int nAlertCode = pEntry->prtAlertCode;
				if (nAlertCode <= 0)
				{
					nAlertCode = ENUM_STRUCT_VALUE(PrinterAlertCode)::MultipleGroupsUnKnown;
				}
				if (nAlertCode < 100)
				{
					nAlertCode += 100;	//参考:【PrinterAlert_Def.h】中的【PrinterAlertCode】信安宝扩展多组预警
				}
				m_pCallBack->OnStatus(nAlertCode, CStringToChar(szDesc));
			}
		}
	}
}

void CSnmpMonitorHelper::CheckMeter()
{
	if (!m_pPrinterMeterMonitor)
	{
		return;
	}

	{
		CCriticalSection2::Owner lock(m_cs4InputMap);
		m_pPrinterMeterMonitor->SetMeterInfo(m_oInputMap);
	}
	{
		CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);
		m_pPrinterMeterMonitor->SetMeterInfo(m_oMarkerSuppliesMap);
	}
}

void CSnmpMonitorHelper::CheckAlertEx()
{
	CCriticalSection2::Owner lock(m_cs4AlertMap);

	CString szFault = "";
	CString szOther = "";
	CString szUnKnow = "";
	PrtAlertEntryMap::iterator it;
	for (it=m_oAlertMap.begin(); it!=m_oAlertMap.end(); it++)
	{
		PPrtAlertEntry pEntry = it->second;
		if (pEntry)
		{
			CString szAlertDescription = CCommonFun::UTF8ToMultiByte(pEntry->prtAlertDescription);
			//theLog.Write("CSnmpMonitorHelper::CheckFaultCode,prtAlertSeverityLevel=%d,prtAlertTrainingLevel=%d,prtAlertGroup=%d,"
			//	"prtAlertGroupIndex=%d,prtAlertLocation=%d,prtAlertCode=%d,prtAlertDescription=%s,prtAlertTime=%u"
			//	, pEntry->prtAlertSeverityLevel, pEntry->prtAlertTrainingLevel, pEntry->prtAlertGroup
			//	, pEntry->prtAlertGroupIndex, pEntry->prtAlertLocation, pEntry->prtAlertCode, szAlertDescription, pEntry->prtAlertTime);

			//此处只处理Other和UnKnown两种预警，其它的在CheckAlert()中过滤处理。
			if (pEntry->prtAlertCode != ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other
				&& pEntry->prtAlertCode != ENUM_STRUCT_VALUE(PrtAlertCodeTC)::UnKnown)
			{
				continue;
			}

			if (IsFalutInfo(szAlertDescription))
			{
				szFault.AppendFormat("%s\r\n", szAlertDescription);
			}
			else
			{
				//多条预警信息合并成一条，并且多条之间换行处理。
				if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::Other)
				{
					szOther.AppendFormat("%s\r\n", szAlertDescription);
				}
				else if (pEntry->prtAlertCode == ENUM_STRUCT_VALUE(PrtAlertCodeTC)::UnKnown)
				{
					szUnKnow.AppendFormat("%s\r\n", szAlertDescription);
				}
			}
		}
	}

	szFault.TrimRight("\r\n");
	szFault.Trim();
	if (szFault.GetLength()>0 && m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::PrinterFaultInfo, CStringToChar(szFault));
	}

	szOther.TrimRight("\r\n");
	szOther.Trim();
	if (szOther.GetLength()>0 && m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::MultipleGroupsOther, CStringToChar(szOther));
	}

	szUnKnow.TrimRight("\r\n");
	szUnKnow.Trim();
	if (szUnKnow.GetLength()>0 && m_pCallBack)
	{
		m_pCallBack->OnStatus(ENUM_STRUCT_VALUE(PrinterAlertCode)::MultipleGroupsUnKnown, CStringToChar(szUnKnow));
	}
}

//清理SNMP获取到的OID信息
void CSnmpMonitorHelper::ClearAll()
{
    ClearPrinterTable();
	ClearGeneral();
	ClearStorageRef();
	ClearDeviceRef();
	ClearCover();
	ClearLocalization();
	ClearInput();
	ClearOutput();
	ClearMarker();
	ClearMarkerSupplies();
	ClearMarkerColorant();
	ClearMediaPath();
	ClearChannel();
	ClearInterpreter();
	ClearConsoleDisplayBuffer();
	ClearConsoleLight();
	ClearAlert();
}

void CSnmpMonitorHelper::ClearGeneral()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearStorageRef()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearDeviceRef()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearCover()
{
	CCriticalSection2::Owner lock(m_cs4CoverMap);

	PrtCoverEntryMap::iterator it;
	for (it=m_oCoverMap.begin(); it!=m_oCoverMap.end(); it++)
	{
		PPrtCoverEntry pEntry = it->second;
		if (pEntry)
		{
			delete pEntry;
			pEntry = NULL;
		}
	}
	m_oCoverMap.clear();
}

void CSnmpMonitorHelper::ClearLocalization()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearInput()
{
	CCriticalSection2::Owner lock(m_cs4InputMap);

	PrtInputEntryMap::iterator it;
	for (it=m_oInputMap.begin(); it!=m_oInputMap.end(); it++)
	{
		PPrtInputEntry pEntry = it->second;
		if (pEntry)
		{
			delete pEntry;
			pEntry = NULL;
		}
	}
	m_oInputMap.clear();
}

void CSnmpMonitorHelper::ClearOutput()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearMarker()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearMarkerSupplies()
{
	CCriticalSection2::Owner lock(m_cs4MarkerSuppliesMap);

	PrtMarkerSuppliesEntryMap::iterator it;
	for (it=m_oMarkerSuppliesMap.begin(); it!=m_oMarkerSuppliesMap.end(); it++)
	{
		PPrtMarkerSuppliesEntry pEntry = it->second;
		if (pEntry)
		{
			delete pEntry;
			pEntry = NULL;
		}
	}
	m_oMarkerSuppliesMap.clear();
}

void CSnmpMonitorHelper::ClearMarkerColorant()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearMediaPath()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearChannel()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearInterpreter()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearConsoleDisplayBuffer()
{
	CCriticalSection2::Owner lock(m_cs4ConsoleDisplayBufferMap);

	PrtConsoleDisplayBufferEntryMap::iterator it;
	for (it=m_oConsoleDisplayBufferMap.begin(); it!=m_oConsoleDisplayBufferMap.end(); it++)
	{
		PPrtConsoleDisplayBufferEntry pEntry = it->second;
		if (pEntry)
		{
			delete pEntry;
			pEntry = NULL;
		}
	}
	m_oConsoleDisplayBufferMap.clear();
}

void CSnmpMonitorHelper::ClearConsoleLight()
{
#pragma message("CSnmpMonitorHelper::XX 暂未实现")
}

void CSnmpMonitorHelper::ClearAlert()
{
	CCriticalSection2::Owner lock(m_cs4AlertMap);

	PrtAlertEntryMap::iterator it;
	for (it=m_oAlertMap.begin(); it!=m_oAlertMap.end(); it++)
	{
		PPrtAlertEntry pEntry = it->second;
		if (pEntry)
		{
			delete pEntry;
			pEntry = NULL;
		}
	}
	m_oAlertMap.clear();
}

void CSnmpMonitorHelper::InitOID()
{
    m_szHrDeviceEntryOID = DecryptOID(HrDeviceEntryOID);
    m_szHrPrinterEntryOID = DecryptOID(HrPrinterEntryOID);

	//printmib.prtGeneral.prtGeneralTable.prtGeneralEntry
	m_szPrtGeneralEntryOID = DecryptOID(PrtGeneralEntryOID);
	m_szPrtGeneralConfigChangesOID = DecryptOID(PrtGeneralConfigChangesOID);
	m_szPrtGeneralCurrentLocalizationOID = DecryptOID(PrtGeneralCurrentLocalizationOID);
	m_szPrtGeneralResetOID = DecryptOID(PrtGeneralResetOID);
	m_szPrtGeneralCurrentOperatorOID = DecryptOID(PrtGeneralCurrentOperatorOID);
	m_szPrtGeneralServicePersonOID = DecryptOID(PrtGeneralServicePersonOID);
	m_szPrtInputDefaultIndexOID = DecryptOID(PrtInputDefaultIndexOID);
	m_szPrtOutputDefaultIndexOID = DecryptOID(PrtOutputDefaultIndexOID);
	m_szPrtMarkerDefaultIndexOID = DecryptOID(PrtMarkerDefaultIndexOID);
	m_szPrtMediaPathDefaultIndexOID = DecryptOID(PrtMediaPathDefaultIndexOID);
	m_szPrtConsoleLocalizationOID = DecryptOID(PrtConsoleLocalizationOID);
	m_szPrtConsoleNumberOfDisplayLinesOID = DecryptOID(PrtConsoleNumberOfDisplayLinesOID);
	m_szPrtConsoleNumberOfDisplayCharsOID = DecryptOID(PrtConsoleNumberOfDisplayCharsOID);
	m_szPrtConsoleDisableOID = DecryptOID(PrtConsoleDisableOID);
#if 0
	m_szPrtAuxiliarySheetStartupPageOID = DecryptOID(PrtAuxiliarySheetStartupPageOID);
	m_szPrtAuxiliarySheetBannerPageOID = DecryptOID(PrtAuxiliarySheetBannerPageOID);
	m_szPrtGeneralPrinterNameOID = DecryptOID(PrtGeneralPrinterNameOID);
	m_szPrtGeneralSerialNumberOID = DecryptOID(PrtGeneralSerialNumberOID);
	m_szPrtAlertCriticalEventsOID = DecryptOID(PrtAlertCriticalEventsOID);
	m_szPrtAlertAllEventsOID = DecryptOID(PrtAlertAllEventsOID);
#else
	m_szPrtAuxiliarySheetStartupPageOID.Format("%s.14", m_szPrtGeneralEntryOID);
	m_szPrtAuxiliarySheetBannerPageOID.Format("%s.15", m_szPrtGeneralEntryOID);
	m_szPrtGeneralPrinterNameOID.Format("%s.16", m_szPrtGeneralEntryOID);
	m_szPrtGeneralSerialNumberOID.Format("%s.17", m_szPrtGeneralEntryOID);
	m_szPrtAlertCriticalEventsOID.Format("%s.18", m_szPrtGeneralEntryOID);
	m_szPrtAlertAllEventsOID.Format("%s.19", m_szPrtGeneralEntryOID);
#endif

	//printmib.prtCover.prtCoverTable.prtCoverEntry
	m_szPrtCoverEntryOID = DecryptOID(PrtCoverEntryOID);
	m_szPrtCoverIndexOID = DecryptOID(PrtCoverIndexOID);
	m_szPrtCoverDescriptionOID = DecryptOID(PrtCoverDescriptionOID);
	m_szPrtCoverStatusOID = DecryptOID(PrtCoverStatusOID);

	//printmib.prtLocalization.prtLocalizationTable.prtLocalizationEntry
	m_szPrtLocalizationEntryOID = DecryptOID(PrtLocalizationEntryOID);
	m_szPrtLocalizationIndexOID = DecryptOID(PrtLocalizationIndexOID);
	m_szPrtLocalizationLanguageOID = DecryptOID(PrtLocalizationLanguageOID);
	m_szPrtLocalizationCountryOID = DecryptOID(PrtLocalizationCountryOID);
	m_szPrtLocalizationCharacterSetOID = DecryptOID(PrtLocalizationCharacterSetOID);

	//printmib.prtInput.prtInputTable.prtInputEntry
	m_szPrtInputEntryOID = DecryptOID(PrtInputEntryOID);
	m_szPrtInputIndexOID = DecryptOID(PrtInputIndexOID);
	m_szPrtInputTypeOID = DecryptOID(PrtInputTypeOID);
	m_szPrtInputDimUnitOID = DecryptOID(PrtInputDimUnitOID);
	m_szPrtInputMediaDimFeedDirDeclaredOID = DecryptOID(PrtInputMediaDimFeedDirDeclaredOID);
	m_szPrtInputMediaDimXFeedDirDeclaredOID = DecryptOID(PrtInputMediaDimXFeedDirDeclaredOID);
	m_szPrtInputMediaDimFeedDirChosenOID = DecryptOID(PrtInputMediaDimFeedDirChosenOID);
	m_szPrtInputMediaDimXFeedDirChosenOID = DecryptOID(PrtInputMediaDimXFeedDirChosenOID);
	m_szPrtInputCapacityUnitOID = DecryptOID(PrtInputCapacityUnitOID);
	m_szPrtInputMaxCapacityOID = DecryptOID(PrtInputMaxCapacityOID);
	m_szPrtInputCurrentLevelOID = DecryptOID(PrtInputCurrentLevelOID);
	m_szPrtInputStatusOID = DecryptOID(PrtInputStatusOID);
	m_szPrtInputMediaNameOID = DecryptOID(PrtInputMediaNameOID);
	m_szPrtInputNameOID = DecryptOID(PrtInputNameOID);
	m_szPrtInputVendorNameOID = DecryptOID(PrtInputVendorNameOID);
	m_szPrtInputModelOID = DecryptOID(PrtInputModelOID);
	m_szPrtInputVersionOID = DecryptOID(PrtInputVersionOID);
	m_szPrtInputSerialNumberOID = DecryptOID(PrtInputSerialNumberOID);
	m_szPrtInputDescriptionOID = DecryptOID(PrtInputDescriptionOID);
	m_szPrtInputSecurityOID = DecryptOID(PrtInputSecurityOID);
	m_szPrtInputMediaWeightOID = DecryptOID(PrtInputMediaWeightOID);
	m_szPrtInputMediaTypeOID = DecryptOID(PrtInputMediaTypeOID);
	m_szPrtInputMediaColorOID = DecryptOID(PrtInputMediaColorOID);
	m_szPrtInputMediaFormPartsOID = DecryptOID(PrtInputMediaFormPartsOID);
#ifdef RFC_3805
#if 0
	m_szPrtInputMediaLoadTimeoutOID = DecryptOID(PrtInputMediaLoadTimeoutOID);
	m_szPrtInputNextIndexOID = DecryptOID(PrtInputNextIndexOID);
#else
	m_szPrtInputMediaLoadTimeoutOID.Format("%s.24", m_szPrtInputEntryOID);
	m_szPrtInputNextIndexOID.Format("%s.25", m_szPrtInputEntryOID);
#endif
#endif

	//printmib.prtOutput.prtOutputTable.prtOutputEntry
	m_szPrtOutputEntryOID = DecryptOID(PrtOutputEntryOID);
	m_szPrtOutputIndexOID = DecryptOID(PrtOutputIndexOID);
	m_szPrtOutputTypeOID = DecryptOID(PrtOutputTypeOID);
	m_szPrtOutputCapacityUnitOID = DecryptOID(PrtOutputCapacityUnitOID);
	m_szPrtOutputMaxCapacityOID = DecryptOID(PrtOutputMaxCapacityOID);
	m_szPrtOutputRemainingCapacityOID = DecryptOID(PrtOutputRemainingCapacityOID);
	m_szPrtOutputStatusOID = DecryptOID(PrtOutputStatusOID);
	m_szPrtOutputNameOID = DecryptOID(PrtOutputNameOID);
	m_szPrtOutputVendorNameOID = DecryptOID(PrtOutputVendorNameOID);
	m_szPrtOutputModelOID = DecryptOID(PrtOutputModelOID);
	m_szPrtOutputVersionOID = DecryptOID(PrtOutputVersionOID);
	m_szPrtOutputSerialNumberOID = DecryptOID(PrtOutputSerialNumberOID);
	m_szPrtOutputDescriptionOID = DecryptOID(PrtOutputDescriptionOID);
	m_szPrtOutputSecurityOID = DecryptOID(PrtOutputSecurityOID);
	m_szPrtOutputDimUnitOID = DecryptOID(PrtOutputDimUnitOID);
	m_szPrtOutputMaxDimFeedDirOID = DecryptOID(PrtOutputMaxDimFeedDirOID);
	m_szPrtOutputMaxDimXFeedDirOID = DecryptOID(PrtOutputMaxDimXFeedDirOID);
	m_szPrtOutputMinDimFeedDirOID = DecryptOID(PrtOutputMinDimFeedDirOID);
	m_szPrtOutputMinDimXFeedDirOID = DecryptOID(PrtOutputMinDimXFeedDirOID);
	m_szPrtOutputStackingOrderOID = DecryptOID(PrtOutputStackingOrderOID);
	m_szPrtOutputPageDeliveryOrientationOID = DecryptOID(PrtOutputPageDeliveryOrientationOID);
	m_szPrtOutputBurstingOID = DecryptOID(PrtOutputBurstingOID);
	m_szPrtOutputDecollatingOID = DecryptOID(PrtOutputDecollatingOID);
	m_szPrtOutputPageCollatedOID = DecryptOID(PrtOutputPageCollatedOID);
	m_szPrtOutputOffsetStackingOID = DecryptOID(PrtOutputOffsetStackingOID);

	//printmib.prtMarker.prtMarkerTable.prtMarkerEntry
	m_szPrtMarkerEntryOID = DecryptOID(PrtMarkerEntryOID);
	m_szPrtMarkerIndexOID = DecryptOID(PrtMarkerIndexOID);
	m_szPrtMarkerMarkTechOID = DecryptOID(PrtMarkerMarkTechOID);
	m_szPrtMarkerCounterUnitOID = DecryptOID(PrtMarkerCounterUnitOID);
	m_szPrtMarkerLifeCountOID = DecryptOID(PrtMarkerLifeCountOID);
	m_szPrtMarkerPowerOnCount = DecryptOID(PrtMarkerPowerOnCount);
	m_szPrtMarkerProcessColorantsOID = DecryptOID(PrtMarkerProcessColorantsOID);
	m_szPrtMarkerSpotColorantsOID = DecryptOID(PrtMarkerSpotColorantsOID);
	m_szPrtMarkerAddressabilityUnitOID = DecryptOID(PrtMarkerAddressabilityUnitOID);
	m_szPrtMarkerAddressabilityFeedDirOID = DecryptOID(PrtMarkerAddressabilityFeedDirOID);
	m_szPrtMarkerAddressabilityXFeedDirOID = DecryptOID(PrtMarkerAddressabilityXFeedDirOID);
	m_szPrtMarkerNorthMarginOID = DecryptOID(PrtMarkerNorthMarginOID);
	m_szPrtMarkerSouthMarginOID = DecryptOID(PrtMarkerSouthMarginOID);
	m_szPrtMarkerWestMarginOID = DecryptOID(PrtMarkerWestMarginOID);
	m_szPrtMarkerEastMarginOID = DecryptOID(PrtMarkerEastMarginOID);
	m_szPrtMarkerStatusOID = DecryptOID(PrtMarkerStatusOID);

	//printmib.prtMarkerSupplies.prtMarkerSuppliesTable.prtMarkerSuppliesEntry
	m_szPrtMarkerSuppliesEntryOID = DecryptOID(PrtMarkerSuppliesEntryOID);
	m_szPrtMarkerSuppliesIndexOID = DecryptOID(PrtMarkerSuppliesIndexOID);
	m_szPrtMarkerSuppliesMarkerIndexOID = DecryptOID(PrtMarkerSuppliesMarkerIndexOID);
	m_szPrtMarkerSuppliesColorantIndexOID = DecryptOID(PrtMarkerSuppliesColorantIndexOID);
	m_szPrtMarkerSuppliesClassOID = DecryptOID(PrtMarkerSuppliesClassOID);
	m_szPrtMarkerSuppliesTypeOID = DecryptOID(PrtMarkerSuppliesTypeOID);
	m_szPrtMarkerSuppliesDescriptionOID = DecryptOID(PrtMarkerSuppliesDescriptionOID);
	m_szPrtMarkerSuppliesSupplyUnitOID = DecryptOID(PrtMarkerSuppliesSupplyUnitOID);
	m_szPrtMarkerSuppliesMaxCapacityOID = DecryptOID(PrtMarkerSuppliesMaxCapacityOID);
	m_szPrtMarkerSuppliesLevelOID = DecryptOID(PrtMarkerSuppliesLevelOID);

	//printmib.prtMarkerColorant.prtMarkerColorantTable.prtMarkerColorantEntry
	m_szPrtMarkerColorantEntryOID = DecryptOID(PrtMarkerColorantEntryOID);
	m_szPrtMarkerColorantIndexOID = DecryptOID(PrtMarkerColorantIndexOID);
	m_szPrtMarkerColorantMarkerIndexOID = DecryptOID(PrtMarkerColorantMarkerIndexOID);
	m_szPrtMarkerColorantRoleOID = DecryptOID(PrtMarkerColorantRoleOID);
	m_szPrtMarkerColorantValueOID = DecryptOID(PrtMarkerColorantValueOID);
	m_szPrtMarkerColorantTonalityOID = DecryptOID(PrtMarkerColorantTonalityOID);

	//printmib.prtMediaPath.prtMediaPathTable.prtMediaPathEntry
	m_szPrtMediaPathEntryOID = DecryptOID(PrtMediaPathEntryOID);
	m_szPrtMediaPathIndexOID = DecryptOID(PrtMediaPathIndexOID);
	m_szPrtMediaPathMaxSpeedPrintUnitOID = DecryptOID(PrtMediaPathMaxSpeedPrintUnitOID);
	m_szPrtMediaPathMediaSizeUnitOID = DecryptOID(PrtMediaPathMediaSizeUnitOID);
	m_szPrtMediaPathMaxSpeedOID = DecryptOID(PrtMediaPathMaxSpeedOID);
	m_szPrtMediaPathMaxMediaFeedDirOID = DecryptOID(PrtMediaPathMaxMediaFeedDirOID);
	m_szPrtMediaPathMaxMediaXFeedDirOID = DecryptOID(PrtMediaPathMaxMediaXFeedDirOID);
	m_szPrtMediaPathMinMediaFeedDirOID = DecryptOID(PrtMediaPathMinMediaFeedDirOID);
	m_szPrtMediaPathMinMediaXFeedDirOID = DecryptOID(PrtMediaPathMinMediaXFeedDirOID);
	m_szPrtMediaPathTypeOID = DecryptOID(PrtMediaPathTypeOID);
	m_szPrtMediaPathDescriptionOID = DecryptOID(PrtMediaPathDescriptionOID);
	m_szPrtMediaPathStatusOID = DecryptOID(PrtMediaPathStatusOID);

	//printmib.prtChannel.prtChannelTable.prtChannelEntry
	m_szPrtChannelEntryOID = DecryptOID(PrtChannelEntryOID);
	m_szPrtChannelIndexOID = DecryptOID(PrtChannelIndexOID);
	m_szPrtChannelTypeOID = DecryptOID(PrtChannelTypeOID);
	m_szPrtChannelProtocolVersionOID = DecryptOID(PrtChannelProtocolVersionOID);
	m_szPrtChannelCurrentJobCntlLangIndexOID = DecryptOID(PrtChannelCurrentJobCntlLangIndexOID);
	m_szPrtChannelDefaultPageDescLangIndexOID = DecryptOID(PrtChannelDefaultPageDescLangIndexOID);
	m_szPrtChannelStateOID = DecryptOID(PrtChannelStateOID);
	m_szPrtChannelIfIndexOID = DecryptOID(PrtChannelIfIndexOID);
	m_szPrtChannelStatusOID = DecryptOID(PrtChannelStatusOID);
#if 0
	m_szPrtChannelInformationOID = DecryptOID(PrtChannelInformationOID);
#else
	m_szPrtChannelInformationOID.Format("%s.9", m_szPrtChannelEntryOID);
#endif

	//printmib.prtInterpreter.prtInterpreterTable.prtInterpreterEntry
	m_szPrtInterpreterEntryOID = DecryptOID(PrtInterpreterEntryOID);
	m_szPrtInterpreterIndexOID = DecryptOID(PrtInterpreterIndexOID);
	m_szPrtInterpreterLangFamilyOID = DecryptOID(PrtInterpreterLangFamilyOID);
	m_szPrtInterpreterLangLevelOID = DecryptOID(PrtInterpreterLangLevelOID);
	m_szPrtInterpreterLangVersionOID = DecryptOID(PrtInterpreterLangVersionOID);
	m_szPrtInterpreterDescriptionOID = DecryptOID(PrtInterpreterDescriptionOID);
	m_szPrtInterpreterVersionOID = DecryptOID(PrtInterpreterVersionOID);
	m_szPrtInterpreterDefaultOrientationOID = DecryptOID(PrtInterpreterDefaultOrientationOID);
	m_szPrtInterpreterFeedAddressabilityOID = DecryptOID(PrtInterpreterFeedAddressabilityOID);
	m_szPrtInterpreterXFeedAddressabilityOID = DecryptOID(PrtInterpreterXFeedAddressabilityOID);
	m_szPrtInterpreterDefaultCharSetInOID = DecryptOID(PrtInterpreterDefaultCharSetInOID);
	m_szPrtInterpreterDefaultCharSetOutOID = DecryptOID(PrtInterpreterDefaultCharSetOutOID);
	m_szPrtInterpreterTwoWayOID = DecryptOID(PrtInterpreterTwoWayOID);

	//printmib.prtConsoleDisplayBuffer.prtConsoleDisplayBufferTable.prtConsoleDisplayBufferEntry
	m_szPrtConsoleDisplayBufferEntryOID = DecryptOID(PrtConsoleDisplayBufferEntryOID);
	m_szPrtConsoleDisplayBufferIndexOID = DecryptOID(PrtConsoleDisplayBufferIndexOID);
	m_szPrtConsoleDisplayBufferTextOID = DecryptOID(PrtConsoleDisplayBufferTextOID);

	//printmib.prtConsoleLight.prtConsoleLightTable.prtConsoleLightEntry
	m_szPrtConsoleLightEntryOID = DecryptOID(PrtConsoleLightEntryOID);
	m_szPrtConsoleLightIndexOID = DecryptOID(PrtConsoleLightIndexOID);
	m_szPrtConsoleOnTimeOID = DecryptOID(PrtConsoleOnTimeOID);
	m_szPrtConsoleOffTimeOID = DecryptOID(PrtConsoleOffTimeOID);
	m_szPrtConsoleColorOID = DecryptOID(PrtConsoleColorOID);
	m_szPrtConsoleDescriptionOID = DecryptOID(PrtConsoleDescriptionOID);

	//printmib.prtAlert.prtAlertTable.prtAlertEntry
	m_szPrtAlertEntryOID = DecryptOID(PrtAlertEntryOID);
	m_szPrtAlertIndexOID = DecryptOID(PrtAlertIndexOID);
	m_szPrtAlertSeverityLevelOID = DecryptOID(PrtAlertSeverityLevelOID);
	m_szPrtAlertTrainingLevelOID = DecryptOID(PrtAlertTrainingLevelOID);
	m_szPrtAlertGroupOID = DecryptOID(PrtAlertGroupOID);
	m_szPrtAlertGroupIndexOID = DecryptOID(PrtAlertGroupIndexOID);
	m_szPrtAlertLocationOID = DecryptOID(PrtAlertLocationOID);
	m_szPrtAlertCodeOID = DecryptOID(PrtAlertCodeOID);
	m_szPrtAlertDescriptionOID = DecryptOID(PrtAlertDescriptionOID);
	m_szPrtAlertTimeOID = DecryptOID(PrtAlertTimeOID);
}