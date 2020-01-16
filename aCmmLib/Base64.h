// Base64.h: interface for the CBase64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64_H__75A5AF36_DA7F_4E19_B741_3E7A0E95FE38__INCLUDED_)
#define AFX_BASE64_H__75A5AF36_DA7F_4E19_B741_3E7A0E95FE38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _AA_DLL_EXPORT_ CBase64  
{
public:
	CBase64();
	static int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
	static int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen);
	virtual ~CBase64();
	static int Base64DecodeSize(const char* pBase64);	//获取Base64解码长度
	static int Base64EncodeSize(int nOrgSize);			//获取Base64编码长度

};

#endif // !defined(AFX_BASE64_H__75A5AF36_DA7F_4E19_B741_3E7A0E95FE38__INCLUDED_)
