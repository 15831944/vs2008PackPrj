// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CLinkFormat 包装类

class CLinkFormat : public COleDispatchDriver
{
public:
	CLinkFormat(){} // 调用 COleDispatchDriver 默认构造函数
	CLinkFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CLinkFormat(const CLinkFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// LinkFormat 方法
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
	CString get_SourceFullName()
	{
		CString result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SourceFullName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AutoUpdate()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutoUpdate(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Update()
	{
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void BreakLink()
	{
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// LinkFormat 属性
public:

};
