// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
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
	CString get__Default()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_EnableResize()
	{
		BOOL result;
		InvokeHelper(0x4a8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableResize(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4a8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Height()
	{
		double result;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Height(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Left()
	{
		double result;
		InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Left(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Top()
	{
		double result;
		InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Top(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Width()
	{
		double result;
		InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Width(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x22e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x22e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_SourceName()
	{
		CString result;
		InvokeHelper(0x2d1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_SourcePath()
	{
		CString result;
		InvokeHelper(0xbb1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_WindowState()
	{
		long result;
		InvokeHelper(0x18c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WindowState(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Workbook()
	{
		LPDISPATCH result;
		InvokeHelper(0x2f0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Activate()
	{
		InvokeHelper(0x130, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL Close()
	{
		BOOL result;
		InvokeHelper(0x115, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Edit(VARIANT& WriteResPassword, VARIANT& UpdateLinks)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x232, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &WriteResPassword, &UpdateLinks);
		return result;
	}

	// ProtectedViewWindow 属性
public:

};
