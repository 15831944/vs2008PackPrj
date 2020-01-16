// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CSparkPoints 包装类

class CSparkPoints : public COleDispatchDriver
{
public:
	CSparkPoints(){} // 调用 COleDispatchDriver 默认构造函数
	CSparkPoints(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSparkPoints(const CSparkPoints& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SparkPoints 方法
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
	LPDISPATCH get_Negative()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Markers()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Highpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Lowpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Firstpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb8f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Lastpoint()
	{
		LPDISPATCH result;
		InvokeHelper(0xb90, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// SparkPoints 属性
public:

};
