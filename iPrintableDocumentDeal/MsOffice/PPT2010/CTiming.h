// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CTiming 包装类

class CTiming : public COleDispatchDriver
{
public:
	CTiming(){} // 调用 COleDispatchDriver 默认构造函数
	CTiming(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTiming(const CTiming& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Timing 方法
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
	float get_Duration()
	{
		float result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Duration(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TriggerType()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TriggerType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_TriggerDelayTime()
	{
		float result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_TriggerDelayTime(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_TriggerShape()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_TriggerShape(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_RepeatCount()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_RepeatCount(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_RepeatDuration()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_RepeatDuration(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Speed()
	{
		float result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Speed(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Accelerate()
	{
		float result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Accelerate(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Decelerate()
	{
		float result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Decelerate(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7db, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AutoReverse()
	{
		long result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutoReverse(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SmoothStart()
	{
		long result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SmoothStart(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SmoothEnd()
	{
		long result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SmoothEnd(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_RewindAtEnd()
	{
		long result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_RewindAtEnd(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7df, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Restart()
	{
		long result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Restart(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BounceEnd()
	{
		long result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BounceEnd(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_BounceEndIntensity()
	{
		float result;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_BounceEndIntensity(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_TriggerBookmark()
	{
		CString result;
		InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_TriggerBookmark(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7e3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// Timing 属性
public:

};
