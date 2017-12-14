// GGA.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GGA.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
	AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
	Func::clean();

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GGA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GGA));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GGA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GGA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_GGA));

    return RegisterClassExW(&wcex);
}

INT_PTR CALLBACK Input(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,
	   CW_USEDEFAULT,0 , 1000, 800, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
			CFileDialog dlgopen(TRUE, L"dat", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||");
			CFileDialog dlgsave(FALSE, L"dat", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||");
			int ret;
            switch (wmId)
            {
			case ID_IMPORT:
				ret=MessageBox(hWnd, L"You may lost all data in workspace, do you still want to continue?", L"Warning!", MB_OKCANCEL);
				if (ret==1&&dlgopen.DoModal() == IDOK) {
					Func::readFile(CStringA(dlgopen.GetPathName()).GetBuffer());
					InvalidateRect(hWnd, NULL, true);
					
				}
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO ,MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
			case ID_EXPORT:
				if (Func::record.length == 0) MessageBox(hWnd, L"No data in workspace!", L"Error!", MB_OK);
				else if (dlgsave.DoModal() == IDOK) {
					InvalidateRect(hWnd, NULL, true);
					Func::writeFile(CStringA(dlgsave.GetPathName()).GetBuffer());
				}
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
			case ID_UNDO:
				Func::undo();
				InvalidateRect(hWnd, NULL, true);
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
			case ID_REDO:
				Func::redo();
				InvalidateRect(hWnd, NULL, true);
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
			case ID_CONTINUATION:
				if (Func::record.length == 0) MessageBox(hWnd, L"No data in workspace!", L"Error!", MB_OK);
				else{
					ret = MessageBox(hWnd, L"Are you sure you want do the continuation?", L"Warning!", MB_OKCANCEL);
					if (ret == 1&&DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUTBOX), hWnd, Input)) {
						InvalidateRect(hWnd, NULL, true);
					}
				}
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
			case ID_DERIVATION:
				if (Func::record.length == 0) MessageBox(hWnd, L"No data in workspace!", L"Error!", MB_OK);
				else {
					ret = MessageBox(hWnd, L"Are you sure you want to do the derivation?", L"Warning!", MB_OKCANCEL);
					if (ret == 1) {
						Func::derivation();
						InvalidateRect(hWnd, NULL, true);
					}
				}
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
			case ID_CLEAN:
				ret = MessageBox(hWnd, L"You may lost all data in workspace, do you still want to continue?", L"Warning!", MB_OKCANCEL);
				if (ret == 1) {
					Func::clean();
					InvalidateRect(hWnd, NULL, true);
				}
				if (Func::record.length == 1) EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_UNDO, MF_ENABLED);
				if (Func::record.length == Func::record.logs.size()) EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_GRAYED);
				else EnableMenuItem(GetMenu(hWnd), ID_REDO, MF_ENABLED);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				ret = MessageBox(hWnd, L"You may lost all data in workspace, do you still want to continue?", L"Warning!", MB_OKCANCEL);
				if (ret==1) DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			DrawText(ps.hdc, Func::message, -1, &(ps.rcPaint), DT_LEFT);
			Func::draw(hWnd);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Input(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				WCHAR p[100];
				double z;
				GetDlgItemText(hDlg, IDC_TEXT, p, 100);
				swscanf(p,L"%lf", &z);
				Func::continuation(z);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
