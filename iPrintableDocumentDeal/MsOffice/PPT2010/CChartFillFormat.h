// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CChartFillFormat 包装类

class CChartFillFormat : public COleDispatchDriver
{
public:
	CChartFillFormat(){} // 调用 COleDispatchDriver 默认构造函数
	CChartFillFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartFillFormat(const CChartFillFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// ChartFillFormat 方法
public:
	void Solid()
	{
		InvokeHelper(0x7d1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void UserTextured(LPCTSTR TextureFile)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TextureFile);
	}
	LPDISPATCH get_BackColor()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ForeColor()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	float get_GradientDegree()
	{
		float result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	CString get_TextureName()
	{
		CString result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
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
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void OneColorGradient(long Style, long Variant, float Degree)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R4 ;
		InvokeHelper(0x7d8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style, Variant, Degree);
	}
	long get_GradientColorType()
	{
		long result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_GradientStyle()
	{
		long result;
		InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_GradientVariant()
	{
		long result;
		InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Pattern()
	{
		long result;
		InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Patterned(long Pattern)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Pattern);
	}
	void PresetGradient(long Style, long Variant, long PresetGradientType)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style, Variant, PresetGradientType);
	}
	long get_PresetGradientType()
	{
		long result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_PresetTexture()
	{
		long result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void PresetTextured(long PresetTexture)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, PresetTexture);
	}
	long get_TextureType()
	{
		long result;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void TwoColorGradient(long Style, long Variant)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style, Variant);
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void UserPicture(VARIANT& PictureFile, VARIANT& PictureFormat, VARIANT& PictureStackUnit, VARIANT& PicturePlacement)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &PictureFile, &PictureFormat, &PictureStackUnit, &PicturePlacement);
	}
	long get_Visible()
	{
		long result;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Visible(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ChartFillFormat 属性
public:

};
