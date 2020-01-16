// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CPowerRex 包装类

class CPowerRex : public COleDispatchDriver
{
public:
	CPowerRex(){} // 调用 COleDispatchDriver 默认构造函数
	CPowerRex(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPowerRex(const CPowerRex& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// _PowerRex 方法
public:
	void OnAsfEncoderEvent(VARIANT& erorCode, VARIANT& bstrErrorDesc)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &erorCode, &bstrErrorDesc);
	}

	// _PowerRex 属性
public:

};
