#pragma once



// Windows ͷ�ļ�:
#include <windows.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <tchar.h>
#include <time.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "c_singleton.h"
#include "c_event.h"
#include "c_thread.h"
#include "types.h"
#include "c_string.h"
#include "c_socket.h"
#include "c_lock.h"


//namespace
using namespace utility;
using namespace std;

//lib
#pragma comment(lib,"ws2_32.lib")