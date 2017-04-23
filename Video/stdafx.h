// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <string>
#include <vector>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
#include "tyeps.h"
#include "c_string.h"
#include "c_singleton.h"
#include "c_event.h"
#include "c_thread.h"
#include <dshow.h>


//namespace
using namespace utility;
using namespace std;



//lib
#pragma comment (lib,"Strmiids")
#pragma comment (lib,"quartz")




#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef	std::string tstring;
#endif
