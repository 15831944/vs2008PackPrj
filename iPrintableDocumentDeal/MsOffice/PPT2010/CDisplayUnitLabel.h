// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CDisplayUnitLabel 包装类

class CDisplayUnitLabel : public COleDispatchDriver
{
public:
	CDisplayUnitLabel(){} // 调用 COleDispatchDriver 默认构造函数
	CDisplayUnitLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDisplayUnitLabel(const CDisplayUnitLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// DisplayUnitLabel 方法
public:
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Characters(VARIANT& Start, VARIANT& Length)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Start, &Length);
		return result;
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_HorizontalAlignment(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_HorizontalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	double get_Left()
	{
		double result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Left(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_Orientation(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Orientation()
	{
		VARIANT result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL get_Shadow()
	{
		BOOL result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Shadow(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_Text(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Text()
	{
		CString result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	double get_Top()
	{
		double result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Top(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_VerticalAlignment(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_VerticalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_AutoScaleFont(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7db, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_AutoScaleFont()
	{
		VARIANT result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Interior()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Fill()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Border()
	{
		LPDISPATCH result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x7e2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
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
		InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ReadingOrder()
	{
		long result;
		InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReadingOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Height()
	{
		double result;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Width()
	{
		double result;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Formula(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Formula()
	{
		CString result;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormulaR1C1()
	{
		CString result;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormulaLocal(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7ea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormulaLocal()
	{
		CString result;
		InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1Local(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormulaR1C1Local()
	{
		CString result;
		InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// DisplayUnitLabel 属性
public:

};
