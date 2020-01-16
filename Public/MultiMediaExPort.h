#ifndef _MULTIMEDIADLL_PORT_HEADER
#define _MULTIMEDIADLL_PORT_HEADER
// MultiMediaExPort.h: 
//
//////////////////////////////////////////////////////////////////////
#include "..\MultiMediaEx\Header\CommonDef.h"
#include "..\MultiMediaEx\Header\AudioBroadcast.h"
#include "..\MultiMediaEx\Header\VideoBroadcast.h"
#include "..\MultiMediaEx\Header\RECTeaching.h"
#include "..\MultiMediaEx\Header\VideoMonitor.h"
#include "..\MultiMediaEx\Header\AudioInterphone.h"
#include "..\MultiMediaEx\Header\AudioBroadcast.h"
#include "..\MultiMediaEx\Header\MultiAudioBroadcast.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\MultiMediaEx.lib")
#else
#pragma comment(lib,"..\\Release\\MultiMediaEx.lib")
#endif

#endif