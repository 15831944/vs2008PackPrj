// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CTickLabels 包装类

class CTickLabels : public COleDispatchDriver
{
public:
	CTickLabels(){} // 调用 COleDispatchDriver 默认构造函数
	CTickLabels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTickLabels(const CTickLabels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// TickLabels 方法
public:
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x7d2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_NumberFormat()
	{
		CString result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormat(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_NumberFormatLinked()
	{
		BOOL result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormatLinked(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_NumberFormatLocal()
	{
		VARIANT result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormatLocal(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_Orientation()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Orientation(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x7d9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_AutoScaleFont()
	{
		VARIANT result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_AutoScaleFont(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_Depth()
	{
		long result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Offset()
	{
		long result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Offset(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MultiLevel()
	{
		BOOL result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MultiLevel(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
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
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Alignment()
	{
		long result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Alignment(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ReadingOrder()
	{
		long result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReadingOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// TickLabels 属性
public:

};
