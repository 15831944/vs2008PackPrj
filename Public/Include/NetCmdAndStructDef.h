
#ifndef _NET_CMDANDSTRUCTDEF_HEADER
#define _NET_CMDANDSTRUCTDEF_HEADER
//----------------------------
#define NET_MAXPACK_SIZE	2048

#define PACKINDENTIFY			32165
#define PACK_SMALL			0x0001//小包 tcp默认都是小包，tcp协议保证了顺序
#define PACK_LARGE			0x0002//大包 组播包默认是大包，保证顺序
#define PACK_COMPRESS		0x0004//压缩包
#pragma pack(push, 1)
//包头
typedef struct tag_Net_Pack_Head
{
	UINT		nPackBodySize;		//包的大小，不包括包头
	UINT		nMainCmd;			//主命令
	UINT		nSubCmd;			//子命令
	WORD		nIndentify;			//包的标识
	WORD		nPackMask;			//分大包，小包，是否为	压缩包，用掩码区分
	UINT		nReserve;			//tcp时，保留字段，上层不允许使用此字段，当为压缩包时，此字段标识未压缩前包的大小
	//组播包接收时，标识组播数据源的ip或学生id
}NET_PACK_HEAD,*PNET_PACK_HEAD;

//--------------------------
//大包包头，nPackMask为大包时才会使用
typedef struct tag_LargePackHead
{
	UINT		nGUID;				//UDP数据包时用
	WORD		nPackCount;			//大包时为：数据包个数
	WORD		nPackIndex;			//大包时为：数据包索引，下标从1开始
	UINT		nSrcID;				//数据来源标识，当同时存在多个数据源时使用
	UINT		nAllBodySize;		//一个大包的总大小
}LARGEPACK_HEAD,*PLARGEPACK_HEAD;

//-----------------------------
//压缩包头，nPackMask为压缩包时才会使用
typedef struct tag_CompressPackHead
{
	UINT		nCompressedSize;		//压缩后大小
	UINT		nUncompressedSize;		//未压缩大小
	UINT		nCompressLevel;			//压缩级别
}COMPRESSPACK_HEAD,*PCOMPRESSPACK_HEAD;

typedef struct tag_BufferBlockHead
{
	UINT	nBufferBlockSize;	//整个缓冲区的大小
	UINT	nBufferHeadSize;	//缓冲区头结构的大小
	long	nRefCount;			//引用计数，为0时释放缓冲区
	UINT	nSourceThreadID;	//数据源的线程ID
	UINT    nSourceEquipID;		//数据源设备ID
	BOOL	bTempBigBlock;		//是否是临时开辟的大内存缓冲区	
//	UINT	nSourceClassID;		//缓冲块是属于哪个班级模型的，（学生或老师的）
}BUFBLOCK_HEAD;

// 策略验证多出的数据部分
// edited by jarry 2012-1-16
typedef struct tag_ExternSegment
{
	DWORD	dwCulTime;
}EXTERNSEG;

typedef struct tag_DATA_PACK	//直接转发数据包结构
{
	UINT	nMainCmd;
	UINT	nSubCmd;
	UINT	nMgrID;					//管理机ID
	UINT	nEquipCount;			//学生个数
	UINT	nEquipIDListOffset;	//学生列表
//	UINT	nClassCount;		//班级模型个数
//	UINT	nClassListOffset;	//班级模型列表偏移
} DATA_PACK,*PDATA_PACK;

#pragma pack(pop)
typedef void (*PFOnBCData)(BYTE* pBuf,int nLen,DWORD,DWORD,int);
typedef void (*PFOnBCDataWithSrcID)(BYTE* pBuf,int nLen,DWORD,DWORD,int,UINT);

#define WM_NETBASE				WM_USER + 1000
#define WM_NEW_CLIENT_SOCK		WM_NETBASE + 1
#define WM_SOCKET_ON_CLOSE		WM_NETBASE + 2
#define WM_REDIRECT_PACK		WM_NETBASE + 3
#define WM_SOCKET_CMPLT			WM_NETBASE + 4
#endif