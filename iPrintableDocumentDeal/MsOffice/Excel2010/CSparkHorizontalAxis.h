// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSparkHorizontalAxis 包装类

class CSparkHorizontalAxis : public COleDispatchDriver
{
public:
	CSparkHorizontalAxis(){} // 调用 COleDispatchDriver 默认构造函数
	CSparkHorizontalAxis(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSparkHorizontalAxis(const CSparkHorizontalAxis& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SparkHorizontalAxis 方法
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
	LPDISPATCH get_Axis()
	{
		LPDISPATCH result;
		InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_IsDateAxis()
	{
		BOOL result;
		InvokeHelper(0xb93, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_RightToLeftPlotOrder()
	{
		BOOL result;
		InvokeHelper(0xb94, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_RightToLeftPlotOrder(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xb94, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// SparkHorizontalAxis 属性
public:

};
