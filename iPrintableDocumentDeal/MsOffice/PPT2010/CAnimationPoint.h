// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CAnimationPoint 包装类

class CAnimationPoint : public COleDispatchDriver
{
public:
	CAnimationPoint(){} // 调用 COleDispatchDriver 默认构造函数
	CAnimationPoint(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAnimationPoint(const CAnimationPoint& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// AnimationPoint 方法
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
	void Delete()
	{
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	float get_Time()
	{
		float result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Time(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Value()
	{
		VARIANT result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Value(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	CString get_Formula()
	{
		CString result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Formula(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// AnimationPoint 属性
public:

};
