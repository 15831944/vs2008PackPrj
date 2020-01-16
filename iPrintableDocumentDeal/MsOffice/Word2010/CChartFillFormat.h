// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
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
	void OneColorGradient(long Style, long Variant, float Degree)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style, Variant, Degree);
	}
	void TwoColorGradient(long Style, long Variant)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style, Variant);
	}
	void PresetTextured(long PresetTexture)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, PresetTexture);
	}
	void Solid()
	{
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Patterned(long Pattern)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Pattern);
	}
	void UserPicture(VARIANT& PictureFile, VARIANT& PictureFormat, VARIANT& PictureStackUnit, VARIANT& PicturePlacement)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &PictureFile, &PictureFormat, &PictureStackUnit, &PicturePlacement);
	}
	void UserTextured(LPCTSTR TextureFile)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TextureFile);
	}
	void PresetGradient(long Style, long Variant, long PresetGradientType)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style, Variant, PresetGradientType);
	}
	LPDISPATCH get_BackColor()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ForeColor()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020009, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_GradientColorType()
	{
		long result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	float get_GradientDegree()
	{
		float result;
		InvokeHelper(0x6002000b, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	long get_GradientStyle()
	{
		long result;
		InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_GradientVariant()
	{
		long result;
		InvokeHelper(0x6002000d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Pattern()
	{
		long result;
		InvokeHelper(0x6002000e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_PresetGradientType()
	{
		long result;
		InvokeHelper(0x6002000f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_PresetTexture()
	{
		long result;
		InvokeHelper(0x60020010, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_TextureName()
	{
		CString result;
		InvokeHelper(0x60020011, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_TextureType()
	{
		long result;
		InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Type()
	{
		long result;
		InvokeHelper(0x60020013, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Visible()
	{
		long result;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Visible(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
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

	// ChartFillFormat 属性
public:

};
