// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CSequence 包装类

class CSequence : public COleDispatchDriver
{
public:
	CSequence(){} // 调用 COleDispatchDriver 默认构造函数
	CSequence(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSequence(const CSequence& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Sequence 方法
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	VARIANT _Index(long Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
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
	LPDISPATCH Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH AddEffect(LPDISPATCH Shape, long effectId, long Level, long trigger, long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Shape, effectId, Level, trigger, Index);
		return result;
	}
	LPDISPATCH Clone(LPDISPATCH Effect, long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Effect, Index);
		return result;
	}
	LPDISPATCH FindFirstAnimationFor(LPDISPATCH Shape)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Shape);
		return result;
	}
	LPDISPATCH FindFirstAnimationForClick(long click)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, click);
		return result;
	}
	LPDISPATCH ConvertToBuildLevel(LPDISPATCH Effect, long Level)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Effect, Level);
		return result;
	}
	LPDISPATCH ConvertToAfterEffect(LPDISPATCH Effect, long After, long DimColor, long DimSchemeColor)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Effect, After, DimColor, DimSchemeColor);
		return result;
	}
	LPDISPATCH ConvertToAnimateBackground(LPDISPATCH Effect, long AnimateBackground)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x7d9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Effect, AnimateBackground);
		return result;
	}
	LPDISPATCH ConvertToAnimateInReverse(LPDISPATCH Effect, long animateInReverse)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x7da, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Effect, animateInReverse);
		return result;
	}
	LPDISPATCH ConvertToTextUnitEffect(LPDISPATCH Effect, long unitEffect)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x7db, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Effect, unitEffect);
		return result;
	}
	LPDISPATCH AddTriggerEffect(LPDISPATCH pShape, long effectId, long trigger, LPDISPATCH pTriggerShape, LPCTSTR bookmark, long Level)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 VTS_I4 VTS_DISPATCH VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, pShape, effectId, trigger, pTriggerShape, bookmark, Level);
		return result;
	}

	// Sequence 属性
public:

};
