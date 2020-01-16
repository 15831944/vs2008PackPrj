// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSparkVerticalAxis 包装类

class CSparkVerticalAxis : public COleDispatchDriver
{
public:
	CSparkVerticalAxis(){} // 调用 COleDispatchDriver 默认构造函数
	CSparkVerticalAxis(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSparkVerticalAxis(const CSparkVerticalAxis& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SparkVerticalAxis 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_MinScaleType()
	{
		long result;
		InvokeHelper(0xb95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MinScaleType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb95, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_CustomMinScaleValue()
	{
		VARIANT result;
		InvokeHelper(0xb96, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_CustomMinScaleValue(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xb96, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_MaxScaleType()
	{
		long result;
		InvokeHelper(0xb97, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MaxScaleType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb97, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_CustomMaxScaleValue()
	{
		VARIANT result;
		InvokeHelper(0xb98, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_CustomMaxScaleValue(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xb98, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// SparkVerticalAxis 属性
public:

};
