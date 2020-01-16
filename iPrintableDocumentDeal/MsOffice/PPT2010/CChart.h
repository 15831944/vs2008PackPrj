// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CChart 包装类

class CChart : public COleDispatchDriver
{
public:
	CChart(){} // 调用 COleDispatchDriver 默认构造函数
	CChart(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChart(const CChart& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// Chart 方法
public:
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ApplyDataLabels(long Type, VARIANT& LegendKey, VARIANT& AutoText, VARIANT& HasLeaderLines, VARIANT& ShowSeriesName, VARIANT& ShowCategoryName, VARIANT& ShowValue, VARIANT& ShowPercentage, VARIANT& ShowBubbleSize, VARIANT& Separator)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x782, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Type, &LegendKey, &AutoText, &HasLeaderLines, &ShowSeriesName, &ShowCategoryName, &ShowValue, &ShowPercentage, &ShowBubbleSize, &Separator);
	}
	long get_ChartType()
	{
		long result;
		InvokeHelper(0x578, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ChartType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x578, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasDataTable()
	{
		BOOL result;
		InvokeHelper(0x574, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasDataTable(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x574, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void ApplyCustomType(long ChartType, VARIANT& TypeName)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x579, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ChartType, &TypeName);
	}
	void GetChartElement(long X, long Y, long * ElementID, long * Arg1, long * Arg2)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x581, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y, ElementID, Arg1, Arg2);
	}
	void SetSourceData(LPCTSTR Source, VARIANT& PlotBy)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x585, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Source, &PlotBy);
	}
	long get_PlotBy()
	{
		long result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PlotBy(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void AutoFormat(long Gallery, VARIANT& Format)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Gallery, &Format);
	}
	void SetBackgroundPicture(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	LPDISPATCH get_DataTable()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Paste(VARIANT& Type)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Type);
	}
	long get_BarShape()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BarShape(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetDefaultChart(VARIANT& Name)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Name);
	}
	void ApplyChartTemplate(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveChartTemplate(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	LPDISPATCH get_SideWall()
	{
		LPDISPATCH result;
		InvokeHelper(0x949, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_BackWall()
	{
		LPDISPATCH result;
		InvokeHelper(0x94a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_ChartStyle()
	{
		VARIANT result;
		InvokeHelper(0x9a1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ChartStyle(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x9a1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void ClearToMatchStyle()
	{
		InvokeHelper(0x9a2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_HasPivotFields()
	{
		BOOL result;
		InvokeHelper(0x717, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasPivotFields(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x717, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_ShowDataLabelsOverMaximum(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowDataLabelsOverMaximum()
	{
		BOOL result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void ApplyLayout(long Layout, VARIANT& ChartType)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x9a4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Layout, &ChartType);
	}
	void Refresh()
	{
		InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_ChartData()
	{
		LPDISPATCH result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Shapes()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Area3DGroup()
	{
		LPDISPATCH result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH AreaGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_Bar3DGroup()
	{
		LPDISPATCH result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH BarGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_Column3DGroup()
	{
		LPDISPATCH result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH ColumnGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_Line3DGroup()
	{
		LPDISPATCH result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH LineGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_Pie3DGroup()
	{
		LPDISPATCH result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH PieGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH DoughnutGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH RadarGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_SurfaceGroup()
	{
		LPDISPATCH result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH XYGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void _ApplyDataLabels(long Type, VARIANT& LegendKey, VARIANT& AutoText, VARIANT& HasLeaderLines)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Type, &LegendKey, &AutoText, &HasLeaderLines);
	}
	void put_AutoScaling(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7df, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoScaling()
	{
		BOOL result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Axes(VARIANT& Type, long AxisGroup)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x7e0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Type, AxisGroup);
		return result;
	}
	LPDISPATCH get_ChartArea()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH ChartGroups(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7e2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPDISPATCH get_ChartTitle()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ChartWizard(VARIANT& Source, VARIANT& Gallery, VARIANT& Format, VARIANT& PlotBy, VARIANT& CategoryLabels, VARIANT& SeriesLabels, VARIANT& HasLegend, VARIANT& Title, VARIANT& CategoryTitle, VARIANT& ValueTitle, VARIANT& ExtraTitle)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7e4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Source, &Gallery, &Format, &PlotBy, &CategoryLabels, &SeriesLabels, &HasLegend, &Title, &CategoryTitle, &ValueTitle, &ExtraTitle);
	}
	void Copy(VARIANT& Before, VARIANT& After)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Before, &After);
	}
	void CopyPicture(long Appearance, long Format, long Size)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Appearance, Format, Size);
	}
	LPDISPATCH get_Corners()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Delete()
	{
		InvokeHelper(0x7e8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_DepthPercent()
	{
		long result;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DepthPercent(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DisplayBlanksAs()
	{
		long result;
		InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayBlanksAs(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Elevation()
	{
		long result;
		InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Elevation(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL Export(LPCTSTR FileName, VARIANT& FilterName, VARIANT& Interactive)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7ec, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, FileName, &FilterName, &Interactive);
		return result;
	}
	LPDISPATCH get_Floor()
	{
		LPDISPATCH result;
		InvokeHelper(0x7ed, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_GapDepth()
	{
		long result;
		InvokeHelper(0x7ee, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_GapDepth(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7ee, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_HasAxis(VARIANT& Index1, VARIANT& Index2)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7ef, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index1, &Index2);
		return result;
	}
	void put_HasAxis(VARIANT& Index1, VARIANT& Index2, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7ef, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &Index1, &Index2, &newValue);
	}
	BOOL get_HasLegend()
	{
		BOOL result;
		InvokeHelper(0x7f0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasLegend(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7f0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasTitle()
	{
		BOOL result;
		InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasTitle(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7f1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HeightPercent()
	{
		long result;
		InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_HeightPercent(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Legend()
	{
		LPDISPATCH result;
		InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Perspective()
	{
		long result;
		InvokeHelper(0x7f5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Perspective(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_PlotArea()
	{
		LPDISPATCH result;
		InvokeHelper(0x7f6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_PlotVisibleOnly()
	{
		BOOL result;
		InvokeHelper(0x7f7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PlotVisibleOnly(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7f7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_RightAngleAxes()
	{
		VARIANT result;
		InvokeHelper(0x7f8, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_RightAngleAxes(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7f8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Rotation()
	{
		VARIANT result;
		InvokeHelper(0x7f9, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Rotation(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7f9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void Select(VARIANT& Replace)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7fa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Replace);
	}
	LPDISPATCH SeriesCollection(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x7fb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	void SetElement(long Element)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7fc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Element);
	}
	long get_Subtype()
	{
		long result;
		InvokeHelper(0x7fd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Subtype(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7fd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x7fe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Type(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7fe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Walls()
	{
		LPDISPATCH result;
		InvokeHelper(0x7ff, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0x800, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_ShowReportFilterFieldButtons()
	{
		BOOL result;
		InvokeHelper(0x801, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowReportFilterFieldButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x801, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowLegendFieldButtons()
	{
		BOOL result;
		InvokeHelper(0x802, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowLegendFieldButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x802, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowAxisFieldButtons()
	{
		BOOL result;
		InvokeHelper(0x803, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowAxisFieldButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x803, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowValueFieldButtons()
	{
		BOOL result;
		InvokeHelper(0x804, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowValueFieldButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x804, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowAllFieldButtons()
	{
		BOOL result;
		InvokeHelper(0x805, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowAllFieldButtons(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x805, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_AlternativeText()
	{
		CString result;
		InvokeHelper(0x806, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_AlternativeText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x806, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Title()
	{
		CString result;
		InvokeHelper(0x807, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Title(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x807, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// Chart 属性
public:

};
