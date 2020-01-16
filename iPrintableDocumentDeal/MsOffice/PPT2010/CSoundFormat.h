// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CSoundFormat 包装类

class CSoundFormat : public COleDispatchDriver
{
public:
	CSoundFormat(){} // 调用 COleDispatchDriver 默认构造函数
	CSoundFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSoundFormat(const CSoundFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SoundFormat 方法
public:
	void Play()
	{
		InvokeHelper(0x7d0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Import(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	long Export(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_SourceFullName()
	{
		CString result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// SoundFormat 属性
public:

};
