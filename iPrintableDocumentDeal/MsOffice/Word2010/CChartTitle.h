// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
// CChartTitle 包装类

class CChartTitle : public COleDispatchDriver
{
public:
	CChartTitle(){} // 调用 COleDispatchDriver 默认构造函数
	CChartTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartTitle(const CChartTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ChartTitle 方法
public:
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Characters(VARIANT& Start, VARIANT& Length)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Start, &Length);
		return result;
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_HorizontalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_HorizontalAlignment(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	double get_Left()
	{
		double result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Left(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Orientation()
	{
		VARIANT result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Orientation(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	BOOL get_Shadow()
	{
		BOOL result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Shadow(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Text()
	{
		CString result;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Text(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Top()
	{
		double result;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Top(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_VerticalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_VerticalAlignment(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_ReadingOrder()
	{
		long result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReadingOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_AutoScaleFont()
	{
		VARIANT result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_AutoScaleFont(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Interior()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Fill()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020017, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x60020018, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Border()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020019, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x6002001b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x6002001c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL get_IncludeInLayout()
	{
		BOOL result;
		InvokeHelper(0x972, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IncludeInLayout(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x972, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Position()
	{
		long result;
		InvokeHelper(0x687, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Position(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x687, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020021, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
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
	double get_Height()
	{
		double result;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Width()
	{
		double result;
		InvokeHelper(0x60020025, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	CString get_Formula()
	{
		CString result;
		InvokeHelper(0x60020026, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Formula(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020026, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormulaR1C1()
	{
		CString result;
		InvokeHelper(0x60020028, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020028, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormulaLocal()
	{
		CString result;
		InvokeHelper(0x6002002a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormulaLocal(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002002a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormulaR1C1Local()
	{
		CString result;
		InvokeHelper(0x6002002c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1Local(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002002c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ChartTitle 属性
public:

};
