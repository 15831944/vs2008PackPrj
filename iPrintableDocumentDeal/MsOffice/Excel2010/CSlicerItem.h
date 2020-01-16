// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSlicerItem 包装类

class CSlicerItem : public COleDispatchDriver
{
public:
	CSlicerItem(){} // 调用 COleDispatchDriver 默认构造函数
	CSlicerItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSlicerItem(const CSlicerItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SlicerItem 方法
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
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	VARIANT get_SourceName()
	{
		VARIANT result;
		InvokeHelper(0x2d1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	CString get_SourceNameStandard()
	{
		CString result;
		InvokeHelper(0x864, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Value()
	{
		CString result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_Selected()
	{
		BOOL result;
		InvokeHelper(0x463, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Selected(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x463, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasData()
	{
		BOOL result;
		InvokeHelper(0xbad, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}

	// SlicerItem 属性
public:

};
