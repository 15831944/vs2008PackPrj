// test_libmimetic.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <mimetic/mimetic.h>

using namespace std;
using namespace mimetic;

#ifdef _DEBUG
#pragma comment(lib, "../win32/libmimetic_d.lib")
#else
#pragma comment(lib, "../win32/libmimetic.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{

	MimeEntity me;
	me.header().from("me <me@domain.com>");
	me.header().to("you <you@domain.com>");
	me.header().subject("my first mimetic msg");
	me.body().assign("hello there!");
	cout << me << endl;
	return system("pause");
}

