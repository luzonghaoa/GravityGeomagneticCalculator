// GGA.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GGA.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣
	AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
	Func::clean();

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GGA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GGA));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
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

// �����ڡ������Ϣ�������
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
