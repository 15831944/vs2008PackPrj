// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CNamedSlideShow 包装类

class CNamedSlideShow : public COleDispatchDriver
{
public:
	CNamedSlideShow(){} // 调用 COleDispatchDriver 默认构造函数
	CNamedSlideShow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CNamedSlideShow(const CNamedSlideShow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// NamedSlideShow 方法
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
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void Delete()
	{
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	VARIANT get_SlideIDs()
	{
		VARIANT result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// NamedSlideShow 属性
public:

};
