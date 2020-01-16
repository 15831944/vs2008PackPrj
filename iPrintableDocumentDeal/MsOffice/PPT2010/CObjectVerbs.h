// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CObjectVerbs 包装类

class CObjectVerbs : public COleDispatchDriver
{
public:
	CObjectVerbs(){} // 调用 COleDispatchDriver 默认构造函数
	CObjectVerbs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CObjectVerbs(const CObjectVerbs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ObjectVerbs 方法
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
	CString Item(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}

	// ObjectVerbs 属性
public:

};
