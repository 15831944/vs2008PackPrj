#include "stdafx.h"
#include "XmlParse.h"

/************************************************************************/
/* 基本用法示例
CString szXml("C:\\Users\\Administrator\\Desktop\\Web.config");
CXmlParse xml;
if (xml.ParseXml(szXml))
{
	MSXML2::IXMLDOMDocument2Ptr doc = xml.GetXmlDocment();
	MSXML2::IXMLDOMNodePtr configNode = xml.SelectSingleNode(doc, "configuration");
	MSXML2::IXMLDOMNodePtr appNode = xml.SelectSingleNode(configNode, "appSettings");
	MSXML2::IXMLDOMNodePtr WebHostIpNode = xml.SelectSingleNode(appNode, "add[@key='WebHostIp']");
	xml.SetAttribute(WebHostIpNode, "value", "192.168.4.147");
	xml.Save(doc, szXml);
}
*/
/************************************************************************/

#define HRCALL(a) \
do { \
	HRESULT hr;\
    hr = (a); \
    if (FAILED(hr) || hr==S_FALSE) { \
	WriteFileLog( _T("%s:%d  HRCALL Failed: 0x%.8x = %s\n"), \
                __FILE__, __LINE__,  hr, #a ); \
        goto clean; \
    } \
} while (0)

CXmlParse::CXmlParse(void)
{
	//xml初始化开始
	::CoInitialize(NULL);  

	m_pRoot = NULL;
	m_pXMLDom = NULL;
}

CXmlParse::~CXmlParse(void)
{
}

void CXmlParse::WriteFileLog(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	CString sTmp;
	sTmp.FormatV(lpszFormat, argList);
	va_end(argList);

	WriteFileLog(sTmp);
}

void CXmlParse::WriteFileLog(CString sLog)
{
	if( sLog.GetLength() > 250 )
		TRACE(sLog.Left(250));
	else
		TRACE(sLog);
	TRACE(_T("\n"));

//	if( !g_bIsWriteLog )
//		return;

	CString sSysDir = CCommonFun::GetLogDir();

	CFileFind fd;

	if( !fd.FindFile(sSysDir) )
	{
		if( !::CreateDirectory(sSysDir,NULL) )
		return;
	}

	CTime time = CTime::GetCurrentTime();
	CString sDate = time.Format(_T("%y%m%d"));
	
	CString sFileName;
	sFileName.Format(_T("%s\\As_CXmlParse_%s.Log"),sSysDir,sDate);
	
	
	try
	{
		CFile fileLog;
		if(!fileLog.Open(sFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
			return;
		
		CString sTime = time.Format( _T("%Y-%m-%d %H:%M:%S") );
		CString sTmp;
		
		sTmp.Format(_T("%s,	 %s"),sTime,sLog);
		sTmp += "\r\n";
		fileLog.SeekToEnd();
		fileLog.Write(sTmp,sTmp.GetLength());
		fileLog.Close();
	}
	catch ( CFileException* ex ) 
	{
		ex->Delete();
	}
	catch (...) 
	{
		;
	}

}

MSXML2::IXMLDOMDocument2Ptr CXmlParse::DomFromCOM()
{
   MSXML2::IXMLDOMDocument2Ptr pxmldoc = NULL;

     /*CoCreateInstance(__uuidof(DOMDocument40),
                      NULL,
                      CLSCTX_INPROC_SERVER,
                      __uuidof(IXMLDOMDocument),
                      (void**)&pxmldoc));*/
#if 0
   HRCALL( CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
	   IID_IXMLDOMDocument, (void**)&pxmldoc));


   HRCALL( pxmldoc->put_async(VARIANT_FALSE));
   HRCALL( pxmldoc->put_validateOnParse(VARIANT_FALSE));
   HRCALL( pxmldoc->put_resolveExternals(VARIANT_FALSE));
#else
   HRCALL(pxmldoc.CreateInstance(MSXML2::CLSID_DOMDocument));
#endif

   return pxmldoc;
clean:
   return NULL;
}

// 解析XMl数据
BOOL CXmlParse::ParseXml(CString szXml,CString sRowTag /*= "Table"*/)
{
	BOOL bRet = FALSE;
	CComBSTR bstr;
	m_nRow = 0;
	m_nCol = 0;
	m_sRowTag = sRowTag;
	BSTR bsXml = _bstr_t(szXml);

	MSXML2::IXMLDOMParseErrorPtr pXMLErr=NULL;
	MSXML2::IXMLDOMElementPtr pRoot = NULL;
	MSXML2::IXMLDOMNodeListPtr pList = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMNodePtr pListItem = NULL;
	try
	{
		VARIANT_BOOL status;
		m_pXMLDom = DomFromCOM();
		if (!m_pXMLDom) 
		{
			bRet = FALSE;
			goto clean;
		}

		status = m_pXMLDom->load(bsXml);
		
		if (status!=VARIANT_TRUE) 
		{
			HRCALL(m_pXMLDom->get_parseError(&pXMLErr));
			HRCALL(pXMLErr->get_reason(&bstr));
			bRet = FALSE;
			goto clean;
		}
		
		HRCALL(m_pXMLDom->get_documentElement(&m_pRoot));
		if (!m_pRoot)
		{
			bRet = FALSE;
			goto clean;
		}
		
		//获取行数
		pList = m_pXMLDom->selectNodes(sRowTag.GetString());
		HRCALL(pList->get_length(&m_nRow));

		if(m_nRow > 0)
		{
			HRCALL(pList->get_item(long(0),&pListItem)); //MyData
			HRCALL(pListItem->get_childNodes(&pNodeList));
			HRCALL(pNodeList->get_length(&m_nCol));
		}

		bRet = TRUE;
	}
	catch(...)
	{
		bRet = FALSE;
	}

clean:
	return bRet;
}

// 根据名称获取数据项
CString CXmlParse::GetItemText(int nItem, MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	CString sRet;
	CComBSTR bstrText;
	long nLen = 0;
	MSXML2::IXMLDOMNodePtr pListItem = NULL;

	HRCALL(pNodeList->get_length(&nLen));
	if( nItem < nLen )
	{
		HRCALL(pNodeList->get_item(long(nItem),&pListItem));
		HRCALL(pListItem->get_text(&bstrText));
	}

clean:
	if( pListItem) pListItem->Release();	

	sRet = bstrText;
	return sRet;
}

CString CXmlParse::GetItemText(CString sItemName, MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	MSXML2::IXMLDOMNodePtr pListItem = NULL;
	CComBSTR bstrName;
	CComBSTR bstrText;

	CString sNodeName,sNodeText;
	long lItemLen=0;	
	int j=0;
	HRCALL(pNodeList->get_length(&lItemLen));
	for( j=0;j<lItemLen;j++)
	{		
		if( pListItem )
		{
			pListItem->Release();
			pListItem = NULL;
		}
		HRCALL(pNodeList->get_item(long(j),&pListItem)); //Record Item
		
		HRCALL(pListItem->get_nodeName(&bstrName));
		sNodeName = bstrName;
		if( sNodeName.CompareNoCase(sItemName) == 0 )
		{
			HRCALL(pListItem->get_text(&bstrText));
			sNodeText = bstrText;
			break;
		}
	}
clean:
	if( pListItem) pListItem->Release();	
	//-------------------------------------

	return sNodeText;
}

MSXML2::IXMLDOMElementPtr CXmlParse::GetXmlRoot(void)
{
	return m_pRoot;
}

MSXML2::IXMLDOMDocument2Ptr CXmlParse::GetXmlDocment(void)
{
	return m_pXMLDom;
}

MSXML2::IXMLDOMNodePtr CXmlParse::SelectSingleNode(MSXML2::IXMLDOMNodePtr pNode, CString szNodeName)
{
	MSXML2::IXMLDOMNodePtr ptr;
	try
	{
		if (!pNode)
		{
			return ptr;
		}
		ptr = pNode->selectSingleNode(szNodeName.GetString());			
	}
	catch(...)
	{
	}
	return ptr;
}

MSXML2::IXMLDOMNodeListPtr CXmlParse::SelectNodes(MSXML2::IXMLDOMNodePtr pNode, CString szNodeName)
{
	MSXML2::IXMLDOMNodeListPtr ptr;
	try
	{
		if (!pNode)
		{
			return ptr;
		}
		ptr = pNode->selectNodes(szNodeName.GetString());			
	}
	catch(...)
	{
	}
	return ptr;
}

BOOL CXmlParse::GetNodeValue(MSXML2::IXMLDOMNodePtr pNode, CString szNodeName, CString& szNodeValue)
{
	try
	{
		if (!pNode)
		{
			return FALSE;
		}

		MSXML2::IXMLDOMNodePtr pNodeSub = SelectSingleNode(pNode, szNodeName);
		if (!pNodeSub)
		{
			return FALSE;
		}

		BSTR bstr;
		HRESULT hr = pNodeSub->get_text(&bstr);
		if (SUCCEEDED(hr))
		{
			szNodeValue = bstr;
			::SysFreeString(bstr);
			return TRUE;
		}
	}
	catch(...)
	{
	}
	return FALSE;
}

BOOL CXmlParse::SetNodeValue(MSXML2::IXMLDOMNodePtr pNode, CString szNodeName, CString szNodeValue)
{
	try
	{
		if (!pNode)
		{
			return FALSE;
		}

		MSXML2::IXMLDOMNodePtr pNodeSub = SelectSingleNode(pNode, szNodeName);
		if (!pNodeSub && m_pXMLDom)
		{
			//子节点不存在，则创建一个子节点
			BSTR bstr = szNodeValue.AllocSysString();
			_variant_t varNodeType((short)MSXML2::NODE_ELEMENT);
			MSXML2::IXMLDOMElementPtr pElement = m_pXMLDom->createNode(varNodeType, _bstr_t(szNodeName), _bstr_t(""));
			pElement->put_text(bstr);
			pNodeSub = pNode->appendChild(pElement);
			::SysFreeString(bstr);
			if (pNodeSub)
			{
				MSXML2::IXMLDOMNodePtr pNodeSub33 = SelectSingleNode(pNode, szNodeName);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		BSTR bstr = szNodeValue.AllocSysString();
		HRESULT hr = pNodeSub->put_text(bstr);
		if (SUCCEEDED(hr))
		{
			::SysFreeString(bstr);
			return TRUE;
		}	
	}
	catch(...)
	{
	}
	return FALSE;
}

BOOL CXmlParse::GetAttribute(MSXML2::IXMLDOMElementPtr pElement, CString szAttributeName, CString& szAttributeValue)
{
	try
	{
		if (!pElement)
		{
			return FALSE;
		}

		_variant_t var = pElement->getAttribute(szAttributeName.GetString());
		if (var.vt == VT_NULL || var.vt == VT_EMPTY)
		{
			szAttributeValue = "";
		}
		else
		{
			var.ChangeType(VT_BSTR);
			szAttributeValue = var.bstrVal;
		}
		var.Clear();
		return TRUE;
	}
	catch(...)
	{
	}
	return FALSE;
}

BOOL CXmlParse::SetAttribute(MSXML2::IXMLDOMElementPtr pElement, CString szAttributeName, CString szAttributeValue)
{
	try
	{
		if (!pElement)
		{
			return FALSE;
		}
		
		_variant_t var(szAttributeValue.GetString());
		HRESULT hr = pElement->setAttribute(szAttributeName.GetString(), var);
		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	catch(...)
	{
	}
	return FALSE;
}

BOOL CXmlParse::Save(MSXML2::IXMLDOMDocument2Ptr pDoc, CString szXmlFilePath)
{
	try
	{
		if (!pDoc)
		{
			return FALSE;
		}

		_variant_t destination(szXmlFilePath.GetString());
		HRESULT hr = pDoc->save(destination);
		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	catch(...)
	{
	}
	return FALSE;	
}


CString CXmlParse::GetItemText(int nRow,CString sColName)
{
	if(GetColCount() <= nRow)
	{
		ASSERT(FALSE);
		return _T("");
	}
	MSXML2::IXMLDOMElementPtr pRoot = NULL;
	MSXML2::IXMLDOMNodeListPtr pList = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMNodePtr pListItem = NULL;
	CComBSTR bstrText;
	CString sNodeText;
	try
	{
		pRoot = GetXmlRoot();
		if( pRoot == NULL ) 
			goto clean;

		pList = pRoot->getElementsByTagName(m_sRowTag.GetString());			
		HRCALL(pList->get_item(long(nRow),&pListItem)); //MyData
		HRCALL(pListItem->get_childNodes(&pNodeList));
		
		sNodeText = GetItemText(sColName,pNodeList);

clean:
		if( pList) pList->Release();
		if( pNodeList) pNodeList->Release();
		if( pListItem) pListItem->Release();	
		//-------------------------------------
	}
	catch(...)
	{
		if( pList) pList->Release();
		if( pNodeList) pNodeList->Release();
		if( pListItem) pListItem->Release();	
	}

	return sNodeText;	
}

CString CXmlParse::GetItemText(int nRow,int nCol)
{
	if(GetColCount() <= nCol || GetRowCount() <= nRow)
	{
		ASSERT(FALSE);
		return _T("");
	}
	MSXML2::IXMLDOMElementPtr pRoot = NULL;
	MSXML2::IXMLDOMNodeListPtr pList = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMNodePtr pListItem = NULL;
	CComBSTR bstrText;
	CString sNodeText;
	try
	{
		pRoot = GetXmlRoot();
		if( pRoot == NULL ) 
			goto clean;

		pList = pRoot->getElementsByTagName(m_sRowTag.GetString());			
		HRCALL(pList->get_item(long(nRow),&pListItem)); //MyData
		HRCALL(pListItem->get_childNodes(&pNodeList));
		HRCALL(pNodeList->get_item(long(nCol),&pListItem));
		HRCALL(pListItem->get_text(&bstrText));
		sNodeText = bstrText;

clean:
		if( pList) pList->Release();
		if( pNodeList) pNodeList->Release();
		if( pListItem) pListItem->Release();	
		//-------------------------------------
	}
	catch(...)
	{
		if( pList) pList->Release();
		if( pNodeList) pNodeList->Release();
		if( pListItem) pListItem->Release();	
	}

	return sNodeText;	
}

int CXmlParse::GetColCount(void)
{
	return m_nCol;
}

int CXmlParse::GetRowCount(void)
{
	return m_nRow;
}
