// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CView1 包装类

class CView1 : public COleDispatchDriver
{
public:
	CView1(){} // 调用 COleDispatchDriver 默认构造函数
	CView1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CView1(const CView1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// View 方法
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
	long get_Type()
	{
		long result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Zoom()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Zoom(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Paste()
	{
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Slide()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_Slide(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void GotoSlide(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long get_DisplaySlideMiniature()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplaySlideMiniature(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ZoomToFit()
	{
		long result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ZoomToFit(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void PasteSpecial(long DataType, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DataType, DisplayAsIcon, IconFileName, IconIndex, IconLabel, Link);
	}
	LPDISPATCH get_PrintOptions()
	{
		LPDISPATCH result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void PrintOut(long From, long To, LPCTSTR PrintToFile, long Copies, long Collate)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, From, To, PrintToFile, Copies, Collate);
	}
	LPDISPATCH Player(VARIANT& ShapeId)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7dd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &ShapeId);
		return result;
	}
	long get_MediaControlsVisible()
	{
		long result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsLeft()
	{
		float result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsTop()
	{
		float result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsWidth()
	{
		float result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_MediaControlsHeight()
	{
		float result;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}

	// View 属性
public:

};
