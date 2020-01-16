#ifndef _DLL_DATASERV_HEADER_
#define _DLL_DATASERV_HEADER_


#include "..\eDbSvcInf\Ado.h"
#include "..\eDbSvcInf\EquipSvc.h"
#include "..\eDbSvcInf\AdoFactory.h"
#include "..\eDbSvcInf\DbInstaller.h"
// #include "..\eDbSvcInf\ResourceSvcInf.h"
// #include "..\eDbSvcInf\FilteSvcInf.h"
// #include "..\eDbSvcInf\ProcAndUrlSvc.h"
#include "..\eDbSvcInf\UserAndDepartmentSvc.h"
// #include "..\eDbSvcInf\LogSvcInf.h"
#include "..\eDbSvcInf\printsvc.h"
/*#include "../eDbSvcInf/PopedomSvc.h"*/

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\eDbSvcInf.lib")
#else
#pragma comment(lib,"..\\Release\\eDbSvcInf.lib")
#endif

#endif