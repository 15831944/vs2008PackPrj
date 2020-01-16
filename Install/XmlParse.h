#pragma once

#import <msxml3.dll> named_guids
// using namespace MSXML2;

class  CXmlParse
{
public:
	CXmlParse(void);
	~CXmlParse(void);
public:
	// 解析XMl数据
	MSXML2::IXMLDOMDocument2Ptr DomFromCOM();
	BOOL ParseXml(CString szXml, CString sRowTag = _T("Table"));
	MSXML2::IXMLDOMElementPtr GetXmlRoot(void);
	MSXML2::IXMLDOMDocument2Ptr GetXmlDocment(void);
	MSXML2::IXMLDOMNodePtr SelectSingleNode(MSXML2::IXMLDOMNodePtr pNode, CString szNodeName);
	MSXML2::IXMLDOMNodeListPtr SelectNodes(MSXML2::IXMLDOMNodePtr pNode, CString szNodeName);
	BOOL SetAttribute(MSXML2::IXMLDOMElementPtr pElement, CString szAttributeName, CString szAttributeValue);
	BOOL Save(MSXML2::IXMLDOMDocument2Ptr pDoc, CString szXmlFilePath);

public:
	// 根据名称获取数据项
	CString GetItemText(int nRow,int nCol);
	CString GetItemText(int nItem, MSXML2::IXMLDOMNodeListPtr pNodeList);
	CString GetItemText(CString sItemName, MSXML2::IXMLDOMNodeListPtr pNodeList);
	CString GetItemText(int nRow,CString sColName);
	int GetColCount(void);
	int GetRowCount(void);

protected:
	MSXML2::IXMLDOMElementPtr			m_pRoot;
	MSXML2::IXMLDOMDocument2Ptr			m_pXMLDom;
	long	m_nCol;	//列
	long	m_nRow;	//行
	CString m_sRowTag;
	void WriteFileLog(LPCTSTR lpszFormat, ...);
	void WriteFileLog(CString sLog);
};
