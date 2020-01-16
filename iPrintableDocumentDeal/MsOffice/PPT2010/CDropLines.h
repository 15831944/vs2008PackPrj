// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CDropLines 包装类

class CDropLines : public COleDispatchDriver
{
public:
	CDropLines(){} // 调用 COleDispatchDriver 默认构造函数
	CDropLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDropLines(const CDropLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// DropLines 方法
public:
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Border()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}

	// DropLines 属性
public:

};
