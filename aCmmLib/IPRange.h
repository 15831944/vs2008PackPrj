#pragma once

#pragma pack(push, 1)
typedef struct tag_IPRange
{
	UINT	nBIP;	//开始IP
	UINT	nEIP;	//结束IP
}IPRange;

#ifndef HOST_SIZE
#define HOST_SIZE 50
#endif
typedef struct tag_NameRange
{
	char	cPrefix[HOST_SIZE];	//名字前缀
	char	cBName[HOST_SIZE];
	char	cEName[HOST_SIZE];
}NameRange;
#pragma pack(pop)
//ip范围
class _AA_DLL_EXPORT_ CIPRange : public IPRange
{
public:
	CIPRange(IPRange* pRg);
	CIPRange(UINT b,UINT e);
	BOOL IsInRng(UINT nIP);

	CString GetBIP()
	{
		CString str;
		str.Format(_T("%d.%d.%d.%d"), FIRST_IPADDRESS(nBIP), \
			SECOND_IPADDRESS(nBIP), THIRD_IPADDRESS(nBIP), FOURTH_IPADDRESS(nBIP));
		return str;
	}

	CString GetEIP()
	{
		CString str;
		str.Format(_T("%d.%d.%d.%d"), FIRST_IPADDRESS(nEIP), \
			SECOND_IPADDRESS(nEIP), THIRD_IPADDRESS(nEIP), FOURTH_IPADDRESS(nEIP));
		return str;
	}

	static DWORD StringToDWORD(CStringA strIP);
};
//ip范围列表
class _AA_DLL_EXPORT_ CIPRangeList : public CList<CIPRange*,CIPRange*>
{
public:
	virtual ~CIPRangeList();
	void Attach(IPRange* pRng,int nCount);
	BOOL GetListData(BYTE* pData,int nLen);
	BOOL IsConflict(UINT nBIP,UINT nEIP);
	BOOL DeleteOne(CIPRange* pIP);
	CIPRange* AddOne(UINT nBIP,UINT nEIP);
	CIPRange* AddOne(CStringA sBIP,CStringA sEIP);
	BOOL IsInList(UINT nIP);
	void Release();
};
//机器名范围
class _AA_DLL_EXPORT_ CHostNameRange : public NameRange
{
public:
	CHostNameRange(NameRange* pRg);
	CHostNameRange(CString sPre,CString sB,CString sE);
	BOOL IsInRng(CString sIP);

	CString GetBName()
	{
		CString str;
		str.Format(_T("%s%s"), cPrefix, cBName);
		return str;
	}

	CString GetEName()
	{
		CString str;
		str.Format(_T("%s%s"), cPrefix, cEName);
		return str;
	}
};

//机器名列表
class _AA_DLL_EXPORT_ CHostNameRangeList : public CList<CHostNameRange*,CHostNameRange*>
{
public:
	virtual ~CHostNameRangeList();
	BOOL GetListData(BYTE* pData,int nLen);
	BOOL IsConflict(CString sPre,CString sB,CString sE);
	BOOL DeleteOne(CHostNameRange* pRng);
	CHostNameRange* AddOne(CString sPre,CString sB,CString sE);
	void Attach(NameRange* pRng,int nCount);
	BOOL IsInList(CString sHost);
	void Release();
};