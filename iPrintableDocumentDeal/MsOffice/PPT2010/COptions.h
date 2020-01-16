// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// COptions 包装类

class COptions : public COleDispatchDriver
{
public:
	COptions(){} // 调用 COleDispatchDriver 默认构造函数
	COptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COptions(const COptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Options 方法
public:
	long get_DisplayPasteOptions()
	{
		long result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayPasteOptions(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DoNotPromptForConvert()
	{
		long result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DoNotPromptForConvert(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowCoauthoringMergeChanges()
	{
		BOOL result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowCoauthoringMergeChanges(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// Options 属性
public:

};
