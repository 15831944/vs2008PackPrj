// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CPrintRange 包装类

class CPrintRange : public COleDispatchDriver
{
public:
	CPrintRange(){} // 调用 COleDispatchDriver 默认构造函数
	CPrintRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPrintRange(const CPrintRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// PrintRange 方法
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
	long get_Start()
	{
		long result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_End()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Delete()
	{
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// PrintRange 属性
public:

};
