// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include <Shellapi.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <Dwrite.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#if defined(WIN32) || defined(WIN64)
// Copied from linux libc sys/stat.h:
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
#include <io.h>
#ifdef _MSC_VER
#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma comment(lib, "kernel32.lib")   
#pragma comment(lib, "user32.lib")   
#pragma comment(lib, "gdi32.lib")   
#pragma comment(lib, "shell32.lib")   
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")

// TODO: reference additional headers your program requires here
