// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CAutoCorrect 包装类

class CAutoCorrect : public COleDispatchDriver
{
public:
	CAutoCorrect(){} // 调用 COleDispatchDriver 默认构造函数
	CAutoCorrect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutoCorrect(const CAutoCorrect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// AutoCorrect 方法
public:
	BOOL get_DisplayAutoCorrectOptions()
	{
		BOOL result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayAutoCorrectOptions(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayAutoLayoutOptions()
	{
		BOOL result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayAutoLayoutOptions(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// AutoCorrect 属性
public:

};
