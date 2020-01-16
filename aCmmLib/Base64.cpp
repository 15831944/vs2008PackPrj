// Base64.cpp: implementation of the CBase64 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Base64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char DeBase64Tab[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    62,        // '+'
    0, 0, 0,
    63,        // '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
    0, 0, 0, 0, 0, 0,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
};
 
const char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

CBase64::CBase64()
{

}

CBase64::~CBase64()
{

}


int CBase64::DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
    int nDstLen;            // 输出的字符计数
    int nValue;             // 解码用到的长整数
    int i;
 
    i = 0;
    nDstLen = 0;
 
    // 取4个字符，解码到一个长整数，再经过移位得到3个字节
    while (i < nSrcLen)
    {
        if (*pSrc != '\r' && *pSrc!='\n')
        {
            nValue = DeBase64Tab[*pSrc++] << 18;
            nValue += DeBase64Tab[*pSrc++] << 12;
            *pDst++ = (nValue & 0x00ff0000) >> 16;
            nDstLen++;
 
            if (*pSrc != '=')
            {
                nValue += DeBase64Tab[*pSrc++] << 6;
                *pDst++ = (nValue & 0x0000ff00) >> 8;
                nDstLen++;
 
                if (*pSrc != '=')
                {
                    nValue += DeBase64Tab[*pSrc++];
                    *pDst++ =nValue & 0x000000ff;
                    nDstLen++;
                }
            }
 
            i += 4;
        }
        else        // 回车换行，跳过
        {
            pSrc++;
            i++;
        }
     }
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;
}


 
int CBase64::EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen)
{
    unsigned char c1, c2, c3;    // 输入缓冲区读出3个字节
    int nDstLen = 0;             // 输出的字符计数
    int nLineLen = 0;            // 输出的行长度计数
    int nDiv = nSrcLen / 3;      // 输入数据长度除以3得到的倍数
    int nMod = nSrcLen % 3;      // 输入数据长度除以3得到的余数
 
    // 每次取3个字节，编码成4个字符
    for (int i = 0; i < nDiv; i ++)
    {
        // 取3个字节
        c1 = *pSrc++;
        c2 = *pSrc++;
        c3 = *pSrc++;
 
        // 编码成4个字符
        *pDst++ = EnBase64Tab[c1 >> 2];
        *pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
        *pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
        *pDst++ = EnBase64Tab[c3 & 0x3f];
        nLineLen += 4;
        nDstLen += 4;
     }
 
    // 编码余下的字节
    if (nMod == 1)
    {
        c1 = *pSrc++;
        *pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
        *pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
        *pDst++ = '=';
        *pDst++ = '=';
        nLineLen += 4;
        nDstLen += 4;
    }
    else if (nMod == 2)
    {
        c1 = *pSrc++;
        c2 = *pSrc++;
        *pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
        *pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
        *pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
        *pDst++ = '=';
        nDstLen += 4;
    }
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;
}

//3*8=4*6,三个unsigned char转换成四个char
//原文剩余的字节根据编码规则继续单独转(1变2,2变3;不够的位数用0补全)，再用=号补满4个字节。这就是为什么有些Base64编码会以一个或两个等号结束的原因，但等号最多只有两个。因为：
//一个原字节至少会变成两个目标字节,所以余数任何情况下都只可能是0，1，2这三个数中的一个。
//如果余数是0的话，就表示原文字节数正好是3的倍数（最理想的情况）。
//如果是1的话，转成2个Base64编码字符，为了让Base64编码是4的倍数，就要补2个等号；
//同理，如果是2的话，就要补1个等号。
int CBase64::Base64DecodeSize(const char* pBase64)	//获取Base64解码长度
{
	int nBase64Len = strlen(pBase64);
	if (nBase64Len < 4)
	{
		return 0;
	}

	int nEqualCount = 0;
	char cLast1 = pBase64[nBase64Len - 1];
	char cLast2 = pBase64[nBase64Len - 2];
	if (cLast1 == '=')
	{
		nEqualCount++;
	}
	if (cLast2 == '=')
	{
		nEqualCount++;
	}

	int n1 = nBase64Len / 4 * 3;
	return n1 - nEqualCount;
}

int CBase64::Base64EncodeSize(int nOrgSize)		//获取Base64编码长度
{
	if (nOrgSize <= 0)
	{
		return 0;
	}

	int n1 = nOrgSize / 3;
	int n2 = n1 * 3;
	if (nOrgSize > n2)
	{
		n1 += 1;
	}
	return n1 * 4;
}
