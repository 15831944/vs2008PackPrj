// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CFileExportConverter 包装类

class CFileExportConverter : public COleDispatchDriver
{
public:
	CFileExportConverter(){} // 调用 COleDispatchDriver 默认构造函数
	CFileExportConverter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFileExportConverter(const CFileExportConverter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// FileExportConverter 方法
public:
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
	CString get_Extensions()
	{
		CString result;
		InvokeHelper(0xad1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Description()
	{
		CString result;
		InvokeHelper(0xda, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_FileFormat()
	{
		long result;
		InvokeHelper(0x120, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// FileExportConverter 属性
public:

};
