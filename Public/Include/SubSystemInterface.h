
#ifndef _ISUBSYSTEM_HEADER
#define _ISUBSYSTEM_HEADER
//----------------------------
//模块调用主框架的接口
struct _AA_DLL_EXPORT_ ISubSystemCallback
{
public:
	virtual int NotifyFrame(int nCmd,int w,int l) {ASSERT(0);return 0;}
};

//模块调用主框架的接口
struct _AA_DLL_EXPORT_ ISubSystemInnerCallback
{
public:

	virtual int OnReceive(BYTE* pData,int nLen) {ASSERT(0);return 0;}
	virtual int OnReceive(CBuffer* pBuf) {ASSERT(0);return 0;}
	virtual int OnEquipLogon(int){ASSERT(0);return 0;}
	virtual int OnEquipLogoff(int){ASSERT(0);return 0;}
	virtual int OnEnd(){ASSERT(0);return 0;}
};

class _AA_DLL_EXPORT_ CCmdInterace
{
public:
	virtual int SendToSvr(int nCmd,int nSub = 0,BYTE* pData = 0,int nLen = 0) {ASSERT(0);return 0;}
	virtual int SendToEquip(int nId,BYTE* pData = 0,int nLen = 0) {ASSERT(0);return 0;}
	virtual int SendToEquip(CDWordArray& ary,BYTE* pData = 0,int nLen = 0) {ASSERT(0);return 0;}
	virtual int SendToAllEquip(BYTE* pData = 0,int nLen = 0) {ASSERT(0);return 0;}
	virtual char* GetTcpIP() {ASSERT(0);return 0;}
	virtual char* GetUdpIP() {ASSERT(0);return 0;}
	virtual int GetTcpPort() {ASSERT(0);return 0;}
	virtual int GetUdpPort(int nIndex) {ASSERT(0);return 0;}
	virtual int GetEquipId(){ASSERT(0);return 0;}
	virtual int NotifyFrame(int nCmd,int w,int l) {ASSERT(0);return 0;}

	ISubSystemInnerCallback* m_pInnerCallback;
	//ISubSystemCallback* m_pCallback;
};

#endif