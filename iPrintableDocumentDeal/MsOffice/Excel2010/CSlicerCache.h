// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSlicerCache 包装类

class CSlicerCache : public COleDispatchDriver
{
public:
	CSlicerCache(){} // 调用 COleDispatchDriver 默认构造函数
	CSlicerCache(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSlicerCache(const CSlicerCache& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SlicerCache 方法
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
	long get_Index()
	{
		long result;
		InvokeHelper(0x1e6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_OLAP()
	{
		BOOL result;
		InvokeHelper(0x81d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_SourceType()
	{
		long result;
		InvokeHelper(0x2ad, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_WorkbookConnection()
	{
		LPDISPATCH result;
		InvokeHelper(0x9f0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Slicers()
	{
		LPDISPATCH result;
		InvokeHelper(0xb41, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PivotTables()
	{
		LPDISPATCH result;
		InvokeHelper(0x2b2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SlicerCacheLevels()
	{
		LPDISPATCH result;
		InvokeHelper(0xb9e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_VisibleSlicerItems()
	{
		LPDISPATCH result;
		InvokeHelper(0xb9f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_VisibleSlicerItemsList()
	{
		VARIANT result;
		InvokeHelper(0xba0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_VisibleSlicerItemsList(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xba0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_SlicerItems()
	{
		LPDISPATCH result;
		InvokeHelper(0xba1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_CrossFilterType()
	{
		long result;
		InvokeHelper(0xba2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CrossFilterType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xba2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SortItems()
	{
		long result;
		InvokeHelper(0xba3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SortItems(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xba3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_SourceName()
	{
		CString result;
		InvokeHelper(0x2d1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_SortUsingCustomLists()
	{
		BOOL result;
		InvokeHelper(0xa0e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SortUsingCustomLists(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xa0e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowAllItems()
	{
		BOOL result;
		InvokeHelper(0x1c4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowAllItems(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1c4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void ClearManualFilter()
	{
		InvokeHelper(0xa22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Delete()
	{
		InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// SlicerCache 属性
public:

};
