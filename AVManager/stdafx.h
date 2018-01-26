// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include "resource.h"
// C 运行时头文件
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
// TODO: 在此处引用程序需要的其他头文件


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