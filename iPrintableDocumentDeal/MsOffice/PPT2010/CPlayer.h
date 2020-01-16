// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CPlayer 包装类

class CPlayer : public COleDispatchDriver
{
public:
	CPlayer(){} // 调用 COleDispatchDriver 默认构造函数
	CPlayer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPlayer(const CPlayer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Player 方法
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
	void Play()
	{
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Pause()
	{
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Stop()
	{
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GoToNextBookmark()
	{
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GoToPreviousBookmark()
	{
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_CurrentPosition()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CurrentPosition(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_State()
	{
		long result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// Player 属性
public:

};
