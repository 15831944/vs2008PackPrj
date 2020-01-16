// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CTableStyle 包装类

class CTableStyle : public COleDispatchDriver
{
public:
	CTableStyle(){} // 调用 COleDispatchDriver 默认构造函数
	CTableStyle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTableStyle(const CTableStyle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// TableStyle 方法
public:
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Id()
	{
		CString result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// TableStyle 属性
public:

};
