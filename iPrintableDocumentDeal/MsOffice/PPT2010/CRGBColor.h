// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CRGBColor 包装类

class CRGBColor : public COleDispatchDriver
{
public:
	CRGBColor(){} // 调用 COleDispatchDriver 默认构造函数
	CRGBColor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRGBColor(const CRGBColor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// RGBColor 方法
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
	long get_RGB()
	{
		long result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_RGB(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// RGBColor 属性
public:

};
