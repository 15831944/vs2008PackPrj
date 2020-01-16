// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CResampleMediaTasks 包装类

class CResampleMediaTasks : public COleDispatchDriver
{
public:
	CResampleMediaTasks(){} // 调用 COleDispatchDriver 默认构造函数
	CResampleMediaTasks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CResampleMediaTasks(const CResampleMediaTasks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ResampleMediaTasks 方法
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	VARIANT _Index(long Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	void Pause()
	{
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Cancel()
	{
		InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Resume()
	{
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_PercentComplete()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// ResampleMediaTasks 属性
public:

};
