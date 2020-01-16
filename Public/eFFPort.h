#pragma once

//eCC.dll与打印产品的软加密文件有冲突，增加一个eFF.dll的加密库文件，功能与eCC.dll一致。

#include "../public/eFFHeader.h"
#ifdef _DEBUG
#pragma comment(lib,"../public/lib/debug/eFF.lib")
#else
#pragma comment(lib,"../public/lib/release/eFF.lib")
#endif
