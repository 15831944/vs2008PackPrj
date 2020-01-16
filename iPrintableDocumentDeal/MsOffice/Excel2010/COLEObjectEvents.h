// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// COLEObjectEvents 包装类

class COLEObjectEvents : public COleDispatchDriver
{
public:
	COLEObjectEvents(){} // 调用 COleDispatchDriver 默认构造函数
	COLEObjectEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COLEObjectEvents(const COLEObjectEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// OLEObjectEvents 方法
public:
	void GotFocus()
	{
		InvokeHelper(0x605, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LostFocus()
	{
		InvokeHelper(0x606, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// OLEObjectEvents 属性
public:

};
