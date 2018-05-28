#include <windows.h> // 윈도우 헤더 파일
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define windowX 800
#define windowY 600

#define IDC_REVERSE 100
#define IDC_EXIT 101

#define IDC_R1 102
#define IDC_R2 103
#define IDC_R3 104
#define IDC_R4 105
#define IDC_R5 106
#define IDC_R6 107

#define IDC_SCROLL 108

HINSTANCE g_hInst;
LPCTSTR lpszClass = _T("Window Class Name");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //메인
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, NULL, WS_OVERLAPPEDWINDOW, 0, 0, windowX, windowY, NULL, (HMENU)NULL, hInstance, NULL);

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = "ChildClass"; // 차일드 윈도우 클래스 이름
	WndClass.lpfnWndProc = (WNDPROC)ChildProc; // 차일드 윈도우 프로시저 지정
	RegisterClassEx(&WndClass);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
HDC hDC;
BOOL reverse;
BOOL Break = FALSE;
int move;

int Shape = 0;
int size = 0;
int anim = 0;
int cage = 85;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	HWND child_hWnd;
	static HWND hbutton[2];
	static HWND rbutton[6];
	static HWND scroll;

	int x = 0, y = 0;
	switch (iMessage)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);

		hbutton[0] = CreateWindow("button", "돌아가", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 530, 480, 50, 50, hWnd, (HMENU)IDC_REVERSE, g_hInst, NULL);
		hbutton[1] = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 480, 50, 50, hWnd, (HMENU)IDC_EXIT, g_hInst, NULL);
		CreateWindow("button", "Graph", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 530, 5, 120, 110, hWnd, (HMENU)NULL, g_hInst, NULL);
		rbutton[0] = CreateWindow("button", "Circle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 540, 20, 100, 30, hWnd, (HMENU)IDC_R1, g_hInst, NULL);
		rbutton[1] = CreateWindow("button", "Sin", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 540, 50, 100, 30, hWnd, (HMENU)IDC_R2, g_hInst, NULL);
		rbutton[2] = CreateWindow("button", "Spring", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 540, 80, 100, 30, hWnd, (HMENU)IDC_R3, g_hInst, NULL);
		CheckRadioButton(hWnd, IDC_R1, IDC_R3, IDC_R1);
		CreateWindow("button", "Size", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 530, 100, 120, 110, hWnd, (HMENU)NULL, g_hInst, NULL);
		rbutton[3] = CreateWindow("button", "좀 큼", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 540, 115, 100, 30, hWnd, (HMENU)IDC_R4, g_hInst, NULL);
		rbutton[4] = CreateWindow("button", "적당히 큼", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 540, 145, 100, 30, hWnd, (HMENU)IDC_R5, g_hInst, NULL);
		rbutton[5] = CreateWindow("button", "시력체크", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 540, 175, 100, 30, hWnd, (HMENU)IDC_R6, g_hInst, NULL);
		CheckRadioButton(hWnd, IDC_R4, IDC_R6, IDC_R4);
		scroll = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 530, 230, 170, 20, hWnd, (HMENU)IDC_SCROLL, g_hInst, NULL);
		SetScrollRange(scroll, SB_CTL, 0, 170, TRUE);
		SetScrollPos(scroll, SB_CTL, cage, TRUE);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_PAINT: //Paint 메세지 불렸을 때
		hDC = BeginPaint(hWnd, &ps);
		child_hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, "ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_THICKFRAME, 0, 0, 500, 500, hWnd, (HMENU)NULL, g_hInst, NULL);

		EndPaint(hWnd, &ps);
		break;
	case WM_HSCROLL:
	{
		int TempPos = cage;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT: TempPos = max(0, TempPos - 1); break;
		case SB_LINERIGHT: TempPos = min(255, TempPos + 1); break;
		case SB_PAGELEFT: TempPos = max(0, TempPos - 10); break;
		case SB_PAGERIGHT: TempPos = min(255, TempPos + 10); break;
		case SB_THUMBTRACK: TempPos = HIWORD(wParam); break;
		}
		cage = TempPos;
		Break = TRUE;
		SetScrollPos((HWND)lParam, SB_CTL, TempPos, TRUE);
		//InvalidateRect(hWnd, NULL, true);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_REVERSE:
			if (reverse)
				reverse = FALSE;
			else
				reverse = TRUE;
			break;
		case IDC_EXIT:
			PostQuitMessage(0);
			break;
		case IDC_R1:
			Shape = 0;
			anim = 0;
			break;
		case IDC_R2:
			Shape = 1;
			anim = 0;
			break;
		case IDC_R3:
			Shape = 2;
			anim = -500;
			break;
		case IDC_R4:
			size = 0;
			break;
		case IDC_R5:
			size = 1;
			break;
		case IDC_R6:
			size = 2;
			break;
		}
		break;
	case WM_DESTROY: //Destroy 메세지 불렸을 때
		PostQuitMessage(0); //창 종료
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam)); //처리되지 않은 메세지는 여기서 처리
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	RECT cirRECT;
	static HBRUSH hBrush;
	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 85, NULL);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MoveToEx(hDC, 250, 0, NULL);
		LineTo(hDC, 250, 500);
		MoveToEx(hDC, 0, 250, NULL);
		LineTo(hDC, 500, 250);

		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(hDC, hBrush);
		switch (Shape)
		{
		case 0:
			Ellipse(hDC, 50, 50, 450, 450);
			break;
		case 1:
			for (int f = 0; f < 500; f++)
			{
				int y = (int)(sin(f*3.14 / 180) * 100) + 250;
				SetPixel(hDC, (int)f, y, RGB(0, 0, 0));
			}
			break;
		case 2:
			for (int f = -500; f < 500; f++)
			{
				int x = (int)(cos(f*3.14 / 180) * 100) + 250;
				int y = (int)(sin(f*3.14 / 180) * 100) + 250;
				SetPixel(hDC, x + f / 3.14, y, RGB(0, 0, 0));
			}
			break;
		}
		hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		SelectObject(hDC, hBrush);
		switch (Shape)
		{
		case 0:
		{
			int x = (int)(cos(anim*3.14 / 180) * 200) + 250;
			int y = (int)(sin(anim*3.14 / 180) * 200) + 250;
			Ellipse(hDC, x - (15 - size * 5), y - (15 - size * 5), x + (15 - size * 5), y + (15 - size * 5));
		}
		break;
		case 1:
		{
			int x = anim;
			int y = (int)(sin(anim*3.14 / 180) * 100) + 250;
			Ellipse(hDC, x - (15 - size * 5), y - (15 - size * 5), x + (15 - size * 5), y + (15 - size * 5));
		}
		break;
		case 2:
		{
			int x = (int)(cos(anim*3.14 / 180) * 100) + 250;
			int y = (int)(sin(anim*3.14 / 180) * 100) + 250;
			Ellipse(hDC, x - (15 - size * 5) + anim / 3.14, y - (15 - size * 5), x + (15 - size * 5) + anim / 3.14, y + (15 - size * 5));
		}
		break;
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (wParam == 1)
		{
			if (Break)
			{
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, 170 - cage, NULL);
				Break = FALSE;
			}
			if (reverse)
				anim--;
			else
				anim++;

			switch (Shape)
			{
			case 0:
				if (reverse)
				{
					if (anim < 0)
						anim = 360;
				}
				else
				{
					if (anim > 359)
						anim = 0;
				}

				break;
			case 1:
				if (reverse)
				{
					if (anim < 0)
						anim = 500;
				}
				else
				{
					if (anim > 499)
						anim = 0;
				}

				break;
			case 2:
				if (reverse)
				{
					if (anim < -499)
						anim = 500;
				}
				else
				{
					if (anim > 499)
						anim = -500;
				}

				break;
			}
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}