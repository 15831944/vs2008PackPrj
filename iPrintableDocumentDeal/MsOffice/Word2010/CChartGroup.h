// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
// CChartGroup 包装类

class CChartGroup : public COleDispatchDriver
{
public:
	CChartGroup(){} // 调用 COleDispatchDriver 默认构造函数
	CChartGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartGroup(const CChartGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ChartGroup 方法
public:
	long get_AxisGroup()
	{
		long result;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AxisGroup(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DoughnutHoleSize()
	{
		long result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DoughnutHoleSize(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_DownBars()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DropLines()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_FirstSliceAngle()
	{
		long result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FirstSliceAngle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_GapWidth()
	{
		long result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_GapWidth(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasDropLines()
	{
		BOOL result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasDropLines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasHiLoLines()
	{
		BOOL result;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasHiLoLines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasRadarAxisLabels()
	{
		BOOL result;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasRadarAxisLabels(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasSeriesLines()
	{
		BOOL result;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasSeriesLines(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasUpDownBars()
	{
		BOOL result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasUpDownBars(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_HiLoLines()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Index()
	{
		long result;
		InvokeHelper(0x60020015, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Overlap()
	{
		long result;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Overlap(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_RadarAxisLabels()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH SeriesCollection(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60020019, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_SeriesLines()
	{
		LPDISPATCH result;
		InvokeHelper(0x6002001a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_SubType()
	{
		long result;
		InvokeHelper(0x6002001b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SubType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002001b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x6002001d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Type(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002001d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_UpBars()
	{
		LPDISPATCH result;
		InvokeHelper(0x6002001f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_VaryByCategories()
	{
		BOOL result;
		InvokeHelper(0x60020020, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_VaryByCategories(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020020, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SizeRepresents()
	{
		long result;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SizeRepresents(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BubbleScale()
	{
		long result;
		InvokeHelper(0x60020024, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BubbleScale(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020024, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowNegativeBubbles()
	{
		BOOL result;
		InvokeHelper(0x60020026, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowNegativeBubbles(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020026, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SplitType()
	{
		long result;
		InvokeHelper(0x60020028, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SplitType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020028, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_SplitValue()
	{
		VARIANT result;
		InvokeHelper(0x6002002a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_SplitValue(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6002002a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_SecondPlotSize()
	{
		long result;
		InvokeHelper(0x6002002c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SecondPlotSize(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002002c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Has3DShading()
	{
		BOOL result;
		InvokeHelper(0x6002002e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Has3DShading(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6002002e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
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

	// ChartGroup 属性
public:

};
