#pragma once

typedef enum tagInfraredInductionType
{
	IIT_ERROR,
	IIT_SIGNAL
}InfraredInductionType;

class IInfraredInductionInterface
{
public:
	virtual void OnInfraredInduction(InfraredInductionType type, DWORD dwData) = NULL;
};

#define	DataBufferLen	4	//读取com数据的长度

class CCOMPortHelper;

class _AA_DLL_EXPORT_ CInfraredInduction : public ICOMPortInterface
{
public:
	CInfraredInduction(void);
	~CInfraredInduction(void);

	BOOL Init(int nComIndex);
	void Exit();
	void SetCallBack(IInfraredInductionInterface* pCallBack);

public:
	virtual void OnByte(BYTE byte);
	virtual void OnErr(DWORD dwError);

protected:
	IInfraredInductionInterface* m_pCallBack;
	CCOMPortHelper *m_pHelper;
	BYTE m_oDataBuffer[DataBufferLen];
	int m_nReadDataBufferLen;
};
