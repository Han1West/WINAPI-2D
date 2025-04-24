#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>	// 메모리 누수 체크

#include <string>
using std::string;
using std::wstring;
#include <atlstr.h>


#include <vector>
#include <list>
#include <map>
using std::vector;
using std::list;
using std::map;
using std::multimap;
using std::make_pair;


// 라이브러리 링크
#pragma comment(lib, "Msimg32.lib")


#include "global.h"