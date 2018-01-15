#include "resource.h"
#include "stdafx.h"
#include <vector>
using namespace std;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define TRAYICONID 1
#define SWM_TRAYMSG WM_APP

#define SWM_SHOW (WM_APP + 1)
#define SWM_SCAN (WM_APP + 2)
#define SWM_HIDE (WM_APP + 3)
#define SWM_EXIT (WM_APP + 4)
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
void				ShowContextMenu(HWND hWnd);
ULONGLONG			GetDllVersion(LPCTSTR lpszDllname);

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND g_hWnd;
HINSTANCE g_hInst;
NOTIFYICONDATA g_niData;
HFONT g_hFont;

HWND g_hTxt;
HWND g_hLst;
LVCOLUMN g_LvCol;
LVITEM g_LvItem;
int g_iSelect = 0;



LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnHotKey();

vector<LPITEMIDLIST> g_FullPidlExes;
vector<LPITEMIDLIST> g_RelPidlExes;
vector<int> g_UsageTimes;

void ShellTraverse();
int isRegularFile(const char *path);
//char* RelPidlToFullPath(LPITEMIDLIST pidl, char* path);

//char* RelPidlToName(LPITEMIDLIST pidl, char* name);
LPITEMIDLIST relativePidlToFullPidl(LPITEMIDLIST rPidl, LPITEMIDLIST pidlParent);
HRESULT SHPathToPidl(LPWSTR szPath, LPITEMIDLIST* ppidl);
void getExesPidl(vector<LPITEMIDLIST> pidlForFolderContainExe);

WNDPROC oldEditProc;
LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oldListViewProc;
LRESULT CALLBACK subListViewProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
