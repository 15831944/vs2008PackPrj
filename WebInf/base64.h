#include <string>

#ifndef _BASE64_H_
#define _BASE64_H_

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

class CBase64  
{
public:
	CBase64();
	int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
	int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen);
	virtual ~CBase64();
	int Base64DecodeSize(const char* pBase64);	//��ȡBase64���볤��
	int Base64EncodeSize(int nOrgSize);			//��ȡBase64���볤��

};

#endif