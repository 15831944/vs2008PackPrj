#pragma once
#include <afxinet.h>
class CGetContenByUrl
{
public:
	CGetContenByUrl(void);
	~CGetContenByUrl(void);
public:
	BOOL getHttpContentByUrl(CString szUrl,CString& szMsgBack);
	BOOL getHttpsContentByUrl(CString szUrl,CString& szMsgBack);
};
