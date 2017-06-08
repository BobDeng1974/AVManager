// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <initguid.h>
#include <string>
#include <vector>
#include <tchar.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "types.h"

//namespace
using namespace std;



//lib
#pragma comment(lib,"Dsound.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"dxguid.lib")

//define
#define INITGUID

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef	std::string tstring;
#endif