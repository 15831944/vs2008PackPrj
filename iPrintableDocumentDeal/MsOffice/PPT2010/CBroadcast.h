// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CBroadcast 包装类

class CBroadcast : public COleDispatchDriver
{
public:
	CBroadcast(){} // 调用 COleDispatchDriver 默认构造函数
	CBroadcast(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBroadcast(const CBroadcast& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Broadcast 方法
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
	void Start(LPCTSTR serverUrl)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, serverUrl);
	}
	void End()
	{
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_AttendeeUrl()
	{
		CString result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_IsBroadcasting()
	{
		BOOL result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}

	// Broadcast 属性
public:

};
