// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CTags 包装类

class CTags : public COleDispatchDriver
{
public:
	CTags(){} // 调用 COleDispatchDriver 默认构造函数
	CTags(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTags(const CTags& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Tags 方法
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	VARIANT _Index(long Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
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
	CString Item(LPCTSTR Name)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Name);
		return result;
	}
	void Add(LPCTSTR Name, LPCTSTR Value)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name, Value);
	}
	void Delete(LPCTSTR Name)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void AddBinary(LPCTSTR Name, LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name, FilePath);
	}
	long BinaryValue(LPCTSTR Name)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Name);
		return result;
	}
	CString Name(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	CString Value(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}

	// Tags 属性
public:

};
