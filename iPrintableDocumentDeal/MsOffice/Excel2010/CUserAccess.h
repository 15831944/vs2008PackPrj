// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CUserAccess 包装类

class CUserAccess : public COleDispatchDriver
{
public:
	CUserAccess(){} // 调用 COleDispatchDriver 默认构造函数
	CUserAccess(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CUserAccess(const CUserAccess& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// UserAccess 方法
public:
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_AllowEdit()
	{
		BOOL result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AllowEdit(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Delete()
	{
		InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// UserAccess 属性
public:

};
