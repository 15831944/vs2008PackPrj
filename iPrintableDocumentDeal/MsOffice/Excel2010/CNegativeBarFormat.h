// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CNegativeBarFormat 包装类

class CNegativeBarFormat : public COleDispatchDriver
{
public:
	CNegativeBarFormat(){} // 调用 COleDispatchDriver 默认构造函数
	CNegativeBarFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CNegativeBarFormat(const CNegativeBarFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// NegativeBarFormat 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ColorType()
	{
		long result;
		InvokeHelper(0x893, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ColorType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x893, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BorderColorType()
	{
		long result;
		InvokeHelper(0xb99, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BorderColorType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb99, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Color()
	{
		LPDISPATCH result;
		InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_BorderColor()
	{
		LPDISPATCH result;
		InvokeHelper(0xb9a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// NegativeBarFormat 属性
public:

};
