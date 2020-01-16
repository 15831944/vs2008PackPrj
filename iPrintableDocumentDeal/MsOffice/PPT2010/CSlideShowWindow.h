// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CSlideShowWindow 包装类

class CSlideShowWindow : public COleDispatchDriver
{
public:
	CSlideShowWindow(){} // 调用 COleDispatchDriver 默认构造函数
	CSlideShowWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSlideShowWindow(const CSlideShowWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SlideShowWindow 方法
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
	LPDISPATCH get_View()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Presentation()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_IsFullScreen()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	float get_Left()
	{
		float result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Left(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Top()
	{
		float result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Top(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Width()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Width(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Height()
	{
		float result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Height(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HWND()
	{
		long result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Active()
	{
		long result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Activate()
	{
		InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// SlideShowWindow 属性
public:

};
