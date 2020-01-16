// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CCustomerData 包装类

class CCustomerData : public COleDispatchDriver
{
public:
	CCustomerData(){} // 调用 COleDispatchDriver 默认构造函数
	CCustomerData(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomerData(const CCustomerData& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// CustomerData 方法
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
	LPDISPATCH Item(LPCTSTR Id)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Id);
		return result;
	}
	LPDISPATCH Add()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Delete(LPCTSTR Id)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Id);
	}

	// CustomerData 属性
public:

};
