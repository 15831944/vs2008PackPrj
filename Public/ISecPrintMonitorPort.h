#pragma once

#include "../ISecPrintMonitor/userInterface/ZPrinterMonitorHelper.h"
#include "../ISecPrintMonitor/userInterface/ZSnmpInfoHelper.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\debug\\ISecPrintMonitor.lib")
#else
#pragma comment(lib,"..\\Release\\ISecPrintMonitor.lib")
#endif