// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CUserAccessList 包装类

class CUserAccessList : public COleDispatchDriver
{
public:
	CUserAccessList(){} // 调用 COleDispatchDriver 默认构造函数
	CUserAccessList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CUserAccessList(const CUserAccessList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// UserAccessList 方法
public:
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH Add(LPCTSTR Name, BOOL AllowEdit)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0xb5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Name, AllowEdit);
		return result;
	}
	void DeleteAll()
	{
		InvokeHelper(0x8bf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get__Default(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// UserAccessList 属性
public:

};
