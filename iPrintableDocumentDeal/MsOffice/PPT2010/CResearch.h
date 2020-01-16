// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CResearch 包装类

class CResearch : public COleDispatchDriver
{
public:
	CResearch(){} // 调用 COleDispatchDriver 默认构造函数
	CResearch(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CResearch(const CResearch& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Research 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Query(LPCTSTR ServiceID, VARIANT * QueryString, VARIANT * QueryLanguage, BOOL UseSelection, BOOL LaunchQuery)
	{
		static BYTE parms[] = VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ServiceID, QueryString, QueryLanguage, UseSelection, LaunchQuery);
	}
	void SetLanguagePair(VARIANT * Language1, VARIANT * Language2)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Language1, Language2);
	}
	BOOL IsResearchService(LPCTSTR ServiceID)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ServiceID);
		return result;
	}

	// Research 属性
public:

};
