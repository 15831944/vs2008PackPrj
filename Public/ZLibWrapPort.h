#pragma once
#include "../ZLibWrapLib/ZLibWrapLib.h"

#ifdef _DEBUG
#pragma comment(lib,"../Debug/ZLibWrapLib.lib")
#else
#pragma comment(lib,"../Release/ZLibWrapLib.lib")
#endif