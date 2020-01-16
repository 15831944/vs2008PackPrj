// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CAxis 包装类

class CAxis : public COleDispatchDriver
{
public:
	CAxis(){} // 调用 COleDispatchDriver 默认构造函数
	CAxis(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAxis(const CAxis& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Axis 方法
public:
	BOOL get_AxisBetweenCategories()
	{
		BOOL result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AxisBetweenCategories(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AxisGroup()
	{
		long result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AxisTitle()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_CategoryNames()
	{
		VARIANT result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_CategoryNames(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_Crosses()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Crosses(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_CrossesAt()
	{
		double result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_CrossesAt(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL get_HasMajorGridlines()
	{
		BOOL result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasMajorGridlines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasMinorGridlines()
	{
		BOOL result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasMinorGridlines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasTitle()
	{
		BOOL result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasTitle(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_MajorGridlines()
	{
		LPDISPATCH result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_MajorTickMark()
	{
		long result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MajorTickMark(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MajorUnit()
	{
		double result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MajorUnit(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_LogBase()
	{
		double result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_LogBase(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_TickLabelSpacingIsAuto()
	{
		BOOL result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TickLabelSpacingIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7df, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MajorUnitIsAuto()
	{
		BOOL result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MajorUnitIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MaximumScale()
	{
		double result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MaximumScale(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MaximumScaleIsAuto()
	{
		BOOL result;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MaximumScaleIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MinimumScale()
	{
		double result;
		InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MinimumScale(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7e3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MinimumScaleIsAuto()
	{
		BOOL result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MinimumScaleIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_MinorGridlines()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_MinorTickMark()
	{
		long result;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MinorTickMark(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MinorUnit()
	{
		double result;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MinorUnit(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MinorUnitIsAuto()
	{
		BOOL result;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MinorUnitIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7e8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ReversePlotOrder()
	{
		BOOL result;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ReversePlotOrder(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ScaleType()
	{
		long result;
		InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ScaleType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x7eb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_TickLabelPosition()
	{
		long result;
		InvokeHelper(0x7ec, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TickLabelPosition(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_TickLabels()
	{
		LPDISPATCH result;
		InvokeHelper(0x7ed, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_TickLabelSpacing()
	{
		long result;
		InvokeHelper(0x7ee, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TickLabelSpacing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ee, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TickMarkSpacing()
	{
		long result;
		InvokeHelper(0x7ef, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TickMarkSpacing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ef, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x7f0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Type(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BaseUnit()
	{
		long result;
		InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BaseUnit(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_BaseUnitIsAuto()
	{
		BOOL result;
		InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_BaseUnitIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7f2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MajorUnitScale()
	{
		long result;
		InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MajorUnitScale(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MinorUnitScale()
	{
		long result;
		InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MinorUnitScale(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CategoryType()
	{
		long result;
		InvokeHelper(0x7f5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CategoryType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Left()
	{
		double result;
		InvokeHelper(0x7f6, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Top()
	{
		double result;
		InvokeHelper(0x7f7, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Width()
	{
		double result;
		InvokeHelper(0x7f8, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Height()
	{
		double result;
		InvokeHelper(0x7f9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	long get_DisplayUnit()
	{
		long result;
		InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayUnit(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7fa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_DisplayUnitCustom()
	{
		double result;
		InvokeHelper(0x7fb, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_DisplayUnitCustom(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7fb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasDisplayUnitLabel()
	{
		BOOL result;
		InvokeHelper(0x7fc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasDisplayUnitLabel(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7fc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_DisplayUnitLabel()
	{
		LPDISPATCH result;
		InvokeHelper(0x7fd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Border()
	{
		LPDISPATCH result;
		InvokeHelper(0x7fe, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x7ff, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
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
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x800, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// Axis 属性
public:

};
