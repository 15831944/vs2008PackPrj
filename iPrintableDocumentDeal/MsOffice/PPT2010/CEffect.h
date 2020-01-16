// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CEffect 包装类

class CEffect : public COleDispatchDriver
{
public:
	CEffect(){} // 调用 COleDispatchDriver 默认构造函数
	CEffect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CEffect(const CEffect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Effect 方法
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
	LPDISPATCH get_Shape()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_Shape(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void MoveTo(long toPos)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, toPos);
	}
	void MoveBefore(LPDISPATCH Effect)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Effect);
	}
	void MoveAfter(LPDISPATCH Effect)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Effect);
	}
	void Delete()
	{
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_Index()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Timing()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_EffectType()
	{
		long result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EffectType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_EffectParameters()
	{
		LPDISPATCH result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_TextRangeStart()
	{
		long result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_TextRangeLength()
	{
		long result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Paragraph()
	{
		long result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Paragraph(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DisplayName()
	{
		CString result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_Exit()
	{
		long result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Exit(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Behaviors()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EffectInformation()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// Effect 属性
public:

};
