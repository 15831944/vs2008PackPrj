#ifndef _XX_MESSAGE__XXX_NET
#define _XX_MESSAGE__XXX_NET



#include "../eMessage/datastream.pb.h"
#include "../eMessage/cmmmessage.pb.h"
/*#include "../eMessage/cryptmessage.pb.h"*/
#include "../eMessage/printermessage.pb.h"
// #include "../eMessage/logmessage.pb.h"
// #include "../eMessage/appmessage.pb.h"
#include "../eMessage/depmessage.pb.h"
// #include "../eMessage/popedom.pb.h"
// #include "../eMessage/intranetclt.pb.h"
#include "../eMessage/mobile.pb.h"


#ifdef _DEBUG
#pragma comment(lib,"..\\debug\\eMessage.lib")
#pragma comment(lib,"..\\public\\lib\\debug\\libprotobuf.lib")
//#pragma comment(lib,"..\\public\\lib\\debug\\libprotoc.lib")
#else
#pragma comment(lib,"..\\release\\eMessage.lib")
#pragma comment(lib,"..\\public\\lib\\release\\libprotobuf.lib")
//#pragma comment(lib,"..\\public\\lib\\release\\libprotoc.lib")
#endif


#endif 