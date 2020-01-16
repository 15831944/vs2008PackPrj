#include "StdAfx.h"
#include "SimpleCry.h"
#define XOR_VALUE 0X83
#define ADD_VALUE 0X12
static BYTE xor_key = 0x83;
static BYTE add_key = 0x12;

CSimpleCry::CSimpleCry(void)
{
}

CSimpleCry::~CSimpleCry(void)
{
}

void CSimpleCry::InitKey(BYTE xor,BYTE add)
{
	xor_key = xor;
	add_key = add;
}

void CSimpleCry::Encrypt(char* szMing, char* szMi, int nLength)
{
	UCHAR b, c;
	for(int i = 0; i < nLength; i++)
	{
		b = c = *(UCHAR*)(szMing + i);
		b &= 7;
		b <<= 5;//5
		c >>= 3;//3
		b += c;
		b ^= xor_key;
		*(UCHAR*)(szMi + i) = b + add_key;
	}
}

void CSimpleCry::Decrypt(char* szMi, char* szMing, int nLength)
{
	UCHAR b, c;
	for(int i = 0; i < nLength; i++)
	{
		b = *(UCHAR*)(szMi + i) - add_key;
		b ^= xor_key;
		c = b;

		c &= 31;
		c <<= 3;//3
		b >>= 5;//5
		b += c;
		*(UCHAR*)(szMing + i)= b;
	}
}