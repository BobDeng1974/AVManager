// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <Wininet.h>

#include "types.h"
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


#pragma comment(lib,"version.lib") 
#pragma comment(lib, "Wininet")
#pragma comment(lib,"Shell32.lib") 