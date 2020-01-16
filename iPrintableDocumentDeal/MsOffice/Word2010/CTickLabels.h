// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
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
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_NumberFormat()
	{
		CString result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormat(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_NumberFormatLinked()
	{
		BOOL result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormatLinked(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_NumberFormatLocal()
	{
		VARIANT result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormatLocal(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_Orientation()
	{
		long result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Orientation(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_ReadingOrder()
	{
		long result;
		InvokeHelper(0x6002000d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReadingOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_AutoScaleFont()
	{
		VARIANT result;
		InvokeHelper(0x6002000f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_AutoScaleFont(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6002000f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_Depth()
	{
		long result;
		InvokeHelper(0x60020011, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Offset()
	{
		long result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Offset(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Alignment()
	{
		long result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Alignment(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MultiLevel()
	{
		BOOL result;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MultiLevel(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
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

	// TickLabels 属性
public:

};
