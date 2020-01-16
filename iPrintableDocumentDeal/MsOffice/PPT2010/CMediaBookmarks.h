// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CMediaBookmarks 包装类

class CMediaBookmarks : public COleDispatchDriver
{
public:
	CMediaBookmarks(){} // 调用 COleDispatchDriver 默认构造函数
	CMediaBookmarks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMediaBookmarks(const CMediaBookmarks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// MediaBookmarks 方法
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
	LPDISPATCH Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH Add(long Position, LPCTSTR Name)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Position, Name);
		return result;
	}

	// MediaBookmarks 属性
public:

};
