#pragma once

#define ENABLE_PWD_CRY

#include "../encryptlib/PWDCry.h"

#ifdef _DEBUG
#pragma comment(lib,"../debug/encryptlib.lib")
#else
#pragma comment(lib,"../release/encryptlib.lib")
#endif
