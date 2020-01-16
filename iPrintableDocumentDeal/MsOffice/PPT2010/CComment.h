// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CComment 包装类

class CComment : public COleDispatchDriver
{
public:
	CComment(){} // 调用 COleDispatchDriver 默认构造函数
	CComment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CComment(const CComment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Comment 方法
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
	CString get_Author()
	{
		CString result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_AuthorInitials()
	{
		CString result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Text()
	{
		CString result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	DATE get_DateTime()
	{
		DATE result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
		return result;
	}
	long get_AuthorIndex()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	float get_Left()
	{
		float result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	float get_Top()
	{
		float result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void Delete()
	{
		InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// Comment 属性
public:

};
