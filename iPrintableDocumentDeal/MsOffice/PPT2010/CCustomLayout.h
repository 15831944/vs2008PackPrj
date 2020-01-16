// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CCustomLayout 包装类

class CCustomLayout : public COleDispatchDriver
{
public:
	CCustomLayout(){} // 调用 COleDispatchDriver 默认构造函数
	CCustomLayout(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomLayout(const CCustomLayout& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// CustomLayout 方法
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
	LPDISPATCH get_Shapes()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_HeadersFooters()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Background()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Delete()
	{
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	float get_Height()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_Width()
	{
		float result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Hyperlinks()
	{
		LPDISPATCH result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Design()
	{
		LPDISPATCH result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_TimeLine()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SlideShowTransition()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_MatchingName()
	{
		CString result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_MatchingName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Preserved()
	{
		long result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Preserved(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7df, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Index()
	{
		long result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Select()
	{
		InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Cut()
	{
		InvokeHelper(0x7e2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Copy()
	{
		InvokeHelper(0x7e3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH Duplicate()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void MoveTo(long toPos)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, toPos);
	}
	long get_DisplayMasterShapes()
	{
		long result;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayMasterShapes(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FollowMasterBackground()
	{
		long result;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FollowMasterBackground(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_ThemeColorScheme()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CustomerData()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// CustomLayout 属性
public:

};
