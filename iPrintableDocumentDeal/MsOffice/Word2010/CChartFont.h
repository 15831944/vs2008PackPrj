// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
// CChartFont 包装类

class CChartFont : public COleDispatchDriver
{
public:
	CChartFont(){} // 调用 COleDispatchDriver 默认构造函数
	CChartFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartFont(const CChartFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ChartFont 方法
public:
	VARIANT get_Background()
	{
		VARIANT result;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Background(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Bold()
	{
		VARIANT result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Bold(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Color()
	{
		VARIANT result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Color(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_ColorIndex()
	{
		VARIANT result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ColorIndex(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FontStyle()
	{
		VARIANT result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FontStyle(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Italic()
	{
		VARIANT result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Italic(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Name()
	{
		VARIANT result;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Name(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_OutlineFont()
	{
		VARIANT result;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_OutlineFont(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Shadow()
	{
		VARIANT result;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Shadow(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Size()
	{
		VARIANT result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Size(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_StrikeThrough()
	{
		VARIANT result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_StrikeThrough(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Subscript()
	{
		VARIANT result;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Subscript(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Superscript()
	{
		VARIANT result;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Superscript(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Underline()
	{
		VARIANT result;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Underline(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
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

	// ChartFont 属性
public:

};
