// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CValueChange 包装类

class CValueChange : public COleDispatchDriver
{
public:
	CValueChange(){} // 调用 COleDispatchDriver 默认构造函数
	CValueChange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CValueChange(const CValueChange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ValueChange 方法
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
	long get_Order()
	{
		long result;
		InvokeHelper(0xc0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_VisibleInPivotTable()
	{
		BOOL result;
		InvokeHelper(0xb9b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PivotCell()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Tuple()
	{
		CString result;
		InvokeHelper(0xb9c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	double get_Value()
	{
		double result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	long get_AllocationValue()
	{
		long result;
		InvokeHelper(0xb3a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_AllocationMethod()
	{
		long result;
		InvokeHelper(0xb3b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_AllocationWeightExpression()
	{
		CString result;
		InvokeHelper(0xb3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void Delete()
	{
		InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// ValueChange 属性
public:

};
