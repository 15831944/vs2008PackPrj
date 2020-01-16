#pragma once

#include "../public/eCCHeader.h"
#ifdef _DEBUG
#pragma comment(lib,"../public/lib/debug/eCC.lib")
#else
#pragma comment(lib,"../public/lib/release/eCC.lib")
#endif
