// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CPane 包装类

class CPane : public COleDispatchDriver
{
public:
	CPane(){} // 调用 COleDispatchDriver 默认构造函数
	CPane(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPane(const CPane& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Pane 方法
public:
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Activate()
	{
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_Active()
	{
		long result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ViewType()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// Pane 属性
public:

};
