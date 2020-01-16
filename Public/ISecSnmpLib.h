#pragma once

#include "../ISecSnmpLib/ZSnmpOp.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/ISecSnmpLib.lib")
#else
#pragma comment(lib, "../Release/ISecSnmpLib.lib")
#endif