// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CDocumentWindow 包装类

class CDocumentWindow : public COleDispatchDriver
{
public:
	CDocumentWindow(){} // 调用 COleDispatchDriver 默认构造函数
	CDocumentWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDocumentWindow(const CDocumentWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// DocumentWindow 方法
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
	LPDISPATCH get_Selection()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_View()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Presentation()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ViewType()
	{
		long result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ViewType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BlackAndWhite()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BlackAndWhite(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Active()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_WindowState()
	{
		long result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WindowState(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	float get_Left()
	{
		float result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Left(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Top()
	{
		float result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Top(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7db, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Width()
	{
		float result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Width(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Height()
	{
		float result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Height(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void FitToPage()
	{
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Activate()
	{
		InvokeHelper(0x7df, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LargeScroll(long Down, long Up, long ToRight, long ToLeft)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Down, Up, ToRight, ToLeft);
	}
	void SmallScroll(long Down, long Up, long ToRight, long ToLeft)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Down, Up, ToRight, ToLeft);
	}
	LPDISPATCH NewWindow()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Close()
	{
		InvokeHelper(0x7e3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_HWND()
	{
		long result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActivePane()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Panes()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_SplitVertical()
	{
		long result;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SplitVertical(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SplitHorizontal()
	{
		long result;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SplitHorizontal(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH RangeFromPoint(long X, long Y)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, X, Y);
		return result;
	}
	long PointsToScreenPixelsX(float Points)
	{
		long result;
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7ea, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Points);
		return result;
	}
	long PointsToScreenPixelsY(float Points)
	{
		long result;
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7eb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Points);
		return result;
	}
	void ScrollIntoView(float Left, float Top, float Width, float Height, long Start)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4 ;
		InvokeHelper(0x7ec, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top, Width, Height, Start);
	}
	BOOL IsSectionExpanded(long sectionIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ed, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sectionIndex);
		return result;
	}
	void ExpandSection(long sectionIndex, BOOL Expand)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x7ee, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sectionIndex, Expand);
	}

	// DocumentWindow 属性
public:

};
