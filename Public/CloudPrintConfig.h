/************************************************************************/
/*		 本文件是云打印配置文件，控制一些编译开关                       */
/************************************************************************/

#ifndef _CLOUDPRINTCONFIG_174B8D23_01E1_44F0_901B_92EE43D73C17_H_ 
#define _CLOUDPRINTCONFIG_174B8D23_01E1_44F0_901B_92EE43D73C17_H_

#define Enable_Http_Interface						1			//云服务接口调用方式，0-启用webservice接口，1-启用HTTP接口
#define Enable_Encrypt_Router						0			//是否启用加密网关，0-禁用，1-启用
#define Enable_Domain_Config						0			//是否启用webservice域名的配置界面
#define Enable_Server_WebService_Same_Domain		0			//是否linux的c++服务器与webservice使用同一域名
#define Enable_Client_Not_Compress_Print_Doc		1			//是否启用客户端不压缩打印作业文件，0-禁用，1-启用
#define Enable_Router_Not_Compress_Scan_Doc			1			//是否启用网关不压缩扫描作业文件，0-禁用，1-启用
#define Enable_DirectPrint							1			//是否支持直打
#endif	//_CLOUDPRINTCONFIG_174B8D23_01E1_44F0_901B_92EE43D73C17_H_
