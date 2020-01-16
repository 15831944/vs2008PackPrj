/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2008 Ren?Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren?Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/
#include "stdafx.h"
#include "base64.h"
#include <iostream>

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) 
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 
{
  std::string ret;
  int i = 0, j = 0;
  unsigned char char_array_3[3], char_array_4[4];

  while (in_len--)
	{
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) 
		{
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) 
{
  int in_len = encoded_string.size();
  int i = 0, j = 0, in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 
	{
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) 
	{
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) 
			ret += char_array_3[j];
  }

  return ret;
}

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
	int nDstLen;            // ������ַ�����
	int nValue;             // �����õ��ĳ�����
	int i;

	i = 0;
	nDstLen = 0;

	// ȡ4���ַ������뵽һ�����������پ�����λ�õ�3���ֽ�
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
		else        // �س����У�����
		{
			pSrc++;
			i++;
		}
	}

	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}



int CBase64::EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen)
{
	unsigned char c1, c2, c3;    // ���뻺��������3���ֽ�
	int nDstLen = 0;             // ������ַ�����
	int nLineLen = 0;            // ������г��ȼ���
	int nDiv = nSrcLen / 3;      // �������ݳ��ȳ���3�õ��ı���
	int nMod = nSrcLen % 3;      // �������ݳ��ȳ���3�õ�������

	// ÿ��ȡ3���ֽڣ������4���ַ�
	for (int i = 0; i < nDiv; i ++)
	{
		// ȡ3���ֽ�
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

		// �����4���ַ�
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;

	}

	// �������µ��ֽ�
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

	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}

//3*8=4*6,����unsigned charת�����ĸ�char
//ԭ��ʣ����ֽڸ��ݱ�������������ת(1��2,2��3;������λ����0��ȫ)������=�Ų���4���ֽڡ������Ϊʲô��ЩBase64�������һ���������ȺŽ�����ԭ�򣬵��Ⱥ����ֻ����������Ϊ��
//һ��ԭ�ֽ����ٻ�������Ŀ���ֽ�,���������κ�����¶�ֻ������0��1��2���������е�һ����
//���������0�Ļ����ͱ�ʾԭ���ֽ���������3�ı�������������������
//�����1�Ļ���ת��2��Base64�����ַ���Ϊ����Base64������4�ı�������Ҫ��2���Ⱥţ�
//ͬ�������2�Ļ�����Ҫ��1���Ⱥš�
int CBase64::Base64DecodeSize(const char* pBase64)	//��ȡBase64���볤��
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

int CBase64::Base64EncodeSize(int nOrgSize)		//��ȡBase64���볤��
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
