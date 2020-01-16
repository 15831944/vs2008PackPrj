#pragma once

#define HW_VERSION_ONE 0x0001
#define HW_VERSION_TWO 0x0002
class CHWConfig
{
public:
	static CHWConfig& GetInstance()
	{
		static CHWConfig one;
		return one;
	}
	CHWConfig()
	{
		m_dwHwVersion = HW_VERSION_TWO;
	}
	DWORD m_dwHwVersion;
	DWORD m_dwSubVersion;
	void Load()
	{

	}
	BOOL IsHwVerTwo()
	{
		return TRUE;
	}
};