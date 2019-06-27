// ������������.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "������������.h"
#include <shellapi.h>

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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
    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
	
    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 900, 800, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Engine engine;
	static RECT rect;
	static HDC memDC;
	static HDC gDC;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
			{
				SendMessage(hWnd, WM_TIMER, 0, 0);
				//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			}
                break;
            case IDM_EXIT:
                //DestroyWindow(hWnd);
				//menu.animationClose(hWnd, &menu, true);
				//mainF.animationClose(hWnd, &mainF, true);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_IME_COMPOSITION:
	{
		static char a[3];
		WideCharToMultiByte(CP_ACP, 0, (WCHAR*)&wParam, 2, a, 2, nullptr, nullptr);
		a[2] = 0;
		if (lParam == GCS_RESULTSTR)
			engine.keyEvent(hWnd, a, true);
		else
			engine.keyEvent(hWnd, a, false);
		TextOutA(gDC, 0, 0, a, strlen(a));
	}
		break;
	case WM_CHAR:
	{
		static char a[3];
		a[0] = wParam;
		a[1] = 0;
		a[2] = 0;
		engine.keyEvent(hWnd, a, true);
		TextOutA(gDC, 0, 0, a, strlen(a));
	}
		break;
	case WM_SIZE:
		SetRect(&rect, 0, 0, engine.mainFrame.width, engine.mainFrame.height);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		MoveWindow(hWnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, true);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			//BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, rect.right, rect.bottom,  memDC, 0, 0, engine.mainFrame.width, engine.mainFrame.height, SRCCOPY);
			//BitBlt(gDC, 0, 0, 800, 600, memDC, 0, 0, SRCCOPY);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONUP:
	{
		long x = LOWORD(lParam);
		long y = HIWORD(lParam);
		engine.mouseLUp(hWnd, x, y);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		long x = LOWORD(lParam);
		long y = HIWORD(lParam);
		engine.mouseLDown(hWnd, x, y);
	}
	break;
		
	case WM_MOUSEMOVE:
	{
		long x = LOWORD(lParam);
		long y = HIWORD(lParam);
		engine.mouseMove(hWnd, x, y);
	}
	break;
	case WM_TIMER:
	{
		volatile static bool running = false;
		volatile static unsigned int frame = 0;
		switch (wParam) {
		case 0:
		{
			if (!running) {
				DeleteDC(memDC);
				ReleaseDC(hWnd, gDC);
				gDC = GetDC(hWnd);
				memDC = CreateCompatibleDC(gDC);
				SelectObject(memDC, CreateCompatibleBitmap(gDC, engine.mainFrame.width, engine.mainFrame.height));
			}
		}
		break;
		case 1:// SetTimer�� ����� ���� �ƴ϶� SendMessage�� wParam�� 1�� �Ѿ�� ���
		{
			if (!running){
				running = true;
				SetTimer(hWnd, 2, 13, nullptr);
			}
			frame = 0;
		}
		break;
		case 2:// �̰� ��¥ Ÿ�̸�
		{
			if (frame < 200) {
				DeleteObject(SelectObject(memDC, CreateSolidBrush(RGB(0, 0, 0))));
				Rectangle(memDC, 0, 0, engine.mainFrame.width, engine.mainFrame.height);
				engine.draw(memDC);
				//BitBlt(gDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
				StretchBlt(gDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, engine.mainFrame.width, engine.mainFrame.height, SRCCOPY);
				//InvalidateRect(hWnd, 0, true);
				frame++;
			}
			else {
				running = false;
				KillTimer(hWnd, 2);
			}
		}
		break;
		}
	}
		break;
	
	case WM_CREATE:
	{
		gDC = GetDC(hWnd);
		//SetStretchBltMode(gDC, HALFTONE);
		memDC = CreateCompatibleDC(gDC);
		SetRect(&rect, 0, 0, engine.mainFrame.width, engine.mainFrame.height);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		MoveWindow(hWnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top,true);
		SelectObject(memDC, CreateCompatibleBitmap(gDC, 1500, 1500));
		engine.start(hWnd);
	}
		break;
    case WM_DESTROY:
		DeleteDC(memDC);
		ReleaseDC(hWnd,gDC);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
