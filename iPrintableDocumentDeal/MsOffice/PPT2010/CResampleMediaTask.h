// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CResampleMediaTask 包装类

class CResampleMediaTask : public COleDispatchDriver
{
public:
	CResampleMediaTask(){} // 调用 COleDispatchDriver 默认构造函数
	CResampleMediaTask(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CResampleMediaTask(const CResampleMediaTask& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ResampleMediaTask 方法
public:
	long get_profile()
	{
		long result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_IsLinked()
	{
		BOOL result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_IsEmbedded()
	{
		BOOL result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_AudioSamplingRate()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_VideoFrameRate()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_SampleHeight()
	{
		long result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_SampleWidth()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_ContainerType()
	{
		CString result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_VideoCompressionType()
	{
		CString result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_AudioCompressionType()
	{
		CString result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Shape()
	{
		LPDISPATCH result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// ResampleMediaTask 属性
public:

};
