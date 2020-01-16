#pragma once

#include "../CurlWrapLib/MailSender.h"
#include "../CurlWrapLib/HttpClient.h"

#pragma comment(lib, "../CurlWrapLib/libcurl/lib/I386/LIBCURL.LIB")

#ifdef _DEBUG
#pragma comment(lib, "../CurlWrapLib/mimetic-0.9.8/win32/libmimetic_d.lib")
#pragma comment(lib,"../Debug/CurlWrapLib.lib")
#else
#pragma comment(lib, "../CurlWrapLib/mimetic-0.9.8/win32/libmimetic.lib")
#pragma comment(lib,"../Release/CurlWrapLib.lib")
#endif