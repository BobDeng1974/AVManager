// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include <string>
#include <vector>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
