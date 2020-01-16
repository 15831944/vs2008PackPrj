#ifndef _XX_Cloud_Print_MESSAGE__XXX_NET
#define _XX_Cloud_Print_MESSAGE__XXX_NET


#include "../eCloudMessage/cloudprintermessage.pb.h"

using namespace isafetec_cloud;

#ifdef _DEBUG
#pragma comment(lib,"..\\debug\\eCloudMessage.lib")
#pragma comment(lib,"..\\public\\lib\\debug\\libprotobuf.lib")
#else
#pragma comment(lib,"..\\release\\eCloudMessage.lib")
#pragma comment(lib,"..\\public\\lib\\release\\libprotobuf.lib")
#endif


#endif 