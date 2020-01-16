// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CChartData 包装类

class CChartData : public COleDispatchDriver
{
public:
	CChartData(){} // 调用 COleDispatchDriver 默认构造函数
	CChartData(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartData(const CChartData& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ChartData 方法
public:
	LPDISPATCH get_Workbook()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Activate()
	{
		InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_IsLinked()
	{
		BOOL result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void BreakLink()
	{
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// ChartData 属性
public:

};
