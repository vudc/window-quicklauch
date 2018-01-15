// Ho va ten: Duong Cong Vu
//MSSV: 1512680
//Email: dcongvu307@gmail.com
// 1512680@student.hcmus.edu.vn

#include "header.h"
void getMatchResult();
void viewStatistic();
wchar_t wPath1[1024];
wchar_t wPath2[1024];
wchar_t wPath3[1024];
wchar_t wPath4[1024];
wchar_t wPath5[1024];
wchar_t wPath6[1024];
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_QUICKLAUNCH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	RegisterHotKey(NULL, 1, MOD_WIN | MOD_NOREPEAT, VK_ESCAPE);
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKLAUNCH));
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (msg.message == WM_HOTKEY) {
			OnHotKey();
		}
		else if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKLAUNCH);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	InitCommonControls();

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - 360) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - 360) / 2;

	g_hInst = hInstance;
	g_hWnd = CreateWindowEx(WS_EX_TOPMOST, szWindowClass, szTitle, WS_MINIMIZEBOX,
		xPos, yPos, 350, 530, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd) return FALSE;

	ZeroMemory(&g_niData, sizeof(NOTIFYICONDATA));
	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
	if (ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
		g_niData.cbSize = sizeof(NOTIFYICONDATA);
	else g_niData.cbSize = NOTIFYICONDATA_V2_SIZE;

	g_niData.uID = TRAYICONID;

	// state which structure members are valid
	g_niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// load the icon
	g_niData.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	g_niData.hWnd = g_hWnd;
	g_niData.uCallbackMessage = SWM_TRAYMSG;

	// tooltip message
	lstrcpyn(g_niData.szTip, _T("Press Windows + Ecs for Main Window"), sizeof(g_niData.szTip) / sizeof(TCHAR));

	Shell_NotifyIcon(NIM_ADD, &g_niData);

	// free icon handle
	if (g_niData.hIcon && DestroyIcon(g_niData.hIcon))
		g_niData.hIcon = NULL;


	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent;

	switch (message) {
	case SWM_TRAYMSG:
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
		}
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		//On text change
		if (wmEvent == EN_CHANGE && wmId == IDC_EDIT) {
			getMatchResult();
			return 1;
		}

		switch (wmId)
		{
		case SWM_SCAN:
			ShellTraverse();
			break;
		case SWM_SHOW:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case SWM_HIDE:
			ShowWindow(hWnd, SW_HIDE);
			break;
		case SWM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 1;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			ShowWindow(g_hWnd, SW_HIDE);
			break;
		}
		else if (wParam == VK_RETURN) {
			int iPos = ListView_GetNextItem(g_hLst, -1, LVNI_SELECTED);
			if (iPos != -1) {
				ZeroMemory((char*)(&g_LvItem), sizeof(g_LvItem));

				g_LvItem.iItem = iPos;
				g_LvItem.mask = LVIF_PARAM;
				ListView_GetItem(g_hLst, &g_LvItem);

				char path[1024];
				wchar_t wpath[1024];
				SHGetPathFromIDList(g_FullPidlExes[g_LvItem.lParam], wPath1);
				WideCharToMultiByte(CP_ACP, 0, wPath1, 1024, path, 1024, NULL, NULL);
				mbstowcs(wpath, path, 1024);

				ShellExecute(GetDesktopWindow(), L"open", wpath, NULL, NULL, SW_SHOW);

				g_UsageTimes[g_LvItem.lParam]++;
			}

			ShowWindow(g_hWnd, SW_HIDE);
			break;
		}
		else {
			SetFocus(g_hTxt);
			SendMessage(g_hTxt, message, wParam, lParam);
			break;
		}
	
	case WM_CHAR:
		SetFocus(g_hTxt);
		SendMessage(g_hTxt, message, wParam, lParam);
		break;

		//Hide window
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		break;

		//restarts
	case WM_SHOWWINDOW:
		SetWindowText(g_hTxt, L"");
		SendMessage(g_hLst, LVM_DELETEALLITEMS, 0, 0);
		SetFocus(g_hTxt);
		break;

	case WM_CTLCOLOREDIT: {
		HDC hdc = (HDC)wParam;
		SetBkColor(hdc, 0x00EFD5C4);
		return (LRESULT)GetStockObject(DC_BRUSH);
	}

	HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
	HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}

void OnDestroy(HWND hwnd) {
	UnregisterHotKey(NULL, 1);
	g_niData.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &g_niData);
	PostQuitMessage(0);
}
HWND handletext;
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	ncm.lfMessageFont.lfHeight = 24;
	g_hFont = ::CreateFontIndirect(&ncm.lfMessageFont);
	handletext = CreateWindowEx(0, L"STATIC", L"Type you program below  ", WS_CHILD | WS_VISIBLE | SS_CENTER
		, 2, 2, 340, 24, hWnd, NULL, g_hInst, NULL);
	SendMessage(handletext, WM_SETFONT, WPARAM(g_hFont), TRUE);
	g_hTxt = CreateWindowEx(0, WC_EDIT, L"", WS_CHILD | WS_VISIBLE | ES_WANTRETURN, 0, 50, 360, 36, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);
	SendMessage(g_hTxt, WM_SETFONT, WPARAM(g_hFont), TRUE);
	oldEditProc = (WNDPROC)SetWindowLongPtr(g_hTxt, GWLP_WNDPROC, (LONG_PTR)subEditProc);

	g_hLst = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_NOCOLUMNHEADER | LVS_REPORT | LVS_SHOWSELALWAYS, 0, 90, 360, 502, hWnd, NULL, g_hInst, NULL);
	SendMessage(g_hLst, WM_SETFONT, WPARAM(g_hFont), TRUE);
	ListView_SetExtendedListViewStyle(g_hLst, LVS_EX_FULLROWSELECT);
	oldListViewProc = (WNDPROC)SetWindowLongPtr(g_hLst, GWLP_WNDPROC, (LONG_PTR)subListViewProc);

	memset(&g_LvCol, 0, sizeof(g_LvCol));
	g_LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	g_LvCol.cx = 360;
	g_LvCol.pszText = L"";

	SendMessage(g_hLst, LVM_INSERTCOLUMN, 0, (LPARAM)&g_LvCol);

	return true;
}

void OnHotKey() {
	ShowWindow(g_hWnd, SW_RESTORE);
	UpdateWindow(g_hWnd);
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if (hMenu)
	{
		if (IsWindowVisible(hWnd))
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, _T("Hide"));
		else
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SHOW, _T("Show"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SCAN, _T("Scan"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));

		// note:	must set window to the foreground or the
		//			menu won't disappear when it should
		SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}

ULONGLONG GetDllVersion(LPCTSTR lpszDllName) {
	ULONGLONG ullVersion = 0;
	HINSTANCE hInstDll;
	hInstDll = LoadLibrary(lpszDllName);
	if (hInstDll) {
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hInstDll, "DllGetVersion");
		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);
			if (SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion, 0, 0);
		}
		FreeLibrary(hInstDll);
	}
	return ullVersion;
}


//Duyet file exe ===================================================
void ShellTraverse() {
	LPITEMIDLIST pidPF = NULL;
	LPITEMIDLIST pidWs = NULL;
	LPITEMIDLIST pidlSystem32 = NULL;
	LPITEMIDLIST pidlSysWOW64 = NULL;

	SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES, &pidPF);
	SHGetSpecialFolderLocation(NULL, CSIDL_WINDOWS, &pidWs);
	SHGetSpecialFolderLocation(NULL, CSIDL_SYSTEMX86, &pidlSystem32);
	SHGetSpecialFolderLocation(NULL, CSIDL_SYSTEM, &pidlSysWOW64);

	vector<LPITEMIDLIST> pidlForFolderContainExe;
	pidlForFolderContainExe.push_back(pidWs);
	pidlForFolderContainExe.push_back(pidlSystem32);
	pidlForFolderContainExe.push_back(pidlSysWOW64);

	//Lay pidl cua tat ca thu muc con cua thu muc Program Files
	LPMALLOC pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	LPENUMIDLIST penumIDL = NULL;

	LPSHELLFOLDER pFolder = NULL;

	LPSHELLFOLDER tmpPsf = NULL;
	SHGetDesktopFolder(&tmpPsf);
	tmpPsf->BindToObject(pidPF, NULL, IID_IShellFolder, reinterpret_cast<LPVOID*>(&pFolder));
	tmpPsf->Release();

	pFolder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &penumIDL);

	LPITEMIDLIST pidl = NULL;
	HRESULT hr = NULL;
	do {
		hr = penumIDL->Next(1, &pidl, NULL);
		if (hr == S_OK) {
			//Chuyen relative pidl sang absolute pidl roi luu lai
			LPITEMIDLIST absolutePidl = relativePidlToFullPidl(pidl, pidPF);

			pidlForFolderContainExe.push_back(absolutePidl);
			char path[1024];
			SHGetPathFromIDList(absolutePidl, wPath2);
			WideCharToMultiByte(CP_ACP, 0, wPath2, 1024, path, 1024, NULL, NULL);

			pMalloc->Free(pidl);
		}
	} while (hr == S_OK);
	pMalloc->Release();
	pFolder->Release();

	getExesPidl(pidlForFolderContainExe);
}

int isRegularFile(const char *path) {
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

/////////////

char* RelPidlToFullPath(LPITEMIDLIST pidl, char* path) {
	STRRET strret;
	TCHAR buffer[1024];

	LPSHELLFOLDER pParent = NULL;

	SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&pParent, NULL);

	pParent->GetDisplayNameOf(pidl, SHGDN_NORMAL | SHGDN_FORPARSING, &strret);

	StrRetToBuf(&strret, pidl, buffer, 1024);
	WideCharToMultiByte(CP_ACP, 0, buffer, 1024, path, 1024, NULL, NULL);

	pParent->Release();

	return path;
}

char* RelPidlToName(LPITEMIDLIST pidl, char* name) {
	STRRET strret;
	TCHAR buffer[1024];

	LPSHELLFOLDER pParent = NULL;

	SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&pParent, NULL);

	pParent->GetDisplayNameOf(pidl, SHGDN_NORMAL, &strret);
	StrRetToBuf(&strret, pidl, buffer, 1024);
	WideCharToMultiByte(CP_ACP, 0, buffer, 1024, name, 1024, NULL, NULL);

	pParent->Release();

	return name;
}

LPITEMIDLIST relativePidlToFullPidl(LPITEMIDLIST rPidl, LPITEMIDLIST pidlParent) {
	LPITEMIDLIST absolutePidl = NULL;

	char path[1024] = { '\0' };
	SHGetPathFromIDList(pidlParent, wPath3);
	WideCharToMultiByte(CP_ACP, 0, wPath3, 1024, path, 1024, NULL, NULL);
	char name[1024] = { '\0' };
	RelPidlToName(rPidl, name);

	sprintf(path + strlen(path), "\\");
	strcpy(path + strlen(path), name);

	wchar_t wpath[1024];
	mbstowcs(wpath, path, strlen(path) + 1);

	SHPathToPidl(wpath, &absolutePidl);
	return absolutePidl;
}

HRESULT SHPathToPidl(LPWSTR szPath, LPITEMIDLIST* ppidl)
{
	LPSHELLFOLDER pShellFolder = NULL;
	ULONG nCharsParsed = 0;
	// Get an IShellFolder interface pointer
	HRESULT hr = SHGetDesktopFolder(&pShellFolder);
	if (FAILED(hr))
		return hr;
	// Call ParseDisplayName() to do the job
	hr = pShellFolder->ParseDisplayName(NULL, NULL, szPath, &nCharsParsed, ppidl, NULL);
	// Clean up
	pShellFolder->Release();
	return hr;
}

void getExesPidl(vector<LPITEMIDLIST> pidlForFolderContainExe) {
	for (int i = 0; i < pidlForFolderContainExe.size(); i++) {
		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);
		LPENUMIDLIST penumIDL = NULL;

		LPSHELLFOLDER pFolder = NULL;
		LPSHELLFOLDER tmpPsf = NULL;
		SHGetDesktopFolder(&tmpPsf);
		tmpPsf->BindToObject(pidlForFolderContainExe[i], NULL, IID_IShellFolder, reinterpret_cast<LPVOID*>(&pFolder));
		tmpPsf->Release();

		pFolder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &penumIDL);

		LPITEMIDLIST pidl = NULL;
		HRESULT hr = NULL;
		do {
			hr = penumIDL->Next(1, &pidl, NULL);
			if (hr == S_OK) {
				//Chuyen relative pidl thanh absulute pidl roi luu lai
				LPITEMIDLIST absolutePidl = relativePidlToFullPidl(pidl, pidlForFolderContainExe[i]);
				char fullPath[1024];
				SHGetPathFromIDList(absolutePidl, wPath4);
				WideCharToMultiByte(CP_ACP, 0, wPath4, 1024, fullPath, 1024, NULL, NULL);
				if (isRegularFile(fullPath)) {
					int len = strlen(fullPath);
					if (fullPath[len - 4] == '.' && fullPath[len - 3] == 'e' && fullPath[len - 2] == 'x' && fullPath[len - 1] == 'e') {
						g_FullPidlExes.push_back(absolutePidl);
						g_RelPidlExes.push_back(pidl);
					}
				}

				//pMalloc->Free(pidl);
			}
		} while (hr == S_OK);
		pMalloc->Release();
		pFolder->Release();
	}
	int nUsedTime = 0;
	g_UsageTimes.resize(g_FullPidlExes.size(), nUsedTime);
}

LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		//Hide wwindow
		case VK_ESCAPE:
			ShowWindow(g_hWnd, SW_HIDE);
			break;
		case WM_CTLCOLORSTATIC:
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		case VK_RETURN:
		{
			int iPos = ListView_GetNextItem(g_hLst, -1, LVNI_SELECTED);
			if (iPos != -1) {
				ZeroMemory((char*)(&g_LvItem), sizeof(g_LvItem));

				g_LvItem.iItem = iPos;
				g_LvItem.mask = LVIF_PARAM;
				ListView_GetItem(g_hLst, &g_LvItem);

				char path[1024];
				wchar_t wpath[1024];
				SHGetPathFromIDList(g_FullPidlExes[g_LvItem.lParam], wPath5);
				WideCharToMultiByte(CP_ACP, 0, wPath5, 1024, path, 1024, NULL, NULL);
				mbstowcs(wpath, path, 1024);

				ShellExecute(GetDesktopWindow(), L"open", wpath, NULL, NULL, SW_SHOW);

				g_UsageTimes[g_LvItem.lParam]++;
			}

			ShowWindow(g_hWnd, SW_HIDE);
			return 0;
		}
		//Chuc nang dung phim mui ten de chon app
		case VK_UP: case VK_DOWN:
			SetFocus(g_hLst);
			SendMessage(g_hLst, msg, wParam, lParam);
			return 0;
		}
		

	default:
		return CallWindowProc(oldEditProc, wnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK subListViewProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		//Tu Dong An Cua so khi nhan Esc
		case VK_ESCAPE:
			ShowWindow(g_hWnd, SW_HIDE);
			break;
		case VK_RETURN:
		{
			int iPos = ListView_GetNextItem(g_hLst, -1, LVNI_SELECTED);
			if (iPos != -1) {
				ZeroMemory((char*)(&g_LvItem), sizeof(g_LvItem));

				g_LvItem.iItem = iPos;
				g_LvItem.mask = LVIF_PARAM;
				ListView_GetItem(g_hLst, &g_LvItem);

				char path[1024];
				wchar_t wpath[1024];
				SHGetPathFromIDList(g_FullPidlExes[g_LvItem.lParam], wPath6);
				WideCharToMultiByte(CP_ACP, 0, wPath6, 1024, path, 1024, NULL, NULL);
				mbstowcs(wpath, path, 1024);

				ShellExecute(GetDesktopWindow(), L"open", wpath, NULL, NULL, SW_SHOW);

				g_UsageTimes[g_LvItem.lParam]++;
			}

			ShowWindow(g_hWnd, SW_HIDE);
			return 0;
		}
		case VK_DOWN:
			g_iSelect = ListView_GetNextItem(g_hLst, -1, LVNI_SELECTED);
			if (g_iSelect == ListView_GetItemCount(g_hLst) - 1) {
				return CallWindowProc(oldListViewProc, wnd, msg, VK_HOME, lParam);
			}
			else {
				return CallWindowProc(oldListViewProc, wnd, msg, wParam, lParam);
			}

		case VK_UP:
			g_iSelect = ListView_GetNextItem(g_hLst, -1, LVNI_SELECTED);
			if (g_iSelect == 0) {
				return CallWindowProc(oldListViewProc, wnd, msg, VK_END, lParam);
			}
			else {
				return CallWindowProc(oldListViewProc, wnd, msg, wParam, lParam);
			}

		default:
			return CallWindowProc(oldListViewProc, wnd, msg, wParam, lParam);
		}

	//auto focus text box
	case WM_CHAR:
		SetFocus(g_hTxt);
		return 0;

	default:
		return CallWindowProc(oldListViewProc, wnd, msg, wParam, lParam);
	}
	return 0;
}


void getMatchResult() {
	wchar_t text[200];
	GetWindowText(g_hTxt, text, 200);

	if (wcslen(text) > 0) {
		char searchString[200];
		wcstombs(searchString, text, 200);

		int nMatchCase = 0;
		vector<int> matchCases;
		char path[1024];
		int size = g_FullPidlExes.size();
		for (int i = 0; i < size; i++) {
			RelPidlToName(g_RelPidlExes[i], path);
			if (strstr(path, searchString) != 0) {
				matchCases.push_back(i);
			}
		}
		nMatchCase = matchCases.size();
		SendMessage(g_hLst, LVM_DELETEALLITEMS, 0, 0);

		if (nMatchCase > 0) {

			//Sort 3 frist app
			int FirstUse = 0;
			int Second = 0;
			int Thirst = 0;
			for (int i = 1; i < nMatchCase; i++) {
				if (g_UsageTimes[matchCases[i]] > g_UsageTimes[matchCases[FirstUse]]) {
					Thirst = Second;
					Second = FirstUse;
					FirstUse = i;
				}
				else if (g_UsageTimes[matchCases[i]] > g_UsageTimes[matchCases[Second]] || FirstUse == Second) {
					Thirst = Second;
					Second = i;
				}
				else if (g_UsageTimes[matchCases[i]] > g_UsageTimes[matchCases[Thirst]] || FirstUse == Second || Thirst == Second) {
					Thirst = i;
				}
			}

			swap(matchCases[FirstUse], matchCases[nMatchCase - 1]);
			if (nMatchCase >= 2) swap(matchCases[Second], matchCases[nMatchCase - 2]);
			if (nMatchCase >= 3) swap(matchCases[Thirst], matchCases[nMatchCase - 3]);


			//Show winndow
			for (int i = 0; i < nMatchCase; i++) {
				char text[1024];
				RelPidlToName(g_RelPidlExes[matchCases[i]], text);

				wchar_t wtext[1024];
				mbstowcs(wtext, text, 1024);

				memset(&g_LvItem, 0, sizeof(g_LvItem));

				g_LvItem.mask = LVIF_TEXT | LVIF_PARAM;
				g_LvItem.cchTextMax = 1024;

				g_LvItem.iItem = 0;
				g_LvItem.iSubItem = 0;
				g_LvItem.lParam = (LPARAM)(i);
				g_LvItem.pszText = wtext;

				SendMessage(g_hLst, LVM_INSERTITEM, 0, (LPARAM)&g_LvItem);
			}
			ListView_SetItemState(g_hLst, 0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
			SendMessage(g_hLst, WM_KEYDOWN, VK_HOME, 0);
		}
	}
	else {
		viewStatistic();
	}
}

void viewStatistic() {
}
