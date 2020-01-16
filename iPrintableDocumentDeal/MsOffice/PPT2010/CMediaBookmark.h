// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CMediaBookmark 包装类

class CMediaBookmark : public COleDispatchDriver
{
public:
	CMediaBookmark(){} // 调用 COleDispatchDriver 默认构造函数
	CMediaBookmark(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMediaBookmark(const CMediaBookmark& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// MediaBookmark 方法
public:
	long get_Index()
	{
		long result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_Position()
	{
		long result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Delete()
	{
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// MediaBookmark 属性
public:

};
