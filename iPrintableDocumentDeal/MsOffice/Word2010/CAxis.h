// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
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
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AxisBetweenCategories(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AxisGroup()
	{
		long result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AxisTitle()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_CategoryNames()
	{
		VARIANT result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_CategoryNames(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_Crosses()
	{
		long result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Crosses(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_CrossesAt()
	{
		double result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_CrossesAt(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Delete()
	{
		VARIANT result;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL get_HasMajorGridlines()
	{
		BOOL result;
		InvokeHelper(0x6002000b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasMajorGridlines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasMinorGridlines()
	{
		BOOL result;
		InvokeHelper(0x6002000d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasMinorGridlines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasTitle()
	{
		BOOL result;
		InvokeHelper(0x6002000f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasTitle(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_MajorGridlines()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_MajorTickMark()
	{
		long result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MajorTickMark(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MajorUnit()
	{
		double result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MajorUnit(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_LogBase()
	{
		double result;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_LogBase(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_TickLabelSpacingIsAuto()
	{
		BOOL result;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TickLabelSpacingIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MajorUnitIsAuto()
	{
		BOOL result;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MajorUnitIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MaximumScale()
	{
		double result;
		InvokeHelper(0x6002001c, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MaximumScale(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x6002001c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MaximumScaleIsAuto()
	{
		BOOL result;
		InvokeHelper(0x6002001e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MaximumScaleIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002001e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MinimumScale()
	{
		double result;
		InvokeHelper(0x60020020, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MinimumScale(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020020, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MinimumScaleIsAuto()
	{
		BOOL result;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MinimumScaleIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_MinorGridlines()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020024, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_MinorTickMark()
	{
		long result;
		InvokeHelper(0x60020025, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MinorTickMark(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020025, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_MinorUnit()
	{
		double result;
		InvokeHelper(0x60020027, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_MinorUnit(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020027, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MinorUnitIsAuto()
	{
		BOOL result;
		InvokeHelper(0x60020029, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MinorUnitIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020029, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ReversePlotOrder()
	{
		BOOL result;
		InvokeHelper(0x6002002b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ReversePlotOrder(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002002b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ScaleType()
	{
		long result;
		InvokeHelper(0x6002002d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ScaleType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002002d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0x6002002f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_TickLabelPosition()
	{
		long result;
		InvokeHelper(0x60020030, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TickLabelPosition(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020030, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_TickLabels()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020032, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_TickLabelSpacing()
	{
		long result;
		InvokeHelper(0x60020033, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TickLabelSpacing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020033, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TickMarkSpacing()
	{
		long result;
		InvokeHelper(0x60020035, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TickMarkSpacing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020035, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x60020037, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Type(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020037, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BaseUnit()
	{
		long result;
		InvokeHelper(0x60020039, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BaseUnit(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020039, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_BaseUnitIsAuto()
	{
		BOOL result;
		InvokeHelper(0x6002003b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_BaseUnitIsAuto(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002003b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MajorUnitScale()
	{
		long result;
		InvokeHelper(0x6002003d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MajorUnitScale(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002003d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MinorUnitScale()
	{
		long result;
		InvokeHelper(0x6002003f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MinorUnitScale(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002003f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CategoryType()
	{
		long result;
		InvokeHelper(0x60020041, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CategoryType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020041, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Left()
	{
		double result;
		InvokeHelper(0x60020043, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Top()
	{
		double result;
		InvokeHelper(0x60020044, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Width()
	{
		double result;
		InvokeHelper(0x60020045, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	double get_Height()
	{
		double result;
		InvokeHelper(0x60020046, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	long get_DisplayUnit()
	{
		long result;
		InvokeHelper(0x60020047, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayUnit(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020047, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_DisplayUnitCustom()
	{
		double result;
		InvokeHelper(0x60020049, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_DisplayUnitCustom(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60020049, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasDisplayUnitLabel()
	{
		BOOL result;
		InvokeHelper(0x6002004b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasDisplayUnitLabel(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002004b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_DisplayUnitLabel()
	{
		LPDISPATCH result;
		InvokeHelper(0x6002004d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Border()
	{
		LPDISPATCH result;
		InvokeHelper(0x6002004e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020050, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
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

	// Axis 属性
public:

};
