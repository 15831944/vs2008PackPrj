// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CSlideShowView 包装类

class CSlideShowView : public COleDispatchDriver
{
public:
	CSlideShowView(){} // 调用 COleDispatchDriver 默认构造函数
	CSlideShowView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSlideShowView(const CSlideShowView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SlideShowView 方法
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
	long get_Zoom()
	{
		long result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Slide()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_PointerType()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PointerType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_State()
	{
		long result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_State(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AcceleratorsEnabled()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AcceleratorsEnabled(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_PresentationElapsedTime()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_SlideElapsedTime()
	{
		float result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_SlideElapsedTime(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_LastSlideViewed()
	{
		LPDISPATCH result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_AdvanceMode()
	{
		long result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PointerColor()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_IsNamedShow()
	{
		long result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_SlideShowName()
	{
		CString result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void DrawLine(float BeginX, float BeginY, float EndX, float EndY)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4 ;
		InvokeHelper(0x7df, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BeginX, BeginY, EndX, EndY);
	}
	void EraseDrawing()
	{
		InvokeHelper(0x7e0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void First()
	{
		InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Last()
	{
		InvokeHelper(0x7e2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Next()
	{
		InvokeHelper(0x7e3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Previous()
	{
		InvokeHelper(0x7e4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GotoSlide(long Index, long ResetSlide)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index, ResetSlide);
	}
	void GotoNamedShow(LPCTSTR SlideShowName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7e6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SlideShowName);
	}
	void EndNamedShow()
	{
		InvokeHelper(0x7e7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ResetSlideTime()
	{
		InvokeHelper(0x7e8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Exit()
	{
		InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void InstallTracker(LPUNKNOWN pTracker, long Presenter)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_I4 ;
		InvokeHelper(0x7ea, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pTracker, Presenter);
	}
	long get_CurrentShowPosition()
	{
		long result;
		InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void GotoClick(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ec, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long GetClickIndex()
	{
		long result;
		InvokeHelper(0x7ed, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetClickCount()
	{
		long result;
		InvokeHelper(0x7ee, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL FirstAnimationIsAutomatic()
	{
		BOOL result;
		InvokeHelper(0x7ef, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Player(VARIANT& ShapeId)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7f0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &ShapeId);
		return result;
	}
	long get_MediaControlsVisible()
	{
		long result;
		InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsLeft()
	{
		float result;
		InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsTop()
	{
		float result;
		InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsWidth()
	{
		float result;
		InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsHeight()
	{
		float result;
		InvokeHelper(0x7f5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}

	// SlideShowView 属性
public:

};
