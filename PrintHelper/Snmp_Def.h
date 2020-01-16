#ifndef _SNMP_DEF_CE60A9FB_B2AE_4DFF_9E01_E380BD3EE304_H_
#define _SNMP_DEF_CE60A9FB_B2AE_4DFF_9E01_E380BD3EE304_H_

//参考http://bbs.csdn.net/topics/390691305
//参考http://blog.csdn.net/luckysym/article/details/1666114
//说明：解决由于C++中枚举类型的成员变量的全局唯一性引起的在成员命名时的重命名错误
//宏定义枚举结构体
#define  BEGIN_ENUM_STRUCT(name) struct tag_##name{	enum name
#define  END_ENUM_STRUCT(name) ;};
#define  ENUM_STRUCT_TYPE(name) tag_##name::name
#define  ENUM_STRUCT_VALUE(name) tag_##name

/************************************************************************/
/*         枚举结构体 使用方式  

	BEGIN_ENUM_STRUCT(test)
	{
		one = 1,
		two = 2
	}
	END_ENUM_STRUCT(test)

	等同于
	struct tag_test
	{
		enum test
		{
			one = 1,
			two = 2
		};
	};

	ENUM_STRUCT_TYPE(test) t;
	t = ENUM_STRUCT_VALUE(test)::two;
	printf("t = %d", t);

*/
/************************************************************************/

//http://tools.ietf.org/html/rfc2578
typedef int Integer32;						// between -2^31 and 2^31-1 inclusive (-2147483648 to 2147483647 decimal)
typedef unsigned int Integer;				//
typedef unsigned char OCTET_STRING;			// The OCTET STRING type represents arbitrary binary or textual data.
											// Although the SMI-specified size limitation for this type is 65535
											// octets, MIB designers should realize that there may be implementation
											// and interoperability limitations for sizes in excess of 255 octets.
typedef unsigned char OBJECT_IDENTIFIER;	// The OBJECT IDENTIFIER type represents administratively assigned
											// names.  Any instance of this type may have at most 128 sub-
											// identifiers.  Further, each sub-identifier must not exceed the value
											// 2^32-1 (4294967295 decimal).
typedef BYTE OBJECT_BITS;
typedef unsigned int Counter32;				//The Counter32 type represents a non-negative integer which
											// monotonically increases until it reaches a maximum value of 2^32-1
											// (4294967295 decimal), when it wraps around and starts increasing
											// again from zero.
typedef unsigned int Gauge32;				//The Gauge32 type represents a non-negative integer, which may
											// increase or decrease, but shall never exceed a maximum value, nor
											// fall below a minimum value.  The maximum value can not be greater
											// than 2^32-1 (4294967295 decimal), and the minimum value can not be
											// smaller than 0. 
typedef unsigned int TimeTicks;				//The TimeTicks type represents a non-negative integer which represents
											// the time, modulo 2^32 (4294967296 decimal), in hundredths of a second
											// between two epochs.  When objects are defined which use this ASN.1
											// type, the description of the object identifies both of the reference
											// epochs.
											//				http://tools.ietf.org/html/rfc2579
											//"A date-time specification.	
											// field  octets  contents                  range
											// -----  ------  --------                  -----
											// 1      1-2   year*                     0..65536
											// 2       3    month                     1..12
											// 3       4    day                       1..31
											// 4       5    hour                      0..23
											// 5       6    minutes                   0..59
											// 6       7    seconds                   0..60
											// (use 60 for leap-second)
											// 7       8    deci-seconds              0..9
											// 8       9    direction from UTC        '+' / '-'
											// 9      10    hours from UTC*           0..13
											// 10      11    minutes from UTC          0..59
											// 
											// * Notes:
											// - the value of year is in network-byte order
											// - daylight saving time in New Zealand is +13
											// 
											// For example, Tuesday May 26, 1992 at 1:30:15 PM EDT would be
											// displayed as:
											// 
											// 1992-5-26,13:30:15.0,-4:0
											// 
											// Note that if only local time is known, then timezone
											// information (fields 8-10) is not present."
// #define Opaque								// The Opaque type is provided solely for backward-compatibility, and
// 											// shall not be used for newly-defined object types.
typedef unsigned __int64 Counter64;			// The Counter64 type represents a non-negative integer which
											// monotonically increases until it reaches a maximum value of 2^64-1
											// (18446744073709551615 decimal), when it wraps around and starts
											// increasing again from zero.
typedef unsigned int Unsigned32;	// The Unsigned32 type represents integer-valued information between 0
											// and 2^32-1 inclusive (0 to 4294967295 decimal).
typedef unsigned int TimeStampS;	
typedef OCTET_STRING SnmpAdminString;	//0-255，参考【SNMPv2-TC.mib】
typedef OCTET_STRING DateAndTime;		//0-31，DISPLAY-HINT "2d-1d-1d,1d:1d:1d.1d,1a1d:1d"，参考【SNMP-FRAMEWORK-MIB.mib】
typedef OCTET_STRING DisplayString;		//0-255，参考【SNMP-FRAMEWORK-MIB.mib】
typedef OCTET_STRING InternationalDisplayString;		//0-127
typedef OCTET_STRING PhysAddress;		//0-15
typedef OCTET_STRING IpAddress;			//0-3

#define OCTET_STRING_SIZE_1			1
#define OCTET_STRING_SIZE_2			2
#define OCTET_STRING_SIZE_3			3
#define OCTET_STRING_SIZE_4			4
#define OCTET_STRING_SIZE_8			8
#define OCTET_STRING_SIZE_16		16
#define OCTET_STRING_SIZE_32		32
#define OCTET_STRING_SIZE_64		64
#define OCTET_STRING_SIZE_128		128
#define OCTET_STRING_SIZE_256		256

#define OID_NULL					TEXT(".0.0")		//空的OID
#define OBJECT_IDENTIFIER_SIZE		1024
#define PhysAddress_SIZE			16
#define IpAddress_SIZE				4
#define DateAndTime_Size			32
#define InternationalDisplayString_Size	128

#include <list>
#include <map>
using namespace std;

#endif	//_SNMP_DEF_CE60A9FB_B2AE_4DFF_9E01_E380BD3EE304_H_