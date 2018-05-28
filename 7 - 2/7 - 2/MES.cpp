#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
#include "resource.h"

#define IDC_CHECK1 101
#define IDC_CHECK2 102
#define IDC_CHECK3 103
#define IDC_CHECK4 104
#define IDC_CHECK5 105
#define IDC_CHECK6 106
#define IDC_CHECK7 107
#define IDC_CHECK8 108
#define IDC_CHECK9 109

#define IDC_MAP1 201
#define IDC_MAP2 202


LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

BOOL CollisionCheck(int x, int y, int mx, int my);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //메인
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hinst = hInstance;
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
	hWnd = CreateWindow(lpszClass, NULL, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = "ChildClass"; // 차일드 윈도우 클래스 이름
	WndClass.lpfnWndProc = (WNDPROC)ChildProc; // 차일드 윈도우 프로시저 지정
	RegisterClassEx(&WndClass);
//WndClass.lpfnWndProc = (WNDPROC)ChildProc2; // 차일드 윈도우 프로시저 지정

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = "ChildClass2"; // 차일드 윈도우 클래스 이름
	WndClass.lpfnWndProc = (WNDPROC)ChildProc2; // 차일드 윈도우 프로시저 지정
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
HDC hdc, memdc, memdc2;

struct Moving {
	int x = 30;
	int y = 30;
};

static BOOL MoveX = FALSE;
static BOOL MoveY = FALSE;
static BOOL KillMoveX = FALSE;
static BOOL KillMoveY = FALSE;
static int count1 = 0, count2 = 0;
static int Movecount[4] = { 0};
static BOOL MoveX2 = FALSE;
static BOOL MoveY2 = FALSE;
static BOOL KillMoveX2 = FALSE;
static BOOL KillMoveY2 = FALSE;

static BOOL JUMP1 = FALSE;
static BOOL JUMP2 = FALSE;

static BOOL CLOSE = FALSE;
static BOOL dir[2] = { TRUE };
static BOOL dir2[2] = { TRUE };

static BOOL CHANGE = FALSE;

static Moving rectangle;
static Moving Esi;
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	

	switch (uMsg) {

	case WM_CREATE:

		
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (CHANGE == FALSE) {
				if (rectangle.x > 300)
					dir[0] = FALSE;
				else if (rectangle.x < 0)
					dir[0] = TRUE;

				if (dir[0] == TRUE)
					rectangle.x += 10;
				else
					rectangle.x -= 10;
			}
			else {
				if (Esi.x > 300)
					dir2[0] = FALSE;
				else if (Esi.x < 0)
					dir2[0] = TRUE;

				if (dir2[0] == TRUE)
					Esi.x += 10;
				else
					Esi.x -= 10;
			}
			break;

		case 2:
			if (CHANGE == FALSE) {
				if (rectangle.y > 300)
					dir[1] = FALSE;
				else if (rectangle.y < 0)
					dir[1] = TRUE;

				if (dir[1] == TRUE)
					rectangle.y += 10;
				else
					rectangle.y -= 10;
			}
			else {
				if (Esi.y > 300)
					dir2[1] = FALSE;
				else if (Esi.y < 20)
					dir2[1] = TRUE;

				if (dir2[1] == TRUE)
					Esi.y += 10;
				else
					Esi.y -= 10;
			}
			break;

		case 3:
			if (CHANGE == FALSE) {
				if (count1 < 5) {
					rectangle.y -= 5;
					count1++;
				}
				else {
					rectangle.y += 5;
					count1++;
				}

				if (count1 == 10) {
					count1 = 0;
					KillTimer(hWnd, 3);
				}
			}
			else {
				if (count1 < 5) {
					Esi.y -= 5;
					count1++;
				}
				else {
					Esi.y += 5;
					count1++;
				}

				if (count1 == 10) {
					count1 = 0;
					KillTimer(hWnd, 3);
				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (MoveX == TRUE) {
			SetTimer(hWnd, 1, 100, NULL);
			MoveX = FALSE;
		}
		if (MoveY == TRUE) {
			SetTimer(hWnd, 2, 100, NULL);
			MoveY = FALSE;
		}

		if (KillMoveX == TRUE) {
			KillTimer(hWnd, 1);
			KillMoveX = FALSE;
		}

		if (KillMoveY == TRUE) {
			KillTimer(hWnd, 2);
			KillMoveY = FALSE;
		}

		if (JUMP1 == TRUE) {
			SetTimer(hWnd, 3, 10, NULL);
			JUMP1 = FALSE;
		}
		
		if (CHANGE == FALSE)
			Rectangle(hdc, rectangle.x, rectangle.y, rectangle.x + 30, rectangle.y + 30);
		else
			Ellipse(hdc, Esi.x - 20, Esi.y - 20, Esi.x + 20, Esi.y + 20);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0); 
		break;    
	}   
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
} 



LRESULT CALLBACK ChildProc2(HWND hDlg, UINT uMsg2, WPARAM wParam2, LPARAM lParam2) {

	PAINTSTRUCT ps;

	switch (uMsg2) {
	case WM_LBUTTONDOWN: // 마우스 좌측 버튼을 누른 경우 

		break;
	case WM_CREATE:

		break;

	case WM_TIMER:
		switch (wParam2) {
		case 1:
			if (CHANGE == FALSE) {
				if (Esi.x > 300)
					dir2[0] = FALSE;
				else if (Esi.x < 0)
					dir2[0] = TRUE;

				if (dir2[0] == TRUE)
					Esi.x += 10;
				else
					Esi.x -= 10;
			}
			else {
				if (rectangle.x > 300)
					dir[0] = FALSE;
				else if (rectangle.x < 0)
					dir[0] = TRUE;

				if (dir[0] == TRUE)
					rectangle.x += 10;
				else
					rectangle.x -= 10;
			}
			break;

		case 2:
			if (CHANGE == FALSE) {
				if (Esi.y > 300)
					dir2[1] = FALSE;
				else if (Esi.y < 20)
					dir2[1] = TRUE;

				if (dir2[1] == TRUE)
					Esi.y += 10;
				else
					Esi.y -= 10;
			}
			else {
				if (rectangle.y > 300)
					dir[1] = FALSE;
				else if (rectangle.y < 0)
					dir[1] = TRUE;

				if (dir[1] == TRUE)
					rectangle.y += 10;
				else
					rectangle.y -= 10;
			}
			break;


		case 3:
			if (CHANGE == FALSE) {
				if (count2 < 5) {
					Esi.y -= 5;
					count2++;
				}
				else {
					Esi.y += 5;
					count2++;
				}

				if (count2 == 10) {
					count2 = 0;
					KillTimer(hDlg, 3);
				}
			}
			else
			{
				if (count2 < 5) {
					rectangle.y -= 5;
					count2++;
				}
				else {
					rectangle.y += 5;
					count2++;
				}

				if (count2 == 10) {
					count2 = 0;
					KillTimer(hDlg, 3);
				}
			}
			break;
		}

		InvalidateRect(hDlg, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);

		if (MoveX2 == TRUE) {
			SetTimer(hDlg, 1, 100, NULL);
			MoveX2 = FALSE;
		}
		if (MoveY2 == TRUE) {
			SetTimer(hDlg, 2, 100, NULL);
			MoveY2 = FALSE;
		}

		if (KillMoveX2 == TRUE) {
			KillTimer(hDlg, 1);
			KillMoveX2 = FALSE;
		}

		if (KillMoveY2 == TRUE) {
			KillTimer(hDlg, 2);
			KillMoveY2 = FALSE;
		}

		if (JUMP2 == TRUE) {
			SetTimer(hDlg, 3,10,NULL);
			JUMP2 = FALSE;
		}
		if (CHANGE == FALSE)
			Ellipse(hdc, Esi.x - 20, Esi.y - 20, Esi.x + 20, Esi.y + 20);
		else
			Rectangle(hdc, rectangle.x, rectangle.y, rectangle.x + 30, rectangle.y + 30);
		EndPaint(hDlg, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hDlg, uMsg2, wParam2, lParam2);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	static int count = 0;
	static HWND hCheck[9], hList, child_hWnd, board1, board2;
	static int cList[2];

	switch (iMessage) {
	case WM_CREATE:
	
//		board1 = CreateWindow("CHILD", "좀 나와바" , WS_CHILD | WS_CHILD | WS_BORDER, 50, 50, 300, 300, hWnd, NULL, NULL, NULL);
		hCheck[0] = CreateWindow("button", "1번 윈도우 좌우", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 140, 400, 140, 30, hWnd, (HMENU)IDC_CHECK1, g_hinst, NULL);
		hCheck[1] = CreateWindow("button", "1번 윈도우 상하", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 290, 400, 140, 30, hWnd, (HMENU)IDC_CHECK2, g_hinst, NULL);
		hCheck[2] = CreateWindow("button", "1번 윈도우 JUMP", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 400, 140, 30, hWnd, (HMENU)IDC_CHECK3, g_hinst, NULL);

		hCheck[3] = CreateWindow("button", "2번 윈도우 좌우", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 140, 450, 140, 30, hWnd, (HMENU)IDC_CHECK4, g_hinst, NULL);
		hCheck[4] = CreateWindow("button", "2번 윈도우 상하", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 290, 450, 140, 30, hWnd, (HMENU)IDC_CHECK5, g_hinst, NULL);
		hCheck[5] = CreateWindow("button", "2번 윈도우 JUMP", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 450, 140, 30, hWnd, (HMENU)IDC_CHECK6, g_hinst, NULL);

		hCheck[6] = CreateWindow("button", "두 도형이 정지함", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 140, 500, 140, 30, hWnd, (HMENU)IDC_CHECK7, g_hinst, NULL);
		hCheck[7] = CreateWindow("button", "두 도형이 바뀐다", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 290, 500, 140, 30, hWnd, (HMENU)IDC_CHECK8, g_hinst, NULL);
		hCheck[8] = CreateWindow("button", "종료~~힛힝", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 500, 140, 30, hWnd, (HMENU)IDC_CHECK9, g_hinst, NULL);
	//	hList = CreateWindow("check", "테스트중", WS_CHILD | WS_VISIBLE | BS_BOX)
		break; 

	case WM_COMMAND: 
		switch (LOWORD(wParam)) {
		case IDC_CHECK1:
			if (Movecount[0] % 2 == 0) {
				MoveX = TRUE;
				Movecount[0]++;
			}
			else if (Movecount[0] % 2 == 1) {
				KillMoveX = TRUE;
				Movecount[0]++;
			}
			
			//ChildProc(NULL, NULL, InvalidateRect(NULL,NULL,FALSE), NULL);
			break;

		case IDC_CHECK2:
			if (Movecount[1] % 2 == 0) {
				MoveY = TRUE;
				Movecount[1]++;
			}
			else {
				KillMoveY = TRUE;
				Movecount[1]++;
			}
		//	ChildProc(NULL, NULL, InvalidateRect(NULL, NULL, FALSE), NULL);
			break;

		case IDC_CHECK3:
			JUMP1 = TRUE;
			break;

		case IDC_CHECK4:
			if (Movecount[2] % 2 == 0) {
				MoveX2 = TRUE;
				Movecount[2]++;
			}
			else {
				KillMoveX2 = TRUE;
				Movecount[2]++;
			}
		//	ChildProc2(NULL, NULL,InvalidateRect(NULL,NULL,FALSE), NULL);
			break;
			
		case IDC_CHECK5:
			if (Movecount[3] % 2 == 0) {
				MoveY2 = TRUE;
				Movecount[3]++;
			}
			else {
				KillMoveY2 = TRUE;
				Movecount[3]++;
			}
			break;

		case IDC_CHECK6:
			JUMP2 = TRUE;
			break;
		//	ChildProc2(NULL, NULL, InvalidateRect(NULL, NULL, FALSE), NULL);
			break;

		case IDC_CHECK7:
			KillMoveX = TRUE;
			KillMoveY = TRUE;
			KillMoveX2 = TRUE;
			KillMoveY2 = TRUE;
			break;

		case IDC_CHECK8:
			if (count % 2 == 0)
				CHANGE = TRUE;
			else
				CHANGE = FALSE;
			count++;
			break;
		case IDC_CHECK9:
			PostQuitMessage(0);
			break;
			
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		board1 = CreateWindowEx(WS_EX_WINDOWEDGE, "ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_THICKFRAME, 10, 10, 350, 350, hWnd, (HMENU)IDC_MAP1 , g_hinst, NULL);
		board2 = CreateWindowEx(WS_EX_WINDOWEDGE, "ChildClass2", NULL, WS_CHILD | WS_VISIBLE | WS_THICKFRAME, 370, 10, 350, 350, hWnd, (HMENU)IDC_MAP2 , g_hinst, NULL);


		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}     
	return DefWindowProc (hWnd, iMessage, wParam, lParam);
}



BOOL CollisionCheck(int x, int y, int mx, int my)
{
	if (mx > x&&mx<x + 150 && my>y&&my < y + 150)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
