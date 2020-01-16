/*********************************************************************
* 版权所有 (C)2011，ISafeTec。
* 
* 文件名称：  SorenDefine.h
* 内容摘要：  安密系统使用定义
* 说    明：  
* 当前版本：  1.0.0.0
* 作    者：  SongQX
* 日    期：  2011/06/27
***********************************************************************/

#pragma once

#pragma pack(push, 1)

//--------------------------------------------
// 文件加密结构体定义开始 
//--------------------------------------------

#define SOFTWARE_NAME_LEN	50
#define SOFTWARE_VER_LEN	20

// 程序信息
typedef struct tagSoftwareInfo
{
	CHAR		szName[SOFTWARE_NAME_LEN];	// 软件名称，只做显示用
	CHAR		szVersion[SOFTWARE_VER_LEN]; // 软件版本
	CHAR		szImage[MAX_PATH]; // 程序镜像名
}SOFTWAREINFO;

// 软件加密规则
typedef struct tagSoftwareRule
{
	DWORD			dwSize;	// 结构体大小，必须放在结构体开始位置
	SOFTWAREINFO	SoftwareInfo; // 程序信息
	DWORD			dwCryptLength; // 加密扩展名的长度
	DWORD			dwIgnorLength; // 忽略扩展名的长度
	BYTE			pszExtList[1]; // 扩展名内容，以'|'分隔，前面是加密扩展名，后面是忽略扩展名
}SOFTWARERULE;


// 加密方式
#define SOREN_CA_NULL			0XFFFF //未加密
#define SOREN_CA_XOR			0XFFFE //使用简单异或方式加密
#define SOREN_CA_AES_LOC		0XFFFD //使用aes保存本地文件

// 算法信息
typedef struct tagCryptAlgInfo
{
	DWORD		dwSize;	// 结构体大小
	DWORD		dwAlgCode;	// 算法代号
	DWORD		dwKeyLength; // 密钥长度
	BYTE		pbKey[1];	// 密钥内容
}CRYPTALGINFO;


// 加密类型使用的算法
typedef struct tagCryptAlgType
{
	DWORD			dwSize;	// 结构体大小
	DWORD			dwTypeID; // 使用算法的类型，例如文件、密钥保存、传输等。
	CRYPTALGINFO	CryptAlgInfo;
}CRYPTALGTYPE;

//--------------------------------------------
// 文件加密结束
//--------------------------------------------


//--------------------------------------------
// 文件日志结构体定义开始 
//--------------------------------------------

// 文件日志：操作类型
#define SOREN_LOG_OP_CREATE		0X1
#define SOREN_LOG_OP_READ		0X2
#define SOREN_LOG_OP_WRITE		0X4
#define SOREN_LOG_OP_DELETE		0X8
#define SOREN_LOG_OP_RENAME		0x10 

#define SOREN_LOG_OP_TYPE_ALL	(SOREN_LOG_OP_CREATE | SOREN_LOG_OP_READ | SOREN_LOG_OP_WRITE | SOREN_LOG_OP_DELETE | SOREN_LOG_OP_RENAME) // 用于文件日志过滤

#define SOREN_LOG_FILTER_USB	0X1000 //日志过滤类型，usb

// 文件日志：操作说明

// 删除文件
#define SOREN_LOG_OP_PARAM_DELETE_CLOSE			0X0	//create文件时带有delete参数
#define SOREN_LOG_OP_PARAM_DELETE_SETINF		0X1	//通过irp_setinformation FileDispositionInformation删除


// 文件日志中 读写文件操作的参数
typedef struct tagReadWriteLogParam
{
	LARGE_INTEGER	ByteOffset;
	ULONG			Length;	
}READWRITELOGPARAM;

// 文件日志
//--------------------------------------------
// 说明: 原先的FILESYSTEMLOG结构体长度不固定，改成固定的FILESYSTEMLOGEX
// 时间: 2011/07/22
// Add by SongQX
//--------------------------------------------

typedef struct tagFileSystemLogEx
{
	DWORD		dwSize; //结构体大小,目前大小是固定的，所以不需要用到这个值。
	DWORD		dwPid; // 进程id
	char		szPath[MAX_PATH]; //路径
	DWORD		dwOpType; //操作类型
	DWORD		dwOpParam; //操作说明，内容取决于操作类型
	//BYTE		bParam[1]; //其他说明，内容取决于操作类型
	union
	{
		READWRITELOGPARAM param;		//读写时使用
		char	szNewPath[MAX_PATH];	//重命名使用，文件名字符串
	};
}FILESYSTEMLOGEX;

//--------------------------------------------
// 文件日志结束 
//--------------------------------------------


//--------------------------------------------
// 文件权限开始 
//--------------------------------------------

// 文件文件夹公用
#define SOREN_FILE_ACCESS_GET_INFO		0X1 //获取属性，未测试
#define SOREN_FILE_ACCESS_SET_INFO		0X2 //设置属性，未测试
#define SOREN_FILE_ACCESS_RENAME		0X4 //重命名
#define SOREN_FILE_ACCESS_DELETE		0X8 //删除
#define SOREN_FILE_ACCESS_SHOW			0X10 //显示
#define SOREN_FILE_ACCESS_OPEN			0X20 //打开（createfile)

// 文件夹
//#define SOREN_FILE_ACCESS_DIR			0X100 //列出文件夹内容，使用该文件夹下的子目录或文件。不支持，使用SOREN_FILE_ACCESS_OPEN
//#define SOREN_FILE_ACCESS_MD			0X200 //创建子文件夹或文件，尚未实现

// 文件

//#define SOREN_FILE_ACCESS_READ		0X100 //读文件。不支持，使用SOREN_FILE_ACCESS_OPEN
#define SOREN_FILE_ACCESS_WRITE			0X200 //写文件。


//#define SOREN_FILE_ACCESS_ALL	(SOREN_FILE_ACCESS_GET_INFO | SOREN_FILE_ACCESS_SET_INFO | SOREN_FILE_ACCESS_RENAME | SOREN_FILE_ACCESS_DELETE | SOREN_FILE_ACCESS_SHOW | SOREN_FILE_ACCESS_READ | SOREN_FILE_ACCESS_WRITE)
#define SOREN_FILE_ACCESS_ALL			0xFFFF

typedef struct tagFileAccessAuth
{
	DWORD	Size;		//结构体大小
	BYTE	Directory;	//是否是文件夹，取值只有0和1，不要弄出FILE_ATTRIBUTE_DIRECTORY（0n16）来
	ULONG	Access;		//权限
	char	Path[MAX_PATH];	//路径
}FILEACCESSAUTH;
//--------------------------------------------
// 文件权限结束 
//--------------------------------------------


//------------------------------------------------------
// [11/4/2011 Songqx]
// 程序的文件访问权限

// 扩展名
#define  FILEACCESSLISTMODE_BLACK 0
#define  FILEACCESSLISTMODE_WHITE 1

#define  FILEACCESSLISTTYPE_EXT		0
#define  FILEACCESSLISTTYPE_PATH	1

typedef struct tagSoftwareFileAccess
{
	DWORD			dwSize;	// 结构体大小，必须放在结构体开始位置
	SOFTWAREINFO	SoftwareInfo; // 程序信息
	DWORD			dwMode;//模式，0为黑名单，1为白名单。
	DWORD			dwType;//类型，0：扩展名，1：路径
	DWORD			dwParam; // 扩展参数
	BYTE			pszTextList[1]; // 扩展名或路径内容， 之间以'|'分隔.
}SOFTWAREFILEACCESS;

//------------------------------------------------------
#pragma pack(pop)

#define MAX_SOREN_FILE_SIZE		(10 * 1024 * 1024) (10MB)





