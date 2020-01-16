#pragma once
#include "../iodriver/FilterBase.h"
#include "../iodriver\NtProcessMonitor.h"
#include "..\iodriver\ScreenCapture.h"
#include "../iodriver/NdisOp.h"
#include "../iodriver/NdisOp2.h"
#include "../iodriver/HttpOp.h"
#include "../iodriver/filtermgr.h"
#include "../iodriver/NtProcessMonitor.h"
#include "../IODriver/PrinterMonitor.h"
#include "WebMailPort.h"
#include "../IODriver/webmailop.h"
#include "../IODriver/EnumProc.h"
#include "../IODriver/RegProtect.h"
#include "../IODriver/IPInterface.h"
#include "../IODriver/ClientProtect.h"
#include "../IODriver/QQChecker.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\iodriver.lib")
#else
#pragma comment(lib,"..\\Release\\iodriver.lib")
#endif

