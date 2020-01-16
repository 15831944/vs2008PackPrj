// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// COLEDBError 包装类

class COLEDBError : public COleDispatchDriver
{
public:
	COLEDBError(){} // 调用 COleDispatchDriver 默认构造函数
	COLEDBError(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COLEDBError(const COLEDBError& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// OLEDBError 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_SqlState()
	{
		CString result;
		InvokeHelper(0x643, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_ErrorString()
	{
		CString result;
		InvokeHelper(0x5d2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_Native()
	{
		long result;
		InvokeHelper(0x769, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Number()
	{
		long result;
		InvokeHelper(0x1c3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Stage()
	{
		long result;
		InvokeHelper(0x76a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// OLEDBError 属性
public:

};
