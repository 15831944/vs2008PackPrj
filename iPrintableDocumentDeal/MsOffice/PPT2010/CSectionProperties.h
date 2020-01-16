// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CSectionProperties 包装类

class CSectionProperties : public COleDispatchDriver
{
public:
	CSectionProperties(){} // 调用 COleDispatchDriver 默认构造函数
	CSectionProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSectionProperties(const CSectionProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// SectionProperties 方法
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
	long get_Count()
	{
		long result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString Name(long sectionIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sectionIndex);
		return result;
	}
	void Rename(long sectionIndex, LPCTSTR sectionName)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sectionIndex, sectionName);
	}
	long SlidesCount(long sectionIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sectionIndex);
		return result;
	}
	long FirstSlide(long sectionIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sectionIndex);
		return result;
	}
	long AddBeforeSlide(long SlideIndex, LPCTSTR sectionName)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x7d8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SlideIndex, sectionName);
		return result;
	}
	long AddSection(long sectionIndex, VARIANT& sectionName)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x7d9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sectionIndex, &sectionName);
		return result;
	}
	void Move(long sectionIndex, long toPos)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sectionIndex, toPos);
	}
	void Delete(long sectionIndex, BOOL deleteSlides)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x7db, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sectionIndex, deleteSlides);
	}
	CString SectionID(long sectionIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sectionIndex);
		return result;
	}

	// SectionProperties 属性
public:

};
