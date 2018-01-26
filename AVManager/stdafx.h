// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include "resource.h"
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <conio.h>
#include <new.h>
#include <signal.h>
#include <exception>
#include <sys/stat.h>
#include <psapi.h>
#include <rtcapi.h>
#include <Shellapi.h>
#include <dbghelp.h>
#include <DShow.h>
#include <Wininet.h>

#include "types.h"
#include "AUDIO_SDK.h"
#include "VIDEO_SDK.H"
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�


#include <Initguid.h>
#include "..\DuiLib\UIlib.h"


// using namespace
using namespace DuiLib;
using namespace std;


// lib
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#   endif
#endif


#pragma comment(lib,"Audio.lib")
#pragma comment(lib,"Video.lib")
#pragma comment (lib,"Strmiids")
#pragma comment (lib,"quartz")
#pragma comment(lib, "Wininet")
#pragma comment(lib, "version")