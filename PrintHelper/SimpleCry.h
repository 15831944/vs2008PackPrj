#pragma once

class CSimpleCry
{
public:
	CSimpleCry(void);
	~CSimpleCry(void);
	static void InitKey(BYTE xor,BYTE add);
	static void Encrypt(char* szMing, char* szMi, int nLength);
	static void Decrypt(char* szMi, char* szMing, int nLength);
};
