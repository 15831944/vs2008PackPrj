// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
// CMediaFormat 包装类

class CMediaFormat : public COleDispatchDriver
{
public:
	CMediaFormat(){} // 调用 COleDispatchDriver 默认构造函数
	CMediaFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMediaFormat(const CMediaFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// MediaFormat 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	float get_Volume()
	{
		float result;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Volume(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x7d2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Muted()
	{
		BOOL result;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Muted(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Length()
	{
		long result;
		InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_StartPoint()
	{
		long result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_StartPoint(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EndPoint()
	{
		long result;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EndPoint(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FadeInDuration()
	{
		long result;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FadeInDuration(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FadeOutDuration()
	{
		long result;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FadeOutDuration(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_MediaBookmarks()
	{
		LPDISPATCH result;
		InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SetDisplayPicture(long Position)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Position);
	}
	void SetDisplayPictureFromFile(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7db, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void Resample(BOOL Trim, long SampleHeight, long SampleWidth, long VideoFrameRate, long AudioSamplingRate, long VideoBitRate)
	{
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Trim, SampleHeight, SampleWidth, VideoFrameRate, AudioSamplingRate, VideoBitRate);
	}
	void ResampleFromProfile(long profile)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7dd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, profile);
	}
	long get_ResamplingStatus()
	{
		long result;
		InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_IsLinked()
	{
		BOOL result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_IsEmbedded()
	{
		BOOL result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_AudioSamplingRate()
	{
		long result;
		InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_VideoFrameRate()
	{
		long result;
		InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_SampleHeight()
	{
		long result;
		InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_SampleWidth()
	{
		long result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_VideoCompressionType()
	{
		CString result;
		InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_AudioCompressionType()
	{
		CString result;
		InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// MediaFormat 属性
public:

};
