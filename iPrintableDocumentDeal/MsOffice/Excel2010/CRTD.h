// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CRTD 包装类

class CRTD : public COleDispatchDriver
{
public:
	CRTD(){} // 调用 COleDispatchDriver 默认构造函数
	CRTD(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRTD(const CRTD& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// RTD 方法
public:
	long get_ThrottleInterval()
	{
		long result;
		InvokeHelper(0x8c0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ThrottleInterval(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8c0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void RefreshData()
	{
		InvokeHelper(0x8c1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RestartServers()
	{
		InvokeHelper(0x8c2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// RTD 属性
public:

};
