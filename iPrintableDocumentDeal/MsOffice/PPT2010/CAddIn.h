// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CAddIn 包装类

class CAddIn : public COleDispatchDriver
{
public:
	CAddIn(){} // 调用 COleDispatchDriver 默认构造函数
	CAddIn(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAddIn(const CAddIn& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// AddIn 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_FullName()
	{
		CString result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Path()
	{
		CString result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_Registered()
	{
		long result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Registered(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AutoLoad()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutoLoad(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Loaded()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Loaded(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DisplayAlerts()
	{
		long result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayAlerts(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_RegisteredInHKLM()
	{
		long result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// AddIn 属性
public:

};
