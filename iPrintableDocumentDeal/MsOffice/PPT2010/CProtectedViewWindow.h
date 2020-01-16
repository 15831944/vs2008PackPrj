// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CProtectedViewWindow 包装类

class CProtectedViewWindow : public COleDispatchDriver
{
public:
	CProtectedViewWindow(){} // 调用 COleDispatchDriver 默认构造函数
	CProtectedViewWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CProtectedViewWindow(const CProtectedViewWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ProtectedViewWindow 方法
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
	LPDISPATCH get_Presentation()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Active()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_WindowState()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WindowState(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_SourcePath()
	{
		CString result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_SourceName()
	{
		CString result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	float get_Left()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Left(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Top()
	{
		float result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Top(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Width()
	{
		float result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Width(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Height()
	{
		float result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Height(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7db, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Activate()
	{
		InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Close()
	{
		InvokeHelper(0x7dd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH Edit(LPCTSTR ModifyPassword)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, ModifyPassword);
		return result;
	}
	long get_HWND()
	{
		long result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// ProtectedViewWindow 属性
public:

};
