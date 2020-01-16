// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CDesigns 包装类

class CDesigns : public COleDispatchDriver
{
public:
	CDesigns(){} // 调用 COleDispatchDriver 默认构造函数
	CDesigns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDesigns(const CDesigns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Designs 方法
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
	LPDISPATCH Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH Add(LPCTSTR designName, long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, designName, Index);
		return result;
	}
	LPDISPATCH Load(LPCTSTR TemplateName, long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, TemplateName, Index);
		return result;
	}
	LPDISPATCH Clone(LPDISPATCH pOriginal, long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, pOriginal, Index);
		return result;
	}

	// Designs 属性
public:

};
