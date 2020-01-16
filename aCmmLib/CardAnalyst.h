#pragma once
#include "stdafx.h"

class CCardAnalyst
{
public:
	CCardAnalyst(void);
	virtual ~CCardAnalyst(void);

	static CCardAnalyst& GetInstance()
	{
		static CCardAnalyst one;
		return one;
	}
	CString HandleCardNo(CString& szParam);//卡号处理入口函数

	CString DecToHex(CString& szParam);//十进制字符串转16进制字符串
	CString HexToDec(CString& szParam);//16进制字符串转10进制字符串
	CString DecRevsToHex(CString& szParam);//10进制字符串反转16进制字符串
	CString DecRevsToDec(CString& szParam);//10进制字符串反转10进制字符串
	CString HexRevsToDec(CString& szParam);//16进制字符串反转10进制字符串
	CString HexRevsToHex(CString& szParam);//16进制字符串反转16进制字符串
	CString Intercept(CString& szParam);//截取右边位数
	CString AddToLeft(CString& szParam);//左侧填充
	CString AddToRight(CString& szParam);//右侧填充
	CString Wiegand34To26(CString& szParam);//韦根34转26
	CString Concat( CString& szParam);
	CString InterceptLeft(CString& szParam);
	CString InterceptRight(CString& szParam);


	BOOL Split(CString& source, CString& funout, CString& paramout);
	BOOL CheckValidate(CString& szParam);//检测语句合法性
};
