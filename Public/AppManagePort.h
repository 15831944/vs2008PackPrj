#ifndef _APPMANAGE_PORT_HEADER
#define _APPMANAGE_PORT_HEADER
// AppManagePort.h: 
//
//////////////////////////////////////////////////////////////////////////
#include "setupapi.h"
#include "../AppManage/HardwareResFun.h"
#include "../AppManage/SoftwareResFun.h"
#include "../AppManage/ResCheckFun.h"
#include "../AppManage/DeviceManageFun.h"
#include "../AppManage/header/ComputerSystemInformation.h"
#include "../AppManage/header/CPUInformation.h"
#include "../AppManage/header/BIOSInformation.h"
#include "../AppManage/header/CDRomDriveInformation.h"
#include "../AppManage/header/ComputerSystemProductInformation.h"
#include "../AppManage/header/DiskPartitionInformation.h"
#include "../AppManage/header/DisplayConfigurationInformation.h"
#include "../AppManage/header/IDEControllerInformation.h"
#include "../AppManage/header/KeyboardInformation.h"
#include "../AppManage/header/LogicalDiskInformation.h"
#include "../AppManage/header/MainBoardInformation.h"
#include "../AppManage/header/MemoryDeviceInformation.h"
#include "../AppManage/header/MemoryInformation.h"
#include "../AppManage/header/MouseInformation.h"
#include "../AppManage/header/NetWorkAdapterInformation.h"
#include "../AppManage/header/PhysicalDiskDriversInformation.h"
#include "../AppManage/header/PhysicalMediaInformation.h"
#include "../AppManage/header/USBControllerInformation.h"
#include "../AppManage/header/VideoDisplayDeviceInformation.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\AppManage.lib")
#else
#pragma comment(lib,"..\\Release\\AppManage.lib")
#endif
#endif