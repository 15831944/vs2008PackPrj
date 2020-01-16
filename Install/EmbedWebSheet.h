
#if _MSC_VER > 1000
#pragma once
#endif

#include "EmbedWebPageOne.h"
#include "EmbedWebPageTwo.h"

// CEmbedWebSheet

class CEmbedWebSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CEmbedWebSheet)

public:
	CEmbedWebSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEmbedWebSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    CEmbedWebSheet(UINT iPage ,LPCTSTR pszCaption);
    virtual ~CEmbedWebSheet();
	
    CEmbedWebPageOne page1;
    CEmbedWebPageTwo page2;
    void WriteTheURL(BOOL bQueit = FALSE, CString szSerIp = "", CString szSerPort = "80");
    //void WriteTheFtp(BOOL bQueit = FALSE);
    BOOL m_bSuc;
protected:

protected:
	DECLARE_MESSAGE_MAP()
};


