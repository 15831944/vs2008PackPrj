// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// Cnterior0 包装类

class Cnterior0 : public COleDispatchDriver
{
public:
	Cnterior0(){} // 调用 COleDispatchDriver 默认构造函数
	Cnterior0(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Cnterior0(const Cnterior0& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Interior 方法
public:
	void put_Color(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Color()
	{
		VARIANT result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ColorIndex(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_ColorIndex()
	{
		VARIANT result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_InvertIfNegative(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_InvertIfNegative()
	{
		VARIANT result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Pattern(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Pattern()
	{
		VARIANT result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_PatternColor(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_PatternColor()
	{
		VARIANT result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_PatternColorIndex(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_PatternColorIndex()
	{
		VARIANT result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
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
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// Interior 属性
public:

};
