// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>


#ifdef _DEBUG
#pragma comment(lib,"..\\public\\lib\\debug\\libprotobuf.lib")
//#pragma comment(lib,"..\\public\\lib\\debug\\libprotoc.lib")
#else
#pragma comment(lib,"..\\public\\lib\\release\\libprotobuf.lib")
//#pragma comment(lib,"..\\public\\lib\\release\\libprotoc.lib")

#endif