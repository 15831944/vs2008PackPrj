// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CMotionEffect 包装类

class CMotionEffect : public COleDispatchDriver
{
public:
	CMotionEffect(){} // 调用 COleDispatchDriver 默认构造函数
	CMotionEffect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMotionEffect(const CMotionEffect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// MotionEffect 方法
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
	float get_ByX()
	{
		float result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_ByX(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_ByY()
	{
		float result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_ByY(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_FromX()
	{
		float result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_FromX(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_FromY()
	{
		float result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_FromY(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_ToX()
	{
		float result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_ToX(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_ToY()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_ToY(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Path()
	{
		CString result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Path(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// MotionEffect 属性
public:

};
